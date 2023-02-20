#![allow(dead_code)]
/* automatically generated by rust-bindgen 0.63.0 */

pub const _MB_LEN_MAX: u32 = 1;
pub const _NEWLIB_VERSION: &[u8; 6usize] = b"4.1.0\0";
pub const _PICOLIBC_MINOR__: u32 = 7;
pub const _PICOLIBC_VERSION: &[u8; 6usize] = b"1.7.4\0";
pub const _PICOLIBC__: u32 = 1;
pub const __NEWLIB_MINOR__: u32 = 1;
pub const __NEWLIB_PATCHLEVEL__: u32 = 0;
pub const __NEWLIB__: u32 = 4;
pub const __PICOLIBC_MINOR__: u32 = 7;
pub const __PICOLIBC_PATCHLEVEL__: u32 = 4;
pub const __PICOLIBC_VERSION__: &[u8; 6usize] = b"1.7.4\0";
pub const __PICOLIBC__: u32 = 1;
pub const _DEFAULT_SOURCE: u32 = 1;
pub const _POSIX_SOURCE: u32 = 1;
pub const _POSIX_C_SOURCE: u32 = 200809;
pub const _ATFILE_SOURCE: u32 = 1;
pub const __ATFILE_VISIBLE: u32 = 1;
pub const __BSD_VISIBLE: u32 = 1;
pub const __GNU_VISIBLE: u32 = 0;
pub const __ISO_C_VISIBLE: u32 = 2011;
pub const __LARGEFILE_VISIBLE: u32 = 0;
pub const __MISC_VISIBLE: u32 = 1;
pub const __POSIX_VISIBLE: u32 = 200809;
pub const __SVID_VISIBLE: u32 = 1;
pub const __XSI_VISIBLE: u32 = 0;
pub const __SSP_FORTIFY_LEVEL: u32 = 0;
pub const __have_longlong64: u32 = 1;
pub const __have_long32: u32 = 1;
pub const ___int8_t_defined: u32 = 1;
pub const ___int16_t_defined: u32 = 1;
pub const ___int32_t_defined: u32 = 1;
pub const ___int64_t_defined: u32 = 1;
pub const ___int_least8_t_defined: u32 = 1;
pub const ___int_least16_t_defined: u32 = 1;
pub const ___int_least32_t_defined: u32 = 1;
pub const ___int_least64_t_defined: u32 = 1;
pub const __int20: u32 = 2;
pub const __int20__: u32 = 2;
pub const __INT8: &[u8; 3usize] = b"hh\0";
pub const __INT16: &[u8; 2usize] = b"h\0";
pub const __INT64: &[u8; 3usize] = b"ll\0";
pub const __FAST8: &[u8; 3usize] = b"hh\0";
pub const __FAST16: &[u8; 2usize] = b"h\0";
pub const __FAST64: &[u8; 3usize] = b"ll\0";
pub const __LEAST8: &[u8; 3usize] = b"hh\0";
pub const __LEAST16: &[u8; 2usize] = b"h\0";
pub const __LEAST64: &[u8; 3usize] = b"ll\0";
pub const __int8_t_defined: u32 = 1;
pub const __int16_t_defined: u32 = 1;
pub const __int32_t_defined: u32 = 1;
pub const __int64_t_defined: u32 = 1;
pub const __int_least8_t_defined: u32 = 1;
pub const __int_least16_t_defined: u32 = 1;
pub const __int_least32_t_defined: u32 = 1;
pub const __int_least64_t_defined: u32 = 1;
pub const __int_fast8_t_defined: u32 = 1;
pub const __int_fast16_t_defined: u32 = 1;
pub const __int_fast32_t_defined: u32 = 1;
pub const __int_fast64_t_defined: u32 = 1;
pub const WINT_MIN: u32 = 0;
pub type __int8_t = cty::c_schar;
pub type __uint8_t = cty::c_uchar;
pub type __int16_t = cty::c_short;
pub type __uint16_t = cty::c_ushort;
pub type __int32_t = cty::c_int;
pub type __uint32_t = cty::c_uint;
pub type __int64_t = cty::c_longlong;
pub type __uint64_t = cty::c_ulonglong;
pub type __int_least8_t = cty::c_schar;
pub type __uint_least8_t = cty::c_uchar;
pub type __int_least16_t = cty::c_short;
pub type __uint_least16_t = cty::c_ushort;
pub type __int_least32_t = cty::c_int;
pub type __uint_least32_t = cty::c_uint;
pub type __int_least64_t = cty::c_longlong;
pub type __uint_least64_t = cty::c_ulonglong;
pub type __intmax_t = cty::c_longlong;
pub type __uintmax_t = cty::c_ulonglong;
pub type __intptr_t = cty::c_int;
pub type __uintptr_t = cty::c_uint;
pub type intmax_t = __intmax_t;
pub type uintmax_t = __uintmax_t;
pub type int_least8_t = __int_least8_t;
pub type uint_least8_t = __uint_least8_t;
pub type int_least16_t = __int_least16_t;
pub type uint_least16_t = __uint_least16_t;
pub type int_least32_t = __int_least32_t;
pub type uint_least32_t = __uint_least32_t;
pub type int_least64_t = __int_least64_t;
pub type uint_least64_t = __uint_least64_t;
pub type int_fast8_t = cty::c_schar;
pub type uint_fast8_t = cty::c_uchar;
pub type int_fast16_t = cty::c_short;
pub type uint_fast16_t = cty::c_ushort;
pub type int_fast32_t = cty::c_int;
pub type uint_fast32_t = cty::c_uint;
pub type int_fast64_t = cty::c_longlong;
pub type uint_fast64_t = cty::c_ulonglong;
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct lncdc_c {
    pub dummy: *mut cty::c_void,
}
pub type cdc_event_handler = ::core::option::Option<
    unsafe extern "C" fn(
        cookie: *mut cty::c_void,
        interface: cty::c_int,
        event: cty::c_int,
        payload: cty::c_uint,
    ),
>;
extern "C" {
    pub fn lncdc_create(instance: cty::c_uint) -> *mut lncdc_c;
}
extern "C" {
    pub fn lncdc_delete(arg1: *mut lncdc_c);
}
extern "C" {
    pub fn lncdc_read(arg1: *mut lncdc_c, buffer: *mut u8, maxSize: cty::c_int) -> cty::c_int;
}
extern "C" {
    pub fn lncdc_write(arg1: *mut lncdc_c, buffer: *const u8, maxSize: cty::c_int) -> cty::c_int;
}
extern "C" {
    pub fn lncdc_flush(arg1: *mut lncdc_c);
}
extern "C" {
    pub fn lncdc_clear_input_buffers(arg1: *mut lncdc_c);
}
extern "C" {
    pub fn lncdc_set_event_handler(
        arg1: *mut lncdc_c,
        h: cdc_event_handler,
        cookie: *mut cty::c_void,
    );
}
