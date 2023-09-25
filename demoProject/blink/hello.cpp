#include "lnArduino.h"

#define LED LN_SYSTEM_LED

void setup()
{
    lnPinMode(LED, lnOUTPUT);
}
/**
 *
 */
int roundup = 0;
void loop()
{
    lnDigitalWrite(LED, false);
    xDelay(1000);
    lnDigitalWrite(LED, true);
    xDelay(1000);

    while (1)
    {
        roundup++;
        xDelay(500);
        lnDigitalToggle(LED);
        Logger("*Hey*\n");
    }
}
