#include "lnArduino.h"
#include "lnUsbStack.h"
#include "tusb.h"
#include "device/dcd.h"
#include "lnUsbCDC.h"

lnUsbCDC *_cdc_instances[2]={NULL,NULL};

/**
*/
lnUsbCDC::lnUsbCDC(int instance)
{
  _instance=instance;
  if(_cdc_instances[_instance]) xAssert(0);
  _cdc_instances[_instance]=this;
  _eventCookie=NULL;
  _eventHandler=NULL;
}
void lnUsbCDC::incomingEvent(lnUsbCDCEvents ev)
{
  if(_eventHandler)
  {
    _eventHandler(_eventCookie,_instance,ev);
  }
}
/**
*/
int lnUsbCDC::read(uint8_t *buffer, int maxSize)
{
  return    tud_cdc_n_read(_instance,buffer, maxSize);
}
/**
*/
int lnUsbCDC::write(uint8_t *buffer, int size)
{
  int sent=0;
  while(size)
  {
    int n= tud_cdc_n_write(_instance,buffer, size);
    if(n<0)
      return -1;
    if(!n)
    {
      tud_cdc_n_write_flush(_instance);
      lnDelay(1); // dont busy loop
      continue;
    }
    buffer+=n;
    size-=n;
    sent+=n;
  }
  return sent;
}
int write(uint8_t *buffer, int maxSize);
/**
*/
void lnUsbCDC::flush()
{
  tud_cdc_n_write_flush(_instance);
}


void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
  lnUsbCDC *me=_cdc_instances[itf];
  xAssert(me);
  if(dtr)
    me->incomingEvent(lnUsbCDC::CDC_SESSION_START);
  else
    me->incomingEvent(lnUsbCDC::CDC_SESSION_END);

}
// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
  lnUsbCDC *me=_cdc_instances[itf];
  xAssert(me);
  me->incomingEvent(lnUsbCDC::CDC_DATA_AVAILABLE);
}

//EOF
