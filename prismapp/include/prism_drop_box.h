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
//  prism_includes.h - Prism master header file, includes all API header files
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_DROP_BOX_
#define _PRISM_DROP_BOX_

class Pm_Drop_Box;     

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Drop_List : public Pm_Vert_List
{
    public:
        Pm_Drop_List(Pm_Widget *pBox, Pm_Region &Size, uint16_t Id,
                  uint32_t Style);

        virtual void CloseList();

        virtual pm_int_t Notify(const pm_event_t &Event);

        virtual void PositionChildren(void)
        {
            Pm_Vert_List::PositionChildren();
        }

    private:

        Pm_Widget *mpBox;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Drop_Box : public Pm_Widget
{
    public:

        Pm_Drop_Box(const Pm_Region &Size, uint16_t Id = 0, 
                    uint32_t Style = PM_BORDER_THIN);

        virtual ~Pm_Drop_Box();

        virtual pm_int_t Clear()
        {
            return mpList->Clear();
        }
        void CloseList(void);

        virtual Pm_Widget *Find(uint16_t Id, pm_bool_t Recursive = TRUE);

        virtual pm_int_t GetIndex(Pm_Widget *pWho)
        {
            return mpList->GetIndex(pWho);
        }

        pm_int_t GetCloseHeight(void)
        {
            return mCloseHeight;
        }

        Pm_Drop_List *GetListPointer()
        {
            return mpList;
        }

        pm_int_t GetNumItems(void)
        {
            return mpList->GetNumItems();
        }
        pm_int_t GetOpenHeight(void)
        {
            return mOpenHeight;
        }

        virtual Pm_Widget *GetSelected(void)
        {
            return mpList->GetSelected();
        }

        virtual pm_int_t GetSelectedIndex(void) 
        {
            return mpList->GetSelectedIndex();
        }
        virtual Pm_Widget *GetWidget(pm_int_t Index) 
        {
            return mpList->GetWidget(Index);
        }

        virtual void Insert(Pm_Widget *pWho, pm_int_t Where, 
                            pm_bool_t Select = TRUE,
                            pm_bool_t Show = TRUE);

        pm_bool_t IsOpen(void) {return mOpen;}

        virtual void Link(Pm_Widget *pChild, pm_bool_t Show = TRUE);
        virtual void LinkTail(Pm_Widget *pChild, pm_bool_t Show = TRUE);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventIdOpenWasClicked(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventHide(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        pm_int_t OnEventNewLanguage(const pm_event_t &Event);

        void OpenList(void);
        virtual void Paint(const Pm_Region &Invalid);

        Pm_Widget *PageDown(void) {return mpList->PageDown();}
        Pm_Widget *PageUp(void) {return mpList->PageUp();}

        virtual void Resize(const Pm_Region &NewSize);
        Pm_Widget *SelectNext(void)
        {
            return mpList->SelectNext();
        }
        Pm_Widget *SelectPrevious(void)
        {
            return mpList->SelectPrevious();
        }
        void SetOpenHeight(pm_int_t Height)
        {
            mOpenHeight = Height;
        }
        void SetScrollMode(pm_uint_t Mode);

        virtual void SetSelected(Pm_Widget *pWho)
        {
            mpList->SetSelected(pWho);
            SnapshotSelectedToBmp();
        }
        virtual Pm_Widget *SetSelected(pm_int_t Index)
        {
            Pm_Widget *pReturn = mpList->SetSelected(Index);
            SnapshotSelectedToBmp();
            return pReturn;
        }

        void SetSeparation(pm_int_t Sep) {mpList->SetSeparation(Sep);}

        virtual Pm_Widget *Unlink(Pm_Widget *pChild);

        enum ButtonIds {
            PM_BTN_ID_OPEN = 1000
        };

    protected:

        void CalcClosedHeight(void);
        void SnapshotSelectedToBmp(void);
        void SendListSelect(void);

        Pm_Icon_Btn *mpOpenButton;
        Pm_Drop_List *mpList;
        pm_bitmap_t *mpBitmap;
        Pm_Widget *mpLastSelectWidget;

        pm_int_t mOpenHeight;
        pm_int_t mCloseHeight;

        pm_bool_t mOpen;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Text_Drop_Box : public Pm_Text_Btn
{
    public:

        Pm_Text_Drop_Box(const Pm_Region &Size, uint16_t Id = 0, 
                    uint32_t Style = PM_ACTIVE);
        virtual ~Pm_Text_Drop_Box();

        virtual void LinkItem(pm_uint_t StringId, uint16_t Id = 0);
        virtual void LinkItem(const pm_char_t *pText, uint16_t Id = 0);
        virtual void LinkItemToEnd(pm_uint_t StringId, uint16_t Id = 0);
        virtual void LinkItemToEnd(const pm_char_t *pText, uint16_t Id = 0);
        virtual pm_int_t Clear() {return mpList->Clear();}
        void CloseList(void);
        virtual void DisableItem(pm_int_t Index);
        virtual void Paint(const Pm_Region &Invalid);
        virtual void EnableItem(pm_int_t Index);

        virtual Pm_Widget *Find(uint16_t Id, pm_bool_t Recursive = TRUE);
        virtual pm_int_t GetIndex(Pm_Widget *pWho)
        {
            return mpList->GetIndex(pWho);
        }
        Pm_Drop_List *GetListPointer() {return mpList;}
        pm_int_t GetNumItems(void) {return mpList->GetNumItems();}
        pm_int_t GetOpenHeight(void) {return mOpenHeight;}
        virtual Pm_Widget *GetSelected(void) {return mpList->GetSelected();}

        virtual pm_int_t GetSelectedIndex(void) 
        {
            return mpList->GetSelectedIndex();
        }
        virtual Pm_Widget *GetWidget(pm_int_t Index) 
        {
            return mpList->GetWidget(Index);
        }

        virtual void InsertItem(pm_uint_t StringId, pm_int_t Where, 
                            pm_bool_t Select = TRUE, uint16_t Id = 0);
        virtual void InsertItem(const pm_char_t *pText, pm_int_t Where, 
                            pm_bool_t Select = TRUE, uint16_t Id = 0);
        pm_bool_t IsOpen(void) {return mOpen;}

        virtual pm_int_t Notify(const pm_event_t &Event);

        void OpenList(void);

        Pm_Widget *PageDown(void) {return mpList->PageDown();}
        Pm_Widget *PageUp(void) {return mpList->PageUp();}

        virtual void RemoveItem(pm_int_t Index);

        virtual void Select(Pm_Widget *pWho)
        {
            mpList->SetSelected(pWho);
            SnapshotSelected();
        }
        virtual Pm_Widget *Select(pm_int_t Index)
        {
            Pm_Widget *pReturn = mpList->SetSelected(Index);
            SnapshotSelected();
            return pReturn;
        }

        Pm_Widget *SelectNext(void) {return mpList->SelectNext();}
        Pm_Widget *SelectPrevious(void) {return mpList->SelectPrevious();}
        void SetOpenHeight(pm_int_t Height) {mOpenHeight = Height;}
        void SetScrollMode(pm_uint_t Mode);
        void SetSeparation(pm_int_t Sep) {mpList->SetSeparation(Sep);}

        void SetTextColor(uint8_t Index, pm_uint_t ColorId);
        void SetTextFont(pm_uint_t FontId);

    protected:

        void SnapshotSelected(void);
        void SendListSelect(void);

        virtual Pm_Widget *ConstructItem(pm_uint_t StringId, uint16_t Id);
        virtual Pm_Widget *ConstructItem(const pm_char_t *pText, uint16_t Id);

        pm_uint_t mTextColorIds[4];

        Pm_Drop_List *mpList;
        Pm_Widget *mpLastSelectWidget;

        pm_uint_t mTextFontId;
        pm_int_t mOpenHeight;
        pm_bool_t mOpen;
};

#endif


