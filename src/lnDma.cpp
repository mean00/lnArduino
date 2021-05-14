/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnDma.h"
#include "lnDma_priv.h"
#include "lnPeripheral_priv.h"

#define z0(x) DMA0_Channel##x##_IRQn
#define z1(x) DMA1_Channel##x##_IRQn

static lnDMA *_lnDmas[2][7]={ {NULL,NULL,NULL,NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL,NULL,NULL,NULL}};
static const IRQn  _dmaIrqs[2][7]= { { z0(0),z0(1),z0(2), z0(3),z0(4),z0(5),z0(6)},{ z1(0),z1(1),z1(2),z1(3),z1(4),IRQn_Type(0),IRQn_Type(0)}}; // Warning DMA CHANNEL5/6 is not available
static const uint32_t _dmas[2]={LN_DMA0_ADR,LN_DMA1_ADR};
/**
 */

DMA_struct *dma0=(DMA_struct *)LN_DMA0_ADR;

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
    
    DMA_struct *d=(DMA_struct *)_dma;
    DMA_channels *c=d->channels+_channelInt;
    
    
    c->CNT&=LN_DMA_CHAN_KEEP_MASK; // disable
    
    // Clear interrupts & flags
    uint32_t mask=0xf<<dmaChannel;
    d->INTC|=mask;
    
    
    uint32_t sw;
    uint32_t source,target;
    switch(type)
    {
        case DMA_MEMORY_TO_PERIPH:
        {
            source=memoryWidth(sourceWidth);
            target=peripheralWidth(targetWidth);
            _control|=source;
            _control|=target;
            _control|=LN_DMA_CHAN_DIR_M2P;
            _control|=LN_DMA_CHAN_PRIO_HIGH;
            break;
        }
        default:
            xAssert(0);
            break;                
    }
    _cb=NULL;
    _cookie=NULL;
    
    if(_lnDmas[_dmaInt][_channelInt]) xAssert(0);
    _lnDmas[_dmaInt][_channelInt]=this;  
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
 * @param count
 * @param source
 * @param repeat
 * @return 
 */
bool lnDMA::doMemoryToPeripheralTransfer(int count, const uint16_t *source,const uint16_t *target,  bool repeat)
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

// fine tune + interrutps    
    if(!repeat)
    {
        control|=LN_DMA_CHAN_MINCREASE; // increase address
    }
        
    control|=LN_DMA_CHAN_ERRIE+LN_DMA_CHAN_TFTFIE; // error and transmit complete interrupt
    
    c->CTL=control|LN_DMA_CHAN_ENABLE; // GO!       
    eclic_enable_interrupt(_irq);        
    return true;
}
/**
 * 
 */
void    lnDMA::invokeCallback()
{
    xAssert(_cb);
    _cb(_cookie);
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
    d->INTC=(3)<<(4*channel); // clear global+Transmit    
    // Disable DMA
    d->channels[channel].CTL&=~LN_DMA_CHAN_ENABLE;
    // disable interrupt
    eclic_disable_interrupt(_dmaIrqs[dma][channel]);       
    // then call handler
    lnDMA *l=_lnDmas[dma][channel];
    xAssert(l);
    l->invokeCallback();
}


// EOF