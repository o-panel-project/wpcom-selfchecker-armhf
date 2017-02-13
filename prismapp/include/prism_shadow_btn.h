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
//  prism_shadow_btn.h - Prism shadow button class declaration.
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_SHADOW_BTN_
#define _PRISM_SHADOW_BTN_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Shadow_Btn : public Pm_Bmp_Btn
{
    public:
        Pm_Shadow_Btn(const Pm_Region &Size, pm_uint_t NormalBmp,
            pm_uint_t PressedBmp, pm_uint_t FocusedBmp = 0,
            uint16_t Id = 0,
            uint32_t Style =PM_ACTIVE|
                            PM_BORDER_NONE|
                            PM_ALIGN_HCENTER|
                            PM_ALIGN_VCENTER|
                            PM_BTN_HOVER);

        virtual pm_int_t Notify(const pm_event_t &Event);
        pm_int_t OnEventFocusChange(const pm_event_t &Event);
        virtual void Paint(const Pm_Region &Invalid);

    protected:
};

END_NAMESPACE_PRISM

#endif
