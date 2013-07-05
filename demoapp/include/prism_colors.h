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
//  prism_colors.h - Prism color related definitions
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_COLOR_DEFS_
#define	_PRISM_COLOR_DEFS_

BEGIN_NAMESPACE_PRISM


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if (PRISM_COLOR_FORMAT >= PM_CF_4BPP_VGA)

const pm_color_t PM_CLR_TRANS       =  PRISM_DEF_TRANSPARENCY;
const pm_color_t PM_CLR_BLACK         =  PRISM_DEF_BLACK;
const pm_color_t PM_CLR_WHITE         =  PRISM_DEF_WHITE;
const pm_color_t PM_CLR_LIGHTGRAY     =  PRISM_DEF_LIGHTGRAY;
const pm_color_t PM_CLR_DARKGRAY      =  PRISM_DEF_DARKGRAY;
const pm_color_t PM_CLR_RED           =  PRISM_DEF_RED;
const pm_color_t PM_CLR_GREEN         =  PRISM_DEF_GREEN;
const pm_color_t PM_CLR_BROWN         =  PRISM_DEF_BROWN;
const pm_color_t PM_CLR_BLUE          =  PRISM_DEF_BLUE;
const pm_color_t PM_CLR_MAGENTA       =  PRISM_DEF_MAGENTA;
const pm_color_t PM_CLR_CYAN          =  PRISM_DEF_CYAN;
const pm_color_t PM_CLR_LIGHTRED      =  PRISM_DEF_LIGHTRED;
const pm_color_t PM_CLR_LIGHTGREEN    =  PRISM_DEF_LIGHTGREEN;
const pm_color_t PM_CLR_YELLOW        =  PRISM_DEF_YELLOW;
const pm_color_t PM_CLR_LIGHTBLUE     =  PRISM_DEF_LIGHTBLUE;
const pm_color_t PM_CLR_LIGHTMAGENTA  =  PRISM_DEF_LIGHTMAGENTA;
const pm_color_t PM_CLR_LIGHTCYAN     =  PRISM_DEF_LIGHTCYAN;
#else

#if (PRISM_COLOR_FORMAT >= PM_CF_4BPP_GRAY)

const pm_color_t PM_CLR_TRANS      =  PRISM_DEF_TRANSPARENCY;
const pm_color_t PM_CLR_BLACK      =  PRISM_DEF_BLACK;
const pm_color_t PM_CLR_DARKGRAY   =  PRISM_DEF_DARKGRAY;
const pm_color_t PM_CLR_LIGHTGRAY  =  PRISM_DEF_LIGHTGRAY;
const pm_color_t PM_CLR_WHITE      =  PRISM_DEF_WHITE;

const pm_color_t PM_CLR_GRAY0      = 0;
const pm_color_t PM_CLR_GRAY1      = 1;
const pm_color_t PM_CLR_GRAY2      = 2;
const pm_color_t PM_CLR_GRAY3      = 3;
const pm_color_t PM_CLR_GRAY4      = 4;
const pm_color_t PM_CLR_GRAY5      = 5;
const pm_color_t PM_CLR_GRAY6      = 6;
const pm_color_t PM_CLR_GRAY7      = 7;
const pm_color_t PM_CLR_GRAY8      = 8;
const pm_color_t PM_CLR_GRAY9      = 9;
const pm_color_t PM_CLR_GRAY10     = 10;
const pm_color_t PM_CLR_GRAY11     = 11;
const pm_color_t PM_CLR_GRAY12     = 12;
const pm_color_t PM_CLR_GRAY13     = 13;
const pm_color_t PM_CLR_GRAY14     = 14;
const pm_color_t PM_CLR_GRAY15     = 15;

#else

#if (PRISM_COLOR_FORMAT >= PM_CF_2BPP_GRAY)
const pm_color_t PM_CLR_TRANS       =  PRISM_DEF_TRANSPARENCY;
const pm_color_t PM_CLR_BLACK         =  PRISM_DEF_BLACK;
const pm_color_t PM_CLR_WHITE         =  PRISM_DEF_WHITE;
const pm_color_t PM_CLR_LIGHTGRAY     =  PRISM_DEF_LIGHTGRAY;
const pm_color_t PM_CLR_DARKGRAY      =  PRISM_DEF_DARKGRAY;
#else

const pm_color_t PM_CLR_TRANS       =  PRISM_DEF_TRANSPARENCY;
const pm_color_t PM_CLR_BLACK         =  PRISM_DEF_BLACK;
const pm_color_t PM_CLR_WHITE         =  PRISM_DEF_WHITE;

#endif
#endif
#endif

///////////////////////////////////////////////////////////////////////////////

enum WidgetStdColors {
    PM_CI_NORMAL = 0,
    PM_CI_FOCUSFILL,
    PM_CI_TEXT,
    PM_CI_FOCUS_TEXT,
    PM_NUM_WIDGET_COLORS
};

#if defined(PRISM_MICRO)
#define PM_NUM_BTN_COLORS PM_NUM_WIDGET_COLORS
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef PRISM_GRADIENT_BUTTONS

enum
{
    PM_BCI_BORDER1_NORMAL = 0,
    PM_BCI_BORDER1_PUSHED,
    PM_BCI_TEXT_NORMAL,      
    PM_BCI_TEXT_PUSHED,  
    PM_BCI_TEXT_HASFOCUS,
    PM_BCI_TEXT_DISABLED,
    PM_BCI_BORDER1_HASFOCUS,
    PM_BCI_BORDER1_DISABLED,
    PM_BCI_BORDER2_NORMAL,
    PM_BCI_BORDER2_PUSHED,
    PM_BCI_BORDER2_HASFOCUS,
    PM_BCI_BORDER2_DISABLED,
    PM_BCI_GRADIENT1_NORMAL,
    PM_BCI_GRADIENT1_PUSHED,
    PM_BCI_GRADIENT1_HASFOCUS,
    PM_BCI_GRADIENT1_DISABLED,
    PM_BCI_GRADIENT2_NORMAL,
    PM_BCI_GRADIENT2_PUSHED,
    PM_BCI_GRADIENT2_HASFOCUS,
    PM_BCI_GRADIENT2_DISABLED,
    PM_NUM_BTN_COLORS
};
#endif

// weight values * 256 for converting r:g:b to gray value:

#define GRAYSCALE_RED_WEIGHT    77    
#define GRAYSCALE_GREEN_WEIGHT  150   
#define GRAYSCALE_BLUE_WEIGHT   37    


END_NAMESPACE_PRISM

#endif	

