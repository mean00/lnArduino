#include "esprit.h"

#define LED LN_SYSTEM_LED

void setup()
{
    lnPinMode(LED, lnOUTPUT);
}
/**
 *
 */
void loop()
{
    int val = 0;
    Logger("Starting heelo ...\n");
    while (1)
    {
        lnDigitalToggle(LED);
    }
}
