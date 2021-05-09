/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
/**
 */
class lnDMA
{
public:    
    typedef void (doneCallback)(void *);
    
    enum DmaTransferType
    {
        DMA_MEMORY_TO_PERIPH
    };
    
                lnDMA(DmaTransferType type, int dmaEngine, int dmaChannel, int sourceWith, int targetWidth);
                ~lnDMA();
        void    attachCallback(doneCallback *cb, void *cookie);
  
        bool    doMemoryToPeripheralTransfer(int count, const uint16_t *source,const uint16_t *target,  bool repeat);
        void    setWordSize(int sourceWordSize, int targetWordSize);
        void    invokeCallback();
protected:
    dma_channel_enum _channel;
    uint32_t         _dma;
    int             _channelInt;
    int             _dmaInt;
    DmaTransferType _type;
    IRQn_Type       _irq;
    
    doneCallback    *_cb;
    void            *_cookie;
    uint32_t        _control;
    
};