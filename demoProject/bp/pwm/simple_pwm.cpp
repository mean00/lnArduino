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

    timer.setPwmFrequency(1000);
    timer.setPwmMode(50); // 50 % duty cycle
    timer.enable();
    while (1)
    {
        lnDigitalToggle(LED);
        xDelay(1000);
    }
}
// EOF
