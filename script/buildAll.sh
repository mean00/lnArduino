#!/bin/bash

export ARCH=RISCV
export MCU=VF103
export SDK=Arduino_gd32_freeRTOS

function fail()
{
echo "********* FAIL ***********"
echo "********* FAIL ***********"
exit 4
}
export CUR=$PWD
for i in `ls ../demoProject/`;
do
    cd $CUR
    echo "___________________________________"
    echo "$ARCH:$MCU => $i"
    echo "___________________________________"
    cd ../demoProject/$i
    rm -f ${SDK}
    ln -s $PWD/../../../${SDK} .
    rm -Rf build
    mkdir build
    cd build
    cmake -DLN_ARCH=${ARCH} -DLN_MCU=${MCU} .. || fail cmake
    make -j 4  || fail make

done
