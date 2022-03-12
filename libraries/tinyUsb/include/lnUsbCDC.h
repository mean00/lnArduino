

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
          int write(const uint8_t *buffer, int maxSize);
          void flush();
          void setEventHandler(lnUsbCDCEventsHandler *h, void *cookie)
          {
              _eventHandler=h;
              _eventCookie=cookie;
          }
// internal API
          void incomingEvent(lnUsbCDCEvents ev);
          void encodingChanged(const void *newEncoding); // this is a const  cdc_line_coding_t * in disguise

protected:
        int _instance;
        lnUsbCDCEventsHandler *_eventHandler;
        void *_eventCookie;
};
