#pragma once
#include "lnArduino.h"

#define LN_FREERTOS_MAGIC  0x1FEEBAEUL

struct lnFreeRTOSDebug
{
  int MAGIC;
  int NB_OF_PRIORITIES;
  int MPU_ENABLED;
  int MAX_TASK_NAME_LEN;

};
