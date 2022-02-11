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
  xAssert(0);
  return false;
}
/**
*/
bool      lnUsbDevice::power(bool onoff)
{ 
  xAssert(0);
  return false;
}
/**
*/
bool      lnUsbDevice::enableIrq()
{ 
  xAssert(0);
  return false;
}
/**
*/
bool     lnUsbDevice:: disableIrq()
{
  xAssert(0);
  return false;
}
/**
*/
bool      lnUsbDevice::registerInterruptHandler(lnUsbIrqHandler *h)
{
    _handler=h; 
    return true;
}
extern "C"
{
void dcd_int_enable(uint8_t rhport)  
{
    xAssert(0);
}
void dcd_init(uint8_t rhport)
{
    xAssert(0);
}  
void dcd_edpt_open(uint8_t rhport, tusb_desc_endpoint_t const * desc_edpt)
{
    xAssert(0);
}  
void dcd_remote_wakeup(uint8_t rhport)
{ 
    xAssert(0);
}
void dcd_edpt_xfer(uint8_t rhport, uint8_t ep_addr, uint8_t * buffer, uint16_t total_bytes)
{ 
    xAssert(0);
}  
void dcd_set_address(uint8_t rhport, uint8_t dev_addr)
{
    xAssert(0);
}
/**
*/
void dcd_edpt_stall(uint8_t rhport, uint8_t dev_addr)
{
    xAssert(0);
}
/**
*/

void dcd_edpt_close_all(uint8_t rhport)
{
    xAssert(0);
}
void dcd_edpt_clear_stall(uint8_t rhport, uint8_t dev_addr)
{
    xAssert(0);
}
}
// EOF
