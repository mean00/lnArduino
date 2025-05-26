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
void test_time(int ms)
{
    int before = lnGetUs();
    xDelay(ms);
    int after = lnGetUs();
    Logger("Before=%d \n", before);
    Logger("After=%d \n", after);
    Logger("delta=%d , should be %d 000\n", after - before, ms);
}
void loop()
{
    bool onoff = true;
    lnDigitalWrite(LED, true);

    Logger("Starting a 1 sec wait...\n");
    xDelay(1000);
    Logger("End of 1 sec wait\n");
    test_time(10);
    test_time(50);
    test_time(100);
    test_time(200);

    while (1)
    {
        roundup++;
        xDelay(500);
        lnDigitalToggle(LED);
        onoff = !onoff;
    }
}
