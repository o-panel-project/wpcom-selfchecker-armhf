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
//  prism_graphic_reader.h 
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// NOTES:
// 
// Giving credit to those who are due:
//
// The Pm_Graphic_Reader class utilizes the Floyd-Steinberg dithering
// algorithm to dither high color depth source images to lower color depths.
// The algorithm is well known in graphics circles and and source code can be
// downloaded from many sources, including this:
// 
// http://www.home.unix-ag.org/simon/gimp/fsdither.c
//
// Prism utilizes the versionn published by Xiaolin Wu in Graphics Gems II.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_IMAGE_READER_
#define _PRISM_IMAGE_READER_

BEGIN_NAMESPACE_PRISM

#if defined(PRISM_GRAPHIC_READER_FILEIO) && defined(PRISM_FILE_STDC)
#include "stdio.h"
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PM_GR_IDLE           0x01  
#define PM_GR_PROPERTIES     0x02 
#define PM_GR_HAS_PALETTE    0x04 
#define PM_GR_IMAGE_READY    0x10  
#define PM_GR_DONE           0x20  

#define PM_GR_ERROR          0x80 
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PM_GR_MODE_NO_CONVERT     0x00  
#define PM_GR_MODE_INLINE_DITHER  0x01  
#define PM_GR_MODE_INLINE_REMAP   0x02  
#define PM_GR_MODE_RGB_ORDER      0x04  
#define PM_GR_MODE_ALPHA_TRANS    0x08 
#define PM_GR_MODE_RGB_TRANS      0x10  
#define PM_GR_MODE_INDEX_TRANS    0x20  
#define PM_GR_MODE_555            0x40  
#define PM_GR_MODE_INLINE_RESIZE  0x80

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define PM_GR_FILETYPE_UNKNOWN 0
#define PM_GR_FILETYPE_BMP  1
#define PM_GR_FILETYPE_GIF  2
#define PM_GR_FILETYPE_JPG  3
#define PM_GR_FILETYPE_PNG  4


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// This value is used only for the custom palette producer
#define MAX_COLORMAP_SIZE        256

// This value determines the circular buffer size if reading
// data from memory.

#ifndef PM_GR_INPUT_BUF_SIZE
#define PM_GR_INPUT_BUF_SIZE     1024
#endif

#ifndef PM_GR_SLEEP_TIME
#define PM_GR_SLEEP_TIME         10
#endif

// Floyd-Steinburg dither algorithm constant
#define FS_SCALE 1024

#define HI_COLORS           0x1000        
#define TRUE_COLORS         0x2000      
#define TRUE_ALPHA_COLORS   0x4000       
#define YUVA_COLORS         0x8000      

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define BitSet(byte, bit)    (((byte) & (bit)) == (bit))
#define ReadOK(buffer,len)    (ConsumeImageInput(buffer, len) == len)
#define ReadBytes(buffer, len) ConsumeImageInput(buffer, len)

///////////////////////////////////////////////////////////////////////////////

class Pm_Pixel  {

    public:
        Pm_Pixel(void)
        {
            Red = Green = Blue = Alpha = 0;
        }
        Pm_Pixel(int32_t _Red, int32_t _Green, int32_t _Blue);

        pm_bool_t operator != (const Pm_Pixel &Pix) const;
        pm_bool_t operator == (const Pm_Pixel &Pix) const;

        int32_t Red;
        int32_t Green;
        int32_t Blue;
        uint8_t Alpha;
};

///////////////////////////////////////////////////////////////////////////////
struct ConversionData {
    pm_bitmap_t *pBitmap;
    int32_t DataSize;
};


///////////////////////////////////////////////////////////////////////////////
struct pm_graphic_info_t {
    pm_int_t Width;
    pm_int_t Height;
    pm_int_t BitsPerPix;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class Pm_Graphic_Reader
{
    public:

        Pm_Graphic_Reader(pm_uint_t Id);
        virtual ~Pm_Graphic_Reader();

        pm_bool_t AllocLocalPalette(pm_int_t Entries);
        pm_bool_t AssignImage(pm_bitmap_t *pSrc);

        pm_uint_t ConsumeImageInput(uint8_t *Buffer, pm_uint_t Length);
        void DestroyImages(void);

        pm_uint_t GetBitmapCount(void) {return mImageCount;}
        pm_bitmap_t *GetBitmapPointer(pm_uint_t Index);
        int32_t GetDataSize(pm_uint_t Index = 0);

       #ifdef PRISM_GRAPHIC_READER_FILEIO
        virtual pm_bool_t GetGraphicInfo(Pm_File *Src, pm_graphic_info_t *info) = 0;
        static uint8_t GetGraphicType(Pm_File *Src);
       #endif

       #if defined(PRISM_GR_MEMDATA_SUPPORT)
        static uint8_t GetGraphicType(uint8_t *pData);
       #endif

        pm_uint_t GetMode(void) {return mMode;}
        uint8_t GetOutputBitsPerPix(void)
        {
            return mOutBitsPerPix;
        }
        Pm_Pixel GetPixelColor(uint8_t *pStart, pm_uint_t YPos,
                               pm_uint_t Index);

        pm_uint_t GetRowsConverted(void)
        {
            return mRowsConverted;
        }
        pm_uint_t GetState(void) {return mState;}
        static pm_uint_t GetStride(pm_uint_t Width, pm_uint_t Bits,
            pm_uint_t HasAlpha = 0);

        static pm_uint_t GetStride(pm_bitmap_t *pMap);

        Pm_Pixel GetTransColor()
        {
            return mTransColor;
        }

        Pm_Pixel GetULCColor(void);

        void Id(pm_uint_t Id) {mId = Id;}
        pm_uint_t Id(void) {return mId;}
        uint8_t ImageType(void)
        {
            return mImageType;
        }

       #if !defined(PRISM_GRAPHIC_READER_FILEIO)
        pm_uint_t InputFreeSpace(void);
       #endif

        void KeepAlpha(pm_bool_t Keep);

       #ifdef PRISM_GRAPHIC_READER_FILEIO
        virtual pm_bool_t ReadImage(Pm_File *pFile, pm_int_t Count = 100) = 0;
       #else
        virtual pm_bool_t ReadImage(pm_int_t Count = 100) = 0;
       #endif

        pm_bool_t RemapBitmap(pm_bool_t Dither = FALSE);

       #ifdef PRISM_RUNTIME_RLE_ENCODER
        pm_bool_t RleEncode(pm_bool_t Force = FALSE);
       #endif

       #if !defined(PRISM_GRAPHIC_READER_FILEIO)
        pm_uint_t SendData(uint8_t *Get, pm_uint_t Size);
       #endif

        void SetGrayscale(pm_bool_t Gray)
        {
            mGrayscale = Gray;
        }

        void SetIdleCallback(pm_bool_t (*pFunc)(uint16_t, uint16_t,
                             Pm_Graphic_Reader *));

       #if defined(PRISM_GR_MEMDATA_SUPPORT)
        void SetInputBuffer(uint8_t *pGet, uint32_t lSize);
       #endif

        void SetMode(pm_uint_t Mode)
        {
            mMode = Mode;
        }

        void SetPalStart(pm_uint_t Index) {mPalStart = Index;}

        void SetSystemPalette(uint8_t *pPal, pm_uint_t PalSize, 
                              pm_bool_t Fast = FALSE);

       #if defined(PRISM_GRAPHIC_SCALING)
        void SetTargetSize(pm_int_t Width, pm_int_t Height)
        {
            mDestWidth = Width;
            mDestHeight = Height;
            mMode |= PM_GR_MODE_INLINE_RESIZE;
        }
       #endif

        void SetTransColor(Pm_Pixel pPixel);
        void SetTransColor(uint8_t Index);

    protected:

        pm_bitmap_t *AddNewBitmapToList(int32_t Size);

        pm_uint_t CountDuplicates(uint8_t *Start, pm_uint_t Index,
                               pm_uint_t Width);

        void DitherBitmapRow(uint8_t *pGet, uint8_t *pPut, pm_uint_t Width);
        void EndAllRemapping(void);
        void EndDither(void);
        void EndRemapping(pm_bitmap_t *pMap);
        uint8_t *GetLocalPalette(void){return mpLocalPalette;}

        static int32_t GetRawPix1(uint8_t *pData, pm_uint_t Index);
        static int32_t GetRawPix2(uint8_t *pData, pm_uint_t Index);
        static int32_t GetRawPix4(uint8_t *pData, pm_uint_t Index);
        static int32_t GetRawPix8Alpha(uint8_t *pData, pm_uint_t Index);
        static int32_t GetRawPix8(uint8_t *pData, pm_uint_t Index);
        static int32_t GetRawPix16Alpha(uint8_t *pData, pm_uint_t Index);
        static int32_t GetRawPix16(uint8_t *pData, pm_uint_t Index);
        static int32_t GetRawPix24Alpha(uint8_t *pData, pm_uint_t Index);
        static int32_t GetRawPix24(uint8_t *pData, pm_uint_t Index);

        pm_bool_t InitDither(pm_uint_t cols);

        pm_bool_t InlineMode(void)
        {
            if (mMode & (PM_GR_MODE_INLINE_DITHER|PM_GR_MODE_INLINE_REMAP|
                PM_GR_MODE_INLINE_RESIZE))
            {
                return TRUE;
            }
            return FALSE;
        }

        uint8_t LookupBestColor(Pm_Pixel *pPixel);

        void GraphicError(void);

        void ReadPixel1To4(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);
        void ReadPixel8Alpha(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);
        void ReadPixel8(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);
        void ReadPixel16_555(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);
        void ReadPixel16_555Alpha(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);
        void ReadPixel16_565(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);
        void ReadPixel16_565Alpha(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);
        void ReadPixel24Alpha(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);
        void ReadPixel24(uint8_t *pGet, pm_uint_t Index, Pm_Pixel *pPut);

        void ColorMapRow(uint8_t *pGet, uint8_t *pPut, pm_uint_t Width);

       #if defined(PRISM_GRAPHIC_SCALING)
        void ResizeBitmapRow(pm_int_t SrcWidth);
       #endif

       #ifdef PRISM_RUNTIME_RLE_ENCODER
        pm_uint_t RleEncode(uint8_t *pData, uint8_t *pPut, pm_uint_t Width);
       #endif

        void SendState(pm_uint_t State);
        void SetInputStride(pm_uint_t ImgWidth);
        void SetInternalBpp(pm_uint_t Bpp);
        void SetOutputColorDepth(pm_uint_t Bpp);
        void SetOutputStride(pm_uint_t ImgWidth);
        pm_bool_t SetupInlineBuf(pm_uint_t Width);
        virtual void SetLocalPalette(pm_uint_t Index) {}

       #if defined(PRISM_GRAPHIC_SCALING)
        pm_bool_t SetupInlineResizeBuf(void);
       #endif

        pm_bool_t TransMode(void)
        {
            if (mMode & (PM_GR_MODE_INDEX_TRANS|PM_GR_MODE_RGB_TRANS|PM_GR_MODE_ALPHA_TRANS))
            {
                return TRUE;
            }
            return FALSE;
        }

       void WritePixel1(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel2(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel4(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel8_332(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel8(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel16_555_Alpha(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel16_555(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel16_565_Alpha(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel16_565_RGB(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel16_565_BGR(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel24_RGB_Alpha(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel32_RGB_NoAlpha(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel24_RGB(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel24_BGR_Alpha(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel32_BGR_NoAlpha(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);
       void WritePixel24_BGR(uint8_t *pLine,
                        pm_uint_t PixIndex, Pm_Pixel *pPixel);

       #if defined(PRISM_GRAPHIC_SCALING)
        static void WriteRaw1(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
        static void WriteRaw2(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
        static void WriteRaw4(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
        static void WriteRaw8Alpha(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
        static void WriteRaw8(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
        static void WriteRaw16Alpha(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
        static void WriteRaw16(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
        static void WriteRaw24Alpha(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
        static void WriteRaw24(uint8_t *pData, pm_uint_t Index,
                              int32_t Val);
       #endif

        void WriteTrans1to4(uint8_t *pPut, pm_uint_t PixIndex);
        void WriteTrans8_332(uint8_t *pPut, pm_uint_t PixIndex);
        void WriteTrans8(uint8_t *pPut, pm_uint_t PixIndex);
        void WriteTrans16(uint8_t *pPut, pm_uint_t PixIndex);
        void WriteTrans24RGB(uint8_t *pPut, pm_uint_t PixIndex);
        void WriteTrans24BGR(uint8_t *pPut, pm_uint_t PixIndex);
        void WriteTrans32RGB(uint8_t *pPut, pm_uint_t PixIndex);
        void WriteTrans32BGR(uint8_t *pPut, pm_uint_t PixIndex);

        pm_bool_t  (*mpCallBack) (uint16_t, uint16_t, Pm_Graphic_Reader *);
        int32_t  (*mpReadRaw)    (uint8_t *, pm_uint_t);

       #if defined(PRISM_GRAPHIC_SCALING)
        void     (*mpWriteRaw)   (uint8_t *, pm_uint_t, int32_t);
       #endif

        void (Pm_Graphic_Reader::*mpReadPixel)(uint8_t *, pm_uint_t, Pm_Pixel *);
        void (Pm_Graphic_Reader::*mpWritePixel)(uint8_t *, pm_uint_t, Pm_Pixel *);
        void (Pm_Graphic_Reader::*mpWriteTrans)(uint8_t *, pm_uint_t);

        Pm_Pixel mTransColor;
        Pm_Pixel mPixelWritten;
        ConversionData *mpConversionData;

       #ifdef PRISM_GRAPHIC_READER_FILEIO
        Pm_File *mFile;
       #endif
	   
       #if defined(PRISM_GR_MEMDATA_SUPPORT)
        uint8_t *mpInBufferStart;
        uint8_t *mpInBufferPtr;
        uint32_t mInBufferSize;
       #endif

        int32_t *mpThisRerr;
        int32_t *mpNextRerr;
        int32_t *mpThisGerr;
        int32_t *mpNextGerr;
        int32_t *mpThisBerr;
        int32_t *mpNextBerr;
        uint8_t *mpSystemPalette;

        uint8_t *mpLocalPalette;
        uint8_t *mpInlineReadBuf;     

       #ifndef PRISM_GRAPHIC_READER_FILEIO
        uint8_t *mpInBuf;
        uint8_t *mpPutInData;
        uint8_t *mpGetInData;
        uint8_t *mpInBufStop;
       #endif

       #if defined(PRISM_GRAPHIC_SCALING)
        uint8_t   *mpInlineResizeBuf;    
        pm_int_t   mDestWidth;      
        pm_int_t   mDestHeight;
       #endif

        int32_t   mDataSize;
        pm_uint_t mOutBitsPerPix;
        pm_uint_t mInternalBitsPerPix;
        pm_uint_t mSysPalSize;
        pm_uint_t mLocalPalSize;
        pm_uint_t mInputStride;
        pm_uint_t mOutputStride;
        pm_uint_t mRowsConverted;
        pm_uint_t mImageCount;
        pm_uint_t mPalStart;
        pm_uint_t mState;
        pm_uint_t mMode;
        pm_uint_t mId;
       
        pm_bool_t mDitherDirection;
        pm_bool_t mDithering;
        pm_bool_t mGrayscale;
        pm_bool_t mWantAlpha;        
        pm_bool_t mInputHasAlpha; 
        pm_bool_t mAlphaIncluded;
        uint8_t   mImageType;
};


#endif   













