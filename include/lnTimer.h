#pragma once

/**
 * \brief Careful all the channels of a given timer share the same frequency!
 * @param timer
 * @param channel
 */
class lnTimer
{
public:
             lnTimer(int timer,int channel);
             lnTimer(int pin);
    virtual ~lnTimer();
        void setPwmMode(int ratio);
        void setTimerFrequency(int fqInHz); // This program the timer fq with a tick of 1024
        void setChannelRatio(int ratio1024); // Ratio=1024 => 100%, 512 => 50%, 0 => 0%
        void disable();
        void enable();
protected:
        int _timer,  _channel;

};

