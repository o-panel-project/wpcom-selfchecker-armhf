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
//  prism_widget_types.h - Prism widget type definitions
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef _PRISM_WIDGET_TYPES_
#define _PRISM_WIDGET_TYPES_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

const uint16_t TYPE_WIDGET             = 1;
const uint16_t TYPE_TITLE              = 2;

const uint16_t TYPE_BUTTON             = 5;
const uint16_t TYPE_HSCROLL            = 6;
const uint16_t TYPE_VSCROLL            = 7;
const uint16_t TYPE_ICON               = 8;
const uint16_t TYPE_MENU_BAR           = 9;
const uint16_t TYPE_MENU_BTN           = 10;
const uint16_t TYPE_TEXT_BTN           = 11;
const uint16_t TYPE_ICON_BTN           = 12;
const uint16_t TYPE_BMP_BTN            = 13;
const uint16_t TYPE_EXCLUSIVE_BTN      = 14;
const uint16_t TYPE_TOGGLE_BTN         = 15;
const uint16_t TYPE_STATUS_BAR         = 16;
const uint16_t TYPE_PROMPT             = 17;
const uint16_t TYPE_VPROMPT            = 18;
const uint16_t TYPE_PROGRESS           = 19;
const uint16_t TYPE_ANIM_PROMPT        = 20;
const uint16_t TYPE_BMP_SLIDER         = 21;
const uint16_t TYPE_TEXT_INPUT         = 22;
const uint16_t TYPE_SLIDER             = 23;
const uint16_t TYPE_SPIN_BTN           = 24;
const uint16_t TYPE_GROUP              = 25;
const uint16_t TYPE_ML_TEXT_BTN        = 26;
const uint16_t TYPE_TOOL_BAR_PANEL     = 27;
const uint16_t TYPE_TOOL_BAR           = 28;
const uint16_t TYPE_TXT_ICON_BTN       = 29;
const uint16_t TYPE_SPRITE             = 30;
const uint16_t TYPE_PASSWORD_INPUT     = 31;
const uint16_t TYPE_BMP_PROMPT         = 32;
const uint16_t TYPE_BMP_TEXT_BTN       = 33;
const uint16_t TYPE_SHADOW_BTN         = 34;


const uint16_t LAST_PRISM_CONTROL_TYPE   = 0x00ff;

const uint16_t FIRST_APP_CONTROL_TYPE = 0x0100;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

const uint16_t TYPE_PANEL              = 0x0200;
const uint16_t TYPE_MENU               = 0x0202;
const uint16_t TYPE_DATA_TABLE         = 0x0203;
const uint16_t TYPE_ML_PROMPT          = 0x0204;
const uint16_t TYPE_POPUP_PANEL        = 0x0205;
const uint16_t TYPE_DECORATED_PANEL    = 0x0206;
const uint16_t TYPE_TAB_PANEL          = 0x0207;
const uint16_t TYPE_TREEVIEW           = 0x0208;
const uint16_t TYPE_LIST               = 0x0209;
const uint16_t TYPE_VLIST              = 0x020a;
const uint16_t TYPE_HLIST              = 0x020b;
const uint16_t TYPE_DROP_BOX           = 0x020c;
const uint16_t TYPE_ML_TEXT_INPUT      = 0x020d;
const uint16_t TYPE_ML_POPUP           = 0x020e;
const uint16_t TYPE_VVLIST             = 0x020f;
const uint16_t TYPE_DROP_LIST          = 0x0210;
const uint16_t TYPE_TEXT_DROPBOX       = 0x0211;



#if defined(PRISM_CHART_WIDGETS)
const uint16_t TYPE_LINE_CHART         = 0x0220;
#endif


const uint16_t FIRST_APP_PANEL_TYPE  = 0x0300;

END_NAMESPACE_PRISM

#endif
