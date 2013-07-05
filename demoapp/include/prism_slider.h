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
//  prism_slider.h - Prism slider widget class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISMSLIDER_
#define _PRISMSLIDER_

BEGIN_NAMESPACE_PRISM

const uint32_t PM_SLDR_SNAP        = 0x00000100;
const uint32_t PM_SLDR_SCALE       = 0x00000200;
const uint32_t PM_SLDR_WEST        = 0x00000020;
const uint32_t PM_SLDR_NORTH       = 0x00000020;
const uint32_t PM_SLDR_NORTH_UP    = 0x00000040;
const uint32_t PM_SLDR_EAST_UP     = 0x00000040;
const uint32_t PM_SLDR_VERT        = 0x00000080;
const uint32_t PM_SLDR_TICS        = 0x00000100;
const uint32_t PM_SLDR_USER_PATH   = 0x00000200;
const uint32_t PM_SLDR_USER_TRVL   = 0x00000400;
const uint32_t PM_SLDR_THIN        = 0x00000800;
const uint32_t PM_SLDR_THICK       = 0x00001000;
const uint32_t PM_SLDR_POLY        = 0x00002000;
const uint32_t PM_SLDR_EAST        = 0x00004000;
const uint32_t PM_SLDR_SOUTH       = 0x00004000;

const uint32_t PM_SLDR_STD_STYLE   = PM_SLDR_WEST|PM_SLDR_NORTH_UP|
                                    PM_SLDR_VERT|PM_SLDR_TICS|
                                    PM_SLDR_POLY;


enum Pm_SliderBitmaps {
    PM_BI_SLIDER_BACKGROUND = 0,
    PM_BI_SLIDER_NEEDLE
};


///////////////////////////////////////////////////////////////////////////////
class Pm_Slide_Btn : public Pm_Widget
{
    public:
        Pm_Slide_Btn(pm_int_t Min, pm_int_t Max, pm_int_t Scale,
                       pm_uint_t Flags = 0);
        virtual ~Pm_Slide_Btn();

        int32_t CheckRange(int32_t Val);
        pm_int_t GetCurrentValue(void) {return mCurrent;}
        void KeyMove(pm_int_t Direction);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventHide(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPenUp(const pm_event_t &Event);
        pm_int_t OnEventPenDrag(const pm_event_t &Event);

       #ifdef PRISM_HOVER_SUPPORT
        pm_int_t OnEventMouseOver(const pm_event_t &Event);
        pm_int_t OnEventMouseNotOver(const pm_event_t &Event);
       #endif

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        virtual void Paint(const Pm_Region &Invalid);
        virtual void ParentShift(pm_int_t xShift, pm_int_t yShift);

        void Reset(pm_int_t Min, pm_int_t Max, pm_int_t New);

        void SetCurrentValue(pm_int_t iNewVal);
        void SetMaxValue(pm_int_t Max);
        void SetMinValue(pm_int_t Min);
        void SetOffset(pm_int_t Offset)
        {
            mOffset = Offset;
        }

        void SetScale(pm_int_t Scale)
        {
            mScale = Scale;
            SetSize();
        }

    protected:

        virtual void CalculatePosition(void);
        void SnapToPos(pm_point_t DragPos, pm_bool_t Vertical);
        virtual void SetSize(void);
        virtual void UpdateHorzPosition(pm_int_t Delta);
        virtual void UpdatePosition(pm_point_t Point);
        void UpdateVertPosition(pm_int_t Delta);

        #if defined(PRISM_GRADIENT_BUTTONS)
        pm_gradient_t mGradient;
        #endif

        pm_point_t mBeginDrag;
        pm_bool_t mDragMode;
        pm_bool_t mIsHover;

        pm_int_t mMin;
        pm_int_t mMax;
        pm_int_t mCurrent;
        pm_int_t mScale;
        pm_int_t mDimension;
        pm_int_t mOffset;
};


///////////////////////////////////////////////////////////////////////////////
class Pm_Slider : public Pm_Widget
{
    public:
        Pm_Slider(const Pm_Region &Size, int32_t Min, int32_t Max,
                  pm_uint_t Id = 0, uint32_t Style = PM_BORDER_RAISED,
                  int32_t Scale = -1, pm_bool_t CreateButton = TRUE);

        virtual ~Pm_Slider();

        virtual void Paint(const Pm_Region &Invalid);

        virtual pm_int_t GetCurrentValue(void)
        {
            return mpSlideButton->GetCurrentValue();
        }

        virtual pm_int_t GetMaxValue(void) {return mMax;}
        virtual pm_int_t GetMinValue(void) {return mMin;}
        pm_int_t GetNeedleOffset() const { return mNeedleOffset; }
        virtual pm_int_t GetScale(void)    {return mScale;}
        virtual pm_int_t IncrementValue(pm_int_t Val);

        pm_int_t MinTravelPix(void);
        pm_int_t MaxTravelPix(void);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventSliderMove(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        virtual void Reset(pm_int_t Min, pm_int_t Max, pm_int_t New);

        virtual void SetCurrentValue(int32_t NewVal)
        {
            mpSlideButton->SetCurrentValue(NewVal);
        }

        void SetMaxTravelOffset(pm_int_t Offset)
        {
            mMaxTravel = Offset;
        }
        virtual void SetMaxValue(int32_t Max);
        void SetMinTravelOffset(pm_int_t Offset)
        {
            mMinTravel = Offset;
        }
        virtual void SetMinValue(int32_t Min);
        void SetNeedleOffset(pm_int_t Offset)
        {
            mpSlideButton->SetOffset(Offset);
        }
        virtual void SetScale(pm_int_t Scale)
        {
            mScale = Scale;
            mpSlideButton->SetScale(Scale);
        }

        virtual void SetStyle(uint32_t Style);

        pm_int_t ValToPos(pm_int_t Val);
        pm_int_t PosToVal(pm_int_t Pos);

    protected:

        void PaintBitmapNeedle();
        void PaintScale(void);
        void PaintToComposite();
        virtual void SetTravelPoints();

        Pm_Slide_Btn *mpSlideButton;

        pm_int_t mMin;
        pm_int_t mMax;
        pm_int_t mScale;

        pm_int_t mMinTravel;
        pm_int_t mMaxTravel;
        pm_int_t mNeedleOffset;
};



///////////////////////////////////////////////////////////////////////////////
class Pm_Bmp_Slide_Btn : public Pm_Slide_Btn
{
    public:
        Pm_Bmp_Slide_Btn(pm_int_t Min, pm_int_t Max, pm_int_t Scale,
                       pm_uint_t NeedleBmp = 0, pm_uint_t Flags = 0);

        virtual void Paint(const Pm_Region &Invalid);
        pm_uint_t GetBitmap() { return mBitmapId; }
        void AssignBitmap(pm_uint_t BmpId) { mBitmapId = BmpId; }

    protected:
        virtual void SetSize(void);

        pm_uint_t mBitmapId;

};

///////////////////////////////////////////////////////////////////////////////
class Pm_Bmp_Slider : public Pm_Slider
{
    public:

        Pm_Bmp_Slider(const Pm_Region &Size, int32_t Min, int32_t Max,
                        pm_uint_t BkgndBmp = 0, pm_uint_t NeedleBmp = 0,
                        pm_uint_t Id = 0, uint32_t Style = PM_BORDER_RAISED,
                        int32_t Scale = -1);

        virtual void Paint(const Pm_Region &Invalid);

        pm_uint_t GetBitmap(pm_uint_t Index) const {
            if (Index)
            {
                return (((Pm_Bmp_Slide_Btn *) mpSlideButton)->GetBitmap());
            }
            else
            {
                return mBkgBitmap;
            }
        }

        virtual void AssignBitmap(pm_int_t Index, pm_uint_t BmpId)
        {
            if (Index)
            {
                ((Pm_Bmp_Slide_Btn *)mpSlideButton)->AssignBitmap(BmpId);
            }
            else
            {
                mBkgBitmap = BmpId;
            }
        }

    protected:
        void PaintScale(void);
        pm_uint_t mBkgBitmap;
        pm_uint_t mCompBitmap;

};

END_NAMESPACE_PRISM

#endif

