

#pragma once

#include "usbd.h"
#include "class/cdc/cdc_device.h"

class lnTUSB
{
public:
              lnTUSB();
      virtual ~lnTUSB();
      void    init(int nbDescriptorLine, const char **deviceDescriptor);
      void    setConfiguration(const uint8_t *hsConfiguration, const uint8_t *fsConfiguration,const tusb_desc_device_t *desc,const tusb_desc_device_qualifier_t *qual)
      {
          _hs=hsConfiguration;
          _fs=fsConfiguration;
          _descDevice=desc;
          _qualifier=qual;
      }
      const uint8_t  *getHSConfiguration() {return _hs;}
      const uint8_t  *getFSConfiguration() {return _fs;}
      const tusb_desc_device_t  *getDeviceDescriptor() {return _descDevice;}
      const tusb_desc_device_qualifier_t *getQualifier() {return _qualifier;}
      void    start();
      void    stop();
public:      
      void    task();
protected:
      bool     _running;
      const char **_deviceDescriptor;
      int       _nbDeviceDescriptor;
      const tusb_desc_device_qualifier_t *_qualifier;
      const tusb_desc_device_t *_descDevice;
      const uint8_t *_hs,*_fs;
      
public: // dont use these
      uint16_t const* getDeviceDescriptor(int index) ;
      
};