#![no_std]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![feature(default_alloc_error_handler)]

use core::alloc::{GlobalAlloc, Layout};
extern crate alloc;
use panic_reset as _;
pub type size_t = cty::c_uint;

pub mod rnarduino;
pub mod rnGpio;
pub mod rnI2C;
pub mod rnOsHelper;
pub mod rnExti;
pub mod rnFastEventGroup;
pub mod rnTimingAdc;

pub struct FreeRtosAllocator;


unsafe impl GlobalAlloc for FreeRtosAllocator {
   unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
     let res = pvPortMalloc(layout.size() as  cty::c_uint);
       return res as *mut u8;
   }

   unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) ->() {
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