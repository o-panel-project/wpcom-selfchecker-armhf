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
//  prism_mouse_device.h - Prism mouse driver declaration
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_MOUSEDEV_
#define _PRISM_MOUSEDEV_

#ifndef PRISM_MOUSE_POLL_DELAY
#define PRISM_MOUSE_POLL_DELAY    5    // how long does mouse task suspend
#endif                               // between check for movement?

// Define limits on mouse x and y positions

#define PRISM_MOUSE_MIN_XPOS     0
#define PRISM_MOUSE_MIN_YPOS     0
#define PRISM_MOUSE_MAX_XPOS     (PRISM_VIRTUAL_XSIZE - 2)
#define PRISM_MOUSE_MAX_YPOS     (PRISM_VIRTUAL_YSIZE - 2)

// Definitions for mouse button states
#ifndef PRISM_MOUSE_LBUTTON
#define PRISM_MOUSE_LBUTTON       1
#endif

#ifndef PRISM_MOUSE_RBUTTON
#define PRISM_MOUSE_RBUTTON       2
#endif
///////////////////////////////////////////////////////////////////////////////
// Single Key Structure
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    pm_uint_t PrevButtonState;
    pm_uint_t NewButtonState;
    pm_point_t PrevPos;
    pm_point_t NewPos;
} PRISM_MOUSE_STATE;

#if defined(PRISM_MOUSE_SUPPORT) && defined(PRISM_MOUSE_INCLUDE)
///////////////////////////////////////////////////////////////////////////////
class PrismMouseDevice : public Pm_Input_Device
{
    public:
        PrismMouseDevice(void);
        virtual ~PrismMouseDevice();

        virtual pm_bool_t Poll(void);
        virtual void    Run(void);

    protected:
        virtual void    SendMaskedEvent(void);
        virtual pm_bool_t DetectMouse(void) { return FALSE; }

        PRISM_MOUSE_STATE mState;
};

///////////////////////////////////////////////////////////////////////////////
PrismMouseDevice *CreatePrismMouseDevice(void);

#endif  
#endif 


