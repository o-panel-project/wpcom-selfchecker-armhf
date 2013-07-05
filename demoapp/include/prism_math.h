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
//  prism_math.h - Prism math constants and utility functions
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_MATH_
#define _PRISM_MATH_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PRISM_MIN(a, b)  (a > b ? b:a)
#define PRISM_MAX(a, b)  (a > b ? a:b)

#define PRISM_ABS(a) (((a) < 0)?(-(a)):(a))
#define PRISM_ABSDIFF(a, b)  ((a) > (b) ? (a)-(b) : (b)-(a))

pm_int_t PmSquareRoot(int32_t x);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// a nifty swap-in-place algorithm
#define PRISMSWAP(_a, _b) {_a ^= _b; _b ^= _a; _a ^= _b;}
uint32_t PrismEndianSwapLong(uint32_t Val);

void GetSinCos(pm_int_t Angle, pm_int_t *pSin, pm_int_t *pCos);

void LookupInverseSin(pm_int_t Val, pm_int_t *pInvSin);
void LookupInverseCos(pm_int_t Val, pm_int_t *pInvCos);

END_NAMESPACE_PRISM

#endif 

