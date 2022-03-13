#include "lnArduino.h"
#include "lnSerial.h"

#define LED LN_SYSTEM_LED
/**
 */
void setup()
{
    lnPinMode(LED, lnOUTPUT);
}

int rx=0,tx=0;

class rxTask : public xTask
{
public:
  rxTask(lnSerial *serial) : xTask("RX")
  {
      _ser=serial;
  }
  static void cb(void *cookie, lnSerial::Event ev)
  {
      rxTask *t=(rxTask *)cookie;
      t->eventHandler(ev);
  }
  void eventHandler(lnSerial::Event ev)
  {
      _evGroup->setEvents(1);
  }
  void run()
  {
    _evGroup=new xFastEventGroup;
    _evGroup->takeOwnership();
    _ser->setCallback(cb,this);
    _ser->enableRx(true);
    uint8_t c[4];
    while(1)
    {
      int r=_ser->read(3,c);
      if(r) rx+=r;
      else
      {
        _evGroup->waitEvents(1,10);
      }
    }

  }
  lnSerial *_ser;
  xFastEventGroup *_evGroup;

};

void loop()
{
    //Logger("Starting lnUart Test\n");

    lnSerial serial(2);
    serial.init();
    serial.setSpeed(115200);
    rxTask receiveTask(&serial);
    receiveTask.start();
    while(1)
    {
            serial.transmit(4,(const uint8_t *)"ABCD");
            tx+=4;
            lnDelayMs(100);
            lnDigitalToggle(LED);
            //Logger("*\n");
    }
}
