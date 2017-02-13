///////////////////////////////////////////////////////////////////////////////
//
//            Copyright (c) 2010 by Blue Water Embedded, Inc.
//
//  This software is copyrighted by and is the sold property of Blue Water
//  Embedded, Inc. All rights, title, ownership, or other intersts in the
//  software remain the property of Blue Water Embedded, Inc. This software
//  may only be used in accordance with the corresponding license agreement.
//  Any unauthorized use, duplications, transmission, distribution, or
//  disclosure of this software is expressly forbidden.
//
//  This Copyright notice may not be removed or modified without prior
//  written consent of Blue Water Embedded, Inc.
//
//  Blue Water Embedded, Inc. reserves the right to modify this software
//  without notice.
//
//  Blue Water Embedded, Inc.               info@bwembedded.com
//  3847 Pine Grove Ave                     http://www.bwembedded.com
//  Suite A
//  Fort Gratiot, MI 48059
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//  prism_for_x11.h - X11 integration file allowing Prism to
//      build and run with Linux/X11 operating systems
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_FOR_X11_H_
#define _PRISM_FOR_X11_H_

#ifndef PRISM_TICS_SEC
#define PRISM_TICS_SEC                20
#endif

#ifndef RTOS_TICS_SEC
#define RTOS_TICS_SEC               100
#endif

//extern unsigned char PrismMainThread;

#ifndef PRISM_FREE_EVENTS
#define PRISM_FREE_EVENTS            20
#endif

#ifndef PRISM_PRIORITY
#define PRISM_PRIORITY 240
#endif

#define PRISM_TIMER_PRIORITY (PRISM_PRIORITY + 1)
#define PRISM_TOUCH_PRIORITY (PRISM_PRIORITY + 2)

#define PRISM_INPUT_TASK_STACK      2048

#define PRISM_NULL_TASK        ((PRISM_TASK_TYPE) NULL)


struct pm_event_t;           // forward reference

struct PrismLinuxPool
{
    pm_event_t *pFirst;
    pm_event_t *pLast;
};

typedef struct {
    pm_event_t *pFirst;     // FIFO head
    pm_event_t *pLast;      // FIFO tail
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;    
} pm_queue_t;

#define PRISM_QUEUE_TYPE      pm_queue_t *
#define PRISM_NULL_QUEUE      ((PRISM_QUEUE_TYPE) NULL)

int LinuxGetTics(void);

#define PM_SYSTEM_TICKS       LinuxGetTics();
#define PRISM_DELAY(a)        PrismNanoSleep(a)

#define PRISM_TASK_TYPE        pthread_t
#define PRISM_MUTEX_TYPE       pthread_mutex_t

#define PRISM_NULL_QUEUE      ((PRISM_QUEUE_TYPE) NULL)

extern Display          *gpDisplay;
extern int              gtScreen;
extern Window           gtDevWin;
extern GC               gtGC;
extern Colormap         gtColormap;
extern int              gnColorDepth;

#if defined(PRISM_THREADS_SUPPORT)

extern pthread_t        PrismMainThread;

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
void EnterCriticalSection();
void ExitCriticalSection();
void LockEventManager();
void UnlockEventManager();
void LockTimerList();
void UnlockTimerList();
int LinuxGetTics();
void LockXlib();	//@@@
void UnlockXlib();	//@@@

PRISM_QUEUE_TYPE CreatePrismEventQueue();
void DeletePrismEventQueue(PRISM_QUEUE_TYPE tQueue);
int RouteEventToTask(pm_event_t *pEvent, pm_queue_t *pQueue);
void PrismNanoSleep(int count);


///////////////////////////////////////////////////////////////////////////////
// Multithread defines
///////////////////////////////////////////////////////////////////////////////
#define PRISM_ENTER_CRITICAL       EnterCriticalSection();
#define PRISM_EXIT_CRITICAL        ExitCriticalSection();
#define PRISM_LOCK_EVENTS          LockEventManager();
#define PRISM_UNLOCK_EVENTS        UnlockEventManager();
#define PRISM_LOCK_TIMERS          LockTimerList();
#define PRISM_UNLOCK_TIMERS        UnlockTimerList();
#define CURRENT_TASK               pthread_self()
#define PRISM_TASK_PTR             PrismMainThread
#define CREATE_EVENT_QUEUE         CreatePrismEventQueue()
#define DELETE_EVENT_QUEUE(a)      DeletePrismEventQueue(a)
#define ENQUEUE_TASK_EVENT(a, b)   RouteEventToTask(a, b)


#else

#define PRISM_ENTER_CRITICAL
#define PRISM_EXIT_CRITICAL
#define PRISM_LOCK_EVENTS
#define PRISM_UNLOCK_EVENTS
#define PRISM_LOCK_TIMERS
#define PRISM_UNLOCK_TIMERS

#endif

#endif


