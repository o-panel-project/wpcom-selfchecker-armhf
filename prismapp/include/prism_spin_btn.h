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
//  prism_spin_btn.h - Prism event manager class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_SPIN_
#define _PRISM_SPIN_

const uint32_t PM_SPIN_VERT        = 0x00001000;

class Pm_Spin_Btn : public Pm_Widget
{
    public:
        Pm_Spin_Btn(const Pm_Region &Size, pm_uint_t Id = 0,
                      uint32_t Style = PM_ACTIVE|PM_SPIN_VERT);
        Pm_Spin_Btn(const Pm_Region &Size, Pm_Text_Widget *buddy,
                      int32_t Min, int32_t Max, pm_int_t Step, pm_uint_t Id = 0,
                      uint32_t Style = PM_ACTIVE|PM_SPIN_VERT);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventUpRightClick(const pm_event_t &Event);
        pm_int_t OnEventDownLeftClick(const pm_event_t &Event);

        void SetBuddy(Pm_Text_Widget *buddy)
        {
            mpSlave = buddy;
        }
        void SetLimits(int32_t Min, int32_t Max, pm_int_t Step = -1);
        void SetOutputWidth(pm_int_t Width) {mOutputWidth = Width;}

        enum MyButtonIds 
        {
            PM_BTN_ID_UPBUTTON = 1010,
            PM_BTN_ID_DOWNBUTTON,
            PM_BTN_ID_LEFTBUTTON,
            PM_BTN_ID_RIGHTBUTTON
        };
                
    protected:

        void AddSpinButtons(void);
        void UpdateSlave(pm_uint_t Dir);

        Pm_Text_Widget *mpSlave;
        int32_t mMin;
        int32_t mMax;
        pm_int_t mStep;
        pm_int_t mOutputWidth;
};

#endif

