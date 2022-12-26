#![allow(dead_code)]

use crate::rnarduino as rn;
pub use rn::lnPin as  rnPin;
pub use rn::lnGpioMode as rnGpioMode;
pub use rn::lnEdge as rnEdge;

///
///
///
pub fn attach_interrupt(
        pin: rn::lnPin,
        edge: rn::lnEdge,
        cb: rn::lnExtiCallback,
        cookie: *mut cty::c_void)
{
  unsafe {
        rn::lnExtiAttachInterrupt(pin,edge,cb,cookie);
    }
}
///
///
///
pub fn detach_interrupt(pin: rnPin)
{
    unsafe {
        rn::lnExtiDetachInterrupt(pin);
    }
}
///
///
///
pub fn enable_interrupt(pin: rnPin)
{
  unsafe {
        rn::lnExtiEnableInterrupt(pin);
    }
}
///
///
///
pub fn disable_interrupt(pin: rnPin)
{
  unsafe {
        rn::lnExtiDisableInterrupt(pin);
    }
}
// EOF

