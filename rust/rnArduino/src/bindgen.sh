#!/bin/bash
#
export out=rnarduino.rs
# rustified_enum
bash ../../../cmake/rustgen_c.sh  lnAll.h   ${out}

