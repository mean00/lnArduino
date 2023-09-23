#include "lnArduino.h"
#include "stdio.h"
#include "lnDma.h"
#include "lnIRQ.h"

#include "hardware/gpio.h"

FILE *const stdout=NULL;

extern void setup();
extern void loop();

uint8_t ucHeap[configTOTAL_HEAP_SIZE];

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

extern "C" {
    void xPortSysTickHandler( void );
    void xPortPendSVHandler( void );    
    void vPortSVCHandler( void );
}


int main()
{
    lnPinMode( GPIO17, lnUART);
    lnPinMode( GPIO16, lnUART);


    LoggerInit();

    lnSetInterruptHandler(LN_IRQ_SYSTICK, xPortSysTickHandler);
    lnSetInterruptHandler(LN_IRQ_PENDSV,  xPortPendSVHandler);
    lnSetInterruptHandler(LN_IRQ_SVCALL,  vPortSVCHandler);


    lnCreateTask(initTask, "entryTask", LN_INITIAL_STACK_SIZE, NULL, LN_INITIAL_TASK_PRIORITY);
    vTaskStartScheduler();
    //lnGetFreeRTOSDebug();
    deadEnd(25);
}

extern "C" void deadEnd(int code)
{    
    __asm__("bkpt #0");
}

lnDMA::lnDMA(lnDMA::DmaTransferType type, int dmaEngine, int dmaChannel, int sourceWith, int targetWidth)
{

}
lnDMA::~lnDMA()
{

}