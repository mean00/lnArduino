
#include "lnArduino.h"
#include "lnGPIO.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
/**
    \fn    
*/
void lnPinMode(const lnPin pin, const lnGpioMode mode)
{
    gpio_init(pin);
    gpio_set_function(pin, GPIO_FUNC_SIO);
    

#define AS_INPUT(pin, up, down)       gpio_set_pulls(pin,up, down);   \
                    gpio_set_dir(pin,false); \
                    gpio_set_input_enabled(pin,true);

    switch(mode)
    {
        case lnFLOATING : 
                    AS_INPUT(pin,false,false);
                    break;
        case lnINPUT_PULLUP:
                    AS_INPUT(pin,true,false);
                    break;    
        case lnINPUT_PULLDOWN :
                    AS_INPUT(pin,false,true);
                    break;
        case lnOUTPUT:
                    gpio_set_input_enabled(pin,false);
                    gpio_set_pulls(pin,false, false);  
                    gpio_set_dir(pin,true);
                    break;
        default:
                    xAssert(0);
                    break;
    }
    /*
    lnOUTPUT_OPEN_DRAIN = 4,
    lnALTERNATE_PP,
    lnALTERNATE_OD,
    lnPWM,
    lnADC_MODE,
    lnDAC_MODE
    */
}
/**
    \fn    
*/

void lnDigitalWrite(const lnPin pin, bool value)
{
   gpio_put(pin, value);  
}
/**
    \fn    
*/

bool lnDigitalRead(const lnPin pin)
{
    return gpio_get(pin);
}
/**
    \fn    
*/

void lnDigitalToggle(const lnPin pin)
{
    gpio_xor_mask( 1<<pin);
}
/**
    \fn    
    //----------------- This is bluepill common specific !-------------
*/

volatile uint32_t *lnGetGpioToggleRegister(int port) // Bop register for port "port" with port A:0, B:1, ...
{
    xAssert(0);
    return NULL;
}
/**
    \fn    
     Direction register for the bit 0..7 of port "port" , A=0, B=1, ...
     //----------------- This is bluepill common specific !-------------
*/

volatile uint32_t *lnGetGpioDirectionRegister(  int port) 
{
    xAssert(0);
    return NULL;
}
/**
    \fn    
    //----------------- This is bluepill common specific !-------------
*/

volatile uint32_t *lnGetGpioValueRegister(int port) // Bit value for LOW bits of port "port"
{
    xAssert(0);
    return NULL;
}
/**
    \fn    
    //----------------- This is bluepill common specific !-------------
*/

uint32_t lnReadPort(int port)
{
    xAssert(0);
    return 0;
}

// EOF
