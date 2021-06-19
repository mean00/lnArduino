/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnGPIO.h"
#include "lnGPIO_priv.h"
#include "lnPeripheral_priv.h"
#include "lnPinMapping.h"
#include "lnTimer.h"


static  LN_GPIO *gpio[5]={(LN_GPIO *)LN_GPIOA_ADR,(LN_GPIO *)LN_GPIOB_ADR,(LN_GPIO *)LN_GPIOC_ADR,(LN_GPIO *)LN_GPIOD_ADR,(LN_GPIO *)LN_GPIOE_ADR};

static  LN_GPIO *gpioA=(LN_GPIO *)LN_GPIOA_ADR;
static  LN_GPIO *gpioB=(LN_GPIO *)LN_GPIOB_ADR;
static  LN_GPIO *gpioC=(LN_GPIO *)LN_GPIOC_ADR;

/**
 * 
 * @param pin
 * @param mode
 */
void lnPinMode(const lnPin xpin, const GpioMode mode)
{
    LN_GPIO *port=gpio[xpin>>4];
    const LN_PIN_MAPPING *lnPin=pinMappings+xpin;
    xAssert(lnPin->pin==xpin);
    int pin=xpin&0xf;
    
    volatile uint32_t *CTL;
    if(pin>7)
    {
        CTL=&(port->CTL1);
        pin&=7;
    }else
    {
        CTL=&(port->CTL0);
    }
    uint32_t value;
    switch(mode)
    {
        case lnADC_MODE:
        case lnDAC_MODE:
                                value=LNGPIOSET(LN_CTL_MD_INPUT,LN_CTL_INPUT_ANALOG); break;    
        case lnFLOATING:        value=LNGPIOSET(LN_CTL_MD_INPUT,LN_CTL_INPUT_FLOATING); break;
        case lnINPUT_PULLUP:    value=LNGPIOSET(LN_CTL_MD_INPUT,LN_CTL_INPUT_PP); break;
        case lnINPUT_PULLDOWN:  value=LNGPIOSET(LN_CTL_MD_INPUT,LN_CTL_INPUT_PP); break;
        
        case lnOUTPUT:          value=LNGPIOSET (LN_CTL_MD_OUTPUT,LN_CTL_OUTPUT_PP); break;
        case lnOUTPUT_OPEN_DRAIN:value=LNGPIOSET(LN_CTL_MD_OUTPUT,LN_CTL_OUTPUT_OD); break;
        case lnPWM:             
                                xAssert(lnPin->timer!=-1);                                
                                
                                //lnTimer::setPwmMode(lnPin->timer, lnPin->timerChannel);
                                
        case lnALTERNATE_PP:    value=LNGPIOSET (LN_CTL_MD_OUTPUT,LN_CTL_OUTPUT_ALTERNAT_PP); break;
        case lnALTERNATE_OD:    value=LNGPIOSET (LN_CTL_MD_OUTPUT,LN_CTL_OUTPUT_ALTERNAT_OD); break;
        default:
            xAssert(0);
            break;
    }
    uint32_t ref=*CTL;
    ref&=~(0Xf<<(pin*4));
    ref|=value<<(pin*4);
    *CTL=ref;        
}
/**
 * 
 * @param pin
 * @param value
 */
void lnDigitalWrite(const lnPin pin, bool value)
{
    LN_GPIO *port=gpio[pin>>4];
    int xpin=pin&0xf;
    uint32_t mask,radix;
    if(value)
    {
        radix=1;
    }else
    {
        radix=(1<<16);
    }
    port->BOP=radix<<xpin;
}
void lnDigitalToggle(const lnPin pin)
{
    LN_GPIO *port=gpio[pin>>4];
    int xpin=pin&0xf; 
    uint32_t val=port->OCTL;
    val^=1<<xpin;
    port->OCTL=val;
}


/**
 * 
 * @param pin
 * @return 
 */
bool lnDigitalRead(const lnPin pin)
{
    xAssert(0);
    return false;
}
// EOF



