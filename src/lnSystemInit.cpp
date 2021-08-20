
#include "lnArduino.h"
#include "lnPeripheral_priv.h"

uint8_t ucHeap[configTOTAL_HEAP_SIZE];

extern void setup();
extern void loop();
extern "C" void _init();
void lnIrqSysInit();
void lnExtiSysInit();
void lnDmaSysInit();
void lnRunTimeInit();
void lnRunTimeInitPostPeripherals();
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

void resetMe(const Peripherals periph)
{
    lnPeripherals::reset(periph);
    lnPeripherals::enable(periph);
     
}

int main()
{
    //
    lnRunTimeInit();
    // Initialize system    
    lnInitSystemClock();
    //ECLIC init
    lnIrqSysInit();
    
    // The LEDs are all on GPIO A
    resetMe(pGPIOA);
    resetMe(pGPIOB);
    resetMe(pGPIOC);
        
    // We need alternate functions too
    resetMe(pAF);
    resetMe(pDMA0);
    resetMe(pDMA1);
    
    // I2C0 & 1
    resetMe(pI2C0);
    resetMe(pI2C1);
    
    // Enable exti
    lnExtiSysInit();
    
    // init DMA
    lnDmaSysInit();
    
    
    // Init SPI0/1/2
    resetMe(pSPI0);
    resetMe(pSPI1);
    resetMe(pSPI2);
    
    // Init Timer 1..4
    resetMe(pTIMER1);
    resetMe(pTIMER2);
    resetMe(pTIMER3);
    resetMe(pTIMER4);
    
    // Basic timer
    resetMe(pTIMER5);
    resetMe(pTIMER6);
    
    // DAC
     resetMe(pDAC);
    
     // ADC
     resetMe(pADC0);
     resetMe(pADC1);
    
     
    lnRunTimeInitPostPeripherals();
     
    // Start freertos
    xTaskCreate(initTask,"entryTask",500,NULL,2,NULL);    
    vTaskStartScheduler();      
    deadEnd(25);    
}
// EOF

