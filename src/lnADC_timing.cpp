/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnPeripheral_priv.h"
#include "lnADC.h"
#include "lnADC_priv.h"
#include "lnTimer.h"
#include "lnDma.h"
#include "lnPinMapping.h"


//------------------------------------------------------------------


/**
 * 
 * @param instance
 */
lnTimingAdc::lnTimingAdc(int instance)  : lnBaseAdc(instance), _dma( lnDMA::DMA_PERIPH_TO_MEMORY,   lnAdcDesc[_instance].dmaEngine, lnAdcDesc[_instance].dmaChannel,  16,16)
{
    xAssert(instance==0); // we need DMA so only ADC0
    _timer=-1;
    _channel=-1;
    _fq=-1;
    _adcTimer=NULL;
    _nbPins=-1;

}
/**
 * 
 */
lnTimingAdc::~lnTimingAdc()
{
    if(_adcTimer) delete _adcTimer;
    _adcTimer=NULL;
}
/**
 * 
 * @param timer
 * @param channel
 * @param fq
 * @return 
 */
bool     lnTimingAdc::setSource( int timer, int channel, int fq,int nbPins, lnPin *pins)
{
    LN_ADC_Registers *adc=lnAdcDesc[_instance].registers;
    _fq=fq;
    _nbPins=nbPins;
    _channel=channel;
    _timer=timer;
    int source=-1;
    int timerId=-1,timerChannel=-1;
    
#define SETTIM(c,a,b) case a*10+b: source=LN_ADC_CTL1_ETSRC_SOURCE_##c;timerId=a;timerChannel=b;break;
    switch(_timer*10+_channel)
    {
        SETTIM(T0CH0,0,0)
        SETTIM(T0CH1,0,1)
        SETTIM(T0CH2,0,2)
        SETTIM(T1CH1,1,1)
        SETTIM(T2TRGO,2,0)
        SETTIM(T3CH3,3,3)
        default:
            xAssert(0);
            break;
    }
    // set source
    uint32_t src=adc->CTL1&LN_ADC_CTL1_ETSRC_MASK;
    src |=LN_ADC_CTL1_ETSRC_SET(source);
    adc->CTL1=src;
    //
    if(_adcTimer) delete _adcTimer;
    _adcTimer=new lnAdcTimer(timerId, timerChannel);
    _adcTimer->setTimerFrequency(fq);
    
    // add our channel(s)
    adc->RSQS[0]=((uint32_t)nbPins)<<20;
    xAssert(_nbPins<6);
    uint32_t rsq2=0;
    for(int i=0;i<_nbPins;i++)
    {
        rsq2 |=(adcChannel(pins[i])<<(5*i));
    }
    adc->RSQS[2]=rsq2;    
    return true;
}
/**
 * 
 * @param t
 */
void lnTimingAdc::dmaDone_(void *t)
{
    lnTimingAdc *me=(lnTimingAdc *)t;
    me->dmaDone();
}
/**
 * 
 */
void lnTimingAdc::dmaDone()
{
    _dmaSem.give();
}
/**
 * 
 * @param fq
 * @param nbSamplePerChannel
 * @param nbPins
 * @param pins
 * @param output
 * @return 
 */
bool     lnTimingAdc::multiRead( int nbSamplePerChannel,  int *output) 
{
    LN_ADC_Registers *adc=lnAdcDesc[_instance].registers;       
    xAssert(_fq>0);
    // Program DMA
    _dma.beginTransfer();
    _dma.attachCallback(lnTimingAdc::dmaDone_,this);
    _dma.doPeripheralToMemoryTransferNoLock(nbSamplePerChannel*_nbPins, (uint16_t *)output,(uint16_t *)&( adc->RDATA),  false);
    // go !
    _adcTimer->enable();    
    _dmaSem.take();
    _adcTimer->disable();
    // cleanup
    adc->CTL1&=~LN_ADC_CTL1_DMA;
    _dma.endTransfer();    
    return false;
}

// EOF