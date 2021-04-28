/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnSerial.h"
 static lnSerial *SerialInstance[4]={NULL,NULL,NULL,NULL};
/**
 * 
 * @param instance
 * @param irq
 */
lnSerial::lnSerial(int instance, IRQn_Type irq,uint32_t adr)
{
    _instance=instance;
    _irq=irq;
    _stateTx=false;
    _adr=adr;
    xAssert(SerialInstance[instance]==NULL) ;
    SerialInstance[instance]=this;
}
/**
 * 
 * @param speed
 * @return 
 */
bool lnSerial::setSpeed(int speed)
{
     usart_baudrate_set(_adr, speed);
     return true;
}
/**
 * 
 * @return 
 */
bool lnSerial::init()
{
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
    usart_deinit(_adr);
    usart_baudrate_set(_adr, 115200);
    usart_parity_config(_adr, USART_PM_NONE);
    usart_word_length_set(_adr, USART_WL_8BIT);
    usart_stop_bit_set(_adr, USART_STB_1BIT);
    usart_hardware_flow_rts_config(_adr, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(_adr, USART_CTS_DISABLE);
    usart_receive_config(_adr, USART_RECEIVE_DISABLE);
    usart_transmit_config(_adr, USART_TRANSMIT_DISABLE);
    usart_enable(_adr); 
    return true;
}
/**
 * 
 * @return 
 */
bool lnSerial::enableTx(bool onoff)
{
    
    if(onoff)
    {
        usart_transmit_config(_adr, USART_TRANSMIT_ENABLE);;
        usart_interrupt_disable(_adr,USART_INT_TBE);
        usart_interrupt_disable(_adr,USART_INT_TC);        
        eclic_enable_interrupt(_irq);        
    }else
    {
         eclic_disable_interrupt(_irq);        
         usart_transmit_config(_adr, USART_TRANSMIT_DISABLE);;
    }
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
    _mutex.lock();
    _tail=buffer+size;
    _cur=buffer+1;
    // send 1st byte
    usart_data_transmit(_adr, buffer[0]);
    // enable interrupt    
     //usart_interrupt_enable(_adr,USART_INT_TC);
     usart_interrupt_enable(_adr,USART_INT_TBE);     
     //_txDone.take();
    _mutex.unlock();
    return true;
}
/**
 * 
 */
void lnSerial::_interrupt(void)
{
    if(_cur && _cur<_tail)  // sending ?
    {
        if(usart_interrupt_flag_get(_adr,USART_INT_FLAG_TBE))
        {
            
            // next one        
            usart_data_transmit(_adr, *_cur);
            _cur++;
            if(_cur==_tail)
            {
                _cur=_tail=NULL;
                usart_interrupt_disable(_adr,USART_INT_TBE);     
                _txDone.give();
            }
        }else
        {
            xAssert(0);
        }
    }
    // Clear interrupts
    usart_interrupt_flag_clear(_adr,USART_INT_FLAG_TBE);
    //usart_interrupt_flag_clear(_adr,USART_INT_TC);
}

/**
 * 
 */
extern "C" void IRQ_USART0()
{
    lnSerial::interrupts(0);
}
extern "C" void IRQ_USART1()
{
    lnSerial::interrupts(1);
}
extern "C" void IRQ_USART2()
{
    lnSerial::interrupts(2);
}
extern "C" void IRQ_USART3()
{
    lnSerial::interrupts(3);
}



void lnSerial::interrupts(int instance)
{
    lnSerial *inst=SerialInstance[instance];
    xAssert(inst);
    inst->_interrupt();
}
// EOF
