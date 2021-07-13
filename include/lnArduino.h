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




