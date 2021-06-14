/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnTimer.h"
#include "lnTimer_priv.h"
uint32_t     *aTimer1=(uint32_t *)(LN_TIMER1_ADR);
uint32_t     *aTimer2=(uint32_t *)(LN_TIMER2_ADR);
uint32_t     *aTimer3=(uint32_t *)(LN_TIMER3_ADR);
uint32_t     *aTimer4=(uint32_t *)(LN_TIMER4_ADR);
/**
 * 
 * @param timer
 * @param channel
 */
void lnTimer::setPwmMode(int timer, int channel)
{
    xAssert(0);
}
/**
 * 
 * @param timer
 */
void lnTimer::setTimerFrequency(int timer)
{
    xAssert(0);
}

/**
 * 
 * @param ratioBy100
 */
void lnTimer::setChannelRatio(int ratioBy100)
{
    xAssert(0);
}

//EOF