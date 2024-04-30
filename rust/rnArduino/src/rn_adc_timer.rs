#![allow(dead_code)]
use alloc::boxed::Box;
use crate::rnarduino as rn;
pub use rn::lnPin as  rnPin;
pub use rn::lnGpioMode as rnGpioMode;
pub use rn::lnEdge as rnEdge;

//use cty::c_int;

pub struct rnTimingAdc
{
     ln : Box<rn::lnTimingAdc> ,
}
//
// 
// 
// 
impl rnTimingAdc
{
    //
    // 
    // 
    // 
    pub fn new(instance : usize) -> Self
    {
        unsafe {
            let t: rnTimingAdc = rnTimingAdc{
                ln : Box::new(rn::lnTimingAdc::new(instance as i32)),
            };
            t
        }
    }
    //
    // 
    // 
    // 
    pub fn set_source(&mut self, timer : usize, channel: usize, frequency : usize,  pins : &[rnPin] ) -> bool
    {
        unsafe {
            self.ln.setSource(
                                    timer      as cty::c_int,    
                                    channel  as cty::c_int , 
                                    frequency  as cty::c_int ,    
                                    pins.len() as cty::c_int , 
                                    pins.as_ptr())        
        }
    }
    //
    // 
    //   
    pub  fn multi_read(&mut self, nbSamplePerChannel: cty::c_int, output: &mut [u16]) -> bool 
    {
        unsafe {
            self.ln.multiRead( nbSamplePerChannel, output.as_mut_ptr())
        }
    }
}
// EOF

