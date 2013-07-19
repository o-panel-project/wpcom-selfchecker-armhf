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
//  prism_input_field.h - Prism text input field
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_INPUT_FIELD_
#define _PRISM_INPUT_FIELD_

BEGIN_NAMESPACE_PRISM

const uint32_t PM_ALLOW_EDIT     = 0x00000100;
const uint32_t PM_AUTOWRAP       = 0x00000400;
const uint32_t PM_SELECT_ALL     = 0x00000800;
// Edit Style:


typedef struct {

} StateFlags;


class Pm_Text_Input : public Pm_Text_Widget
{
    public:

        Pm_Text_Input(const Pm_Region &Size,
                     pm_uint_t StringId = 0,
                     uint16_t Id = 0,
                     uint32_t Style = PM_BORDER_RECESSED|PM_ACTIVE|PM_ALLOW_EDIT,
                     pm_int_t Len = -1);

        virtual ~Pm_Text_Input();

        void CopyToScratchPad(void);
        virtual void TextChanged(void);
 
        void DeleteMarkedText(void);
        virtual void Paint(const Pm_Region &Invalid);

        virtual pm_int_t GetMarkEnd(void) {return mMarkEnd;}
        virtual pm_int_t GetMarkStart(void) {return mMarkStart;}

        pm_int_t GetMaxLen(void);

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventHide(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventLostInput(const pm_event_t &Event);
        pm_int_t OnEventGainedInput(const pm_event_t &Event);
       #else
        pm_int_t OnEventLostFocus(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
       #endif

        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPenUp(const pm_event_t &Event);

       #ifdef PRISM_MOUSE_SUPPORT
        pm_int_t OnEventPenDrag(const pm_event_t &Event);
        pm_int_t OnEventMouseOver(const pm_event_t &Event);
        pm_int_t OnEventMouseNotOver(const pm_event_t &Event);
       #endif

        pm_int_t OnEventCut(const pm_event_t &Event);
        pm_int_t OnEventCopy(const pm_event_t &Event);
        pm_int_t OnEventPaste(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        virtual void ParentShift(pm_int_t x, pm_int_t y);

        void PasteFromScratchPad(void);
        virtual void SetMark(pm_int_t Start, pm_int_t End);
        virtual void SetMark(pm_char_t *pMarkStart, pm_char_t *pMarkEnd);

        void SetMaxLen(pm_int_t Length);
        virtual void SetStyle(uint32_t Style);

    protected:
        virtual void AdvanceCursor(pm_int_t New);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_bool_t CheckControlKey(pm_int_t Key, pm_int_t Flags);
       #endif

        virtual void Initialize(uint32_t Style);
        virtual pm_bool_t InsertKey(pm_int_t Key);
        virtual void PaintMarked(void);
        void ReplaceMarkedText(pm_int_t Key);
        virtual void RetardCursor(pm_int_t New);
        virtual void SetCursorPos(pm_point_t PickPoint);

        pm_point_t mCursorPos;

        int8_t mEditMode;
        int8_t mMarked;
        int8_t mFullMark;
        int8_t mMarking;
        int8_t mHasEdits;

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_char_t *mpBackup;
       #endif

        pm_int_t mMaxLength;
        pm_int_t mCursorIndex;
        pm_int_t mMarkStart;
        pm_int_t mMarkEnd;
        pm_int_t mMarkAnchor;
        pm_int_t mFirstVisibleChar;
};

END_NAMESPACE_PRISM

#endif

