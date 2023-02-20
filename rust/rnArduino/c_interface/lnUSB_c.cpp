#pragma once
#include "lnArduino.h"
#include "include/lnUsbStack.h"
#include "lnUSB_c.h"

#define WRAPME(x) ((lnUsbStack *)x)

// this is done by the C side
const uint8_t *usb_hsConfiguration = NULL;
const uint8_t *usb_fsConfiguration= NULL;
const tusb_desc_device_t *usb_des_device= NULL;
const tusb_desc_device_qualifier_t *usb_qualifier= NULL;

void lnusb_set_global_configuration(const uint8_t *hsConfiguration, const uint8_t *fsConfiguration,
                          const tusb_desc_device_t *desc, const tusb_desc_device_qualifier_t *qual);


void lnusb_set_global_configuration(const uint8_t *hsConfiguration, const uint8_t *fsConfiguration,
                          const tusb_desc_device_t *desc, const tusb_desc_device_qualifier_t *qual)
{
    usb_hsConfiguration =     hsConfiguration;
    usb_fsConfiguration = fsConfiguration;
    usb_des_device = desc; 
    usb_qualifier = qual;
}

/**
*/
lnusb_c *   lnusb_create(uint32_t instance)
{
    return (lnusb_c *)(new lnUsbStack());
}
/**
*/
void        lnusb_delete(lnusb_c *c)
{
    delete WRAPME(c);
    c=NULL;
}
/**
*/
void lnusb_init(lnusb_c *inst, int nbDescriptorLine, const char **deviceDescriptor)
{
    WRAPME(inst)->init(nbDescriptorLine, deviceDescriptor);
}
/**
*/
void lnusb_setConfiguration(lnusb_c *inst)
{
    xAssert(usb_hsConfiguration);
    xAssert(usb_fsConfiguration);
    xAssert(usb_des_device);
    xAssert(usb_qualifier);

    WRAPME(inst)->setConfiguration(usb_hsConfiguration, usb_fsConfiguration, usb_des_device, usb_qualifier);
}
/**
*/
void lnusb_setEventHandler(lnusb_c *inst, void *cookie, lnUsbStackEventHandler *ev)
{
    lnUsbStack::lnUsbStackEventHandler *ev_c = (lnUsbStack::lnUsbStackEventHandler *)ev;

    WRAPME(inst)->setEventHandler(cookie, ev_c);
}
/**
*/
void lnusb_start(lnusb_c *inst )
{
    WRAPME(inst)->start();
}
/**
*/
void lnusb_stop(lnusb_c *inst)
{
    WRAPME(inst)->stop();
}

          
// EOF