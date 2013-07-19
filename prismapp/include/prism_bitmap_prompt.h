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
//  prism_bitmap_prompt.h - Prism bitmap prompt widget
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_BITMAP_PROMPT_
#define _PRISM_BITMAP_PROMPT_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
class Pm_Bmp_Prompt : public Pm_Prompt
{
    public:

        Pm_Bmp_Prompt(const Pm_Region &Size,
                      pm_int_t StringId = 0, uint16_t Id = 0,
                      uint32_t Style = PM_BORDER_NONE|
                                    PM_JUSTIFY_LEFT|
                                    PM_PAINT_TRANS|
                                    PM_ANIM_ALWAYS);

        void AssignAllBitmaps(pm_uint_t NormalMap, pm_uint_t SelectedMap,
                        pm_uint_t LeftMap, pm_uint_t RightMap);

        void AssignBitmap(pm_int_t Index, pm_uint_t BitmapId);

        pm_int_t GetBitmap(pm_int_t Index) { return mBitmapTable[Index]; }
        pm_int_t GetTextJustification(void) {return mLeftTextBorder;}
        virtual Pm_Region GetTextRegion(void);

        virtual void Paint(const Pm_Region &Invalid);

        void SetTextJustification(pm_int_t val) {mLeftTextBorder = val;}
        void SetWrapSpace(pm_int_t Space) {mLeaderPixels = Space;}

    protected:

        pm_int_t mBitmapTable[4];
        pm_int_t mLeftTextBorder;
        pm_int_t mLeaderPixels;
};

END_NAMESPACE_PRISM

#endif

