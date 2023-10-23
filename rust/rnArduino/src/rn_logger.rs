#![allow(dead_code)]

use crate::rnarduino as rn;
use cty::{c_char};
use ufmt::uWrite;
use core::{convert::Infallible};


pub struct W;

impl uWrite for W {
    type Error = Infallible;

    fn write_str(&mut self, s: &str) -> Result<(), Infallible> {
        unsafe {
            rn::Logger_chars(s.len() as i32, s.as_ptr() as *const c_char);
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
