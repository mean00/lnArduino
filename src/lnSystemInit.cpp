
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
void disabled(const Peripherals periph)
{
    lnPeripherals::disable(periph);
}
// dummy call to prevent the linker from removing it...
extern const uint32_t *lnGetFreeRTOSDebug();

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
    disabled(pI2C0);
    disabled(pI2C1);

    // Enable exti
    lnExtiSysInit();

    // init DMA
    lnDmaSysInit();


    // Init SPI0/1/2
    disabled(pSPI0);
    disabled(pSPI1);
    disabled(pSPI2);

    // Init Timer 1..4
    disabled(pTIMER1);
    disabled(pTIMER2);
    disabled(pTIMER3);
    disabled(pTIMER4);

    // Basic timer
    disabled(pTIMER5);
    disabled(pTIMER6);

    // DAC
     disabled(pDAC);

     // ADC
     resetMe(pADC0);
     resetMe(pADC1);


    lnRunTimeInitPostPeripherals();

    // Start freertos
    xTaskCreate(initTask,"entryTask",500,NULL,2,NULL);
    vTaskStartScheduler();
    lnGetFreeRTOSDebug();
    deadEnd(25);
}
// EOF
