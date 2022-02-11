/*
 *  (C) 2022 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"

#include "lnUSBD.h"
#include "lnUSBD_priv.h"
#include "lnPinMapping.h"

#include "tusb_option.h"
#include "tusb_config.h"
#include "tusb_common.h"
#include "tusb_types.h"

LN_USBD_Registersx     *aUSBD0=(LN_USBD_Registersx *)(LN_USBD0_ADR);

/**
*/
lnUsbDevice::lnUsbDevice(int port)
{
  xAssert(port==0);
  _handler=NULL;
  init();
}
/**
*/
lnUsbDevice::~lnUsbDevice()
{
  power(false);
}
/**
*/
bool     lnUsbDevice:: init()
{
  // first disable interrupt
  irqEnabled(false);    
  // give it a clock...
  lnPeripherals::disable(Peripherals::pUSB);  
  // 48 Mhz input to usb
  lnPeripherals::enableUsb48Mhz();
  return true;
}
/**
*/
bool      lnUsbDevice::power(bool onoff)
{ 
  if(onoff)
  {
    lnPeripherals::enable(Peripherals::pUSB);   
  }else
  {
    lnPeripherals::disable(Peripherals::pUSB);
  }
  return true;
}
/**
*/
bool      lnUsbDevice::irqEnabled(bool onoff)
{ 
  if(onoff)
  {
    lnEnableInterrupt(LN_IRQ_USB_HP_CAN_TX);
    lnEnableInterrupt(LN_IRQ_USB_LP_CAN_RX0);
  }else
  {
    lnDisableInterrupt(LN_IRQ_USB_HP_CAN_TX);
    lnDisableInterrupt(LN_IRQ_USB_LP_CAN_RX0);    
  }
  return true;
}
/**
*/
bool      lnUsbDevice::registerInterruptHandler(lnUsbIrqHandler *h)
{
    _handler=h; 
    return true;
}
/**
*/
bool      lnUsbDevice:: setAddress(int address)
{
  xAssert(0);
  return true;
}

// EOF
