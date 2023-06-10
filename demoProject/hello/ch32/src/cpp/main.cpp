/**
 *  Massively over engineered power supply
 *

 *
 */
#include "lnArduino.h"

/**
 *
 */
void setup()
{

    Logger("Setuping up Hello ...\n");
}
/**
 *
 */
void loop_blink()
{
#define LED1 PA15
    lnPinMode(LED1, lnOUTPUT);
    while (1)
    {
        lnDigitalToggle(LED1);
        Logger("X\n");
        xDelay(1000);
    }
}

void loop()
{
    loop_blink();
}

// EOF
