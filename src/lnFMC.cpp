/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnFMC.h"
#include "lnFMC_priv.h"
#include "lnPeripheral_priv.h"


LN_FMC *aFMC=(LN_FMC *)LN_FMC_ADR;

class autoNoInterrupt
{
public:
        autoNoInterrupt()
        {
            noInterrupts();
        }
        ~autoNoInterrupt()
        {
            interrupts();
        }
};


static void unlock()
{
    // is it busy  ?
    xAssert(!(aFMC->STAT&LN_FMC_STAT_BUSY));
    if(!(aFMC->CTL  & LN_FMC_CTL_LK)) return; // already unlocked
    //
    aFMC->KEY=0x45670123;
    aFMC->KEY=0xCDEF89AB;
    xAssert(!(aFMC->CTL  & LN_FMC_CTL_LK));
}
static void waitNotBusy()
{
    // is it busy  ?
    while(1)
    {
        if(!(aFMC->STAT&LN_FMC_STAT_BUSY))
            return;
    }
}

/**
 * 
 * @param startAddress
 * @param sizeInKBytes
 * @return 
 */
bool lnFMC::erase(const uint32_t startAddress, int sizeInKBytes)
{
    uint32_t adr=startAddress;
    // Check alignment
    xAssert(adr>=0x800*0x10000);
    xAssert(adr<(0x800*0x10000+256*1024));
    xAssert(!(adr & ((1<<10)-1))); // 1 kB aligned
    autoNoInterrupt noInt;
    unlock();
    for(int i=0;i<sizeInKBytes;i++)
    {
        aFMC->CTL|=LN_FMC_CTL_PER;
        aFMC->ADDR=adr;
        aFMC->CTL|=LN_FMC_CTL_START;
        waitNotBusy();
        adr+=1024;
    }
    return true;
}
/**
 * 
 * @param startAddress
 * @param data
 * @param sizeInBytes
 * @return 
 */
bool lnFMC::write(const uint32_t startAddress, const uint8_t *data, int sizeInBytes)
{
    uint32_t adr=startAddress;
    // Check alignment
    xAssert(adr>=0x800*0x10000);
    xAssert(adr<(0x800*0x10000+256*1024));
    autoNoInterrupt noInt;
    unlock();
    
    // if not aligned
#warning todo
    xAssert(!(adr&1));
    // do 16 bytes aligned write
    uint16_t  *adr16=(uint16_t *)adr;
    int nbWord=sizeInBytes/2;
    for(int i=0;i<nbWord;i++)
    {
          uint16_t data16=(data[0])+(data[1]<<8);
          aFMC->CTL|=LN_FMC_CTL_PG;
          *adr16=data16;
          waitNotBusy();
          adr16++;
          data+=2;
    }
    sizeInBytes&=1;
#warning todo
    xAssert(!sizeInBytes);
    return false;
}


// EOF
