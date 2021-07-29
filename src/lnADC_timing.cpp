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

//------------------------------------------------------------------
/**
 * 
 * @param instance
 */
lnTimingAdc::lnTimingAdc(int instance)
{
    
}
/**
 * 
 */
lnTimingAdc::~lnTimingAdc()
{
    
}
/**
 * 
 * @param fq
 * @param nbSamplePerChannel
 * @param nbPins
 * @param pins
 * @param output
 * @return 
 */
bool     lnTimingAdc::multiRead(int fq, int nbSamplePerChannel, int nbPins, lnPin *pins, int *output)
{
    return false;
}

// EOF