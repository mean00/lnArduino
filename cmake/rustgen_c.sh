#!/bin/bash
#set -x
#
#set -x
if [ "$#" -ne 2 ]; then
    echo "rustgen.sh xxx.h xxx.rs"
    exit 1
fi
#input=$1
output=$2
ME=$0
export ME=`realpath $ME`
export ME=`dirname $ME`
#export BINDGEN=/usr/bin/bindgen
export BINDGEN=~/.cargo/bin/bindgen
export LN=`realpath $ME/..` 

export PLATFORM_TOOLCHAIN_PATH=`cmake -DHOME=$LN -P $ME/toolchain_env.cmake 2>& 1`
#export PLATFORM_TOOLCHAIN_PATH=/home/fx/Arduino_stm32/arm-gcc-2021q4/bin

export PATH=$PLATFORM_TOOLCHAIN_PATH:$PATH

echo "** Processing $1 **"
echo "   using <$LN> as basedir **"
echo "   using <$PLATFORM_TOOLCHAIN_PATH> as PLATFORM_TOOLCHAIN_PATH **"
echo "$1 => $2"
export TMP=$1.tmp
echo "Tmp .h file = $TMP"
rm -f $TMP
echo "#ifdef __ARM_ARCH "> $TMP
echo "   #undef uint32_t ">> $TMP
echo "   #define _UINT32_T_DECLARED " >> $TMP
echo "   #define uint32_t unsigned int ">> $TMP
echo "#endif" >> $TMP
cat $1 >> $TMP
\rm -f rnArduino.rs.tmp
$BINDGEN      --use-core --no-doc-comments \
     --no-layout-tests      $TMP \
    --ctypes-prefix core::ffi \
     \
    --rustified-enum "ln.*" \
    -o $2.tmp \
    -- -x c   -DLN_ARCH=LN_ARCH_ARM \
    -I$LN \
    -I$LN/include/  \
    -I$LN/arduinoLayer/include/ \
    -I$LN/FreeRTOS/include/  \
    -I$LN/mcus/arm_gd32fx/include/ \
    -I$LN/FreeRTOS/portable/GCC/ARM_CM3/ \
    -target "thumbv7m-none-eabi"  \
    -I${PLATFORM_TOOLCHAIN_PATH}/../arm-none-eabi/include/ \
    
cat header.rs.in > ${output}
cat $2.tmp >> ${output}
cat tail.rs.in >> ${output}
\rm -f $2.tmp
     #--whitelist-function  "^[lLx][nN].*|deadEnd|Logger" \

