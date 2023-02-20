#!/bin/bash
#
gen()
{
bash ../../../cmake/rustgen.sh  ${1}   ${2}
}


gen lnI2C_c.h ../src/rn_i2c_c.rs
gen lnCDC_c.h ../src/rn_cdc_c.rs
gen lnUSB_c.h ../src/rn_usb_c.rs

