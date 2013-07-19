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
//  prism_sprite.h - Prism drawing engine base class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef _PRISM_SPRITE_
#define _PRISM_SPRITE

typedef struct {
    pm_int_t    BitmapId;
    pm_int_t    Delay;
    pm_int_t    xOffset;
    pm_int_t    yOffset;
    pm_int_t    Disposal; 
} pm_sprite_info_t;

#define DISPOSAL_BACK_FILL   2
#define DISPOSAL_PREV_FRAME  4


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define PRISM_SPRITE_CONTINUOUS      0x0001
#define PRISM_SPRITE_AWAYS_VISIBLE   0x0002
#define PRISM_SPRITE_AUTOTRIGGER     0x0004

#define ANIMATION_TIMER              0x1000


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Sprite : public Pm_Widget
{
    public:

        Pm_Sprite(const Pm_Region &Size, pm_sprite_info_t *pFrameList,
            uint16_t Id = 0, uint32_t Style = PM_BORDER_NONE);

        Pm_Sprite(const Pm_Region &Size, uint16_t Id = 0,
            uint32_t Style = PM_BORDER_NONE);

        virtual ~Pm_Sprite(void);

        void AssignFrameList(pm_sprite_info_t *pFrameList);

        pm_int_t GetCurrentFrame(void) {return mFrame;}

        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventTimeout(const pm_event_t &Event);
        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventHide(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);

        void SetCurrentFrame(pm_int_t Frame);
        void SetMode(uint16_t Mode);

        void Start(pm_int_t Frame = -1);
        void Stop(void);

    protected:
        void InitializeFrameList(pm_sprite_info_t *pFrames);

        pm_sprite_info_t *mpCurrentFrame;
        pm_sprite_info_t *mpFrameList;

        uint16_t  mMode;
        pm_int_t  mFrame;
        pm_bool_t mRunning;
};

#endif 

