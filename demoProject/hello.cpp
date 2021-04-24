#include "stdio.h"
#include "Arduino.h"
#include "systemHelper.h"
extern "C"
{
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
   
}
// green = PA1, blue = PA2, RED PC13
#define LED PA2

uint8_t ucHeap[configTOTAL_HEAP_SIZE];

extern "C"
{
    void _init();    
}

extern "C" void _exit(int x);
int roundup=0;
void MainTask(void *a)
{
    pinMode(LED,OUTPUT);
    bool onoff=true;
    digitalWrite(LED,HIGH);
    while(1)
    {
        roundup++;
        vTaskDelay(500);
        digitalToggle(LED);
        onoff=!onoff;
    }
}
int before=0;
int main()
{
    // Initialize system
    _init();
    
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); //四位优先级组全配置为lvl
    eclic_global_interrupt_enable();                       //使能全局中断


    //__asm volatile( "ecall" );
     rcu_periph_clock_enable(RCU_GPIOA);

    // Start freertos
    xTaskCreate( MainTask, "MainTask", 350, NULL, 10, NULL );   
    vTaskStartScheduler();      
    _exit(-1);
    
}
extern "C" void myInterruptHandler()
{
    
}
int dummyFunc()
{
    return 55;

}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t t, char *s)
{
    _exit(2);
}