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
//  prism_widget.h - Prism graphical widget
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_WIDGET_
#define _PRISM_WIDGET_

BEGIN_NAMESPACE_PRISM

#define PM_EVENT_HANDLER (pm_int_t (Pm_Widget::*)(const pm_event_t &))

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct {
    uint32_t EventType;
    pm_int_t (Pm_Widget::*Handler) (const pm_event_t &Event);
} pm_event_table_entry;


const uint32_t PM_BORDER_NONE       = 0x00000001;
const uint32_t PM_BORDER_THIN       = 0x00000002;
const uint32_t PM_BORDER_RECESSED   = 0x00000004;
const uint32_t PM_BORDER_RAISED     = 0x00000008;
const uint32_t PM_BORDER_THICK      = 0x00000010;
const uint32_t PM_BORDER_MASK       = 0x0000001f;

#if defined(PRISM_FLICK_GESTURE_SUPPORT)
const uint32_t PM_FLICK_MASTER       = 0x01000000;
const uint32_t PM_FLICK_SLAVE        = 0x02000000;
#endif

const uint32_t PM_PAINT_SELECTED  = 0x20000000;
const uint32_t PM_PAINT_TRANS     = 0x40000000;
const uint32_t PM_ACTIVE          = 0x80000000;


class Pm_Widget
{
    friend class Pm_Screen_Manager;

   #if defined(PRISM_INSIGHT)
    friend class SelectWin;   
   #endif
 
    public:

        Pm_Widget(const Pm_Region &Size,
                  uint16_t Id = 0,
                  uint32_t Style = PM_BORDER_NONE);

        Pm_Widget(uint16_t Id = 0,
                  uint32_t Style = PM_BORDER_NONE);

        virtual ~Pm_Widget();

        virtual void AddStatus(uint32_t OrVal) {mStatus |= OrVal;}
        virtual void AddStyle(uint32_t OrVal) {mStyle |= OrVal;}

        void CaptureInput(pm_bool_t Reset = FALSE);

        void Center(Pm_Widget *pWho);
        pm_point_t CenterOf(Pm_Widget *pWho);

        void ClipChildren(void);

        pm_bool_t Contains(const pm_point_t &Spot)
        {
            return mClip.Contains(Spot);
        }

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t DefaultKeyHandler(const pm_event_t &InEvent);
       #endif

        void Destroy(Pm_Widget *pWho);

        int32_t Distance(pm_point_t p1, pm_point_t p2);

        virtual Pm_Widget *Find(uint16_t Id, pm_bool_t Recursive = TRUE);

        Pm_Widget *First (void) const {return mpFirst;}

        uint16_t FrameStyle(void) {return ((uint16_t) (mStyle & PM_BORDER_MASK));}

        void FrameStyle(uint32_t Style)
        {
            mStyle &= ~PM_BORDER_MASK;
            AddStyle(Style);
        }

       #if defined(PRISM_KEYPAD_SUPPORT) && defined(PRISM_ARROW_KEY_SUPPORT)
        Pm_Widget *GetClosestSibling(Pm_Widget *pStart,
                    pm_int_t Key, pm_point_t Center);

        Pm_Widget *GetClosestSibling(Pm_Widget *pStart, pm_int_t Key,
                 pm_point_t Center, int32_t *pPutDist, pm_bool_t Sloppy);
       #endif 

        virtual pm_uint_t GetColor(const uint8_t Index);
        uint16_t GetEventMask(void) {return mEventMask;}
        pm_portal_t *GetFirstPortal(void) { return mpPortalList; }  
        uint16_t GetId(void) {return mId;}
        uint32_t GetStatus(void) {return mStatus;}
        uint32_t GetStyle(void) {return mStyle;}
        uint16_t GetType(void) { return mType; }

       #if defined(PRISM_KEYPAD_SUPPORT) && defined(PRISM_ARROW_KEY_SUPPORT)
        pm_bool_t HandleArrowKey(pm_int_t Key);
       #endif

        void Hide(Pm_Widget *pChild);
        void InitializeClientRegion(void);

       #if defined(PRISM_KEYPAD_SUPPORT) && defined(PRISM_TAB_KEY_SUPPORT)
        void InitializeTabOrder(void);
       #endif

        void Invalidate(const Pm_Region &Size);
        
        void Invalidate(void)
        {
            Invalidate(mSize);
        }

        pm_bool_t IsDescendentOf(Pm_Widget *pParent);

        virtual void KillFocus(Pm_Widget *);

        inline void KillTimer(pm_int_t TimerId)
        {
            Pm_Framework::TimerManager()->DestroyTimer(this, TimerId);
        }

        virtual void Link(Pm_Widget *pWho, pm_bool_t DoShow = TRUE);
        virtual void LinkTail(Pm_Widget *pWho, pm_bool_t DoShow = TRUE);

        virtual void MoveFocusToFirstClientChild(void);
        void MoveToFront(Pm_Widget *pWho);

        Pm_Widget *Next (void) const {return mpNext;}

       #if defined(PRISM_KEYPAD_SUPPORT) && defined(PRISM_TAB_KEY_SUPPORT)
        Pm_Widget *NextTabLink(void);
       #endif

        virtual pm_int_t Notify(const pm_event_t &Event);
        void NotifyChildren(const pm_event_t &Event);
        pm_int_t NotifyParent(const pm_event_t &Event);

        pm_int_t OnEventCanvasChange(const pm_event_t &Event);
        pm_int_t OnEventDestroy(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventHide(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventKeypadPush(const pm_event_t &InEvent);
       #endif

        pm_int_t OnEventLink(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPaint(const pm_event_t &Event);
        pm_int_t OnEventResize(const pm_event_t &Event);
        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventUnLink(const pm_event_t &Event);

        inline pm_int_t OpenCanvas(const Pm_Region &Invalid)
        {
            return (Pm_Framework::Engine()->OpenCanvas(this, Invalid, mCanvas));
        }

        inline pm_int_t OpenCanvas(const Pm_Region &Invalid, pm_int_t Canvas)
        {
            return (Pm_Framework::Engine()->OpenCanvas(this, Invalid, Canvas));
        }

        virtual void Paint(const Pm_Region &Invalid);
        void PaintBorder(pm_uint_t ColorId);
        void PaintChildren(const Pm_Region &Invalid);

        Pm_Widget *Parent (void) const {return mpParent;}
        virtual void ParentShift(pm_int_t x, pm_int_t y);

        Pm_Widget *Previous(void) const {return mpPrev;}

       #if defined(PRISM_KEYPAD_SUPPORT) && defined(PRISM_TAB_KEY_SUPPORT)
        Pm_Widget *PrevTabLink(void);
       #endif

        void ReleaseInput(void);

        virtual void RemoveStatus(uint32_t AndVal) {mStatus &= ~AndVal;}
        virtual void RemoveStyle(uint32_t AndVal) {mStyle &= ~AndVal;}

        virtual void Resize(const Pm_Region &Size);

        void SendEvent(uint8_t Event);

        pm_bool_t SendMaskedEvent(uint8_t Event)
        {
            if (mpParent && mId && (mEventMask & CEN_MASK((pm_uint_t)Event)) &&
                mpParent->StatusIs(PM_SF_VISIBLE))
            {
                SendEvent(Event);
                return TRUE;
            }
            return FALSE;
        }

        void SetCanvas(pm_int_t Canvas, pm_bool_t bRecursive = TRUE);
        virtual void SetColor(const uint8_t Index, const pm_uint_t ColorId);

        void SetEventMask(uint16_t Mask) {mEventMask = Mask;}

        void SetEventMask(uint16_t Id, uint16_t Mask)
        {
            mId = Id;
            mEventMask = Mask;
        }

        void SetId(uint16_t Id) {mId = Id;}

        virtual void SetStyle(uint32_t Style) {mStyle = Style;}

       #if defined(PRISM_KEYPAD_SUPPORT) && defined(PRISM_TAB_KEY_SUPPORT)
        void SetTabLink(Pm_Widget *pNext) {mpChainLink = pNext;}
        void SetTabOrder(uint16_t *pIds);
       #endif

        void SetType(uint16_t Set) {mType = Set;}

        pm_bool_t Show(Pm_Widget *pWho);

        inline void StartTimer(uint16_t Id, int32_t Count, int32_t Reset)
        {
            Pm_Framework::TimerManager()->StartTimer(this, Id, Count, Reset);
        }

        pm_bool_t  StatusIs(uint32_t Mask)
        {
            if (mStatus & Mask)
            {
                return TRUE;
            }
            return FALSE;
        }

        pm_bool_t  StyleIs(uint32_t Mask)
        {
            if (mStyle & Mask)
            {
                return TRUE;
            }
            return FALSE;
        }

        virtual Pm_Widget *Unlink(Pm_Widget *pWho);

        const pm_char_t *Version(void);
        
        pm_int_t mCanvas;
        Pm_Region mSize;
        Pm_Region mClient;
        Pm_Region mClip;
        pm_uint_t  mColorTable[4];

    protected:

        Pm_Widget *mpFirst;
        Pm_Widget *mpNext;
        Pm_Widget *mpParent;
        Pm_Widget *mpPrev;
        pm_portal_t *mpPortalList;

       #if defined(PRISM_KEYPAD_SUPPORT) && defined(PRISM_TAB_KEY_SUPPORT)
        Pm_Widget *mpChainLink;
       #endif

        uint32_t  mStatus;
        uint32_t  mStyle;

        uint16_t mType;
        uint16_t mId;
        uint16_t mEventMask;
};



END_NAMESPACE_PRISM

#endif




