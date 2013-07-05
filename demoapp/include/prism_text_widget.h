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
//  prism_text_widget.h - Prism text widget base class
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_TEXT_BASE_
#define _PRISM_TEXT_BASE_

BEGIN_NAMESPACE_PRISM

const uint32_t PM_JUSTIFY_RIGHT     = 0x00000020;
const uint32_t PM_JUSTIFY_LEFT      = 0x00000040;
const uint32_t PM_JUSTIFY_CENTER    = 0x00000080;
const uint32_t PM_JUSTIFY_MASK      = 0x000000E0;
const uint32_t PM_PRIVATE_COPY      = 0x10000000;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Text_Widget : public Pm_Widget
{
    public:

        Pm_Text_Widget(const Pm_Region &Size, pm_uint_t StringId = 0,
            uint16_t Id = 0, uint32_t Style = PM_BORDER_NONE,
            pm_uint_t FontId = 0);

        Pm_Text_Widget(const pm_char_t *pText, const Pm_Region &Size, 
            uint16_t Id = 0, uint32_t Style = PM_PRIVATE_COPY|PM_BORDER_NONE,
            pm_uint_t FontId = 0);

        Pm_Text_Widget(pm_uint_t StringId, uint16_t Id,
            uint32_t Style = PM_BORDER_NONE, pm_uint_t FontId = 0);

        Pm_Text_Widget(const pm_char_t *pText, uint16_t Id,
            uint32_t Style = PM_PRIVATE_COPY|PM_BORDER_NONE,
            pm_uint_t FontId = 0);

        Pm_Text_Widget(uint32_t Style = PM_BORDER_NONE);

        virtual ~Pm_Text_Widget();

        virtual void AssignFont(pm_uint_t FontIndex);

        void AssignText(pm_uint_t StringId)
        {
            mString.AssignText(StringId);
            TextChanged();

            if (StatusIs(PM_SF_VISIBLE))
            {
                Invalidate();
            }
        }

        void AssignText(const pm_char_t *pText)
        {
            mString.AssignText(pText);
            TextChanged();

            if (StatusIs(PM_SF_VISIBLE))
            {
                Invalidate();
            }
        }

        void AssignText(const char *pText)
        {
            mString.AssignText(pText);
            TextChanged();

            if (StatusIs(PM_SF_VISIBLE))
            {
                Invalidate();
            }
        }

        const pm_char_t *GetText(void) {return mString.mpText;}

        void ClearText(void)
        {
            mString.AssignText((const pm_char_t *) NULL);

            if (StatusIs(PM_SF_VISIBLE))
            {
                Invalidate();
            }
        }
      
        const pm_font_t *GetFont(void) {return mpFont;}
        pm_uint_t GetFontId(void) {return mFontId;}
       
        pm_string_t &GetString(void) {return mString;}
        pm_uint_t GetStringId(void) {return mString.mStringId;}
      
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventNewLanguage(const pm_event_t &Event);
      
        virtual pm_int_t RangeCheck(pm_int_t Min, pm_int_t Max, pm_int_t Def);

        virtual void TextChanged(void) {}

        pm_int_t TextLength(void)
        {
            return mString.mStrLen;
        }

    protected:

        const pm_font_t *mpFont;
        pm_uint_t mFontId;
        pm_string_t mString;

    private:
   
};

END_NAMESPACE_PRISM

#endif


