#![allow(dead_code)]
#![allow(unsafe_op_in_unsafe_fn)]
/* automatically generated by rust-bindgen 0.71.1 */

pub const __bool_true_false_are_defined: u32 = 1;
pub const true_: u32 = 1;
pub const false_: u32 = 0;
pub const _ELIX_LEVEL: u32 = 4;
pub const _NEWLIB_VERSION: &[u8; 6] = b"4.3.0\0";
pub const _PICOLIBC_CTYPE_SMALL: u32 = 0;
pub const _PICOLIBC_MINOR__: u32 = 8;
pub const _PICOLIBC_VERSION: &[u8; 6] = b"1.8.6\0";
pub const _PICOLIBC__: u32 = 1;
pub const __NEWLIB_MINOR__: u32 = 3;
pub const __NEWLIB_PATCHLEVEL__: u32 = 0;
pub const __NEWLIB__: u32 = 4;
pub const __PICOLIBC_MINOR__: u32 = 8;
pub const __PICOLIBC_PATCHLEVEL__: u32 = 6;
pub const __PICOLIBC_VERSION__: &[u8; 6] = b"1.8.6\0";
pub const __PICOLIBC__: u32 = 1;
pub const _ATFILE_SOURCE: u32 = 1;
pub const _DEFAULT_SOURCE: u32 = 1;
pub const _ISOC99_SOURCE: u32 = 1;
pub const _ISOC11_SOURCE: u32 = 1;
pub const _ISOC2X_SOURCE: u32 = 1;
pub const _POSIX_SOURCE: u32 = 1;
pub const _POSIX_C_SOURCE: u32 = 200809;
pub const _XOPEN_SOURCE: u32 = 700;
pub const _XOPEN_SOURCE_EXTENDED: u32 = 1;
pub const _LARGEFILE64_SOURCE: u32 = 1;
pub const __ATFILE_VISIBLE: u32 = 1;
pub const __BSD_VISIBLE: u32 = 1;
pub const __GNU_VISIBLE: u32 = 1;
pub const __ZEPHYR_VISIBLE: u32 = 0;
pub const __ISO_C_VISIBLE: u32 = 2020;
pub const __LARGEFILE_VISIBLE: u32 = 1;
pub const __LARGEFILE64_VISIBLE: u32 = 1;
pub const __MISC_VISIBLE: u32 = 1;
pub const __POSIX_VISIBLE: u32 = 200809;
pub const __SVID_VISIBLE: u32 = 1;
pub const __XSI_VISIBLE: u32 = 700;
pub const __SSP_FORTIFY_LEVEL: u32 = 0;
pub const __OBSOLETE_MATH_DEFAULT: u32 = 1;
pub const __OBSOLETE_MATH: u32 = 1;
pub const __OBSOLETE_MATH_FLOAT: u32 = 1;
pub const __OBSOLETE_MATH_DOUBLE: u32 = 1;
pub const __RAND_MAX: u32 = 2147483647;
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
pub const __GNUCLIKE_ASM: u32 = 3;
pub const __GNUCLIKE___TYPEOF: u32 = 1;
pub const __GNUCLIKE___SECTION: u32 = 1;
pub const __GNUCLIKE_CTOR_SECTION_HANDLING: u32 = 1;
pub const __GNUCLIKE_BUILTIN_CONSTANT_P: u32 = 1;
pub const __GNUCLIKE_BUILTIN_VARARGS: u32 = 1;
pub const __GNUCLIKE_BUILTIN_STDARG: u32 = 1;
pub const __GNUCLIKE_BUILTIN_VAALIST: u32 = 1;
pub const __GNUC_VA_LIST_COMPATIBILITY: u32 = 1;
pub const __GNUCLIKE_BUILTIN_NEXT_ARG: u32 = 1;
pub const __GNUCLIKE_BUILTIN_MEMCPY: u32 = 1;
pub const __CC_SUPPORTS_INLINE: u32 = 1;
pub const __CC_SUPPORTS___INLINE: u32 = 1;
pub const __CC_SUPPORTS___INLINE__: u32 = 1;
pub const __CC_SUPPORTS___FUNC__: u32 = 1;
pub const __CC_SUPPORTS_WARNING: u32 = 1;
pub const __CC_SUPPORTS_VARADIC_XXX: u32 = 1;
pub const __CC_SUPPORTS_DYNAMIC_ARRAY_INIT: u32 = 1;
pub const __int20: u32 = 2;
pub const __int20__: u32 = 2;
pub const __INT8: &[u8; 3] = b"hh\0";
pub const __INT16: &[u8; 2] = b"h\0";
pub const __INT64: &[u8; 3] = b"ll\0";
pub const __FAST8: &[u8; 3] = b"hh\0";
pub const __FAST16: &[u8; 2] = b"h\0";
pub const __FAST64: &[u8; 3] = b"ll\0";
pub const __LEAST8: &[u8; 3] = b"hh\0";
pub const __LEAST16: &[u8; 2] = b"h\0";
pub const __LEAST64: &[u8; 3] = b"ll\0";
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
pub struct ln_i2c_c {
    pub dummy: *mut cty::c_void,
}
unsafe extern "C" {
    pub fn lni2c_create(instance: cty::c_int, speed: cty::c_int) -> *mut ln_i2c_c;
}
unsafe extern "C" {
    pub fn lni2c_delete(i2c: *mut ln_i2c_c);
}
unsafe extern "C" {
    pub fn lni2c_setSpeed(ptr: *mut ln_i2c_c, speed: cty::c_int);
}
unsafe extern "C" {
    pub fn lni2c_setAddress(ptr: *mut ln_i2c_c, address: cty::c_int);
}
unsafe extern "C" {
    pub fn lni2c_write(ptr: *mut ln_i2c_c, n: cty::c_uint, data: *const u8) -> bool;
}
unsafe extern "C" {
    pub fn lni2c_read(ptr: *mut ln_i2c_c, n: cty::c_uint, data: *mut u8) -> bool;
}
unsafe extern "C" {
    pub fn lni2c_write_to(
        ptr: *mut ln_i2c_c,
        target: cty::c_int,
        n: cty::c_uint,
        data: *const u8,
    ) -> bool;
}
unsafe extern "C" {
    pub fn lni2c_multi_write_to(
        ptr: *mut ln_i2c_c,
        target: cty::c_int,
        nbSeqn: cty::c_uint,
        seqLength: *const cty::c_uint,
        data: *mut *const u8,
    ) -> bool;
}
unsafe extern "C" {
    pub fn lni2c_read_from(
        ptr: *mut ln_i2c_c,
        target: cty::c_int,
        n: cty::c_uint,
        data: *mut u8,
    ) -> bool;
}
unsafe extern "C" {
    pub fn lni2c_begin(ptr: *mut ln_i2c_c, target: cty::c_int) -> bool;
}
