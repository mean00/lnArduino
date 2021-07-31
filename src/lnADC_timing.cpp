/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnPeripheral_priv.h"
#include "lnADC.h"
#include "lnADC_priv.h"
#include "lnBasicTimer.h"
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

}
/**
 * 
 */
lnTimingAdc::~lnTimingAdc()
{
}
/**
 * 
 * @param timer
 * @param channel
 * @param fq
 * @return 
 */
bool     lnTimingAdc::setSource( int timer, int channel, int fq)
{
    LN_ADC_Registers *adc=lnAdcDesc[_instance].registers;
    _fq=fq;
    _channel=channel;
    _timer=timer;
    int source=-1;
    switch(_timer*10+_channel)
    {
        case 0 : source=LN_ADC_CTL1_ETSRC_SOURCE_T0CH0;break;
        case 1 : source=LN_ADC_CTL1_ETSRC_SOURCE_T0CH1;break;
        case 2 : source=LN_ADC_CTL1_ETSRC_SOURCE_T0CH2;break;
        case 11 : source=LN_ADC_CTL1_ETSRC_SOURCE_T1CH1;break;
        case 20 : source=LN_ADC_CTL1_ETSRC_SOURCE_T2TRGO;break;
        case 33 : source=LN_ADC_CTL1_ETSRC_SOURCE_T3CH3;break;
        default:
            xAssert(0);
            break;
    }
    // set source
    uint32_t src=adc->CTL1&LN_ADC_CTL1_ETSRC_MASK;
    src |=LN_ADC_CTL1_ETSRC_SET(source);
    adc->CTL1=src;
    //
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
bool     lnTimingAdc::multiRead( int nbSamplePerChannel, int nbPins, lnPin *pins, int *output) 
{
    LN_ADC_Registers *adc=lnAdcDesc[_instance].registers;       
    xAssert(_fq>0);
    // Program DMA
    _dma.beginTransfer();
    _dma.attachCallback(lnTimingAdc::dmaDone_,this);
    _dma.doPeripheralToMemoryTransferNoLock(nbSamplePerChannel*nbPins, (uint16_t *)output,(uint16_t *)&( adc->RDATA),  false);
    //
    _dmaSem.take();
    // go !
    adc->CTL1&=~LN_ADC_CTL1_DMA;
    _dma.endTransfer();    
    return false;
}

// EOF