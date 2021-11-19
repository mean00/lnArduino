/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnBasicTimer.h"
#include "lnBasicTimer_priv.h"
#include "lnPinMapping.h"


lnBasicDelayTimer        *btimerInstances[2]={NULL,NULL};
extern LN_BTimers_Registers *aBTimers[];

/**
 * 
 * @param timer
 * @param channel
 */
lnBasicDelayTimer::lnBasicDelayTimer(int timer) : lnBasicTimer(timer)
{
    xAssert(!btimerInstances[timer]);
    btimerInstances[timer]=this;
    _handler=NULL;
    _cookie=NULL;
}

/**
 */
lnBasicDelayTimer::~lnBasicDelayTimer()
{
    disable();
    disableInterrupt();
    _handler=NULL;
    _cookie=NULL;    
    btimerInstances[_timer]=NULL;
}

/**
 * 
 */
void lnBasicDelayTimer::enableInterrupt()
{
    LN_BTimers_Registers *t=aBTimers[_timer];
    t->INTF &=~LN_BTIMER_INTF_UPIF;
}
/**
 * 
 */
void lnBasicDelayTimer::disableInterrupt()
{
    LN_BTimers_Registers *t=aBTimers[_timer];
    t->INTF |=LN_BTIMER_INTF_UPIF;
}
/**
 * 
 * @param handler
 * @param cookie
 */
void lnBasicDelayTimer::setInterrupt(lnBasicTimerIrq *handler, void *cookie)
{
    _handler=handler;
    _cookie=cookie;
}
/**
 * 
 * @param id
 */
 void lnBasicDelayTimer::interruptHandler(int id)
{
    lnBasicDelayTimer *instance=btimerInstances[id];
    xAssert(instance);
    instance->irq();
}
/**
 * 
 */
void lnBasicDelayTimer::irq(void)
{
    disableInterrupt();
    disable();
    xAssert(_handler);
    _handler(_cookie);
}

/**
 * 
 */
void TIMER5_IRQHandler()
{
    lnBasicDelayTimer::interruptHandler(0);
}
/**
 * 
 */
void TIMER6_IRQHandler()
{
    lnBasicDelayTimer::interruptHandler(1);
}
//EOF