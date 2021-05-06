/**
 
 */
#include "lnArduino.h"
#include "lnDma.h"
#include "gd32vf103_dma.h"

#define z0(x) DMA0_Channel##x##_IRQn
#define z1(x) DMA1_Channel##x##_IRQn

static lnDMA *_lnDmas[2][7]={ {NULL,NULL,NULL,NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL,NULL,NULL,NULL}};
static const IRQn  _dmaIrqs[2][7]= { { z0(0),z0(1),z0(2), z0(3),z0(4),z0(5),z0(6)},{ z1(0),z1(1),z1(2),z1(3),z1(4),IRQn_Type(0),IRQn_Type(0)}}; // Warning DMA CHANNEL5/6 is not available
static const uint32_t _dmas[2]={DMA0,DMA1};
static const dma_channel_enum _channels[7]={DMA_CH0,DMA_CH1,DMA_CH2,DMA_CH3,DMA_CH4,DMA_CH5,DMA_CH6};

/**
 * 
 * @param v
 * @return 
 */
static uint32_t memoryWidth(int v)
{
    switch(v)
    {
        case 8: return  DMA_MEMORY_WIDTH_8BIT;break;
        case 16: return DMA_MEMORY_WIDTH_16BIT;break;
        case 32: return DMA_MEMORY_WIDTH_32BIT;break;
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
        case 8: return  DMA_PERIPHERAL_WIDTH_8BIT;break;
        case 16: return DMA_PERIPHERAL_WIDTH_16BIT;break;
        case 32: return DMA_PERIPHERAL_WIDTH_32BIT;break;
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
    _channelInt=dmaChannel;
    _dmaInt=dmaEngine;
    
    _dma=_dmas[dmaEngine];
    _channel=_channels[dmaChannel];
    _type=type;
    _irq=_dmaIrqs[dmaEngine][dmaChannel];
    
    
    
    dma_deinit(_dma,_channel);
    dma_channel_disable(_dma,_channel);
    uint32_t sw;
    uint32_t source,target;
    switch(type)
    {
        case DMA_MEMORY_TO_PERIPH:
        {
            source=memoryWidth(sourceWidth);
            target=peripheralWidth(targetWidth);
            dma_memory_width_config(_dma,_channel,source);
            dma_periph_width_config(_dma,_channel,target);
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
 */
lnDMA::~lnDMA()
{
    _lnDmas[_dmaInt][_channelInt]=NULL;
     dma_deinit(_dma,_channel);
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
    dma_transfer_direction_config(_dma,_channel, DMA_MEMORY_TO_PERIPHERAL);
    DMA_CHPADDR(_dma, _channel) = (uint32_t)target;
    dma_memory_address_config(_dma,_channel,(uint32_t)(source));
    if(repeat)
        dma_memory_increase_disable(_dma,_channel);
    else
        dma_memory_increase_enable(_dma,_channel);
    dma_transfer_number_config(_dma,_channel, count);
        
    // enable IRQ
    // Start DMA
    dma_flag_clear(_dma,_channel, DMA_FLAG_G);
    dma_interrupt_enable(_dma,_channel,DMA_INT_FTF  );
    eclic_enable_interrupt(_dmaIrqs[_dmaInt][_channelInt]);        
    dma_channel_enable(_dma,_channel);
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
    dma_interrupt_flag_clear(_dmas[dma],_channels[channel],DMA_INT_FTF |DMA_INT_ERR);
    // then call handler
    lnDMA *l=_lnDmas[dma][channel];
    xAssert(l);
    l->invokeCallback();
}


// EOF