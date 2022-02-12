/* 

  Derived fromt the example from tinyUsb
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include "lnArduino.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "tusb.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "timers.h"
#include "lnUSBD.h"

// Increase stack size when debug log is enabled
#define USBD_STACK_SIZE    (3*configMINIMAL_STACK_SIZE/2) * (CFG_TUSB_DEBUG ? 2 : 1)

// static task for cdc
#define CDC_STACK_SZIE      configMINIMAL_STACK_SIZE

#if 0
  #define DEBUG_ENTER() Logger("-%s\n",__PRETTY_FUNCTION__ )
#else
  #define DEBUG_ENTER() {}
#endif
static lnUsbDevice *_usbd=NULL;

void usb_device_task(void* param);
void cdc_task(void* params);

extern "C" void dcd_edpt_xfer(uint8_t rhport, uint8_t ep_addr, uint8_t * buffer, uint16_t total_bytes);


static const tusb_desc_endpoint_t ep0OUT_desc =
{
  .bLength          = sizeof(tusb_desc_endpoint_t),
  .bDescriptorType  = TUSB_DESC_ENDPOINT,

  .bEndpointAddress = 0x00,
  .bmAttributes     = { .xfer = TUSB_XFER_CONTROL },
  .wMaxPacketSize   = CFG_TUD_ENDPOINT0_SIZE,
  .bInterval        = 0
};

static const tusb_desc_endpoint_t ep0IN_desc =
{
  .bLength          = sizeof(tusb_desc_endpoint_t),
  .bDescriptorType  = TUSB_DESC_ENDPOINT,

  .bEndpointAddress = 0x80,
  .bmAttributes     = { .xfer = TUSB_XFER_CONTROL },
  .wMaxPacketSize   = CFG_TUD_ENDPOINT0_SIZE,
  .bInterval        = 0
};

// One of these for every EP IN & OUT, uses a bit of RAM....
typedef struct
{
  uint8_t * buffer;
  // tu_fifo_t * ff;  // TODO support dcd_edpt_xfer_fifo API
  uint16_t total_len;
  uint16_t queued_len;
  uint16_t pma_ptr;
  uint8_t max_packet_size;
  uint8_t pma_alloc_size;
} xfer_ctl_t;


extern "C" int Logger_C(const char *fmt,...)
{
  char str[64];
  
  va_list args;
  va_start(args, fmt);
  vsnprintf(str, 63,fmt, args);
  va_end(args);      
  Logger(str);
  return 0;
}

/**
*/

void tinyUsbInit()
{
  DEBUG_ENTER();
  // Create a task for tinyusb device stack
  (void) xTaskCreate( usb_device_task, "usbd", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES-1,  NULL);

  // Create CDC task
  (void) xTaskCreate( cdc_task, "cdc", CDC_STACK_SZIE, NULL, configMAX_PRIORITIES-2,  NULL);

}


// USB Device Driver task
// This top level thread process all usb events and invoke callbacks
void usb_device_task(void* param)
{
  DEBUG_ENTER();
  (void) param;

  // This should be called after scheduler/kernel is started.
  // Otherwise it could cause kernel issue since USB IRQ handler does use RTOS queue API.
  tusb_init();

  // RTOS forever loop
  while (1)
  {
    // tinyusb device task
    tud_task();
  }
}
#if 0
// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  DEBUG_ENTER();
  (void) remote_wakeup_en;
  
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  DEBUG_ENTER();
}
#endif
//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
void cdc_task(void* params)
{
  DEBUG_ENTER();
  (void) params;

  // RTOS forever loop
  while ( 1 )
  {
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_connected() )
    {
      // There are data available
      if ( tud_cdc_available() )
      {
        uint8_t buf[64];

        // read and echo back
        uint32_t count = tud_cdc_read(buf, sizeof(buf));
        (void) count;

        // Echo back
        // Note: Skip echo by commenting out write() and write_flush()
        // for throughput test e.g
        //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
        tud_cdc_write(buf, count);
        tud_cdc_write_flush();
      }
    }

    // For ESP32-S2 this delay is essential to allow idle how to run and reset wdt
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
  DEBUG_ENTER();
  (void) itf;
  (void) rts;

  // TODO set some indicator
  if ( dtr )
  {
    // Terminal connected
  }else
  {
    // Terminal disconnected
  }
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
  DEBUG_ENTER();
  (void) itf;
}

//---------------------------------
//
//---------------------------------
#define STFSDEV_EP_COUNT 4
extern "C"
{
void dcd_init(uint8_t rhport)
{
  DEBUG_ENTER();
  _usbd=new lnUsbDevice(rhport);
  _usbd->power(true); 
  _usbd->init();
}  
void dcd_int_enable(uint8_t rhport)  
{
  DEBUG_ENTER();
   _usbd->irqEnabled(true);
}
void dcd_int_disable(uint8_t rhport)  
{
    DEBUG_ENTER();
   _usbd->irqEnabled(false);
}
void dcd_set_address(uint8_t rhport, uint8_t dev_addr)
{  
    DEBUG_ENTER();
    dcd_edpt_xfer(rhport, tu_edpt_addr(0, TUSB_DIR_IN), NULL, 0); //reply
   //usbd->setAddress(dev_addr);
}
void dcd_remote_wakeup(uint8_t rhport)
{ 
    DEBUG_ENTER();
    _usbd->wakeUpHost();
}
//-------------
void dcd_edpt_open(uint8_t rhport, tusb_desc_endpoint_t const * desc_edpt)
{
    DEBUG_ENTER();
    xAssert(0);
}  
/**
*/
void dcd_edpt_xfer(uint8_t rhport, uint8_t ep_addr, uint8_t * buffer, uint16_t total_bytes)
{ 
    DEBUG_ENTER();
    xAssert(0);
}  
/**
*/
void dcd_edpt_stall(uint8_t rhport, uint8_t dev_addr)
{
    DEBUG_ENTER();
    xAssert(0);
}
/**
*/
void dcd_edpt_clear_stall(uint8_t rhport, uint8_t dev_addr)
{
    DEBUG_ENTER();
    xAssert(0);
}
/**
*/
void dcd_edpt_close_all(uint8_t rhport)
{
    DEBUG_ENTER();
    xAssert(0);
}

}
// EOF
