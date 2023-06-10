#pragma once
#include "lnDma.h"

typedef void(lnTimerIrq)(void *cookie);

enum lnTimerMode
{
    lnTimerModePwm0,
    lnTimerModePwm1
};
/**
 * \brief Careful all the channels of a given timer share the same frequency!
 * Max frequency is around 100 kHz in PWM mode !
 *
 *
 * In all mode , after setup the channel is disabled
 * You have to explicitely call timer->enable()
 *
 * @param timer
 * @param channel
 */
class lnTimer
{
  public:
    lnTimer(int timer, int channel);
    lnTimer(int pin);
    virtual ~lnTimer();
    // PWM API
    void setPwmMode(int ratio100);    // The ratio "low", 25 => 25% time low, 75=>75% time low
    void setPwmFrequency(int fqInHz); // PMW bas frequency
    // non PWM API
    void setChannelRatio(int ratio100); // Ratio=100 => 100%, 50 => 50%, 0 => 0%
    //
    void singleShot(int durationMs, bool down = false);
    void setForceState(bool forceHigh = false);
    void disable();
    void enable();
    int timer()
    {
        return _timer;
    }
    int channel()
    {
        return _channel;
    }

  protected:
    int _timer, _channel;
    void setTickFrequency(int fqInHz);
    void setMode(lnTimerMode mode);
};

/**
 * /!\ Changing the frequency alters ALL channels
 * @param timer
 * @param channel
 * @return
 */
class lnAdcTimer : public lnTimer
{
  public:
    lnAdcTimer(int timer, int channel) : lnTimer(timer, channel)
    {
    }
    // this is a single pulse timer
    void setPwmFrequency(int fqInHz);
    int getPwmFrequency();

  protected:
    int _actualPwmFrequency;
};
/**
 *
 */
class lnSquareSignal : public lnTimer
{
  public:
    lnSquareSignal(int timer, int channel) : lnTimer(timer, channel)
    {
    }
    lnSquareSignal(int pin) : lnTimer(pin)
    {
    }
    // this is a single pulse timer
    void setFrequency(int fqInHz);
};

/**
 * /!\ Changing the frequency alters ALL channels
 * @param timer
 * @param channel
 * @return
 */

class lnDmaTimerCallback
{
  public:
    virtual bool timerCallback(bool half) = 0; // return true if we continue, zero if we stop
};
/**
 * Careful, the ratio values are between 0 and rollover and rollover depends on fq !
 *
 * @param pin
 */
class lnDmaTimer : public lnTimer
{
  public:
    /* bits is 8 or 16, the size of the data to send to the timer*/
    lnDmaTimer(int bits, lnPin pin);
    virtual ~lnDmaTimer();
    bool pwmSetup(int frequency);
    int rollover();
    bool attachDmaCallback(lnDmaTimerCallback *cb);
    bool start(int nbSample, uint8_t *data);
    void stop();
    bool setTickFrequency(int fq);

  protected:
    lnDmaTimerCallback *_cb;
    int _rollover;
    lnDMA *_dma;
    int _bits; /* 8 or 16 bits buffer ? */
  public:
    void dmaInterrupt(bool h);
};
/**
 * \brief careful : this will use the whole timer,  not just a channel
 */
class lnDelayTimer : lnTimer
{
  public:
    lnDelayTimer(int timer, int channel);
    virtual ~lnDelayTimer();
    void arm(int delayUs);
    void setInterrupt(lnTimerIrq *handler, void *cookie);
    //--
    static void interruptHandler(int timer);

  protected:
    void irq();
    void enableInterrupt();
    void disableInterrupt();

    lnTimerIrq *_handler;
    void *_cookie;
};
