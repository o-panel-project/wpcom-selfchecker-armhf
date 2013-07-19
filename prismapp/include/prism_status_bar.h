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
//  prism_status_bar.h - Pm_Status_Bar class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef _PRISM_STATUS_BAR_
#define _PRISM_STATUS_BAR_

BEGIN_NAMESPACE_PRISM

class Pm_Status_Bar : public Pm_Text_Widget
{
    public:
        Pm_Status_Bar();
        virtual ~Pm_Status_Bar();

        virtual Pm_Prompt *AddTextField(pm_int_t Width, uint16_t Id,
                                        pm_uint_t StringId = 0);

        virtual Pm_Prompt *AddTextField(const pm_char_t *pText,
            pm_int_t Width, uint16_t Id);

        virtual void AssignFont(pm_uint_t FontId);

        Pm_Prompt *GetPrompt(pm_uint_t Id);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventPenDrag(const pm_event_t &Event);
        pm_int_t OnEventMouseNotOver(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);
        virtual void SetTextField(uint16_t Id, pm_uint_t StringId);
        virtual void SetTextField(uint16_t Id, const pm_char_t *pText);
        virtual void SetTextField(uint16_t Id, const char *pText);

    protected:

        void CheckPaintDragMap(Pm_Widget *Check = NULL);

        Pm_Prompt *NewTextField(pm_int_t Width,  uint16_t Id,
            pm_uint_t StringId, const pm_char_t *pText);

        virtual void SizeToParent(void);

        Pm_Prompt *mpLastPrompt;
        pm_bool_t  mShowPointer;
};


#endif

