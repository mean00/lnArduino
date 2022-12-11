#!/bin/bash
#
export out=../src/rn_i2c_c.rs
# rustified_enum
bash ../../../cmake/rustgen.sh  lnI2C_c.h   ${out}

