
/*
 *
 * */
#include "lnFreeRTOSDebug.h"

extern const uint32_t *lnGetFreeRTOSDebug() LN_USED;

extern "C" const lnFreeRTOSDebug freeRTOSDebug;
const lnFreeRTOSDebug freeRTOSDebug ={
  LN_FREERTOS_MAGIC,
  configMAX_PRIORITIES,//int NB_OF_PRIORITIES;
  0, //int MPU_ENABLED;
  configMAX_TASK_NAME_LEN, // MAX_TASK_NAME_LEN
};
/*
  dummy function to prevent the linker from removing it
*/
const uint32_t *lnGetFreeRTOSDebug()
{
    return (uint32_t *)&freeRTOSDebug;
}


// EOF
