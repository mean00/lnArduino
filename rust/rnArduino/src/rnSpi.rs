#![allow(dead_code)]
use alloc::boxed::Box;

use crate::rnarduino as rn;
//use crate::rnPin;
pub use rn::lnPin as  rnPin;
pub use rn::hwlnSPIClass as  hwlnSPIClass;
pub use rn::lnGpioMode as rnGpioMode;

type cint = cty::c_int;
type uint = cty::c_uint;


#[derive(Copy,Clone)]
pub enum rnSpiBitOrder
{
    SPI_LSBFIRST=0,
    SPI_MSBFIRST=1
}

pub struct rnSPISettings 
{
    pub pinCS   : rnPin,        //CS pin associated to the configuration
    pub speed   : u32,          //specifies the spi bus maximum clock speed
    pub bOrder  : rnSpiBitOrder,  //bit order (MSBFirst or LSBFirst)
    pub dMode   : usize,   //one of the data mode
}
  

pub struct rnSPI
{
     ln             :      Box<hwlnSPIClass> ,
     setting        :      rnSPISettings,
}

/**
 * 
 * 
 */
impl rnSPI 
{
    pub fn new(instance : u32, speed: u32) -> Self
    {
        unsafe {
            let t: rnSPI = rnSPI{
                ln : Box::new(hwlnSPIClass::new(instance as  cint ,  -1 as cint)),  
                setting : rnSPISettings{
                    pinCS : rnPin::NoPin,
                    speed : speed,
                    bOrder : rnSpiBitOrder::SPI_LSBFIRST,
                    dMode  : 0,
                }
            };
            t
        }
    }

    pub fn rn2ln(rn : &rnSPISettings) -> rn::lnSPISettings
    {
        rn::lnSPISettings{
            pinCS: rn.pinCS as cty::c_int,
            speed: rn.speed as cty::c_uint,
            bOrder: rn.bOrder as rn::spiBitOrder,
            dMode: rn.dMode as rn::spiDataMode,
        
        }

    }
    pub fn begin(&mut self)
    {
        unsafe {
            self.ln.begin()
            }
    }
    pub fn set_speed(&mut self, speed: u32)
    {
        unsafe {
        self.ln.setSpeed(speed as cint);
        }
    }
    pub fn set_data_mode(&mut self, mode : u8)
    {
        unsafe {
        self.ln.setDataMode(mode as u32);
        }
    }   

    pub fn begin_session(&mut self,b : u8)
    {
        unsafe {
            self.ln.beginSession(b as cty::c_int);
            }        
    }
    pub fn end_session(&mut self)
    {
        unsafe {
            self.ln.endSession();
            }        
    }

    pub fn begin_transaction(&mut self, st : &rnSPISettings)
    {
        unsafe {
            self.ln.beginTransaction(&mut (Self::rn2ln(st)));
            }        
    }
    pub fn end_transaction(&mut self)
    {
        unsafe {
            self.ln.endTransaction();
            }        
    }


    pub fn write( &mut self, data : u8  ) -> bool
    {
        unsafe {
                self.ln.write(data as i32)
            }
    }
    pub fn write16( &mut self,  data : u16  ) -> bool
    {
        unsafe {
            self.ln.write16(data as i32)
        }
    }
    pub fn write16_repeat( &mut self, nb : usize, data : u16  ) -> bool
    {
        unsafe {
            self.ln.write16Repeat(nb as i32, data)
        }
    }
    pub fn write_data(&mut self, d: &[u8]) -> bool
    {
        unsafe {
            self.ln.write1(d.len() as i32, d.as_ptr(),false)
        }                  
    }
    pub fn write_data16(&mut self, d: &[u16]) -> bool
    {
        unsafe {
            self.ln.write161(d.len() as i32, d.as_ptr(),false)
        }
        
    }
    pub fn transfer(&mut self, tx : &mut [u8], rx:&mut [u8]) -> bool
    {
        if tx.len() != rx.len()
        {
            panic!("oops");
        }
        unsafe {
            self.ln.transfer(tx.len() as i32, tx.as_mut_ptr(), rx.as_mut_ptr())
        }
    }
}
// eof
