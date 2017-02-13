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
//  prism_status.h - Prism status flag definitions
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef _PRISM_STATUS_
#define	_PRISM_STATUS_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

const uint32_t PM_SF_VISIBLE           = 0x00000001;
const uint32_t PM_SF_HIDDEN            = 0x00000002;
const uint32_t PM_SF_ALWAYS_ON_TOP     = 0x00000004;
const uint32_t PM_SF_PORTAL            = 0x00000008;

const uint32_t PM_SF_CURRENT           = 0x00000010;
const uint32_t PM_SF_SELECTABLE        = 0x00000020;
const uint32_t PM_SF_TAB_STOP          = 0x00000040;
const uint32_t PM_SF_ACCEPTS_FOCUS     = 0x00000080;
const uint32_t PM_SF_KEEPS_CHILD_FOCUS = 0x00000100;
const uint32_t PM_SF_DEFAULT_FOCUS     = 0x00000200;

const uint32_t PM_SF_SIZEABLE          = 0x00000400;
const uint32_t PM_SF_MOVEABLE          = 0x00000800;
const uint32_t PM_SF_NONCLIENT         = 0x00001000;

const uint32_t PM_SF_CONTINUOUS_SCROLL = 0x00002000;
const uint32_t PM_SF_OWNS_POINTER      = 0x00004000;
const uint32_t PM_SF_BUTTON_DERIVED    = 0x00008000;
const uint32_t PM_SF_PANEL_DERIVED     = 0x00010000;

END_NAMESPACE_PRISM

#endif // sentry
