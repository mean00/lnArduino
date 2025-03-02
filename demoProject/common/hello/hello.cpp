#include "lnArduino.h"

#define LED LN_SYSTEM_LED

void setup()
{
    pinMode(LED, OUTPUT);
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
        digitalToggle(LED);
    }
}
