#!/bin/bash
export LN=` realpath $PWD/../../..`
echo "Processing $LN"
rm -f  rnArduino.rs.tmp 
bindgen   --use-core --ctypes-prefix cty  --no-layout-tests      lnAll.h  -o rnArduino.rs.tmp -- -x c++   -DLN_ARCH=LN_ARCH_ARM \
    -I$LN -I$LN/include/  -I$LN/arduinoLayer/include/ -I$LN/FreeRTOS/include/  -I$LN/legacy/boards/bluepill/ -I$LN/FreeRTOS/portable/GCC/ARM_CM3/


cat header.h.in > lib.rs
cat rnArduino.rs.tmp >> lib.rs

# --whitelist-function  "^[lLx][nN].*"
