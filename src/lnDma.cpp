/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnDma.h"
#include "lnDma_priv.h"
#include "lnPeripheral_priv.h"

#define z0(x) LN_IRQ_DMA0_Channel##x
#define z1(x) LN_IRQ_DMA1_Channel##x

static lnDMA *_lnDmas[2][7]={ {NULL,NULL,NULL,NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL,NULL,NULL,NULL}};
static const LnIRQ  _dmaIrqs[2][7]= { { z0(0),z0(1),z0(2), z0(3),z0(4),z0(5),z0(6)},{ z1(0),z1(1),z1(2),z1(3),z1(4),LN_IRQ_NONE,LN_IRQ_NONE}}; // Warning DMA CHANNEL5/6 is not available
static const uint32_t _dmas[2]={LN_DMA0_ADR,LN_DMA1_ADR};
static xMutex *dmaMutex[2][7];
/**
 */

DMA_struct *adma0=(DMA_struct *)LN_DMA0_ADR;
DMA_struct *adma1=(DMA_struct *)LN_DMA1_ADR;

/**
 * 
 */
void lnDmaSysInit()
{
    for(int i=0;i<7;i++)
    {
        dmaMutex[0][i]=new xMutex;
        dmaMutex[1][i]=new xMutex;
    }
}

/**
 * 
 * @param v
 * @return 
 */
static uint32_t memoryWidth(int v)
{
    switch(v)
    {
        case 8: return  LN_DMA_CHAN_MWIDTH_8;break;
        case 16: return LN_DMA_CHAN_MWIDTH_16;break;
        case 32: return LN_DMA_CHAN_MWIDTH_32;break;
        default:
            xAssert(0);
            return 0;
    }
}
/**
 * 
 * @param v
 * @return 
 */
static uint32_t peripheralWidth(int v)
{
    switch(v)
    {
        case 8: return  LN_DMA_CHAN_PWIDTH_8;break;
        case 16: return LN_DMA_CHAN_PWIDTH_16;break;
        case 32: return LN_DMA_CHAN_PWIDTH_32;break;
        default:
            xAssert(0);
            return 0;
    }
}
      
/**
 * 
 * @param type
 * @param dmaEngine
 * @param dmaChannel
 * @param sourceWith
 * @param targetWidth
 */
lnDMA::lnDMA(DmaTransferType type, int dmaEngine, int dmaChannel, int sourceWidth, int targetWidth)
{
    _control=0;
    _channelInt=dmaChannel;
    _dmaInt=dmaEngine;
    
    _dma=_dmas[dmaEngine];
    _type=type;
    _irq=_dmaIrqs[dmaEngine][dmaChannel];
    
    _sourceWidth=sourceWidth;
    _targetWidth=targetWidth;
}
/**
 * 
 * @param sourceWordSize
 * @param targetWordSize
 */
void    lnDMA::setWordSize(int sourceWordSize, int targetWordSize)
{
    uint32_t source=0,target=0;
    switch(_type)
    {
        case DMA_MEMORY_TO_PERIPH:
            {
                source=memoryWidth(sourceWordSize);
                target=peripheralWidth(targetWordSize);
                _control&=LN_DMA_CHAN_WIDTH_MASK;
                _control|=source;
                _control|=target;
            }
        break;
        case DMA_PERIPH_TO_MEMORY:
            {
                source=peripheralWidth(sourceWordSize);                
                target=memoryWidth(targetWordSize);
                _control&=LN_DMA_CHAN_WIDTH_MASK;
                _control|=source;
                _control|=target;
            }
        break;        
        default:
            xAssert(0);
            break;
      }
}

/**
 * 
 */
lnDMA::~lnDMA()
{
    DMA_struct *d=(DMA_struct *)_dma;
    DMA_channels *c=d->channels+_channelInt;
    c->CTL=0;
}

/**
 * 
 * @param cb
 * @param cookie
 */
void lnDMA::attachCallback(doneCallback *cb, void *cookie)
{
    _cb=cb;
    _cookie=cookie;
}
/**
 * 
 */
void    lnDMA::detachCallback()
{
    _cb=NULL;
    _cookie=NULL;
}

/**
 * 
 * @param count
 * @param source
 * @param repeat
 * @return 
 */

/**
 * 
 */
void lnDMA::beginTransfer()
{
    dmaMutex[_dmaInt][_channelInt]->lock();
    // Ok now we can configure the dma
    DMA_struct *d=(DMA_struct *)_dma;
    DMA_channels *c=d->channels+_channelInt;
    
    
    c->CNT&=LN_DMA_CHAN_KEEP_MASK; // disable
    
    // Clear interrupts & flags
    uint32_t mask=0xf<<_channelInt;
    d->INTC|=mask;
    
    
    uint32_t sw;
    uint32_t source,target;
    switch(_type)
    {
        case DMA_MEMORY_TO_PERIPH:
        {
            source=memoryWidth(_sourceWidth);
            target=peripheralWidth(_targetWidth);
            _control&=LN_DMA_CHAN_WIDTH_MASK;
            _control|=source;
            _control|=target;
            _control|=LN_DMA_CHAN_DIR_M2P;
            _control|=LN_DMA_CHAN_PRIO_HIGH;
            break;
        }
        case DMA_PERIPH_TO_MEMORY:
        {
            source=peripheralWidth (_sourceWidth);
            target=memoryWidth(_targetWidth);
            _control&=LN_DMA_CHAN_WIDTH_MASK;
            _control|=source;
            _control|=target;
            _control|=LN_DMA_CHAN_DIR_P2M;
            _control|=LN_DMA_CHAN_PRIO_HIGH;
            break;
        }
        default:
            xAssert(0);
            break;                
    }
    
    if(_lnDmas[_dmaInt][_channelInt]) xAssert(0);
    _lnDmas[_dmaInt][_channelInt]=this;     
}
/**
 * 
 */
void lnDMA::cancelTransfer()
{
    DMA_struct *d=(DMA_struct *)_dma;
    DMA_channels *c=d->channels+_channelInt;
    noInterrupts();
    uint32_t control=c->CTL;    
    control &=~LN_DMA_CHAN_ENABLE;
    control &=~(LN_DMA_CHAN_ERRIE+LN_DMA_CHAN_TFTFIE);
    c->CTL=control;
    
    _cb=NULL;
    _cookie=NULL;
    uint32_t mask=0xf<<_channelInt;
    d->INTC|=mask;
    _lnDmas[_dmaInt][_channelInt]=NULL;         
    interrupts(); 
}
void lnDMA::endTransfer()
{
    cancelTransfer();
    dmaMutex[_dmaInt][_channelInt]->unlock();
}
   

/**
 * 
 * @param count
 * @param source
 * @param target
 * @param repeat
 * @return 
 */
bool lnDMA::doMemoryToPeripheralTransferNoLock(int count, const uint16_t *source,const uint16_t *target,  bool repeat,bool circular,bool  bothInterrupts)
{
     // clear pending bits
    DMA_struct *d=(DMA_struct *)_dma;
    d->INTC|=0xff<<_channelInt;
 
    DMA_channels *c=d->channels+_channelInt;
    uint32_t control=c->CTL;    
    control&=LN_DMA_CHAN_KEEP_MASK;

    c->CTL=control; // also disable
    
    control|=_control; // base configuration

    c->PADDR=(uint32_t)target;
    c->MADDR=(uint32_t)source;
    c->CNT=  (uint32_t)count;
    if(!repeat)
        control|=LN_DMA_CHAN_MINCREASE; // increase address

    
    if(circular)
    {
        control|=LN_DMA_CHAN_CMEN; // replay the dma in a loop
        control|=LN_DMA_CHAN_ERRIE; // error  interrupt
        if(bothInterrupts)
        {
            control |=LN_DMA_CHAN_TFTFIE+LN_DMA_CHAN_HTFIE;
        }else
        {
            control&=~LN_DMA_CHAN_TFTFIE;
        }
    }else
    {
        control&=~LN_DMA_CHAN_CMEN; 
        control|=LN_DMA_CHAN_ERRIE+LN_DMA_CHAN_TFTFIE; // error and transmit complete interrupt        
        if(bothInterrupts)
            control|=LN_DMA_CHAN_HTFIE;
    }
        
    
    
    c->CTL=control|LN_DMA_CHAN_ENABLE; // GO!       
    lnEnableInterrupt(_irq);        
    return true;
}
/**
 * 
 * @param count
 * @param target
 * @param source
 * @param repeat
 * @param circularMode
 * @return 
 */
bool    lnDMA::doPeripheralToMemoryTransferNoLock(int count, const uint16_t *target,const uint16_t *source,  bool circular,bool bothInterrupts)
{
     // clear pending bits
    DMA_struct *d=(DMA_struct *)_dma;
    d->INTC|=0xff<<_channelInt;
 
    DMA_channels *c=d->channels+_channelInt;
    uint32_t control=c->CTL;    
    control&=LN_DMA_CHAN_KEEP_MASK;

    c->CTL=control; // also disable
    
    control=_control; // base configuration

    
    c->PADDR=(uint32_t)source;
    c->MADDR=(uint32_t)target;
    c->CNT=  (uint32_t)count;

    control|=LN_DMA_CHAN_MINCREASE; // increase address
    
    if(circular)
    {
        control|=LN_DMA_CHAN_CMEN; // replay the dma in a loop
        control|=LN_DMA_CHAN_ERRIE; // error  interrupt
        if(bothInterrupts)
        {
            control |=LN_DMA_CHAN_TFTFIE+LN_DMA_CHAN_HTFIE;
        }else
        {
            control&=~LN_DMA_CHAN_TFTFIE;
        }
    }else
    {
        control&=~LN_DMA_CHAN_CMEN; 
        control|=LN_DMA_CHAN_ERRIE+LN_DMA_CHAN_TFTFIE; // error and transmit complete interrupt        
        if(bothInterrupts)
            control|=LN_DMA_CHAN_HTFIE;
    }
    c->CTL=control|LN_DMA_CHAN_ENABLE; // GO!       
    lnEnableInterrupt(_irq);        
    return true;
}
/**
 * 
 */
void    lnDMA::invokeCallback()
{
    DmaInterruptType typ;
    DMA_struct *d=(DMA_struct *)_dmas[_dmaInt];
    int pending= d->INTF;
    pending>>=(4*_channelInt);
    if(pending & 2) // full
        typ=DMA_INTERRUPT_FULL;
    else if(pending & 4) // half
        typ=DMA_INTERRUPT_HALF;
    else
        xAssert(0);
    xAssert(_cb);
     d->INTC=(7)<<(4*_channelInt); // clear all
    _cb(_cookie,typ);
}
/**
 * 
 * @param dma
 * @param channel
 */
void dmaIrqHandler(int dma, int channel)
{
    // First clear the interrupt
    DMA_struct *d=(DMA_struct *)_dmas[dma];
    uint32_t status= d->INTF; 
    status>>=(4*channel); // ERR DONE HALFDONE GLOBAL
    if(status & 8) // error bit
    {
        xAssert(0);
    }
    
    
    // Are we in circular mode ?
    if(d->channels[channel].CTL & LN_DMA_CHAN_CMEN)
    {  // ok, is it full or helf interrupt ?
         // then call handler
        lnDMA *l=_lnDmas[dma][channel];
        xAssert(l);
        l->invokeCallback();
        return;
    }
    
    // single shot mode    
    // Disable DMA
    d->channels[channel].CTL&=~LN_DMA_CHAN_ENABLE;
    // disable interrupt
    lnDisableInterrupt(_dmaIrqs[dma][channel]);   

    lnDMA *l=_lnDmas[dma][channel];
    xAssert(l);
    l->invokeCallback();
    return;       
}


// EOF