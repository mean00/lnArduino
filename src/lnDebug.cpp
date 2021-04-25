#include "lnArduino.h"
#include "stdarg.h"
//#include "embedded_printf/printf.h"
static char buffer[1024];

void sendString(const char *st)
{
    int l=strlen(st);
    for(int i=0;i<l;i++)
    {
        while(usart_flag_get(USART0, USART_FLAG_TBE) == RESET)
        {
            
        }
        usart_data_transmit(USART0, (uint8_t) st[i]);
    }
    
    
}
void debugLogger(const char *fmt...)
{
     sendString(fmt);
}
void Logger(const char *fmt...)
{
    va_list va;
    va_start(va,fmt);
    vsnprintf(buffer,1022,fmt,va);
    
    buffer[1023]=0;
    sendString(buffer);
    va_end(va);
    
}
/**
 * 
 */
void LoggerInit()
{
    rcu_periph_clock_enable(RCU_USART0);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0); 
}

