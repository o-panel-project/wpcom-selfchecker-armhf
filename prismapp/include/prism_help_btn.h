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
//  prism_help_btn.h - Prism help button class implementation
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_HELP_BTN_
#define _PRISM_HELP_BTN_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Help_Prompt : public Pm_Prompt
{
    public:
        Pm_Help_Prompt(Pm_Region &Size, pm_int_t TextId);
        virtual void Paint(const Pm_Region &Invalid);

};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Help_Btn : public Pm_Icon_Btn
{
    public:
        Pm_Help_Btn(Pm_Region &Size, pm_int_t BitmapId,
            pm_int_t TextId, pm_uint_t Id, uint32_t Style = PM_ACTIVE);

        virtual ~Pm_Help_Btn(void);
        virtual pm_int_t Notify(const pm_event_t &Event);
        
        pm_int_t OnEventMouseOver(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPenDrag(const pm_event_t &Event);
        pm_int_t OnEventMouseNotOver(const pm_event_t &Event);
        pm_int_t OnEventTimeout(const pm_event_t &Event);

        void SetSID(pm_int_t Sid) {mSID = Sid;}

    private:

        Pm_Help_Prompt *mpHelpPrompt;
        pm_int_t mSID;
        pm_int_t mYPosition;
        pm_bool_t mClicked;
};

#endif
