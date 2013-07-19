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
//  prism_scroll.h - Prism scroll bar class definitions
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_SCROLL_
#define _PRISM_SCROLL_

const uint32_t PM_SCROLL_END_BTNS    = 0x00001000;
const uint32_t PM_SCROLL_TILE_BKG    = 0x00002000;
const uint32_t PM_SCROLL_TILE_THUMB  = 0x00004000;
const uint32_t PM_SCROLL_SIZE_THUMB  = 0x00008000;

#define PPM_SPIN_VERT  1
#define PSB_HORIZONTAL 0

class Pm_Panel;

struct pm_scroll_info_t
{
    pm_int_t Min;
    pm_int_t Max;
    pm_int_t Current;
    pm_int_t Step;
    pm_int_t Visible;
};



///////////////////////////////////////////////////////////////////////////////
enum PmScrollBitmapIndex {
    PM_BI_SCROLL_UP = 0,
    PM_BI_SCROLL_DN,
    PM_BI_SCROLL_FILL,
    PM_BI_THUMB_FILL,
    PM_BI_THUMB_UP,
    PM_BI_THUMB_DN,
    NUM_SCROLL_BITMAPS
};

#define PM_CI_THUMB_COLOR   PM_CI_FOCUSFILL

///////////////////////////////////////////////////////////////////////////////
struct pm_scroll_paint_info_t
{
    pm_uint_t BitmapIds[NUM_SCROLL_BITMAPS];
    uint32_t Style;
    int16_t MinOffset;
    int16_t MaxOffset;
    int16_t Width;
    int16_t ThumbWidth;
};

///////////////////////////////////////////////////////////////////////////////
class Pm_Scroll_Thumb : public Pm_Btn
{
    public:
        Pm_Scroll_Thumb(pm_bool_t AmVertical, uint32_t Style);
        virtual ~Pm_Scroll_Thumb();

        virtual void Paint(const Pm_Region &Invalid);
        virtual pm_int_t Notify(const pm_event_t &Event);

    protected:

        pm_bool_t MovePaint(pm_int_t Pos);

        Pm_Region mPos;
        pm_int_t mLastMove;
        pm_int_t mMoveStart;
        pm_bool_t mAmVertical;
        pm_bool_t mMoveMode;
};


///////////////////////////////////////////////////////////////////////////////
class Pm_Scroll : public Pm_Widget
{
    public:
        Pm_Scroll(pm_scroll_paint_info_t *pPaintInfo = NULL, pm_bool_t Vertical = TRUE);

        Pm_Scroll(const Pm_Region &InSize, pm_scroll_info_t *si, uint16_t Id = 0,
            pm_scroll_paint_info_t *pPaintInfo = NULL, pm_bool_t Vertical = TRUE);

        virtual ~Pm_Scroll();

        void AssignBitmap(pm_uint_t Index, pm_uint_t BitmapId);

        pm_uint_t GetBitmap(pm_uint_t Index)
        {
            if (Index < NUM_SCROLL_BITMAPS)
            {
                return mPaintInfo.BitmapIds[Index];
            }
            return 0;
        }

        pm_int_t GetMinOffset() { return (pm_int_t) mPaintInfo.MinOffset; }
        pm_int_t GetMaxOffset() { return (pm_int_t) mPaintInfo.MaxOffset; }
        pm_scroll_info_t *GetScrollInfo() {return &mScrollInfo;}
        virtual pm_int_t Notify(const pm_event_t &Event);
        virtual void Reset();
        virtual void Reset(pm_scroll_info_t *);

        void SetPaintInfo(pm_scroll_paint_info_t *pInfo, pm_bool_t Size = TRUE);
        void SetOffsets(pm_int_t Top, pm_int_t Bottom);

    protected:

        virtual void CheckScrollLimits(void);
        virtual void CreateButtons(void) {}

       #if defined(PRISM_SCROLL_SUPPORT)
        virtual void ReadParentScrollInfo(void) = 0;
       #endif

        virtual void SetThumbButtonPos(void) = 0;

        Pm_Scroll_Thumb *mpThumbButton;
        pm_scroll_info_t mScrollInfo;
        pm_scroll_paint_info_t mPaintInfo;
};


///////////////////////////////////////////////////////////////////////////////
class Pm_Vert_Scroll : public Pm_Scroll
{
    public:

        Pm_Vert_Scroll(pm_scroll_paint_info_t *pPaintInfo = NULL);

        Pm_Vert_Scroll(const Pm_Region &InSize, pm_scroll_info_t *pSI, uint16_t Id = 0,
            pm_scroll_paint_info_t *pPaintInfo = NULL);

        virtual ~Pm_Vert_Scroll();

        virtual void Paint(const Pm_Region &Invalid);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnUpButtonClicked(const pm_event_t &Event);
        pm_int_t OnDownButtonClicked(const pm_event_t &Event);
        pm_int_t OnEventVScroll(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);

        virtual void Resize(const Pm_Region &NewSize);

        enum MyButtonIds
        {
           PM_BTN_ID_UPBUTTON = 1020,
           PM_BTN_ID_DOWNBUTTON
        };

    protected:

        virtual void CreateButtons(void);

       #if defined(PRISM_SCROLL_SUPPORT)
        virtual void ReadParentScrollInfo(void);
       #endif

        virtual void SetThumbButtonPos(void);
};

///////////////////////////////////////////////////////////////////////////////
class Pm_Horz_Scroll : public Pm_Scroll
{
    public:

        Pm_Horz_Scroll(pm_scroll_paint_info_t *pPaintInfo = NULL);

        Pm_Horz_Scroll(const Pm_Region &InSize, pm_scroll_info_t *pSI, uint16_t Id = 0,
            pm_scroll_paint_info_t *pPaintInfo = NULL);

        virtual ~Pm_Horz_Scroll();

        virtual void Paint(const Pm_Region &Invalid);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnLeftButtonClicked(const pm_event_t &Event);
        pm_int_t OnRightButtonClicked(const pm_event_t &Event);
        pm_int_t OnEventHScroll(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);

        virtual void Resize(const Pm_Region &NewSize);

        enum MyButtonIds
        {
           PM_BTN_ID_LEFTBUTTON = 1020,
           PM_BTN_ID_RIGHTBUTTON
        };

    protected:

        virtual void CreateButtons(void);

       #if defined(PRISM_SCROLL_SUPPORT)
        virtual void ReadParentScrollInfo(void);
       #endif

        virtual void SetThumbButtonPos(void);
};

#endif

