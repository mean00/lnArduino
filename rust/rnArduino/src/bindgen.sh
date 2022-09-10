#!/bin/bash
# Clang
export PLATFORM_TOOLCHAIN_PATH=/home/fx/Arduino_stm32/arm-gcc-2021q4/bin
export  PATH=$PLATFORM_TOOLCHAIN_PATH:$PATH


#
export LN=` realpath $PWD/../../..`
export out=rnarduino.rs
echo "Processing $LN"
\rm -f rnArduino.rs.tmp
# rustified_enum

bindgen      --use-core --no-doc-comments \
     --no-layout-tests      lnAll.h \
    --ctypes-prefix cty \
    --rustified-enum "ln.*" \
   -o rnArduino.rs.tmp -- -x c++   -DLN_ARCH=LN_ARCH_ARM \
    -I$LN -I$LN/include/  -I$LN/arduinoLayer/include/ -I$LN/FreeRTOS/include/  -I$LN/legacy/boards/bluepill/ -I$LN/FreeRTOS/portable/GCC/ARM_CM3/ \
    -target "thumbv7m-none-eabi"  \
    -I${PLATFORM_TOOLCHAIN_PATH}/../arm-none-eabi/include/ 
cat header.rs.in > ${out}
cat rnArduino.rs.tmp >> ${out}
cat tail.rs.in >> ${out}
\rm -f rnArduino.rs.tmp
#patch -p4 < patched_lnPin_enum.diff 
     #--whitelist-function  "^[lLx][nN].*|deadEnd|Logger" \

