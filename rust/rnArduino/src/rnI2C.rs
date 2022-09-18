#![allow(dead_code)]
use alloc::boxed::Box;

use crate::rnarduino as rn;
pub use rn::lnI2C as  lnI2C;
pub use rn::lnGpioMode as rnGpioMode;

type cint = cty::c_int;

pub struct rnI2C
{
     ln : Box<lnI2C> ,
}
/**
 * 
 * 
 */
impl rnI2C 
{
    fn new(instance : u32, speed: u32) -> rnI2C
    {
        unsafe {
            let t: rnI2C = rnI2C{
                ln : Box::new(lnI2C::new(instance as  cint ,  speed as cint)),  
            };
            t
        }
    }
    fn setSpeed(&mut self, speed: u32)
    {
        unsafe {
        self.ln.setSpeed(speed as cint);
        }
    }
    fn setAddress(&mut self, address: u8)
    {
        unsafe {
        self.ln.setAddress(address as cint);
        }
    }
    fn begin(&mut self, target : u8)            
    {
        unsafe {
            self.ln.begin(target as cint);
            }
    }
    
    fn write( &mut self, data : &[u8]  ) -> bool
    {
        unsafe {
                self.ln.write(data.len() as cint, data.as_ptr() as *const u8)
            }
    }
    fn writeTo( &mut self, target: u8, data : &[u8]  ) -> bool
    {
        unsafe {
                self.ln.write1(target as cint, data.len() as cint, data.as_ptr() as *const u8)
            }
    }
    fn read( &mut self,  data : &mut [u8]  ) -> bool
    {
        unsafe {
                self.ln.read( data.len() as cint, data.as_ptr() as *mut u8)
            }
    }
    fn readFrom( &mut self, target: u8, data : &mut [u8]  ) -> bool
    {
        unsafe {
                self.ln.read1( target as cint, data.len() as cint, data.as_ptr() as *mut u8)
            }
    }
}
/*                   
                    bool multiWrite(int target, int nbSeqn,int *seqLength, uint8_t **data);                    
*/
