#include "lnArduino.h"
#include "lnSerial.h"

lnDMA dummyDma(lnDMA::DMA_MEMORY_TO_PERIPH, 0,0,0,0);



    lnSerial::lnSerial(int instance, int rxBufferSize ) : _txDma( dummyDma)
    {}
    bool lnSerial::init()
    {
        return true;
    }
    bool lnSerial::setSpeed(int speed)
    {
        return true;
    }
    bool lnSerial::enableRx(bool enabled)
    {
        return true;
    }

    bool lnSerial::transmit(int size, const uint8_t *buffer)
    {
        return true;
    }

    bool lnSerial::dmaTransmit(int size, const uint8_t *buffer)
    {
        return true;
    }

    void lnSerial::disableInterrupt()
    {
    
    }

    void lnSerial::enableInterrupt(bool txInterruptEnabled)
    {
    }

    void lnSerial::purgeRx()
    {

    }

    void lnSerial::_interrupt(void)
    {
        
    }

    int lnSerial::read(int max, uint8_t *to)
    {
        return 0;
    }


    int lnSerial::getReadPointer(uint8_t **to)
    {
        return 0;
    }

    void lnSerial::consume(int n)
    {
    }



    void lnSerial::rawWrite(const char *c)
    {
    }

  
    void lnSerial::txInterruptHandler(void)
    {
    }

    void lnSerial::rxInterruptHandler(void)
    {
        
    }

    bool lnSerial::_programTx(void)
    {
        return true;
    }


  
    bool lnSerial::_enableTx(txState mode)
    {
        return true;
    }

    
    void lnSerial::txDmaCb()
    {
    }

 void lnSerial::_dmaCallback(void *c, lnDMA::DmaInterruptType it)
{

}
     void lnSerial::interrupts(int instance)
     {

     }
// EOF