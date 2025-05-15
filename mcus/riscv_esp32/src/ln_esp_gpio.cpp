
#include "lnArduino.h"
#include "driver/gpio.h"
#include "lnGPIO_pins.h"
/**
 */
void lnPinMode(const lnPin pin, const lnGpioMode mode, const int speedInMhz)
{

}
/**
*
*
*/
void lnDigitalWrite(const lnPin pin, bool value)
{

}
/**
*
*
*/
bool lnDigitalRead(const lnPin pin)
{
  return false;
}
/**
*
*
*/
void lnDigitalToggle(const lnPin pin)
{

}
/**
*
*/
void lnOpenDrainClose(const lnPin pin, const bool close) // if true, the open drain is passing, else it is hiz
{

}
/**
*
*/
extern "C" void deadEnd(int code)
{
    __asm__("ebreak");
//    lnSoftSystemReset();
}
//  EOF
