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
//  prism_gif_reader.h - Prism GIF file reader class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
#ifdef PRISM_RUNTIME_GIF_READER

#ifndef _PRISM_GIF_READER_
#define _PRISM_GIF_READER_

BEGIN_NAMESPACE_PRISM

#define GIF_CODE_STACK_SIZE 1024     

#define MAX_LWZ_BITS      12

#define INTERLACE        0x40
#define LOCALCOLORMAP    0x80

#define GIF16(a, b)          (((b) << 8) | (a))

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef struct {
    uint8_t           IsGif89;
    pm_uint_t         Width;
    pm_uint_t         Height;
    pm_uint_t         Colors;
    pm_color_t        BackColor;
    uint8_t           BackColorIndex;
    uint8_t           AspectRatio;
} GIF_HEADER;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef struct {
    uint8_t         *pPalette;
    pm_int_t        PalSize;
    pm_int_t        xOffset;
    pm_int_t        yOffset;
    pm_uint_t       Width;
    pm_uint_t       Height;
    pm_uint_t       Delay;
    uint8_t         HasTrans;
    uint8_t         TransColorIndex;
    uint8_t         InputFlag;
    uint8_t         Disposal;
} GIF_IMAGE_INFO;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Gif_Reader : public Pm_Graphic_Reader
{
    public:

        Pm_Gif_Reader(pm_uint_t Id = 0);
        virtual ~Pm_Gif_Reader(void);

        void DestroyImages(void);

        GIF_HEADER *GetGifHeader(void) {return &mGifHead;}
        GIF_IMAGE_INFO *GetGifInfo(void) {return mpImageInfo;}

       #ifdef PRISM_GRAPHIC_READER_FILEIO
        virtual pm_bool_t GetGraphicInfo(Pm_File *pSrc, pm_graphic_info_t *info);
       #endif

        pm_bool_t ReadFrame();

       #ifdef PRISM_GRAPHIC_READER_FILEIO
        pm_bool_t ReadHeader(Pm_File *pSrc);
        virtual pm_bool_t ReadImage(Pm_File *pSrc, pm_int_t Count = 10);
       #else
        pm_bool_t ReadHeader();
        virtual pm_bool_t ReadImage(pm_int_t Count = 10);
       #endif

    private:

        pm_int_t CheckStack(void);
        GIF_IMAGE_INFO *CurrentImageInfo(void);
        void DeleteImageInfo(void);

        pm_int_t DoExtension(pm_int_t label);
        pm_int_t GetCode(pm_int_t code_size, pm_int_t flag);
        pm_int_t GetDataBlock(uint8_t *pBuf);
        pm_int_t LWZReadByte(pm_int_t flag, pm_int_t input_code_size);

        pm_int_t ReadColorMap(pm_int_t number, uint8_t *pPalette);
        pm_bool_t ReadImage(pm_int_t len, pm_int_t height, pm_int_t interlace);

       #if defined(PRISM_GRAPHIC_SCALING)
        pm_bool_t ReadImageToSize(pm_int_t len, pm_int_t height, pm_int_t interlace);
       #endif

        virtual void SetLocalPalette(pm_uint_t Index);

        GIF_IMAGE_INFO *mpImageInfo;
        int16_t *mpStack;
        uint8_t *mpDataBuf;

        pm_bool_t mZeroDataBlock;
        pm_bool_t mFresh;

        pm_int_t mCurbit;
        pm_int_t mLastbit;

        pm_int_t mMaxCode;
        pm_int_t mMaxCodeSize;

        pm_int_t mInfoCount;

        pm_int_t mDone;
        pm_int_t mLastByte;
        pm_int_t mCodeSize;
        pm_int_t mSetCodeSize;

        pm_int_t mFirstCode;
        pm_int_t mOldCode;
        pm_int_t mClearCode;
        pm_int_t mEndCode;

        GIF_HEADER mGifHead;

        int16_t mCodeTable[2][( 1 << MAX_LWZ_BITS)];
        int16_t mStack[GIF_CODE_STACK_SIZE];
};

END_NAMESPACE_PRISM

#endif
#endif

