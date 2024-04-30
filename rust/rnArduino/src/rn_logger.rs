#![allow(dead_code)]

use crate::rn_freertos_c::Logger_chars;

use cty::c_schar;
use ufmt::uWrite;
use core::convert::Infallible;


pub struct W;

impl uWrite for W {
    type Error = Infallible;

    fn write_str(&mut self, s: &str) -> Result<(), Infallible> {
        unsafe {
            Logger_chars(s.len() as i32, s.as_ptr() as *const c_schar);
        }
        Ok(())
    }
}

#[macro_export]
macro_rules! lnLogger {    
    
    ($x:expr) => {
        uwrite!(&mut W, "{}", $x).unwrap()
    };

    ($x:expr, $($y:expr),+) => {
        uwrite!(&mut W, $x, $($y),+).unwrap()
    };
}

#[macro_export]
macro_rules! lnLogger_init {    
    () => {
            use ufmt::uwrite;
            use rnarduino::rn_logger::W;
    }    
}
