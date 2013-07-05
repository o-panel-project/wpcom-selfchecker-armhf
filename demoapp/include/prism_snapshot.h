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
//  prism_snapshot.h - Prism Snapshot class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_SNAPSHOT_
#define _PRISM_SNAPSHOT_

BEGIN_NAMESPACE_PRISM

class Pm_Snapshot
{
    public:
        Pm_Snapshot(void);
        ~Pm_Snapshot();
       
        pm_bitmap_t *Bitmap(void) {return &mBitmap;}

        int32_t  GetDataSize(void) {return mDataSize;}
        pm_bool_t IsValid(void) const {return mValid;}
        void MoveTo(pm_int_t Left, pm_int_t Top);

        Pm_Region &Position(void) {return mRegion;}
        pm_point_t Point(void);
        void Reset(void);

        void SetPos(Pm_Region &Region)
        {
            mRegion = Region;
            mBitmap.Width = (uint16_t) Region.Width();
            mBitmap.Height = (uint16_t) Region.Height();
        }

        void SetValid(pm_bool_t Valid) {mValid = Valid;}
        void SetDataSize(int32_t Size);

        void Shift(pm_int_t xShift, pm_int_t yShift)
        {
            mRegion.Shift(xShift, yShift);
        }

    private:

        pm_bitmap_t mBitmap;
        int32_t     mDataSize;
        Pm_Region   mRegion;
        pm_bool_t   mValid;
};

END_NAMESPACE_PRISM

#endif

