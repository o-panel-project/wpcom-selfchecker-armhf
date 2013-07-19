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
//  prism_progress_bar.h - Prism library progress bar widget
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef _PRISM_PROG_BAR_
#define _PRISM_PROG_BAR_

BEGIN_NAMESPACE_PRISM

const uint32_t PM_PROG_VAL        = 0x00000100;
const uint32_t PM_PROG_RECESSED   = 0x00000200;
const uint32_t PM_PROG_LED        = 0x00000400;
const uint32_t PM_PROG_VERT       = 0x00000800;
const uint32_t PM_PROG_PCNT        = 0x00001000;

class Pm_Progress_Bar : public Pm_Text_Widget
{
    public:
        Pm_Progress_Bar(const Pm_Region &Size,
                       uint32_t Style = PM_BORDER_THIN|
                                        PM_PROG_VAL|
                                        PM_PROG_PCNT, 
                       pm_int_t Min = 0, pm_int_t Max = 100,
                       pm_int_t Current = 0);

        Pm_Btn *GetButton() {return mpButton;}
        pm_int_t GetValue(void) {return mCurrent;}

        virtual void Paint(const Pm_Region &Invalid);
        virtual void ParentShift(pm_int_t x, pm_int_t y);
        void Reconfig(uint32_t Style, pm_int_t Min, pm_int_t Max);
        virtual void Resize(const Pm_Region &Size);
        virtual void SetColor(const uint8_t Index, const pm_uint_t ColorId);

        void Update(pm_int_t Val);

    protected:

        void GetButtonSize(void);

       #ifdef PRISM_GRADIENT_SUPPORT
        pm_gradient_t mGradient;
       #endif

        Pm_Btn     *mpButton;
        Pm_Region   mButtonRegion;
        pm_int_t    mMin;
        pm_int_t    mMax;
        pm_int_t    mCurrent;
};

END_NAMESPACE_PRISM

#endif


