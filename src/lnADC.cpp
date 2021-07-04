/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnPeripheral_priv.h"
#include "lnADC.h"
#include "lnADC_priv.h"
#include "lnBasicTimer.h"
#include "lnDma.h"
#include "lnPinMapping.h"
/**
 */
struct LN_ADC_DESCR
{
    int   dmaEngine;
    int   dmaChannel;
    LN_ADC_Registers *registers;
    
};


LN_ADC_Registers *aadc0=(LN_ADC_Registers *)LN_ADC0_ADR;
LN_ADC_Registers *aadc1=(LN_ADC_Registers *)LN_ADC1_ADR;

// Timer 5 and 6 are a good fit
const LN_ADC_DESCR lnAdcDesc[2]=
{
    {0,0,(LN_ADC_Registers *)LN_ADC0_ADR},
    {-1,-1,(LN_ADC_Registers *)LN_ADC1_ADR}, // ADC1 cannot do DMA !
};
/**
 * 
 * @param instance
 * @param pin
 */
lnSimpleADC::lnSimpleADC(int instance, lnPin pin)
{
    _instance=instance;
    setPin(pin);
    
    setup();
}
/**
 * 
 */
lnSimpleADC::~lnSimpleADC()
{

}
/**
 * 
 * @param pin
 * @return 
 */

extern const LN_PIN_MAPPING pinMappings[];

static int adcChannel(lnPin pin)
{
     const LN_PIN_MAPPING *p=pinMappings;
     while(1)
     {
         xAssert(p->pin!=-1);
         if(p->pin==pin) return p->adc;
         p++;
     }
}
/**
 * 
 */
void   lnSimpleADC:: setup()
{
    // Setup default value
    LN_ADC_Registers *adc=lnAdcDesc[_instance].registers;
    
    adc->CTL1=0;
    adc->CTL0=0;    
    // sw mode by default
    adc->CTL1|=LN_ADC_CTL1_ETSRC_SET(LN_ADC_CTL1_ETSRC_SOURCE_SW);
    
  
    adc->SAMPT[0]=0;
    adc->SAMPT[1]=0;
    adc->SAMPT[1]=5; ; // 55.5 cycles
    
    for(int i=0;i<4;i++)
        adc->IOFF[i]=0;
    
    adc->WDHT=0;
    adc->WDLT=0;
    
    for(int i=0;i<3;i++)
        adc->RSQS[i]=0;
    
    adc->RSQS[2]=adcChannel(_pin);
    adc->RSQS[0]=1<<20; // one channel
    adc->ISQ=0;
    adc->OVRS=0;
    
    // Set default clock divider
    lnPeripherals::setAdcDivider(lnADC_CLOCK_DIV_BY_8);
    
    
    adc->CTL1 |=LN_ADC_CTL1_ADCON;
    //  calibration    
    delayMicroseconds(10);
    // reset calibration
    adc->CTL1|=LN_ADC_CTL1_RSTCLB;
    adc->CTL1|=LN_ADC_CTL1_CLB;
    while(adc->CTL1&LN_ADC_CTL1_CLB)
    {
        __asm__("nop");
    }
    
    // read vcc
    if(!_instance)
    {
        adc->CTL1 &=~LN_ADC_CTL1_ADCON;
        adc->RSQS[2]=17;  // VREF
        adc->CTL1 |=LN_ADC_CTL1_ADCON;
        //
        adc->CTL1|=LN_ADC_CTL1_SWRCST;
        while( (adc->STAT & LN_ADC_STAT_EOC))
        {
            __asm__("nop");
        }
        //
        _vcc=adc->RDATA&0xfff;
        //
        adc->CTL1 &=~LN_ADC_CTL1_ADCON;
        adc->RSQS[2]=adcChannel(_pin);
        adc->CTL1 |=LN_ADC_CTL1_ADCON;
    }
}
/**
 * 
 * @return 
 */
lnPin   lnSimpleADC::pin()
{
    return _pin;
}
/**
 * 
 * @param pin
 * @return 
 */
bool    lnSimpleADC::setPin(lnPin pin)
{
    _pin=pin;    
    setup();
    return true;
}
/**
 * 
 * @return 
 */
int     lnSimpleADC::simpleRead()
{
    int value;
    if(!multiRead(1,&_pin,&value)) return -1;
    return value;
}
/**
 * 
 * @param nbPins
 * @param pins
 * @param output
 * @return 
 */
bool    lnSimpleADC::multiRead(int nbPins, lnPin *pins, int *output)
{

}
// EOF