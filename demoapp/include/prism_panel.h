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
//  prism_panel.h - Prism panel class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_PANEL_
#define _PRISM_PANEL_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef PRISM_SCROLL_SUPPORT
const uint8_t PM_SM_VSCROLL       = 0x01;
const uint8_t PM_SM_HSCROLL       = 0x02;
const uint8_t PM_SM_AUTOVSCROLL   = 0x04;
const uint8_t PM_SM_AUTOHSCROLL   = 0x08;
const uint8_t PM_SM_AUTOSCROLL    = 0x0c;
const uint8_t PM_SM_CONTINUOUS    = 0x10;
#endif

///////////////////////////////////////////////////////////////////////////////

#define PANEL_MOVEMODE_LEFT        0x01
#define PANEL_MOVEMODE_RIGHT       0x02
#define PANEL_MOVEMODE_TOP         0x04
#define PANEL_MOVEMODE_BOTTOM      0x08

#define PANEL_MOVEMODE_ALL         0x80

#define PANEL_MOVEMODE_UL          0x05
#define PANEL_MOVEMODE_UR          0x06
#define PANEL_MOVEMODE_LL          0x09
#define PANEL_MOVEMODE_LR          0x0a

///////////////////////////////////////////////////////////////////////////////

#if defined(PRISM_MULTIPLE_CANVASES)

enum Pm_PanelBlendModes {

    PANEL_BLEND_NONE = 0,
    PANEL_BLEND_OVERLAY,
    PANEL_BLEND_ALPHA
};

#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

enum PM_PANEL_TRANSITION_TYPES
{
    PM_TRANS_TYPE_NONE = 0,
    PM_TRANS_TYPE_SLIDE_UP,
    PM_TRANS_TYPE_SLIDE_DOWN,
    PM_TRANS_TYPE_SLIDE_LEFT,
    PM_TRANS_TYPE_SLIDE_RIGHT,

    PM_TRANS_TYPE_FADE_IN,
    PM_TRANS_TYPE_FADE_OUT,
    NUM_PM_TRANS_TYPES
};

///////////////////////////////////////////////////////////////////////////////
enum PM_PANEL_TRANS_STATES
{
    PM_TRANS_STATE_NONE = 0,
    PM_TRANS_STATE_ENTER,
    PM_TRANS_STATE_EXIT
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct pm_transition_info_t {
    pm_point_t TargetPos;
    pm_int_t XStep;
    pm_int_t YStep;

    pm_int_t   HardwareCanvas;
    pm_bool_t  DestroyOnExit;

    uint8_t State;
    uint8_t FadeStart;
    uint8_t FadeStop;
    uint8_t EnterMode;
    uint8_t ExitMode;
    uint8_t ActiveMode;
    uint8_t Steps;
    uint8_t Timer;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Pm_Panel : public Pm_Text_Widget
{
    friend class Pm_Paint_Engine;

    public:
        Pm_Panel(const Pm_Region &Size, uint32_t Style = PM_BORDER_THICK);
        Pm_Panel(uint32_t Style = PM_BORDER_THICK);
        virtual ~Pm_Panel();

        virtual void AddIcon(Pm_Icon *pIcon);

        virtual void BeginEnterTransition(void);
        virtual void BeginExitTransition(void);

       #if defined(PRISM_MULTIPLE_CANVASES)
        uint8_t GetBlendMode(void)
        {
            return mBlendMode;
        }
        uint8_t GetBlendRatio(void)
        {
            return mBlendRatio;
        }
       #endif

        #ifdef PRISM_SCROLL_SUPPORT

        pm_bool_t CheckAutoScroll(void);
        virtual void GetHScrollInfo(pm_scroll_info_t *pPut);
        virtual void GetVScrollInfo(pm_scroll_info_t *pPut);
        virtual Pm_Scroll *CreateHScroll()
        {
            return new Pm_Horz_Scroll();
        }
        virtual Pm_Scroll *CreateVScroll()
        {
            return new Pm_Vert_Scroll();
        }

        #endif

       #if defined(PRISM_MOUSE_SUPPORT) || defined(PRISM_TOUCH_SUPPORT)
        uint8_t CurrentMoveMode(void)
        {
            return mMoveMode;
        }
       #endif

        virtual void Paint(const Pm_Region &Invalid);
        virtual void PaintFrame(pm_bool_t Fill = TRUE);
        virtual pm_int_t Execute(pm_bool_t AutoAdd = TRUE);

        virtual pm_uint_t GetIcon(void) {return mIcon;}

       #ifdef PRISM_THREADS_SUPPORT
        pm_int_t GlobalModalExecute(void);
       #endif

        virtual void InitializeClientRegion(void);
        pm_bool_t IsMaximized(void) { return(mMaximized ? TRUE : FALSE); }
        pm_bool_t IsModal(void) { return(mIsModal ? TRUE : FALSE); }

        virtual void Link(Pm_Widget *pWho, pm_bool_t Show = TRUE);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventPenDrag(const pm_event_t &Event);

       #if defined(PRISM_MOUSE_SUPPORT)
        pm_int_t OnEventPenMove(const pm_event_t &Event);
        pm_int_t OnEventMove(const pm_event_t &Event);
        pm_int_t OnEventMouseNotOver(const pm_event_t &Event);
       #endif

        pm_int_t OnEventTimeout(const pm_event_t &Event);
        pm_int_t OnEventShow(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventGainedInput(const pm_event_t &Event);
       #endif

        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLinkIcon(const pm_event_t &Event);
   
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPenUp(const pm_event_t &Event);
        pm_int_t OnEventVScroll(const pm_event_t &Event);
        pm_int_t OnEventHScroll(const pm_event_t &Event);
        pm_int_t OnEventSizeToParent(const pm_event_t &Event);
        pm_int_t OnEventRestore(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        pm_int_t OnEventClose(const pm_event_t &Event);
        pm_int_t OnEventCENClose(const pm_event_t &Event);

        virtual void Resize(const Pm_Region &NewSize);
        
       #if defined(PRISM_SCROLL_SUPPORT)
        void ResetScrollBars(void);
       #endif

       #if defined(PRISM_MULTIPLE_CANVASES)
        pm_bool_t SetBlendMode(uint8_t Mode, uint8_t Ratio = 0);

        void SetBlendRatio(uint8_t Ratio)
        {
            mBlendRatio = Ratio;

            if (mCanvas > 0)
            {
                Pm_Framework::Engine()->SetCanvasAlpha(mCanvas, Ratio);
            }
        }

        void SetBlendCanvas(pm_int_t Canvas) { mBlendCanvas = Canvas; }

       #endif

        void SetTransitionMode(uint8_t EnterMode,
             uint8_t ExitMode = PM_TRANS_TYPE_NONE,
             uint8_t Steps = 8, uint8_t Timer = 1,
             uint8_t FadeStart = PRISM_MAX_ALPHA / 8,
             uint8_t FadeStop = PRISM_MAX_ALPHA,
             pm_bool_t DestroyOnExit = TRUE,
             pm_int_t HardwareCanvas = -1);

        virtual void SetIcon(pm_uint_t BitmapId) {mIcon = BitmapId;}

        #ifdef PRISM_SCROLL_SUPPORT
        virtual void SetScrollMode(pm_uint_t Mode);
        #endif

        virtual void SetWallpaper(pm_int_t Wallpaper, uint8_t Tile = 1);
        virtual void SetWallpaper(pm_bitmap_t *pWallpaper, uint8_t Tile = 1);

    protected:

       #if defined (PRISM_MOUSE_SUPPORT)
        void CheckPointerType(const pm_event_t &Event);
       #endif

        void CheckMove(const pm_event_t &InEvent);
        void CheckSizeLimit(Pm_Region &Size);

       #if defined(PRISM_MOUSE_SUPPORT)
        virtual uint8_t DoesFrameContain(const pm_point_t &Point);
       #endif

        virtual void DoTransition(void);
        virtual void EnterTransCompleted(void) {}
        virtual void ExitTransCompleted(void);

       #if defined(PRISM_SCROLL_SUPPORT)
        void FillScrollCorner(void);
       #endif

        virtual void MoveClientObjects(pm_int_t xShift, pm_int_t yShift,
                                       pm_bool_t Repaint = TRUE);

        void CanvasChange(void);
 
        Pm_Region mRestoreSize;

       #ifdef PRISM_SCROLL_SUPPORT
        Pm_Scroll *mpVScroll;
        Pm_Scroll *mpHScroll;
       #endif

        pm_transition_info_t *mpTransitionInfo;

        pm_bitmap_t *mpWallpaper;
        pm_uint_t mWallpaper;
        pm_uint_t mIcon;

       #ifdef PRISM_MULTIPLE_CANVASES
        pm_int_t mBlendCanvas;
        uint8_t mBlendMode;
        uint8_t mBlendRatio;
       #endif

       #if defined(PRISM_SCROLL_SUPPORT)
        uint8_t mScrollMode;
       #endif

       #if defined(PRISM_MOUSE_SUPPORT) || defined(PRISM_TOUCH_SUPPORT)
        pm_point_t mStartMovePoint;
        pm_point_t mCurrentMovePoint;
        uint8_t mMoveMode;
        uint8_t mShowPointer;
       #endif
        
        uint8_t mIsModal;
        uint8_t mMaximized;
        uint8_t mMoveFrame;
        uint8_t mTileFill;
};

END_NAMESPACE_PRISM

#endif

