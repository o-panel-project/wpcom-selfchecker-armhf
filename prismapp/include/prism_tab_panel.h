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
//  prism_tab_panel.h - Prism tab control class implementation
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_TAB_PANEL_
#define _PRISM_TAB_PANEL_

const uint32_t PM_TAB_ON_TOP      = 0x00000100;        
const uint32_t PM_TAB_ON_BOTTOM   = 0x00000200;
const uint32_t PM_TABTYPE_TEXT    = 0x00000400;

class Pm_Tab_Panel : public Pm_Panel
{
    public:
        Pm_Tab_Panel(const Pm_Region &Size, uint32_t Style,
            uint8_t NumTabs);

        virtual ~Pm_Tab_Panel();

        virtual void AssignFont(pm_uint_t FontId);
        void CalculateTabPositions(void);
        uint8_t GetCurrentPage(void) {return mSelectedTab;}
        Pm_Widget *GetPageClient(uint8_t Index);
        Pm_Widget *GetTabClient(uint8_t Index);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventSpinUp(const pm_event_t &Event);
        pm_int_t OnEventResize(const pm_event_t &Event);
        pm_int_t OnEventSpinDown(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventGainedInput(const pm_event_t &Event);
        pm_int_t OnEventLostInput(const pm_event_t &Event);
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        pm_int_t OnEventNewLanguage(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);

        virtual void PositionTabClients(void);

        Pm_Widget *RemovePageClient(uint8_t Index);

        virtual void Resize(const Pm_Region &NewSize);

        void ResetNumTabs(uint8_t Num);
        void ResetTabStyle(uint32_t Style);
        void SelectTab(uint8_t Tab);

        void SetPageClient(uint8_t Index, Pm_Widget *pPageClient);
        void SetTabClient(uint8_t Index, Pm_Widget *pTabClient);
        void SetTabString(uint8_t Index, pm_int_t StringId);
        void SetTabString(uint8_t Index, const pm_char_t *String);

        enum MyIds 
        {
            TAB_SPIN = 1000
        };

    protected:

        virtual void AddCurrentPageClient();
        pm_bool_t CheckTabClick(pm_point_t Click);
        virtual void PaintFrame(pm_bool_t Fill = TRUE);
        void PaintTabs(const Pm_Region &Invalid, pm_bool_t Selected);
        void RepaintTabs(void);

        pm_char_t **mpTabStrings;

        Pm_Widget **mpTabClients;
        Pm_Widget **mpPageClients;

        pm_int_t mTabWidth;
        pm_int_t mTabHeight;
        uint8_t mFirstTab;
        uint8_t mNumTabs;
        uint8_t mSelectedTab;
        uint8_t mVisibleTabs;
};

#endif


