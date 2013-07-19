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
//  prism_paint_engine.h - Prism paint engine base class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_PAINT_ENGINE_
#define _PRISM_PAINT_ENGINE_

BEGIN_NAMESPACE_PRISM

#define PM_NUM_CANVASES        50

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_mouse_t
{
    pm_bitmap_t *Bitmap;
    pm_int_t xOffset;
    pm_int_t yOffset;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_portal_t;     // forward reference


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_canvas_t
{
    pm_color_t **pScanPointers;
    pm_portal_t *pPortal;
    pm_bitmap_t *pMap;
    Pm_Widget *pNotify;
    pm_canvas_t *pNext;
    pm_canvas_t *pPrev;

    Pm_Region Invalid;

    pm_int_t Id;
    pm_int_t Alpha;
    pm_uint_t Type;
    pm_int_t PaintCount;
    pm_int_t HardwareLayer;

    pm_int_t PaintOffsetX;
    pm_int_t PaintOffsetY;
    pm_int_t DisplayOffsetX;
    pm_int_t DisplayOffsetY;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct pm_paint_context_t
{
    pm_portal_t *pPortalStart;
    pm_color_t **pScanPointers;
   
   #if defined(PRISM_PAINT_ROTATED)
    pm_int_t   xPaintOffset;
   #else
    pm_int_t   yPaintOffset;
   #endif

    Pm_Region  Clip;
    pm_canvas_t *pCanvas;
};

#define PM_CANVAS_TYPE_SIMPLE      0x0000    
#define PM_CANVAS_TYPE_HARDWARE    0x0001 
#define PM_CANVAS_TYPE_VISIBLE     0x0002 
#define PM_CANVAS_TYPE_OVERLAY     0x0004 
#define PM_CANVAS_TYPE_MANAGED     0x0008 
#define PM_CANVAS_TYPE_CLIPPED     0x0010 
#define PM_CANVAS_TYPE_ALPHA_CHAN  0x0020 

///////////////////////////////////////////////////////////////////////////////

enum PrismPointerTypes {
    PM_POINTER_NORMAL = 0,
    PM_POINTER_YSTRETCH,
    PM_POINTER_XSTRETCH,
    PM_POINTER_NESW_SIZE,
    PM_POINTER_NWSE_SIZE,
    PM_POINTER_TEXT_SELECT,
    PM_NUM_SYSTEM_POINTERS
};

#define PM_NUM_POINTER_IDS (PM_NUM_SYSTEM_POINTERS + PRISM_USER_PTR_TYPES)

#if defined(PRISM_PAINT_ROTATED)
  #define PM_PIX_PTR(_Line_) \
     (mpContext->pScanPointers[_Line_ - mpContext->xPaintOffset])
#else
  #define PM_PIX_PTR(_Line_) \
     (mpContext->pScanPointers[_Line_ - mpContext->yPaintOffset])
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct RGB_VAL
{
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    RGB_VAL(uint8_t _Red, uint8_t _Blue, uint8_t _Green)
    {
        Red = _Red;
        Green = _Green;
        Blue = _Blue;
    }
    RGB_VAL()
    {
        Red = Green = Blue = 0;
    }
    RGB_VAL operator += (RGB_VAL InVal)
    {
        Red = (uint8_t) (Red + InVal.Red);
        Green = (uint8_t) (Green + InVal.Green);
        Blue = (uint8_t) (Blue + InVal.Blue);
        return *this;
    }
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Paint_Engine
{
    public:
        Pm_Paint_Engine(const Pm_Region &);
        virtual ~Pm_Paint_Engine();

       #if defined(PRISM_MOUSE_SUPPORT)
        uint8_t AddPointerType(pm_bitmap_t *pMap, pm_int_t XOffset,
                                pm_int_t YOffset);
       #endif

        void Bitmap(pm_point_t Where, pm_bitmap_t *pMap);

        void BitmapFill(Pm_Region &Size, pm_bitmap_t *pMap);

        void BitmapOverlay(Pm_Widget *pCaller, Pm_Region Size,
            pm_bitmap_t *pMap);

        void CanvasToFront(pm_int_t Canvas);

       #ifdef PRISM_FULL_GRAPHICS
        virtual void Circle(pm_int_t xCenter, pm_int_t yCenter,
                            pm_int_t Rad, Pm_Brush &Brush);
       #endif

        virtual void CloseCanvas();

        virtual uint8_t ColorFormat(void) {return mColorFormat;}

        virtual pm_bitmap_t *CreateBitmap(pm_int_t Width, pm_int_t Height,
            pm_int_t Style = BMP_FORMAT_RAW|BMP_TRANS_FLAG);

        virtual pm_int_t CreateCanvas(pm_int_t Type,
            pm_int_t Width, pm_int_t Height, pm_int_t xOffset, pm_int_t yOffset,
            pm_int_t HardLayer = -1, Pm_Widget *pNotify = NULL);

        void DestroyBitmap(pm_bitmap_t *pMap);
        virtual void DestroyCanvas(pm_int_t Canvas);
        pm_bitmap_t *DestructiveBitmapRetrieve(pm_int_t Canvas);

       #ifdef PRISM_BITMAP_WRITER
        uint8_t *DumpWindowsBitmap(uint32_t *pPutSize, Pm_Region &Portal);
       #endif

        pm_canvas_t *FindCanvas(pm_int_t CanvasId);

       #if defined(PRISM_FULL_GRAPHICS)
        void FillCircle(const Pm_Region &Portal, pm_int_t xCenter,
                pm_int_t yCenter, pm_int_t Rad, pm_color_t Color);
       #endif

       #ifdef PRISM_GRAPHIC_SCALING
        virtual pm_color_t GetBitmapPixel(pm_int_t x, pm_int_t y,
                                        pm_bitmap_t *pMap) = 0;
       #endif

        virtual uint8_t *GetPalette(uint32_t *pPutSize) = 0;
        virtual uint8_t GetColorFormat() {return mColorFormat;}
        virtual pm_color_t GetPixel(pm_int_t x, pm_int_t y);

       #if defined(PRISM_MOUSE_SUPPORT)
        pm_bitmap_t *GetPointer(void) {return mpCurPointer;}
        pm_point_t GetPointerPos(void)
        {
            return mLastPointerPos;
        }
        uint8_t GetPointerType(void);
       #endif

        pm_bitmap_t *GetCanvasBitmap(pm_int_t Canvas);

       #if defined(PRISM_MOUSE_SUPPORT)
        pm_int_t GetXPointerOffset(void) {return mCurrentXOffset;}
       #endif

        pm_int_t GetXRes(void) {return mHRes;}

       #if defined(PRISM_MOUSE_SUPPORT)
        pm_int_t GetYPointerOffset(void) {return mCurrentYOffset;}
       #endif

        pm_int_t GetYRes(void) {return mVRes;}

        virtual pm_canvas_t *HideCanvas(pm_int_t Canvas);

        virtual void InitHardwareCanvas(pm_int_t Width, pm_int_t Height,
            pm_canvas_t *pCanvas, pm_int_t HardLayer) {}

       #if defined(PRISM_MOUSE_SUPPORT)
        virtual void InitMousePointers();
       #endif

        virtual void Line(pm_int_t XStart, pm_int_t YStart,
                          pm_int_t XEnd, pm_int_t YEnd, Pm_Brush &Brush);

        virtual pm_int_t OpenCanvas(Pm_Widget *Caller,
            const Pm_Region &Invalid, pm_int_t Canvas);

        pm_uint_t PaintNesting(void) {return mPaintNesting;}

       #if (PRISM_COLOR_FORMAT >= PM_CF_16BPP_RGB)
        virtual void PaintAliasMap(pm_point_t Where, const pm_bitmap_t *pMap,
            Pm_Brush &Brush);
       #endif

        void PaintRotatedText(pm_point_t Center, pm_int_t iAngle,
            const pm_char_t *pText, Pm_Brush &Brush);

        virtual void PaintText(pm_point_t Where,
                              const pm_char_t *Text, Pm_Brush &Brush,
                              pm_int_t Count = -1);

       #ifdef PRISM_PLOTPOINT_FUNCTION
        virtual void PlotPointToPortal(pm_int_t x, pm_int_t y, pm_color_t c) = 0;
       #endif

       #if defined(PRISM_FULL_GRAPHICS)
        virtual void Polygon(pm_point_t *pPoints,
                             pm_uint_t NumPoints, Pm_Brush &Brush);

        virtual void PolyLine(pm_point_t *pPoints,
                              pm_uint_t NumPoints, Pm_Brush &Brush);
       #endif

       #ifdef PRISM_GRAPHIC_SCALING
        virtual void PutBitmapPixel(pm_int_t x, pm_int_t y, pm_bitmap_t *pMap,
                                    pm_color_t c) = 0;
       #endif

        virtual void  PutPixel(pm_int_t x, pm_int_t y,
                               pm_color_t Color);

        virtual void Rectangle(const Pm_Region &Size, Pm_Brush &Brush);

       #if defined(PRISM_FULL_GRAPHICS)
        void RectangleXOR(const Pm_Region &InRect);
       #endif

        virtual pm_bool_t RegionMove(Pm_Region &Get, Pm_Region &ClipTo,
                                 pm_int_t xShift, pm_int_t yShift);

        virtual void ResetPalette(void) = 0;

       #ifdef PRISM_GRAPHIC_SCALING
        virtual pm_bitmap_t *ResizeImage(pm_bitmap_t *pSrc, pm_int_t Width,
                                       pm_int_t Height);
       #endif

        virtual void Restore(Pm_Snapshot *Info);

        virtual void SetPalette(pm_int_t First, pm_int_t Num,
                                const uint8_t *pGet) = 0;

        virtual void SetPowerSaveMode(pm_int_t Mode) {}

       #if defined(PRISM_MOUSE_SUPPORT)
        virtual void SetPointerPos(pm_point_t);
        virtual void SetPointerType(pm_uint_t Type);
       #endif

        virtual void SetCanvasAlpha(pm_int_t Canvas, pm_int_t Alpha);

        void SetCanvasDisplayOffset(pm_int_t Canvas,
            pm_int_t xOffset, pm_int_t yOffset);

        void SetCanvasPaintOffset(pm_int_t Canvas,
            pm_int_t xOffset, pm_int_t yOffset);

        void SetCanvasOffset(pm_int_t Canvas, pm_int_t xOffset,
            pm_int_t yOffset);

        virtual void SetCanvasSize(pm_int_t Canvas, pm_int_t xSize,
            pm_int_t ySize, pm_int_t xOffset, pm_int_t yOffset);

       #if defined(PRISM_MOUSE_SUPPORT)
        virtual void ShowPointer(pm_bool_t Show);
       #endif

        virtual pm_canvas_t *ShowCanvas(pm_int_t Canvas);

        virtual void Snapshot(Pm_Snapshot *Info, Pm_Region &Region) = 0;

        pm_int_t TextHeight(pm_int_t FontId)
        {
            pm_font_t *pFont = Pm_Resource_Manager::GetFont(FontId);
            if (pFont)
            {
                return TextHeight(pFont);
            }
            return 0;
        }

        virtual pm_int_t TextHeight(const pm_font_t *pFont);

        pm_int_t TextWidth(const pm_char_t *pText, pm_int_t FontId,
            pm_int_t Len = -1)
        {
            pm_font_t *pFont = Pm_Resource_Manager::GetFont(FontId);

            if (pFont)
            {
                return TextWidth(pText, pFont, Len);
            }
            return 0;
        }

        virtual pm_int_t TextWidth(const pm_char_t *pText, const pm_font_t *pFont,
                                 pm_int_t Len = -1);

        pm_bool_t TrimRegion(Pm_Region &Size)
        {
            if (!mPaintNesting)
            {
                return FALSE;
            }

            Size &= mpContext->Clip;

            if (Size.Left > Size.Right ||
                Size.Bottom < Size.Top)
            {
                return FALSE;
            }
            return TRUE;
        }

    protected:

        virtual void BitmapToPortal(const pm_point_t Where,
            const pm_bitmap_t *pMap, const Pm_Region &Portal) = 0;

       #if defined(PRISM_FULL_GRAPHICS)
        void CircleToPortal(const Pm_Region &LimitRect,    
            pm_int_t xCenter,  pm_int_t yCenter,
            pm_int_t Rad, Pm_Brush &Brush);
       #endif

       #if defined(PRISM_FULL_GRAPHICS)
        virtual void FillPolygon(Pm_Region &Bound,
                                 pm_point_t *pPoints, pm_int_t NumPoints,
                                 pm_color_t Color);

        virtual void FillPolygonInPortal(const Pm_Region &Portal,
            pm_point_t *pPoints, pm_int_t NumPoints, pm_color_t Color);
       #endif

       #if defined(PRISM_FREETYPE_SUPPORT)
        pm_int_t FreetypeTextWidth(const pm_char_t *Text,
            const pm_font_t *Font, pm_int_t Len);
       #endif

       #if defined(PRISM_MOUSE_SUPPORT)
        virtual void HidePointer(void);
       #endif

        virtual void HorizontalLine(pm_int_t xStart, pm_int_t xEnd,
                                    pm_int_t yVal, pm_color_t Color,
                                    pm_int_t Width) = 0;
       #if defined(PRISM_FULL_GRAPHICS)
        virtual void HorizontalLineXOR(pm_int_t xStart, pm_int_t xEnd,
            pm_int_t yVal) = 0;
       #endif

       #if defined(PRISM_GRAPHICS_HEAP)
         virtual void InitializeGraphicsHeap(uint8_t *pStart, uint32_t Size);
       #endif

        virtual void LineToPortal(pm_int_t xStart, pm_int_t yStart,
                              pm_int_t xEnd, pm_int_t yEnd,  Pm_Region &Portal,
                              Pm_Brush &Brush) = 0;

        virtual void MemoryToVisible(void) = 0;
        
       #if defined(PRISM_FULL_GRAPHICS)
        void OutlineCircle(const Pm_Region &Portal, pm_int_t xCenter,
                 pm_int_t yCenter, pm_int_t Rad, Pm_Brush &Brush);
       #endif

       #if defined(PM_SW_MOUSE)
        virtual pm_bool_t EnterMouseContext(const Pm_Region &Invalid);
        virtual void ExitMouseContext(void) {CloseCanvas();}
       #endif

       #if (PRISM_COLOR_FORMAT >= PM_CF_16BPP_RGB)
        virtual void PaintAAMapToPortal(pm_point_t Where,
            const pm_bitmap_t *pMap, Pm_Brush &Brush,
            const Pm_Region &Portal) = 0;
       #endif

       #if defined(PRISM_FREETYPE_SUPPORT)

        virtual void PaintFreetypeChar(FT_Bitmap *pMap,
            pm_int_t xPos, pm_int_t yPos, pm_color_t Color,
            Pm_Region &Portal) = 0;

        void PaintFreetypeText(pm_point_t Where,
            const pm_char_t *pText, Pm_Brush &Brush,
            pm_int_t Count, Pm_Region &Portal);

       #endif

        virtual void PaintTextToPortal(pm_point_t Put, const pm_char_t *Text,
                                  Pm_Brush &Brush, pm_int_t Len,
                                  Pm_Region &Portal) = 0;

        void PaintWideLine(pm_int_t xStart, pm_int_t xEnd, pm_int_t YVal,
            pm_int_t Width, pm_color_t LineColor);

        void PaintWideVertLine(pm_int_t yStart, pm_int_t yEnd, pm_int_t xVal,
            pm_int_t Width, pm_color_t LineColor);

       #ifdef PRISM_ALIASED_LINES

        virtual void PaintAntiAliasedLine(pm_int_t XStart, pm_int_t YStart,
                            pm_int_t XEnd, pm_int_t YEnd, Pm_Brush &Brush);

        virtual void PaintAntiAliasedLineToPortal(pm_int_t xStart,
            pm_int_t yStart, pm_int_t xEnd, pm_int_t yEnd,
            const Pm_Region &Portal, Pm_Brush &Brush) = 0;
       #endif

       #if defined(PRISM_FULL_GRAPHICS)
        virtual void PatternFillPolygon(Pm_Region &Bound,
                                pm_point_t *pPoints, pm_int_t NumPoints,
                                Pm_Brush &Brush);

        virtual void PatternFillPolygonInPortal(const Pm_Region &Portal,
                                            pm_point_t *pPoints,
                                            pm_int_t NumPoints, Pm_Brush &Brush);

        virtual void PatternLine(pm_int_t XStart,
                                 pm_int_t YStart, pm_int_t XEnd, pm_int_t YEnd,
                                 Pm_Brush &Brush);

        virtual void PatternLineToPortal(pm_int_t XStart, pm_int_t YStart,
                        pm_int_t XEnd, pm_int_t YEnd, Pm_Region &Size,
                        Pm_Brush &Brush);
       #endif

        virtual pm_color_t ReadPortalPixel(pm_int_t x, pm_int_t y) = 0;

        virtual void RectangleToPortal(const Pm_Region &InRect,
            Pm_Region &Portal, Pm_Brush &Brush);

        virtual void ShiftRegionInPortal(const Pm_Region &Portal,
            const pm_int_t xShift, const pm_int_t yShift) = 0;

        virtual void SetCanvasDisplayOffset(pm_canvas_t *pCanvas,
            pm_int_t xOffset, pm_int_t yOffset);

        virtual void SetCanvasPaintOffset(pm_canvas_t *pCanvas,
            pm_int_t xOffset, pm_int_t yOffset);

        virtual void SetCanvasOffset(pm_canvas_t *pCanvas,
            pm_int_t xOffset, pm_int_t yOffset);

        virtual void SquareEndLine(pm_int_t xStart, pm_int_t yStart,
         pm_int_t xEnd, pm_int_t yEnd, Pm_Brush &Brush);

        void SquareEndLineToPortal(const Pm_Region &LimitRect,
            pm_fixed_point *pPoints, Pm_Brush &Brush);

        void AssignCanvasPortal(pm_canvas_t *pCanvas);

        virtual void VerticalLine(pm_int_t yStart, pm_int_t yEnd,
                                  pm_int_t xVal, pm_color_t Color,
                                  pm_int_t Width) = 0;

      #if defined(PRISM_FULL_GRAPHICS)
        virtual void VerticalLineXOR(pm_int_t ys, pm_int_t ye, pm_int_t x) = 0;
      #endif

        pm_canvas_t *mpCanvasList;
        uint8_t mColorFormat;
        pm_int_t mOrientation;       

       #if defined(PRISM_GRAPHICS_HEAP)
        pm_heap_t mGraphicsHeap;
       #endif

       #if defined(PRISM_MOUSE_SUPPORT)
        pm_bitmap_t *mpCurPointer;
        pm_bool_t mPointerHidden;
        pm_bool_t mShowPointer;
        pm_int_t mCurrentXOffset;
        pm_int_t mCurrentYOffset;


        pm_mouse_t mpPointers[PM_NUM_POINTER_IDS];

        #if defined(PRISM_MOUSE_LAYER)
        pm_int_t mMouseCanvas;
        #endif

        #if defined(PM_SW_MOUSE)
        pm_portal_t *mpMousePortal;
        Pm_Snapshot mSnapshot;
        #endif

        pm_point_t mLastPointerPos;

       #endif

        pm_paint_context_t *mpContext;

        pm_int_t mHRes;
        pm_int_t mVRes;

        pm_uint_t mPaintNesting;

       #if defined(PRISM_MOUSE_SUPPORT)

       #endif

     private:

        pm_paint_context_t *mpContextStack;

       #ifdef PRISM_GRAPHIC_SCALING

        void BitmapStretch(pm_int_t SrcWidth,  pm_int_t SrcHeight,
                         pm_int_t DestWidth, pm_int_t DestHeight,
                         pm_bitmap_t *pSrc, pm_bitmap_t *pDest);

        void BitmapStretchStep(pm_int_t x2, pm_int_t y2,
                    pm_int_t yr,pm_int_t yw, pm_bitmap_t *pSrc,
                    pm_bitmap_t *pDest);
       #endif
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
Pm_Paint_Engine *CreatePm_Paint_Engine(const Pm_Region &Size);

END_NAMESPACE_PRISM

#endif




