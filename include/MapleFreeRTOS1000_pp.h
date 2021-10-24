/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
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
        bool tryTake();
        bool take(int timeoutMs);
        bool give();
  
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
                void    start();
                virtual void run()=0; // Put your code here
                static void Trampoline(void *param)
                {
                     xTask *tsk=(xTask *) param;
                     tsk->run();
                }

protected:
                TaskHandle_t    _taskHandle;
                const char      *_name;
                int             _priority;
                int             _taskSize;                
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
    void        takeOwnership(); // the task calling this will own the FastEventGroup
    void        setEvents(uint32_t events);
    uint32_t    waitEvents(uint32_t maskint, int timeout=0); //  the events are cleared upon return from here ! returns  0 if timeout
    uint32_t    readEvents(uint32_t maskInt); // it is also cleared automatically !
protected:
    uint32_t    _value;
    uint32_t    _mask;    
    TaskHandle_t _waitingTask;
           
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


/**
 * 
 * @param tex
 */
class xAutoMutex
{
public:    
    xAutoMutex(xMutex *tex)
    {
        _tex=tex;
        _tex->lock();
    }
    ~xAutoMutex()
    {
        _tex->unlock();
        _tex=NULL;
    }
protected:    
    xMutex *_tex;
};


void xDelay(int ms);

extern "C" 
{void do_assert(const char *a); }
#define xAssert(a) if(!(a)) {do_assert(#a);}
