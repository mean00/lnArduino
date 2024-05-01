#![allow(dead_code)]

use crate::rn_exti_c as exti;
use crate::rn_gpio as gpio;
pub use gpio::rnPin;
pub use exti::lnEdge as rnEdge;

//
fn rnPin2extiPin( _pin : gpio::rnPin) -> exti::lnPin {
    panic!();
}
fn rnEdge2extiEdge( _pin : gpio::rnEdge) -> exti::lnEdge {
    panic!( );
}
//
//
//
pub fn attach_interrupt(
        pin: gpio::rnPin,
        edge: gpio::rnEdge,
        cb: exti::lnExtiCallback,
        cookie: *mut cty::c_void)
{
  unsafe {
        exti::lnExtiAttachInterrupt( rnPin2extiPin(pin) ,rnEdge2extiEdge(edge),cb,cookie);
    }
}
//
//
//
pub fn detach_interrupt(pin: rnPin)
{
    unsafe {
        exti::lnExtiDetachInterrupt(rnPin2extiPin(pin));
    }
}
//
//
//
pub fn enable_interrupt(pin: rnPin)
{
  unsafe {
        exti::lnExtiEnableInterrupt(rnPin2extiPin(pin));
    }
}
//
//
//
pub fn disable_interrupt(pin: rnPin)
{
  unsafe {
        exti::lnExtiDisableInterrupt(rnPin2extiPin(pin));
    }
}
// EOF

