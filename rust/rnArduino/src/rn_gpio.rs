#![allow(dead_code)]

#[cfg(feature = "rp2040")]
pub use rn_gpio_rp2040_c as gpio;
#[cfg(feature = "rp2040")]
pub use crate::rn_gpio_rp2040_c ;
#[cfg(not(feature = "rp2040"))]
pub use crate::rn_gpio_bp_c ;
#[cfg(not(feature = "rp2040"))]
pub use crate::rn_gpio_bp_c as gpio;
//
#[cfg(feature = "rp2040")]
pub use crate::rn_fast_gpio_rp2040 as rn_fast_gpio;
#[cfg(not(feature = "rp2040"))]
pub use crate::rn_fast_gpio_bp as rn_fast_gpio;

pub use crate::rn_exti;

//pub use crate::rn_gpio_bp_c ;
//pub use crate::rn_gpio_bp_c as gpio;

//
pub use gpio::lnPin as rnPin;
//
pub use rn_exti::rnEdge as  rnEdge;
pub use gpio::lnGpioMode as rnGpioMode;
pub use gpio::lnGetGpioToggleRegister;

pub fn digitalWrite( pin: rnPin, value: bool)
{
    unsafe {
            gpio::lnDigitalWrite(pin,value);
    }
}
pub fn digitalToggle( pin: rnPin)
{
    unsafe {
            gpio::lnDigitalToggle(pin);
    }
}
pub fn digitalRead( pin: rnPin) -> bool
{
    unsafe {
            gpio::lnDigitalRead(pin)
    }
}

pub fn pinMode( pin : rnPin, mode: rnGpioMode)
{
    unsafe {
            gpio::lnPinMode(pin,mode,0);
    }
}

pub fn pinModeSpeed( pin : rnPin, mode: rnGpioMode,speed: i32)
{
    unsafe {
            gpio::lnPinMode(pin,mode,speed);
    }
}

pub fn digital_write( pin: rnPin, value: bool)
{
    unsafe {
            gpio::lnDigitalWrite(pin,value);
    }
}
pub fn digital_toggle( pin: rnPin)
{
    unsafe {
            gpio::lnDigitalToggle(pin);
    }
}
pub fn digital_read( pin: rnPin) -> bool
{
    unsafe {
            gpio::lnDigitalRead(pin)
    }
}

pub fn pin_mode( pin : rnPin, mode: rnGpioMode)
{
    unsafe {
            gpio::lnPinMode(pin,mode,0);
    }
}

pub fn pin_mode_speed( pin : rnPin, mode: rnGpioMode, speed: i32)
{
    unsafe {
            gpio::lnPinMode(pin,mode,speed);
    }
}

//--

