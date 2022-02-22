/*
  Derived from CDC example
  Original copyright below

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
 #if 0
#include "lnArduino.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tusb.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

// Increase stack size when debug log is enabled
#define CDC_STACK_SZIE configMINIMAL_STACK_SIZE
#define USBD_STACK_SIZE (3 * configMINIMAL_STACK_SIZE / 2) * (CFG_TUSB_DEBUG ? 2 : 1)
#include "device/dcd.h"
#include "lnUSBD.h"
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

void tinyUsbInit()
// int main(void)
{
    _usbDevice = new lnUsbDevice(0);
    // Create a task for tinyusb device stack
    (void)xTaskCreate(usb_device_task, "usbd", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES - 1,
                      NULL); //, &usb_device_taskdef);
    // Create CDC task
    (void)xTaskCreate(cdc_task, "cdc", CDC_STACK_SZIE, NULL, configMAX_PRIORITIES - 2,
                      NULL); // cdc_stack, &cdc_taskdef);
}

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
    _usbDevice->init();
    _usbDevice->registerEventHandler(&myEventHandler);
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

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
void cdc_task(void *params)
{
    (void)params;

    // RTOS forever loop
    while (1)
    {
        // connected() check for DTR bit
        // Most but not all terminal client set this when making connection
        // if ( tud_cdc_connected() )
        {
            // There are data available
            if (tud_cdc_available())
            {
                uint8_t buf[64];

                // read and echo back
                uint32_t count = tud_cdc_read(buf, sizeof(buf));
                (void)count;

                // Echo back
                // Note: Skip echo by commenting out write() and write_flush()
                // for throughput test e.g
                //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
                tud_cdc_write(buf, count);
                tud_cdc_write_flush();
            }
        }
        xDelay(10);
    }
}

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
    (void)itf;
    (void)rts;

    // TODO set some indicator
    if (dtr)
    {
        // Terminal connected
    }
    else
    {
        // Terminal disconnected
    }
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
    (void)itf;
}
// EOF
#endif