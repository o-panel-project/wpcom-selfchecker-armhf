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
//  prism_title_bar.h - Pm_Title_Bar
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_TITLE_
#define _PRISM_TITLE_


const uint32_t PM_TF_NONE            = 0x00000000;
const uint32_t PM_TF_SYSBUTTON       = 0x00000200;
const uint32_t PM_TF_MINMAXBUTTON    = 0x00000400;
const uint32_t PM_TF_CLOSEBUTTON     = 0x00000800;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
enum
{
    PM_CI_TITLE_TEXT,        
    PM_CI_TITLE_TEXT_SEL,    
    PM_CI_TITLE_GRAD1 = 0,
    PM_CI_TITLE_GRAD1_SEL,
    PM_CI_TITLE_GRAD2,
    PM_CI_TITLE_GRAD2_SEL,
    PM_CI_TITLE_GRAD3,
    PM_CI_TITLE_GRAD3_SEL,
    PM_CI_TITLE_GRAD4,
    PM_CI_TITLE_GRAD4_SEL,
    PM_CI_TITLE_GRAD5,
    PM_CI_TITLE_GRAD5_SEL,
    MAX_EXT_TITLE_COLORS
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Title : public Pm_Text_Widget
{
    public:
        Pm_Title(pm_int_t StringId,
         uint32_t Style = PM_JUSTIFY_CENTER);

        Pm_Title(const pm_char_t *pText, uint32_t Style = 
                 PM_PRIVATE_COPY|PM_JUSTIFY_CENTER);

        virtual ~Pm_Title();

        virtual void AssignFont(pm_uint_t FontIndex);
        void AssignMenu(Pm_Menu_Description *pMenu);

        virtual pm_uint_t GetColor(const uint8_t Index);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventParentSized(const pm_event_t &Event);
        pm_int_t OnEventResize(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventSystemClicked(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);
        pm_int_t OnEventMaxClicked(const pm_event_t &Event);
        pm_int_t OnEventRestore(const pm_event_t &Event);
        pm_int_t OnEventMinClicked(const pm_event_t &Event);
        pm_int_t OnEventCloseClicked(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);
        virtual void SetColor(const uint8_t Index, const pm_uint_t ColorId);

        enum TitleButtons 
        {
            PM_BTN_ID_MAX = 1008,
            PM_BTN_ID_MIN,
            PM_BTN_ID_RESTORE,
            PM_BTN_ID_SYSTEM
        };

    protected:
            
        void Construct(uint32_t Style);
        virtual void SizeToParent(void);

        #if defined(PRISM_GRADIENT_SUPPORT)
        pm_uint_t *mpColorTable;
        pm_gradient_t mGradient;
        #endif

        Pm_Menu *mpMenu;
        Pm_Icon_Btn *mpSysButton;
        Pm_Icon_Btn *mpCloseButton;
        Pm_Icon_Btn *mpMinButton;
        Pm_Icon_Btn *mpMaxButton;

        pm_int_t mHeight;
};


#endif

