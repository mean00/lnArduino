/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnTimer.h"
#include "lnTimer_priv.h"
#include "lnPinMapping.h"
#if 0
LN_Timers_Registers     *aTimer1=(LN_Timers_Registers *)(LN_TIMER1_ADR);
LN_Timers_Registers     *aTimer2=(LN_Timers_Registers *)(LN_TIMER2_ADR);
LN_Timers_Registers     *aTimer3=(LN_Timers_Registers *)(LN_TIMER3_ADR);
LN_Timers_Registers     *aTimer4=(LN_Timers_Registers *)(LN_TIMER4_ADR);


#define READ_CHANNEL_CTL(channel)       (  t->CHCTLs[channel>>1])>>(8*(channel&1))
#define WRITE_CHANNEL_CTL(channel,val)  \
    { \
        int shift=8*(channel&1); \
        uint32_t r=t->CHCTLs[(channel)>>1]; \
        r&=0xff<<shift; \
        r|=(val&0xff)<<shift; \
        t->CHCTLs[channel>>1]=r; \
    }

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
void lnTimer::setPwmFrequency(int fqInHz)
{
//--
  LN_Timers_Registers *t=aTimers[_timer-1];
    Peripherals per=pTIMER1;
    per=(Peripherals)((int)per+_timer-1);
    uint32_t clock=lnPeripherals::getClock(per);
    // If ABP1 prescale=1, clock*=2 ???? see 5.2 in GD32VF103
    // disable
    t->CTL0&=~LN_TIMER_CTL0_CEN;
    
    int divider=(clock+(fqInHz*512))/(fqInHz*1024);
    divider*=2;
    int preDiv=2;  
    while(divider>65535)
    {
        preDiv=preDiv*2;
        divider=divider/2;
    }
    if(preDiv>8) preDiv=8;
    
    //
    lnADC_DIVIDER adcDiv;
    switch(preDiv)
    {
        switch(preDiv)
        {
            case 2:  adcDiv=lnADC_CLOCK_DIV_BY_2;break;
            case 4:  adcDiv=lnADC_CLOCK_DIV_BY_4;break;
            case 8:  adcDiv=lnADC_CLOCK_DIV_BY_8;break;
            default: xAssert(0);break;
        }
    }
    lnPeripherals::setAdcDivider(adcDiv);
    uint32_t ctl0=t->CTL0;    
    if(!divider) divider=1;
    t->PSC=divider-1;    
    t->CAR=1024;
}
/**
 * 
 * @param fqInHz
 */
void lnTimer::setTickFrequency(int fqInHz)
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    Peripherals per=pTIMER1;
    per=(Peripherals)((int)per+_timer-1);
    uint32_t clock=lnPeripherals::getClock(per);
    // If ABP1 prescale=1, clock*=2 ???? see 5.2 in GD32VF103
    // disable
    t->CTL0&=~LN_TIMER_CTL0_CEN;
    
    int divider=(clock+fqInHz/2)/(fqInHz);
    divider*=2;
    int preDiv=2;  
    while(divider>65535)
    {
        preDiv=preDiv*2;
        divider=divider/2;
    }
    if(preDiv>8) preDiv=8;
    
    //
    lnADC_DIVIDER adcDiv;
    switch(preDiv)
    {
        switch(preDiv)
        {
            case 2:  adcDiv=lnADC_CLOCK_DIV_BY_2;break;
            case 4:  adcDiv=lnADC_CLOCK_DIV_BY_4;break;
            case 8:  adcDiv=lnADC_CLOCK_DIV_BY_8;break;
            default: xAssert(0);break;
        }
    }
    lnPeripherals::setAdcDivider(adcDiv);
    uint32_t ctl0=t->CTL0;    
    if(!divider) divider=1;
    t->PSC=divider-1;
}
/**
 * 
 * @param timer
 * @param channel
 */
void lnTimer::setPwmMode(int ratio1024)
{
  LN_Timers_Registers *t=aTimers[_timer-1];
  uint32_t chCtl=READ_CHANNEL_CTL(_channel);
  
    chCtl&=LN_TIME_CHCTL0_CTL_MASK;
    chCtl|=LN_TIME_CHCTL0_CTL_PWM1;
    chCtl&=LN_TIME_CHCTL0_MS_MASK;
    chCtl|=LN_TIME_CHCTL0_MS_OUPUT;
  
  WRITE_CHANNEL_CTL(_channel,chCtl)

    
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
    t->CTL0&=~LN_TIMER_CTL0_CEN;
    t->CNT=0;
    // ?? t->SWEV |= LN_TIMER_SWEVG_UPG;
    t->CHCTL2 |=LN_TIMER_CHTL2_CHxEN(_channel); // basic enable, active high
    t->CTL0|=LN_TIMER_CTL0_CEN;
}
/**
 * 
 */
void lnTimer::disable()
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    t->CHCTL2 &=~(LN_TIMER_CHTL2_CHxEN(_channel)); // basic enable, active high
    t->CTL0&=~LN_TIMER_CTL0_CEN;
    t->CNT=0;
    uint32_t chCtl=READ_CHANNEL_CTL(_channel);
    chCtl&=LN_TIME_CHCTL0_CTL_MASK;
    chCtl|=LN_TIME_CHCTL0_CTL_FORCE_LOW;  
    WRITE_CHANNEL_CTL(_channel,chCtl)
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
#if 0
#define SPEEDUP 10
#else
#define SPEEDUP 1
#endif
void lnTimer::singleShot(int durationMs, bool down)
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    xAssert(durationMs<=100);
   // noInterrupts();
    disable();
    setTickFrequency(10*1000*SPEEDUP); // 1 tick=1ms
    t->CAR=10000;
    t->CNT=0;
    
    uint32_t chCtl=READ_CHANNEL_CTL(_channel);
    chCtl&=LN_TIME_CHCTL0_MS_MASK;
    chCtl|=LN_TIME_CHCTL0_MS_OUPUT;
    chCtl&=LN_TIME_CHCTL0_CTL_MASK;
    chCtl|=LN_TIME_CHCTL0_CTL_PWM0;  
    WRITE_CHANNEL_CTL(_channel,chCtl)
    t->CHCVs[_channel] =durationMs*10; // high then low when timer elapsed
    noInterrupts();
    //t->CTL0|=LN_TIMER_CTL0_SPM; // sign
    t->CNT=t->CAR-2;
    t->CTL0|=LN_TIMER_CTL0_CEN;
    t->CHCTL2 |=LN_TIMER_CHTL2_CHxEN(_channel); // basic enable, active high
    interrupts();
    xDelay(durationMs+10);
    disable();        
}

//--
void lnAdcTimer::setPwmFrequency(int fqInHz)
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
    
    uint32_t chCtl=READ_CHANNEL_CTL(_channel);
  
    
    chCtl&=LN_TIME_CHCTL0_CTL_MASK;
    chCtl|=LN_TIME_CHCTL0_CTL_PWM1;
    chCtl&=LN_TIME_CHCTL0_MS_MASK;
    chCtl|=LN_TIME_CHCTL0_MS_OUPUT;
    
    WRITE_CHANNEL_CTL(_channel,chCtl)
    

    t->CHCVs[_channel] =2; // A/R
    t->CHCTL2 &=~(LN_TIMER_CHTL2_CHxP(_channel)); // active low
    
}
#endif
/**
 * 
 * @param pin
 */
lnDmaTimer::lnDmaTimer(int pin) : lnTimer(pin)
{

}
/**
 * 
 */
lnDmaTimer:: ~lnDmaTimer()
{

}
/**
 * 
 * @param frequency
 * @return 
 */
bool    lnDmaTimer::pwmSetup(int frequency)
{
    return false;
}
/**
 * 
 * @return 
 */
int     lnDmaTimer::rollover()
{
    return _rollover;
}
/**
 * 
 * @param cb
 * @return 
 */
bool    lnDmaTimer::attachDmaCallback(lnDmaTimerCallback *cb)
{
    return false;
}
/**
 * 
 * @param nbSample
 * @param data
 * @return 
 */
bool    lnDmaTimer::start(int nbSample, uint16_t *data)
{
    return false;
}
// EOF
