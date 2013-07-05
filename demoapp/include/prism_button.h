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
//  prism_button.h - Prism button base class
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_BUTTON_
#define _PRISM_BUTTON_

BEGIN_NAMESPACE_PRISM

const uint32_t PM_BTN_REPEAT        = 0x00010000;
const uint32_t PM_BTN_PUSHED        = 0x00020000;
const uint32_t PM_BTN_DOWNEVENT     = 0x00040000;
const uint32_t PM_BTN_DIVIDER       = 0x00080000;
const uint32_t PM_BTN_TOGGLE        = 0x00100000;
const uint32_t PM_BTN_EXCLUSIVE     = 0x00200000;
const uint32_t PM_BTN_HOVER         = 0x00400000;

const uint32_t PM_BMP_RIGHT         = 0x00000020;
const uint32_t PM_BMP_LEFT          = 0x00000040;
const uint32_t PM_ALIGN_HCENTER     = 0x00000080;
const uint32_t PM_ALIGN_VCENTER     = 0x00000100;
const uint32_t PM_BMP_TOP           = 0x00000200;
const uint32_t PM_BMP_BOTTOM        = 0x00000400;
const uint32_t PM_BMP_MASK          = 0x000007E0;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

enum PrismButtonIds {
    PM_BTN_ID_OK = 1000,
    PM_BTN_ID_APPLY,
    PM_BTN_ID_YES,
    PM_BTN_ID_NO,
    PM_BTN_ID_CANCEL,
    PM_BTN_ID_RETRY,
    PM_BTN_ID_ABORT,
    PM_BTN_ID_CLOSE
};

#ifndef PM_BTN_WHITESPACE
#define PM_BTN_WHITESPACE 3
#endif

#ifndef PM_BTN_GRADIENT_STEP
#define PM_BTN_GRADIENT_STEP 16
#endif

#ifndef PM_BTN_FRAME_WIDTH
#define PM_BTN_FRAME_WIDTH 4
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Btn : public Pm_Text_Widget
{
    public:
        Pm_Btn(void);

        Pm_Btn(const Pm_Region &Size, pm_uint_t StringId = 0,
            uint16_t Id = 0,
            uint32_t Style = PM_ACTIVE|PM_BORDER_RAISED|PM_BTN_HOVER,
            pm_uint_t FontId = 0);

        virtual ~Pm_Btn();

        void Disable(void);
        void Enable(void);

       #ifdef PRISM_GRADIENT_BUTTONS
        virtual pm_uint_t GetColor(const uint8_t Index);
        virtual uint32_t MakeGradientStyle() { return mGradient.Style; }
       #endif

        virtual void HandleDownClick(pm_int_t ButtonType);
        virtual void HandleUpClick(const pm_event_t &Event);

        virtual void Initialize(uint32_t Style);

        virtual pm_bool_t IsPushed(void)
        {
            if (mStyle & PM_BTN_PUSHED)
            {
                return TRUE;
            }
            return FALSE;
        }

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPenUp(const pm_event_t &Event);
        pm_int_t OnEventPenDrag(const pm_event_t &Event);

       #ifdef PRISM_HOVER_SUPPORT
        pm_int_t OnEventMouseOver(const pm_event_t &Event);
        pm_int_t OnEventMouseNotOver(const pm_event_t &Event);
       #endif

        pm_int_t OnEventHide(const pm_event_t &Event);
        pm_int_t OnEventTimeout(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventLostInput(const pm_event_t &Event);
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
        pm_int_t OnEventKeypadRelease(const pm_event_t &Event);
       #endif

        virtual void Paint(const Pm_Region &Invalid);
        virtual void PaintBorder(pm_uint_t c1, pm_uint_t c2);

       #ifdef PRISM_GRADIENT_BUTTONS
        virtual void Resize(const Pm_Region &Size);
        virtual void SetColor(const uint8_t Index, const pm_uint_t ColorId);
        virtual void SetGradientStyle(uint32_t Style) { mGradient.Style = Style; }
       #endif

        virtual void SetSelected(pm_bool_t Selected = TRUE);
        virtual void SetStyle(uint32_t Style);

    protected:

        void CheckEvent(pm_bool_t Selected);
        virtual void UnselectSiblings(void) const;

       #ifdef PRISM_GRADIENT_BUTTONS
        void UpdateGradientColors();
       #endif

        pm_bool_t mKeyPressed;

       #ifdef PRISM_MOUSE_SUPPORT
        pm_bool_t mIsHover;
       #endif

       #ifdef PRISM_GRADIENT_BUTTONS
        pm_uint_t *mpColorTable;
        pm_gradient_t mGradient;
       #endif

        pm_bool_t mPrivateColors;
};

END_NAMESPACE_PRISM

#endif

