#include "lnArduino.h"
#include "stdio.h"
#include "lnDma.h"
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
int main()
{
    LoggerInit();
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