#include "hello.h"
#include "include/lnUsbCDC.h"
#include "include/lnUsbStack.h"
#include "lnArduino.h"
#include "lnBMP_usb_descriptor.h"
#define LED LN_SYSTEM_LED
/**
 */
void serialInit();

#define MEVENT(x)                                                                                                      \
    case lnUsbStack::USB_##x:                                                                                          \
        Logger(#x);
void helloUsbEvent(void *cookie, lnUsbStack::lnUsbStackEvents event)
{
    switch (event)
    {
        MEVENT(CONNECT)
        break;
        MEVENT(DISCONNECT)

        break;
        MEVENT(SUSPEND)
        break;
        MEVENT(RESUME)
        break;
    default:
        xAssert(0);
        break;
    }
}

void setup()
{
    Logger("Starting lnTUSB Test\n");
    pinMode(LED, OUTPUT);
    pinMode(PB8, OUTPUT);
    pinMode(PB9, OUTPUT);

    // Prepare USB
    Logger("Starting CDC \n");
    lnUsbStack *usb = new lnUsbStack;
    usb->init(7, descriptor);
    usb->setConfiguration(desc_hs_configuration, desc_fs_configuration, &desc_device, &desc_device_qualifier);
    usb->setEventHandler(NULL, helloUsbEvent);

    serialInit();
    usb->start();
}

void loop()
{
    while (1)
    {
        lnDelayMs(1000);
        digitalToggle(LED);
        Logger("*\n");
    }
}
// EOF
