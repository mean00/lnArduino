/**
 * @file rp_dma.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 * https://github.com/rossihwang/pico_dma_uart/blob/main/dma_uart.cpp
 */
#include "lnArduino.h"
#include "ln_rp_dma.h"
#include "stdint.h"

/**
 * @brief Construct a new ln Rp D M A::ln Rp D M A object
 *
 * @param type
 * @param dmaChannel
 * @param sourceWith
 * @param targetWidth
 */
lnRpDMA::lnRpDMA(DmaTransferType type, int dmaChannel, int transferWidth)
{
    memset(&_dmaconfig, 0, sizeof(_dmaconfig));

    _control = 0;
    _channelInt = dmaChannel;
    _type = type;
    _transferWidth = transferWidth;
    setWordSize(_transferWidth);
}
/**
 * @brief Destroy the lnRpDMA object
 *
 */
lnRpDMA::~lnRpDMA()
{
    cancelTransfer();
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
 * @param target
 * @param repeat
 * @param circularMode
 * @param bothInterrutps
 * @return true
 * @return false
 */
bool lnRpDMA::doMemoryToPeripheralTransferNoLock(int count, const uint16_t *source, int dreq, bool repeat,
                                                 bool circularMode, bool bothInterrutps)
{
    channel_config_set_read_increment(&_dmaconfig, !repeat);
    channel_config_set_write_increment(&_dmaconfig, false);
    channel_config_set_dreq(&_dmaconfig, dreq); // use DREQ

    xAssert(0);
    return false;
}
/**
 * @brief
 *
 * @param count
 * @param target
 * @param source
 * @param circularMode
 * @param bothInterrutps
 * @return true
 * @return false
 */
bool lnRpDMA::doPeripheralToMemoryTransferNoLock(int count, const uint16_t *target, int dreq, bool circularMode,
                                                 bool bothInterrutps)
{
    setWordSize(_transferWidth);
    channel_config_set_read_increment(&_dmaconfig, false);
    channel_config_set_write_increment(&_dmaconfig, true);
    channel_config_set_dreq(&_dmaconfig, dreq); // use DREQ
    xAssert(0);
    return false;
}
/**
 * @brief
 *
 * @param full
 * @param half
 * @return true
 * @return false
 */
bool lnRpDMA::setInterruptMask(bool full, bool half)
{
    xAssert(0);
    return false;
}
/**
 * @brief
 *
 * @param prio
 */
void lnRpDMA::setPriority(DmaPriority prio) // prio between 0 (low) to 3 (ultra high)
{
    xAssert(0);
}
/**
 * @brief
 *
 */
void lnRpDMA::beginTransfer()
{
    channel_config_set_enable(&_dmaconfig, true);
    dma_channel_start(_channelInt);
}
/**
 * @brief
 *
 * @return uint32_t
 */
uint32_t lnRpDMA::getCurrentCount()
{
    xAssert(0);
}
/**
 * @brief
 *
 */
void lnRpDMA::endTransfer()
{
    dma_channel_abort(_channelInt);
}
/**
 * @brief
 *
 */
void lnRpDMA::cancelTransfer()
{
    dma_channel_abort(_channelInt);
}
/**
 * @brief
 *
 * @param sourceWordSize
 * @param targetWordSize
 */
void lnRpDMA::setWordSize(int sourceWordSize)
{
    enum dma_channel_transfer_size size;
    switch (sourceWordSize)
    {
    case 8:
        size = DMA_SIZE_8;
        break;
    case 32:
        size = DMA_SIZE_32;
        break;
    default:
        xAssert(0);
        break;
    }
    channel_config_set_transfer_data_size(&_dmaconfig, size);
}
/**
 * @brief
 *
 */
void lnRpDMA::invokeCallback()
{
    DmaInterruptType typ = DMA_INTERRUPT_FULL;
    _cb(_cookie, typ);
}

// EOF