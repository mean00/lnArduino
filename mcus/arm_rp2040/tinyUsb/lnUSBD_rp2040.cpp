
/*

 */
#include "lnArduino.h"
#include "lnPinMapping.h"
#include "lnUSBD.h"
#include "lnUSBD_priv.h"


/**
 */
lnUsbDevice::lnUsbDevice(int port)
{
}
/**
 */
lnUsbDevice::~lnUsbDevice()
{
}
/**
 */
bool lnUsbDevice::init()
{
   
    return true;
}
/**
 */
bool lnUsbDevice::power(bool onoff)
{
   
    return true;
}
/**
 */
bool lnUsbDevice::irqEnabled(bool onoff)
{


    return true;
}
/**
 */
bool lnUsbDevice::registerEventHandler(const lnUsbEventHandler *h)
{
   
    return true;
}

/**

*/
void lnUsbDevice::hwReset()
{

}
// EOF
