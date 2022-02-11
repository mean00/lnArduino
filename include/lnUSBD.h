

#include "lnArduino.h"

class lnUsbIrqHandler
{
public:
    virtual void endPointInterrupt()=0;
};

class lnUsbDevice
{
public:
                  lnUsbDevice(int port=0);
        virtual   ~lnUsbDevice();
        bool      init();
        bool      power(bool onoff);
        bool      enableIrq();
        bool      disableIrq();
        bool      registerInterruptHandler(lnUsbIrqHandler *h);
protected:
        int       _instance;
        lnUsbIrqHandler *_handler;
};