#include "lnArduino.h"
#include "lnTimer.h"
#include "lnDAC.h"
#include "lnADC.h"
#include "math.h"
#define LED PA2
#define PWM PB9
#define TIMER 3
#define CHANNEL 3

void setup()
{
    pinMode(LED,OUTPUT);
    
}
/**
 * 
 */
#define TIMER_ID 3
#define TIMER_CHANNEL 3
#define FREQUENCY (50*1000)
#define TIMER_PIN PB9

int val;
void loop()
{
    bool onoff=true;
    digitalWrite(LED,true);
    int roundup=0;

    lnPinMode(PA0,lnADC_MODE);
    lnPinMode(PA1,lnADC_MODE);
    lnPinMode(PA4,lnDAC_MODE);
    lnPinMode(PA5,lnDAC_MODE);


    lnTimingAdc *adc=new lnTimingAdc(0);
    lnDAC *dac0=new lnDAC(0);
    lnDAC *dac1=new lnDAC(1);
    
    dac0->simpleMode();
    dac1->simpleMode();
    
    dac0->enable();
    dac1->enable();
    
    Logger("Connect PA4 and PA0\n");
    Logger("Connect PA5 and PA1\n");
#define SAMPLE_PER_CHANNEL 10    
    int output[2*SAMPLE_PER_CHANNEL];
    lnPin pins[2]={PA0,PA1};
    
    lnPinMode(TIMER_PIN,lnPWM);


#if 0
    {    
    lnAdcTimer pwm(TIMER_ID,TIMER_CHANNEL);
    pwm.setTimerFrequency(1000);
    pwm.enable();
    while(1)
    {
        xDelay(10);
    }
    }
#endif    

    
#if 0
    {
    lnTimer pwm(TIMER_ID,TIMER_CHANNEL);
    pwm.setTimerFrequency(1000);
    pwm.setPwmMode(512);
    pwm.enable();
    while(1)
    {
        xDelay(10);
    }
    }
#endif    
    adc->setSource(TIMER_ID,TIMER_CHANNEL,FREQUENCY,2,pins);
    while(1)
    {
        dac0->setValue(500);
        dac1->setValue(3500);   
        delay(10);
        memset(output,0,2*SAMPLE_PER_CHANNEL*sizeof(int));
        
        adc->multiRead(SAMPLE_PER_CHANNEL,output);
        Logger("500:3500 PA0: %d PA1 :%d \n",output[0],output[1]);
        
        for(int i=0;i<SAMPLE_PER_CHANNEL;i++)
        {
#if 0
            Logger(" 500 : %d \n",output[0+2*i]);
            Logger("3500 : %d \n",output[1+2*i]);
#else            
            xAssert(output[0+2*i]<1000) ;
            xAssert(output[1+2*i]>2800) ;
#endif
        }
        delay(500);
        dac0->setValue(3500);
        dac1->setValue(500);
        delay(10);
        memset(output,0,2*SAMPLE_PER_CHANNEL*sizeof(int));        
        adc->multiRead(SAMPLE_PER_CHANNEL,output);

        Logger("3500:500 PA0: %d PA1 :%d \n",output[0],output[1]);        
        for(int i=0;i<SAMPLE_PER_CHANNEL;i++)
        {
#if 0            
            Logger("3500 : %d \n",output[0+2*i]);
            Logger(" 500 : %d \n",output[1+2*i]);
#else            
            xAssert(output[0+2*i]>2800) ;
            xAssert(output[1+2*i]<1000) ;
#endif            
        }
        delay(500);

    }
}
