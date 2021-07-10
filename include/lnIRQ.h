/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"

#if 0
#if LN_ARCH == RISCV
    #include "lnIRQ_riscv.h"
#else
    #if LN_ARCH == ARM
        #include "lnIRQ_arm.h"
    #else
        #error UNSUPPORTED ARCH
    #endif
#endif
#else

 #include "lnIRQ_arm.h"
#endif



void lnEnableInterrupt(const LnIRQ &irq);
void lnDisableInterrupt(const LnIRQ &irq);
