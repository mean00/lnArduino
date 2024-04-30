#![allow(dead_code)]
use alloc::boxed::Box;

use crate::rnarduino as rn;
use crate::rn_spi_c as rc;
use rc::*;
//use rc::{spiBitOrder,spiDataMode,lnSPISettings,ln_spi_c};
//use crate::rnPin;
pub use rn::lnPin as  rnPin;
pub use rc::ln_spi_c as  lnSPI;
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
     ln         :      * mut lnSPI ,
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
                ln : lnspi_create(instance as  cint ,  -1 as cint),  
                setting : rnSPISettings{
                    pinCS : rnPin::NoPin,
                    speed,
                    bOrder : rnSpiBitOrder::SPI_LSBFIRST,
                    dMode  : 0,
                }
            };
            t
        }
    }

    pub fn rn2ln(rn : &rnSPISettings) -> rc::lnSPISettings
    {
        rc::lnSPISettings{
            pinCS: rn.pinCS as cty::c_int,
            speed: rn.speed as cty::c_uint,
            bOrder: rn.bOrder as rc::spiBitOrder,
            dMode: rn.dMode as rc::spiDataMode,
        
        }

    }
    pub fn begin(&mut self, sz: usize)
    {
        unsafe {
            rc::lnspi_begin(self.ln, sz as cty::c_uint)
            }
    }
    pub fn end(&mut self)
    {
        unsafe {
            rc::lnspi_end(self.ln)
            }
    }
    pub fn set_speed(&mut self, speed: u32)
    {
        unsafe {
        rc::lnspi_set_speed(self.ln, speed as cty::c_uint);
        }
    }
    pub fn set_data_mode(&mut self, mode : u8)
    {
        unsafe {
        rc::lnspi_set_data_mode(self.ln, mode as u32);
        }
    }   



    pub fn write8( &mut self, data : u8  ) -> bool
    {
        unsafe {
                rc::lnspi_write8(self.ln,data)
            }
    }
    pub fn write16( &mut self,  data : u16  ) -> bool
    {
        unsafe {
                rc::lnspi_write16(self.ln,data)
        }
    }
}
// eof
