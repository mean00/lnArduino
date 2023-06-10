/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
#include "lnArduino.h"

class lnStopWatch
{
  public:
    lnStopWatch(int ms);
    bool restart(int durationMs);
    bool elapsed();

  protected:
    uint32_t _start;
    uint32_t _end;
};
/**
 *
 */
class lnCycleClock
{
  public:
    lnCycleClock();
    void restart();
    uint32_t elapsed();

  protected:
    uint32_t _start;
};
