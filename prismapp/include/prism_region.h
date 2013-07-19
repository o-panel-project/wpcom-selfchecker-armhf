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
//  prism_region.h - Prism Region data type
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_REGION_
#define _PRISM_REGION_

BEGIN_NAMESPACE_PRISM

struct Pm_Region
{
    void Set(pm_point_t UpperLeft, pm_point_t BottomRight)
    {
        Left = UpperLeft.x;
        Top = UpperLeft.y;
        Right = BottomRight.x;
        Bottom = BottomRight.y;
    }

    void Set(pm_int_t xLeft, pm_int_t yTop,
        pm_int_t xRight, pm_int_t yBottom)
    {
        Left = (int16_t) xLeft;
        Top = (int16_t) yTop;
        Right = (int16_t) xRight;
        Bottom = (int16_t) yBottom;
    }

    Pm_Region operator ++(int);
    Pm_Region operator += (int x);
    Pm_Region operator --(int);
    Pm_Region operator -= (int x);

    Pm_Region operator &=(const Pm_Region &Other);
    Pm_Region operator &(const Pm_Region &Size) const;
    Pm_Region operator |= (const Pm_Region &Other);
    Pm_Region operator ^= (const Pm_Region &Size);
    pm_bool_t operator != (const Pm_Region &Size) const;
    pm_bool_t operator == (const Pm_Region &Size) const;

    pm_bool_t Contains(pm_point_t Test) const;
    pm_bool_t Contains(pm_int_t x, pm_int_t y) const;
    pm_bool_t Contains(const Pm_Region &Size) const;

    pm_int_t Height(void) const { return (Bottom - Top + 1);}
    pm_bool_t Overlap(const Pm_Region &Size) const;

    void MoveTo(pm_int_t x, pm_int_t y);
    void Shift(pm_int_t xShift, pm_int_t yShift);                     
    pm_int_t Width(void) const {return (Right - Left + 1);}

    int16_t Left;
    int16_t Top;
    int16_t Right;
    int16_t Bottom;
};

END_NAMESPACE_PRISM

#endif

