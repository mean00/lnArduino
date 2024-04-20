/*
 *  (C) 2021/2024 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnDma.h"
#include "lnSPI.h"
#include "lnSPI_bp.h"
#include "lnSPI_priv.h"
/**
 *
 * @param wordSize
 * @param nbBytes
 * @param data
 * @param repeat
 * @return
 */

#define LN_SPI_TIMEOUT 100
#define LN_SPI_LONG_TIMEOUT (1000)

static int nbReq = 0;
static int nbInterrupt = 0;
static int nbSkipped = 0;

/**
 * @brief
 *
 * @param wordSize
 * @param nbTransfer
 * @param data
 * @param repeat
 * @return true
 * @return false
 */
bool lnSPI_bp::dmaWriteInternal(int wordSize, int nbTransfer, const uint8_t *data, bool repeat)
{
    bool r = true;
    sdisable();
    if (!nbTransfer)
        return true;

    // that will clear errror
    updateMode(_regs, lnTxOnly); // tx only

    // 1- Configure DMA
    txDma.beginTransfer();
    txDma.attachCallback(exTxDone, this);
    txDma.setWordSize(wordSize, wordSize);
    txDma.doMemoryToPeripheralTransferNoLock(nbTransfer, (uint16_t *)data, (uint16_t *)&(_regs->DATA), repeat);

    // 2- Configure SPI
    updateMode(_regs, lnTxOnly);     // tx only
    updateDataSize(_regs, wordSize); // 16 bits at a time
    updateDmaTX(_regs, true);        // activate DMA

    // 3- Go!

    nbReq++;
    if (_regs->STAT & LN_SPI_STAT_CONFERR)
    {
        txDma.endTransfer();
        Logger("Conf Error\n");
        return false;
    }
    csOn();
    senable();
    if (false == _done.take(LN_SPI_TIMEOUT)) // 100 ms should be plenty enough!
    {
        r = false;
    }
    else
    {
        waitForCompletion();
    }
    txDma.endTransfer();
    csOff();
    // sdisable();
    updateDmaTX(_regs, false);
    return r;
}

/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @return true
 * @return false
 */
bool lnSPI_bp::blockWrite8(int nbBytes, const uint8_t *data)
{
    return dmaWriteInternal(8, nbBytes, data, false);
}

/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @return true
 * @return false
 */
bool lnSPI_bp::blockWrite8Repeat(int nbBytes, const uint8_t data)
{
    return dmaWriteInternal(8, nbBytes, (uint8_t *)&data, true);
}

/**
 * @brief
 *
 * @param nbWord
 * @param data
 * @return true
 * @return false
 */
bool lnSPI_bp::blockWrite16(int nbWord, const uint16_t *data)
{
    return dmaWriteInternal(16, nbWord, (uint8_t *)data, false);
}

/**
 * @brief
 *
 * @param nbWord
 * @param data
 * @return true
 * @return false
 */
bool lnSPI_bp::blockWrite16Repeat(int nbWord, const uint16_t data)
{
    return dmaWriteInternal(16, nbWord, (uint8_t *)&data, true);
}

/**
 * @brief
 *
 * @param c
 * @param it
 */
void lnSPI_bp::exTxDone(void *c, lnDMA::DmaInterruptType it)
{
    nbInterrupt++;
    auto *i = (lnSPI_bp *)c;
    i->txDone();
}
/**
 * @brief
 *
 * @param wordSize
 * @param nbWord
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool lnSPI_bp::asyncWrite(int wordSize, int nbWord, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    bool r = true;
    _done.tryTake();
    if (!nbWord)
        return true;

    // that will clear errror
    updateMode(_regs, lnTxOnly); // tx only

    // 1- Configure DMA
    txDma.beginTransfer();
    txDma.setWordSize(wordSize, wordSize);
    updateDataSize(_regs, wordSize); // 16 bits at a time
    switch (wordSize)
    {
    case 8:
        return nextWrite8(nbWord, data, cb, cookie, repeat);
    case 16:
        return nextWrite16(nbWord, (const uint16_t *)data, cb, cookie, repeat);
    default:
        xAssert(0);
        break;
    }
}
/**
 * @brief
 *
 * @param nbWord
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool lnSPI_bp::asyncWrite16(int nbWord, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    return asyncWrite(16, nbWord, (const uint8_t *)data, cb, cookie, repeat);
}

/**
 * @brief
 *
 * @param nbWord
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool lnSPI_bp::asyncWrite8(int nbWord, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    return asyncWrite(8, nbWord, data, cb, cookie, repeat);
}
/**
 * @brief
 *
 * @return true
 * @return false
 */
bool lnSPI_bp::finishAsyncDma()
{
    txDone();
    return true;
}
/**
 * @brief
 *
 * @return true
 * @return false
 */
bool lnSPI_bp::waitForAsync()
{
    bool ret = true;
    if (!_done.take(LN_SPI_LONG_TIMEOUT)) // 100 ms should be plenty enough!
    {
        ret = false;
    }
    else
    {
        waitForCompletion();
    }
    txDma.endTransfer();
    csOff();
    sdisable();
    updateDmaTX(_regs, false);
    return ret;
}
/**
 * @brief
 *
 * @param a
 * @param b
 */
void asyncTrampoline(void *a, lnDMA::DmaInterruptType b)
{
    (void)b;
    auto *me = (lnSPI_bp *)a;
    me->invokeCallback();
}
/**
 * @brief
 *
 */
void lnSPI_bp::invokeCallback()
{
    lnSpiCallback *cb = _callback;
    _callback = nullptr;
    xAssert(cb);
    cb(_callbackCookie);
}
/**
 * @brief
 *
 * @param nbTransfer
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool lnSPI_bp::nextWrite(int nbTransfer, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    if (0 == nbTransfer)
        return false;

    _callback = cb;
    _callbackCookie = cookie;

    txDma.attachCallback(asyncTrampoline, this);
    txDma.doMemoryToPeripheralTransferNoLock(nbTransfer, (uint16_t *)data, (uint16_t *)&_regs->DATA, repeat);

    // 2- Configure SPI
    updateMode(_regs, lnTxOnly); // tx only
    updateDmaTX(_regs, true);    // activate DMA
    _regs->CTL0 |= LN_SPI_CTL0_SPIEN;
    return true;
}

/**
 * @brief
 *
 * @param nbTransfer
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool lnSPI_bp::nextWrite16(int nbTransfer, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    return nextWrite(nbTransfer, (const uint8_t *)data, cb, cookie, repeat);
}
/**
 * @brief
 *
 * @param nbTransfer
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool lnSPI_bp::nextWrite8(int nbTransfer, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    return nextWrite(nbTransfer, data, cb, cookie, repeat);
}
// EOF
