#!/bin/bash

export CUR=$PWD
#
#
#
function fail()
{
echo "********* FAIL ***********"
echo "********* FAIL ***********"
exit 4
}
#
#
#
function buildAll()
{
echo "** STARTING ARCH ${ARCH}:${MCU}"
for i in `ls  ../demoProject/ | sort -n| grep -v cmake | grep -v everything`;
#for i in adc;
do
    cd $CUR
    echo "___________________________________"
    echo "$ARCH:$MCU => $i"
    cd ../demoProject/$i
    rm -f ${SDK}
    ln -s $PWD/../../ lnArduino
    rm -Rf build${ARCH}_${MCU}
    mkdir build${ARCH}_${MCU}
    cd build${ARCH}_${MCU}
    #echo "*** ${i} ****"
    cmake -DLN_ARCH="${ARCH}" -DLN_MCU_RAM_SIZE="${RAM}" -DLN_MCU="${MCU}" .. >& /tmp/log_$i_cmake.txt|| fail cmake
    make -j 6 >& /tmp/log_$i.txt  || fail make
    #echo "/*** ${i} ****"
    echo "   OK"
    cd $CUR
done
echo "** DONE FOR ARCH ${ARCH}:${MCU}"
}
export SDK=lnArduino
export ARCH=ARM
export MCU=M3
buildAll

export ARCH=RISCV
export MCU=VF103
buildAll

export ARCH=ARM
export MCU=M4
buildAll
echo "*** ALL DONE  OK ***"
