/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnTimer.h"
#include "lnTimer_priv.h"
#include "lnPinMapping.h"

extern LN_Timers_Registers *aTimers[4];

/**
 * 
 * @param pin
 */
lnDmaTimer::lnDmaTimer(int pin) : lnTimer(pin)
{
    const LN_PIN_MAPPING *pins=pinMappings;
    int dmaChannel=-1;
    int dmaEngine=-1;
    _timer=-1;
    while(1)
    {
        xAssert(pins->pin!=-1); //t2c1
        if(pins->pin==pin)
        {
            _timer=pins->timer;
            _channel=pins->timerChannel;
            break;
        }
        pins++;
    }
    if(_timer==-1) xAssert(0);
    switch(_timer*10+_channel)
    {
        case 20:     dmaChannel=5;dmaEngine=0;break;
        case 02:     dmaChannel=5;dmaEngine=0;break;
        case 22:     dmaChannel=2;dmaEngine=0;break;
        case 21:     dmaChannel=1;dmaEngine=0;break;
        default:     xAssert(0);break;
        
    }
               
    _dma=new lnDMA(lnDMA::DMA_MEMORY_TO_PERIPH,dmaEngine,dmaChannel,16,16);
}
/**
 * 
 */
lnDmaTimer:: ~lnDmaTimer()
{
    if(_dma)
    {
        delete _dma;
        _dma=NULL;
    }
}
/**
 * 
 * @param frequency
 * @return 
 */
bool    lnDmaTimer::pwmSetup(int frequency)
{
    LN_Timers_Registers *t=aTimers[_timer-1];
    setTickFrequency(frequency);
    _rollover=t->CAR+1;
    setMode(lnTimerModePwm1);
    return true;
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
    _cb=cb;
    return true;
}
/**
 * 
 * @param nbSample
 * @param data
 * @return 
 */


static void _dmaCallback(void *c)
{
    lnDmaTimer *t=(lnDmaTimer *)c;
    t->dmaInterrupt();
}
/**
 * 
 */
void lnDmaTimer::dmaInterrupt()
{
    xAssert(0);
}
/**
 * 
 * @param nbSample
 * @param data
 * @return 
 */
bool    lnDmaTimer::start(int nbSample, uint16_t *data)
{   
    LN_Timers_Registers *t=aTimers[_timer-1];
    
    _dma->beginTransfer(); // lock dma
    _dma->attachCallback(_dmaCallback,this);
    // circular, no repeat, both interrupt
                                                                                    //  bool repeat,bool circular,bool  bothInterrupts)
    _dma->doMemoryToPeripheralTransferNoLock(nbSample,(uint16_t *)data,(uint16_t *)&(t->CHCVs[_channel]),false,true,true);                
    // PWM mode 1
    t->CHCVs[_channel]=_rollover/2;
    t->DMAINTEN|=1<<(_channel+9);
    enable();
    while(1)
    {
        xDelay(100);
    }
    t->DMAINTEN&=~(1<<(_channel+9));
    _dma->endTransfer();
    return false;
}
/**
 * 
 * @param fq
 * @return 
 */
  bool    lnDmaTimer::setTickFrequency(int fqInHz)
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

#warning FIXME we assume no prescaler needed
    t->PSC=0;

    if(!divider) divider=1;
    t->CAR=divider-1;
    return true;
  }

// EOF
