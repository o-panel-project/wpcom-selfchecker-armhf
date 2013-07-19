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
//  prism_tree_view.h - Prism tree view files
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_TREE_VIEW_
#define _PRISM_TREE_VIEW_

class Pm_Tree_View;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Tree_Node : public Pm_Text_Widget
{
    friend class Pm_Tree_View;

    public:
        Pm_Tree_Node(pm_int_t StringId = 0, pm_int_t BmpId = 0);
        Pm_Tree_Node(const pm_char_t *pText, pm_int_t BmpId = 0);

        pm_int_t BranchHeight();
        pm_int_t BranchWidth(Pm_Tree_View *Parent);
        void Close(void) {mOpen = FALSE;}
        pm_int_t Count(void);

        Pm_Tree_Node *FirstNode(void)
        {
            return (Pm_Tree_Node *) First();
        }
        void ForceOpen(void) {mOpen = TRUE;}
        pm_int_t GetMap(void) {return mBitmapId;}

        void Insert(Pm_Tree_Node *Sib);
        pm_bool_t IsOpen(void) {return mOpen;}
        pm_bool_t IsPushed(void) {return mSelected;}

        virtual void Link(Pm_Widget *pChild, pm_bool_t DoShow = FALSE);
        void LinkAbove(Pm_Widget *pChild, pm_bool_t DoShow = FALSE);

        void MoveToTop(Pm_Tree_Node *pChild);

        Pm_Tree_Node *NextNode(void)
        {
            return (Pm_Tree_Node *) Next();
        }
        Pm_Tree_Node *NodeAbove(void);
        Pm_Tree_Node *NodeBelow(void);
        Pm_Tree_Node *NodeBottom(void);

        pm_int_t NodeHeight();
        pm_int_t NodeWidth();

        void Open(void) {if (First()) mOpen = TRUE;}
        Pm_Tree_Node *ParentNode(void)
        {
            return (Pm_Tree_Node *) Parent();
        }
        void SetMap(pm_int_t BitmapId) {mBitmapId = BitmapId;}
        void SetNext(Pm_Widget *Next) {mpNext = Next;}
        void SetSelected(pm_bool_t Selected){mSelected = Selected;}

    protected:

        pm_bool_t mOpen;
        pm_bool_t mSelected;
        pm_int_t mBitmapId;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Pm_Tree_View : public Pm_Panel
{
    public:
        Pm_Tree_View(const Pm_Region &Size, pm_int_t StringId,
            uint32_t Style, pm_int_t BitmapId = 0);

        Pm_Tree_View(const Pm_Region &Size, const pm_char_t *pText,
            uint32_t Style, pm_int_t BitmapId = 0);

        virtual ~Pm_Tree_View();

        void DestroyNode(Pm_Tree_Node *pWho);

        Pm_Tree_Node *FindNode(pm_int_t Level, const pm_char_t *Text);

        virtual void GetHScrollInfo(pm_scroll_info_t *pPut);
        pm_int_t GetIndent(void) {return mIndent;}
        Pm_Tree_Node *GetSelected(void) {return mpSelected;}
        virtual void GetVScrollInfo(pm_scroll_info_t *pPut);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventRPenUp(const pm_event_t &Event);
        pm_int_t OnEventVScroll(const pm_event_t &Event);
        pm_int_t OnEventHScroll(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventGainedInput(const pm_event_t &Event);
        pm_int_t OnEventLostInput(const pm_event_t &Event);
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        virtual void Paint(const Pm_Region &Invalid);
        void PaintNode(Pm_Tree_Node *pStart, pm_point_t Put,
            pm_int_t MaxMapWidth);

        Pm_Tree_Node *RemoveNode(Pm_Tree_Node *pWho);
        void Reset(pm_int_t TopStringId);
        void Reset(const pm_char_t *pText);

        void Select(Pm_Tree_Node *pWho);
        void SetIndent(pm_int_t Val) {mIndent = Val;}
        virtual void ToggleBranch(Pm_Tree_Node *);
        Pm_Tree_Node *TopNode(void) {return mpTopNode;}

    protected:

        void CalcNodePositions(Pm_Tree_Node *pStart, pm_point_t Put,
                               pm_int_t MaxMapWidth);

        pm_bool_t CheckNodeClick(pm_point_t Click, Pm_Tree_Node *pStart);

        void Construct(pm_int_t StringId, const pm_char_t *pText,
            pm_int_t TopMap);

        Pm_Tree_Node *FindSubNode(Pm_Tree_Node *pTop, pm_int_t CurLevel,
                          pm_int_t FindLevel, const pm_char_t *pText);

        Pm_Tree_Node *mpTopNode;
        Pm_Tree_Node *mpSelected;

        pm_int_t mTopOffset;
        pm_int_t mSearchLevel;
        pm_int_t mLeftOffset;
        pm_int_t mIndent;
};



#endif

