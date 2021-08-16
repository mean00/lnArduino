/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnSPI.h"
#include "lnSPI_priv.h"
/**
 * 
 * @param wordSize
 * @param nbBytes
 * @param data
 * @param repeat
 * @return 
 */

int nbReq=0;
int nbInterrupt=0;
int nbSkipped=0;

bool hwlnSPIClass::dmaWriteInternal(int wordSize,int nbTransfer, const uint8_t *data,bool repeat)
{
    bool r=true;
    if(!nbTransfer) return true;
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    // 1- Configure DMA
    txDma.beginTransfer();
    txDma.attachCallback(exTxDone,this);    
    txDma.setWordSize(wordSize,wordSize);        
    txDma.doMemoryToPeripheralTransferNoLock(nbTransfer, (uint16_t *)data, (uint16_t *)&d->DATA,repeat);        
    
    // 2- Configure SPI
    updateMode(d,false); // tx only
    updateDataSize(d,wordSize);// 16 bits at a time
    updateDmaTX(d,true); // activate DMA

    // 3- Go!
    
    nbReq++;    
    if(d->STAT & LN_SPI_STAT_CONFERR )
    {
        txDma.endTransfer();
        Logger("Conf Error\n");
        return false;
    }    
    csOn();    
    senable();
    if(false==_done.take(100)) // 100 ms should be plenty enough!
    {
        r=false;
    }else
    {
        waitForCompletion(); 
    }
    txDma.endTransfer();
    csOff();    
    sdisable();
    updateDmaTX(d,false);
    return r;
}

/**
 */
bool hwlnSPIClass::dmaWrite(int nbBytes, const uint8_t *data)
{
    return dmaWriteInternal(8,nbBytes,data,false);
}

/**
 */
bool hwlnSPIClass::dmaWrite16(int nbWord, const uint16_t *data)
{
    return dmaWriteInternal(16,nbWord,(uint8_t *)data,false);
}



/**
 */
bool hwlnSPIClass::dmaWrite16Repeat(int nbWord, const uint16_t data)
{
    return dmaWriteInternal(16,nbWord,(uint8_t *)&data,true);
}


/**
 * 
 * @param c
 */
void hwlnSPIClass::exTxDone(void *c)
{
    nbInterrupt++;
    hwlnSPIClass *i=(hwlnSPIClass *)c;
    i->txDone();
}