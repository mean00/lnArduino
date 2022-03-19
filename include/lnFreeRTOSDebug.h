#pragma once
#include "lnArduino.h"

#define LN_FREERTOS_MAGIC  0x1FEEBAEUL


#if !defined(configUSE_TRACE_FACILITY)
  #error configUSE_TRACE_FACILITY must be defined to 1
#endif

#if configUSE_TRACE_FACILITY!=1
  #error configUSE_TRACE_FACILITY must be defined to 1
#endif


struct lnFreeRTOSDebug
{
  int MAGIC;
  int NB_OF_PRIORITIES;
  int MPU_ENABLED;
  int MAX_TASK_NAME_LEN;
  int OFFSET_TASK_NUM;
};
