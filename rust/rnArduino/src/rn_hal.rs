#![allow(unused_imports)]
#![allow(dead_code)]
extern crate alloc;
use alloc::boxed::Box;
use embedded_hal as hal;
use embedded_hal::i2c::{ErrorKind, ErrorType, SevenBitAddress, TenBitAddress, I2c, Operation};
use embedded_hal::i2c::ErrorKind::*;
//use embedded_hal::spi::{ErrorKind as SpiErrorKind, ErrorType as SpiErrorType};
//use embedded_hal::spi::ErrorKind::*;


use core::fmt::Error;

use core::convert::Infallible;
use hal::delay::DelayUs;
use hal::digital::*;
//---------------------------------
use crate::rnOsHelper::rnDelay;
use crate::rnGpio as gpio;
use crate::rnI2C::rnI2C;
//use crate::rnI2C::rnI2C as i2c;
use crate::rnarduino::lnPin;
//---------------------------------
use super::*;

struct rn_hal
{

}
///-------------------------------------------------
///
/// ** PINS ***
/// 
///-------------------------------------------------
struct Pin
{
    pin: lnPin,
}
///
/// 
/// 

impl embedded_hal::digital::ErrorType for Pin {
       type Error = Error;
}
impl Pin
{
    pub fn new( p : lnPin) -> Self
    {
        Pin {
            pin : p , 
        }
    }
}
///
/// 
/// 
impl InputPin for Pin
{       
    fn is_high(&self) ->   Result<bool, Self::Error>
    {
        Ok(gpio::digitalRead(self.pin))
    }
    fn is_low(&self) ->   Result<bool, Self::Error>
    {
        Ok(!gpio::digitalRead(self.pin))
    }
}
impl OutputPin for Pin
{       
    fn set_high(&mut self) ->   Result<(), Self::Error>
    {
        gpio::digitalWrite(self.pin,true);
        Ok(())
    }
    fn set_low(&mut self) ->   Result<(), Self::Error>
    {
        gpio::digitalWrite(self.pin,false);
        Ok(())
    }
}
///
/// 
/// 
impl ToggleableOutputPin for Pin
{       
    fn toggle(&mut self) -> Result<(), Self::Error>
    {
        gpio::digitalToggle(self.pin);
        Ok(())
    }
}
///
/// 
/// 
/// 
impl StatefulOutputPin for Pin
{
    /// Is the pin in drive high mode?
    ///
    /// *NOTE* this does *not* read the electrical state of the pin
    fn is_set_high(&self) -> Result<bool, Self::Error>
    {
        Ok(gpio::digitalRead(self.pin))
    }

    /// Is the pin in drive low mode?
    ///
    /// *NOTE* this does *not* read the electrical state of the pin
    fn is_set_low(&self) -> Result<bool, Self::Error>
    {
        Ok(!gpio::digitalRead(self.pin))
    }
}

///-------------------------------------------------
///
/// ** Delay ***
/// 
///-------------------------------------------------
impl DelayUs for rn_hal
{
    type Error = Infallible;

    fn delay_ms(&mut self, ms: u32) -> Result<(), Self::Error>
    {
        rnDelay(ms as u32);
        Ok(())
    }
    fn delay_us(&mut self, us: u32) -> Result<(), Self::Error>
    {
        rnDelay(us as u32);
        Ok(())
    }
}

///-------------------------------------------------
///
/// ** I2C ***
///
///-------------------------------------------------


struct I2C
{
    rni2c: Box<rnI2C>,
}

impl I2C
{
    fn new( p : u32, speed : u32 ) -> Self
    {
        I2C {
            rni2c : Box::new(rnI2C::new(p, speed)) , 
        }
    }
}
impl ErrorType for I2C { type Error = ErrorKind; }
impl I2c< SevenBitAddress > for I2C
{
    fn read(&mut self, address: SevenBitAddress, buffer: &mut [u8]) -> Result<(), Self::Error>
    {
        match self.rni2c.readFrom(address,buffer)
        {
            true => Ok(()),
            false => Err(Other),
        }
    }

    fn write(&mut self, address: SevenBitAddress, bytes: &[u8]) -> Result<(), Self::Error>
    {
        match self.rni2c.writeTo(address,bytes)
        {
            true => Ok(()),
            false => Err( Other ),
        }
    }
    fn write_iter<B: IntoIterator<Item = u8>>(&mut self, _address: u8, _bytes: B ) -> Result<(), Self::Error>
    {
        panic!();
        
    }

    fn write_read(
        &mut self,
        _address: SevenBitAddress,
        _bytes: &[u8],
        _buffer: &mut [u8],
    ) -> Result<(), Self::Error>
    {
        panic!();

    }
    
    /// Writes bytes to slave with address `address` and then reads enough bytes to fill `buffer` *in a
    /// single transaction*
    ///
    /// # I2C Events (contract)
    ///
    /// Same as the `write_read` method
    fn write_iter_read<B: IntoIterator<Item = u8>>(&mut self, _addr: u8, _bytes: B, _buffer: &mut [u8]) -> Result<(), Self::Error>
    {
        panic!();

    }

    fn transaction<'a>(
        &mut self,
        _address: SevenBitAddress,
        _operations: &mut [Operation<'a>],
    ) -> Result<(), Self::Error>
    {
        panic!();

    }
    fn transaction_iter<'a, O>(&mut self, _address: SevenBitAddress, _operations: O) -> Result<(), Self::Error>
    where
        O: IntoIterator<Item = Operation<'a>>
        {
            panic!();

        }
}
/*
*/
