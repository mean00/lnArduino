/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnPeripheral_priv.h"
#include "lnDAC.h"
#include "lnDAC_priv.h"
#include "lnTimer.h"

LN_DAC_Registers *adac=(LN_DAC_Registers *)LN_DAC_ADR;

/**
 * 
 * @param instance
 */
lnDAC::lnDAC(int instance)
{
    _instance=instance;
    disable();
}
/**
 * 
 */
lnDAC::~lnDAC()
{
    disable();
}
/**
 * 
 */
void    lnDAC::enable()
{
    adac->CTL|=LN_DAC_CTL_ENABLE(_instance);
}
/**
 * 
 */
void    lnDAC::disable()
{
    adac->CTL&=~LN_DAC_CTL_ENABLE(_instance);
}

void    lnDAC::simpleMode()
{
    uint32_t ctl=adac->CTL;
    ctl&=LN_DAC_CTL_DTSEL_MASK(_instance);
    ctl|=LN_DAC_CTL_DTSEL(_instance,LN_DAC_CTL_DTSEL_SOURCE_SW);
    ctl|=LN_DAC_CTL_DTEN(_instance);
    ctl|=LN_DAC_CTL_DBOFF(_instance);
    adac->CTL=ctl;
    
}

/**
 * 
 * @param value
 */
void    lnDAC::setValue(int value)
{
    adac->DATAS[_instance].DAC_R12DH=value;
    adac->SWT|=LN_DAC_SWT_ENABLE(_instance);
}