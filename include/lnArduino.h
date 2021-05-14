/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once

#include "stdio.h"
#include "string.h"
#include "stdint.h"
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
#include "MapleFreeRTOS1000_pp.h"
#include "lnDebug.h"
#include "lnGPIO.h"
#include "lnRCU.h"
#include "gd32vf103.h"
#include "lnPeripherals.h"
#define byte uint8_t 


