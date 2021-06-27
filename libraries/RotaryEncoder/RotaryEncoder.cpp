#include "lnArduino.h"
#include "Rotary.h"
#include "RotaryEncoder.h"

static lnRotary *current=NULL;
#define THRESHOLD           3        // 3ms
#define TIME_LONG_PRESS     1000     // 2sec
#define TIME_SHORT_PRESS    8


 /**
  */
 
static void myInterrupt(lnPin pin,void *arg)
{
    lnRotary *w=(lnRotary *)arg;
    w->rotaryInterrupt();
}
static void myPushInterrupt(lnPin pin,void *arg )
{
    lnRotary *w=(lnRotary *)arg;
    w->pushInterrupt();
}
/**
 * 
 */
void lnRotary::pushInterrupt()
{
    bool state=lnDigitalRead(_pinPush);
    uint32_t m=millis();
    if((m-_lastRead)<THRESHOLD) return;
    if(!state) // down
    {
        _down=m;
        return;
    }else
    {
        uint32_t time=m-_down;
        if(time>TIME_LONG_PRESS)
        {
                _events.setEvents(LONG_PRESS);
        }else
        {
            if(time>TIME_SHORT_PRESS)
            {
                _events.setEvents(SHORT_PRESS);
            }
        }
    }
}

 /**
  */
 void lnRotary::rotaryInterrupt()
 {
     switch(_rotary->process())
     {
        case DIR_CCW:  _count++;_events.setEvents(ROTARY_CHANGE);break;
        case DIR_CW:   _count--;_events.setEvents(ROTARY_CHANGE);break;
        default:       break;
     }
 }
/**
 */
 lnRotary::lnRotary(int pinA,int pinB, int pinPush ) 
 {
    current=this;
    _count=0;    
    _event=NONE;
    _down=0;
    _lastRead=0;    
    _pinA=pinA;
    _pinB=pinB;
    _pinPush=pinPush;
    pinMode(_pinA,lnINPUT_PULLUP); 
    pinMode(_pinB,lnINPUT_PULLUP); 
    pinMode(_pinPush,lnINPUT_PULLUP);
    _rotary=new Rotary(pinA,pinB);
    
 }
 
 /**
  * 
  */
 void        lnRotary::start()
 {
    noInterrupts(); 
    lnExtiAttachInterrupt(_pinA, LN_EDGE_BOTH,myInterrupt,this);
    lnExtiAttachInterrupt(_pinB,LN_EDGE_BOTH, myInterrupt,this);
    lnExtiAttachInterrupt(_pinPush,LN_EDGE_BOTH,myPushInterrupt, this );
    lnExtiEnableInterrupt(_pinA);
    lnExtiEnableInterrupt(_pinB);
    lnExtiEnableInterrupt(_pinPush);
    interrupts();
    _rotary->begin(true);
     
 }
 /*
  */
 int          lnRotary::getCount()
 {
    noInterrupts();
    int c=_count;
    _count=0;
    interrupts();
    return c;
 }
/**
 * 
 * @return 
 */
lnRotary::EVENTS      lnRotary::readEvent()
{
    return (EVENTS)_events.readEvents(  SHORT_PRESS |     LONG_PRESS |    ROTARY_CHANGE);
}
/**
 * 
 * @return 
 */
lnRotary::EVENTS      lnRotary::waitForEvent()
{
    return (EVENTS)_events.waitEvents(  SHORT_PRESS |     LONG_PRESS |    ROTARY_CHANGE);
}
 // EOF

