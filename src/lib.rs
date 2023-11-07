use minhook::MinHook;

use std::{string::FromUtf8Error, char, fs::{File, remove_file}, io::{Write, Take}, path::Path, thread::sleep, time::Duration, ptr, mem};

use wchar::{wchar_t, wchz};

use toy_arms::{internal::{self, module::Module, GameObject, cast}, derive::GameObject};

internal::create_entrypoint!(main_thread);

const GNAMES_OFFSET: usize = 0x3515230;
const GOBJECTS_OFFSET: usize = 0x35152D8;
const PROCESSEVENT_OFFSET: usize = 0x109ca0;

static mut orig_processevent_addr: usize = 0;

static mut gobjects_global: Vec<UObject> = Vec::new();

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

unsafe fn fake_process_event(uobject_address: usize, ufunction_address: usize, params: usize){
    type ProcessEvent = unsafe extern "fastcall" fn(uobject: usize, ufunction: usize, params: usize);

    let process_event: ProcessEvent = unsafe { std::mem::transmute(orig_processevent_addr)};

    if(get_uobject_from_vec_by_address(uobject_address, &gobjects_global).is_some() && get_uobject_from_vec_by_address(uobject_address, &gobjects_global).is_some()){
        let uobject_name = &get_uobject_from_vec_by_address(uobject_address, &gobjects_global).unwrap().name;
        let ufunction_name = &get_uobject_from_vec_by_address(ufunction_address, &gobjects_global).unwrap().name;

        println!("ProcessEvent hook called! UObject {}, UFunction {}, Params addr of {:x}", uobject_name, ufunction_name, params);
    }
    else{
        //println!("ProcessEvent hook called, but we failed to lookup the UObject/UFunction! UObject addr of {:x}, UFunction addr of {:x}, Params addr of {:x}", uobject_address, ufunction_address, params);
        
    }

    process_event(uobject_address, ufunction_address, params);
    

    //println!("Calling the OG ProcessEvent...");

   

    
}

#[derive(GameObject)]
struct ScuffedTArray{
    pointer: usize,
    num: u32,
    count: u32
}

#[derive(GameObject)]
struct ConsoleCommandParams{
    command: &'static [wchar_t]
}

#[derive(GameObject)]
struct ReturnToMenuParams{
    reason: usize
}

#[derive(GameObject)]
struct FString{
    data: &'static [wchar_t],
    count: u16,
    max: u16
}

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

        println!("Enabling ProcessEvent hook...");

        let _ = MinHook::enable_all_hooks().unwrap();

        let test_functions_uobject: &UObject = get_uobject_from_vec("PoplarPlayerController.PoplarGame.DebugReturnToMainMenu".to_owned(), Some("Core.Function".to_owned()), &gobjects_global).unwrap();

        let test_functions_target_uobject: &UObject = get_uobject_from_vec("PersistentLevel.TheWorld.MenuMap_P.PoplarPlayerController".to_owned(), Some("PoplarGame.PoplarPlayerController".to_owned()), &gobjects_global).unwrap();

        println!("Test function UObject address: {:x}", test_functions_uobject.address);

        //type ProcessEvent = unsafe extern "cdecl" fn(uobject: usize, ufunction: usize, params: usize);

        //let process_event_real: ProcessEvent = unsafe { std::mem::transmute(orig_processevent_addr)};

        println!("Waiting for a few secs to hit menu to call the test function...");

        sleep(Duration::from_secs(10));

        println!("Here it goes...");

        let reason = wchz!("Hello Battleborn!");

        let fstring: FString = FString { data: reason, count: 0, max: 0 };

        let params: ReturnToMenuParams = ReturnToMenuParams{ reason: ptr::addr_of!(fstring) as usize};

        //println!("{:x}", ptr::addr_of!(params) as usize);

        fake_process_event(test_functions_target_uobject.address, test_functions_uobject.address, ptr::addr_of!(params) as usize);

        //Parms -> TArray<wchar_t> whose pointer points to wchar_t

        loop{
            
        }
    }
}