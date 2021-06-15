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
    virtual ~lnTimer();
        void setPwmMode(int ratio);
        void setTimerFrequency(int fqInHz);
        void setChannelRatio(int ratioBy100);
protected:
        int _timer,  _channel;

};

