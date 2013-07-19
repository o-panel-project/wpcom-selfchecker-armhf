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
//  prism_list.cpp - Prism list type container classes
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_LIST_
#define _PRISM_LIST_

const uint32_t PM_WRAP_STYLE     = 0x00002000;

class Pm_List : public Pm_Panel
{
    public:
        Pm_List(const Pm_Region &Size, pm_uint_t Id = 0, 
                uint32_t Style = PM_BORDER_THIN);

        virtual ~Pm_List();

        virtual pm_int_t Clear();

        virtual pm_int_t GetIndex(Pm_Widget *);
        pm_int_t GetNumItems(void);

        virtual Pm_Widget *GetSelected(void)
        {
            return mpLastSelect;
        }

        virtual pm_int_t GetSelectedIndex(void);
        virtual Pm_Widget *GetWidget(pm_int_t Index);

        virtual void Insert(Pm_Widget *pWho, pm_int_t Where,
            pm_bool_t Select=TRUE, pm_bool_t Show = TRUE);

        virtual void Link(Pm_Widget *pWho, pm_bool_t Show = TRUE);
        virtual void LinkTail(Pm_Widget *pWho, pm_bool_t Show = TRUE);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        Pm_Widget *PageDown(void);
        Pm_Widget *PageUp(void);

        virtual void SetSelected(Pm_Widget *pWho);
        virtual Pm_Widget *SetSelected(pm_int_t Index);
        Pm_Widget *SelectNext(void);
        Pm_Widget *SelectPrevious(void);

        void SetSeparation(pm_int_t Space)
        {
            mInnerItemSpace = Space;
        }
        virtual Pm_Widget *Unlink(Pm_Widget *pWho);

    protected:

        virtual void PositionChildren(void) = 0;
        void MakeSelectedVisible();

        Pm_Widget *mpLastSelect;

        pm_int_t mInnerItemSpace;
        pm_bool_t mLastChanged;
};


///////////////////////////////////////////////////////////////////////////////
class Pm_Vert_List : public Pm_List
{
    public:
        Pm_Vert_List(const Pm_Region &Size, pm_uint_t Id = 0,
                    uint32_t Style = PM_BORDER_THIN);

    protected:
        virtual void PositionChildren(void);
        
};
///////////////////////////////////////////////////////////////////////////////
class Pm_Horz_List : public Pm_List
{
    public:
        Pm_Horz_List(const Pm_Region &Size, pm_uint_t Id = 0,
                    uint32_t Style = PM_BORDER_THIN);

    protected:
        virtual void PositionChildren(void);
};

///////////////////////////////////////////////////////////////////////////////


#endif


