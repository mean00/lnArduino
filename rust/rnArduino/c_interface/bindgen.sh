#!/bin/bash
#
gen_cpp()
{
bash ../../../cmake/rustgen.sh  ${1}   ${2}
}

gen_c()
{
bash ../../../cmake/rustgen_c.sh  ${1}   ${2}
}


gen_cpp lnI2C_c.h ../src/rn_i2c_c.rs
gen_c lnCDC_c.h ../src/rn_cdc_c.rs
gen_c lnUSB_c.h ../src/rn_usb_c.rs

