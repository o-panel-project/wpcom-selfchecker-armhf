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
//  prism_pe_base_24bpp.h - Prism drawing engine common functions for 16-bpp 
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
 
#ifndef _ENGINE_COMMON_24BPP_
#define _ENGINE_COMMON_24BPP_

#define REDVAL(c)    ((uint8_t) (((uint32_t)c) >> 16))
#define GREENVAL(c)  ((uint8_t) ((((uint32_t)c) >> 8) & 0xff))
#define BLUEVAL(c)   ((uint8_t) (((uint32_t)c) & 0xff))

#define MakeTrueColor(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define BLEND(c1, c2, a) (uint8_t)((((c1) * (a)) + ((c2 * (256 - (a))))) >> 8)

#if (PRISM_COLOR_FORMAT == PM_CF_24BPP_RGB)
#define BlendRGB(_r, _g, _b, _a) \
        { \
        *pPut = BLEND((_r), *pPut, (_a)); \
        pPut++; \
        *pPut = BLEND((_g), *pPut, (_a)); \
        pPut++; \
        *pPut = BLEND((_b), *pPut, (_a)); \
        pPut++; \
        }
#else
#define BlendRGB(_r, _g, _b, _a) \
        { \
        *pPut = BLEND((_b), *pPut, (_a)); \
        pPut++; \
        *pPut = BLEND((_g), *pPut, (_a)); \
        pPut++; \
        *pPut = BLEND((_r), *pPut, (_a)); \
        pPut++; \
        }
#endif

#if (PRISM_COLOR_FORMAT == PM_CF_24BPP_RGB)
#define WriteRGBColor(_r, _g, _b) \
        { \
        *pPut++ = (_r); \
        *pPut++ = (_g); \
        *pPut++ = (_b); \
        }

#else
#define WriteRGBColor(_r, _g, _b) \
        { \
        *pPut++ = (_b); \
        *pPut++ = (_g); \
        *pPut++ = (_r); \
        }
#endif

#define CopyColor() \
        { \
        *pPut++ = *pGet++; \
        *pPut++ = *pGet++; \
        *pPut++ = *pGet++; \
        }

#ifndef PRISM_RUNTIME_PAINT_ENGINE

#if (PRISM_COLOR_FORMAT == PM_CF_24BPP_RGB)
#define PlotPointToPortal(x, y, c) \
        { \
        uint8_t *_pPut = (uint8_t *) PM_PIX_PTR(y) + ((x) * 3); \
        *_pPut++ = REDVAL(c); \
        *_pPut++ = GREENVAL(c); \
        *_pPut = BLUEVAL(c); \
        }

#else
#define PlotPointToPortal(x, y, c) \
        { \
        uint8_t *_pPut = (uint8_t *) PM_PIX_PTR(y) + ((x) * 3); \
        *_pPut++ = BLUEVAL(c); \
        *_pPut++ = GREENVAL(c); \
        *_pPut = REDVAL(c); \
        }
#endif
#endif

class Pm_Paint_Engine_24Bit : public Pm_Paint_Engine
{
    public:
        Pm_Paint_Engine_24Bit(const Pm_Region &Size);
        virtual ~Pm_Paint_Engine_24Bit();

        virtual void Snapshot(Pm_Snapshot *Info, Pm_Region &Size);

        virtual pm_bitmap_t *CreateBitmap(pm_int_t Width, pm_int_t Height,
            pm_int_t Style = BMP_FORMAT_RAW|BMP_TRANS_FLAG);

        virtual uint8_t *GetPalette(uint32_t *pPutSize);

       #ifdef PRISM_GRAPHIC_SCALING
        virtual pm_color_t GetBitmapPixel(pm_int_t x, pm_int_t y, pm_bitmap_t *pMap)
        {
            pm_color_t cVal;
            uint8_t *pGet = pMap->pData;
            pGet += pMap->Width * y * 3;
            pGet += x * 3;
            cVal = *pGet++;
            cVal <<= 8;
            cVal |= *pGet++;
            cVal <<= 8;
            cVal |= *pGet;
            return cVal;
        }
       #endif

        virtual pm_color_t ReadPortalPixel(pm_int_t x, pm_int_t y)
        {
            if (mpContext)
            {
                pm_color_t cVal;
                uint8_t *pGet = (uint8_t *) PM_PIX_PTR(y) + (x * 3);

                cVal = *pGet++;
                cVal <<= 8;
                cVal |= *pGet++;
                cVal <<= 8;
                cVal |= *pGet;
                return cVal;
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
          if (pMap->Bpp == 24)
          {
             uint8_t *pPut = pMap->pData;
             pPut += pMap->Width * y * 3;
             pPut += x * 3;

             *pPut++ = (uint8_t) (Val >> 16);
             *pPut++ = (uint8_t) (Val >> 8);
             *pPut = (uint8_t) Val;
          }
        }
       #endif

        virtual void ResetPalette(void);
        virtual void SetPalette(pm_int_t First, pm_int_t Num, 
                                const uint8_t *pGet);

    protected:

       #ifdef PRISM_ALIASED_LINES
        virtual void PaintAntiAliasedLineToPortal(pm_int_t xStart, pm_int_t yStart, pm_int_t xEnd, 
                                pm_int_t yEnd, const Pm_Region &Portal, Pm_Brush &Brush);
       #endif

        virtual void BitmapToPortal(const pm_point_t Where, 
                                const pm_bitmap_t *pGetmap, 
                                const Pm_Region &Portal);

       #if defined(PRISM_MULT_SW_CANVASES)

        void BlendCanvas(pm_canvas_t *pCanvas, Pm_Region &Copy,
            uint8_t *pComposite);

        pm_bool_t BuildCompositeCanvas(Pm_Region &Copy, uint8_t *pComposite);

        void CopyCanvas(pm_canvas_t *pCanvas, Pm_Region &Copy,
            uint8_t *pComposite);

       #endif

       #if defined(PRISM_FREETYPE_SUPPORT)

        virtual void PaintFreetypeChar(FT_Bitmap *pMap,
            pm_int_t xPos, pm_int_t yPos, pm_color_t Color,
            Pm_Region &Portal);

       #endif

       #ifdef PRISM_ALIASED_TEXT
        virtual void PaintSimpleAliasedText(pm_point_t Where, const pm_char_t *pText, 
                                     Pm_Brush &Brush, 
                                     pm_int_t Count, Pm_Region &Rect);

        virtual void PaintTrueAliasedText(pm_point_t Where, const pm_char_t *pText, 
                               Pm_Brush &Brush,  
                               pm_int_t Count, Pm_Region &Rect);

        virtual void PaintAliasedTextToAlphaMap(pm_point_t Where,
                const pm_char_t *pText, Pm_Brush &Brush,
                pm_int_t Count, Pm_Region &Clip);
       #endif

        virtual void PaintAAMapToPortal(pm_point_t Where, const pm_bitmap_t *pMap,
            Pm_Brush &Brush, const Pm_Region &Portal);

        virtual void PaintRleBitmap(const pm_point_t Where, const Pm_Region &Portal,
                                   const pm_bitmap_t *pGetmap);

        virtual void PaintTextToPortal(pm_point_t Where, const pm_char_t *pText, 
                                  Pm_Brush &Brush, 
                                  pm_int_t Count, Pm_Region &Rect);

        virtual void PaintTrueColorBitmap(const pm_point_t Where, 
                                         const Pm_Region &Portal,
                                         const pm_bitmap_t *pGetmap);

        virtual void PaintTrueColorAlphaBitmap(const pm_point_t Where,
                                              const Pm_Region &Portal,
                                              const pm_bitmap_t *pGetMap);

        virtual uint8_t *GetFrameBufferAddress(void);

        virtual void HorizontalLine(pm_int_t XStart, pm_int_t XEnd, pm_int_t YPos,
                                    pm_color_t Color, pm_int_t Width);

        virtual void HorizontalLineXOR(pm_int_t XStart, pm_int_t XEnd, 
                                       pm_int_t YPos);

        virtual void LineToPortal(pm_int_t xStart, pm_int_t yStart, pm_int_t xEnd, 
                              pm_int_t yEnd, Pm_Region &Rect, Pm_Brush &Brush);

        void MakeAAColors(uint8_t *pPutColors, Pm_Brush &Brush);

        virtual void ShiftRegionInPortal(const Pm_Region &Portal,
                                  const pm_int_t xMove, const pm_int_t yMove);

        pm_color_t TrueAlias(pm_color_t Back, pm_color_t Fore, pm_int_t Blend);
        void PlotTrueAlias(uint8_t *Where, pm_color_t Fore, pm_int_t Blend);

       #ifdef PRISM_ALIASED_LINES
        virtual void TrueAliasLineToPortal(pm_int_t xStart, pm_int_t yStart, pm_int_t xEnd, 
                                pm_int_t yEnd, const Pm_Region &Portal, Pm_Brush &Brush);
       #endif

        virtual void VerticalLine(pm_int_t YStart, pm_int_t YEnd, pm_int_t XPos,
                                  pm_color_t Color, pm_int_t Width);
        virtual void VerticalLineXOR(pm_int_t YStart, pm_int_t YEnd, pm_int_t XPos);


        uint8_t mPalette[256 * 3]; 
};

#endif



