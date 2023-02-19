#![no_std]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![feature(lang_items)]
use core::alloc::{GlobalAlloc, Layout};
extern crate alloc;

pub type size_t = cty::c_uint;

use core::panic::PanicInfo;
// C api -> bindgen
mod rn_i2c_c;
pub mod rnarduino;
// internal API
pub mod rn_gpio;
pub mod rn_i2c;
pub mod rn_os_helper;
pub mod rn_exti;
pub mod rn_fast_event_group;
pub mod rn_adc_timer;
pub mod rn_spi;
// embedded hal
pub mod rn_hal_gpio;

#[cfg(feature = "cdc")]
pub mod rn_usb_cdc;
#[cfg(feature = "cdc")]
pub mod rn_cdc_c;

pub struct FreeRtosAllocator;


unsafe impl GlobalAlloc for FreeRtosAllocator {
   unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
    let res = pvPortMalloc(layout.size() as  cty::c_uint);
    res as *mut u8
   }

   unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout)  {
       vPortFree(ptr as  *mut cty::c_void);
   }
}
#[global_allocator] // borrowed from https://github.com/lobaro/FreeRTOS-rust
static GLOBAL: FreeRtosAllocator = FreeRtosAllocator;
extern "C" {
    pub fn pvPortMalloc(xSize: size_t) -> *mut cty::c_void;
}
extern "C" {
    pub fn vPortFree(pv: *mut cty::c_void);
}

extern "C" {
    pub fn deadEnd(code: cty::c_int);
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    unsafe {
    deadEnd(55); //: cty::c_int);
    }
    loop {
        
    }
}

