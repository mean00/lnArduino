#!/bin/bash
#
export out=rnarduino.rs
# rustified_enum
bash ../../../cmake/rustgen.sh  lnAll.h   ${out}

