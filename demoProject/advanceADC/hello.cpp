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
    
    while(1)
    {
        dac0->setValue(500);
        dac1->setValue(3500);   
        delay(10);
        adc->multiRead(1000,SAMPLE_PER_CHANNEL,2,pins,output);
        Logger("500:3500 PA0: %d PA1 :%d \n",output[0],output[1]);
        for(int i=0;i<SAMPLE_PER_CHANNEL;i++)
        {
            xAssert(output[0+2*i]<900) ;
            xAssert(output[1+2*i]>3000) ;
        }
        delay(500);
        dac0->setValue(3500);
        dac1->setValue(500);
        delay(10);
        adc->multiRead(1000,SAMPLE_PER_CHANNEL,2,pins,output);
        Logger("3500:500 PA0: %d PA1 :%d \n",output[0],output[1]);        
        for(int i=0;i<SAMPLE_PER_CHANNEL;i++)
        {
            xAssert(output[0+2*i]>3000) ;
            xAssert(output[1+2*i]<900) ;
        }
        delay(500);

    }
}
