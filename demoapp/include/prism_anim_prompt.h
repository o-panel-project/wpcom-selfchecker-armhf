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
//  prism_anim_prompt.h - Prism animated prompt text output widget
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_ANIM_TEXT_
#define _PRISM_ANIM_TEXT_

BEGIN_NAMESPACE_PRISM

const uint32_t PM_ANIM_ONFOCUS  = 0x00001000;     
const uint32_t PM_ANIM_ALWAYS   = 0x00002000;    
const uint32_t PM_ANIM_TRIGGER  = 0x00003000;
const uint32_t PM_ANIM_WRAP     = 0x00004000;
const uint32_t PM_ADD_DOTS      = 0x00008000;     
const uint32_t PM_ANIM_FOREVER  = 0x00010000;     
const uint32_t PM_ANIM_MASK     = 0x0001F000;

///////////////////////////////////////////////////////////////////////////////
// PrismAnimatedPrompt Bitmap Indexes
///////////////////////////////////////////////////////////////////////////////
enum PRISM_AP_BMPS {
    PM_BI_NORM_BACK = 0, 
    PM_BI_SEL_BACK,      
    PM_BI_LEFT,          
    PM_BI_RIGHT          
};

///////////////////////////////////////////////////////////////////////////////
class Pm_Animated_Prompt : public Pm_Prompt
{
    public:

        Pm_Animated_Prompt(const Pm_Region &Size,
                        pm_int_t StringId = 0, uint16_t Id = 0,
                        uint32_t Style = PM_BORDER_NONE|
                                         PM_JUSTIFY_LEFT|
                                         PM_PAINT_TRANS|
                                         PM_ANIM_ALWAYS);

        virtual ~Pm_Animated_Prompt();

        void AssignAllBitmaps(pm_uint_t NormalMap, pm_uint_t SelectedMap,
             pm_uint_t LeftMap, pm_uint_t RightMap);

        virtual void AssignFont(pm_uint_t Font);
        void AssignBitmap(pm_int_t Index, pm_uint_t BitmapId);
        pm_int_t GetBitmap(pm_int_t Index) { return mBitmapTable[Index]; }
        pm_int_t GetTextJustification(void) {return mLeftTextBorder;}

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventHide(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);
        pm_int_t OnEventTimeout(const pm_event_t &Event);
        pm_int_t OnEventNewResources(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);

        void SetScrollSpeed(pm_int_t Amount, pm_int_t Timeout);
        void SetTextJustification(pm_int_t val) {mLeftTextBorder = val;}

        virtual void TextChanged(void);

    protected:

        void StartPan(void);
        void CreateTextBitmap(void);
        virtual Pm_Region GetTextRegion(void);
        void PanText(void);

        pm_int_t mBitmapTable[4];
        pm_bitmap_t *mpTextBitmap;        
        pm_bitmap_t *mpSelTextBitmap;

        pm_int_t mLeaderPixels;
        pm_int_t mCurrentOffset;
        pm_int_t mTimeout;
        pm_int_t mShiftAmount;
        pm_int_t mLeftTextBorder;
};

END_NAMESPACE_PRISM

#endif

