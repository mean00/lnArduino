#![allow(dead_code)]

use crate::rn_timer_c as rt;
use cty::{c_void};
//use crate::rnarduino::{UBaseType_t,TaskHandle_t};
use alloc::vec::Vec;
use alloc::boxed::Box;
use crate::rn_freertos_c::{lnCreateTask,UBaseType_t};
//--
pub fn delay_ms(to : u32)
{
    unsafe {
        rt::lnDelay(to);
    }
}
pub fn delay_us(to : u32)
{
    unsafe {
        rt::lnDelayUs(to);
    }
}

pub fn  get_time_ms() -> u32
{
    unsafe {
        rt::lnGetMs()
    }
}

pub fn  get_time_us() -> u32
{
    unsafe {
         rt::lnGetUs()
    }
}

pub type rnTaskEntry = fn(param : * mut c_void);

struct TrampolineStruct
{
    pub entry : &'static rnTaskEntry,
    pub param : *mut c_void,

}

extern "C" fn trampoline( p : *mut cty::c_void)
{
    unsafe {
        let xparam = p as *mut TrampolineStruct;

        let function_ptr = *((*xparam).entry);
        function_ptr((*xparam).param);
    }
    // MEAMLEAK HERE!

}

pub fn rn_create_task(function_entry : &'static rnTaskEntry, name: &str, priority : usize, stackSize : u32, param : *mut c_void )
{
    let task_param : TrampolineStruct = TrampolineStruct
    {
        entry : function_entry,
        param,
    };    
    let param_box = Box::new(task_param);
    let name_as_vec :  Vec<char> = name.chars().collect();

    unsafe {
        lnCreateTask( 
            Some(trampoline), //Some(entry) as TaskFunction_t,
            name_as_vec.as_ptr() as *const i8,
            stackSize as i32, 
            Box::<TrampolineStruct>::into_raw(param_box) as *mut c_void ,
            priority as UBaseType_t
        );
    }
}
