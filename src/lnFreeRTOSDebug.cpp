
/*
 *
 * */
#include "lnFreeRTOSDebug.h"

extern const uint32_t *lnGetFreeRTOSDebug() LN_USED;

static const lnFreeRTOSDebug freeRTOSDebug LN_USED={
  0x1FEEBAEUL,
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
