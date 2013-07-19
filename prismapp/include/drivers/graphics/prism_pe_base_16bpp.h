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
//  prism_pe_base_16bpp.h - Prism drawing engine common functions for 16-bpp 
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
 
#ifndef _ENGINE_COMMON_16BPP_
#define _ENGINE_COMMON_16BPP_

///////////////////////////////////////////////////////////////////////////////
#define PlotPointToPortal(x, y, c)  *(PM_PIX_PTR(y) + x) =  c;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Paint_Engine16Bit : public Pm_Paint_Engine
{
    public:
        Pm_Paint_Engine16Bit(const Pm_Region &);
        virtual ~Pm_Paint_Engine16Bit();

        virtual void Snapshot(Pm_Snapshot *Info, Pm_Region &Region);

        virtual pm_bitmap_t *CreateBitmap(pm_int_t Width, pm_int_t Height,
            pm_int_t Style = BMP_FORMAT_RAW|BMP_TRANS_FLAG);

        virtual uint8_t *GetPalette(uint32_t *pPutSize);

       #ifdef PRISM_GRAPHIC_SCALING

        virtual pm_color_t GetBitmapPixel(pm_int_t x, pm_int_t y, pm_bitmap_t *pMap)
        {
            pm_color_t *pGet = (pm_color_t *)pMap->pData;
            pGet += pMap->Width * y;
            pGet += x;
            return *pGet;
        }

       #endif

        virtual pm_color_t ReadPortalPixel(pm_int_t x, pm_int_t y)
        {
            if (mpContext)
            {
                return((pm_color_t) *(PM_PIX_PTR(y) + x));
            }
            else
            {
                return 0;
            }
        }

       #ifdef PRISM_GRAPHIC_SCALING

        virtual void PutBitmapPixel(pm_int_t x, pm_int_t y, pm_bitmap_t *pMap,
                                    pm_color_t Val)
        {
            pm_color_t *pPut = (pm_color_t *)pMap->pData;
            pPut += pMap->Width * y;
            pPut += x;
            *pPut = Val;
        }

       #endif

        virtual void ResetPalette(void);
        virtual void SetPalette(pm_int_t First, pm_int_t Num,
                                const uint8_t *pGet);

    protected:

       #ifdef PRISM_ALIASED_LINES
        virtual void PaintAntiAliasedLineToPortal(pm_int_t xStart, pm_int_t yStart, pm_int_t xEnd,
                                pm_int_t yEnd, const Pm_Region & Portal,
                                Pm_Brush &Brush);
       #endif

        virtual void BitmapToPortal(const pm_point_t Where,
                                const pm_bitmap_t *pGetmap,
                                const Pm_Region &Portal);

        void Blend(pm_color_t *pGet, pm_color_t *pPut, pm_uint_t Alpha,
                      pm_uint_t Len);

        pm_color_t BlendColor(pm_color_t gt, pm_color_t pt, pm_uint_t Alpha,
                      pm_uint_t NotAlpha);

        void BlendAlphaPixels(pm_color_t *pPut, pm_color_t *pGet, pm_uint_t Len);

       #if defined(PRISM_MULT_SW_CANVASES)

        void BlendCanvas(pm_canvas_t *pCanvas, Pm_Region &Copy,
            uint8_t *pComposite);

        pm_bool_t BuildCompositeCanvas(Pm_Region &Copy, uint8_t *pComposite);

        void CopyCanvas(pm_canvas_t *pCanvas, Pm_Region &Copy,
            uint8_t *pComposite);

       #endif

        virtual uint8_t *GetFrameBufferAddress(void) {return NULL;}

        void Paint8BitBitmap(const pm_point_t Where, const Pm_Region &Portal,
            const pm_bitmap_t *pGetmap);

       #if defined(PRISM_FREETYPE_SUPPORT)

        virtual void PaintFreetypeChar(FT_Bitmap *pMap,
            pm_int_t xPos, pm_int_t yPos, pm_color_t Color,
            Pm_Region &Portal);

       #endif

       #ifdef PRISM_ALIASED_TEXT
        virtual void PaintSimpleAliasedText(pm_point_t Where,
                        const pm_char_t *pText, Pm_Brush &Brush,
                        pm_int_t Count, Pm_Region &Region);

        virtual void PaintTrueAliasedText(pm_point_t Where,
                        const pm_char_t *pText, Pm_Brush &Brush,
                        pm_int_t Count, Pm_Region &Region);

        virtual void PaintAliasedTextToAlphaMap(pm_point_t Where,
                        const pm_char_t *pText, Pm_Brush &Brush,
                        pm_int_t Count, Pm_Region &Region);

       #endif

        virtual void PaintAAMapToPortal(pm_point_t Where, const pm_bitmap_t *pMap,
            Pm_Brush &Brush, const Pm_Region &Portal);

        virtual void PaintRleBitmap(const pm_point_t Where, const Pm_Region &Portal,
                                   const pm_bitmap_t *pGetmap);

        void Paint8BitRleBitmap(const pm_point_t Where, const Pm_Region &Portal,
            const pm_bitmap_t *pGetMap);

        virtual void PaintTextToPortal(pm_point_t Where, const pm_char_t *pText,
                                  Pm_Brush &Brush, pm_int_t Count, Pm_Region &Region);

        virtual void HorizontalLine(pm_int_t XStart, pm_int_t XEnd, pm_int_t YPos,
                                    pm_color_t Color, pm_int_t Width);

       #if defined(PRISM_FULL_GRAPHICS)
        virtual void HorizontalLineXOR(pm_int_t XStart, pm_int_t XEnd,
                                       pm_int_t YPos);
       #endif

        virtual void LineToPortal(pm_int_t xStart, pm_int_t yStart, pm_int_t xEnd,
                              pm_int_t yEnd, Pm_Region &Region, Pm_Brush &Brush);

        void MakeAAColors(pm_color_t *pPutColors, Pm_Brush &Brush);

        virtual void PlotSubPixel(pm_fixed_t xVal, pm_fixed_t yVal,
                                  pm_color_t Color);

        virtual void ShiftRegionInPortal(const Pm_Region &Portal,
                                  const pm_int_t xMove, const pm_int_t yMove);

       #ifdef PRISM_ALIASED_LINES
        virtual void TrueAliasLineToPortal(pm_int_t xStart, pm_int_t yStart, pm_int_t xEnd,
                                pm_int_t yEnd, const Pm_Region &Portal,
                                Pm_Brush &Brush);
       #endif

        virtual void VerticalLine(pm_int_t YStart, pm_int_t YEnd, pm_int_t XPos,
                                  pm_color_t Color, pm_int_t Width);

       #if defined(PRISM_FULL_GRAPHICS)
        virtual void VerticalLineXOR(pm_int_t YStart, pm_int_t YEnd, pm_int_t XPos);
       #endif
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define REDVAL555(_c)   (uint8_t)(((_c) >> 10) & 0x1f)
#define REDVAL565(_c)   (uint8_t)(((_c) >> 11) & 0x1f)
#define GREENVAL555(_c) (uint8_t)(((_c) >> 5) & 0x1f)
#define GREENVAL565(_c) (uint8_t)(((_c) >> 5) & 0x3f)
#define BLUEVAL555(_c)  (uint8_t)(((_c) ) & 0x1f)
#define BLUEVAL565(_c)  (uint8_t)(((_c) ) & 0x1f)


#define MAKE565(r, g, b) ((((r) & 0x1f) << 11) | \
                          (((g) & 0x3f) << 5) | \
                          (((b) & 0x1f) ))

#define MAKE555(r, g, b) ((((r) & 0x1f) << 10) | \
                          (((g) & 0x1f) << 5) | \
                          (((b) & 0x1f) ))

#define BLEND(_pt, _gr, _gg, _gb, _pr, _pg, _pb, _a, _na)           \
    {                                                               \
        (_pr) = (((_gr) * (_a)) + ((_pr) * (_na))) >> 8;            \
        (_pg) = (((_gg) * (_a)) + ((_pg) * (_na))) >> 8;            \
        (_pb) = (((_gb) * (_a)) + ((_pb) * (_na))) >> 8;            \
        *(_pt) = MAKEHICOLOR((_pr), (_pg), (_pb));                  \
    }

#define GETRGB565(_c, _r, _g, _b)       \
    {                                   \
        (_r) = REDVAL565(_c);           \
        (_g) = GREENVAL565(_c);         \
        (_b) = BLUEVAL565(_c);          \
    }

#define GETRGB555(_c, _r, _g, _b)       \
    {                                   \
        (_r) = REDVAL555(_c);           \
        (_g) = GREENVAL555(_c);         \
        (_b) = BLUEVAL555(_c);          \
    }


#if defined(PRISM_565_FORMAT)

 #define REDVAL(_c)      REDVAL565(_c)
 #define GREENVAL(_c)    GREENVAL565(_c)
 #define BLUEVAL(_c)     BLUEVAL565(_c)

 #define MAKEHICOLOR(r, g, b) MAKE565(r, g, b)
 #define MAKEHICOLOR_32(r, g, b) MAKE565_32(r, g, b)
 #define GETRGB(_c, _r, _g, _b) GETRGB565((_c), (_r), (_g), (_b))

#else

 #define REDVAL(_c)      REDVAL555(_c)
 #define GREENVAL(_c)    GREENVAL555(_c)
 #define BLUEVAL(_c)     BLUEVAL555(_c)

 #define MAKEHICOLOR(r, g, b) MAKE555(r, g, b)
 #define MAKEHICOLOR_32(r, g, b) MAKE555_32(r, g, b)
 #define GETRGB(_c, _r, _g, _b) GETRGB555((_c), (_r), (_g), (_b))

#endif


#endif



