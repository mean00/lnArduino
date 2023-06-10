#include "lnArduino.h"
#include "lnSPI.h"
#include "signal.h"

// green = PA1, blue = PA2, RED PC13
#define LED PA2
#define LEDRED PC13

/**
 *
 */

void setup()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
}
/**
 */
void loop()
{
    Logger("Entering main app...\n");

    Logger("Starting Signal Generator...\n");
    SignalGenerator *signal = new SignalGenerator(PA4, 0);
    signal->start(25000, SignalGenerator::SignalSine);
    Logger("waiting...\n");
    while (1)
    {
        xDelay(10);
    }
}
