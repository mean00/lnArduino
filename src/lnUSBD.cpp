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
static uint8_t         *_usbd_buffer=NULL;
static lnUsbDevice     *_usbInstance=NULL;
/**
*/
lnUsbDevice::lnUsbDevice(int port)
{
  xAssert(port==0);
  _handler=NULL;
  _usbInstance=this;  
}
/**
*/
lnUsbDevice::~lnUsbDevice()
{
  power(false);
  _usbInstance=NULL;
}
/**
*/
bool     lnUsbDevice:: init()
{
  // first disable interrupt
  irqEnabled(false);    
  // 48 Mhz input to usb
  lnPeripherals::enableUsb48Mhz();  
  // Switch pins... PA11 and PA12
  lnPinMode(PA12,lnINPUT_FLOATING); // D+
  lnPinMode(PA11,lnINPUT_FLOATING); // D-
  // Clear all interrupts
  aUSBD0->USBD_CTL&=~0xff; // disable all interrupts  
  aUSBD0->USBD_DADDR=0; // address 0, disabled
  aUSBD0->USBD_CTL=LN_USBD_CTL_SETRST;  // reset
  lnDelayUs(100);
  aUSBD0->USBD_CTL=LN_USBD_CTL_CLOSE; // clear reset, go to closed dstate  
  if(!_usbd_buffer)
  {
    _usbd_buffer=new uint8_t[512];
  }
  memset(_usbd_buffer,0,512);
  // Set buffer address
  uint32_t asLong=(uint32_t )_usbd_buffer;
  xAssert(!(asLong&7)); // aligned normally  already  
  aUSBD0->USBD_BADDR=asLong;   
  // And go, enable most IT
#define GG(x) LN_USBD_CTL_##x  
  aUSBD0->USBD_CTL= GG(RSTIE)+GG(SOFIE)+GG(ESOFIE)+GG(ERRIE)+GG(PMOUIE)+GG(STIE)+GG(WKUPIE);
  return true;
}
/**
*/
bool      lnUsbDevice::power(bool onoff)
{ 
  if(onoff)
  {
    lnPeripherals::enable(Peripherals::pUSB);   
    lnPeripherals::reset(Peripherals::pUSB);   
    
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
    lnEnableInterrupt(LN_IRQ_USBWAKEUP);
    
  }else
  {
    lnDisableInterrupt(LN_IRQ_USB_HP_CAN_TX);
    lnDisableInterrupt(LN_IRQ_USB_LP_CAN_RX0);    
    lnDisableInterrupt(LN_IRQ_USBWAKEUP);
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

bool lnUsbDevice::wakeUpHost()
{
  aUSBD0->USBD_CTL|= LN_USBD_CTL_RSREQ; // clear suspend
  return true;
}

/**
*/
bool      lnUsbDevice:: setAddress(int address)
{
  aUSBD0->USBD_DADDR=0x80+(address&0x7f); // address 0, enabled
  return true;
}
/**
*/
void USB_TX_IRQHandler()
{
  xAssert(_usbInstance);
  _usbInstance->txIrq();
}
/**
*/
void USB_RX_IRQHandler()
{
  xAssert(_usbInstance);
  _usbInstance->rxIrq();
}
void USB_WAKEUP_IRQHandler()
{
  xAssert(_usbInstance);
  _usbInstance->wakeUpIrq();
}
/**
*/
void      lnUsbDevice:: rxIrq()
{
  xAssert(0);
}
void      lnUsbDevice:: txIrq()
{
  xAssert(0);
}
void      lnUsbDevice:: wakeUpIrq()
{
  xAssert(0);
}
// EOF
