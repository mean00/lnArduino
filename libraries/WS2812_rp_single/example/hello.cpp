#include "lnArduino.h"
#include "lnGPIO.h"
#include "lnSerial.h"

#define LED LN_SYSTEM_LED
#include "lnWS2812_rp_single.h"

int pix, inc;

WS2812_rp2040_single *led;

/**
 * @brief
 *
 */
void setup()
{
    Logger("Starting SPI Test\n");
    led = new WS2812_rp2040_single(0, GPIO23);
}
/**
 * @brief
 *
 */
void loop()
{
    led->setColor(0, 255, 0);
    lnDelay(1000);
    led->setColor(0, 0, 255);
    lnDelay(1000);
    led->setColor(255, 0, 0);
    lnDelay(1000);
}
// EOF