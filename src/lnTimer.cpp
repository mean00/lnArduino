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
    LN_Timers_Registers *t=aTimers[_timer-1];
    Peripherals per=pTIMER1;
    per=(Peripherals)((int)per+_timer-1);
    uint32_t clock=lnPeripherals::getClock(per);
    // If ABP1 prescale=1, clock*=2 ???? see 5.2 in GD32VF103
    clock=clock*2;
    // disable
    t->CTL0&=~LN_TIMER_CTL0_CEN;
    
    int divider=(clock+fqInHz*512-1)/(fqInHz*1024);
    int preDiv=0;
    while(divider>65535)
    {
        preDiv++;
        divider=divider/2;
    }
    if(preDiv>2) preDiv=2;
    
    uint32_t ctl0=t->CTL0;
    ctl0&=~(3<<LN_TIMER_CTL0_CKDIV_SHIFT);
    ctl0|=preDiv<<LN_TIMER_CTL0_CKDIV_SHIFT;
    
    if(!divider) divider=1;
    t->PSC=divider-1;
    // Set reload to 1024
    t->CAR=1024;
    // Enable with default value
    ctl0|=LN_TIMER_CTL0_CEN;
    t->CTL0=ctl0;
    
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
#if 0  
  t->CHCTL2 |=LN_TIMER_CHTL2_CHxP(_channel);
#else
  t->CHCTL2 &=~(LN_TIMER_CHTL2_CHxP(_channel)); // active low
#endif
}
/**
 * 
 */
void lnTimer::enable()
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    t->CNT=0;
    // ?? t->SWEV |= LN_TIMER_SWEVG_UPG;
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
void lnTimer::setChannelRatio(int ratio1024)
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    t->CHCVs[_channel] =ratio1024; // A/R
}
/**
 * 
 * @param ratio1024
 */
void lnTimer::singleShot(int durationMs, bool down)
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    xAssert(durationMs<=100);
   // noInterrupts();
    disable();
    setTimerFrequency(8); // 1024 ticks =  125 ms, 1 tick=0.122 ms
    setPwmMode(durationMs*8);  // 
    if(down)
        t->CHCTL2 |=LN_TIMER_CHTL2_CHxP(_channel); // by default on, it will be stopped when the timer is done
    else
        t->CHCTL2 &=~(LN_TIMER_CHTL2_CHxP(_channel));
    enable();
    //interrupts();
    xDelay(durationMs+10);
    disable();    
}

//--
void lnAdcTimer::setTimerFrequency(int fqInHz)
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    Peripherals per=pTIMER1;
    per=(Peripherals)((int)per+_timer-1);
    uint32_t clock=lnPeripherals::getClock(per);
    // If ABP1 prescale=1, clock*=2 ???? see 5.2 in GD32VF103
    clock=clock*2;
    // disable
    t->CTL0&=~LN_TIMER_CTL0_CEN;
    
    int divider=clock/(fqInHz);
    
    int intDiv=divider/65536;
    int fracDiv=divider & 65535;
    
    if(!intDiv) intDiv=1;
    t->PSC=intDiv-1;
    // Set reload to 0
    t->CAR=fracDiv-1;
    
    uint32_t chCtl=t->CHCTLs[_channel>>1] ;
  
    chCtl&=~(7<<LN_TIMER_CHTLs_OUTPUT_CHxCOMCTL_SHIFT(_channel));
    chCtl&=~(3<<LN_TIMER_CHTLs_OUTPUT_CHxMS_SHIFT(_channel));
    chCtl|=6<<LN_TIMER_CHTLs_OUTPUT_CHxCOMCTL_SHIFT(_channel); // PWM
    chCtl|=0<<LN_TIMER_CHTLs_OUTPUT_CHxMS_SHIFT(_channel);

    t->CHCTLs[_channel>>1]=chCtl;

    t->CHCVs[_channel] =2; // A/R
    t->CHCTL2 &=~(LN_TIMER_CHTL2_CHxP(_channel)); // active low
    // Enable with default value
    uint32_t ctl0=t->CTL0;
    ctl0|=LN_TIMER_CTL0_CEN;
    t->CTL0=ctl0;

}
//EOF