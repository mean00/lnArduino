#![allow(dead_code)]

#[cfg(not(feature = "rp2040"))]
pub use crate::rn_gpio_bp_c;
#[cfg(not(feature = "rp2040"))]
pub use crate::rn_gpio_bp_c as gpio;
#[cfg(feature = "rp2040")]
pub use crate::rn_gpio_rp2040_c;
#[cfg(feature = "rp2040")]
pub use rn_gpio_rp2040_c as gpio;
//
#[cfg(not(feature = "rp2040"))]
pub use crate::rn_fast_gpio_bp as rn_fast_gpio;
#[cfg(feature = "rp2040")]
pub use crate::rn_fast_gpio_rp2040 as rn_fast_gpio;

pub use crate::rn_exti;

//pub use crate::rn_gpio_bp_c ;
//pub use crate::rn_gpio_bp_c as gpio;

//
pub use gpio::lnPin;
//
pub use gpio::lnGetGpioToggleRegister;
pub use gpio::lnGpioMode as rnGpioMode;
pub use rn_exti::rnEdge;
#[cfg(feature = "rp2040")]
#[repr(i32)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub enum lnPin {
    GPIO0 = 0,
    GPIO1 = 1,
    GPIO2 = 2,
    GPIO3 = 3,
    GPIO4 = 4,
    GPIO5 = 5,
    GPIO6 = 6,
    GPIO7 = 7,
    GPIO8 = 8,
    GPIO9 = 9,
    GPIO10 = 10,
    GPIO11 = 11,
    GPIO12 = 12,
    GPIO13 = 13,
    GPIO14 = 14,
    GPIO15 = 15,
    GPIO16 = 16,
    GPIO17 = 17,
    GPIO18 = 18,
    GPIO19 = 19,
    GPIO20 = 20,
    GPIO21 = 21,
    GPIO22 = 22,
    GPIO23 = 23,
    GPIO24 = 24,
    GPIO25 = 25,
    GPIO26 = 26,
    GPIO27 = 27,
    GPIO28 = 28,
    GPIO99 = 99,
    NoPin = -1,
}

#[cfg(not(feature = "rp2040"))]
#[repr(i32)]
#[derive(Debug, Copy, Clone, Hash, PartialEq, Eq)]
pub enum rnPin {
    PA0 = 0,
    PA1 = 1,
    PA2 = 2,
    PA3 = 3,
    PA4 = 4,
    PA5 = 5,
    PA6 = 6,
    PA7 = 7,
    PA8 = 8,
    PA9 = 9,
    PA10 = 10,
    PA11 = 11,
    PA12 = 12,
    PA13 = 13,
    PA14 = 14,
    PA15 = 15,
    PB0 = 16,
    PB1 = 17,
    PB2 = 18,
    PB3 = 19,
    PB4 = 20,
    PB5 = 21,
    PB6 = 22,
    PB7 = 23,
    PB8 = 24,
    PB9 = 25,
    PB10 = 26,
    PB11 = 27,
    PB12 = 28,
    PB13 = 29,
    PB14 = 30,
    PB15 = 31,
    PC0 = 32,
    PC1 = 33,
    PC2 = 34,
    PC3 = 35,
    PC4 = 36,
    PC5 = 37,
    PC6 = 38,
    PC7 = 39,
    PC8 = 40,
    PC9 = 41,
    PC10 = 42,
    PC11 = 43,
    PC12 = 44,
    PC13 = 45,
    PC14 = 46,
    PC15 = 47,
    PD0 = 48,
    PD1 = 49,
    PD2 = 50,
    PD3 = 51,
    PD4 = 52,
    PD5 = 53,
    PD6 = 54,
    PD7 = 55,
    PD8 = 56,
    PD9 = 57,
    PD10 = 58,
    PD11 = 59,
    PD12 = 60,
    PD13 = 61,
    PD14 = 62,
    PD15 = 63,
    PE0 = 64,
    PE1 = 65,
    PE2 = 66,
    PE3 = 67,
    PE4 = 68,
    PE5 = 69,
    PE6 = 70,
    PE7 = 71,
    PE8 = 72,
    PE9 = 73,
    PE10 = 74,
    PE11 = 75,
    PE12 = 76,
    PE13 = 77,
    PE14 = 78,
    PE15 = 79,
    NoPin = -1,
}
/*
 */
pub fn rnpin2lnpin(pin: rnPin) -> lnPin {
    let asi32: i32 = pin as i32;
    unsafe {
        let aslnpin: lnPin = core::mem::transmute(asi32);
        aslnpin
    }
}
/*
 */

pub fn digitalWrite(pin: rnPin, value: bool) {
    unsafe {
        gpio::lnDigitalWrite(rnpin2lnpin(pin), value);
    }
}
/*
 */
pub fn digitalToggle(pin: rnPin) {
    unsafe {
        gpio::lnDigitalToggle(rnpin2lnpin(pin));
    }
}
/*
 */
pub fn digitalRead(pin: rnPin) -> bool {
    unsafe { gpio::lnDigitalRead(rnpin2lnpin(pin)) }
}

pub fn pinMode(pin: rnPin, mode: rnGpioMode) {
    unsafe {
        gpio::lnPinMode(rnpin2lnpin(pin), mode, 0);
    }
}
/*
 */

pub fn pinModeSpeed(pin: rnPin, mode: rnGpioMode, speed: i32) {
    unsafe {
        gpio::lnPinMode(rnpin2lnpin(pin), mode, speed);
    }
}
/*
 */

pub fn digital_write(pin: rnPin, value: bool) {
    unsafe {
        gpio::lnDigitalWrite(rnpin2lnpin(pin), value);
    }
}
/*
 */
pub fn digital_toggle(pin: rnPin) {
    unsafe {
        gpio::lnDigitalToggle(rnpin2lnpin(pin));
    }
}
/*
 */
pub fn digital_read(pin: rnPin) -> bool {
    unsafe { gpio::lnDigitalRead(rnpin2lnpin(pin)) }
}

/*
 */
pub fn pin_mode(pin: rnPin, mode: rnGpioMode) {
    unsafe {
        gpio::lnPinMode(rnpin2lnpin(pin), mode, 0);
    }
}

/*
 */
pub fn pin_mode_speed(pin: rnPin, mode: rnGpioMode, speed: i32) {
    unsafe {
        gpio::lnPinMode(rnpin2lnpin(pin), mode, speed);
    }
}

//--
