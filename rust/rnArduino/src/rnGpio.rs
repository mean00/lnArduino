#![allow(dead_code)]

use crate::rnarduino as rn;
pub use rn::lnPin as  rnPin;
pub use rn::lnGpioMode as rnGpioMode;


pub fn digitalWrite( pin: rnPin, value: bool) -> ()
{
    unsafe {
            rn::lnDigitalWrite(pin,value);
    }
}
pub fn digitalToggle( pin: rnPin) -> ()
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

pub fn pinMode( pin : rnPin, mode: rnGpioMode) -> ()
{
    unsafe {
            rn::lnPinMode(pin,mode);
    }
}
//--

