/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnTimer.h"
#include "lnTimer_priv.h"
#include "lnPinMapping.h"

LN_Timers_Registers     *aTimer1=(LN_Timers_Registers *)(LN_TIMER1_ADR);
LN_Timers_Registers     *aTimer2=(LN_Timers_Registers *)(LN_TIMER2_ADR);
LN_Timers_Registers     *aTimer3=(LN_Timers_Registers *)(LN_TIMER3_ADR);
LN_Timers_Registers     *aTimer4=(LN_Timers_Registers *)(LN_TIMER4_ADR);


LN_Timers_Registers *aTimers[4]={(LN_Timers_Registers *)(LN_TIMER1_ADR),(LN_Timers_Registers *)(LN_TIMER2_ADR),(LN_Timers_Registers *)(LN_TIMER3_ADR),(LN_Timers_Registers *)(LN_TIMER4_ADR)};
/**
 * 
 * @param timer
 * @param channel
 */
lnTimer::lnTimer(int timer,int channel)
{
    _timer=timer;
    _channel=channel;
}
/**
 * 
 * @param pin
 */
lnTimer::lnTimer(int pin)
{
    const LN_PIN_MAPPING *pins=pinMappings;
    while(1)
    {
        xAssert(pins->pin!=-1);
        if(pins->pin==pin)
        {
            _timer=pins->timer;
            _channel=pins->timerChannel;
            return;
        }
        pins++;
    }
}

/**
 */
lnTimer::~lnTimer()
{
    
}

/**
 * 
 * @param timer
 */
void lnTimer::setTimerFrequency(int fqInHz)
{
    Peripherals per=pTIMER1;
    per=(Peripherals)((int)per+_timer-1);
    uint32_t clock=lnPeripherals::getClock(per);
    
    int divider=clock/(fqInHz*1024);
    LN_Timers_Registers *t=aTimers[_timer-1];
    t->PSC=divider-1;
    // Set reload to 1024
    t->CAR=1024;
    // Enable with default value
    t->CTL0=1;
    
}

/**
 * 
 * @param timer
 * @param channel
 */
void lnTimer::setPwmMode(int ratio1024)
{
  LN_Timers_Registers *t=aTimers[_timer-1];
  uint32_t chCtl=t->CHCTLs[_channel>>1] ;
  
  chCtl&=~(7<<LN_TIMER_CHTLs_OUTPUT_CHxCOMCTL_SHIFT(_channel));
  chCtl&=~(3<<LN_TIMER_CHTLs_OUTPUT_CHxMS_SHIFT(_channel));
  chCtl|=6<<LN_TIMER_CHTLs_OUTPUT_CHxCOMCTL_SHIFT(_channel); // PWM
  chCtl|=0<<LN_TIMER_CHTLs_OUTPUT_CHxMS_SHIFT(_channel);
  
  t->CHCTLs[_channel>>1]=chCtl;
    
  t->CHCVs[_channel] =ratio1024; // A/R
  t->CHCTL2 |=LN_TIMER_CHTL2_CHxP(_channel);
}
/**
 * 
 */
void lnTimer::enable()
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    t->CHCTL2 |=LN_TIMER_CHTL2_CHxEN(_channel); // basic enable, active high
}
/**
 * 
 */
void lnTimer::disable()
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    t->CHCTL2 &=~(LN_TIMER_CHTL2_CHxEN(_channel)); // basic enable, active high

}

/**
 * 
 * @param ratioBy100
 */
void lnTimer::setChannelRatio(int ratioBy100)
{
    xAssert(0);
}

//EOF