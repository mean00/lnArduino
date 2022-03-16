/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnSerial.h"
#include "lnSerial_priv.h"
#include "lnPeripheral_priv.h"

LN_USART_Registers *usart0=(LN_USART_Registers *)LN_USART0_ADR;
 static lnSerial *SerialInstance[5]={NULL,NULL,NULL,NULL,NULL};
                                     //0   1    2    3

/**
 */
struct UsartMapping
{
    uint32_t usartEngine;
    uint32_t dmaEngine;
    int      dmaTxChannel;
    LnIRQ    irq;
    lnPin    tx;
    lnPin    rx;
    Peripherals periph;
};
static const UsartMapping usartMapping[3]=
{
    // Adr        DMA CH IRQ         TX   RX   periperal
    {LN_USART0_ADR, 0,3,LN_IRQ_USART0,PA9,PA10,pUART0},
    {LN_USART1_ADR, 0,6,LN_IRQ_USART1,PA2,PA3,pUART1},
    {LN_USART2_ADR, 0,1,LN_IRQ_USART2,PB10,PB11,pUART2},
};
/**
 *
 * @param instance
 * @param irq
 */
#define M(x) usartMapping[instance].x

lnSerial::lnSerial(int instance, int rxBufferSize) :
 _txDma(lnDMA::DMA_MEMORY_TO_PERIPH, M(dmaEngine),M(dmaTxChannel),8,32)
{
    const UsartMapping *m=usartMapping+instance;
    _instance=instance;
    _irq=m->irq;
    _adr=m->usartEngine;
    xAssert(SerialInstance[instance]==NULL) ;
    SerialInstance[instance]=this;
    _txState=txTransmittingIdle;
    _rxBufferSize=rxBufferSize;
    _rxTail=_rxHead=0;
    _rxBuffer=new uint8_t [_rxBufferSize];
    _rxEnabled=false;
    _cbCookie=NULL;
    _cb=NULL;
}

void lnSerial::enableInterrupt(bool tx)
{
    if(tx || _rxEnabled)
      lnEnableInterrupt(_irq);
    else
      lnDisableInterrupt(_irq);
}

/**
*/
int    lnSerial::modulo(int in)
{
  if(in>=_rxBufferSize)
    return in-_rxBufferSize;
  return in;
}

/**
 *
 * @param speed
 * @return
 */
bool lnSerial::setSpeed(int speed)
{
    int freq;

    Peripherals periph=( Peripherals)((int)pUART0+_instance);
    freq=lnPeripherals::getClock(periph);
    // compute closest divider
    int divider=(freq+speed/2)/speed;
    // Fixed 4 buts decimal, just ignore since we oversample by 16
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    d->CTL0&=~LN_USART_CTL0_UEN;
    d->BAUD=divider; // change speed when usart is off
    d->CTL0|=LN_USART_CTL0_UEN;
    return true;
}
/**
 *
 * @return
 */
bool lnSerial::init()
{
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    const UsartMapping *e=usartMapping+_instance;

    switch(_instance)
    {
        case 0:
        case 1:
        case 2:
            lnPinMode(e->tx,lnALTERNATE_PP);
            lnPinMode(e->rx,lnFLOATING);
            lnPeripherals::enable(e->periph);
            break;
        default:
            xAssert(0);
            break;

    }
    // Disable RX & TX
    d->CTL0&=~LN_USART_CTL0_KEEP_MASK; // default is good enough
    setSpeed(115200);
    d->CTL0|=LN_USART_CTL0_UEN; // enable uart
    return true;
}
/**
  \brief purge fifo and uart registers
*/
#define RE_ENABLE_INTERRUPT() enableInterrupt(_txState==txTransmittingInterrupt || _txState==txTransmittingLast )

void lnSerial::purgeRx()
{
  LN_USART_Registers *d=(LN_USART_Registers *)_adr;
  disableInterrupt();
  _rxHead=_rxTail=0;
  volatile uint32_t stat=d->STAT;
  while(stat & (LN_USART_STAT_RBNE))
  {
    uint8_t c=d->DATA;
    stat=d->STAT;
  }
   RE_ENABLE_INTERRUPT();
}
/**

*/
bool lnSerial::enableRx(bool enabled)
{
  LN_USART_Registers *d=(LN_USART_Registers *)_adr;
  d->CTL0&=~LN_USART_CTL0_UEN;
  if(enabled)
  {
      disableInterrupt();
      _rxEnabled=true;
      d->CTL0|=LN_USART_CTL0_RBNEIE;
      d->CTL0|=LN_USART_CTL0_REN;
      enableInterrupt(true);
  }else
  {
      disableInterrupt();
      _rxEnabled=false;
       d->CTL0&=~LN_USART_CTL0_REN;
       d->CTL0&=~LN_USART_CTL0_RBNEIE;
       RE_ENABLE_INTERRUPT();
  }
  d->CTL0|=LN_USART_CTL0_UEN;
  return true;
}

/**
 *
 * @return
 */
bool lnSerial::_programTx()
{
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    d->CTL0&=~LN_USART_CTL0_UEN;
    switch(_txState)
    {
        case txTransmittingLast:
            disableInterrupt();
             // disable TC & TE tx interrupt
            d->CTL0&=~( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
            // enable Tx
            d->CTL2 &=~LN_USART_CTL2_DMA_TX;    // disable TX DMA
            d->CTL0 |= LN_USART_CTL0_TEN; // enable TX
            d->CTL0 |= LN_USART_CTL0_TCIE; // enable TBIE
            enableInterrupt(true);      // enable tx part of interrupt
            break;
        case txTransmittingInterrupt:
            disableInterrupt();
             // disable TC & TE tx interrupt
            d->CTL0&=~( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
            // enable Tx
            d->CTL2 &=~LN_USART_CTL2_DMA_TX;    // disable TX DMA
            d->CTL0 |= LN_USART_CTL0_TEN; // enable TX
            d->CTL0 |= LN_USART_CTL0_TBIE; // enable TBIE
            enableInterrupt(true);      // enable tx part of interrupt
            break;
        case txTransmittingIdle:
             disableInterrupt();
             d->CTL0&=~LN_USART_CTL0_TEN; // disable TX
             d->CTL2&=~LN_USART_CTL2_DMA_TX; // disable TX DMA
             d->CTL0&=~( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
             enableInterrupt(false);   // disable tx part of interrupt
             break;
        case txTransmittingDMA:
            disableInterrupt();
            d->CTL0|=( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
            d->CTL0|=LN_USART_CTL0_TEN; // enable TX
            d->CTL2|=LN_USART_CTL2_DMA_TX; // enable TX DMA
            enableInterrupt(false);
            break;
        default:
            xAssert(0);
            break;
    }
    d->CTL0|=LN_USART_CTL0_UEN;
    return true;
}
/**
 *
 * @param size
 * @param buffer
 * @return
 */
bool lnSerial::transmit(int size,const uint8_t *buffer)
{
    //return true;
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    _txMutex.lock();
    ENTER_CRITICAL();
    _lastTransferSize=size;
    _tail=buffer+size;
    _cur=buffer+1;
    if(size==1)
    {
      _txState=txTransmittingLast;
    }
    else
    {
      _txState=txTransmittingInterrupt;
    }
    d->STAT&=~(LN_USART_STAT_TC);
    // send 1st byte
    d->DATA=(uint32_t )buffer[0];
    // enable TB interrupt
    _programTx();
    EXIT_CRITICAL();
    _txDone.take();
    _txMutex.unlock();
    return true;
}

/**
 *
 */
void lnSerial::txDmaCb()
{
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    // disable TC & TBE
    d->CTL0&=~( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
    // clear TC
    // nope d->STAT&=~(LN_USART_STAT_TC);
    // clear DMA
    d->CTL2&=~(LN_USART_CTL2_DMA_TX);
    _txState=txTransmittingIdle;
    _txDone.give();
}
/**
 *
 * @param c
 */
void lnSerial::_dmaCallback(void *c,lnDMA::DmaInterruptType it)
{
    lnSerial *i=(lnSerial *)c;
    i->txDmaCb();
}

/**
 *
 * @param size
 * @param buffer
 * @return
 */
bool lnSerial::dmaTransmit(int size,const uint8_t *buffer)
{
    //return true;
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    _txMutex.lock(); // lock uart
    _txDma.beginTransfer(); // lock dma
    ENTER_CRITICAL();
    _lastTransferSize=size;
    _txState=txTransmittingDMA;
    _programTx();
    d->STAT&=~LN_USART_STAT_TC;
    _txDma.attachCallback(_dmaCallback,this);
    EXIT_CRITICAL();

    _txDma. doMemoryToPeripheralTransferNoLock(size,(uint16_t *)buffer,(uint16_t *)&(d->DATA),false);

    _txDone.take();
    _txDma.endTransfer();

    // Wait busy bit to clear out
    while(!(d->STAT & LN_USART_STAT_TC))
    {
        __asm__("nop"::);
    }
    d->STAT&=~(LN_USART_STAT_TC);

    _txMutex.unlock();
    return true;
}

/**
 *
 */
volatile int serialRound=0;
/**

*/
void lnSerial::_interrupt(void)
{
    serialRound++;
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
#define ALL_INTERRUPTS (LN_USART_STAT_RBNE+LN_USART_STAT_TC+LN_USART_STAT_TBE+LN_USART_STAT_OVERR)
    volatile int stat=d->STAT &   d->CTL0 & ALL_INTERRUPTS; // filter enabled interrupt only
    if(stat &  (LN_USART_STAT_TC+LN_USART_STAT_TBE))
    {
      if(  _txState==txTransmittingInterrupt || _txState == txTransmittingLast)
        txInterruptHandler();
    }
    if(stat & (LN_USART_STAT_RBNE) && _rxEnabled)
    {
        rxInterruptHandler();
    }
}

/**

*/
void lnSerial::rxInterruptHandler(void)
{
  LN_USART_Registers *d=(LN_USART_Registers *)_adr;
  volatile int stat=d->STAT;
  bool empty=(_rxTail==_rxHead);
  int pushed=0;
  while(stat & (LN_USART_STAT_RBNE))
  {
    uint8_t c=d->DATA;
    // do we have space in the ring buffer ?
    int next=modulo(_rxTail+1);
    if(next!=_rxHead) // is it full ?
    {
      _rxBuffer[next]=c;
      _rxTail=next;
      pushed++;
    }
    stat=d->STAT;
  }
  if(empty && pushed)
  {
      // wakeup receiver...
      if(_cb)
        _cb(_cbCookie,dataAvailable);
  }
}
/**

*/
void lnSerial::txInterruptHandler(void)
{
  LN_USART_Registers *d=(LN_USART_Registers *)_adr;
  volatile int stat=d->STAT;
  volatile int ctl0=d->CTL0;
  switch(_txState)
  {
      case txTransmittingInterrupt:
           xAssert(stat & LN_USART_STAT_TBE) ;
           d->DATA=(uint32_t )*_cur;
          _cur++;
          if(_cur>_tail) xAssert(0);
          if(_cur==_tail)
          {
              d->CTL0|=LN_USART_CTL0_TCIE;
              d->CTL0&=~(LN_USART_CTL0_TBIE ) ; // only let the Transmission complete bit
              _txState=txTransmittingLast;
          }
          return;
          break;
      case txTransmittingLast:
           if(!(stat & LN_USART_STAT_TC))
              xAssert(0); // we got here due to RX interrupt...
           d->CTL0&=~( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
           d->STAT&=~(LN_USART_STAT_TC);
          _txState=txTransmittingIdle;
          _txDone.give();
          return;
          break;
      default:
          xAssert(0);
          break;
  }
}
/**

*/
void lnSerial::disableInterrupt()
{
  const UsartMapping *m=usartMapping+_instance;
  lnDisableInterrupt(m->irq);
}
/**

*/
void lnSerial::interrupts(int instance)
{
    lnSerial *inst=SerialInstance[instance];
    xAssert(inst);
    inst->_interrupt();
}
/**

*/
int lnSerial::read(int max, uint8_t *to)
{
  int z=0;
  disableInterrupt();
  if(_rxHead==_rxTail)
  {
      RE_ENABLE_INTERRUPT();
      return 0;
  }
  if(_rxHead>_rxTail)
  {
    z=_rxBufferSize-_rxHead;
    if(z>max) z=max;
    memcpy(to,_rxBuffer+_rxHead,z);
    _rxHead=modulo(_rxHead+z);
  }else
  {
    z=_rxTail-_rxHead;
    if(z>max) z=max;
    memcpy(to,_rxBuffer+_rxHead,z);
    _rxHead+=z; // cannot wrap here...
  }
  RE_ENABLE_INTERRUPT();
  return z;

}


#define IRQHANDLER(x) extern "C"{  void USART##x##_IRQHandler () LN_INTERRUPT_TYPE ;void USART##x##_IRQHandler () {    lnSerial::interrupts(x);} }

IRQHANDLER(0)
IRQHANDLER(1)
IRQHANDLER(2)
IRQHANDLER(3)
// EOF
