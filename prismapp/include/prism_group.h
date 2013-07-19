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
//  prism_group.h - 
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_GROUP_
#define _PRISM_GROUP_

class Pm_Group : public Pm_Text_Widget
{
    public:
        Pm_Group(const Pm_Region &Size, pm_int_t StringId = 0,
                 uint32_t Style = PM_ACTIVE);

        virtual ~Pm_Group() {}

        virtual void Disable(void);
        virtual void Enable(void);
        virtual void Link(Pm_Widget *pWho, pm_bool_t Showed = TRUE);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventGainedInput(const pm_event_t &Event);

       #if defined(PRISM_TAB_KEY_SUPPORT)
        pm_int_t DefaultKeyHandler(const pm_event_t &Event);
       #else
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
       #endif
       #endif

        virtual void Paint(const Pm_Region &Invalid);
        virtual Pm_Widget *Unlink(Pm_Widget *pWho);

    protected:
};

#endif

