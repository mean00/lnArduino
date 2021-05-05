/**
 
 */
#include "lnArduino.h"
#include "lnDma.h"
#include "gd32vf103_dma.h"

static lnDMA *_lnDmas[2][7]={ {NULL,NULL,NULL,NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL,NULL,NULL,NULL}};

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
            dma_memory_width_config(_dma,_channel, source);
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

bool lnDMA::doMemoryToPeripheralTransfer(int count, const uint16_t *source,  bool repeat)
{
    dma_memory_address_config(_dma,_channel,(uint32_t)(&source));
    if(repeat)
        dma_memory_increase_disable(_dma,_channel);
    else
        dma_memory_increase_enable(_dma,_channel);
    dma_transfer_number_config(_dma,_channel, count);
    dma_channel_enable(_dma,_channel);
    
    // wait for it to be over
    while(0)
    {
       if(1==dma_flag_get(_dma,_channel,DMA_INT_FTF))
       {
           break;
       }
    }
    xDelay(2);
    
    return true;
}