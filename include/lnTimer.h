#pragma once
#include "lnDma.h"


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
             lnTimer(int timer,int channel);
             lnTimer(int pin);
    virtual ~lnTimer();
        // PWM
        void setPwmMode(int ratio1024);
        void setPwmFrequency(int fqInHz); // This program the timer fq with a tick of 1024, i.e. internal fq is 1024* the given fq
        void setChannelRatio(int ratio1024); // Ratio=1024 => 100%, 512 => 50%, 0 => 0%
        //
        void singleShot(int durationMs, bool down=false);
        void setForceState(bool forceHigh=false);
        void disable();
        void enable();
        int  timer() {return _timer;}
        int  channel() {return _channel;}
protected:
        int _timer,  _channel;
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
        lnAdcTimer(int timer,int channel) : lnTimer(timer, channel)
        {

        }    
        // this is a single pulse timer
        void setPwmFrequency(int fqInHz); 
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
        virtual bool timerCallback(bool half)=0; // return true if we continue, zero if we stop
};
/**
 * 
 * @param pin
 */
class lnDmaTimer : public lnTimer
{
public:
                lnDmaTimer(int pin);
        virtual ~lnDmaTimer();                
        bool    pwmSetup(int frequency);
        int     rollover();
        bool    attachDmaCallback(lnDmaTimerCallback *cb);
        bool    start(int nbSample, uint8_t *data);
        void    stop();
        bool    setTickFrequency(int fq);
protected:
        lnDmaTimerCallback *_cb;
        int _rollover;
        lnDMA *_dma;
public:        
        void dmaInterrupt(bool h);
};

