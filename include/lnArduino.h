#pragma once

#include "stdio.h"
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