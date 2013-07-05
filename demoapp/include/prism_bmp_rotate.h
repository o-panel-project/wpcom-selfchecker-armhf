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
//  prism_bmp_rotate.h - Prism bitmap rotator class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#if defined(PRISM_BITMAP_ROTATE)

#ifndef _PRISM_ROTATOR_
#define _PRISM_ROTATOR_

#include "prism_fp_math.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_fp_point_t
{
    pm_fixed_t x;
    pm_fixed_t y;

    pm_fp_point_t()
    {
    }

    pm_fp_point_t(pm_fixed_t InX, pm_fixed_t InY)
    {
        x = InX;
        y = InY;
    }
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_rgbquad_t
{
    pm_fixed_t red;
    pm_fixed_t green;
    pm_fixed_t blue;
    pm_fixed_t alpha;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_alpha_t
{
    pm_fixed_t Color;
    pm_fixed_t Alpha;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_int_list
{
    pm_int_list *pNext;
    int Index;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Bitmap_Rotator
{
public:
    pm_bitmap_t *RotateBitmap(pm_bitmap_t *src, pm_int_t Deg);

    pm_bitmap_t *FastRotateBitmap(pm_bitmap_t *pSrc,
        pm_int_t rotation);

    pm_bitmap_t *RotateText(pm_bitmap_t *pSrc, pm_int_t Deg);

private:

    // These functions are called for the special cases of
    // 90, 180, and 270 degree rotations

    pm_bitmap_t *FastRotate16(pm_bitmap_t *pMap, pm_int_t Rotation);
    pm_bitmap_t *FastRotate24(pm_bitmap_t *pMap, pm_int_t Rotation);
    pm_bitmap_t *FastRotate32(pm_bitmap_t *pMap, pm_int_t Rotation);

    // routines converted from Aarot source code:

    pm_bool_t IsInSquare(pm_fp_point_t, pm_fp_point_t &);

    pm_fixed_t CalcArea();
    uint8_t Limit(pm_fixed_t Fix);
    pm_fixed_t PixOverlap(pm_fp_point_t *, pm_fp_point_t);

     pm_fixed_t RotMin(pm_fixed_t &a, pm_fixed_t &b)
    {
        if (a < b)
        {
            return a;
        }
        return b;
    }

     pm_fixed_t RotMax(pm_fixed_t &Val1, pm_fixed_t &Val2)
    {
        if (Val1 > Val2)
        {
            return Val1;
        }
        return Val2;
    }

    pm_fixed_t RotCos(pm_int_t);
    pm_fixed_t RotSin(pm_int_t);

    void SortPoints();

    pm_fp_point_t *PolyOverlap;
    pm_fp_point_t *PolySorted;
    pm_fp_point_t *Corners;

    pm_bitmap_t *Rotate(pm_bitmap_t *pSrc, pm_int_t Angle);
    pm_bitmap_t *RotateAlpha(pm_bitmap_t *pSrc, pm_int_t Angle);

    pm_fixed_t Sin_s;
    pm_fixed_t Cos_s;

    int PolyOverlapSize;
    int PolySortedSize;
};

#endif      
#endif     

