#!/bin/bash
input=$1
output=$2
echo "$1 => $2"
export BINDGEN=/usr/bin/bindgen
#export BINDGEN=/home/fx/workspace/rust-bindgen/./target/debug/bindgen

export PLATFORM_TOOLCHAIN_PATH=/home/fx/Arduino_stm32/arm-gcc-2021q4/bin
export  PATH=$PLATFORM_TOOLCHAIN_PATH:$PATH
export LN=/home/fx/Arduino_gd32/lnPowerSupply/lnArduino
echo "Processing $LN"
\rm -f rnArduino.rs.tmp
$BINDGEN      --use-core --no-doc-comments \
     --no-layout-tests      $1 \
    --ctypes-prefix cty \
    --no-default "uint32_t" \
   -o $2.tmp -- -x c++   -DLN_ARCH=LN_ARCH_ARM \
    -I$LN -I$LN/include/  -I$LN/arduinoLayer/include/ -I$LN/FreeRTOS/include/  -I$LN/legacy/boards/bluepill/ -I$LN/FreeRTOS/portable/GCC/ARM_CM3/ \
    -target "thumbv7m-none-eabi"  \
    -I${PLATFORM_TOOLCHAIN_PATH}/../arm-none-eabi/include/ -D_UINT32_T_DECLARED -Duint32_t="unsigned long int"
cat header.rs.in > ${output}
cat $2.tmp >> ${output}
cat tail.rs.in >> ${output}
\rm -f $2.tmp
     #--whitelist-function  "^[lLx][nN].*|deadEnd|Logger" \

