/**
*
*/
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "lnADC.h"
#include "lnGPIO.h"

static bool adc_inited = false;

/**
 *
 * @param instance
 */
lnSimpleADC::lnSimpleADC(int instance, lnPin pin)
{
  if(!adc_inited)
  {
    adc_inited=true;
    adc_init();
  }
  xAssert(instance==0);
  setPin(pin);
}
/**
*
*/
lnSimpleADC::~lnSimpleADC()
{

}
/**
*
*/
lnPin lnSimpleADC::pin()
{
return _pin;
}
/**
*
*/
bool lnSimpleADC::setPin(lnPin pin)
{
  _pin=pin;
  lnPinMode(_pin, lnADC_MODE);
}
/**
*
*/
int lnSimpleADC::simpleRead()
{

}
/**
*
*/
int lnSimpleADC::getVref()
{

}
/**
*
*/
float lnSimpleADC::getVcc()
{

}
/**
*
*/
void lnSimpleADC::readVcc()
{

}
/**
*
*/
void lnSimpleADC::setup()
{

}
    //lnPin _pin;

