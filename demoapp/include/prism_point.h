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
//  prism_point.h - Prism library point data type
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_POINT_T_
#define _PRISM_POINT_T_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct pm_point_t
{
    void Set(pm_int_t InX, pm_int_t InY)
    {
        x = (int16_t) InX;
        y = (int16_t) InY;
    }

    pm_point_t operator +(const pm_point_t &Point) const
    {
        pm_point_t NewPt;
        NewPt.Set(x + Point.x, y + Point.y);
        return NewPt;
    }

    pm_point_t operator -(const pm_point_t &Point) const
    {
        pm_point_t NewPt;
        NewPt.Set(x - Point.x, y - Point.y);    
        return NewPt;
    }

    pm_bool_t operator == (const pm_point_t &InPoint) const
    {
        if (x == InPoint.x && y == InPoint.y)
        {
            return TRUE;
        }
        return FALSE;
    }

    pm_bool_t operator != (const pm_point_t &InPoint) const 
    {
        if (x != InPoint.x || y != InPoint.y)
        {
            return TRUE;
        }
        return FALSE;
    }
    int16_t x;
    int16_t y;
};

END_NAMESPACE_PRISM

#endif 
