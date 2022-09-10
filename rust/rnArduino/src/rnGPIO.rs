#![allow(dead_code)]

use crate::rnarduino as rn;
pub use rn::lnPin as  rnPin;
pub use rn::lnGpioMode as rnGpioMode;


pub fn rnDigitalWrite( pin: rnPin, value: bool) -> ()
{
    unsafe {
            rn::lnDigitalWrite(pin,value);
    }
}
pub fn rnDigitalToggle( pin: rnPin) -> ()
{
    unsafe {
            rn::lnDigitalToggle(pin);
    }
}
pub fn rnDigitalRead( pin: rnPin) -> bool
{
    unsafe {
            rn::lnDigitalRead(pin)
    }
}

pub fn rnPinMode( pin : rnPin, mode: rnGpioMode) -> ()
{
    unsafe {
            rn::lnPinMode(pin,mode);
    }
}
//--

