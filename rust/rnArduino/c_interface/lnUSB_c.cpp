#pragma once
#include "lnArduino.h"
#include "lnUsbStack.h"

#define WRAPME(x) ((lnUsbStack *)x)

/**
*/
lnusb_c *   lnusb_create(uint32_t instance)
{
    return new lnUsbStack(instance);
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
void lnusb_setConfiguration(lnusb_c *inst, const uint8_t *hsConfiguration, const uint8_t *fsConfiguration,
                          const tusb_desc_device_t *desc, const tusb_desc_device_qualifier_t *qual)
{
    WRAPME(inst)->setConfiguration(hsConfiguration, fsConfiguration, desc, qual);
}
/**
*/
void lnusb_setEventHandler(lnusb_c *inst, void *cookie, lnUsbStackEventHandler *ev)
{
    WRAPME(inst)->setEventHandler(cookie, ev);
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