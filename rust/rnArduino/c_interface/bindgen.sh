#!/bin/bash
#
export out=../src/rn_i2c_c.rs
# rustified_enum
bash ../../../cmake/rustgen.sh  lnI2C_c.h   ${out}
export out=../src/rn_cdc_c.rs
bash ../../../cmake/rustgen.sh  lnCDC_c.h   ${out}

