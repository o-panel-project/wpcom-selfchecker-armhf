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
//  prism_keypad_device.h - Prism keypad driver declaration
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef __PRISMKEYDEV__
#define __PRISMKEYDEV__

#if defined(PRISM_KEYPAD_SUPPORT) && defined(PRISM_KEYPAD_DEVICE)

///////////////////////////////////////////////////////////////////////////////
// Keyboard defines
///////////////////////////////////////////////////////////////////////////////
#ifndef PRISM_KEYBOARD_POLL_DELAY
#define PRISM_KEYBOARD_POLL_DELAY                     (5)
#endif

#ifndef PRISM_KEYPAD_REP_COUNT
#define PRISM_KEYPAD_REP_COUNT  75
#endif

///////////////////////////////////////////////////////////////////////////////
// If PC keyboard support is required, the driver operates a little different
///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_USB_KEY_SUPPORT) || defined(PRISM_PCAT_KEY_SUPPORT)
#define PRISM_PC_KEY_SUPPORT
#endif

///////////////////////////////////////////////////////////////////////////////
// Single Key Structure
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint16_t NormalKey;
    uint16_t ShiftKey;
} PRISM_KEY_TABLE;

typedef struct
{
    PRISM_KEY_TABLE   *pKeys;
    pm_bool_t         *pCurrentStates;
    pm_bool_t         *pPrevStates;
    pm_int_t          *pRepCount;
} PRISM_KEY_DATA;

///////////////////////////////////////////////////////////////////////////////
// If USB keyboard, extern the key table
///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_USB_KEY_SUPPORT)
extern PRISM_KEY_TABLE PrismUSBKeyTable[];
#define PRISM_USB_KEYTABLE_LEN                        255
#endif


///////////////////////////////////////////////////////////////////////////////
class Pm_Keypad_Device : public Pm_Input_Device
{
    public:
        Pm_Keypad_Device(PRISM_KEY_TABLE *pKeyTable, pm_int_t NumKeys);
        virtual ~Pm_Keypad_Device();

        virtual pm_bool_t Poll(void);
        virtual void Run(void);


       #if defined(PRISM_USB_KEY_SUPPORT)
        virtual pm_bool_t SendUSBKeyNotify(pm_int_t Index, uint16_t UserData);
       #endif

    protected:
       #if !defined(PRISM_USB_KEY_SUPPORT)
        virtual void    AllKeysUnpressed(void);
        virtual void    CheckSendNotify(void);
        virtual pm_bool_t DetectKeyPressed(void)= 0;
        virtual pm_bool_t GetKeyInfo(void) = 0;
       #endif
 

        PRISM_KEY_DATA    mKeyData;
        pm_int_t          mNumKeys;
        pm_bool_t         mCapLock;

};

///////////////////////////////////////////////////////////////////////////////
// this function is declared here and defined in the driver source module
///////////////////////////////////////////////////////////////////////////////
Pm_Keypad_Device *CreatePrismKeypadDevice(void);

#endif  // PRISM_KEYPAD_SUPPORT
#endif  // _PKEYDEV_HPP_

