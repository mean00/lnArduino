/**
 * @file ln_rp_dma.cpp
 * @author mean00
 * @brief 
 * @version 0.1
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "ln_rp_dma.h"
#include "ln_rp_dma_priv.h"
#include "hardware/regs/intctrl.h"
#include "hardware/irq.h"

static lnMutex *dmaMuteces[LN_RP_DMA_CHANNEL_COUNT]={   NULL,NULL,NULL,NULL,
                                                NULL,NULL,NULL,NULL,
                                                NULL,NULL,NULL,NULL};
static lnRpDMA *dmaInstances[LN_RP_DMA_CHANNEL_COUNT] = {   NULL,NULL,NULL,NULL,
                                                NULL,NULL,NULL,NULL,
                                                NULL,NULL,NULL,NULL};

LN_RP_DMA *dmactrl  = (LN_RP_DMA *)LN_RP_DMA_CONTROL;

void dma_irq0_handler()
{
    uint32_t flags = dmactrl->INTS0;
    dmactrl->INTS0=flags; // ack the DMA interrupt
    flags &= dmactrl->INTE0;
    uint32_t copy = flags;
    for(int i=0;i<12 && copy;i++)
    {
        int bit = 1<<i;
        if(copy & bit)
        {
            xAssert( dmaInstances[i])
            dmaInstances[i]->invokeCallback();
        }
        copy&=~bit;
    }    
}
/**
 * @brief 
 * 
 */
void lnRpDmaSysInit()
{
    LN_RP_DMA_channel *dma;
    irq_set_enabled(DMA_IRQ_0, false);
    for(int i=0;i<LN_RP_DMA_CHANNEL_COUNT;i++)
    {
        dmaMuteces[i]=new lnMutex;        
        dma = RP_DMA_CHANNEL(i);
        dma->DMA_CONTROL = 0; // dis
    }
    dmactrl->INTE0=0;
    dmactrl->INTE1=0; // disable all channel's interrupt
    irq_set_exclusive_handler(DMA_IRQ_0, dma_irq0_handler);
    irq_set_enabled(DMA_IRQ_0, true);
}

/**
 * @brief Construct a new ln Rp D M A::ln Rp D M A object
 * 
 * @param type 
 * @param dmaChannel 
 * @param sourceWith 
 * @param targetWidth 
 */
lnRpDMA::lnRpDMA(DmaTransferType type, LN_RP_DMA_DREQ req, int dmaChannel, int transferWidth, DmaPriority prio )
{
    _control = 0;
    _channel = dmaChannel;
    _type = type;    
    _transferWidth = transferWidth;    
    _priority = prio;
    _req=req;
    // now build up default control we'll re use later
    switch(_transferWidth)
    {
        case 8 :  _control |= LN_RP_DMA_CONTROL_SET_DATA_SIZE_8;break;
        case 16 : _control |= LN_RP_DMA_CONTROL_SET_DATA_SIZE_16;break;
        case 32 : _control |= LN_RP_DMA_CONTROL_SET_DATA_SIZE_32;break;            
        default: xAssert(0);break;
    }
    switch(_type)
    {
        case   DMA_MEMORY_TO_PERIPH: _control |= LN_RP_DMA_CONTROL_INCR_READ; break;
        case   DMA_PERIPH_TO_MEMORY: _control |= LN_RP_DMA_CONTROL_INCR_WRITE; break;
        default: xAssert(0);break;
    }
    switch(_priority)
    {
        case     DMA_PRIORITY_LOW :  break;
        case     DMA_PRIORITY_HIGH : _control |=LN_RP_DMA_CONTROL_HIGH_PRIO; break;
        default : xAssert(0);break;
    }
    _control |= LN_RP_DMA_CONTROL_TREQ(_req);
    _control |= LN_RP_DMA_CONTROL_CHAIN_TO(_channel); // chain to self
    //
    dmaInstances[_channel]=this;
    _dma = RP_DMA_CHANNEL(_channel);
    _dma->DMA_CONTROL = _control; // disable it while we are at it
}
/**
 * @brief Destroy the lnRpDMA object
 * 
 */
lnRpDMA::~lnRpDMA()
{
    dmaInstances[_channel]=0;
    _dma->DMA_CONTROL &= ~LN_RP_DMA_CONTROL_ENABLE; 
}
/**
 * @brief 
 * 
 * @param cb 
 * @param cookie 
 */
void lnRpDMA::attachCallback(doneCallback *cb, void *cookie)
{
    _cb = cb;
    _cookie = cookie;
}
/**
 * @brief 
 * 
 */
void lnRpDMA::detachCallback()
{
    _cb = NULL;
    _cookie = NULL;
}
/**
 * @brief 
 * 
 * @param count 
 * @param source 
 * @param repeat 
 * @return true 
 * @return false 
 */
bool  lnRpDMA::doMemoryToPeripheralTransferNoLock(int count, const uint32_t *source, const uint32_t *target, bool repeat)
{
    xAssert(!(_dma->DMA_CONTROL & LN_RP_DMA_CONTROL_ENABLE ));
    _dma->DMA_COUNT=count;
    _dma->DMA_READ=(uint32_t )source;
    _dma->DMA_WRITE=(uint32_t )target;
    uint32_t control = _dma->DMA_CONTROL;
    control &=( LN_RP_DMA_CONTROL_INCR_READ+LN_RP_DMA_CONTROL_INCR_WRITE);
    if(repeat)
    {
         control |=LN_RP_DMA_CONTROL_INCR_READ;
    }    
    _dma->DMA_CONTROL = _control;    
    return true;
}
/**
 * @brief 
 * 
 * @param count 
 * @param target 
 * @return true 
 * @return false 
 */
bool lnRpDMA::doPeripheralToMemoryTransferNoLock(int count, const uint32_t *source, const uint32_t *target)
{
    xAssert(!(_dma->DMA_CONTROL & LN_RP_DMA_CONTROL_ENABLE ));
    _dma->DMA_COUNT=count;
    _dma->DMA_READ=(uint32_t )source;
    _dma->DMA_WRITE=(uint32_t )target;
    uint32_t control = _dma->DMA_CONTROL;
    control &=( LN_RP_DMA_CONTROL_INCR_READ+LN_RP_DMA_CONTROL_INCR_WRITE);
    control |=LN_RP_DMA_CONTROL_INCR_WRITE;
    _dma->DMA_CONTROL = _control;    
    return true;
}
/**
 * @brief 
 * dma_channel_set_irq0_enabled
 */
void lnRpDMA::beginTransfer()
{   
    dmactrl->INTE0 |= (1<<_channel);
    //dmactrl->INTF0 |= (1<<_channel);
    _dma->DMA_CONTROL |= LN_RP_DMA_CONTROL_ENABLE; // this reg is  a trigger, writing it starts DMA
}
/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t lnRpDMA::getCurrentCount()
{
    return _dma->DMA_COUNT;    
}
/**
 * @brief 
 * 
 */
void lnRpDMA::endTransfer()
{
    dmactrl->INTE0 &= ~(1<<_channel);
    _dma->DMA_CONTROL &= ~LN_RP_DMA_CONTROL_ENABLE;
}
/**
 * @brief 
 * 
 */
void lnRpDMA::cancelTransfer()
{
    dmactrl->INTE0 &= ~(1<<_channel);
    _dma->DMA_CONTROL &= ~LN_RP_DMA_CONTROL_ENABLE;    
}
// EOF
