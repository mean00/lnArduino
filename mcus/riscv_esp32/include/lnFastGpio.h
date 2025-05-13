/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once

/**
 *
 * @param p
*/
#if 0 
class lnFastIO
{
  public:
    lnFastIO(lnPin p);
    void on() volatile
    {
        *_onoff = _onbit;
    }
    void off() volatile
    {
        *_onoff = _offbit;
    }
    void pulseLow() volatile __attribute__((always_inline))
    {
        *_onoff = _offbit;
        *_onoff = _onbit;
    }
    void pulseHigh() volatile __attribute__((always_inline))
    {
        *_onoff = _onbit;
        *_onoff = _offbit;
    }

  protected:
    volatile uint32_t *_onoff;
    volatile uint32_t _onbit, _offbit;
};
#endif
// EOF
