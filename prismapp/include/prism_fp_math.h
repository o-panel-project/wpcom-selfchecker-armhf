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
//  prism_fp_math - Prism fixed-point math function definitions
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_FIXED_POINT_
#define _PRISM_FIXED_POINT_

BEGIN_NAMESPACE_PRISM

// This is the most important value, how much are we going to
// shift our fixed point values? A larger value gives us more
// precision, but less range.

#define PM_FPM_SHIFT        10
#define PM_FPM_ONE        1024
#define PM_FPM_HALF        512

#define PM_FPM_REMAINDER_MASK 0x3ff

typedef long int pm_fixed_t;

#define PM_FP_TO_INT(_a)       ((pm_int_t)(((pm_fixed_t)_a) >> PM_FPM_SHIFT))
#define PM_FP_UPRND(_a)   ((((pm_fixed_t)_a) + PM_FPM_ONE - 1) >> PM_FPM_SHIFT)
#define PM_FP_RND(_a)     ((((pm_fixed_t)_a) + PM_FPM_HALF) >> PM_FPM_SHIFT)

#define PM_MAKEFP(_a)      (((pm_fixed_t)_a) << PM_FPM_SHIFT)
#define PM_FP_MUL(_a,_b)    ((((pm_fixed_t)_a) * (_b)) >> PM_FPM_SHIFT)
#define PM_FP_DIV(_a,_b)    ((((pm_fixed_t)_a) << PM_FPM_SHIFT) / ((pm_fixed_t)_b))
#define PM_FP_ADD1(_a)    (((pm_fixed_t)_a) + PM_FPM_ONE)


typedef struct {
    pm_fixed_t x;
    pm_fixed_t y;
} pm_fixed_point;

#endif

