/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
/**
 * 
 * \brief If you do a dma transfer from a task, you just call doMemoryToPeripheralTransferFromTask
 * if you do a dma transfer from an interrupt, you MUST
 * call beginTransfer() from the task, that will lock the DMA for you
 * call doMemoryToPeripheralTransferFromInterrupt() from the task
 * call endTransfer() when you will not use the dma any longer until the next begin
 * 
 * If you call doMemoryToPeripheralTransferFromTask from an interrupt it will assert
 * 
 */
class lnDMA
{
public:    
    typedef void (doneCallback)(void *);
    
    enum DmaTransferType
    {
        DMA_MEMORY_TO_PERIPH,
        DMA_PERIPH_TO_MEMORY
    };
    
                lnDMA(DmaTransferType type, int dmaEngine, int dmaChannel, int sourceWith, int targetWidth);
                ~lnDMA();
        void    attachCallback(doneCallback *cb, void *cookie);
        void    detachCallback();
        
        bool    doMemoryToPeripheralTransferNoLock(int count, const uint16_t *source,const uint16_t *target,  bool repeat);
        void    beginTransfer();
        void    endTransfer();
        void    setWordSize(int sourceWordSize, int targetWordSize);
        void    invokeCallback();
protected:
    uint32_t         _dma;
    int             _channelInt;
    int             _dmaInt;
    DmaTransferType _type;
    LnIRQ           _irq;
    
    doneCallback    *_cb;
    void            *_cookie;
    uint32_t        _control;
    
    int             _sourceWidth,_targetWidth;
    
};