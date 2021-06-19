
#include "lnArduino.h"

uint8_t ucHeap[configTOTAL_HEAP_SIZE];

extern void setup();
extern void loop();
extern "C" void _init();
void lnIrqSysInit();
void lnExtiSysInit();
void lnDmaSysInit();
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
    lnPeripherals::enable(pDMA1);
    
    // I2C0 & 1
    lnPeripherals::enable(pI2C0);
    lnPeripherals::enable(pI2C1);
    
    // Enable exti
    lnExtiSysInit();
    
    // init DMA
    lnDmaSysInit();
    
    
    // Init SPI0/1/2
    lnPeripherals::enable(pSPI0);
    lnPeripherals::enable(pSPI1);
    lnPeripherals::enable(pSPI2);
    
    // Init Timer 1..4
    lnPeripherals::enable(pTIMER1);
    lnPeripherals::enable(pTIMER2);
    lnPeripherals::enable(pTIMER3);
    lnPeripherals::enable(pTIMER4);
    // DAC
     lnPeripherals::enable(pDAC);
    
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
