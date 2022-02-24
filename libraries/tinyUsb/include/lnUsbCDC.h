

#pragma once


class lnUsbCDC
{
public:
      enum lnUsbCDCEvents
      {
        CDC_DATA_AVAILABLE,
        CDC_SESSION_START,
        CDC_SESSION_END
      };
      typedef void lnUsbCDCEventsHandler(void *cookie, int interface,lnUsbCDCEvents event);
public:
              lnUsbCDC(int instance);
          int read(uint8_t *buffer, int maxSize);
          int write(uint8_t *buffer, int maxSize);
          void setHandler(lnUsbCDCEventsHandler *h, void *cookie)
          {
              _eventHandler=h;
              _eventCookie=cookie;
          }
// internal API
          void incomingEvent(lnUsbCDCEvents ev);
protected:
        int _instance;
        lnUsbCDCEventsHandler *_eventHandler;
        void *_eventCookie;
};
