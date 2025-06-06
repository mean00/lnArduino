#include "esprit.h"
#include "lnTimer.h"

#define LED LN_SYSTEM_LED

#define PWM_PIN PA6

void setup()
{
    lnPinMode(LED, lnOUTPUT);
}

void loop()
{
    bool onoff = true;
    lnDigitalWrite(LED, true);
    int roundup = 0;

    lnPinMode(PWM_PIN, lnPWM);
    lnTimer timer(PWM_PIN);

    Logger("PWM on pin %d\n", PWM_PIN);

    while (1)
    {

        timer.singleShot(1, false);
        lnDigitalToggle(LED);
        xDelay(1000);
    }
}
// EOF
