/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
#include "lnArduino.h"
#include <type_traits>

/**
 * @brief
 *
 */

class lnSerialCore
{
  public:
    enum Event
    {
        dataAvailable,
    };
    enum lnSerialMode
    {
        txOnly,
        txRx,
        txRxDma
    };
    lnSerialCore(int instance)
    {
        _instance = instance;
    }
    virtual ~lnSerialCore()
    {
    }
    virtual bool init() = 0;
    virtual bool setSpeed(int speed) = 0;

  protected:
    int _instance;
};
/**
 * @brief
 *
 */
class lnSerialTxOnly : public lnSerialCore
{
  public:
    lnSerialTxOnly(int instance) : lnSerialCore(instance)
    {
    }
    virtual ~lnSerialTxOnly()
    {
    }
    virtual bool transmit(int size, const uint8_t *buffer) = 0;
    virtual bool rawWrite(int size, const uint8_t *buffer) = 0;
};
/**
 * @brief
 *
 */
class lnSerialRxTx : public lnSerialCore
{
  public:
    lnSerialRxTx(int instance) : lnSerialCore(instance)
    {
    }
    virtual ~lnSerialRxTx()
    {
    }
    virtual bool transmit(int size, const uint8_t *buffer) = 0;
    virtual bool enableRx(bool enabled) = 0;
    virtual void purgeRx() = 0;
    virtual int read(int max, uint8_t *to) = 0;
    typedef void lnSerialCallback(void *cookie, lnSerialCore::Event event);
    void setCallback(lnSerialCallback *cb, void *cookie)
    {
        _cb = cb;
        _cbCookie = cookie;
    }
    // no copy interface
    virtual int getReadPointer(uint8_t **to) = 0;
    virtual void consume(int n) = 0;

  protected:
    lnSerialCallback *_cb;
    void *_cbCookie;
};

lnSerialTxOnly *createLnSerialTxOnly(int instance, bool dma, bool buffered);
lnSerialRxTx *createLnSerialRxTx(int instance, int rxBufferSize = 128, bool dma = true);

// EOF
