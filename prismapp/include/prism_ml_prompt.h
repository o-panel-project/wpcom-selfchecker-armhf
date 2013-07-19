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
//  prism_ml_prompt.h - Prism multi-line text output widget
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_ML_TEXT_OUTPUT_
#define _PRISM_ML_TEXT_OUTPUT_

BEGIN_NAMESPACE_PRISM

#ifndef PRISM_TEXT_BORDER
#define PRISM_TEXT_BORDER 2     // white space to leave on borders
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_ML_Prompt : public Pm_Panel
{
    public:
        Pm_ML_Prompt(const Pm_Region &Size, pm_uint_t StringId = 0,
                   uint16_t Id = 0,
                   uint32_t Style = PM_BORDER_RECESSED|
                                    PM_AUTOWRAP|
                                    PM_JUSTIFY_LEFT,
                   pm_int_t MaxChars = 1024);

        virtual ~Pm_ML_Prompt();

        virtual void Append(pm_int_t StringId);
        virtual void Append(const pm_char_t *pText);

        virtual void AssignFont(pm_uint_t FontIndex);
        virtual void AssignText(pm_uint_t StringId);
        virtual void AssignText(const pm_char_t *Text);
        void AssignTextAndReset(pm_int_t StringId);
        void AssignTextAndReset(const pm_char_t *pText);

        const pm_char_t *FindLineStart(pm_int_t Line);

        pm_int_t GetLineIndex(pm_int_t Line);
        const pm_char_t *GetLineStart(pm_int_t Line, pm_int_t *pPutLen);

        pm_int_t GetMaxChars(void) {return mMaxChars;}
        pm_int_t GetTopLine(void) {return mTopLine;}

       #if defined(PRISM_SCROLL_SUPPORT)
        virtual void GetHScrollInfo(pm_scroll_info_t *pPut);
        virtual void GetVScrollInfo(pm_scroll_info_t *pPut);
       #endif

        pm_int_t GetWidestLine(void)
        {
            return mWidestLine;
        }

        pm_int_t LineCount(void) {return mTotalLines;}

        pm_bool_t LineDown(void);
        pm_bool_t LineUp(void);

        void MarkLine(pm_int_t MarkLine) {mMarkLine = MarkLine;}
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventVScroll(const pm_event_t &Event);
        pm_int_t OnEventHScroll(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        pm_int_t OnEventFocusChange(const pm_event_t &Event);

		pm_int_t OnEventPenUp(const pm_event_t &Event);		// worldpicom
		pm_int_t OnEventPenDown(const pm_event_t &Event);	// worldpicom

        virtual void Paint(const Pm_Region &Invalid);
   
        virtual void Resize(const Pm_Region &NewSize);
        void ResizeBuffer(pm_int_t Length);

        void SetMarkedTextBackColor(pm_uint_t ColorId)
        {
            mHiTextBackColor = ColorId;
        }

        void SetBorderWhitespace(pm_int_t Space);
        void SetInterlineSpace(pm_int_t Space);
        void SetMaxChars(pm_int_t MaxChars) {mMaxChars = MaxChars;}

        #ifdef PRISM_SCROLL_SUPPORT
        virtual void SetScrollMode(pm_uint_t Mode)
        {
            Pm_Panel::SetScrollMode(Mode);
            UpdateLineStarts();
        }
        #endif

        void SetTopLine(pm_int_t Line);
        void ScrollToEnd(void);

        void SetVertCenter(pm_bool_t bVertCenter) {mbVertCenter = bVertCenter;} // worldpicom

    protected:

        void ClipToClient(const Pm_Region &Invalid);
        virtual void PaintAllText(const Pm_Region &Invalid);
        virtual void PaintTextLine(pm_int_t Line, pm_point_t PutPoint, 
                                  pm_bool_t Fill = FALSE);
        void EndClipToClient(void);

        void BuildLineStartPtrs(pm_int_t StartLine, pm_bool_t Full);
        virtual const pm_char_t *FindNextLine(const pm_char_t *pText,
            pm_int_t MaxWidth, pm_int_t Dir = 1);

        void Initialize(uint16_t ID, uint32_t Style, pm_int_t MaxChars);

        void ReassignText(void);
        void UpdateLineStarts(void);

        pm_int_t *mpLineStarts;
        pm_char_t *mpBuffer;

        pm_int_t  mLineStartTop;
        pm_int_t  mLineStartEnd;

        pm_int_t  mMarkLine;
        pm_int_t  mTopLine;

        pm_uint_t mHiTextBackColor;
        pm_int_t  mWidestLine;
        pm_int_t  mLeftOffset;
        pm_int_t  mTotalLines;
        pm_int_t  mVisibleLines;
        pm_int_t  mLineHeight;
        pm_int_t  mInterLineSpace;
        pm_int_t  mWhiteSpace;
        pm_int_t  mBufferLength;
        pm_int_t  mMaxChars;

        pm_bool_t mbVertCenter; // worldpicom
};

END_NAMESPACE_PRISM

#endif

