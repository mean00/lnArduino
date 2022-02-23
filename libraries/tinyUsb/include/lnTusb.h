

#pragma once

class lnTUSB
{
public:
              lnTUSB();
      virtual ~lnTUSB();
      void    init(int nbDescriptorLine, const char **deviceDescriptor);
      void    start();
      void    stop();
public:      
      void    task();
protected:
      bool     _running;
      const char **_deviceDescriptor;
      int       _nbDeviceDescriptor;
      
public: // dont use these
      uint16_t const* getDeviceDescriptor(int index) ;
};