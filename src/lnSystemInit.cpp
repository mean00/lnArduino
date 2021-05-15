
#include "lnArduino.h"

uint8_t ucHeap[configTOTAL_HEAP_SIZE];

extern void setup();
extern void loop();
extern "C" void _init();
void lnIrqSysInit();
/**
 * 
 * @param 
 */
void initTask(void *)
{
    LoggerInit();   
    setup();
    while(1)
    {
        loop();
    }
}
/**
 * 
 * @return 
 */
int main()
{
    // Initialize system    
    lnInitSystemClock();
    //ECLIC init
    lnIrqSysInit();
    
    // The LEDs are all on GPIO A
    lnPeripherals::enable(pGPIOA);
    lnPeripherals::enable(pGPIOB);
    lnPeripherals::enable(pGPIOC);
        
    // We need alternate functions too
    lnPeripherals::enable(pAF);
    lnPeripherals::enable(pDMA0);
#warning FIXME FIXME FIXME
#if 0    
    // DMA TX for SPI0 (LCD)
    DMA_CHCTL(LN_DMA0_ADR, 2) = (uint32_t)(DMA_PRIORITY_HIGH | DMA_CHXCTL_DIR); 
    // DMA TX for usart0
    DMA_CHCTL(LN_DMA0_ADR, 4) = (uint32_t)(DMA_PRIORITY_HIGH | DMA_CHXCTL_DIR); 
    //
#endif    
    // Start freertos
    xTaskCreate(initTask,"entryTask",500,NULL,2,NULL);    
    vTaskStartScheduler();      
    deadEnd(25);    
}
// EOF
