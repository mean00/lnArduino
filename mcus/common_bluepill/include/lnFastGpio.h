/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino_macro.h"
/**
 *
 * @param p
 */
class lnFastIO
{
  public:
    lnFastIO(lnPin p);
    LN_ALWAYS_INLINE void on()  
    {
        *_onoff = _onbit;
    }
    LN_ALWAYS_INLINE void off()  
    {
        *_onoff = _offbit;
    }
    LN_ALWAYS_INLINE void pulseLow() 
    {
        *_onoff = _offbit;
        *_onoff = _onbit;
    }
    void pulseHigh() 
    {
        *_onoff = _onbit;
        *_onoff = _offbit;
    }

  protected:
    volatile uint32_t *_onoff;
    uint32_t _onbit, _offbit;
};

// EOF
