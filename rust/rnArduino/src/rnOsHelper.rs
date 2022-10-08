#![allow(dead_code)]

use crate::rnarduino as rn;
use cty::c_char;

//--
pub fn rnDelay(to : u32) -> ()
{
    unsafe {
        rn::lnDelay(to);
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