#include "lnArduino.h"
#include "lnSerial.h"
 
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
    // send 1st byte
    usart_data_transmit(_adr, buffer[0]);
    // enable interrupt    
     usart_interrupt_enable(_adr,USART_INT_TC);
     usart_interrupt_enable(_adr,USART_INT_TBE);     
    // _txDone.take();
    _mutex.unlock();
    return true;
}

// EOF
