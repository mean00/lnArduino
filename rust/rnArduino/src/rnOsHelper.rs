#![allow(dead_code)]

use crate::rnarduino as rn;
use cty::c_char;

//--
pub fn rnDelay(to : i32) -> ()
{
    unsafe {
        rn::lnDelay(to);
    }
}

pub fn  rnLogger( st : &str ) -> ()
{
    unsafe {
        rn::Logger(st.as_ptr() as *const c_char);
    }
}