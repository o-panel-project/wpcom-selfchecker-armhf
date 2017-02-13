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
//  prism_png_reader.h - Prism graphics file reader API definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This software originated as libpng, a free software library. This software
// has been heavily modified and ported to integrate within the Prism
// framework. The authors of libpng request that the following copyright
// notice be included in any software that utilizes libpng:

/*
For the purposes of this copyright and license, "Contributing Authors"
is defined as the following set of individuals:

   Andreas Dilger
   Dave Martindale
   Guy Eric Schalnat
   Paul Schmidt
   Tim Wegner

The PNG Reference Library is supplied "AS IS".  The Contributing Authors
and Group 42, Inc. disclaim all warranties, expressed or implied,
including, without limitation, the warranties of merchantability and of
fitness for any purpose.  The Contributing Authors and Group 42, Inc.
assume no liability for direct, indirect, incidental, special, exemplary,
or consequential damages, which may result from the use of the PNG
Reference Library, even if advised of the possibility of such damage.

Permission is hereby granted to use, copy, modify, and distribute this
source code, or portions hereof, for any purpose, without fee, subject
to the following restrictions:

1. The origin of this source code must not be misrepresented.

2. Altered versions must be plainly marked as such and must not
   be misrepresented as being the original source.

3. This Copyright notice may not be removed or altered from any
   source or altered source distribution.

The Contributing Authors and Group 42, Inc. specifically permit, without
fee, and encourage the use of this source code as a component to
supporting the PNG file format in commercial products.  If you use this
source code in a product, acknowledgment is not required but would be
appreciated.
*/
//
// The complete source code from which this module is derived is
// available here:
//
// http://www.libpng.org/pub/png/libpng.html
//
// The original source code has been modified to support Prism FileIO
// abstraction and easy portability to different operating systems.
// Pm_Png_Reader is a wrapper class that makes it easier to use libpng.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_PNG_
#define _PRISM_PNG_

#if defined(PRISM_RUNTIME_PNG_READER)

#define PRISM_PNG_ZBUF_SIZE 8192

#define PRISM_PNG_COLOR_MASK_PALETTE 1
#define PRISM_PNG_COLOR_MASK_COLOR 2
#define PRISM_PNG_COLOR_MASK_ALPHA 4

#define PRISM_PNG_MAX_UINT ((uint32_t) 0x7FFFFFFFL)

#define PRISM_PNG_COLOR_TYPE_GRAY 0
#define PRISM_PNG_COLOR_TYPE_PALETTE \
        (PRISM_PNG_COLOR_MASK_COLOR | PRISM_PNG_COLOR_MASK_PALETTE)
#define PRISM_PNG_COLOR_TYPE_RGB (PRISM_PNG_COLOR_MASK_COLOR)
#define PRISM_PNG_COLOR_TYPE_RGB_ALPHA \
        (PRISM_PNG_COLOR_MASK_COLOR | PRISM_PNG_COLOR_MASK_ALPHA)
#define PRISM_PNG_COLOR_TYPE_GRAY_ALPHA (PRISM_PNG_COLOR_MASK_ALPHA)

#define PRISM_PNG_COMPRESSION_TYPE_BASE 0

#define PRISM_PNG_FILTER_TYPE_BASE 0 

#define PRISM_PNG_INTERLACE_NONE 0
#define PRISM_PNG_INTERLACE_ADAM7 1 
#define PRISM_PNG_INTERLACE_LAST 2 

#define PRISM_PNG_INFO_PLTE 0x0008
#define PRISM_PNG_INFO_tRNS 0x0010

typedef struct
{
    pm_uint_t mWidth; 
    pm_uint_t mRowBytes; 
    uint8_t mColorType; 
    uint8_t mBitDepth; 
    uint8_t mChannels; 
    uint8_t mPixelDepth; 
} PrismPngRowInfo;


typedef struct
{
    uint32_t mMode;     
    uint32_t mFlags;       
    pm_uint_t mValid;

    PzipStream mZstream;    
    uint8_t* mpZbuf;         
    uint32_t mZbufSize;     
    pm_int_t mZlibLevel;         
    pm_int_t mZlibMethod;         
    pm_int_t mZlibWindowBits;     
    pm_int_t mZlibMemLevel;      
    pm_int_t mZlibStrategy;        

    pm_uint_t mWidth;  
    pm_uint_t mHeight;  
    uint32_t mNumRows; 
    uint32_t mUsrWidth;
    pm_uint_t mRowBytes;    
    uint32_t mIrowBytes; 
    uint32_t mIwidth;       
    pm_uint_t mRowNumber;  
    uint8_t* mpPrevRow;      
    uint8_t* mpRowBuf; 
    uint8_t* mpSubRow; 
    uint8_t* mpUpRow;  
    uint8_t* mpAvgRow;   
    uint8_t* mpPaethRow;       
    PrismPngRowInfo mRowInfo;  

    uint32_t mIdatSize; 
    uint32_t mCrc;       
    pm_uint_t mNumTrans;  
    uint8_t mChunkName[5]; 
    uint8_t mInterlaced;  
    uint8_t mPass;     
    uint8_t mDoFilter;  
    uint8_t mColorType;   
    uint8_t mBitDepth;  
    uint8_t mUsrBitDepth;
    uint8_t mPixelDepth;  
    uint8_t mChannels;    
    uint8_t mUsrChannels; 

    uint32_t mFlushDist; 
    uint32_t mFlushRows; 

    uint8_t* mpTrans; 
    Pm_Pixel mTransVal;

} PrismPngStruct;

#define PRISM_PNG_NO_FILTERS 0x00
#define PRISM_PNG_FILTER_NONE 0x08
#define PRISM_PNG_FILTER_SUB 0x10
#define PRISM_PNG_FILTER_UP 0x20
#define PRISM_PNG_FILTER_AVG 0x40
#define PRISM_PNG_FILTER_PAETH 0x80
#define PRISM_PNG_ALL_FILTERS \
        (PRISM_PNG_FILTER_NONE | PRISM_PNG_FILTER_SUB | PRISM_PNG_FILTER_UP | \
         PRISM_PNG_FILTER_AVG | PRISM_PNG_FILTER_PAETH)


#define PRISM_PNG_FILTER_VALUE_NONE 0
#define PRISM_PNG_FILTER_VALUE_SUB 1
#define PRISM_PNG_FILTER_VALUE_UP 2
#define PRISM_PNG_FILTER_VALUE_AVG 3
#define PRISM_PNG_FILTER_VALUE_PAETH 4
#define PRISM_PNG_FILTER_VALUE_LAST 5


#define PRISM_PNG_HAVE_IHDR 0x01
#define PRISM_PNG_HAVE_PLTE 0x02
#define PRISM_PNG_HAVE_IDAT 0x04
#define PRISM_PNG_AFTER_IDAT 0x08
#define PRISM_PNG_HAVE_IEND 0x10

#define PRISM_PNG_FLAG_ZLIB_CUSTOM_STRATEGY 0x0001
#define PRISM_PNG_FLAG_ZLIB_CUSTOM_LEVEL 0x0002
#define PRISM_PNG_FLAG_ZLIB_CUSTOM_MEM_LEVEL 0x0004
#define PRISM_PNG_FLAG_ZLIB_CUSTOM_WINDOW_BITS 0x0008
#define PRISM_PNG_FLAG_ZLIB_CUSTOM_METHOD 0x0010
#define PRISM_PNG_FLAG_ZLIB_FINISHED 0x0020
#define PRISM_PNG_FLAG_ROW_INIT 0x0040
#define PRISM_PNG_FLAG_CRC_ANCILLARY_USE 0x0100
#define PRISM_PNG_FLAG_CRC_ANCILLARY_NOWARN 0x0200
#define PRISM_PNG_FLAG_CRC_CRITICAL_USE 0x0400
#define PRISM_PNG_FLAG_CRC_CRITICAL_IGNORE 0x0800
#define PRISM_PNG_FLAG_FREE_PLTE 0x1000
#define PRISM_PNG_FLAG_FREE_TRNS 0x2000

#define PRISM_PNG_FLAG_CRC_ANCILLARY_MASK \
        (PRISM_PNG_FLAG_CRC_ANCILLARY_USE | PRISM_PNG_FLAG_CRC_ANCILLARY_NOWARN)


#define PRISM_PNG_IHDR const uint8_t png_IHDR[5] = { 73,  72,  68,  82, '\0' }
#define PRISM_PNG_IDAT const uint8_t png_IDAT[5] = { 73,  68,  65,  84, '\0' }
#define PRISM_PNG_IEND const uint8_t png_IEND[5] = { 73,  69,  78,  68, '\0' }
#define PRISM_PNG_PLTE const uint8_t png_PLTE[5] = { 80,  76,  84,  69, '\0' }
#define PRISM_PNG_tRNS const uint8_t png_tRNS[5] = { 116,  82,  78,  83, '\0' }


#define PRISM_PNG_SIGNATURE_BYTES 8

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Png_Reader : public Pm_Graphic_Reader
{
public:

    Pm_Png_Reader( pm_uint_t Id = 0 );
    virtual ~Pm_Png_Reader();

    uint8_t BitsPerChannel() const { return mpPng->mBitDepth; };
    uint32_t BytesPerRow() const { return mpPng->mRowBytes; };
    uint8_t ColorType() const { return mpPng->mColorType; };

   #ifdef PRISM_GRAPHIC_READER_FILEIO
    virtual pm_bool_t GetGraphicInfo(Pm_File *fd, pm_graphic_info_t *info);
   #endif

    uint32_t Height() const { return mpPng->mHeight; };
    uint8_t InterlaceType() const { return mpPng->mInterlaced; };
    uint8_t NumChannels() const { return mpPng->mChannels; };

   #if defined(PRISM_GRAPHIC_READER_FILEIO)
    virtual pm_bool_t ReadImage( Pm_File *fd, pm_int_t Count = 1 );
   #else
    virtual pm_bool_t ReadImage( pm_int_t Count = 1 );
   #endif

    uint32_t Width() const { return mpPng->mWidth; };

protected:

private:

    pm_bool_t CheckChunkName( uint8_t* chunk_name );

    void CombineRow( uint8_t* row, pm_int_t mask );
    void ConvertRawToPrism(uint8_t *pPut, uint8_t *pGet);
    pm_bool_t CrcError();
    pm_int_t CrcFinish( uint32_t skip );
    pm_bool_t CrcRead( uint8_t* buf, pm_uint_t length );
    pm_bool_t CreateReadStruct();
    void DestroyReadStruct();

    pm_bool_t HandleIEND( uint32_t length );
    pm_bool_t HandleIHDR( uint32_t length );
    pm_bool_t HandlePLTE( uint32_t length );
    pm_bool_t HandleTRNS( uint32_t length );
    pm_bool_t HandleUnknown( uint32_t length );

    pm_bool_t InitializeStructures();
    pm_bool_t IsValid();

    pm_uint_t  PrismPngGetUint16( uint8_t* pBuf );
    uint32_t PrismPngGetUint32( uint8_t* pBuf );

    void ReadFilterRow( PrismPngRowInfo* row_info, uint8_t* row, 
                        uint8_t* prev_row, pm_int_t filter );

    pm_bool_t ReadFinishRow();

   #if defined(PRISM_GRAPHIC_SCALING)
    pm_bool_t ReadPngDataToSize();
   #endif

    pm_bool_t ReadPngData();
    pm_bool_t ReadPngHeader();

    pm_bool_t ReadRow( uint8_t* row );

    pm_bool_t ReadStartRow();

    void WriteRowPixel(uint8_t *pPut, pm_int_t col, uint32_t Val);
    PrismPngStruct* mpPng;
};

#endif 
#endif
