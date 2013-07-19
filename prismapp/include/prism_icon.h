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
//  prism_icon.h - Prism icon widget
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_ICON_
#define _PRISM_ICON_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Icon : public Pm_Widget
{
    public:
        Pm_Icon(Pm_Widget *pProxy, pm_uint_t BitmapId = 0,
            uint16_t Id = 0, uint32_t Style = PM_BORDER_NONE);

        Pm_Icon(const Pm_Region &Where, pm_uint_t BitmapId = 0,
            uint16_t Id = 0, uint32_t Style = PM_BORDER_NONE);

        Pm_Icon(pm_uint_t BitmapId = 0, uint16_t Id = 0,
            uint32_t Style = PM_BORDER_NONE);

        virtual ~Pm_Icon() {}

        pm_bitmap_t *GetBitmap(void){return mpBitmap;}
        pm_uint_t GetIcon(void){return mBitmapId;}
        Pm_Widget *GetProxy(void) {return mpMaster;}

        virtual pm_int_t Notify(const pm_event_t &Event);
        pm_int_t OnEventPenUp(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);	// worldpicom

        virtual void Paint(const Pm_Region &Invalid);

        virtual void SetIcon(pm_uint_t BitmapId)
        {
            mBitmapId = BitmapId;
            Invalidate();
        }

        virtual void SetIcon(pm_bitmap_t *pBitmap)
        {
            mBitmapId = 0;
            mpBitmap = pBitmap;
            Invalidate();
        }

        void SetProxy(Pm_Widget *pProxy)
        {
            mpMaster = pProxy;
        }

    protected:
        Pm_Widget *mpMaster;
        pm_bitmap_t *mpBitmap;
        pm_uint_t mBitmapId;
};

END_NAMESPACE_PRISM

#endif

