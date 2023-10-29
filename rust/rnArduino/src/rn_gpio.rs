#![allow(dead_code)]

use crate::rnarduino as rn;
pub use rn::lnPin as  rnPin;
pub use rn::lnGpioMode as rnGpioMode;
use crate::rnarduino::lnGetGpioToggleRegister;

pub struct rnFastIO
{
    onoff_adr       : *mut u32,
    on_bit          : u32,
    off_bit         : u32
}

impl rnFastIO
{
    pub fn new( pin : rnPin) -> Self
    {
        let bit : u32 = 1_u32 <<( (pin as u32)& 0x0f);
        pinMode(pin,rnGpioMode::lnOUTPUT);
        let r : rnFastIO ;
        unsafe {
            r= rnFastIO {
                onoff_adr   : lnGetGpioToggleRegister( ((pin as usize ) >> 4) as i32),
                on_bit      : bit,
                off_bit     : bit<<16,
            };
        }
        r
    }
    #[inline(always)]
    pub fn on(&mut self)
    {
        unsafe {
            *self.onoff_adr = self.on_bit;
        }
    }
    #[inline(always)]
    pub fn off(&mut self)
    {
        unsafe {
            *self.onoff_adr = self.off_bit;
        }

    }

}


pub fn digitalWrite( pin: rnPin, value: bool)
{
    unsafe {
            rn::lnDigitalWrite(pin,value);
    }
}
pub fn digitalToggle( pin: rnPin)
{
    unsafe {
            rn::lnDigitalToggle(pin);
    }
}
pub fn digitalRead( pin: rnPin) -> bool
{
    unsafe {
            rn::lnDigitalRead(pin)
    }
}

pub fn pinMode( pin : rnPin, mode: rnGpioMode)
{
    unsafe {
            rn::lnPinMode(pin,mode,0);
    }
}

pub fn pinModeSpeed( pin : rnPin, mode: rnGpioMode,speed: i32)
{
    unsafe {
            rn::lnPinMode(pin,mode,speed);
    }
}

pub fn digital_write( pin: rnPin, value: bool)
{
    unsafe {
            rn::lnDigitalWrite(pin,value);
    }
}
pub fn digital_toggle( pin: rnPin)
{
    unsafe {
            rn::lnDigitalToggle(pin);
    }
}
pub fn digital_read( pin: rnPin) -> bool
{
    unsafe {
            rn::lnDigitalRead(pin)
    }
}

pub fn pin_mode( pin : rnPin, mode: rnGpioMode)
{
    unsafe {
            rn::lnPinMode(pin,mode,0);
    }
}

pub fn pin_mode_speed( pin : rnPin, mode: rnGpioMode, speed: i32)
{
    unsafe {
            rn::lnPinMode(pin,mode,speed);
    }
}

//--

