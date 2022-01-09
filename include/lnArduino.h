/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once

#include "stdio.h"
#include "string.h"
#include "stdint.h"


#define LN_ARCH_UNKNOWN     0
#define LN_ARCH_RISCV       1
#define LN_ARCH_ARM         2

#define LN_USED __attribute__((used))


#if LN_ARCH == LN_ARCH_RISCV
    #include "lnIRQ_riscv.h"
    #define LN_FENCE()  __asm volatile("fence.i")
    #define LN_IOWRITE(adr,value)  {*adr=value;}
#else
    #if LN_ARCH == LN_ARCH_ARM        
        #define LN_FENCE() {} // no need for fence on arm
        #define LN_IOWRITE(adr,value) {*adr=value;}
    #else
        #error UNSUPPORTED ARCH
    #endif
#endif

#include "Arduino.h"
#include "systemHelper.h"
extern "C"
{
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
   
}




#include "lnFreeRTOS.h"
#include "lnDebug.h"
#include "lnGPIO.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "lnPeripherals.h"

#include "lnPrintf.h"
#include "lnSystemTime.h"






