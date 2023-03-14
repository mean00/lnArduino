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
void lnUsbCDC::incomingEvent(lnUsbCDCEvents ev,uint32_t payload)
{
  if(_eventHandler)
  {
    _eventHandler(_eventCookie,_instance,ev,payload);
  }
}
/**

*/
void lnUsbCDC::encodingChanged(const void *newEncoding)
{
  const cdc_line_coding_t *enc=(const cdc_line_coding_t *)newEncoding;
  Logger("CDC Encoding changed speed=%d \n",enc->bit_rate);
  incomingEvent(lnUsbCDC::CDC_SET_SPEED,enc->bit_rate);
}

/**
*/
int lnUsbCDC::read(uint8_t *buffer, int maxSize)
{
  return    tud_cdc_n_read(_instance,buffer, maxSize);
}
/**
*/
int lnUsbCDC::write(const uint8_t *buffer, int size)
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
  while(  tud_cdc_n_write_flush(_instance) != 0)
  {
    
  }
}
/**

*/
void lnUsbCDC::clear_input_buffers()
{
  tud_cdc_n_read_flush(_instance);
}

//---- hooks ---
#define PREMABLE(itf) lnUsbCDC *me=_cdc_instances[itf]; xAssert(me);
#define FORWARD_EVENT(ifg,event) PREMABLE(itf);  me->incomingEvent(event);
//---

//void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char);
//void tud_cdc_tx_complete_cb(uint8_t itf);
//void tud_cdc_send_break_cb(uint8_t itf, uint16_t duration_ms);
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding)
{
  PREMABLE(itf);
  return me->encodingChanged(p_line_coding);
}


//---
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
  Logger("DTR=%d RTS=%d\n");
  PREMABLE(itf);
  if(dtr)
    me->incomingEvent(lnUsbCDC::CDC_SESSION_START);
  else
    me->incomingEvent(lnUsbCDC::CDC_SESSION_END);
}
//---
// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
  FORWARD_EVENT(itf,lnUsbCDC::CDC_DATA_AVAILABLE);
}

//EOF
