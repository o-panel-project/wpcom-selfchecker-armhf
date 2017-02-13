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
//  prism_brush.h - PM_Brush class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_BRUSH_
#define _PRISM_BRUSH_

#define PBS_SOLID_FILL    0x0001
#define PBS_BMP_FILL      0x0002
#define PBS_NO_ALIAS      0x0004
#define PBS_SIMPLE_ALIAS  0x0008
#define PBS_TRUE_ALIAS    0x0010
#define PBS_UNDERLINE     0x0020
#define PBS_ROUNDED       0x0040
#define PBS_CENTER_LINE   0x0080
#define PBS_PATTERN       0x0100

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Brush
{
    friend class Pm_Paint_Engine;

    public:
        Pm_Brush();

        Pm_Brush(pm_color_t _LineColor,
                pm_color_t _FillColor,
            pm_int_t BStyle = PBS_NO_ALIAS,
            pm_int_t _LineWidth = 1,
            const pm_font_t *_pFont = NULL);

        void Set(pm_color_t _LineColor,
                 pm_color_t _FillColor,
                 pm_int_t BStyle = PBS_NO_ALIAS,
                 pm_int_t _LineWidth = 1)
            {
                LineColor = _LineColor;
                FillColor = _FillColor;
                Style = BStyle;
                Width = _LineWidth;
            }

        pm_bitmap_t     *pBitmap;
        const pm_font_t *pFont;

        uint32_t        Pattern;

        pm_int_t        Width;
        pm_int_t        Style;

        pm_color_t      LineColor;
        pm_color_t      FillColor;
};

#endif

