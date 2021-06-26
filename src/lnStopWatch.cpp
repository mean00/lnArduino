/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnStopWatch.h"
/**
 * 
 * @param startAt
 * @return 
 */
uint32_t now(uint32_t startAt)
{
    uint32_t tick=xTaskGetTickCount();
    if(tick>=startAt) return tick-startAt;
    uint32_t wrap=0xFFFFFFFFUL;
    uint32_t val=wrap-startAt;
    val+=tick;
    return val;
}

/**
 * 
 * @param ms
 */
lnStopWatch::lnStopWatch(int ms)
{
    _start=now(0);
    _end=ms;
}
/**
 * 
 * @param durationMs
 * @return 
 */
bool lnStopWatch::restart(int durationMs)
{
    _start=now(0);
    _end=durationMs;
    return true;
}
/**
 * 
 * @return 
 */
bool lnStopWatch::elapsed()
{
    uint32_t xnow=now(_start);
    if(xnow>_end) return true;
    return false;
}
//