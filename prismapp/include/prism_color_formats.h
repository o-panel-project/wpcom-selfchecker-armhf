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
//  prism_color_formats.h - Prism color related definitions
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_COLOR_FORMATS_
#define	_PRISM_COLOR_FORMATS_

///////////////////////////////////////////////////////////////////////////////
// color format definitions

#define PM_CF_MONO                  1
#define PM_CF_MONO_INVERTED         2

#define PM_CF_MONO_LIMIT            3

#define PM_CF_2BPP_GRAY             4
#define PM_CF_2BPP_GRAY_INVERTED    5
#define PM_CF_4BPP_GRAY             6
#define PM_CF_4BPP_GRAY_INVERTED    7
#define PM_CF_4BPP_VGA              8
#define PM_CF_8BPP_GRAY             9
#define PM_CF_8BPP_GRAY_INVERTED   10
#define PM_CF_8BPP_PALETTE         11   
#define PM_CF_8BPP_PACKED          12   


#define PM_CF_MICRO_LIMIT          20

#define PM_CF_16BPP_RGB            21
#define PM_CF_16BPP_BGR            22
#define PM_CF_15BPP_RGB            23 
#define PM_CF_15BPP_BGR            24 
#define PM_CF_16BPP_ARGB           25  
#define PM_CF_16BPP_BGRA           26  

#define PM_CF_16BPP_LIMIT          40
        
#define PM_CF_24BPP_RGB            41 
#define PM_CF_24BPP_BGR            42

#define PM_CF_24BPP_LIMIT          60

#define PM_CF_32BPP_ARGB           61 
#define PM_CF_32BPP_RGBA           62
#define PM_CF_32BPP_ABGR           63
#define PM_CF_32BPP_BGRA           64 

///////////////////////////////////////////////////////////////////////////////

#define PRISM_ORIENTATION_NATIVE          0
#define PRISM_ORIENTATION_ROTATE_CW       1
#define PRISM_ORIENTATION_ROTATE_CCW      2

#endif
