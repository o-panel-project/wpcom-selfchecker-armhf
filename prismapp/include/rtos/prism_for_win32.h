
#ifndef _PRISM_FOR_WIN_
#define _PRISM_FOR_WIN_

///////////////////////////////////////////////////////////////////////////////
// If using WINCE, by default the application will fill the entire screen
// To disable this and allow the application to have a title and
// min, max, and close buttons, disable this configuration
///////////////////////////////////////////////////////////////////////////////
#ifdef PRISM_FOR_WINCE
#define PRISM_FULLSIZE_WIN
#endif

#ifdef PRISM_THREADS_SUPPORT

void StartPrismThreads(void);

///////////////////////////////////////////////////////////////////////////////
// Defines used by the Pm_Event_Manager to keep track of threads and
// queues.
///////////////////////////////////////////////////////////////////////////////
#define PRISM_TASK_TYPE  DWORD
#define PRISM_QUEUE_TYPE PrismEventQueue*

///////////////////////////////////////////////////////////////////////////////
// Global reference to the main PRISM task. Note that this is a uint32_t and
// not a HANDLE since GetCurrentThreadId doesn't return a HANDLE. The
// GetCurrentThread function, which does return a HANDLE, is not what we
// want to use for a thread ID since the HANDLE returned is a psuedo HANDLE
// that generally refers to the currently executing thread.
///////////////////////////////////////////////////////////////////////////////
extern PRISM_TASK_TYPE PrismTaskPtr;

///////////////////////////////////////////////////////////////////////////////
// CriticalSection types that have to be shared with the rest of the
// library.
///////////////////////////////////////////////////////////////////////////////
extern CRITICAL_SECTION PrismScreenCriticalSection;
extern CRITICAL_SECTION PrismTimerCriticalSection;
extern CRITICAL_SECTION PrismEventCriticalSection;

///////////////////////////////////////////////////////////////////////////////
// Replacement pm_event_tQueue types and functions
// Note that a thread is assigned a PrismQueue and not a pm_event_tQueue
// for message passing. In the Win32 model, there is only 1 pm_event_tQueue
// instance and it acts as the free message pool for the rest of the
// library/application. A PrismQueue retrieves free messages from the pool
// and returns them when they are done.
///////////////////////////////////////////////////////////////////////////////
struct pm_event_t;  // forward reference

typedef struct
{
    HANDLE  QueueSem;
    pm_event_t *pFirst;
    pm_event_t *pLast;

} PrismEventQueue;

void DeleteEventQueue(PRISM_QUEUE_TYPE xchg);
BOOL PostEvent(void *pEvent, PRISM_QUEUE_TYPE pQueue);
PRISM_QUEUE_TYPE CreateEventQueue(void);

#define CREATE_EVENT_QUEUE          CreateEventQueue()
#define DELETE_EVENT_QUEUE(a)       DeleteEventQueue(a)
#define ENQUEUE_TASK_EVENT(a, b)    PostEvent(a, b)
#define CURRENT_TASK                GetCurrentThreadId()
#define PRISM_TASK_PTR              PrismTaskPtr
#define PRISM_NULL_QUEUE            NULL


///////////////////////////////////////////////////////////////////////////////
// The synchronization functions for protecting the presentation, timer
// and message queues.
///////////////////////////////////////////////////////////////////////////////
#define PRISM_ENTER_CRITICAL  EnterCriticalSection(&PrismScreenCriticalSection);
#define PRISM_EXIT_CRITICAL   LeaveCriticalSection(&PrismScreenCriticalSection);
#define PRISM_LOCK_TIMERS     EnterCriticalSection(&PrismTimerCriticalSection);
#define PRISM_UNLOCK_TIMERS   LeaveCriticalSection(&PrismTimerCriticalSection);
#define PRISM_LOCK_EVENTS     EnterCriticalSection(&PrismEventCriticalSection);
#define PRISM_UNLOCK_EVENTS   LeaveCriticalSection(&PrismEventCriticalSection);


#else
    // here for standalone Win32 build

#define PRISM_ENTER_CRITICAL
#define PRISM_EXIT_CRITICAL
#define PRISM_LOCK_EVENTS
#define PRISM_UNLOCK_EVENTS
#define PRISM_LOCK_TIMERS
#define PRISM_UNLOCK_TIMERS

#endif  // PRISM_THREADS_SUPPORT

#define PRISM_DELAY(_timer_ticks_)    Sleep(_timer_ticks_)

#include <time.h>

#define PM_SYSTEM_TICKS  clock()

#endif  // _WINPRISM_HPP_



