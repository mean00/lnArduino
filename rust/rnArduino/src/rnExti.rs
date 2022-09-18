#![allow(dead_code)]

use crate::rnarduino as rn;
pub use rn::lnPin as  rnPin;
pub use rn::lnGpioMode as rnGpioMode;
pub use rn::lnEdge as rnEdge;

///
///
///
pub fn attachInterrupt(
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
pub fn detachInterrupt(pin: rnPin) -> ()
{
    unsafe {
        rn::lnExtiDetachInterrupt(pin);
    }
}
///
///
///
pub fn enableInterrupt(pin: rnPin) -> ()
{
  unsafe {
        rn::lnExtiEnableInterrupt(pin);
    }
}
///
///
///
pub fn disableInterrupt(pin: rnPin) -> ()
{
  unsafe {
        rn::lnExtiDisableInterrupt(pin);
    }
}
// EOF

