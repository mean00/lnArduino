/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnSerial.h"
#include "lnSerial_priv.h"

LN_USART_Registers *usart0=(LN_USART_Registers *)USART0;
 static lnSerial *SerialInstance[5]={NULL,NULL,NULL,NULL,NULL};
                                     //0   1    2    3  

/**
 */
struct UsartMapping
{
    uint32_t usartEngine;
    uint32_t dmaEngine;
    int      dmaTxChannel;
    IRQn_Type irq;
};
static const UsartMapping usartMapping[5]=
{
    {USART0,0,3,USART0_IRQn},
    {USART1,0,6,USART1_IRQn},
    {USART2,0,1,USART2_IRQn},
    {UART3, 1,4,UART3_IRQn},
    {UART4, 0,7,UART4_IRQn},
};
/**
 * 
 * @param instance
 * @param irq
 */
#define M(x) usartMapping[instance].x

lnSerial::lnSerial(int instance) :  _txDma(lnDMA::DMA_MEMORY_TO_PERIPH, M(dmaEngine),M(dmaTxChannel),8,32)
{
    
    
    const UsartMapping *m=usartMapping+instance;
    _instance=instance;
    _irq=m->irq;
    _stateTx=false;
    _adr=m->usartEngine;
    xAssert(SerialInstance[instance]==NULL) ;
    SerialInstance[instance]=this;
    _txState=txIdle;
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
    switch(_instance)
    {
        case 0:
            lnPeripherals::enable(pUART0);
            lnPinMode(PA9,lnALTERNATE_PP);
            lnPinMode(PA10,lnFLOATING);
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
 * 
 * @return 
 */
bool lnSerial::enableTx(txMode mode)
{
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    d->CTL0&=~LN_USART_CTL0_UEN;
    switch(mode)
    {
        case txInterrupt:
             // disable TC & TE tx interrupt
            d->CTL0&=~( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
            // enable Tx            
            d->CTL2&=~LN_USART_CTL2_DMA_TX;    // disable TX DMA
            d->CTL0|=LN_USART_CTL0_TEN; // enable TX   
            d->CTL0|= LN_USART_CTL0_TBIE; // enable TBIE
            eclic_enable_interrupt(_irq);      // enable eclic   
            break;
        case txNone:
             eclic_disable_interrupt(_irq);    
             d->CTL0&=~LN_USART_CTL0_TEN; // disable TX
             d->CTL2&=~LN_USART_CTL2_DMA_TX; // disable TX DMA
             d->CTL0&=~( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
             break;
        case txDma:
            eclic_disable_interrupt(_irq);    
            d->CTL0|=( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
            d->CTL0|=LN_USART_CTL0_TEN; // enable TX
            d->CTL2|=LN_USART_CTL2_DMA_TX; // enable TX DMA            
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
bool lnSerial::transmit(int size,uint8_t *buffer)
{
    //return true;
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    _mutex.lock();
    ENTER_CRITICAL();
    _txState=txTransmitting;
    _tail=buffer+size;
    _cur=buffer+1;
    // Clear TC
    d->STAT&=~(LN_USART_STAT_TC);
    // enable TB interrupt    
    enableTx(txInterrupt);
    // send 1st byte
    d->DATA=(uint32_t )buffer[0];
    EXIT_CRITICAL();
    _txDone.take();    
    _mutex.unlock();
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
    d->STAT&=~(LN_USART_STAT_TC);
    _txState=txIdle;
    _txDone.give();
}
/**
 * 
 * @param c
 */
void lnSerial:: _dmaCallback(void *c)
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
bool lnSerial::dmaTransmit(int size,uint8_t *buffer)
{
    //return true;
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    _mutex.lock();
    ENTER_CRITICAL();
    _txState=txTransmitting;   
    enableTx(txDma);
    d->STAT&=~LN_USART_STAT_TC;
    _txDma.attachCallback(_dmaCallback,this);
    _txDma.      doMemoryToPeripheralTransfer(size,(uint16_t *)buffer,(uint16_t *)&(d->DATA),false);            
    EXIT_CRITICAL();
    _txDone.take();    
    _mutex.unlock();
    return true;
}
/**
 * 
 */
void lnSerial::_interrupt(void)
{
    LN_USART_Registers *d=(LN_USART_Registers *)_adr;
    switch(_txState)
    {
        case txTransmitting:
             xAssert(d->STAT & LN_USART_STAT_TBE) ;
             d->DATA=(uint32_t )*_cur;
            _cur++;
            if(_cur==_tail)
            {
                
                d->CTL0|=LN_USART_CTL0_TCIE;
                d->CTL0&=~(LN_USART_CTL0_TBIE ) ; // only let the Transmission complete bit
                _txState=txLast;              
            }
            return;
            break;
        case txLast:
             xAssert(d->STAT & LN_USART_STAT_TC) ;
             d->CTL0&=~( LN_USART_CTL0_TBIE +LN_USART_CTL0_TCIE);
             d->STAT&=~(LN_USART_STAT_TC);
            _txState=txIdle;
            _txDone.give();
            return;
            break;
        default:
            xAssert(0);
            break;
    }
            
   
}


void lnSerial::interrupts(int instance)
{
    lnSerial *inst=SerialInstance[instance];
    xAssert(inst);
    inst->_interrupt();
}

#define IRQHANDLER(x) extern "C" void USART##x##_IRQHandler () {    lnSerial::interrupts(x);}

IRQHANDLER(0)
IRQHANDLER(1)
IRQHANDLER(2)
IRQHANDLER(3)
// EOF
