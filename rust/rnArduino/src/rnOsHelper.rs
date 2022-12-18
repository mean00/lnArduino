#![allow(dead_code)]

use crate::rnarduino as rn;
use cty::c_char;
use cty::c_void;
use crate::rnarduino::UBaseType_t;
use crate::rnarduino::TaskHandle_t;
use crate::rnarduino::TaskFunction_t;
use alloc::vec::Vec;
use alloc::boxed::Box;


//--
pub fn rnDelay(to : u32) -> ()
{
    unsafe {
        rn::lnDelay(to);
    }
}
pub fn rnDelayUs(to : u32) -> ()
{
    unsafe {
        rn::lnDelayUs(to);
    }
}


pub fn  rnLogger( st : &str ) -> ()
{
    unsafe {
        rn::Logger_chars(st.len() as i32, st.as_ptr() as *const c_char);
        // n: cty::c_int, data: *const cty::c_char);
        //rn::Logger(st.as_ptr() as *const c_char);
    }
}


pub fn  rnGetTimeMs() -> u32
{
    unsafe {
        return rn::lnGetMs();
    }
}

pub fn  rnGetTimeUs() -> u32
{
    unsafe {
        return rn::lnGetUs();
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

pub fn rnCreateTask(function_entry : &'static rnTaskEntry, name: &str, priority : usize, taskSize : u32, param : *mut c_void )
{
    let task_param : TrampolineStruct = TrampolineStruct
    {
        entry : function_entry,
        param : param
    };    
    let param_box = Box::new(task_param);
    let name_as_vec :  Vec<char> = name.chars().collect();

    unsafe {
        rn::xTaskCreate( 
            Some(trampoline), //Some(entry) as TaskFunction_t,
            name_as_vec.as_ptr() as *const cty::c_char, 
            taskSize as u16, 
            Box::<TrampolineStruct>::into_raw(param_box) as *mut c_void ,
            priority as UBaseType_t, 
            0 as *mut TaskHandle_t
        );
    }
}
