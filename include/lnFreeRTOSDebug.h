#pragma once
#include "lnArduino.h"

#define LN_FREERTOS_MAGIC 0x1FEEBAEUL

#if !defined(configUSE_TRACE_FACILITY)
#error configUSE_TRACE_FACILITY must be defined to 1
#endif

#if configUSE_TRACE_FACILITY != 1
#error configUSE_TRACE_FACILITY must be defined to 1
#endif

struct lnFreeRTOSDebug
{
    int MAGIC;
    int LIST_SIZE;
    int OFFSET_LIST_ITEM_NEXT;
    int OFFSET_LIST_ITEM_OWNER;

    int OFFSET_LIST_NUMBER_OF_ITEM;
    int OFFSET_LIST_INDEX;

    int NB_OF_PRIORITIES;
    int MPU_ENABLED; // that one selects the register/stacking : 0 :=> M0 or M3, 1=> M4 + FPU
    int MAX_TASK_NAME_LEN;
    int OFFSET_TASK_NAME;
    int OFFSET_TASK_NUM;
};
