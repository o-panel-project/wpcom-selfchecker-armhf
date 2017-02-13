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
//  engine_common_32bpp.h - Prism drawing engine common functions for 32-bpp 
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
 
#ifndef _ENGINE_COMMON_32BPP_
#define _ENGINE_COMMON_32BPP_

///////////////////////////////////////////////////////////////////////////////

#define REDVAL(c)    (uint8_t) ((c) >> 16)
#define GREENVAL(c)(uint8_t) (((c) >> 8) & 0xff)
#define BLUEVAL(c)   (uint8_t) ((c) & 0xff)
#define ALPHA_VAL(c) (uint8_t) ((c) >> 24)
#define BLEND(c1, c2, a) (uint8_t)((((c1) * (a)) + ((c2 * (256 - (a))))) >> 8)
#define MAKECOLOR(a, r, g, b) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

///////////////////////////////////////////////////////////////////////////////
#ifndef PRISM_PLOTPOINT_FUNCTION
#define PlotPointToPortal(x, y, c) \
        { \
        pm_color_t *_pPut = PM_PIX_PTR(y) + (x); \
        *_pPut++ = (c); \
        }
#endif


///////////////////////////////////////////////////////////////////////////////
class Pm_Paint_Engine32Bit : public Pm_Paint_Engine
{
    public:
        Pm_Paint_Engine32Bit(const Pm_Region &);
        virtual ~Pm_Paint_Engine32Bit();

        virtual void Snapshot(Pm_Snapshot *Info, Pm_Region &Size);

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

        virtual pm_color_t BlendColors(pm_color_t Fore, pm_color_t Back);

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
        virtual void PaintSimpleAliasedText(pm_point_t Where,
            const pm_char_t *pText, Pm_Brush &Brush, pm_int_t Count,
            Pm_Region &Size);

       #if defined(PRISM_TRUE_ALIAS)
        virtual void PaintTrueAliasedText(pm_point_t Where,
            const pm_char_t *pText, Pm_Brush &Brush, pm_int_t Count,
            Pm_Region &Clip);

        virtual void PaintAliasedTextToAlphaMap(pm_point_t Where,
                        const pm_char_t *pText, Pm_Brush &Brush,
                        pm_int_t Count, Pm_Region &Clip);
       #endif
       #endif

       #ifdef PRISM_TRUE_ALIAS
        virtual void PaintAAMapToPortal(pm_point_t Where,
            const pm_bitmap_t *pMap, Pm_Brush &Brush,
            const Pm_Region &Portal);
       #endif

        virtual void Paint32BitRleBitmap(const pm_point_t Where,
                const pm_bitmap_t *pGetmap, const Pm_Region &Portal);

        virtual void PaintNativeBitmap(const pm_point_t Where,
                                              const pm_bitmap_t *pGetMap,
                                              const Pm_Region &Portal);

        virtual void PaintTextToPortal(pm_point_t Where, const pm_char_t *pText,
                                Pm_Brush &Brush, pm_int_t Count,
                                Pm_Region &Size);

        virtual uint8_t *GetFrameBufferAddress(void);

        virtual void HorizontalLine(pm_int_t XStart, pm_int_t XEnd,
            pm_int_t YPos, pm_color_t Color, pm_int_t Width);

        virtual void HorizontalLineXOR(pm_int_t XStart, pm_int_t XEnd,
                                       pm_int_t YPos);
 
        virtual void LineToPortal(pm_int_t xStart, pm_int_t yStart, pm_int_t xEnd,
                              pm_int_t yEnd, Pm_Region &Size, Pm_Brush &Brush);

        void MakeAAColors(pm_color_t *pPutColors, Pm_Brush &Brush);

         virtual void ShiftRegionInPortal(const Pm_Region &Portal,
                                  const pm_int_t xMove, const pm_int_t yMove);

        pm_color_t TrueAlias(pm_color_t Back, pm_color_t Fore, pm_int_t Blend);

       #if defined(PRISM_ALIASED_LINES) && defined(PRISM_TRUE_ALIAS)
        virtual void TrueAliasLineToPortal(pm_int_t xStart, pm_int_t yStart,
            pm_int_t xEnd, pm_int_t yEnd, const Pm_Region &Portal,
            Pm_Brush &Brush);
       #endif

        virtual void VerticalLine(pm_int_t YStart, pm_int_t YEnd,
            pm_int_t XPos, pm_color_t Color, pm_int_t Width);

        virtual void VerticalLineXOR(pm_int_t YStart, pm_int_t YEnd,
            pm_int_t XPos);

        pm_color_t mPalette[256];
};

#endif



