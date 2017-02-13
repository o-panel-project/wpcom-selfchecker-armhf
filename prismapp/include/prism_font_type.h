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
//  prism_font_type - Prism font structure definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_FONTS_
#define _PRISM_FONTS_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////

struct pm_font_t
{
    uint8_t   Type;        
    uint8_t   Ascent;    
    uint8_t   Descent;     
    uint8_t   TotalHeight;  
    uint8_t   LeadingSpace;     
    uint8_t   PostSpace; 
    uint8_t   LineHeight;  
    uint16_t  BytesPerLine;  
    uint16_t  FirstChar;     
    uint16_t  LastChar;
   
   #if defined(PRISM_FREETYPE_SUPPORT)
    union {     
        uint16_t  *pOffsets;    
        FT_Face   Face;
    };
   #else
    uint16_t *pOffsets;
   #endif

    pm_font_t *pNextPage;
    uint8_t   *pData;    
};


///////////////////////////////////////////////////////////////////////////////

#define PFT_ALIASED  0x04   
#define PFT_ZIPPED   0x08
#define PFT_FREETYPE 0x20   

#define IS_ALIASED(a)  (a->Type & PFT_ALIASED)
#define IS_ZIPPED(a)   (a->Type & PFT_ZIPPED)
#define IS_FREETYPE(a) (a->Type & PFT_FREETYPE)

END_NAMESPACE_PRISM

#endif 

