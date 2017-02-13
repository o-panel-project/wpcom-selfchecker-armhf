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
//  prism_paint_gradient.h - Prism gradient painting engine definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_GRADIENT_HPP_
#define _PRISM_GRADIENT_HPP_

#ifdef PRISM_GRADIENT_SUPPORT

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const uint32_t PM_GRDNT_VERT       = 0x00000001;
const uint32_t PM_GRDNT_HORZ       = 0x00000002;

const uint32_t PM_GRDNT_MASK       = 0x00000003;
const uint32_t PM_GRDNT_COS        = 0x00000010;
const uint32_t PM_GRDNT_MIRROR     = 0x00000020;
const uint32_t PM_GRDNT_DITHER     = 0x00000040;

#define MAX_GRADIENT_COLORS 8
#define MAX_GRADIENT_LEN 256

struct pm_gradient_step_t
{
    pm_color_t Color;
    pm_int_t Pos;
};

struct pm_gradient_t
{
    pm_gradient_t *pNext;
    pm_bitmap_t *pBmp;
    uint32_t Style;
    pm_int_t Width;
    pm_int_t Height;
    pm_int_t ColorCount;
    pm_gradient_step_t Colors[MAX_GRADIENT_COLORS];
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Gradient
{
    public:
        Pm_Gradient();
        ~Pm_Gradient();

        void PaintGradient(Pm_Widget *pCaller, Pm_Region &Size,
                          pm_gradient_t &Gradient);

        int32_t GetGradientColor(pm_int_t Index);
        pm_color_t Get(pm_int_t Index);

        pm_bitmap_t *MakeGradient(const pm_gradient_t &Grad);

        void Set(pm_int_t Len, int32_t First, int32_t Last);
        void Set(pm_int_t Len);

        pm_int_t Root(pm_int_t n);
        pm_int_t Distance(pm_point_t Point1, pm_point_t Point2);

    private:

        void PaintHorizGradient(Pm_Widget *pCaller, Pm_Region Size);
        void PaintVertGradient(Pm_Widget *pCaller, Pm_Region Size);

        void SimpleInterpolate();
        void CosineInterpolate(pm_int_t CosVal);

        pm_bool_t Dither(pm_bitmap_t *pMap);
        void DitherPixel(int32_t c, pm_color_t *pPut, pm_int_t Index);
        void EndDither(void);
        pm_bool_t InitDither(pm_uint_t cols);

        pm_bitmap_t *GetHorizGradient();
        pm_bitmap_t *GetVertGradient();

        int32_t mColorTable[MAX_GRADIENT_LEN];
        pm_gradient_t mGradient;

        int32_t *mpThisRerr;
        int32_t *mpNextRerr;
        int32_t *mpThisGerr;
        int32_t *mpNextGerr;
        int32_t *mpThisBerr;
        int32_t *mpNextBerr;
        pm_bool_t mDitherDirection;

        int32_t mRedStart;
        int32_t mGreenStart;
        int32_t mBlueStart;

        int32_t mRedEnd;
        int32_t mGreenEnd;
        int32_t mBlueEnd;

        int32_t mRedStep;
        int32_t mGreenStep;
        int32_t mBlueStep;

        int32_t mCurrentRed;
        int32_t mCurrentGreen;
        int32_t mCurrentBlue;
        int32_t mCount;

        pm_int_t mRedSign;
        pm_int_t mGreenSign;
        pm_int_t mBlueSign;

        pm_int_t mRedDelta;
        pm_int_t mGreeDelta;
        pm_int_t mBlueDelta;

        pm_int_t mRedConst;
        pm_int_t mGreenConst;
        pm_int_t mBlueConst;

        pm_int_t mLength;
        pm_int_t mStart;

};

#endif 

END_NAMESPACE_PRISM

#endif



