
#include "lnArduino.h"
#include "lnTusb.h"
#include "tusb.h"
#include "device/dcd.h"
#include "lnUSBD.h"

#define USBD_STACK_SIZE configMINIMAL_STACK_SIZE
// local prototypes
void dcd_handle_bus_reset(void);
void dcd_ep_ctr_handler(void);
void dcd_ep_ctr_rx_handler(uint32_t wIstr);
void dcd_ep_ctr_tx_handler(uint32_t wIstr);
void cdc_task(void *params);
lnUsbDevice *_usbDevice = NULL;
static lnTUSB *lnTusbInstance =NULL;
//--------------------------------------------------------------------+
// Main
//--------------------------------------------------------------------+

class tusbEventHandler : public lnUsbEventHandler
{
  public:
    virtual void event(const lnUsbdEvent ev, const uint32_t endPoint) const
    {
        switch (ev)
        {
            case UsbReset:
                            dcd_handle_bus_reset();
                            dcd_event_bus_reset(0, TUSB_SPEED_FULL, true);
                            break;
            case UsbResume: 
                            dcd_event_bus_signal(0, DCD_EVENT_RESUME, true); 
                            break;
            case UsbSuspend: 
                            dcd_event_bus_signal(0, DCD_EVENT_SUSPEND, true); 
                            break;
            case UsbTransferRxCompleted: 
                            dcd_ep_ctr_rx_handler(endPoint);
                            break;
            case UsbTransferTxCompleted: 
                            dcd_ep_ctr_tx_handler(endPoint); 
                            break;
            default: 
                            xAssert(0); 
                            break;
        };
    }
};

static const tusbEventHandler myEventHandler;

void dcd_remote_wakeup(uint8_t rhport)
{
    (void)rhport;
    _usbDevice->wakeUpHost();
}

/**

*/
lnTUSB::lnTUSB()
{
  _running=true;
  lnTusbInstance=this;
  _qualifier=NULL;
  _hs=NULL;
  _fs=NULL;
  _descDevice=NULL;
  _eventHandler=NULL;
  _cookie=NULL;
}
/**

*/
lnTUSB::~lnTUSB()
{
  lnTusbInstance=NULL;
}
/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/

void lnTUSB::init(int nb,const char **desc)
{
      _usbDevice = new lnUsbDevice(0);
      _usbDevice->init();
      _usbDevice->registerEventHandler(&myEventHandler);
      _deviceDescriptor=desc;
      _nbDeviceDescriptor=nb;
}
static void tusbTrampoline(void *a)
{
  lnTUSB *t=(lnTUSB *)a;
  t->task();
}
/**

*/
void    lnTUSB::sendEvent(lnTUSB_Events ev)
{
    if(!_eventHandler) return;
    _eventHandler(_cookie,ev);
}

/**

*/
void    lnTUSB::start()
{
  (void)xTaskCreate(tusbTrampoline, "usbd", USBD_STACK_SIZE, this, configMAX_PRIORITIES - 1,      NULL); 
}
/**

*/
void    lnTUSB::stop()
{
    _running=false;
    
}
/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/

void    lnTUSB::task()
{
    tusb_init();
    while (_running)
    {
        tud_task();
    }
    vTaskDelete(NULL);
}
//----

void dcd_int_disable(uint8_t rhport)
{
    _usbDevice->irqEnabled(false);
}
void dcd_int_enable(uint8_t rhport)
{
    _usbDevice->irqEnabled(true);
}

void dcd_init(uint8_t rhport)
{
}

void dcd_edpt_close_all(uint8_t rhport)
{
    (void)rhport;    
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
/**
USB_CONNECT,
USB_DISCONNECT,
*/
void tud_suspend_cb(bool remote_wakeup_en)
{
  xAssert(lnTusbInstance);
  lnTusbInstance->sendEvent(lnTUSB::USB_SUSPEND);
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  xAssert(lnTusbInstance);
  lnTusbInstance->sendEvent(lnTUSB::USB_RESUME);
}

void tud_mount_cb(void)
{
  xAssert(lnTusbInstance);
  lnTusbInstance->sendEvent(lnTUSB::USB_CONNECT);
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  xAssert(lnTusbInstance);
  lnTusbInstance->sendEvent(lnTUSB::USB_DISCONNECT);
}
/**
*/
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  xAssert(lnTusbInstance);
  return lnTusbInstance->getDeviceDescriptor(index);
}

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* lnTUSB::getDeviceDescriptor(int index)
{
static uint16_t _desc_str[32];

  xAssert(_deviceDescriptor);
  xAssert(_nbDeviceDescriptor);  
  uint16_t *p=&(_desc_str[0]);
  if ( !index )
  {    
    *p++ = (TUSB_DESC_STRING << 8 ) | 4;
    memcpy(p, _deviceDescriptor[0], 2);
    return _desc_str;
  }
  // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
  // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
  if(index>=_nbDeviceDescriptor) return NULL;
  const char* str = _deviceDescriptor[index];
  // Cap at max char
  int mx = strlen(str);
  if ( mx > 31 ) mx = 31;
  
  *p++ = (TUSB_DESC_STRING << 8 ) | (2*mx + 2);
  for(int i=0; i<mx; i++)
      *p++ = str[i];
  return _desc_str;
}
//--//

uint8_t const* tud_descriptor_other_speed_configuration_cb(uint8_t index)
{
  (void) index; // for multiple configurations
  return (tud_speed_get() == TUSB_SPEED_HIGH) ?  lnTusbInstance->getFSConfiguration() : lnTusbInstance->getHSConfiguration();
}

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index; // for multiple configurations
  return (tud_speed_get() == TUSB_SPEED_HIGH) ?  lnTusbInstance->getHSConfiguration() : lnTusbInstance->getFSConfiguration();
}

// Invoked when received GET DEVICE QUALIFIER DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete.
// device_qualifier descriptor describes information about a high-speed capable device that would
// change if the device were operating at the other speed. If not highspeed capable stall this request.
uint8_t const* tud_descriptor_device_qualifier_cb(void)
{
  xAssert(lnTusbInstance);
  return (uint8_t *)lnTusbInstance->getQualifier();
}

uint8_t const * tud_descriptor_device_cb(void)
{
  xAssert(lnTusbInstance); // tusb_desc_device_t
  return (uint8_t const *) lnTusbInstance->getDeviceDescriptor();
}


// EOF
