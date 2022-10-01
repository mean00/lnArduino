#![allow(dead_code)]
use alloc::boxed::Box;

use crate::rnarduino as rn;
pub use rn::lnI2C as  lnI2C;
pub use rn::lnGpioMode as rnGpioMode;

type cint = cty::c_int;
type uint = cty::c_uint;

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
    pub fn new(instance : u32, speed: u32) -> rnI2C
    {
        unsafe {
            let t: rnI2C = rnI2C{
                ln : Box::new(lnI2C::new(instance as  cint ,  speed as cint)),  
            };
            t
        }
    }
    pub fn setSpeed(&mut self, speed: u32)
    {
        unsafe {
        self.ln.setSpeed(speed as cint);
        }
    }
    pub fn setAddress(&mut self, address: u8)
    {
        unsafe {
        self.ln.setAddress(address as cint);
        }
    }
    pub fn begin(&mut self, target : u8)            
    {
        unsafe {
            self.ln.begin(target as cint);
            }
    }
    
    pub fn write( &mut self, data : &[u8]  ) -> bool
    {
        unsafe {
                self.ln.write(data.len() as uint, data.as_ptr() as *const u8)
            }
    }
    pub fn writeTo( &mut self, target: u8, data : &[u8]  ) -> bool
    {
        unsafe {
                self.ln.write1(target as cint, data.len() as uint, data.as_ptr() as *const u8)
            }
    }
    pub fn read( &mut self,  data : &mut [u8]  ) -> bool
    {
        unsafe {
                self.ln.read( data.len() as uint, data.as_ptr() as *mut u8)
            }
    }
    pub fn readFrom( &mut self, target: u8, data : &mut [u8]  ) -> bool
    {
        unsafe {
                self.ln.read1( target as cint, data.len() as uint, data.as_ptr() as *mut u8)
            }
    }
    ///
    /// 
    /// 
    pub fn multiWrite(&mut self, tgt: u8,  lengths :  &[cty::c_uint], data :  &[*const u8]) -> bool
    {
        let nb=lengths.len();
        if  nb != data.len()
        {
            panic!("Invalid multiwrite : length & data mismatch\n");
        }
        if  nb == 0
        {
            panic!("I2C  Zero multiwrite \n");
        }
        
        let sequence_lengh : *const cty::c_uint ;
        sequence_lengh = lengths.as_ptr() as *const cty::c_uint;

        let sequence_data :  *mut *const u8;
        sequence_data = data.as_ptr() as *mut *const u8;
        unsafe {
            return self.ln.multiWrite(
                tgt as cty::c_int, 
                nb as cty::c_uint,
                sequence_lengh,
                sequence_data
                );
        }

    }
    //   bool multiWrite(int target, int nbSeqn,const int *seqLength, const uint8_t **data);

}
// eof