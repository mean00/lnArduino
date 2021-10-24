/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
    Thin wrapper on top of FreeRTOS to be more C++ friendly
 */

#include "MapleFreeRTOS1000_pp.h"
#include "lnIRQ.h"

#define  fos_ms2tick(ms) (((ms)+portTICK_PERIOD_MS-1)/portTICK_PERIOD_MS)

#define EVENT_INDEX 1

extern "C"
{
extern void deadEnd(int code);
void do_assert(const char *a)
{
    deadEnd(0xA000);
    
}
#define xAssert(a) if(!(a)) {do_assert(#a);}
/**
 * 
 */
void vApplicationMallocFailedHook( void )
{
    do_assert("Malloc");
}
}
//-- Binary Semaphor --

/**
 * 
 */
xBinarySemaphore::xBinarySemaphore()
{
    _handle=xSemaphoreCreateBinary();
    xAssert(_handle);
}
/**
 * 
 * @return 
 */
bool xBinarySemaphore::take()
{
  return (bool) xSemaphoreTake(_handle,portMAX_DELAY);
}
/**
 * 
 * @param timeoutMs
 * @return 
 */
bool xBinarySemaphore::take(int timeoutMs)
{
  int ticks=1+(timeoutMs*configTICK_RATE_HZ+500)/1000;
  return (bool) xSemaphoreTake(_handle,ticks);
}
/**
 * 
 * @param timeoutMs
 * @return 
 */
bool xBinarySemaphore::tryTake()
{
     return (bool) xSemaphoreTake(_handle,0);
}
  

/**
 * 
 * @return 
 */
 bool xBinarySemaphore::give()
{   
  if(!underInterrupt)
  {
    xAssert(xSemaphoreGive(_handle)); // should never fail
  }else
  {
    BaseType_t awake;
    xSemaphoreGiveFromISR(_handle,&awake); // should never fail... 
    portYIELD_FROM_ISR(awake); // reschedule
  }
  return true;
}
 /**
  * 
  * @param ms
  */
 void xDelay(int ms)
 {
      const TickType_t xDelay = 1+( ms/ portTICK_PERIOD_MS);
      vTaskDelay(xDelay);
 }

 
 xMutex::xMutex()
 {
     _handle=xSemaphoreCreateRecursiveMutex();
 }
 bool xMutex::lock()
 {
    xAssert(xSemaphoreTakeRecursive(_handle,portMAX_DELAY)); // should never fail
    return true;
 }
 bool xMutex::unlock()
 {
    xAssert(xSemaphoreGiveRecursive(_handle)); // should never fail
    return true;
 }

// Task
/**
 * @brief Construct a new x Task::X Task object
 * 
 * @param name 
 * @param entryPoint 
 * @param priority 
 * @param taskSize 
 */
xTask::xTask(const char *name,  int priority, int taskSize)
{
    _priority=priority;
    _taskSize=taskSize;
    _name=name;
}
/**
 * 
 */
void xTask::start()
{
    BaseType_t er=xTaskCreate(xTask::Trampoline,_name,_taskSize, this,_priority,&_taskHandle);
    xAssert(er==pdPASS);    
}
/**
 * @brief Destroy the x Taskx Task object
 * 
 */
xTask::~xTask()
{
  xAssert(0);
}
#if 0
/**
 * 
 */
xEventGroup::xEventGroup()
{
    _handle=xEventGroupCreate();
    
}
/**
 * 
 */
xEventGroup::~xEventGroup()
{
//    xEventGroupDelete(_handle); No delete !
    xAssert(0);
    _handle=0;
}
/**
 * 
 * @param events
 */
void        xEventGroup::setEvents(uint32_t events)
{
    if(!underInterrupt)
    {
        xEventGroupSetBits(_handle,events);
    }else
    {
        BaseType_t wakeUp ;
        xEventGroupSetBitsFromISR(_handle,events,&wakeUp);
        portYIELD_FROM_ISR(wakeUp);  
    }
}
/**
 * 
 * @param maskint
 * @param timeout
 * @return 
 */
uint32_t    xEventGroup::waitEvents(uint32_t maskint, int timeout)
{
    if(timeout==0) 
        timeout=portMAX_DELAY-1;
    else
        timeout=fos_ms2tick(timeout);
    uint32_t res=xEventGroupWaitBits(
                       _handle,
                       maskint,
                       pdTRUE, // auto clear
                       pdFALSE, // any but
                       timeout );
    return res;
        
}
/**
 * 
 * @param maskInt
 * @return 
 */
uint32_t    xEventGroup::readEvents(uint32_t maskInt) // it is also cleared automatically !
{
     EventBits_t ev=xEventGroupGetBits( _handle );
     ev=ev & maskInt;
     if(ev)
     {
         xEventGroupClearBits(_handle,ev); // Race ?
     }     
     return ev;

}
#endif
//--------------------------------
/**
 * 
 */
#define BEGIN_LOCK() ENTER_CRITICAL()
#define END_LOCK()   EXIT_CRITICAL()



#define INVALID_TASK (TaskHandle_t)-1
xFastEventGroup::xFastEventGroup() 
{
    _value = _mask = 0;
    _waitingTask=INVALID_TASK;
}

/**
 * 
 */
xFastEventGroup::~xFastEventGroup() 
{

}
/**
 * 
 */
void        xFastEventGroup::takeOwnership()
{
    _waitingTask=xTaskGetCurrentTaskHandle(); 
}
/**
 * 
 * @param events
 */

#define BEGIN_LOCK() ENTER_CRITICAL()
#define END_LOCK()   EXIT_CRITICAL()


void xFastEventGroup::setEvents(uint32_t events) 
{
    if(underInterrupt)
    {       
#warning : Could we have a race here between different interrupts ? probably        
        _value = _value | events;
        bool w = _value & _mask;        
        if (!w || _waitingTask==INVALID_TASK) // no need to wake up task
        {     
             return;
        }   
        BaseType_t awake;
        vTaskNotifyGiveIndexedFromISR(_waitingTask,EVENT_INDEX,&awake);
        portYIELD_FROM_ISR(awake); // reschedule
        return;
    }

    // not under interrupt
    BEGIN_LOCK();
    _value = _value | events;
    bool w = _value & _mask;
    if (!w || _waitingTask==INVALID_TASK) // no need to wake up task
    {
        END_LOCK();
        return;
    }   
    END_LOCK();
    xTaskNotifyGiveIndexed(_waitingTask,EVENT_INDEX);  
    return;    
}
/**
 * 
 * @param maskint
 * @param timeout
 * @return 
 */


uint32_t xFastEventGroup::waitEvents(uint32_t maskint, int timeout )
{   
    xAssert(_waitingTask!=INVALID_TASK);
    xAssert(!underInterrupt);
    while(1)
    {
        BEGIN_LOCK();         
        uint32_t set = maskint & _value;
        if (set) 
        {            
            _value &= ~set;
            _mask=0;        
            END_LOCK();
            xTaskNotifyStateClearIndexed(xTaskGetCurrentTaskHandle(),EVENT_INDEX);            
            return set;
        }
        _mask=maskint;        
        END_LOCK(); 
        if(pdFALSE==ulTaskNotifyTakeIndexed(EVENT_INDEX, pdTRUE,timeout)) // cleared on exit
        { // timeout
            return 0;
        }
        // got it
        BEGIN_LOCK();
        set = maskint & _value;
        xAssert(set);
        _value &= ~set;
        _mask=0;         // no need to clear waitintTask, it must be the same !
        END_LOCK();
        return set;
        
    }
}

/**
 * 
 * @param maskInt
 * @return 
 */
uint32_t xFastEventGroup::readEvents(uint32_t maskInt) 
{
    BEGIN_LOCK();    
    uint32_t v = _value & maskInt;
    _value &= ~maskInt;
    _mask = 0;
    xTaskNotifyStateClearIndexed(xTaskGetCurrentTaskHandle(),EVENT_INDEX);
    END_LOCK(); 
    return v;
}

 //EOF
