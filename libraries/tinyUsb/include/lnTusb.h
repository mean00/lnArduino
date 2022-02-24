

#pragma once

#include "class/cdc/cdc_device.h"
#include "usbd.h"
/**

*/
class lnTUSB
{
  public:
    enum lnTUSB_Events
    {
        USB_CONNECT,
        USB_DISCONNECT,
        USB_SUSPEND,
        USB_RESUME
    };

    typedef void lnTUSB_EventHandler(void *cookie, const lnTUSB_Events event);

  public:
    lnTUSB();
    virtual ~lnTUSB();
    void init(int nbDescriptorLine, const char **deviceDescriptor);
    void setConfiguration(const uint8_t *hsConfiguration, const uint8_t *fsConfiguration,
                          const tusb_desc_device_t *desc, const tusb_desc_device_qualifier_t *qual)
    {
        _hs = hsConfiguration;
        _fs = fsConfiguration;
        _descDevice = desc;
        _qualifier = qual;
    }
    void setEventHandler(void *cookie, lnTUSB_EventHandler *ev)
    {
        _eventHandler = ev;
        _cookie = cookie;
    }
    const uint8_t *getHSConfiguration()
    {
        return _hs;
    }
    const uint8_t *getFSConfiguration()
    {
        return _fs;
    }
    const tusb_desc_device_t *getDeviceDescriptor()
    {
        return _descDevice;
    }
    const tusb_desc_device_qualifier_t *getQualifier()
    {
        return _qualifier;
    }
    void sendEvent(lnTUSB_Events ev);
    void start();
    void stop();

  public:
    void task();

  protected:
    bool _running;
    const char **_deviceDescriptor;
    int _nbDeviceDescriptor;
    const tusb_desc_device_qualifier_t *_qualifier;
    const tusb_desc_device_t *_descDevice;
    const uint8_t *_hs, *_fs;
    lnTUSB_EventHandler *_eventHandler;
    void *_cookie;

  public: // dont use these
    uint16_t const *getDeviceDescriptor(int index);
};
