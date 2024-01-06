use minhook::MinHook;

use std::{string::FromUtf8Error, char, fs::{File, remove_file, OpenOptions}, io::{Write, Take, stdout, stdin, Read, BufRead}, path::Path, thread::{sleep, current}, time::Duration, ptr::{self, null}, mem, os::windows::ffi::EncodeWide, process, borrow::Borrow, fmt::format};

use wchar::{wchar_t, wchz};

use widestring::{ucstr, U16CStr, U16CString};

use toy_arms::{internal::{self, module::Module, GameObject, cast}, derive::GameObject, utils::{detect_keydown, keyboard::VirtualKeyCode}};

use process_list::for_each_module;

internal::create_entrypoint!(main_thread);

const GNAMES_OFFSET: usize = 0x3515230;
const GOBJECTS_OFFSET: usize = 0x35152D8;
const PROCESSEVENT_OFFSET: usize = 0x109ca0;
const STATICCONSTRUCTOBJECT_OFFSET: usize = 0x008c050;
const ENGINEPROCESSCOMMAND_OFFSET: usize = 0x01fca00;
const ERROR_FUNC_OFFSET: usize = 0x008f320;
const INIT_LISTEN_OFFSET: usize = 0x0c21e30;
const ADD_LINKER_OFFSET_1: usize = 0x001f0f0;
const ADD_LINKER_OFFSET_2: usize = 0x001ece0;
const COMPUTE_LINKER_OFFSET_1: usize = 0x001e8d0;
const COMPUTE_LINKER_OFFSET_2: usize = 0x001eb40;
const PRE_LOGIN_OFFSET: usize = 0x0752480;
const IMPORTANT_LISTEN_FUNC: usize = 0x136e520;
const NOTIFY_RECIEVED_TEXT: usize = 0x06100b0;
//0x18 linker:
//- 0x001ece0 = no change
//- 0x001f0f0 = no change

const engine_string_pattern: &str = "50 00 6F 00 70 00 6C 00 61 00 72 00 47 00 61 00 6D 00 65 00 2E 00 50 00 6F 00 70 00 6C 00 61 00 72 00 47 00 61 00 6D 00 65 00 43 00 6F 00 6E 00 73 00 74 00 61 00 6E 00 74 00 42 00 75 00 66 00 66 00 65 00 72";

static mut orig_processevent_addr: usize = 0;
static mut orig_staticcreateobject_addr: usize = 0;
static mut orig_engine_exec_addr: usize = 0;
static mut orig_error_func_addr: usize = 0;
static mut orig_pre_login_addr: usize = 0;
static mut orig_important_listen_func_addr: usize = 0;
static mut orig_notify_recieved_text_addr: usize = 0;

static mut engine_addr: usize = 0;
static mut foutputdevice: usize = 0;
static mut module_base_global: usize = 0;
static mut static_construct_object_param_9: usize = 0;

static mut did_the_funny: bool = false;

static mut gobjects_global: Vec<UObject> = Vec::new();

static mut gnames_global: Option<*mut TArray> = None;

static mut the_good_one: usize = 0;

#[derive(GameObject)]
struct TArray {
    pointer: *const usize
}

impl TArray {
    unsafe fn get(&self, idx: usize) -> usize {
        return *cast!(self.pointer as usize + (0x8 * idx), usize);
    }
}

macro_rules! function_from_address {
    ($address:expr, $t:ty) => {
        std::mem::transmute::<*const (), $t>($address as _)
    };
}

unsafe fn get_fname_from_gnames_at_idx(gnames: *mut TArray, idx: usize) -> Option<String>{
    if((*gnames).get(idx) == 0){
        return None;
    }

    let mut out_string:String = String::new();

    for i in 0..64{
        let byte: u8 = *cast!((*gnames).get(idx) + i + 0x18, u8);
        let the_char: char = char::from_u32(byte as u32).unwrap();
        if the_char == char::from_u32(0).unwrap() {
            break;
        }
        out_string.push(the_char);
    }

    return Some(out_string);
}

#[derive(Clone)]
struct UObject{
    address: usize,
    name_index: u32,
    name: String,
    class_name: Option<String>
}

unsafe fn get_outer_uobject_name(gnames: *mut TArray, uobject_address: usize) -> String{
    let mut str_to_return: String = String::new();

    let name_index: u32 = *cast!(uobject_address + 0x40, u32);

    str_to_return.push_str(&get_fname_from_gnames_at_idx(gnames, name_index as usize).unwrap());
    str_to_return.push_str(".");

    let outer_address: usize = *cast!(uobject_address + 0x38, usize);

    if(outer_address != 0){
        str_to_return.push_str(&get_outer_uobject_name(gnames, outer_address));
    }

    return str_to_return;
}

unsafe fn get_uobject_from_address(gnames: *mut TArray, uobject_address: usize, module_base: usize, dont_recurse: bool) -> Option<UObject>{
    if(uobject_address == 0){
        return None;
    }

    let name_index: u32 = *cast!(uobject_address + 0x40, u32);

    let mut class_name: Option<String> = None;

    if(!dont_recurse){
        class_name = Some(get_uobject_from_address(gnames, *cast!(uobject_address + 0x48, usize), module_base, true).unwrap().name);
    }

    let mut name: String = String::new();

    let outer_address: usize = *cast!(uobject_address + 0x38, usize);

    if(outer_address != 0){
        name.push_str(&get_outer_uobject_name(gnames, outer_address));
    }

    name.push_str(&get_fname_from_gnames_at_idx(gnames, name_index as usize).unwrap());

    return Some(UObject { address: uobject_address, name_index: name_index, name: name, class_name: class_name});
}

unsafe fn get_uobject_from_gobjobjects_at_idx(gnames: *mut TArray, idx: usize, gobjects: *mut TArray, module_base: usize) -> Option<UObject>{
    let uobject_address: usize = (*gobjects).get(idx);

    return get_uobject_from_address(gnames, uobject_address, module_base, false);
}

unsafe fn dump_names(gnames: *mut TArray, module: &Module){
    
    let mut names_string = String::new();

    let mut invalid_count: i32 = 0;
    let mut i = 0;

    loop{
        let maybe_name: Option<String> = get_fname_from_gnames_at_idx(gnames, i);

        if(maybe_name.is_some()){
            invalid_count = 0;
            names_string.push_str("[");
            names_string.push_str(&i.to_string());
            names_string.push_str("] ");
            names_string.push_str(&maybe_name.unwrap());
            names_string.push_str("\n");
        }
        else{
            invalid_count = invalid_count + 1;
            if(invalid_count > 10000){
                break;
            }
        }

        i = i + 1;
    }

    /*
    if(Path::new("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\NamesDump.txt").exists()){
        remove_file("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\NamesDump.txt");
    }

    let mut File: File = File::create("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\NamesDump.txt").unwrap();

    File.write_all(names_string.as_bytes());
    */

}

/*
[26b6be7c1a0] [Core.Function] PoplarPlayerController.PoplarGame.ServerSelectCharacter
[26b6be7c2d8] [Core.ObjectProperty] ServerSelectCharacter.PoplarPlayerController.PoplarGame.SelectedCharacter
[26b6be7c3b8] [Core.ObjectProperty] ServerSelectCharacter.PoplarPlayerController.PoplarGame.PreferredSkin
[26b6be7c498] [Core.ObjectProperty] ServerSelectCharacter.PoplarPlayerController.PoplarGame.PreferredTaunt */

unsafe fn fake_pre_login(param1: usize, param2: usize) -> i32{
    type PreLogin = unsafe extern "fastcall" fn(param1: usize, param2: usize) -> i32;

    let pre_login_func: PreLogin = unsafe { std::mem::transmute(orig_pre_login_addr)};

    println!("###################### HOUSTON WE HAVE PRELOGIN ###########################");

    return pre_login_func(param1, param2);
}

#[repr(C, packed)]
struct setFOVParams{
    fov: f32
}

#[repr(C, packed)]
struct SetSensitivityParams{
    X: f32,
    Y: f32
}

unsafe fn get_camera() -> Option<&'static UObject>{
    for uobject in &gobjects_global{
        if uobject.class_name.clone().unwrap() == "PoplarGame.PoplarCamera".to_string(){
            if uobject.name.contains("PersistentLevel.TheWorld."){
                println!("{} {}", uobject.class_name.clone().unwrap(), uobject.name);
                return Some(uobject);
            }
        }
    }

    return None;
}

unsafe fn get_input() -> Option<&'static UObject>{
    //[2a36c4a9850] [PoplarGame.PoplarPlayerInput] PoplarPlayerController.PersistentLevel.TheWorld.Slums_P.PoplarPlayerInput

    for uobject in &gobjects_global{
        if uobject.class_name.clone().unwrap() == "PoplarGame.PoplarPlayerInput".to_string(){
            if uobject.name.contains("PoplarPlayerController.PersistentLevel.TheWorld."){
                println!("{} {}", uobject.class_name.clone().unwrap(), uobject.name);
                return Some(uobject);
            }
        }
    }

    return None;
}

unsafe fn set_sensitivity(gnames: *mut TArray, module: &Module, gobjects: *mut TArray, x: f32, y: f32){
    gobjects_global = dump_objects(gnames, module, gobjects);

    let camera_uobject = get_input().unwrap().address;

    /*
    [2a36a0d3a18] [Core.Function] PlayerInput.Engine.SetSensitivity
[2a36a0d3b50] [Core.FloatProperty] SetSensitivity.PlayerInput.Engine.X
[2a36a0d3c28] [Core.FloatProperty] SetSensitivity.PlayerInput.Engine.Y*/

    let fov_ufunction: usize = get_uobject_from_vec("PlayerInput.Engine.SetSensitivity".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let params: SetSensitivityParams = SetSensitivityParams { X: x, Y: y };

    println!("Changing Sensitivity to X: {:?} Y: {:?} with addrs {:x} {:x} {:x}", x, y, camera_uobject, fov_ufunction, ptr::addr_of!(params) as usize);

    fake_process_event(camera_uobject, fov_ufunction, ptr::addr_of!(params) as usize);
}

unsafe fn set_fov(gnames: *mut TArray, module: &Module, gobjects: *mut TArray, fov: f32){
    gobjects_global = dump_objects(gnames, module, gobjects);

    let camera_uobject = get_player_controller_address(gnames, module, gobjects).unwrap();

    /*
    [287b503ca48] [Core.Function] PlayerController.Engine.SetFOV
[287b503cb80] [Core.FloatProperty] SetFOV.PlayerController.Engine.NewFOV */

    let fov_ufunction: usize = get_uobject_from_vec("PlayerController.Engine.FOV".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let params: setFOVParams = setFOVParams { fov: fov };

    println!("Changing FOV to {:?} with addrs {:x} {:x} {:x}", fov, camera_uobject, fov_ufunction, ptr::addr_of!(params) as usize);

    fake_process_event(camera_uobject, fov_ufunction, ptr::addr_of!(params) as usize);
}

#[derive(GameObject)]
struct SetShowSubtitlesParams{
    showSubtitles: bool
}

unsafe fn set_subtitle_state(gnames: *mut TArray, module: &Module, gobjects: *mut TArray, enabled: bool){
    gobjects_global = dump_objects(gnames, module, gobjects);

    let camera_uobject = get_player_controller_address(gnames, module, gobjects).unwrap();
    /*
    [287b503ca48] [Core.Function] PlayerController.Engine.SetFOV
[287b503cb80] [Core.FloatProperty] SetFOV.PlayerController.Engine.NewFOV */

    let fov_ufunction: usize = get_uobject_from_vec("PlayerController.Engine.SetShowSubtitles".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let params: SetShowSubtitlesParams = SetShowSubtitlesParams { showSubtitles: enabled };

    println!("Setting subtitles to {:?} with addrs {:x} {:x} {:x}", enabled, camera_uobject, fov_ufunction, ptr::addr_of!(params) as usize);

    fake_process_event(camera_uobject, fov_ufunction, ptr::addr_of!(params) as usize);
}

unsafe fn dump_objects(gnames: *mut TArray, module: &Module, gobjects: *mut TArray) -> Vec<UObject>{
    let mut uobjects: Vec<UObject> = Vec::new();

    let mut names_string = String::new();

    let mut invalid_count: i32 = 0;
    let mut i = 0;

    loop{
        let maybe_object: Option<UObject> = get_uobject_from_gobjobjects_at_idx(gnames, i, gobjects, module.base_address);

        if(maybe_object.is_some()){
            let object: UObject = maybe_object.unwrap();
            invalid_count = 0;

            names_string.push_str("[");
            names_string.push_str(&format!("{:x}", &object.address));
            names_string.push_str("] ");
            names_string.push_str("[");
            if(object.class_name.is_some()){
                names_string.push_str(&format!("{}", object.class_name.clone().unwrap()));
            }
            names_string.push_str("] ");
            names_string.push_str(&object.name);
            names_string.push_str("\n");

            uobjects.push(object);
        }
        else{
            invalid_count = invalid_count + 1;
            if(invalid_count > 100){
                break;
            }
        }

        i = i + 1;
    }

    if(Path::new("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\ObjectsDump.txt").exists()){
        remove_file("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\ObjectsDump.txt");
    }

    let mut File: File = File::create("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\ObjectsDump.txt").unwrap();

    File.write_all(names_string.as_bytes());

    return uobjects;
}

unsafe fn get_uobject_from_vec(name: String, class: Option<String>, vec: &Vec<UObject>) -> Option<&UObject>{
    for uobject in vec{
        if(class.is_some()){
            if(uobject.name == name){
                if(uobject.class_name.is_some()){
                    if(uobject.class_name.clone().unwrap() == class.clone().unwrap()){
                        return Some(uobject);
                    }
                }
            }
        }
        else{
            if(uobject.name == name){
                return Some(uobject);
            }
        }
    }

    return None;
}

fn get_uobject_from_vec_by_address(uobject_address: usize, vec: &Vec<UObject>) -> Option<&UObject>{
    for uobject in vec{
        if(uobject.address == uobject_address){
            return Some(&uobject);
        }
    }

    return None;
}

unsafe fn fake_process_event(uobject_address: usize, ufunction_address: usize, params: usize) -> usize{
    type ProcessEvent = unsafe extern "fastcall" fn(uobject: usize, ufunction: usize, params: usize) -> usize;

    let process_event: ProcessEvent = unsafe { std::mem::transmute(orig_processevent_addr)};

    /*
    if get_uobject_from_address(gnames_global.unwrap(), uobject_address, module_base_global, false).is_some() && get_uobject_from_address(gnames_global.unwrap(), ufunction_address, module_base_global, false).is_some() {
        
        let uobject = get_uobject_from_address(gnames_global.unwrap(), uobject_address, module_base_global, false).unwrap();//&get_uobject_from_vec_by_address(uobject_address, &gobjects_global).unwrap().name;
        let ufunction_name = &get_uobject_from_address(gnames_global.unwrap(), ufunction_address, module_base_global, false).unwrap().name;

        let mut debug_message = String::new();
        
        if(uobject.class_name.is_some()){
            debug_message = format!("UFunction {} called on [{}] {} with params addr of {}\n", ufunction_name, uobject.class_name.unwrap(), uobject.name, params);
        }
        else{
            debug_message = format!("UFunction {} called on {} with params addr of {}\n", ufunction_name, uobject.name, params);
        }

        let mut filtered = false;

        if debug_message.contains("InputKey") || debug_message.contains("InitializeChallenges") || debug_message.contains("HearSound")|| debug_message.contains("UpdateClientPerfData")|| debug_message.contains("StartFire")|| debug_message.contains("ParticleSystem")|| debug_message.contains("Timer")|| debug_message.contains("CheckIfSpottedByAI")|| debug_message.contains("CheckPriorityRefresh")|| debug_message.contains("CheckFullyBlocked")|| debug_message.contains("TryBlink")|| debug_message.contains("PointLightComponent"){
            filtered = true;
        }

        if(!filtered){
            println!("{}", debug_message);
        }

        /* 
        let mut file = OpenOptions::new().write(true).append(true).open("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\PE_Logs.txt").unwrap();

        if(!filtered){
            file.write(debug_message.as_bytes());
        }
        */
        //println!("ProcessEvent hook called! UObject [{}] {}, UFunction {}, Params addr of {:x}", uobject.class_name.unwrap(), uobject.name, ufunction_name, params);
    }
    else{
        println!("ProcessEvent hook called, but we failed to lookup the UObject/UFunction! UObject addr of {:x}, UFunction addr of {:x}, Params addr of {:x}", uobject_address, ufunction_address, params);
    }
    */

    return process_event(uobject_address, ufunction_address, params);
    

    //println!("Calling the OG ProcessEvent...");

   

    
}

/*
                    longlong param_1, //UObject, the thing were instantiating
                    undefined8 param_2, //UObject, the parent
                    undefined8 param_3, //aint never seen this be anything but 0
                    ulonglong param_4, //Also flags? Either 100000000 or 0
                   undefined8 param_5, //aint never seen this be anything but 0
                   undefined8 param_6, //Alawys the same, lets steal this shit
                   undefined8 param_7, //Flags probably, it's either 0 or FFFFFFFFFF
                   undefined *param_8, //aint never seen this be anything but 0
                   undefined4 param_9 //No fucking clue tbh
                   
                   UClass*			InClass,
	UObject*		InOuter,
	FName			InName,
	DWORD			InFlags,
	UObject*		InTemplate,
	FOutputDevice*	Error,
	UObject*		SuperObject*/

struct ConsoleCommandParams{
    command: usize
}

#[derive(GameObject)]
struct TcpListenParams{
    returnval: usize
}

unsafe fn fake_static_construct_object(param1: usize, param2: usize, param3: usize, param4: usize, param5: usize, param6: usize, param7: usize, param8: usize, param9: usize) -> usize{    
    type StaticConstructObject = unsafe extern "fastcall" fn(param1: usize, param2: usize, param3: usize, param4: usize, param5: usize, param6: usize, param7: usize, param8: usize, param9: usize) -> usize;

    let static_construct_object: StaticConstructObject = unsafe { std::mem::transmute(orig_staticcreateobject_addr)};

    /*
    let called_on_uobject: Option<&UObject> = get_uobject_from_vec_by_address(param1, &gobjects_global);
    let class_uobject: Option<&UObject> = get_uobject_from_vec_by_address(param2, &gobjects_global);
    //let outer_uobject: Option<&UObject> = get_uobject_from_vec_by_address(param3, &gobjects_global); //param3 is usually 0

    if(called_on_uobject.is_some() && class_uobject.is_some()){
        //println!("StaticConstructObject: Base UObject: {}, Class of {}", called_on_uobject.unwrap().name, class_uobject.unwrap().name);
    }
    else{
        //println!("StaticConstructObject with failure to resolve: base: {}, class: {}", called_on_uobject.is_some(), class_uobject.is_some());
    }
    */

    static_construct_object_param_9 = param9;

    //println!("param1: {:x}, param2: {:x}, param3: {:x}, param4: {:x}, param5: {:x}, param6: {:x}, param7: {:x}, param8: {:x}, param9: {:x}, ", param1, param2, param3, param4, param5, param6, param7, param8, param9);
        /*
        if(!did_the_funny){
        println!("Doing the funny...");

        did_the_funny = true;

        let uobject_to_instantiate: usize = get_uobject_from_vec("Engine.Console".to_owned(), Some("Core.Class".to_owned()), &gobjects_global).unwrap().address;
        let parent_uobject: usize = get_uobject_from_vec("PoplarGameEngine.Transient.PoplarGameViewportClient".to_owned(), Some("PoplarGame.PoplarGameViewportClient".to_owned()), &gobjects_global).unwrap().address;
        
        //let output: usize = static_construct_object(uobject_to_instantiate, parent_uobject, param3, param4, param5, param6, param7, param8, param9);

        let output: usize = static_construct_object(uobject_to_instantiate, parent_uobject, 0, 0, 0, parent_uobject, 0, 0, param9);

        println!("BEHOLD: THE FUNNY: {:x}", output);

        println!("{}", get_outer_uobject_name(gnames_global.unwrap(), output));

            println!("We bouta do the scuffed funny :)");

            for i in (0x1C0..0x1C8).step_by(0x8){ //1C0 is it!
                println!("Doing the scuffed funny at {:x}",i);

                let value = (parent_uobject + i) as *const usize;

                if(std::ptr::read::<usize>(value) == 0x0){
                    let write_value = (parent_uobject + i) as *mut usize;

                    std::ptr::write(write_value, output);
                }
            }
            //[PoplarGame.PoplarPlayerController] PersistentLevel.TheWorld.MenuMap_P.PoplarPlayerController

            /*
            let test_functions_uobject: &UObject = get_uobject_from_vec("Console.Engine.BuildRuntimeAutoCompleteList".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap();

        println!("Here it goes...");

        let string: &[u16; 12] = wchz!("map Slums_P");

        let command: FString = FString { data: string, count: (string.len() + 1) as u32, max: (string.len() + 1) as u32 };

        let params: ConsoleCommandParams = ConsoleCommandParams{ command: 1 };

            println!("{:x}", ptr::addr_of!(params) as usize);

        fake_process_event(output, test_functions_uobject.address, ptr::addr_of!(params) as usize);
        */

        }
        */

        

    //foutputdevice = param6;

    return static_construct_object(param1, param2, param3, param4, param5, param6, param7, param8, param9);
    
    //println!("Calling the OG ProcessEvent...");
}

unsafe fn fake_engine_exec(game_engine_address: usize, command: usize, f_output_device: usize) -> i32{
    type EngineCallCommand = unsafe extern "thiscall" fn(game_engine_address: usize, command: usize, f_output_device: usize) -> i32;

    let engine_call_command: EngineCallCommand = unsafe{ std::mem::transmute(orig_engine_exec_addr)};

    engine_addr = game_engine_address;
    foutputdevice = f_output_device;

    return engine_call_command(game_engine_address, command, f_output_device);

    //println!("Engine exec called: game engine addr of {:x}, wchar_t of {:x}, and f_output_device of {:x}", game_engine_address, command, f_output_device);
    //let game_engine_object = get_uobject_from_address(gnames_global.unwrap(), game_engine_address, module_base_global, false).unwrap();

    
    //minhook::MinHook::disable_all_hooks();

    /*
    if(!did_the_funny){
        did_the_funny = true;

        let mut new_command = wchz!("start Slums_P");

        return engine_call_command(game_engine_address, ptr::addr_of!(*new_command) as usize, f_output_device);

        //new_command = wchz!("open Cascade_P");

        //return engine_call_command(game_engine_address, ptr::addr_of!(*new_command) as usize, f_output_device);
    }
    else{
        
    }
    */
}

unsafe fn fake_error_func(param_1: usize, param_2: usize){
    type ErrorFunc = unsafe extern "cdecl" fn(param_1: usize, param_2: usize);

    let error_func: ErrorFunc = std::mem::transmute(orig_error_func_addr);

    println!("Error func called; param_1: {:x}, param_2: {:x}, param", param_1, param_2);

    if(the_good_one == 0){
        the_good_one = param_1;
    }

    if(param_1 == the_good_one){
        error_func(param_1, param_2);
    }
    else{
        println!("Ignoring call to error func!");
    }

    /*

    if(ptr::read::<usize>((param_1 + 0x40) as *const usize) > gobjects_global.len()){
        println!("Invalid GName on the UObject we were passed, ignoring...");
    }
    else{
        error_func(param_1, param_2);
    }
    */
    
    /*

    let uobject = get_uobject_from_address(gnames_global.unwrap(), param_1, module_base_global, false);


    if(uobject.is_some()){
        println!("{}", uobject.clone().unwrap().name);
        println!("{}", uobject.unwrap().class_name.unwrap());
        error_func(param_1, param_2);
    }
    else{
        println!("Failed to ID UObject, not calling error func!");
    }
    */
    

    
}

#[derive(GameObject)]
struct ScuffedTArray{
    pointer: usize,
    num: i32,
    count: i32
}

#[derive(GameObject)]
struct ReturnToMenuParams{
    reason: usize
}

struct NavToURLParams{
    URL: usize,
    error: usize,
    returnval: bool
}

struct TcpLinkListenParams{
    returnval: usize
}

/*
[1dbb895e328] [Core.Function] PlayerController.Engine.ClientTravel
[1dbb895e460] [Core.StrProperty] ClientTravel.PlayerController.Engine.URL
[1dbb895e538] [Core.ByteProperty] ClientTravel.PlayerController.Engine.TravelType
[1dbb895e618] [Core.BoolProperty] ClientTravel.PlayerController.Engine.bSeamless
[1dbb895e6f8] [Core.StructProperty] ClientTravel.PlayerController.Engine.MapPackageGuid */

struct ClientTravelParams{
    URL: usize,
    travelType: u8,
    bSeamless: u64,
    mapPackageGUID: usize
}

struct SetFrontendStateParams{
    state: u8
}

struct SendPlayerToURLParams{
    playerController: usize,
    URL: usize
}

struct SetClassParams{
    class: usize
}

struct ServerSelectCharacterParams{
    character: usize,
    skin: usize,
    taunt: usize
}

/*
[1f7fa569178] [Engine.WorldInfo] PersistentLevel.TheWorld.MenuMap_P.WorldInfo

[1f7f87060e8] [Core.Function] WorldInfo.Engine.ServerTravel
[1f7f8706220] [Core.StrProperty] ServerTravel.WorldInfo.Engine.URL
[1f7f87062f8] [Core.BoolProperty] ServerTravel.WorldInfo.Engine.bAbsolute
[1f7f87063d8] [Core.BoolProperty] ServerTravel.WorldInfo.Engine.bShouldSkipGameNotify */

unsafe fn get_player_controller_address(gnames: *mut TArray, module: &Module, gobjects: *mut TArray) -> Option<usize>{
    gobjects_global = dump_objects(gnames, &module, gobjects);

    for uobject in &gobjects_global{
        if(uobject.name.contains("PoplarPlayerController") && uobject.name.contains("PersistentLevel.TheWorld") && uobject.class_name == Some("PoplarGame.PoplarPlayerController".to_string())){
            println!("{}", uobject.name);
            return Some(uobject.address);
        }
    }

    return None;
}

#[derive(GameObject)]
struct CreateNamedNetDriverParams{
    name: usize
}

#[derive(GameObject)]
struct FString{
    body: usize,
    len: i32,
    max: i32
}

#[repr(C, packed)]
struct FStringBody{
    body: [usize]
}

#[derive(GameObject)]
struct FurryURL{
    protocol: FinalString,
    host: FinalString,
    port: i32,
    map: FinalString,
    notReallyAFString_options: FString,
    portal: FinalString
}

struct ReturnSingleByteVal{
    val: u8
}

#[derive(GameObject)]
struct FinalString{
    body: *const [u16]
}

unsafe fn listen_the_gbx_way(gnames: *mut TArray, module: &Module, gobjects: *mut TArray){
    //[2429a4cb418] [Core.Class] IpDrv.TcpLink

    let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

    let tcp_link_class: usize = get_uobject_from_vec("IpDrv.TcpLink".to_string(), Some("Core.Class".to_string()), &gobjects_global).unwrap().address;

    let tcp_link: usize = fake_static_construct_object(tcp_link_class, player_controller, 0, 0, 0, 0, 0, 0, static_construct_object_param_9);

    println!("TcpLink: {:x}", tcp_link);

    /*[2429afd43d8] [Core.Function] TcpLink.IpDrv.Listen
[2429afd4510] [Core.BoolProperty] Listen.TcpLink.IpDrv.ReturnValue */

    let listen_ufunction: usize = get_uobject_from_vec("TcpLink.IpDrv.Listen".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let params: TcpLinkListenParams = TcpLinkListenParams { returnval: 69 };

    fake_process_event(tcp_link, listen_ufunction, ptr::addr_of!(params) as usize);

    println!("Return Val: {}", params.returnval);
}

unsafe fn listen(gnames: *mut TArray, module: &Module, gobjects: *mut TArray, map_name: String){
    let game_engine_uobject = get_uobject_from_vec("Transient.PoplarGameEngine".to_string(), Some("PoplarGame.PoplarGameEngine".to_string()), &gobjects_global).unwrap().address;

    let create_named_netdriver = get_uobject_from_vec("GameEngine.Engine.CreateNamedNetDriver".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let params: CreateNamedNetDriverParams = CreateNamedNetDriverParams { name: module_base_global + GNAMES_OFFSET};

    println!("Calling CreateNamedNetDriver...");

    let net_driver_return = fake_process_event(game_engine_uobject, create_named_netdriver, ptr::addr_of!(params) as usize);

    gobjects_global = dump_objects(gnames, &module, gobjects);

    let net_driver = get_uobject_from_vec("Transient.TcpNetDriver".to_string(), Some("IpDrv.TcpNetDriver".to_string()), &gobjects_global).unwrap().address;

    let current_level = get_uobject_from_vec(format!("TheWorld.{}.PersistentLevel", map_name).to_string(), Some("Engine.Level".to_string()), &gobjects_global).unwrap().address;

    //[Engine.World] Wishbone_P.TheWorld

    let current_world: usize = get_uobject_from_vec(format!("{}.TheWorld", map_name).to_string(), Some("Engine.World".to_string()), &gobjects_global).unwrap().address; //[Engine.World] Wishbone_P.TheWorld

    println!("Current World: {:x}", current_world);

    //println!("  [*] Writing ptr at {:x}", 0xdc);
    //ptr::write::<usize>((current_level + 0xdc) as *mut usize, net_driver);

    for i in (0x50..0x100).step_by(0x8){
        if(ptr::read::<usize>((current_world + i) as *const usize) == 0x0){
            println!("  [*] Writing ptr at {:x}", i);
            ptr::write::<usize>((current_world + i) as *mut usize, net_driver);
        }
    }
    println!("  [*] Writing ptr at {:x}", 0x300);
    ptr::write::<usize>((current_world + 0x300) as *mut usize, net_driver);
    println!("  [*] Writing ptr at {:x}", 0xdc);
    ptr::write::<usize>((current_world + 0xdc) as *mut usize, net_driver);
    //println!("  [*] Writing ptr at {:x}", 0x640);
    //ptr::write::<usize>((current_level + 0x640) as *mut usize, net_driver);

    //let net_driver = get_uobject_from_vec("Transient.TcpNetDriver".to_string(), Some("IpDrv.TcpNetDriver".to_string()), &gobjects_global).unwrap().address;

    //let current_level = get_uobject_from_vec(format!("TheWorld.{}.PersistentLevel", map_name).to_string(), Some("Engine.Level".to_string()), &gobjects_global).unwrap().address;

    println!("NetDriver: {:x}", net_driver);

    type InitListenFunc = fn(tcp_net_driver: usize, network_notify: usize, url: usize, string: usize) -> u64;

    let init_listen_func: InitListenFunc = std::mem::transmute(module_base_global + INIT_LISTEN_OFFSET);

    let furred_url: FurryURL = FurryURL{protocol: FinalString{ body: &[0u16; 0] as *const [u16]}, host: FinalString{body: &[0u16; 0] as *const [u16]}, port: 69420, map: FinalString{body: &[0u16; 0] as *const [u16]}, notReallyAFString_options: FString{body: 0, len: 0, max: 0}, portal: FinalString { body: &[0u16; 0] as *const [u16] }};

    init_listen_func(net_driver, current_level, ptr::addr_of!(furred_url) as usize, 0);

    gobjects_global = dump_objects(gnames, module, gobjects);
    
    type AddLinkerFunc = fn(packageMap: usize, linker: usize) -> i32;

    type ComputePackageMapFunc = fn(packageMap: usize);

    let add_linker_func_1: AddLinkerFunc = std::mem::transmute(module_base_global + ADD_LINKER_OFFSET_1);
    let add_linker_func_2: AddLinkerFunc = std::mem::transmute(module_base_global + ADD_LINKER_OFFSET_2);

    let compute_package_map_func_1: ComputePackageMapFunc = std::mem::transmute(module_base_global + COMPUTE_LINKER_OFFSET_1);
    let compute_package_map_func_2: ComputePackageMapFunc = std::mem::transmute(module_base_global + COMPUTE_LINKER_OFFSET_2);

    let package_map: usize = get_uobject_from_vec("Transient.PackageMap".to_string(), Some("Core.PackageMap".to_string()), &gobjects_global).unwrap().address;

    let level: usize = get_uobject_from_vec(format!("TheWorld.{}.PersistentLevel", map_name).to_string(), Some("Engine.Level".to_string()), &gobjects_global).unwrap().address;

    //linker is at 0x8 or 0x18 in UObject, most likely 0x18 but will need to be checked

    //let packageMap = 0

    let linker: usize = std::ptr::read::<usize>((level + 0x18) as *const usize);

    println!("Adding linker to package map...");

    add_linker_func_1(package_map, linker);

    add_linker_func_2(package_map, linker);

    println!("Computing package map...");

    compute_package_map_func_1(package_map);

    compute_package_map_func_2(package_map);

    let world_info_addr: usize = get_uobject_from_vec(format!("PersistentLevel.TheWorld.{}.WorldInfo", map_name).to_string(), Some("Engine.WorldInfo".to_string()), &gobjects_global).unwrap().address;
    println!("AIGHT HERE WE GO.....");

    for i in (1216..1218){
        if(ptr::read::<u8>((world_info_addr + i) as *const u8) == 0x0){
            ptr::write::<u8>((world_info_addr + i) as *mut u8, 0x2);
        }
    }

    //[262bc40a730] [Engine.GameUISceneClient] PoplarUIInteraction.PoplarGameViewportClient.PoplarGameEngine.Transient.GameUISceneClient

    let game_ui_scene_client_addr: usize = get_uobject_from_vec("PoplarUIInteraction.PoplarGameViewportClient.PoplarGameEngine.Transient.GameUISceneClient".to_string(), Some("Engine.GameUISceneClient".to_string()), &gobjects_global).unwrap().address;

    //[Core.Function] GameUISceneClient.Engine.GetCurrentNetMode

    let net_mode_func: usize = get_uobject_from_vec("GameUISceneClient.Engine.GetCurrentNetMode".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let return_val: ReturnSingleByteVal = ReturnSingleByteVal{val: 254};

    fake_process_event(game_ui_scene_client_addr, net_mode_func, ptr::addr_of!(return_val) as usize);

    println!("Double checking our work: {}", return_val.val);
}

unsafe fn test_listen(gnames: *mut TArray, module: &Module, gobjects: *mut TArray, map_name: String){
    gobjects_global = dump_objects(gnames, module, gobjects);

    let world_info_addr: usize = get_uobject_from_vec("PersistentLevel.TheWorld.Wishbone_P.WorldInfo".to_string(), Some("Engine.WorldInfo".to_string()), &gobjects_global).unwrap().address;

    println!("AIGHT HERE WE GO.....");

    for i in (1216..1218){
        if(ptr::read::<u8>((world_info_addr + i) as *const u8) == 0x0){
            ptr::write::<u8>((world_info_addr + i) as *mut u8, 0x2);
        }
    }

    //[262bc40a730] [Engine.GameUISceneClient] PoplarUIInteraction.PoplarGameViewportClient.PoplarGameEngine.Transient.GameUISceneClient

    let game_ui_scene_client_addr: usize = get_uobject_from_vec("PoplarUIInteraction.PoplarGameViewportClient.PoplarGameEngine.Transient.GameUISceneClient".to_string(), Some("Engine.GameUISceneClient".to_string()), &gobjects_global).unwrap().address;

    //[Core.Function] GameUISceneClient.Engine.GetCurrentNetMode

    let net_mode_func: usize = get_uobject_from_vec("GameUISceneClient.Engine.GetCurrentNetMode".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let return_val: ReturnSingleByteVal = ReturnSingleByteVal{val: 254};

    fake_process_event(game_ui_scene_client_addr, net_mode_func, ptr::addr_of!(return_val) as usize);

    println!("Double checking our work: {}", return_val.val);
}

struct SetControllerSensitivityParams{
    sensitivity: i32
}

unsafe fn set_controller_sensitivity(gnames: *mut TArray, module: &Module, gobjects: *mut TArray, sensitivity: i32){
    /*
    [23e338991f8] [Core.Function] WillowPlayerInput.WillowGame.SetControllerSensitivity
[23e33899330] [Core.IntProperty] SetControllerSensitivity.WillowPlayerInput.WillowGame.Sensitivity
[23e33899408] [Core.Function] WillowPlayerInput.WillowGame.SetControllerSensitivityX
[23e33899540] [Core.IntProperty] SetControllerSensitivityX.WillowPlayerInput.WillowGame.Sensitivity
[23e33899618] [Core.Function] WillowPlayerInput.WillowGame.SetControllerSensitivityY
[23e33899750] [Core.IntProperty] SetControllerSensitivityY.WillowPlayerInput.WillowGame.Sensitivity */

    gobjects_global = dump_objects(gnames, module, gobjects);

    let poplar_input_uobject: usize = get_input().unwrap().address;

    let controller_sensitivity_function_1: usize = get_uobject_from_vec("WillowPlayerInput.WillowGame.SetControllerSensitivity".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let controller_sensitivity_function_2: usize = get_uobject_from_vec("WillowPlayerInput.WillowGame.SetControllerSensitivityX".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let controller_sensitivity_function_3: usize = get_uobject_from_vec("WillowPlayerInput.WillowGame.SetControllerSensitivityY".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let controller_sensitivity_params: SetControllerSensitivityParams = SetControllerSensitivityParams{sensitivity: sensitivity};

    fake_process_event(poplar_input_uobject, controller_sensitivity_function_1, ptr::addr_of!(controller_sensitivity_params) as usize);
    fake_process_event(poplar_input_uobject, controller_sensitivity_function_2, ptr::addr_of!(controller_sensitivity_params) as usize);
    fake_process_event(poplar_input_uobject, controller_sensitivity_function_3, ptr::addr_of!(controller_sensitivity_params) as usize);
}

/*
[1e22c4e5a88] [Core.Function] WillowPlayerController.WillowGame.InvertGamepadLook
[1e22c4e5bc0] [Core.BoolProperty] InvertGamepadLook.WillowPlayerController.WillowGame.InvertGamepad */

struct SetControllerInvertParams{
    invert: bool
}

unsafe fn invert_gamepad_look(gnames: *mut TArray, module: &Module, gobjects: *mut TArray, invert: bool){
    gobjects_global = dump_objects(gnames, module, gobjects);

    let poplar_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

    let gamepad_invert_look: usize = get_uobject_from_vec("WillowPlayerController.WillowGame.InvertGamepadLook".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

    let params: SetControllerInvertParams = SetControllerInvertParams { invert: invert };

    fake_process_event(poplar_controller, gamepad_invert_look, ptr::addr_of!(params) as usize);
}

#[derive(GameObject)]
struct PushItemParams{
    item: usize
}

#[derive(GameObject)]
struct ChooseLoadoutParams{
    loadoutIDX: i32
}

#[derive(GameObject)]
struct SetTestPerkFunctionParams{
    functionAssetKey: usize,
    rarity: i32
}

#[derive(GameObject)]
struct SetTestPerkFamilyParams{
    family: usize
}

                /*
                [22763e04fa8] [Core.Function] PoplarInventoryManager.PoplarGame.AddInventory
[22763e050e0] [Core.ObjectProperty] AddInventory.PoplarInventoryManager.PoplarGame.NewItem
[22763e051c0] [Core.BoolProperty] AddInventory.PoplarInventoryManager.PoplarGame.bReady
[22763e052a0] [Core.BoolProperty] AddInventory.PoplarInventoryManager.PoplarGame.bDoNotActivate
[22763e05380] [Core.BoolProperty] AddInventory.PoplarInventoryManager.PoplarGame.bPlayPickupSound
[22763e05460] [Core.BoolProperty] AddInventory.PoplarInventoryManager.PoplarGame.ReturnValue */

#[derive(GameObject)]
struct AddInventoryParams{
    newItem: usize,
    ready: bool,
    doNotActivate: bool,
    playPickupSound: bool
}

#[derive(GameObject)]
struct NativeMetaPerkSlotsUnlockedCheatParams{
    currLevel: i32,
    unlockLevel: i32
}

#[derive(GameObject)]
struct CommandletMainParams{
    params: FString,
    returnVal: i32
}

unsafe fn fake_important_listen_func(param1: usize, param2: i32, param3: usize) -> usize{
    type ImportantListenFunc = unsafe extern "thiscall" fn(param1: usize, param2: i32, param3: usize) -> usize;

    let important_listen_func: ImportantListenFunc = std::mem::transmute(orig_processevent_addr);

    let param1_uobject = get_uobject_from_address(gnames_global.unwrap(), param1, module_base_global, false).unwrap();
    
    //println!("Param1: [{}] {}", param1_uobject.class_name.unwrap(), param1_uobject.name);

    //println!("Param1: {:x}, param2: {}, param3: {:x}", param1, param2, param3);

    let returnval: usize = important_listen_func(param1, param2, param3);

    //println!("Retval: {:x}", returnval);

    return returnval;
}

unsafe fn fake_notify_recieved_text(param1: usize, param2: usize) -> usize{
    type NotifyRecievedText = unsafe extern "thiscall" fn(param1: usize, param2: usize) -> usize;

    let notify_recieved_text: NotifyRecievedText = unsafe { std::mem::transmute(orig_notify_recieved_text_addr)};

    let param1_uobject = get_uobject_from_address(gnames_global.unwrap(), param1, module_base_global, false).unwrap();
    
    println!("Param1: [{}] {}", param1_uobject.class_name.unwrap(), param1_uobject.name);
    
    let param2_uobject = get_uobject_from_address(gnames_global.unwrap(), param2, module_base_global, false).unwrap();
    
    println!("Param2: [{}] {}", param2_uobject.class_name.unwrap(), param2_uobject.name);

    println!("Param1: {:x}, param2: {:x}", param1, param2);

    let returnval: usize = notify_recieved_text(param1, param2);

    println!("Retval: {:x}", returnval);

    return returnval;
}

/*
[28f02013290] [PoplarGame.PoplarGameInfoConstruct] PersistentLevel.TheWorld.Wishbone_P.PoplarGameInfoConstruct
PoplarGameInfo.PoplarGame.SpawnBots 
[2389584c970] [Core.Function] PoplarGameInfo.PoplarGame.SpawnBot
[2389584caa8] [Core.ByteProperty] SpawnBot.PoplarGameInfo.PoplarGame.TeamIndex
[2389584cb88] [Core.StrProperty] SpawnBot.PoplarGameInfo.PoplarGame.ClassDefinitionName
[2389584cc60] [Core.BoolProperty] SpawnBot.PoplarGameInfo.PoplarGame.SpawnAtPlayerLookAt*/

struct SpawnBotParams{
    team_idx: u8,
    class_definition_name: FinalString,
    spawn_player_look_at: bool
}

unsafe fn spawn_bot(team_idx: u8, character_def: String, map_name: String, gobjects: *mut TArray, gnames: *mut TArray, module: &Module){
    let uobjects: Vec<UObject> = dump_objects(gnames, module, gobjects);

    //let game_info: &UObject = get_uobject_from_vec(format!("PersistentLevel.TheWorld.{}.PoplarGameInfoConstruct", map_name), Some("PoplarGame.PoplarGameInfoConstruct".to_string()), &uobjects).unwrap();
    //GameType.GD_Gametypes_Musketeers.GameType_Musketeers
    let game_info: &UObject = get_uobject_from_vec(format!("GameType.GD_Gametypes_Musketeers.GameType_Musketeers"), Some("PoplarGame.PoplarGameInfoDefinition".to_string()), &uobjects).unwrap();
    let spawn_bot_function: &UObject = get_uobject_from_vec("PoplarGameInfo.PoplarGame.SpawnBot".to_string(), Some("Core.Function".to_string()), &uobjects).unwrap();

    /*
    let command = U16CString::from_str(line).unwrap();

                let mut command_intermediate = command.as_slice().to_vec();
    
                command_intermediate.pop();
                command_intermediate.pop();
                command_intermediate.push(0);
    
                let mut command_formatted: &[u16] = &command_intermediate.as_slice();
    
                engine_call_command(gobjects_global[0].address + 0x25ebde8, ptr::addr_of!(*command_formatted) as *const () as usize, 0); */

    let command = U16CString::from_str(character_def).unwrap();

    let mut command_intermediate = command.as_slice().to_vec();
    
    command_intermediate.pop();
    command_intermediate.pop();
    command_intermediate.push(0);
    
    let mut command_formatted: &[u16] = &command_intermediate.as_slice();
    
    //engine_call_command(gobjects_global[0].address + 0x25ebde8, ptr::addr_of!(*command_formatted) as *const () as usize, 0);

    let params: SpawnBotParams = SpawnBotParams{team_idx: team_idx, class_definition_name: FinalString { body: ptr::addr_of!(*command_formatted) as *const [u16] }, spawn_player_look_at: false};

    println!("Spawning bots1...");

    fake_process_event(game_info.address, spawn_bot_function.address, ptr::addr_of!(params) as usize);

    println!("Spawning bots2...");

    let spawn_bot_function_1: &UObject = get_uobject_from_vec("PoplarGameInfo.PoplarGame.SpawnBots".to_string(), Some("Core.Function".to_string()), &uobjects).unwrap();

    //fake_process_event(game_info.address, spawn_bot_function_1.address, 0);


    println!("Spawning bots3...");
    let spawn_bot_function_2: &UObject = get_uobject_from_vec("PoplarGameInfo.PoplarGame.UpdateSpawnBots".to_string(), Some("Core.Function".to_string()), &uobjects).unwrap();

    //fake_process_event(game_info.address, spawn_bot_function_2.address, 0);

    //GameInfo.Engine.StartBots

    println!("{:x}", game_info.address);
}

fn main_thread() {
    println!("ReBorn Injected!");

    println!("Creating ProcessEvent reference...");

    println!("Waiting for module to become valid...");

    loop{
        if(Module::from_name("Battleborn.exe").is_some()){
            break;
        }
    }

    println!("Module valid! Continuing...");
    let mut module: Module = Module::from_name("Battleborn.exe").unwrap();

    let module_base_address: usize = module.base_address;

    println!("Module base address: {:x}", module_base_address);

    /*
    if(Path::new("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\PE_Logs.txt").exists()){
        remove_file("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\PE_Logs.txt");
        File::create(Path::new("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\PE_Logs.txt"));
    }
    else{
        File::create(Path::new("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\PE_Logs.txt"));
    }
    */

    //println!("Reading config from disk...");

    //let mut config_string: String = "".to_string();

    //File::open("config.txt").unwrap().read_to_string(&mut config_string);

    //println!("{}", config_string);
    /*
    let mut str = String::new();
    let mut laststr = String::new();

    loop{
        if Module::from_name("Battleborn.exe").is_some(){
            break;
        }
    }
    loop{
        /*
        str = String::new();
        for_each_module(process::id(), |(address, size), name| {
            str = [str.clone(), name.display().to_string().trim().to_string()].join("|");
        });

        if(str != laststr){
            laststr = str.clone();
            println!("{}", str);
        }
        */
        let mut do_break = false;
        for_each_module(process::id(), |(address, size), name| {
            if(name.display().to_string().contains("dxcore")){
                do_break = true;
            }
        });

        if(do_break){
            break;
        }
    }
    */

    unsafe{
        module_base_global = module_base_address;

        let gnames: *mut TArray = TArray::from_raw(module.read(GNAMES_OFFSET)).unwrap();
        let gobjects: *mut TArray = TArray::from_raw(module.read(GOBJECTS_OFFSET)).unwrap();

        println!("Dumping names...");

        dump_names(gnames, &module);

        gnames_global = Some(gnames);

        println!("Names dump complete!");

        /*
        println!("Waiting for a few secs to hit menu to dump objects...");

        sleep(Duration::from_secs(10));
        */

        println!("Dumping objects...");

        let uobjects: Vec<UObject> = dump_objects(gnames, &module, gobjects);

        gobjects_global = uobjects;

        println!("Objects dump complete!");

        println!("Creating ProcessEvent reference...");

        type ProcessEvent = unsafe extern "thiscall" fn(uobject: usize, ufunction: usize, params: usize) -> usize;

        let process_event: ProcessEvent = unsafe { std::mem::transmute(module_base_address + PROCESSEVENT_OFFSET)};

        println!("Creating ProcessEvent hook...");

        orig_processevent_addr = MinHook::create_hook(process_event as _, fake_process_event as _).unwrap() as usize;

        println!("Creating StaticConstructObject reference...");

        type StaticConstructObject = unsafe extern "fastcall" fn(param1: usize, param2: usize, param3: usize, param4: usize, param5: usize, param6: usize, param7: usize, param8: usize, param9: usize) -> usize;

        let static_construct_object: StaticConstructObject = unsafe{std::mem::transmute(module_base_address + STATICCONSTRUCTOBJECT_OFFSET)};

        println!("Creating StaticConstructObject hook...");

        orig_staticcreateobject_addr = MinHook::create_hook(static_construct_object as _, fake_static_construct_object as _).unwrap() as usize;

        println!("Creating EngineCallCommand reference...");

        type EngineCallCommand = unsafe extern "thiscall" fn(UGameEngine: usize, command: usize, foutputdevice: usize) -> i32;

        let engine_call_command: EngineCallCommand = unsafe{ std::mem::transmute(module_base_address + ENGINEPROCESSCOMMAND_OFFSET)};

        println!("Creating EngineCallCommand hook...");

        orig_engine_exec_addr = MinHook::create_hook(engine_call_command as _, fake_engine_exec as _).unwrap() as usize;

        //type ErrorFunc = unsafe extern "cdecl" fn(param_1: usize, param_2: usize, param_3: usize, param_4: usize);

        //let error_func: ErrorFunc = std::mem::transmute(module_base_address + ERROR_FUNC_OFFSET);

        //orig_error_func_addr = MinHook::create_hook(error_func as _, fake_error_func as _).unwrap() as usize;

        println!("Creating PreLogin reference...");

        type PreLogin = unsafe extern "fastcall" fn(param1: usize, param2: usize) -> i32;

        let pre_login_func: PreLogin = unsafe { std::mem::transmute(module_base_address + PRE_LOGIN_OFFSET)};

        orig_pre_login_addr = MinHook::create_hook(pre_login_func as _, fake_pre_login as _).unwrap() as usize;

        println!("Creating PreLogin reference...");

        type ImportantListenFunc = unsafe extern "thiscall" fn(param1: usize, param2: i32, param3: usize) -> usize;

        let important_listen_func: ImportantListenFunc = unsafe { std::mem::transmute(module_base_address + IMPORTANT_LISTEN_FUNC)};

        orig_important_listen_func_addr = MinHook::create_hook(important_listen_func as _, fake_important_listen_func as _).unwrap() as usize;
        
        type NotifyRecievedText = unsafe extern "thiscall" fn(param1: usize, param2: usize) -> usize;

        let notify_recieved_text: NotifyRecievedText = unsafe { std::mem::transmute(module_base_address + NOTIFY_RECIEVED_TEXT)};

        orig_notify_recieved_text_addr = MinHook::create_hook(notify_recieved_text as _, fake_notify_recieved_text as _).unwrap() as usize;

        //NOTIFY_RECIEVED_TEXT
        println!("Enabling all hooks...");

        let _ = MinHook::enable_all_hooks().unwrap();

        //let command: &[u16; 5] = wchz!("exit");

        //[PoplarGame.PoplarGameEngine] Transient.PoplarGameEngine

        /*

        let game_engine_address: usize = get_uobject_from_vec("Transient.PoplarGameEngine".to_owned(), Some("PoplarGame.PoplarGameEngine".to_owned()), &gobjects_global).unwrap().address;

        let function_object: usize = get_uobject_from_vec("Engine.Engine.BrowseToURL".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

        let URL = wchz!("Slums_P.umap");

        let URLFString: FString = FString { data: URL, count: (URL.len() + 1) as u32, max: (URL.len() + 1) as u32 };

        let error = wchz!("GRE GRO GRU GRA GRA GRA");

        let errorFString: FString = FString { data: error, count: (error.len() + 1) as u32, max: (error.len() + 1) as u32 };

        let params: NavToURLParams = NavToURLParams { URL: ptr::addr_of!(URLFString) as usize, error: ptr::addr_of!(errorFString) as usize, returnval: false };
        */

        //println!("ITS TIME!!!!");

        //fake_process_event(game_engine_address, function_object, ptr::addr_of!(params) as usize);

        /*
        [2911f8f3368] [Core.Function] Engine.Engine.BrowseToURL
[2911f8f34a0] [Core.StrProperty] BrowseToURL.Engine.Engine.BrowseURL
[2911f8f3578] [Core.StrProperty] BrowseToURL.Engine.Engine.Error
[2911f8f3650] [Core.BoolProperty] BrowseToURL.Engine.Engine.ReturnValue */

        /*
        [269cbdbbec0] [Core.Function] GameViewportClient.Engine.ConsoleCommand 
        [269cdbc84f8] [PoplarGame.PoplarGameViewportClient] PoplarGameEngine.Transient.PoplarGameViewportClient*/

        //Parms -> TArray<wchar_t> whose pointer points to wchar_t

        println!("Entering REPL mode...");

        let stdin = stdin();
        let mut stdout = stdout();

        loop{
            let mut line = String::new();

            stdin.lock().read_line(&mut line).unwrap();

            if(line.to_lowercase().find("listen1234").is_some()){
                println!("Attempting to bring listen server online...");
                listen(gnames, &module, gobjects, "Wishbone_P".to_string());
                /*
                if(static_construct_object_param_9 != 0){
                    gobjects_global = dump_objects(gnames, &module, gobjects);

                    let world_addr = get_uobject_from_vec("TheWorld.Wishbone_P.PersistentLevel".to_string(), Some("Engine.Level".to_string()), &gobjects_global).unwrap().address;

                    let poplar_tcp_link_class = get_uobject_from_vec("Engine.ServerCommandlet".to_string(), Some("Core.Class".to_string()), &gobjects_global).unwrap().address;

                    let out_tcp_link = fake_static_construct_object(poplar_tcp_link_class, world_addr, 0, 0, 0, 0, 0, 0, static_construct_object_param_9);
                    
                    /*
                    [1be33befc20] [Core.Function] Commandlet.Core.Main
[1be33befd58] [Core.StrProperty] Main.Commandlet.Core.Params
[1be33befe30] [Core.IntProperty] Main.Commandlet.Core.ReturnValue */

                    let main_func = get_uobject_from_vec("Commandlet.Core.Main".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                    let main_func_params: CommandletMainParams = CommandletMainParams{ params: FString{body: 0, len: 0, max: 0}, returnVal: 621 };

                    fake_process_event(out_tcp_link, main_func, ptr::addr_of!(main_func_params) as usize);

                    println!("We returned {} from servercommandlet!", main_func_params.returnVal);
                }
                */
            }
            else if(line.to_lowercase().find("swap1234").is_some()){
                println!("Swapping...");

                let player_controller: usize = get_player_controller_address(gnames, &module, gobjects).unwrap();

                let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;
                let class_to_switch_to: usize = get_uobject_from_vec("GD_RocketHawk.NameId_RocketHawk".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;
                let params: SetClassParams = SetClassParams { class: class_to_switch_to };
        
                process_event(player_controller, function_object, ptr::addr_of!(params) as usize);
            }
            else if(line.to_lowercase().find("spawn1234").is_some()){
                println!("Spawning...");

                //spawn_bot(1, "GD_WaterMonk.NameId_WaterMonk".to_string(), "Wishbone_P".to_string(), gobjects, gnames, &module);
                spawn_bot(0, "NameId_WaterMonk".to_string(), "Wishbone_P".to_string(), gobjects, gnames, &module);
                spawn_bot(0, "GD_WaterMonk".to_string(), "Wishbone_P".to_string(), gobjects, gnames, &module);
                spawn_bot(0, "WaterMonk".to_string(), "Wishbone_P".to_string(), gobjects, gnames, &module);
                spawn_bot(1, "NameId_WaterMonk".to_string(), "Wishbone_P".to_string(), gobjects, gnames, &module);
                spawn_bot(1, "GD_WaterMonk".to_string(), "Wishbone_P".to_string(), gobjects, gnames, &module);
                spawn_bot(1, "WaterMonk".to_string(), "Wishbone_P".to_string(), gobjects, gnames, &module);
            }
            else if(line.to_lowercase().find("dump1234").is_some()){
                println!("Dumping all UObjects...");

                gobjects_global = dump_objects(gnames, &module, gobjects);

                println!("Dump complete!");
            }
            //[Core.Function] WillowPlayerController.WillowGame.ShowDebugMenu
            //[221eff4e888] [Core.Function] PoplarUIInteraction.PoplarGame.RunDebugMenu
//[221eff4e9c0] [Core.ObjectProperty] RunDebugMenu.PoplarUIInteraction.PoplarGame.PC
            //[221f0cd95f0] [PoplarGame.PoplarUIInteraction] PoplarGameViewportClient.PoplarGameEngine.Transient.PoplarUIInteraction
            else{
                let origline = line.clone();

                println!("Passed command to engine...");
                let command = U16CString::from_str(line).unwrap();

                let mut command_intermediate = command.as_slice().to_vec();
    
                command_intermediate.pop();
                command_intermediate.pop();
                command_intermediate.push(0);
    
                let mut command_formatted: &[u16] = &command_intermediate.as_slice();
    
                engine_call_command(gobjects_global[0].address + 0x25ebde8, ptr::addr_of!(*command_formatted) as *const () as usize, 0);
            }

            stdout.flush();
            /*
            if(detect_keydown!(VirtualKeyCode::VK_I) && static_construct_object_param_9 != 0 && !did_the_funny){
                did_the_funny = true;

                listen(gnames, &module, gobjects, "Wishbone_P".to_string());
                
                
                /*
                [23a36ad8358] [Core.Function] PoplarPlayerController.PoplarGame.NativeMetaPerkSlotsUnlockedCheat
[23a36ad8490] [Core.IntProperty] NativeMetaPerkSlotsUnlockedCheat.PoplarPlayerController.PoplarGame.CurrLevel
[23a36ad8568] [Core.IntProperty] NativeMetaPerkSlotsUnlockedCheat.PoplarPlayerController.PoplarGame.NewLevel */

                /*
                let player_controller: usize = get_player_controller_address(gnames, &module, gobjects).unwrap();

                let perk_slots_unlocked_cheat: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.NativeMetaPerkSlotsUnlockedCheat".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                let params3: NativeMetaPerkSlotsUnlockedCheatParams = NativeMetaPerkSlotsUnlockedCheatParams { currLevel: 0, unlockLevel: 3 };

                fake_process_event(player_controller, perk_slots_unlocked_cheat, ptr::addr_of!(params3) as usize);

                let select_loadout_idx_func: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.ServerSelectPlayerMetaLoadoutIndex".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                let params: ChooseLoadoutParams = ChooseLoadoutParams { loadoutIDX: 1 };

                fake_process_event(player_controller, select_loadout_idx_func, ptr::addr_of!(params) as usize);

                let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.Loadout_PushItem".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                let item = wchz!("PF_Gear_AttackDamage_Legendary_ROG2");

                let params2: PushItemParams = PushItemParams{item: ptr::addr_of!(item) as usize};

                fake_process_event(player_controller, function_object, ptr::addr_of!(params2) as usize);
                */
                /*
                let player_controller: usize = get_player_controller_address(gnames, &module, gobjects).unwrap();

                let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                let class_to_switch_to: usize = get_uobject_from_vec("GD_WaterMonk.NameId_WaterMonk".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                dump_objects(gnames, &module, gobjects);
                */
                /*
                [213f63802c0] [Core.Function] PoplarPlayerController.PoplarGame.Loadout_Choose
[213f63803f8] [Core.IntProperty] Loadout_Choose.PoplarPlayerController.PoplarGame.Index */

//PoplarPlayerController.PoplarGame.Loadout_Save

                //let poplar_inventory_manager = get_uobject_from_vec("PoplarGame.Default__PoplarInventoryManager".to_string(), class, vec)
                /*
                [1d48ce84fa8] [Core.Function] PoplarInventoryManager.PoplarGame.AddInventory
[1d48ce850e0] [Core.ObjectProperty] AddInventory.PoplarInventoryManager.PoplarGame.NewItem
                [1c1d23bff78] [Core.Function] PoplarPlayerController.PoplarGame.Loadout_PushItem
[1c1d23c00b0] [Core.StrProperty] Loadout_PushItem.PoplarPlayerController.PoplarGame.ItemId 
[2835f7a1130] [Core.Function] Object.Core.ToHex
[2835f7a1268] [Core.IntProperty] ToHex.Object.Core.A
[2835f7a1340] [Core.StrProperty] ToHex.Object.Core.ReturnValue
[1d08ebc0f88] [Core.Function] PoplarCheatManager.PoplarGame.GrantEntitlement
[1d08ebc10c0] [Core.StrProperty] GrantEntitlement.PoplarCheatManager.PoplarGame.MetaID*/
/*
                let player_controller = get_player_controller_address(gnames, &module, gobjects).unwrap();

                let enable_cheats: usize = get_uobject_from_vec("PlayerController.Engine.EnableCheats".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                fake_process_event(player_controller, enable_cheats, 0);

                let cheat_manager_class = get_uobject_from_vec("PoplarGame.PoplarCheatManager".to_string(), Some("Core.Class".to_string()), &gobjects_global).unwrap().address;

                let cheat_manager: usize = fake_static_construct_object(cheat_manager_class, player_controller, 0, 0, 0, 0, 0, 0, static_construct_object_param_9);

                println!("Cheat manager addr {:x}", cheat_manager);

                dump_objects(gnames, &module, gobjects);

                /*
                [22b1ec50440] [Core.Function] CheatManager.Engine.InitCheatManager */
                //CheatManager.Engine.Fly

                //1C0 - 0x8 * 4 = cheatmanager
                //1C0 - 0x8 * 3 = cheatmanager class

                /*
                ReplicatedMetagameItem */

                ptr::write((player_controller + (0x1C0 - (0x8 * 4))) as *mut usize, cheat_manager); //please don't ask
                ptr::write((player_controller + (0x1C0 - (0x8 * 3))) as *mut usize, cheat_manager_class); 

                let init_cheat_manager = get_uobject_from_vec("CheatManager.Engine.InitCheatManager".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                fake_process_event(cheat_manager, init_cheat_manager, 0);

                for i in (0x0..0x400).step_by(0x8){
                    if(ptr::read::<usize>((cheat_manager + i) as *const usize) == 0){
                        println!("Doing the funny @ {:x}", i);
                        ptr::write::<usize>((cheat_manager + i) as *mut usize, player_controller);
                    }
                }

                let fly = get_uobject_from_vec("CheatManager.Engine.Fly".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                fake_process_event(cheat_manager, fly, 0);
*/
/*
[22e239e4930] [Core.Function] PoplarPlayerController.PoplarGame.SetTestPerkFamily
[22e239e4a68] [Core.StrProperty] SetTestPerkFamily.PoplarPlayerController.PoplarGame.FamilyName
[22e239e4b40] [Core.Function] PoplarPlayerController.PoplarGame.SetTestPerkFunction
[22e239e4c78] [Core.StrProperty] SetTestPerkFunction.PoplarPlayerController.PoplarGame.FunctionAssetKey
[22e239e4d50] [Core.IntProperty] SetTestPerkFunction.PoplarPlayerController.PoplarGame.Rarity
[22e239e4e28] [Core.Function] PoplarPlayerController.PoplarGame.ChangeTestPerkItemLevel
[22e239e4f60] [Core.IntProperty] ChangeTestPerkItemLevel.PoplarPlayerController.PoplarGame.DeltaItemLevel
[22e239e5038] [Core.Function] PoplarPlayerController.PoplarGame.ActivateTestPerk
[22e239e5170] [Core.IntProperty] ActivateTestPerk.PoplarPlayerController.PoplarGame.LoadoutSlot */

                //let player_controller = get_player_controller_address(gnames, &module, gobjects).unwrap();

                //let activate_test_perk: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.ServerActivatePerk".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                //let params: ChooseLoadoutParams = ChooseLoadoutParams{loadoutIDX: 1};

                //fake_process_event(player_controller, activate_test_perk, ptr::addr_of!(params) as usize);

                /*
                [22763e04fa8] [Core.Function] PoplarInventoryManager.PoplarGame.AddInventory
[22763e050e0] [Core.ObjectProperty] AddInventory.PoplarInventoryManager.PoplarGame.NewItem
[22763e051c0] [Core.BoolProperty] AddInventory.PoplarInventoryManager.PoplarGame.bReady
[22763e052a0] [Core.BoolProperty] AddInventory.PoplarInventoryManager.PoplarGame.bDoNotActivate
[22763e05380] [Core.BoolProperty] AddInventory.PoplarInventoryManager.PoplarGame.bPlayPickupSound
[22763e05460] [Core.BoolProperty] AddInventory.PoplarInventoryManager.PoplarGame.ReturnValue */

                //let poplar_inventory_manager = get_uobject_from_vec("PersistentLevel.TheWorld.Wishbone_P.PoplarInventoryManager".to_string(), Some("PoplarGame.PoplarInventoryManager".to_string()), &gobjects_global).unwrap().address;

                //[PoplarGame.PoplarUIInteraction] PoplarGameViewportClient.PoplarGameEngine.Transient.PoplarUIInteraction

                /*
                let poplar_ui_interaction: usize = get_uobject_from_vec("PoplarGameViewportClient.PoplarGameEngine.Transient.PoplarUIInteraction".to_string(), Some("PoplarGame.PoplarUIInteraction".to_string()), &gobjects_global).unwrap().address;

                let player_controller: usize = get_player_controller_address(gnames, &module, gobjects).unwrap();

                let params: SetTestPerkFamilyParams = SetTestPerkFamilyParams { family: player_controller };
                */
                /*
                [22763f7e888] [Core.Function] PoplarUIInteraction.PoplarGame.RunDebugMenu
[22763f7e9c0] [Core.ObjectProperty] RunDebugMenu.PoplarUIInteraction.PoplarGame.PC */

                //let function: usize = get_uobject_from_vec("PoplarUIInteraction.PoplarGame.RunDebugMenu".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                //fake_process_event(poplar_ui_interaction, function, ptr::addr_of!(params) as usize);

                //PoplarPlayerController.PoplarGame.Loadout_Choose

                //let set_test_perk_family: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SetTestPerkFamily".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                //let wchz_content1 = wchz!("ELD");

                //let fstring1: ScuffedTArray = ScuffedTArray{pointer: ptr::addr_of!(wchz_content1) as usize, num: wchz_content1.len() as i32, count: wchz_content1.len() as i32};

                //let params1: SetTestPerkFamilyParams = SetTestPerkFamilyParams { family: ptr::addr_of!(fstring1) as usize };

                //fake_process_event(player_controller, set_test_perk_family, ptr::addr_of!(params1) as usize);

                //let set_test_perk_function: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SetTestPerkFunction".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                //let wchz_content2 = wchz!("0x128e01cf");

                //let fstring2: ScuffedTArray = ScuffedTArray{pointer: ptr::addr_of!(wchz_content2) as usize, num: (wchz_content2.len()) as i32, count: (wchz_content2.len()) as i32};

                //let params2: SetTestPerkFunctionParams = SetTestPerkFunctionParams { functionAssetKey: ptr::addr_of!(fstring2) as usize, rarity: 1 };

                //fake_process_event(player_controller, set_test_perk_function, ptr::addr_of!(params2) as usize);

                //let activate_test_perk_function: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.ActivateTestPerk".to_string(), Some("Core.Function".to_string()), &gobjects_global).unwrap().address;

                //let params3: ChooseLoadoutParams =  ChooseLoadoutParams{ loadoutIDX: 0 };

                //fake_process_event(player_controller, activate_test_perk_function, ptr::addr_of!(params3) as usize);
                /*
                let oleaut32 = Module::from_name("oleaut32.dll").unwrap();
                
                let ptr: usize = *(oleaut32.read::<usize>(oleaut32.base_address + 0x4a84d)); //4A852

                println!("test: {:x}", ptr + 0x26A8);
                */
                //set_fov(gnames, &module, gobjects, 110_f32);
                //set_subtitle_state(gnames, &module, gobjects, true);

                //set_controller_sensitivity(gnames, &module, gobjects, 99);
                //invert_gamepad_look(gnames, &module, gobjects, true);
            }
            if(detect_keydown!(VirtualKeyCode::VK_O) && !did_the_funny){
                let command_1 = wchz!("open 127.0.0.1");
                engine_call_command(gobjects_global[0].address + 0x25ebde8, ptr::addr_of!(*command_1) as usize, 0);
            }
            if(detect_keydown!(VirtualKeyCode::VK_P) && !did_the_funny){
                let command_1 = wchz!("open Wishbone_P");
                engine_call_command(gobjects_global[0].address + 0x25ebde8, ptr::addr_of!(*command_1) as usize, 0);
            }
            */
            /*
            if(engine_addr != 0 && foutputdevice != 0 && detect_keydown!(VirtualKeyCode::VK_O)){
                let command_1 = wchz!("open 10.0.0.181 -game");
                engine_call_command(engine_addr, ptr::addr_of!(*command_1) as usize, foutputdevice);
            }
            */
            /*
            if(engine_addr != 0 && foutputdevice != 0){
                print!(">");

                let mut command: String = String::new();

                stdout.flush();

                stdin.read_line(&mut command);

                let command_1 = wchz!("open Wishbone_P?listen -game");
                let command_2 = wchz!("open 10.0.0.85 -game");

                if(command.contains("1")){
                    println!("Command 1");
                    engine_call_command(engine_addr, ptr::addr_of!(*command_1) as usize, foutputdevice);
                }
                else if(command.contains("2")){
                    println!("Command 2");
                    engine_call_command(engine_addr, ptr::addr_of!(*command_2) as usize, foutputdevice);
                }
            }
            */
            /*
            if(!did_the_funny && engine_addr != 0 && foutputdevice != 0){
                did_the_funny = true;
                match(config_string.split("|").into_iter().collect::<Vec<_>>()[0].to_string().parse::<u32>().unwrap()){
                    1 => {
                        let command = wchz!("open PvE_Prologue_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    2 => {
                        let command = wchz!("open Caverns_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    3 => {
                        let command = wchz!("open Portal_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    4 => {
                        let command = wchz!("open Captains_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    5 => {
                        let command = wchz!("open Evacuation_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    6 => {
                        let command = wchz!("open Ruins_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    7 => {
                        let command = wchz!("open Observatory_p");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    8 => {
                        let command = wchz!("open Refinery_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    9 => {
                        let command = wchz!("open Cathedral_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    10 => {
                        let command = wchz!("open Slums_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    11 => {
                        let command = wchz!("open Toby_Raid_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    12 => {
                        let command = wchz!("open CullingFacility_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    13 => {
                        let command = wchz!("open TallTales_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    14 => {
                        let command = wchz!("open Heart_Ekkunar_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                    _ => {
                        let command = wchz!("open Dojo_P");

                        engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
                    }
                }

                sleep(Duration::from_secs(10));
                
                match(config_string.split("|").into_iter().collect::<Vec<_>>()[1].to_string().parse::<u32>().unwrap()){
                    1 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_WaterMonk.NameId_WaterMonk".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    2 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_SunPriestess.NameId_SunPriestess_Poplar".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    3 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_SoulCollector.NameId_SoulCollector".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    4 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_PlagueBringer.NameId_PlagueBringer".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    5 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_RocketHawk.NameId_RocketHawk".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    6 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_DwarvenWarrior.NameId_DwarvenWarrior".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    7 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_AssaultJump.NameId_AssaultJump_Poplar".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    8 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_DarkAssassin.NameId_DarkAssassin".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    9 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_LeapingLuchador.NameId_LeapingLuchador".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    10 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_Bombirdier.NameId_Bombirdier".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    11 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_Blackguard.NameId_Blackguard".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    12 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_PapaShotgun.NameId_PapaShotgun".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    13 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_SpiritMech.NameId_SpiritMech".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    14 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_IceGolem.NameId_IceGolem".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    15 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_Sidekick.NameId_SideKick".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    16 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_TacticalBuilder.NameId_TacticalBuilder".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    17 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("gd_gentsniper.NameId_GentSniper".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    18 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_MutantFist.NameId_MutantFist".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    19 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("gd_tribalhealer.NameId_TribalHealer".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    20 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("gd_machinegunner.NameId_MachineGunner".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    21 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_ChaosMage.NameId_ChaosMage".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    22 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_ModernSoldier.NameId_ModernSoldier".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    23 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_CornerSneaker.NameId_CornerSneaker".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    24 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_MageBlade.NameId_MageBlade_Poplar".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    25 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("gd_deathblade.NameId_DeathBlade".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    26 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_RogueCommander.NameId_RogueCommander".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    27 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_BoyAndDjinn.NameId_BoyAndDjinn".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    28 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("gd_darkelfranger.NameId_DarkElfRanger".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    29 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_PenguinMech.NameId_PenguinMech".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    30 => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_RogueSoldier.NameId_RogueSoldier".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                    _ => {
                        let player_controller: usize = get_player_controller_address(gnames, module, gobjects).unwrap();

                        let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                        let class_to_switch_to: usize = get_uobject_from_vec("GD_WaterMonk.NameId_WaterMonk".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                        let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                        process_event(player_controller, function_object, ptr::addr_of!(params) as usize);

                        break;
                    }
                }
            }
            */
            /*
            if(!did_the_funny && engine_addr != 0 && foutputdevice != 0){
                did_the_funny = true;
                let command_1 = wchz!("open Wishbone_P");
                engine_call_command(engine_addr, ptr::addr_of!(*command_1) as usize, foutputdevice);
            }
            */
            /*
            if detect_keydown!(VirtualKeyCode::VK_P) {
                gobjects_global = dump_objects(gnames, &module, gobjects);

                /*
                [1d25f84c1a0] [Core.Function] PoplarPlayerController.PoplarGame.ServerSelectCharacter
[1d25f84c2d8] [Core.ObjectProperty] ServerSelectCharacter.PoplarPlayerController.PoplarGame.SelectedCharacter
[1d25f84c3b8] [Core.ObjectProperty] ServerSelectCharacter.PoplarPlayerController.PoplarGame.PreferredSkin
[1d25f84c498] [Core.ObjectProperty] ServerSelectCharacter.PoplarPlayerController.PoplarGame.PreferredTaunt */
/*
                let player_controller: usize = get_uobject_from_vec("PersistentLevel.TheWorld.Cascade_P.PoplarPlayerController".to_owned(), Some("PoplarGame.PoplarPlayerController".to_owned()), &gobjects_global).unwrap().address;

                let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.ServerSelectCharacter".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;
                
                let class_uobject: usize = get_uobject_from_vec("GD_MutantFist.ClassId_MutantFist".to_owned(), Some("PoplarGame.PoplarPlayerClassIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;
                //[1868186f9e0] [PoplarGame.PoplarPlayerNameIdentifierDefinition] GD_MutantFist.NameId_MutantFists

                //[2d5671def80] [PoplarGame.PoplarMetaSkinDefinition] Skins.GD_MutantFist.SkinId_Color005
                let skin_uobject: usize = get_uobject_from_vec("Skins.GD_MutantFist.SkinId_Color005".to_owned(), Some("PoplarGame.PoplarMetaSkinDefinition".to_owned()), &gobjects_global).unwrap().address;

                //[2d5671df5e0] [PoplarGame.PoplarMetaTauntDefinition] Taunts.GD_MutantFist.Taunt_MutantFist_Taunt_002
                let taunt_uobject: usize = get_uobject_from_vec("Taunts.GD_MutantFist.Taunt_MutantFist_Taunt_002".to_owned(), Some("PoplarGame.PoplarMetaTauntDefinition".to_owned()), &gobjects_global).unwrap().address;

                let params: ServerSelectCharacterParams = ServerSelectCharacterParams { character: class_uobject, skin: skin_uobject, taunt: taunt_uobject};
                */

                let player_controller: usize = get_uobject_from_vec("PersistentLevel.TheWorld.Wishbone_P.PoplarPlayerController".to_owned(), Some("PoplarGame.PoplarPlayerController".to_owned()), &gobjects_global).unwrap().address;

                let function_object: usize = get_uobject_from_vec("PoplarPlayerController.PoplarGame.SwitchPoplarPlayerClass".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap().address;

                let class_to_switch_to: usize = get_uobject_from_vec("GD_DarkAssassin.NameId_DarkAssassin".to_owned(), Some("PoplarGame.PoplarPlayerNameIdentifierDefinition".to_owned()), &gobjects_global).unwrap().address;

                let params: SetClassParams = SetClassParams { class: class_to_switch_to };
                
                process_event(player_controller, function_object, ptr::addr_of!(params) as usize);
            }
            if(engine_addr != 0 && foutputdevice != 0 && detect_keydown!(VirtualKeyCode::VK_O)){
                did_the_funny = true;
                /*
                print!(">");

                let mut command: String = String::new();

                stdout.flush();

                stdin.read_line(&mut command);

                let command_bytes: Vec<u8> = command.into_bytes();

                let wide_command_bytes: Vec<u16> = command_bytes.iter().map(|byte: &u8| *byte as u16).collect();

                let wide_command_slice: &[u16] = wide_command_bytes.as_slice();

                engine_call_command(engine_addr, command_address.into(), foutputdevice);



                let command_1 = wchz!("open IncTut_Freeze_P");
                let command_2 = wchz!("open 10.0.0.85");

                if(command.contains("1")){
                    println!("Command 1");
                    engine_call_command(engine_addr, ptr::addr_of!(*command_1) as usize, foutputdevice);
                }
                else if(command.contains("2")){
                    println!("Command 2");
                    engine_call_command(engine_addr, ptr::addr_of!(*command_2) as usize, foutputdevice);
                }

                [1e74ba843d8] [Core.Function] TcpLink.IpDrv.Listen
[1e74ba84510] [Core.BoolProperty] Listen.TcpLink.IpDrv.ReturnValue

PoplarGame.Default__PoplarTcpLink
PoplarGame.PoplarTcpLink
                                */

                                

                let command = wchz!("open Wishbone_P?listen -game");
                engine_call_command(engine_addr, ptr::addr_of!(*command) as usize, foutputdevice);
            }
            /*
            if(!did_the_funny && foutputdevice != 0){
                did_the_funny = true;
                println!("DOING THE THING!!!!!");
                std::ptr::write((game_engine_address + 0x630) as *mut usize, 0);
                println!("{:x}", ptr::addr_of!(command) as usize);
                engine_call_command(game_engine_address, ptr::addr_of!(command) as usize, foutputdevice);
            }
            */
            */
        }
    }
}