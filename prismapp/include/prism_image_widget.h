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
//  prism_image_widget.h - Prism image decode/display widget
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_IMAGE_WIDGET_
#define _PRISM_IMAGE_WIDGET_

BEGIN_NAMESPACE_PRISM

#ifdef PRISM_RUNTIME_GIF_READER
typedef struct {

    pm_bitmap_t *pMap;
    GIF_IMAGE_INFO Info;
} GIF_FRAME;
#endif


///////////////////////////////////////////////////////////////////////////////
class Pm_Image_Widget : public Pm_Widget
{
    public:
        Pm_Image_Widget();
        virtual ~Pm_Image_Widget();

        void Cleanup(void);
        pm_bool_t DecodeImage(pm_char_t *pPathname);
        pm_bool_t DecodeImage(char *pPathname);

        virtual pm_int_t Notify(const pm_event_t &Event);
        virtual void Paint(const Pm_Region &Invalid);

        void SetMode(uint16_t Mode);
        void SetDither(pm_bool_t Dither);
        pm_bool_t SetRotation(pm_int_t Rotation,
            pm_bool_t AutoSize = TRUE);

    private:

        void CheckAutoSize(pm_bitmap_t *pMap);
        void RefreshImage(void);
        pm_bitmap_t *mpNativeImage;
        pm_bitmap_t *mpDisplayImage;

        pm_int_t    mRotation;
        pm_int_t    mNumFrames;
        pm_int_t    mFrameIndex;
        pm_bool_t   mDither;
        pm_bool_t   mAutoSize;

       #ifdef PRISM_RUNTIME_GIF_READER
        GIF_HEADER *mpGifHeader;
        GIF_FRAME  *mpGifFrames;  
       #endif      

        uint16_t  mMode;
        uint8_t   mImageType;
};


#endif
