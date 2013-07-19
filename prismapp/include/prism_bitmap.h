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
//  prism_bitmap.h - Prism bitmap structure definitions
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef _PRISM_BITMAP_TYPE_
#define _PRISM_BITMAP_TYPE_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
// pm_bitmap_t structure and flags definition.
///////////////////////////////////////////////////////////////////////////////

#define   BMP_FORMAT_RAW      (0)   
#define   BMP_FORMAT_RLE      (1 << 0)     
#define   BMP_IN_GMEM         (1 << 5) 
#define   BMP_FORMAT_555      (1 << 2)    

#define   BMP_FORMAT_GRAYSCALE (1 << 3)    
#define   BMP_FORMAT_ARGB      (1 << 3)    

#define   BMP_INCLUDES_ALPHA  (1 << 7)  

#define   BMP_IS_CAPTURE      (1 << 1) 
#define   BMP_ORDER_RGB       (1 << 6)    
#define   BMP_FORMAT_PACKED   (1 << 6)    
    
#define   BMP_TRANS_FLAG      (1 << 4)   
  
struct pm_bitmap_t
{
    uint8_t *pData;        
    uint32_t tColor;   
    uint16_t Width;        
    uint16_t Height; 
    uint8_t Flags;          
    uint8_t Bpp;       
    uint8_t uDelFlg;	// 0:Delete OK 1:Delete NG add worldpicom
};

#define IS_CAPTURE(_pBmp) (_pBmp->Flags & BMP_IS_CAPTURE)
#define IS_GRAYSCALE(_pBmp) (_pBmp->Flags & BMP_FORMAT_GRAYSCALE)
#define IS_IN_GMEM(_pBmp) (_pBmp->Flags & BMP_IN_GMEM)
#define IS_COMPRESSED(_pBmp) (_pBmp->Flags & BMP_FORMAT_RLE)
#define IS_PACKED_FORMAT(_pBmp) (_pBmp->Flags & BMP_FORMAT_PACKED)
#define IS_RED_FIRST(_pBmp) (_pBmp->Flags & BMP_ORDER_RGB)
#define INCLUDES_ALPHA(_pBmp) (_pBmp->Flags & BMP_INCLUDES_ALPHA)
#define IS_FORMAT_555(_pBmp) (_pBmp->Flags & BMP_FORMAT_555)
#define IS_TRANSPARENT(_pBmp) (_pBmp->Flags & BMP_TRANS_FLAG)

END_NAMESPACE_PRISM

#endif
