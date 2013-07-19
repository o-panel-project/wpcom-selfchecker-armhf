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
//  prism_ml_popup.h - Prism popup panel with multiple text lines
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_ML_POPUP_PANEL_
#define _PRISM_ML_POPUP_PANEL_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////

class Pm_ML_Popup_Panel : public Pm_Panel
{
    public:

        Pm_ML_Popup_Panel(const pm_char_t *pTitle,
                           const pm_char_t *pMessage,
                           uint32_t Style = PM_POPUP_OK|PM_BORDER_RAISED,
                           pm_int_t Icon = 0, Pm_Widget *pOwner = NULL);

        Pm_ML_Popup_Panel(const Pm_Region &Size, pm_uint_t TitleStringId,
                           pm_uint_t MessageId = 0,
                           uint32_t Style = PM_POPUP_OK|PM_BORDER_RAISED,
                           pm_int_t Icon = 0, Pm_Widget *pOwner = NULL);

        Pm_ML_Popup_Panel(pm_uint_t TitleStringId,
                           pm_uint_t MessageId = 0,
                           uint32_t Style = PM_POPUP_OK|PM_BORDER_RAISED,
                           pm_int_t Icon = 0, Pm_Widget *pOwner = NULL);


        virtual ~Pm_ML_Popup_Panel();

        Pm_ML_Prompt *GetTextWidget(void) {return mpTextWidget;}
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);
        pm_int_t OnEventClose(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);

    protected:

        void AddButtons();
        void AddTextWidget(pm_int_t MessageId);
        void AddTextWidget(const pm_char_t *pMessage);
        void CalcButtonTop();
        void CalcButtonWidth();

        Pm_Widget *mpOwner;
        Pm_ML_Prompt* mpTextWidget;
        pm_int_t mButtonWidth;
        pm_int_t mOneButtonWidth;
        pm_int_t mButtonTop;
        pm_int_t mIconId;
};

END_NAMESPACE_PRISM

#endif  


