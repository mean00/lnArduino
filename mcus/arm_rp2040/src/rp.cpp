#include "lnArduino.h"
#include "lnIRQ.h"
#include "stdio.h"

#include "hardware/gpio.h"

FILE *const stdout = NULL;

extern void setup();
extern void loop();

uint8_t ucHeap[configTOTAL_HEAP_SIZE];
extern void lnRpDmaSysInit();
/**
 * @brief
 *
 */
extern "C" void alarm_pool_init_default()
{
// xAssert(0);
#warning this is a stub to avoid pulling the pico-sdk libc malloc
}

/**
 *
 * @param
 */
void initTask(void *)
{
    LoggerInit();
    setup();
    while (1)
    {
        loop();
    }
}

#define LN_INITIAL_STACK_SIZE 1024
#define LN_INITIAL_TASK_PRIORITY 2
uint32_t SystemCoreClock = 100000000;

extern "C"
{
    void xPortSysTickHandler(void);
    void xPortPendSVHandler(void);
    void vPortSVCHandler(void);
}
/**
 * @brief
 *
 * @return int
 */
int main()
{
    lnPinMode(GPIO17, lnUART);
    lnPinMode(GPIO16, lnUART);
    lnRpDmaSysInit();

    lnSetInterruptHandler(LN_IRQ_SYSTICK, xPortSysTickHandler);
    lnSetInterruptHandler(LN_IRQ_PENDSV, xPortPendSVHandler);
    lnSetInterruptHandler(LN_IRQ_SVCALL, vPortSVCHandler);

    lnCreateTask(initTask, "entryTask", LN_INITIAL_STACK_SIZE, NULL, LN_INITIAL_TASK_PRIORITY);
    vTaskStartScheduler();
    // lnGetFreeRTOSDebug();
    deadEnd(25);
}

extern "C" void deadEnd(int code)

{
    __asm__("bkpt #0");
}
