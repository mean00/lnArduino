set -x
export LN=$PWD/../../..
bindgen   --use-core --ctypes-prefix cty --whitelist-function  "^[lL][nN].*" --no-layout-tests      lnAll.h  -o rnArduino.rs.tmp -- -x c++   -DLN_ARCH=LN_ARCH_ARM \
    -I$LN -I$LN/include  -I$LN/arduinoLayer/include/ -I$LN/FreeRTOS/include/  -I$LN/legacy/boards/bluepill/ -I$LN/FreeRTOS/portable/GCC/ARM_CM3/

echo " #![no_std]" > rnArduino.rs
cat rnArduino.rs.tmp >> rnArduino.rs
