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
//  prism_dec_panel.h - Pm_Decorated_Panel
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_DEC_PANEL_
#define _PRISM_DEC_PANEL_

///////////////////////////////////////////////////////////////////////////////
class Pm_Decorated_Panel : public Pm_Panel
{
    public:
        Pm_Decorated_Panel(const Pm_Region &Size,
            uint32_t Style = PM_BORDER_THICK);

        Pm_Decorated_Panel(uint32_t Style = PM_BORDER_THICK);
        virtual ~Pm_Decorated_Panel();

        virtual void InitializeClientRegion(void);

        virtual void Link(Pm_Widget *pWho, pm_bool_t Show = TRUE);
        Pm_Menu_Bar *MenuBar(void) { return mpMenuBar; }
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventKeypadRelease(const pm_event_t &Event);
       #endif

       #if defined(PRISM_TITLE_BAR)
        void SetTitle(pm_int_t StringId)
        {
            if (mpTitle)
            {
                mpTitle->AssignText(StringId);
            }
        }
        
        void SetTitle(const pm_char_t* pText)
        {
            if (mpTitle)
            {
                mpTitle->AssignText(pText);
            }
        }
       #endif

        Pm_Status_Bar *StatusBar(void) 
        {
            return mpStatusBar;
        }

       #if defined(PRISM_TITLE_BAR)
        const pm_char_t *Title(void)
        {
            if (mpTitle)
            {
                return (mpTitle->GetText());
            }
            return NULL;
        }

        Pm_Title *TitleObject(void)
        {
            return mpTitle;
        }
       #endif

        Pm_Tool_Bar *ToolBar(void) 
        {
            return mpToolBar;
        }
        virtual Pm_Widget *Unlink(Pm_Widget *pWho);

    protected:
    
       #if defined(PRISM_TITLE_BAR)
        Pm_Title *mpTitle;
       #endif

        Pm_Menu_Bar *mpMenuBar;

        Pm_Tool_Bar *mpToolBar;
        Pm_Status_Bar *mpStatusBar;
};


#endif

