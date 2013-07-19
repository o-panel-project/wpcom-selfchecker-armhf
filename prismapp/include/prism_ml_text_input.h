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
//  prism_ml_text_input.h - Prism multi-line text input widget
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_ML_INPUT_
#define _PRISM_ML_INPUT_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_cursor_position_t 
{
    void Set(pm_int_t InX, pm_int_t InY)
    {
        x = InX;
        y = InY;
    }

    pm_bool_t operator != (const pm_cursor_position_t &InPoint) const 
    {
        if (x != InPoint.x || y != InPoint.y)
        {
            return TRUE;
        }
        return FALSE;
    }
    pm_bool_t operator == (const pm_cursor_position_t &InPoint) const
    {
        if (x == InPoint.x && y == InPoint.y)
        {
            return TRUE;
        }
        return FALSE;
    }

    pm_int_t x;
    pm_int_t y;

};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_ML_Text_Input : public Pm_ML_Prompt
{
    public:

        Pm_ML_Text_Input(const Pm_Region &Size, pm_uint_t StringId = 0,
                   uint16_t Id = 0,
                   uint32_t Style = PM_BORDER_RECESSED|PM_ALLOW_EDIT|PM_AUTOWRAP,
                   pm_uint_t MaxChars = 1000);

        virtual void Append(pm_int_t StringId);
        virtual void Append(const pm_char_t *pText);

        void CopyToScratchPad(void);

        void DeleteMarkedText(void);
        virtual void Paint(const Pm_Region &Invalid);
        pm_cursor_position_t GetCursorRowCol(void) {return mCursor;}
        void HomeCursor(void);

        pm_bool_t InEditMode(void)
        {
            if (State.EditMode)
            {
                return TRUE;
            }
            return FALSE;
        }

        virtual void InsertCharAtCursor(pm_uint_t Key);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventHide(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPenDrag(const pm_event_t &Event);
        pm_int_t OnEventPenUp(const pm_event_t &Event);
        pm_int_t OnEventMouseOver(const pm_event_t &Event);
        pm_int_t OnEventMouseNotOver(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventGainedInput(const pm_event_t &Event);
        pm_int_t OnEventLostInput(const pm_event_t &Event);
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #else
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);
       #endif

        pm_int_t OnEventCopy(const pm_event_t &Event);
        pm_int_t OnEventPaste(const pm_event_t &Event);
        pm_int_t OnEventCut(const pm_event_t &Event);

        void PasteFromScratchPad(void);
        void SetCursorRowCol(pm_int_t Index);

    protected:

       #if defined(PRISM_SCROLL_SUPPORT)
        virtual pm_bool_t AutoScrollLeft(void);
        virtual pm_bool_t AutoScrollUp(void);
        virtual pm_bool_t CheckAutoScrollDown(void);
        virtual pm_bool_t CheckAutoScrollRight(void);
       #endif

       #ifdef PRISM_KEYPAD_SUPPORT
        virtual pm_bool_t CheckControlKey(pm_uint_t Key, pm_int_t Ctrl);
       #endif

        virtual void CheckMarkMove(void);
        virtual void PaintCharAtCursor(pm_int_t Key);
        virtual void PaintCursor(void);
        virtual void PaintMarkedText(pm_int_t Line, pm_point_t PutPoint);
        virtual void PaintOneLine(pm_int_t Line);
        virtual void PaintSelectText(pm_int_t Line, pm_int_t First, pm_int_t Last,
                                    pm_bool_t Sel);
        virtual void PaintTextLine(pm_int_t Line, pm_point_t PutPoint, 
                                  pm_bool_t Fill = FALSE);

        virtual void ExitEditMode(void);
        virtual const pm_char_t *GetCharAtCursor(void);
        virtual void GetCursorPointFromRowCol(void);
        virtual void GetCursorRowColFromClick(pm_point_t Where);
        virtual void GetMarkStartAndEnd(pm_cursor_position_t *pStartMark, 
                                        pm_cursor_position_t *pEndMark);

        virtual pm_bool_t InsertKey(pm_uint_t Key);
        void NewEditAssignText(void);
        virtual void RemoveCharAtCursor(pm_bool_t BackSpace);
        virtual void RemoveCursor(void);
        virtual void ReplaceMarkedText(pm_uint_t Key);
        virtual void SetCursorRowColFromIndex(pm_int_t Index);
        virtual void TextChanged(void);

        pm_cursor_position_t  mCursor;      
        pm_cursor_position_t  mMarkAnchor;   
        pm_cursor_position_t  mMarkEnd;    
        pm_point_t    mCursorPos; 

        struct 
        {
            pm_uint_t EditMode:1;
            pm_uint_t mMarked:1;
            pm_uint_t Marking:1;
            pm_uint_t mPointerSet:1;
        } State;
};

END_NAMESPACE_PRISM

#endif



