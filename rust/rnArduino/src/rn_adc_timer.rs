#![allow(dead_code)]
use crate::rn_gpio as gpio;
use crate::rnarduino as rn;
use alloc::boxed::Box;
pub use gpio::rnEdge;
pub use gpio::rnGpioMode;
pub use gpio::rnPin;

//use cty::c_int;

pub struct rnTimingAdc {
    ln: u32, //Box<rn::lnTimingAdc> ,
}
//
//
//
//
impl rnTimingAdc {
    //
    //
    //
    //
    pub fn new(instance: usize) -> Self {
        unsafe {
            let t: rnTimingAdc = rnTimingAdc {
                ln: 0, //Box::new(rn::lnTimingAdc::new(instance as i32)),
            };
            t
        }
    }
    //
    //
    //
    //
    pub fn set_source(
        &mut self,
        timer: usize,
        channel: usize,
        frequency: usize,
        pins: &[rnPin],
    ) -> bool {
        false
        /*
        unsafe {
            self.ln.setSource(
                                    timer      as cty::c_int,
                                    channel  as cty::c_int ,
                                    frequency  as cty::c_int ,
                                    pins.len() as cty::c_int ,
                                    pins.as_ptr())
        }
            */
    }
    //
    //
    //
    pub fn multi_read(&mut self, nbSamplePerChannel: cty::c_int, output: &mut [u16]) -> bool {
        false
        /*
        unsafe {
            self.ln.multiRead( nbSamplePerChannel, output.as_mut_ptr())
        }*/
    }
}
// EOF
