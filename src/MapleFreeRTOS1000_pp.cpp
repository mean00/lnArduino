/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
    Thin wrapper on top of FreeRTOS to be more C++ friendly
 */

#include "MapleFreeRTOS1000_pp.h"
uint32_t underInterrupt=0;
#define  fos_ms2tick(ms) (((ms)+portTICK_PERIOD_MS-1)/portTICK_PERIOD_MS)

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
    BaseType_t er=xTaskCreate(xTask::Trampoline,name,taskSize, this,priority,&_taskHandle);
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
//--------------------------------
/**
 * 
 */

#define BEGIN_LOCK() ENTER_CRITICAL()
#define END_LOCK()   EXIT_CRITICAL()

xFastEventGroup::xFastEventGroup() 
{
    _value = _mask = 0;
}

/**
 * 
 */
xFastEventGroup::~xFastEventGroup() 
{

}
/**
 * 
 * @param events
 */
void xFastEventGroup::setEvents(uint32_t events) 
{
    if(!underInterrupt)
    {
        BEGIN_LOCK();
        _value = _value | events;
        bool w = _value & _mask;
        END_LOCK();
        if (w)
            _sem.give();
    }else
    {
        _value = _value | events;
        bool w = _value & _mask;
        if (w)
            _sem.give();
    }
}
/**
 * 
 * @param maskint
 * @param timeout
 * @return 
 */
uint32_t xFastEventGroup::waitEvents(uint32_t maskint, int timeout )
{
    BEGIN_LOCK();
    uint32_t set = maskint & _value;
    if (set) 
    {
        _value &= ~set;
        _mask=0;        
        END_LOCK();
        return set;
    }
    _mask=maskint;
    END_LOCK(); // not atomic !
    _sem.take(timeout);
    BEGIN_LOCK();
    set = maskint & _value;
    if (set) 
    {
        _value &= ~set;
    }
    _mask=0;
    END_LOCK();
    return set;
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
    END_LOCK(); 
    return v;
}

 //EOF
