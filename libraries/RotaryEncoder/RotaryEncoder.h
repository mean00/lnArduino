
#include "MapleFreeRTOS1000_pp.h"

#pragma once
/**
 */
class lnRotary
{
public:
  enum EVENTS
  {
    EVENT_NONE=0,
    SHORT_PRESS=1,
    LONG_PRESS=2,
    ROTARY_CHANGE=4
  };
  
  
                    lnRotary(int pinPush,int pinA,int pinB );
        void        start();
        int         getCount();
        EVENTS      readEvent();
        EVENTS      waitForEvent(int timeout=0); // timeout=0 => dont wait
        
public:        
        void        rotaryInterrupt();
        void        pushInterrupt();
protected:
        int          _count;
        int          _pinA,_pinB,_pinPush;
        int           _event;
        uint32_t      _lastRead;
        uint32_t      _down; // time down was detected
        xFastEventGroup   _events;
        unsigned char _state;
        int           process();
};
