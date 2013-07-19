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
//  prism_txt_icon_btn.h - Prism master header file, includes all API header files
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_TI_BUTTON_
#define _PRISM_TI_BUTTON_

const uint32_t PM_BMP_TOP_RIGHT     = 0x00000800;
const uint32_t PM_BMP_BOTTOM_RIGHT  = 0x00001000;

const uint32_t PM_BMP_POS_MASK = (PM_BMP_TOP_RIGHT|PM_BMP_BOTTOM_RIGHT);

const uint32_t PM_BMP_TOP_POS       = PM_BMP_TOP_RIGHT;     // just one set? top
const uint32_t PM_BMP_RIGHT_POS     = PM_BMP_POS_MASK;      // both set? right
const uint32_t PM_BMP_BOTTOM_POS    = PM_BMP_BOTTOM_RIGHT;  // bottom
const uint32_t PM_BMP_LEFT_POS      = 0;


///////////////////////////////////////////////////////////////////////////////
class Pm_Text_Icon_Btn : public Pm_Btn
{
    public:

         Pm_Text_Icon_Btn(const Pm_Region &Size, pm_int_t StringId,
                           pm_int_t BitmapId, uint16_t Id = 0,
                           uint32_t Style = PM_ACTIVE,
                           pm_bool_t FlyOver = FALSE);

        virtual ~Pm_Text_Icon_Btn() {}

        virtual void Paint(const Pm_Region &Invalid);
        virtual void AssignBitmap(pm_int_t BitmapId)
        {
            mBitmapId = BitmapId;
        }
        virtual pm_int_t GetBitmap(void) { return mBitmapId; }


    protected:
        pm_int_t mBitmapId;
};

#endif


