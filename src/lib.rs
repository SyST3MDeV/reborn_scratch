use minhook::MinHook;

use std::{string::FromUtf8Error, char, fs::{File, remove_file}, io::{Write, Take, stdout, stdin}, path::Path, thread::sleep, time::Duration, ptr::{self, null}, mem, os::windows::ffi::EncodeWide};

use wchar::{wchar_t, wchz};

use widestring::ucstr;

use toy_arms::{internal::{self, module::Module, GameObject, cast}, derive::GameObject};

internal::create_entrypoint!(main_thread);

const GNAMES_OFFSET: usize = 0x3515230;
const GOBJECTS_OFFSET: usize = 0x35152D8;
const PROCESSEVENT_OFFSET: usize = 0x109ca0;
const STATICCONSTRUCTOBJECT_OFFSET: usize = 0x008c050;
const ENGINEPROCESSCOMMAND_OFFSET: usize = 0x01fca00;
const ERROR_FUNC_OFFSET: usize = 0x00646c0;

static mut orig_processevent_addr: usize = 0;
static mut orig_staticcreateobject_addr: usize = 0;
static mut orig_engine_exec_addr: usize = 0;
static mut orig_error_func_addr: usize = 0;

static mut engine_addr: usize = 0;
static mut foutputdevice: usize = 0;

static mut did_the_funny: bool = false;

static mut gobjects_global: Vec<UObject> = Vec::new();

static mut gnames_global: Option<*mut TArray> = None;

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

    if(Path::new("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\NamesDump.txt").exists()){
        remove_file("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\NamesDump.txt");
    }

    let mut File: File = File::create("C:\\Users\\gregorym\\Documents\\ReBorn\\ReBorn\\NamesDump.txt").unwrap();

    File.write_all(names_string.as_bytes());

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

    if(get_uobject_from_vec_by_address(uobject_address, &gobjects_global).is_some() && get_uobject_from_vec_by_address(uobject_address, &gobjects_global).is_some()){
        let uobject_name = &get_uobject_from_vec_by_address(uobject_address, &gobjects_global).unwrap().name;
        let ufunction_name = &get_uobject_from_vec_by_address(ufunction_address, &gobjects_global).unwrap().name;

        //println!("ProcessEvent hook called! UObject {}, UFunction {}, Params addr of {:x}", uobject_name, ufunction_name, params);
    }
    else{
        //println!("ProcessEvent hook called, but we failed to lookup the UObject/UFunction! UObject addr of {:x}, UFunction addr of {:x}, Params addr of {:x}", uobject_address, ufunction_address, params);
        
    }

    return process_event(uobject_address, ufunction_address, params);
    

    //println!("Calling the OG ProcessEvent...");

   

    
}

/*
                    longlong param_1, //UObject, the thing were instantiating
                    undefined8 param_2, //UObject, the class
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

unsafe fn fake_static_construct_object(param1: usize, param2: usize, param3: usize, param4: usize, param5: usize, param6: usize, param7: usize, param8: usize, param9: usize) -> usize{    
    type StaticConstructObject = unsafe extern "fastcall" fn(param1: usize, param2: usize, param3: usize, param4: usize, param5: usize, param6: usize, param7: usize, param8: usize, param9: usize) -> usize;

    let static_construct_object: StaticConstructObject = unsafe { std::mem::transmute(orig_staticcreateobject_addr)};

    let called_on_uobject: Option<&UObject> = get_uobject_from_vec_by_address(param1, &gobjects_global);
    let class_uobject: Option<&UObject> = get_uobject_from_vec_by_address(param2, &gobjects_global);
    //let outer_uobject: Option<&UObject> = get_uobject_from_vec_by_address(param3, &gobjects_global); //param3 is usually 0

    if(called_on_uobject.is_some() && class_uobject.is_some()){
        //println!("StaticConstructObject: Base UObject: {}, Class of {}", called_on_uobject.unwrap().name, class_uobject.unwrap().name);
    }
    else{
        //println!("StaticConstructObject with failure to resolve: base: {}, class: {}", called_on_uobject.is_some(), class_uobject.is_some());
    }

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

    //println!("Engine exec called: game engine addr of {:x}, wchar_t of {:x}, and f_output_device of {:x}", game_engine_address, command, f_output_device);
    engine_addr = game_engine_address;
    foutputdevice = f_output_device;

    return engine_call_command(game_engine_address, command, f_output_device);

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

unsafe fn fake_error_func(param_1: usize, param_2: usize) -> usize{
    type ErrorFunc = unsafe extern "cdecl" fn(param_1: usize, param_2: usize) -> usize;

    let error_func: ErrorFunc = std::mem::transmute(orig_error_func_addr);

    println!("Error func called; param_1: {:x}, param_2: {:x}", param_1, param_2);

    let deciding_value: i32 = ptr::read((param_1) as *const i32);

    if(deciding_value > 0){
        let return_val: usize = error_func(param_1, param_2);

        println!("Valid, returned {:x}", return_val);

        return return_val;
    }
    else{
        println!("INVALID, returned fake {:x}", param_2);

        return 0;
    }

    
}

#[derive(GameObject)]
struct ScuffedTArray{
    pointer: usize,
    num: u32,
    count: u32
}

#[derive(GameObject)]
struct ReturnToMenuParams{
    reason: usize
}

#[derive(GameObject)]
struct FString{
    data: &'static [wchar_t],
    count: u32,
    max: u32
}

struct NavToURLParams{
    URL: usize,
    error: usize,
    returnval: bool
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

/*
[1f7fa569178] [Engine.WorldInfo] PersistentLevel.TheWorld.MenuMap_P.WorldInfo

[1f7f87060e8] [Core.Function] WorldInfo.Engine.ServerTravel
[1f7f8706220] [Core.StrProperty] ServerTravel.WorldInfo.Engine.URL
[1f7f87062f8] [Core.BoolProperty] ServerTravel.WorldInfo.Engine.bAbsolute
[1f7f87063d8] [Core.BoolProperty] ServerTravel.WorldInfo.Engine.bShouldSkipGameNotify */

fn main_thread() {
    println!("ReBorn Injected!");
    println!("Waiting for module to become valid...");
    loop{
        if Module::from_name("Battleborn.exe").is_some(){
            break;
        }
    }
    println!("Module valid! Continuing...");
    let module: Module = Module::from_name("Battleborn.exe").unwrap();

    let module_base_address: usize = module.base_address;

    println!("Module base address: {:x}", module_base_address);

    unsafe{
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

        type ProcessEvent = unsafe extern "thiscall" fn(uobject: usize, ufunction: usize, params: usize);

        let process_event: ProcessEvent = unsafe { std::mem::transmute(module_base_address + PROCESSEVENT_OFFSET)};

        println!("Creating ProcessEvent hook...");

        orig_processevent_addr = MinHook::create_hook(process_event as _, fake_process_event as _).unwrap() as usize;

        println!("Creating StaticConstructObject reference...");

        type StaticConstructObject = unsafe extern "fastcall" fn(param1: usize, param2: usize, param3: usize, param4: usize, param5: usize, param6: usize, param7: usize, param8: usize, param9: usize);

        let static_construct_object: StaticConstructObject = unsafe{std::mem::transmute(module_base_address + STATICCONSTRUCTOBJECT_OFFSET)};

        println!("Creating StaticConstructObject hook...");

        orig_staticcreateobject_addr = MinHook::create_hook(static_construct_object as _, fake_static_construct_object as _).unwrap() as usize;

        println!("Creating EngineCallCommand reference...");

        type EngineCallCommand = unsafe extern "thiscall" fn(UGameEngine: usize, command: usize, foutputdevice: usize) -> i32;

        let engine_call_command: EngineCallCommand = unsafe{ std::mem::transmute(module_base_address + ENGINEPROCESSCOMMAND_OFFSET)};

        println!("Creating EngineCallCommand hook...");

        orig_engine_exec_addr = MinHook::create_hook(engine_call_command as _, fake_engine_exec as _).unwrap() as usize;

        //type ErrorFunc = unsafe extern "cdecl" fn(param_1: usize, param_2: usize) -> usize;

        //let error_func: ErrorFunc = std::mem::transmute(module_base_address + ERROR_FUNC_OFFSET);

        //orig_error_func_addr = MinHook::create_hook(error_func as _, fake_error_func as _).unwrap() as usize;

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

        let stdin = stdin();
        let mut stdout = stdout();

        loop{
            /*
            if(!did_the_funny && engine_addr != 0 && foutputdevice != 0){
                did_the_funny = true;
                let command_1 = wchz!("open Wishbone_P");
                engine_call_command(engine_addr, ptr::addr_of!(*command_1) as usize, foutputdevice);
            }
            */
            if(engine_addr != 0 && foutputdevice != 0){
                print!(">");

                let mut command: String = String::new();

                stdout.flush();

                stdin.read_line(&mut command);

                let command_1 = wchz!("open Wishbone_P");
                let command_2 = wchz!("open 10.0.0.85");

                if(command.contains("1")){
                    println!("Command 1");
                    engine_call_command(engine_addr, ptr::addr_of!(*command_1) as usize, foutputdevice);
                }
                else if(command.contains("2")){
                    println!("Command 2");
                    engine_call_command(engine_addr, ptr::addr_of!(*command_2) as usize, foutputdevice);
                }
                
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
        }
    }
}