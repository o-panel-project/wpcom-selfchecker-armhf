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
//  prism_exclusive_btn.h - Prism exclusive button class declaration
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_EXCL_BUTTON_
#define _PRISM_EXCL_BUTTON_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

enum PM_BI_EXCLUSIVE_BMPS {
    PM_BI_EXCLUSIVE_ON = 0,    
    PM_BI_EXCLUSIVE_OFF,       
    PM_BI_EXCLUSIVE_ON_FOCUS,      
    PM_BI_EXCLUSIVE_OFF_FOCUS,     
    PM_BI_EXCLUSIVE_ON_DIS,      
    PM_BI_EXCLUSIVE_OFF_DIS,
    NUM_EXCL_BTN_BMPS
};



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Exclusive_Btn : public Pm_Btn
{
    public:
        Pm_Exclusive_Btn(const Pm_Region &Size, pm_uint_t StringId = 0,
            uint16_t Id = 0,
            uint32_t Style = PM_ACTIVE|
                             PM_BORDER_NONE|
                             PM_BTN_EXCLUSIVE|
                             PM_BTN_HOVER);
  
        virtual ~Pm_Exclusive_Btn() {}

        virtual void AssignBitmap(pm_uint_t Index, pm_uint_t BitmapId);

        virtual pm_uint_t GetBitmap(pm_uint_t Index)
        {
            if (Index < NUM_EXCL_BTN_BMPS)
            {
                return mBitmapTable[Index];
            }

            return 0;
        }

        virtual void Paint(const Pm_Region &Invalid);
        virtual void SetSelected(pm_bool_t Selected = TRUE);

    protected:

        void ExclusiveButtonInit(void);
        virtual void UnselectSiblings(void) const;
        pm_uint_t mBitmapTable[NUM_EXCL_BTN_BMPS];
};

END_NAMESPACE_PRISM

#endif
