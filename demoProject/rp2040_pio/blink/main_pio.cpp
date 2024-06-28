/**
 *  Demo code for ILI9341 320x240 screen with touch capability
 *
 *
 */
#include "lnArduino.h"

/**
 *
 */
void setup()
{
    Logger("Setuping up pio_swd demo...\n");
}

/**
 * @brief
 *
 */
// extern LN_SPI_Registers *aspi0 ;
extern void runPio();
void loop()
{
    runPio();
    bool one = true;
    while (true)
    {
        lnDigitalToggle(LN_SYSTEM_LED);
        // foo();
        lnDelayMs(900);
    }
}
//
