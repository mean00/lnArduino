
#include "lnArduino.h"
#include "lnTusb.h"
#include "tusb.h"
#include "device/dcd.h"
#include "lnUSBD.h"

#define USBD_STACK_SIZE configMINIMAL_STACK_SIZE
// local prototypes
void dcd_handle_bus_reset(void);
void dcd_ep_ctr_handler(void);
void usb_device_task(void *param);
void dcd_ep_ctr_rx_handler(uint32_t wIstr);
void dcd_ep_ctr_tx_handler(uint32_t wIstr);
void cdc_task(void *params);
lnUsbDevice *_usbDevice = NULL;
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
            case UsbResume: dcd_event_bus_signal(0, DCD_EVENT_RESUME, true); break;
            case UsbSuspend: dcd_event_bus_signal(0, DCD_EVENT_SUSPEND, true); break;
            case UsbTransferRxCompleted: dcd_ep_ctr_rx_handler(endPoint);
            case UsbTransferTxCompleted: dcd_ep_ctr_tx_handler(endPoint); break;
            default: xAssert(0); break;
        };
    }
};

static const tusbEventHandler myEventHandler;

void dcd_remote_wakeup(uint8_t rhport)
{
    (void)rhport;
    _usbDevice->wakeUpHost();
}
// USB Device Driver task
// This top level thread process all usb events and invoke callbacks
void usb_device_task(void *param)
{
    (void)param;
    // This should be called after scheduler/kernel is started.
    // Otherwise it could cause kernel issue since USB IRQ handler does use RTOS queue API.
  
}

/**

*/
lnTUSB::lnTUSB()
{
  _running=true;
}
/**

*/
lnTUSB::~lnTUSB()
{

}
/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/

void lnTUSB::init()
{
      _usbDevice = new lnUsbDevice(0);
      _usbDevice->init();
      _usbDevice->registerEventHandler(&myEventHandler);
}
static void tusbTrampoline(void *a)
{
  lnTUSB *t=(lnTUSB *)a;
  t->task();
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
    // TODO implement dcd_edpt_close_all()
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    //(void) remote_wakeup_en;
    // xTimerChangePeriod(blinky_tm, pdMS_TO_TICKS(BLINK_SUSPENDED), 0);
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    // xTimerChangePeriod(blinky_tm, pdMS_TO_TICKS(BLINK_MOUNTED), 0);
}

// EOF
