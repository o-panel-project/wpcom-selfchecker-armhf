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
//  prism_password.h - Prism panel class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISMPASSWD_
#define _PRISMPASSWD_

BEGIN_NAMESPACE_PRISM

#define PM_MAX_PW_CHARS   64

///////////////////////////////////////////////////////////////////////////////
class Pm_Password : public Pm_Text_Input
{
    public:
        Pm_Password(const Pm_Region &Size,
                         const pm_char_t *pText,
                         uint16_t Id = 0,
                         uint32_t Style = PM_BORDER_RECESSED|
                                          PM_ACTIVE|
                                          PM_ALLOW_EDIT,
                         pm_int_t Length = PM_MAX_PW_CHARS - 1,
                         pm_char_t EchoChar = '*');

        pm_char_t GetEchoChar(void)
        {
            return mEchoChar;
        }
        virtual pm_char_t *GetText(void);
        void InitializeText(const pm_char_t *pText);

        virtual pm_int_t Notify(const pm_event_t &Event);

       #if defined(PRISM_KEYPAD_SUPPORT)
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        pm_int_t OnEventGainedFocus(const pm_event_t &Event);

        void SetEchoChar(pm_char_t Char)
        {
            mEchoChar = Char;
        }

    private:

        void PasswordDeleteMarkedText();
        void PasswordReplaceMarkedText(pm_char_t Char);

        pm_char_t mPasswordText[PM_MAX_PW_CHARS];
        pm_char_t mEchoChar;

};

END_NAMESPACE_PRISM

#endif






