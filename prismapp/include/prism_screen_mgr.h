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
//  prism_screen_mgr.h - Prism library popup panel class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_SCREEN_MGR_
#define _PRISM_SCREEN_MGR_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_portal_t
{
    Pm_Widget *pOwner;
    pm_portal_t *pNext;                
    pm_bool_t BlockHead;              
    Pm_Region  Portal;
};



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if !defined(PRISM_KEYPAD_SUPPORT)
#define ChangeInputWidget(a) mpInputWidget = a
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


class Pm_Screen_Manager : public Pm_Panel
{
    public:
        Pm_Screen_Manager(const Pm_Region &);
        virtual ~Pm_Screen_Manager();

        void AssignFocusTo(Pm_Widget *pCurrent);

        virtual void CaptureInput(Pm_Widget *pWho, pm_bool_t Reset);

        void ClearScratchPad(void);
        virtual pm_int_t DispatchEvent(Pm_Widget *pFrom, pm_event_t *pSend);

        virtual pm_int_t Execute(pm_bool_t AutoAdd = TRUE);

       #if defined(PRISM_PORTALS)
        void FreePortals(Pm_Widget *pStart);
        void GeneratePortalList(Pm_Widget *pStart);
       #endif  

       #if defined(PRISM_GRADIENT_BUTTONS)
        pm_uint_t *GetBtnColorTable(void) {return mBtnColorTable;}
       #endif

        Pm_Widget *GetInputWidget(void) {return mpInputWidget;}
        Pm_Widget *GetPointerOwner(void);

       #if defined(_PRISM_WIN_ENGINE_)
        const pm_char_t *GetScratchPad(void) {return GetWindowsClipboardData();}
       #else
        const pm_char_t *GetScratchPad(void) {return mpScratchPad;}
       #endif

        virtual pm_scroll_paint_info_t *GetVScrollPaintInfo(void)
        {
            return &mVScrollPaintInfo;
        }

        virtual pm_scroll_paint_info_t *GetHScrollPaintInfo(void)
        {
            return &mHScrollPaintInfo;
        }

        int16_t HScrollHeight(void) {return mHScrollPaintInfo.Width;}

       #if defined(PRISM_GRADIENT_BUTTONS)
        void InitBtnColorTable(pm_uint_t *pTable);
        void AssignBtnColor(pm_uint_t Index, pm_uint_t ColorId);
       #endif

        void Invalidate(Pm_Widget *pCaller, const Pm_Region &Size);

        pm_bool_t IsPointerCaptured(void) 
        {
            if (mPointerCaptures)
            {
                return TRUE;
            }
            return FALSE;
        }

        Pm_Widget *LastPointerOver(void) {return mpLastPointerOver;}
        void LastPointerOver(Pm_Widget *pOver) {mpLastPointerOver = pOver;}

        virtual void Link(Pm_Widget *pWho, pm_bool_t DoShow = TRUE);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventTerminate(const pm_event_t &Event);
        pm_int_t OnEventClose(const pm_event_t &Event);
        pm_int_t OnEventMoveFocus(const pm_event_t &Event);
        pm_int_t OnEventDestroy(const pm_event_t &Event);

       #if defined(_PRISM_WIN_ENGINE_) || defined(PRISM_FOR_X11)
        pm_int_t OnEventResize(const pm_event_t &Event);
       #endif

        virtual void PaintInvalid(void);

        Pm_Widget *PanelPop(void);
        Pm_Widget *PanelPop(Pm_Widget *pWho, pm_bool_t Remove = TRUE,
                            pm_bool_t Destroy = TRUE);

        pm_bool_t PanelPush(Pm_Widget *pOld, Pm_Widget *pNew,
                            pm_bool_t RemoveOld = TRUE, 
                            pm_bool_t Center = TRUE,
                            pm_bool_t Modal = FALSE);

        void PanelStackReset(pm_bool_t Remove = FALSE, 
                             pm_bool_t Destroy = TRUE,
                             pm_bool_t Repaint = TRUE);

        void PortalChange(Pm_Widget *pChanged);

        void UpdatePortalList(void);

        void QueueForDelete(Pm_Widget *pDel);

        virtual void ReleaseInput(Pm_Widget *pWho);
        virtual Pm_Widget *Unlink(Pm_Widget *pWho);

        Pm_Widget *SearchForTopWidget(Pm_Widget *pStart,
            pm_point_t Point);

        void SetVScrollPaintInfo(pm_scroll_paint_info_t *pInfo);
        void SetHScrollPaintInfo(pm_scroll_paint_info_t *pInfo);

        void SetExitCallback(void (*pCallback)())
        {
            mpExitCallback = pCallback;
        }
        void SetScratchPad(const pm_char_t *pText);
        void SetUserEventHandler(pm_int_t (*pHandler)(const pm_event_t &Event))
        {
            mpUserEventHandler = pHandler;
        }

        int16_t VScrollWidth(void) {return mVScrollPaintInfo.Width;}
        virtual void WidgetDestroyed(Pm_Widget *pCurrent);
        virtual void WidgetRemoved(Pm_Widget *pCurrent);

    protected:
        
        void AddPortal(Pm_Widget *pTarget, Pm_Region &NewSize);
        void DividePortal(Pm_Widget *pTarget, Pm_Region Top, Pm_Region Bottom);
        void DividePortal(Pm_Widget *pTarget, Pm_Widget *pChild, Pm_Region Under);

       #if defined(PRISM_KEYPAD_SUPPORT)
        void ChangeInputWidget(Pm_Widget *pWho);
       #endif

        void ConsolidateInvalidList(void);

        pm_portal_t *NextFreePortal(void);
        void InsureBranchHasFocus(Pm_Widget *pCurrent);

        virtual void KillFocus(Pm_Widget *pStart);
        void MallocPortals();

        virtual void SetFocus(Pm_Widget *pStart);
        void TrimInvalidSize(Pm_Region &ParentSize,
            Pm_Region &ChildSize);
     
        void (*mpExitCallback)();
        pm_int_t (*mpUserEventHandler)(const pm_event_t &Event);

       #if defined(_PRISM_WIN_ENGINE_)
        void PasteToWindowsClipboard(const pm_char_t *pText);
        pm_char_t *GetWindowsClipboardData(void);
       #endif

        pm_portal_t *mpFreePortalListStart;
        pm_portal_t *mpFreePortalListTail;
        pm_portal_t *mpInvalidList;
        pm_portal_t *mpInvalidListTail;

        Pm_Widget *mpInputWidget;
        Pm_Widget *mpDefaultInputWidget;
        Pm_Widget *mpInvalidListPainting;
        Pm_Widget *mpLastPointerOver;
        Pm_Widget *mpPointerOwners[PRISM_MAX_PTR_CAPTURE_NESTING];
        Pm_Widget **mpScreenStack;
        Pm_Widget *mpDeleteList;

        pm_char_t  *mpScratchPad;
        Pm_Widget *mpPortalChangeStart;

       #if defined(PRISM_GRADIENT_BUTTONS)
        pm_uint_t mBtnColorTable[PM_NUM_BTN_COLORS];
       #endif

        pm_scroll_paint_info_t mVScrollPaintInfo;
        pm_scroll_paint_info_t mHScrollPaintInfo;
        pm_uint_t  mPortalCount;
        pm_int_t   mScreenStackIndex;
        pm_bool_t  mMoveFocusFlag;
        uint8_t mPointerCaptures;
};

END_NAMESPACE_PRISM

#endif

