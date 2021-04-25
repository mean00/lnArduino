/*
 
 */

#include "MapleFreeRTOS1000.h"
#include "systemHelper.h"
#pragma once



/**
 */
class xBinarySemaphore
{
public:
        xBinarySemaphore();
        bool take();
        bool take(int timeoutMs);
        bool give();
        bool giveFromInterrupt();
  
protected:
        SemaphoreHandle_t _handle;
};
/**
 * @brief 
 * 
 */
class xTask
{
public:
                        xTask(const char *name,  int priority=3, int taskSize=100);
                virtual ~xTask();
                virtual void run()=0; // Put your code here
                static void Trampoline(void *param)
                {
                     xTask *tsk=(xTask *) param;
                     tsk->run();
                }

protected:
                TaskHandle_t    _taskHandle;
};
/**
 * 
 */
class xEventGroup
{
public:
                xEventGroup();
    virtual     ~xEventGroup();
    void        setEvents(uint32_t events);
    void        setEventsFromISR(uint32_t events);
    uint32_t    waitEvents(uint32_t maskint, int timeout=0); //  the events are cleared upon return from here ! returns  0 if timeout
    uint32_t    readEvents(uint32_t maskInt); // it is also cleared automatically !
protected:
    EventGroupHandle_t _handle;
};


/**
 * 
 */
class xFastEventGroup
{
public:
                xFastEventGroup();
    virtual     ~xFastEventGroup();
    void        setEvents(uint32_t events);
    void        setEventsFromISR(uint32_t events);
    uint32_t    waitEvents(uint32_t maskint, int timeout=0); //  the events are cleared upon return from here ! returns  0 if timeout
    uint32_t    readEvents(uint32_t maskInt); // it is also cleared automatically !
protected:
    uint32_t    _value;
    uint32_t    _mask;
    xBinarySemaphore _sem;
};



/**
 * 
 */
class xMutex
{
public:
              xMutex();
        bool lock();
        bool unlock();
protected:
        SemaphoreHandle_t _handle;
};

void xDelay(int ms);

extern "C" 
{void do_assert(const char *a); }
#define xAssert(a) if(!(a)) {do_assert(#a);}
