/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "MapleFreeRTOS1000.h"
#include "systemHelper.h"
#pragma once



/**
 */
class lnBinarySemaphore
{
public:
             lnBinarySemaphore();
        bool take();
        bool tryTake();
        bool take(int timeoutMs);
        bool give();

protected:
        SemaphoreHandle_t _handle;
};
#define xBinarySemaphore  lnBinarySemaphore

/**
 * @brief
 *
 */
class lnTask
{
public:
                        lnTask(const char *name,  int priority=3, int taskSize=100);
                virtual ~lnTask();
                void    start();
                virtual void run()=0; // Put your code here
                static void Trampoline(void *param)
                {
                     lnTask *tsk=(lnTask *) param;
                     tsk->run();
                }

protected:
                TaskHandle_t    _taskHandle;
                const char      *_name;
                int             _priority;
                int             _taskSize;
};
#define xTask lnTask
/**
 *
 */
class lnEventGroup
{
public:
                lnEventGroup();
    virtual     ~lnEventGroup();
    void        setEvents(uint32_t events);
    uint32_t    waitEvents(uint32_t maskint, int timeout=0); //  the events are cleared upon return from here ! returns  0 if timeout
    uint32_t    readEvents(uint32_t maskInt); // it is also cleared automatically !
protected:
    EventGroupHandle_t _handle;
};
#define xEventGroup lnEventGroup

/**
 *
 */
class lnFastEventGroup
{
public:
                lnFastEventGroup();
    virtual     ~lnFastEventGroup();
    void        takeOwnership(); // the task calling this will own the FastEventGroup
    void        setEvents(uint32_t events);
    // -1 timeout means wait forever
    uint32_t    waitEvents(uint32_t maskint, int timeout=-1); //  the events are cleared upon return from here ! returns  0 if timeout
    uint32_t    readEvents(uint32_t maskInt); // it is also cleared automatically !
protected:
    uint32_t    _value;
    uint32_t    _mask;
    TaskHandle_t _waitingTask;

};
#define xFastEventGroup lnFastEventGroup


/**
 *
 */
class lnMutex
{
public:
              lnMutex();
        bool lock();
        bool unlock();
protected:
        SemaphoreHandle_t _handle;
};
#define xMutex lnMutex

/**
 *
 * @param tex
 */
class lnAutoMutex
{
public:
    lnAutoMutex(lnMutex *tex)
    {
        _tex=tex;
        _tex->lock();
    }
    ~lnAutoMutex()
    {
        _tex->unlock();
        _tex=NULL;
    }
protected:
    lnMutex *_tex;
};
#define xAutoMutex lnAutoMutex

void xDelay(uint32_t ms);
extern "C"
{void do_assert(const char *a); }
#define xAssert(a) if(!(a)) {do_assert(#a);}
