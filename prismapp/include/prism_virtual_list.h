///////////////////////////////////////////////////////////////////////////////
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
//  prism_virtual_list.h - Prism text output field
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_VLIST_
#define _PRISM_VLIST_

class Pm_Virtual_List : public Pm_Panel
{
    public:

        Pm_Virtual_List(const Pm_Region &Size, uint16_t Id = 0,
                  uint32_t Style = PM_BORDER_THIN|PM_JUSTIFY_CENTER);

        virtual ~Pm_Virtual_List();

        virtual void AssignFont(pm_uint_t FontIndex);
        virtual void AssignVirtualList(void *pList, pm_int_t Count);
        virtual pm_int_t GetSelectedIndex(void) { return mLastSelected; }
        virtual void GetVScrollInfo(pm_scroll_info_t *pPut);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventVScroll(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        virtual void PageDown(void);
        virtual void PageUp(void);
        virtual void Resize(const Pm_Region &NewSize);
        virtual void ScrollIntoView(pm_int_t Index,
                pm_bool_t Repaint = TRUE);

        virtual void SelectNext(void);
        virtual void SelectPrevious(void);

        virtual void SetCellHeight(pm_int_t Height);
        virtual void SetSelected(pm_int_t Index);
        virtual void Unselect(pm_bool_t Repaint = TRUE);

    protected:

        virtual void AssignData(pm_int_t Child, pm_int_t ListIndex);
        virtual void CreateChildWidgets(pm_int_t NumWidgets);
        virtual Pm_Widget *CreateOneChildWidget(Pm_Region &Size,
                            pm_int_t Index);
        virtual void CreateListEntries();
        virtual void Select(pm_int_t Index);

        Pm_Widget **mpChildWidgets;
        void *mpListData;
        pm_int_t mLastSelected;
        pm_int_t mNumWidgets;
        pm_int_t mNumEntries;
        pm_int_t mTopEntry;
        pm_int_t mCellHeight;
};

#endif
