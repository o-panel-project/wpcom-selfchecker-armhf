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
//  prism_bmp_reader.h - Prism graphics file reader API definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_BMP_READER_
#define _PRISM_BMP_READER_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
//
// The following headers and data structures are copied from
// "The Encyclopedia of Graphics File Formats", OReilly
//
///////////////////////////////////////////////////////////////////////////////

typedef struct 
{
    uint16_t   id;
    uint32_t   file_size;
    uint16_t   reserved[2];
    uint32_t   image_offset;
    uint16_t   header_size;
    uint16_t   not_used;
    uint32_t   xres;
    uint32_t   yres;
    uint16_t   numplanes;
    uint16_t   bits_per_pix;
    uint32_t   compression;
    uint32_t   bit_map_size;
    uint32_t   hor_res;
    uint32_t   vert_res;
    uint32_t   number_of_colors;
    uint32_t   num_sig_colors;
} pm_bitmap_header_t;

typedef struct
{    
    uint8_t    Blue;  
    uint8_t    Green;
    uint8_t    Red;
    uint8_t    Count;
} BmpPalEntry;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Bmp_Reader : public Pm_Graphic_Reader
{
    public:

        Pm_Bmp_Reader(pm_uint_t Id = 0);

        virtual ~Pm_Bmp_Reader(void);

       #ifdef PRISM_GRAPHIC_READER_FILEIO
        virtual pm_bool_t GetGraphicInfo(Pm_File *Src,
            pm_graphic_info_t *info);
       #endif

        pm_bitmap_header_t *Getpm_bitmap_header_t(void)
        {
            return &mpm_bitmap_header_t;
        }

        virtual Pm_Pixel GetULCColor(void);

       #ifdef PRISM_GRAPHIC_READER_FILEIO
        virtual pm_bool_t ReadImage(Pm_File *fd, pm_int_t Count = 1);
       #else
        virtual pm_bool_t ReadImage(pm_int_t Count = 1);
       #endif

    protected:

    private:

        uint8_t GetBitmapPaddingBytes();

        pm_bool_t ReadBitmapData(void);
        pm_bool_t ReadBitmapHeader();
        void ReadBitmapPalette(void);
        void ReadBitmapScanLine(uint8_t *pPut, pm_uint_t Actual);

       #ifndef PRISM_GRAPHIC_READER_FILEIO
        void SkipToBitmapOffset(uint32_t Offset);
       #endif
        
        pm_bitmap_header_t mpm_bitmap_header_t;
        uint32_t mBytesRead;
};

END_NAMESPACE_PRISM

#endif



