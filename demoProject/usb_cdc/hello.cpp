#include "lnArduino.h"
#include "lnTimer.h"
#include "include/lnTusb.h"
#include "hello.h"
#define LED LN_SYSTEM_LED
/**
*/
void setup()
{
    pinMode(LED,OUTPUT);    
}

 
void loop()
{
    Logger("Starting lnTUSB Test\n");
    lnTUSB *usb=new lnTUSB;
    usb->init(5,descriptor);
    usb->setConfiguration(desc_hs_configuration,desc_fs_configuration,&desc_device,&desc_device_qualifier);
    usb->start();    
    while(1)
    {
      Logger("*\n");
      delay(1000);    
      lnDigitalToggle(LED);
    }  
}
