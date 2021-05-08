/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnSerial.h"
#include "lnSerial_priv.h"

LN_USART_Registers *usart0=(LN_USART_Registers *)USART0;

 static lnSerial *SerialInstance[4]={NULL,NULL,NULL,NULL};
/**
 * 
 * @param instance
 * @param irq
 */
lnSerial::lnSerial(int instance, IRQn_Type irq,uint32_t adr) :    _txDma(lnDMA::DMA_MEMORY_TO_PERIPH,0,4,8,8)
{
    _instance=instance;
    _irq=irq;
    _stateTx=false;
    _adr=adr;
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
    if(!_instance) freq=rcu_clock_freq_get(CK_APB2);
        else freq=rcu_clock_freq_get(CK_APB1);
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
            rcu_periph_clock_enable(RCU_USART0);
            gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
            gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
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
