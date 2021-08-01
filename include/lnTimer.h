#pragma once

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
        void setTimerFrequency(int fqInHz); // This program the timer fq with a tick of 1024, i.e. internal fq is 1024* the given fq
        void setChannelRatio(int ratio1024); // Ratio=1024 => 100%, 512 => 50%, 0 => 0%
        //
        void singleShot(int durationMs, bool down=false);
        void disable();
        void enable();
        int  timer() {return _timer;}
        int  channel() {return _channel;}
protected:
        int _timer,  _channel;

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
        void setTimerFrequency(int fqInHz); 
};
