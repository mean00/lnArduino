#include "lnADC.h"
#include "lnArduino.h"
#include "lnDAC.h"
#include "lnTimer.h"
#include "math.h"
#if defined(USE_RP2040) || defined(USE_RP2350)
#else
#define LED PA2
#define PWM PB9
#endif
#define TIMER 3
#define CHANNEL 3

void setup()
{
    pinMode(LED, OUTPUT);
}
/**
 *
 */

int val;
void loop()
{
    bool onoff = true;
    digitalWrite(LED, true);
    int roundup = 0;

    lnPinMode(PA0, lnADC_MODE);
    lnPinMode(PA1, lnADC_MODE);
    lnPinMode(PA4, lnDAC_MODE);
    lnPinMode(PA5, lnDAC_MODE);

    lnSimpleADC *adc = new lnSimpleADC(0, PA0);
    lnDAC *dac0 = new lnDAC(0);
    lnDAC *dac1 = new lnDAC(1);

    dac0->simpleMode();
    dac1->simpleMode();

    dac0->enable();
    dac1->enable();

    Logger("Connect PA4 and PA0\n");
    Logger("Connect PA5 and PA1\n");

    uint16_t output[2];
    lnPin pins[2] = {PA0, PA1};

    while (1)
    {
        dac0->setValue(500);
        dac1->setValue(3500);
        delay(10);
        adc->pollingMultiRead(2, pins, output);
        Logger("500:3500 PA0: %d PA1 :%d \n", output[0], output[1]);

        delay(500);
        dac0->setValue(3500);
        dac1->setValue(500);
        delay(10);
        adc->pollingMultiRead(2, pins, output);
        Logger("3500:500 PA0: %d PA1 :%d \n", output[0], output[1]);
        delay(500);
    }
}
