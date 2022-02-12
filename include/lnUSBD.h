

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
        bool      irqEnabled(bool onoff);
        bool      registerInterruptHandler(lnUsbIrqHandler *h);
        bool      setAddress(int address);
        bool      wakeUpHost();
public:
        void      rxIrq();
        void      txIrq();
        void      wakeUpIrq();        
protected:
        int       _instance;
        lnUsbIrqHandler *_handler;
};