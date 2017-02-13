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
//  prism_progress_panel.h - Pm_Progress_Panel class declaration
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_PROGWIN_
#define _PRISM_PROGWIN_

class Pm_Progress_Panel : public Pm_Popup_Panel
{
    public:

        Pm_Progress_Panel(const Pm_Region &Size, pm_int_t TitleStringId,
                          pm_int_t MessageId, uint16_t EventStyle,
                          uint32_t ProgStyle = PM_BORDER_THIN|
                                               PM_PROG_VAL|
                                               PM_PROG_PCNT,
                          pm_int_t Icon = 0, Pm_Widget *pOwner = NULL);

        virtual ~Pm_Progress_Panel();

        Pm_Progress_Bar *GetProgressBar(void) {return mpProgressBar;}

    protected:

        void AddProgressBar(void);
        Pm_Progress_Bar *mpProgressBar;
        uint32_t mProgressStyle;
};

#endif



