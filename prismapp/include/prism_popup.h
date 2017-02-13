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
//  prism_popup.h - Prism library popup panel class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_POPUP_
#define _PRISM_POPUP_

BEGIN_NAMESPACE_PRISM

const uint32_t PM_POPUP_OK          = 0x00000020;
const uint32_t PM_POPUP_YES         = 0x00000040;
const uint32_t PM_POPUP_NO          = 0x00000080;
const uint32_t PM_POPUP_ABORT       = 0x00000100;
const uint32_t PM_POPUP_RETRY       = 0x00000200;
const uint32_t PM_POPUP_CANCEL      = 0x00000400;


///////////////////////////////////////////////////////////////////////////////
class Pm_Popup_Panel : public Pm_Decorated_Panel
{
    public:

        Pm_Popup_Panel(const Pm_Region &Size, pm_int_t TitleStringId,
                         pm_int_t MessageId = 0,
                         uint32_t Style = PM_POPUP_OK|PM_BORDER_RAISED,
                         pm_int_t Icon =0, Pm_Widget *pOwner = NULL);

        Pm_Popup_Panel(pm_int_t TitleStringId,
                         pm_int_t MessageId = 0,
                         uint32_t Style = PM_POPUP_OK|PM_BORDER_RAISED,
                         pm_int_t Icon = 0, Pm_Widget *pOwner = NULL);

        virtual ~Pm_Popup_Panel();

       #if defined(PRISM_TITLE_BAR)
        virtual void AssignTitle(const pm_char_t *pTitle);
       #endif

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);

       #ifdef PRISM_TITLE_BAR
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
       #endif

        pm_int_t OnEventLostFocus(const pm_event_t &Event);
        pm_int_t OnEventCENClose(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);
        virtual void TextChanged(void);

    protected:

        void AddButtons(void);
        void CalcButtonWidth(void);
        void RemoveButtons(void);

        void Reconfigure(void);

        Pm_Widget *mpOwner;
        pm_int_t mIcon;
        pm_int_t mButtonWidth;
        pm_int_t mOneButtonWidth;
        pm_int_t mMessageWidth;
        pm_int_t mTitleWidth;
        pm_bool_t mAutoSize;
};

END_NAMESPACE_PRISM

#endif

