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
//  prism_data_types.h - Prism library simple data types
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_TYPES_
#define _PRISM_TYPES_

BEGIN_NAMESPACE_PRISM

//  Simple data types
//
//  Prism utilizes the C99 simple data types as the base types. These include
//  uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t
//


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL 0
#endif

#if defined(PRISM_FOR_ITRON)
#undef NULL
#define NULL 0
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if !defined(PRISM_FOR_SMX) && !defined(PRISM_FOR_MQX)
#if !defined(PRISM_FOR_ITRON)

#if !defined(TRUE)
#define TRUE 1
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

#endif
#endif

#if defined(C99_TYPES_HEADER)
#include C99_TYPES_HEADER
#else
#include "prism_std_int.h"
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef signed int pm_int_t;        

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef unsigned int pm_uint_t;     

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef signed int pm_bool_t;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
 #ifndef PRISMWIDECHAR                  
  #define PRISMWIDECHAR uint16_t       
 #endif
typedef PRISMWIDECHAR pm_char_t;    

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if !defined(CHART_DATA_TYPE)
#define CHART_DATA_TYPE pm_int_t
#define CHART_MAX_VAL (0x7fffffff)
#define CHART_MIN_VAL (-CHART_MAX_VAL)
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define PRISM_READ_ONLY const

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if (PRISM_COLOR_FORMAT < PM_CF_MICRO_LIMIT)
typedef uint8_t pm_color_t;
#elif (PRISM_COLOR_FORMAT < PM_CF_16BPP_LIMIT)
typedef uint16_t pm_color_t;
#else
typedef uint32_t pm_color_t;
#endif

END_NAMESPACE_PRISM

#endif




