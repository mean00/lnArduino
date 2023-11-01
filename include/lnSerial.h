/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
#include "lnArduino.h"





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
        txRx
    };
    lnSerialCore(int instance, int rxBufferSize = 128)
    {
        _instance=instance;
    }
    virtual  ~lnSerialCore()
    {
    
    }
    virtual bool init(lnSerialMode mode)=0;
    virtual bool setSpeed(int speed)=0;
    virtual bool enableRx(bool enabled)=0;
    virtual bool transmit(int size, const uint8_t *buffer)=0;
    virtual bool dmaTransmit(int size, const uint8_t *buffer)=0;
    virtual void purgeRx()=0;
    virtual int read(int max, uint8_t *to)=0;
    virtual void rawWrite(const char *st)=0;
    typedef void lnSerialCallback(void *cookie, lnSerialCore::Event event);
    void setCallback(lnSerialCallback *cb, void *cookie)
    {
        _cb = cb;
        _cbCookie = cookie;
    }
    // no copy interface
    virtual int getReadPointer(uint8_t **to)=0;
    virtual void consume(int n)=0;

  protected:
    lnSerialMode      _mode;
    int               _instance;
    xMutex            _txMutex;
    xBinarySemaphore  _txDone;
    lnSerialCallback  *_cb;
    void              *_cbCookie;
};
lnSerialCore *createLnSerial(int instance, int rxBufferSize = 128);

// EOF
