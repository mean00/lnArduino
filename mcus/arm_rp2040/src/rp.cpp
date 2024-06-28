#include "lnArduino.h"
#include "lnIRQ.h"
#include "stdio.h"

#include "hardware/gpio.h"
#include "ln_rp_memory_map.h"
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

#define LN_INITIAL_STACK_SIZE 8 * 1024
#define LN_INITIAL_TASK_PRIORITY 2
uint32_t SystemCoreClock = 125000000;

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

/**
 *
 */
void lnSoftSystemReset(void)
{
    volatile uint32_t *aircr = (volatile uint32_t *)(0xe000ed0cUL); // see 2.4.8 in RP2040 doc
    *aircr = (1 << 2) + (0x5fa << 16);                              // SYSRESETREQ + VECTKEY
}
/**
 *
 */
extern "C" void deadEnd(int code)

{
    __asm__("bkpt #0");
    lnSoftSystemReset();
}

/**
 *
 */
void lnRp2040_reboot_to_usb()
{
    // we hardcode the address for the momment
#warning HARDCODED ROM ADDRESS
    typedef void *cb_usb_reset(uint32_t a, uint32_t b);
    cb_usb_reset *reset = (cb_usb_reset *)0x2591;
    reset(0, 0);
}

// EOF
