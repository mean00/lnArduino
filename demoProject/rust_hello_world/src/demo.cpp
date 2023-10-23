
/*
    On the RP2040 gum stick boards the led is driven by GPIO26, it is NOT open drain
*/
#include "lnArduino.h"

#define LED LN_SYSTEM_LED

extern "C" void user_init();
/**
 */
void setup()
{
    lnPinMode(LED, lnOUTPUT_OPEN_DRAIN);
}
void loop()
{
    Logger("Starting Rust demo\n");
    user_init();
    bool onoff = true;
    while (1)
    {
        delay(1000);
        lnOpenDrainClose(LED, onoff);
        onoff = !onoff;
    }
}
// EOF