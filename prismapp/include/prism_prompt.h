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
//  prism_prompt.h - Prism text output field
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_PROMPT_
#define _PRISM_PROMPT_

BEGIN_NAMESPACE_PRISM

class Pm_Prompt : public Pm_Text_Widget
{
    public:

        Pm_Prompt(const Pm_Region &Size, pm_uint_t StringId = 0,
                  uint16_t Id = 0,
                  uint32_t Style = PM_BORDER_NONE|
                                   PM_JUSTIFY_LEFT|
                                   PM_PAINT_TRANS);

        Pm_Prompt(const pm_char_t *pText, const Pm_Region &Size,
                  uint16_t Id = 0,
                  uint32_t Style = PM_PRIVATE_COPY|
                                   PM_BORDER_NONE|
                                   PM_JUSTIFY_LEFT|
                                   PM_PAINT_TRANS);

        Pm_Prompt(const pm_point_t &Put, pm_uint_t StringId = 0,
                  uint16_t Id = 0,
                  uint32_t Style = PM_BORDER_NONE|
                                   PM_JUSTIFY_LEFT|
                                   PM_PAINT_TRANS,
                  pm_uint_t FontId = 0);

        Pm_Prompt(const pm_char_t *pText, const pm_point_t &Put,
                  uint16_t Id = 0,
                  uint32_t Style = PM_PRIVATE_COPY|
                                   PM_BORDER_NONE|
                                   PM_JUSTIFY_LEFT|
                                   PM_PAINT_TRANS,
                  pm_uint_t FontId = 0);

        virtual ~Pm_Prompt();

        void AutoSize(const pm_point_t &Put);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);
        virtual void Paint(const Pm_Region &Invalid);

		void SetPadding(pm_int_t n)	// add worldpicom
		{
			mPadding = n;
		}

    protected:
        void PromptInit(uint32_t Style);

		pm_int_t mPadding;	// add worldpicom
};

END_NAMESPACE_PRISM

#endif

