
#include "lnArduino.h"

uint8_t ucHeap[configTOTAL_HEAP_SIZE];

extern void loop();
extern "C" void _init();
/**
 * 
 * @param 
 */
void initTask(void *)
{
    LoggerInit();    
    loop();
}
/**
 * 
 * @return 
 */
int main()
{
    // Initialize system
    _init();
    
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); 
    eclic_global_interrupt_enable();                       
    
    // The LEDs are all on GPIO A
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    // We need alternate functions too
    rcu_periph_clock_enable(RCU_AF); 
    rcu_periph_clock_enable(RCU_DMA0); 
    // DMA TX for SPI0 (LCD)
    DMA_CHCTL(DMA0, DMA_CH2) = (uint32_t)(DMA_PRIORITY_HIGH | DMA_CHXCTL_DIR); 
    // DMA TX for usart0
    DMA_CHCTL(DMA0, DMA_CH4) = (uint32_t)(DMA_PRIORITY_HIGH | DMA_CHXCTL_DIR); 
    //
    
    // Start freertos
    xTaskCreate(initTask,"entryTask",500,NULL,2,NULL);    
    vTaskStartScheduler();      
    deadEnd(25);    
}
// EOF