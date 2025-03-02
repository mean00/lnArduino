
#include "device/dcd.h"
#include "lnArduino.h"
#include "lnUSBD.h"
#include "lnUSDB_tinyUSB.h"
#include "lnUsbStack.h"
#include "tusb.h"

#if CFG_TUSB_DEBUG != 0
#define USBD_EXTRA_STACK 500
#else
#define USBD_EXTRA_STACK 0
#endif
#define USB_BASE_STACK_SIZE 256
#define USBD_STACK_SIZE (USB_BASE_STACK_SIZE + USBD_EXTRA_STACK)
// local prototypes
void dcd_handle_bus_reset(void);
void dcd_ep_ctr_handler(void);
void dcd_ep_ctr_rx_handler(int ep);
void dcd_ep_ctr_tx_handler(int ep);
void cdc_task(void *params);
extern lnUsbDevice *_usbDevice;
static lnUsbStack *lnUsbStackInstance = NULL;

void dcd_remote_wakeup(uint8_t rhport)
{
    (void)rhport;
    _usbDevice->wakeUpHost();
}

void dcd_int_disable(uint8_t rhport)
{
    _usbDevice->irqEnabled(false);
}
void dcd_int_enable(uint8_t rhport)
{
    _usbDevice->irqEnabled(true);
}

bool dcd_init(uint8_t rhport, const tusb_rhport_init_t *rh_init)
{
    // Send a reset
    _usbDevice->hwReset();
    return true;
}

void dcd_edpt_close_all(uint8_t rhport)
{
    (void)rhport;
}

// EOF
