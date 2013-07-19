///////////////////////////////////////////////////////////////////////////////
//
//            Copyright (c) 2010 by Blue Water Embedded, Inc.
//
//  This software is copyrighted by and is the sole property of Blue Water
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
//  prism_event_mgr.h - Prism event manager class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_EVENT_MGR_
#define _PRISM_EVENT_MGR_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
#define PM_CALL_EVENT_HANDLER(object, ptrToMember)  ((object).*(ptrToMember)) 

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

enum Pm_Common_Events {
    PM_EVENT_SIZED = 0,
    PM_EVENT_FOCUS,
    PM_EVENT_FOCUS_LOST,
    PM_EVENT_CLICK,
    PM_EVENT_RCLICK
};

enum Pm_Text_Input_Events {
    PM_EVENT_TXT_SELECT    = 8,
    PM_EVENT_TXT_EDIT,
    PM_EVENT_TXT_EDITDONE,
    PM_EVENT_PAN_DONE
};

enum Pm_Btn_Events {
    PM_EVENT_TOGGLE_ON = 8,
    PM_EVENT_TOGGLE_OFF,
    PM_EVENT_CHECK,
    PM_EVENT_UNCHECK
};

enum Pm_ScrollEvents {
    PM_EVENT_SCROLL = 8,  
    PM_EVENT_SLIDER_VAL
};

enum PrismSpinEvents {
    PM_EVENT_SPIN_UP     = 8, 
    PM_EVENT_SPIN_DOWN         
};

enum PrismNotebookEvents {
    PM_EVENT_PAGE_SELECT = 8     
};

enum PrismTreeEvents {
    PM_EVENT_NODE_SELECT = 8,    
    PM_EVENT_NODE_DELETE,        
    PM_EVENT_NODE_OPEN,          
    PM_EVENT_NODE_CLOSE,         
    PM_EVENT_NODE_ALT_SEL       
};

enum PrismDataTableEvents {
    PM_EVENT_CELL_SELECT = 8,
    PM_EVENT_COL_SELECT,
    PM_EVENT_ROW_SELECT,
    PM_EVENT_COL_DESELECT,
    PM_EVENT_ROW_DESELECT
};

enum PrismListEvents {
    PM_EVENT_LIST_SELECT = 8
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////

enum PrismSystemEvent {
    PM_EVENT_PENDOWN = 1,          
    PM_EVENT_PENUP,         
    PM_EVENT_RPENDOWN,    
    PM_EVENT_RPENUP,    
    PM_EVENT_MOUSE_OVER,       
    PM_EVENT_MOUSE_NOTOVER,    

    PM_EVENT_RESIZE,        
    PM_EVENT_CLOSE,      
    PM_EVENT_HIDE,         
    PM_EVENT_SHOW,
    PM_EVENT_LINK,
    PM_EVENT_UNLINK,           
    PM_EVENT_DESTROY,        
    PM_EVENT_PEN_DRAG,     
    PM_EVENT_PEN_MOVE,    

   #if defined(PRISM_FLICK_GESTURE_SUPPORT)
    PM_EVENT_ST_PAN_UP,        
    PM_EVENT_ST_PAN_DOWN,     
    PM_EVENT_ST_PAN_LEFT,     
    PM_EVENT_ST_PAN_RIGHT,    
    PM_EVENT_ST_ROTATE_CW,   
    PM_EVENT_ST_ROTATE_CCW,   
    PM_EVENT_MT_PAN_UP,         
    PM_EVENT_MT_PAN_DOWN,      
    PM_EVENT_MT_PAN_LEFT,       
    PM_EVENT_MT_PAN_RIGHT,      
    PM_EVENT_ZOOM_IN,           
    PM_EVENT_ZOOM_OUT,
    PM_EVENT_MULTITOUCH,         
   #endif   
      
    PM_EVENT_PAINT,           
    PM_EVENT_GAINED_FOCUS,    
    PM_EVENT_LOST_FOCUS,     
    PM_EVENT_GAINED_INPUT,   
    PM_EVENT_LOST_INPUT,       

    PM_EVENT_LINKICON,         
    PM_EVENT_MOVE,          
    PM_EVENT_PARENTSIZED,     
    PM_EVENT_VSCROLL,         
    PM_EVENT_HSCROLL,         
    PM_EVENT_SIZE_TO_PARENT,  
    PM_EVENT_RESTORE,         
    PM_EVENT_MINIMIZE,         
    PM_EVENT_TIMEOUT,           
    PM_EVENT_KEYPAD_PUSH,      
    PM_EVENT_KEYPAD_RELEASE,    
    PM_EVENT_CUT,              
    PM_EVENT_COPY,          
    PM_EVENT_PASTE,          
    PM_EVENT_SLIDER_MOVE,     
    PM_EVENT_CLOSING,         
    PM_EVENT_TERMINATE,       
    PM_EVENT_MOVE_FOCUS,     
    PM_EVENT_PAINT_NOTIFY,      
    PM_EVENT_CANVAS_CHANGE,   
    PM_EVENT_NEW_RESOURCES,    
    PM_EVENT_NEW_LANGUAGE      
};

const pm_uint_t FIRST_EVENT         = (1 << 15);
const pm_uint_t FIRST_APP_EVENT     = (1 << 14);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PM_CEN(ID, EVNT) (FIRST_EVENT + ((ID) << 4) + (EVNT))
#define CEN_MASK(a) ((uint16_t) (1 << (a)))
#define ALL_CEN_MASK 0x0F
class Pm_Widget;             

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifdef PRISM_THREADS_SUPPORT

struct PrismTaskInfo
{
    PrismTaskInfo *pNext;
    Pm_Widget *pWidget;

    PRISM_TASK_TYPE    pTask;
    PRISM_QUEUE_TYPE   pQueue;
};

#endif

///////////////////////////////////////////////////////////////////////////////

struct pm_event_t
{
    public:
        pm_event_t() 
        {
            pNext = NULL;
            pSource = NULL;
            pTarget = NULL;
        }

        pm_event_t(uint16_t _Type) 
        {
            pNext = NULL; 
            pSource = NULL;
            pTarget = NULL;
            Type = _Type;
        }

        // we have to put the Target first here, otherwise
        // we will have ambiguous constructors

        pm_event_t(Pm_Widget *Target, uint16_t _Type)
        {
            pNext = NULL;
            pSource = NULL;
            pTarget = Target;
            Type = _Type;
        }

        Pm_Widget *pSource;
        Pm_Widget *pTarget;      
        uint16_t Type;
        uint16_t Param;

        union
        {
            void       *pData;
            int32_t    Payload[2];  

            Pm_Region  Region;
            pm_point_t Point;

            uint32_t   AppPayload_u32[2];
            int32_t    AppPayload_i32[2];
            uint16_t   AppPayload_u16[4];
            int16_t    AppPayload_i16[4];
            uint8_t    AppPayload_u8[8];
        };

        pm_event_t *pNext;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Event_Manager
{
    public:

        Pm_Event_Manager(void);
        ~Pm_Event_Manager();

        pm_bool_t Accept(pm_event_t *pPut, pm_bool_t Suspend = TRUE);

        pm_bool_t IsEmpty(void);
        static pm_bool_t IsInputEvent(uint16_t Type);

        pm_bool_t PostHead(pm_event_t *pGet);
        pm_bool_t PostHead(pm_event_t &In)
        {
            return PostHead(&In);
        }

        pm_bool_t PostTail(pm_event_t *pGet);
        pm_bool_t PostTail(pm_event_t &In)
        {
            return PostTail(&In);
        }

        void Purge(Pm_Widget *Del, uint16_t Type = 0, uint16_t Param = 0);

        void LockInput(void) {mLockCount++;}
        void UnlockInput(void);

       #ifdef PRISM_THREADS_SUPPORT

        PrismTaskInfo *GetTaskInfo(void) {return mpTaskInfo;}
        PRISM_QUEUE_TYPE GetWidgetEventQueue(Pm_Widget* pTarget);
        PRISM_QUEUE_TYPE GetCurrentEventQueue(void);
        pm_bool_t RouteEventToUITask(pm_event_t *pEvent);
        pm_bool_t RouteEventToThread(pm_event_t *pEvent, PRISM_TASK_TYPE ThreadId);
        void Update(pm_event_t *pIn, PRISM_QUEUE_TYPE pQueue);
        void EventThreadBegin(Pm_Widget *pPanel);
        void UITaskTerminate(Pm_Widget *pPanel);
        void EventThreadTerminate(PRISM_TASK_TYPE ThreadID);
       #endif

        void Update(pm_event_t *pIn);

    private:

        pm_int_t mLockCount;

       #ifdef PRISM_THREADS_SUPPORT
        PrismTaskInfo *mpTaskInfo;
       #endif 

       #if defined(PRISM_STANDALONE_EVENT_MANAGER)
        pm_event_t *mpFirst;
        pm_event_t *mpLast;
        pm_event_t *mpFree;
        pm_event_t *mpFreeEnd;
       #endif
};

END_NAMESPACE_PRISM

#endif






