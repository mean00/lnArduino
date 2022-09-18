#![allow(dead_code)]

use crate::rnarduino as rn;

//--
pub fn rnDelay(to : i32) -> ()
{
    unsafe {
        rn::lnDelay(to);
    }
}
