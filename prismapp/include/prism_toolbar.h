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
//  prism_toolbar.h - Pm_Tool_Bar and Pm_Button_Group widgets
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#include "prism_includes.h"

#ifndef _PRISM_TOOLBAR_
#define _PRISM_TOOLBAR_

const uint32_t PM_TOOLBAR_FITPARENT        = 0x00000800;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Button_Group : public Pm_Widget
{
    public:

        Pm_Button_Group(uint16_t ID = 0);
        Pm_Button_Group(const Pm_Region &Size, uint16_t Id =0);

        virtual ~Pm_Button_Group();

        virtual void Link(Pm_Widget *pWho, pm_bool_t DoShow = TRUE);
        virtual void LinkTail(Pm_Widget *pWho, pm_bool_t DoShow = TRUE);
        virtual void Paint(const Pm_Region &Invalid);
        virtual Pm_Widget *Unlink(Pm_Widget *pWho);

    protected:

        virtual void PositionChildren();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Tool_Bar : public Pm_Widget
{
    public:
        Pm_Tool_Bar(uint16_t Id = 0);
        virtual ~Pm_Tool_Bar();

        virtual void LinkButtonGroup(Pm_Button_Group *pPanel);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventParentSized(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);

        virtual void PositionButtonGroups();
        virtual Pm_Widget *UnlinkButtonGroup(Pm_Widget *pWho);

    protected:

        virtual void SizeToParent();
        virtual void SizeToTallestGroup();
        
        pm_int_t  mGroupHeight;
};

#endif 

