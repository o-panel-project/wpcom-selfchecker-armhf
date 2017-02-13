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
//  prism_framework.h - Pm_Framework class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_FRAMEWORK_
#define _PRISM_FRAMEWORK_

BEGIN_NAMESPACE_PRISM

class Pm_Screen_Manager;

///////////////////////////////////////////////////////////////////////////////
class Pm_Framework {

    public:

        static inline Pm_Paint_Engine *Engine() {return mpPaintEngine;}
        static inline Pm_Event_Manager *EventManager() {return mpEventManager;}

       #ifdef PRISM_KEYPAD_SUPPORT
        static inline Pm_Input_Device *KeypadDevice() { return mpKeypadDevice;}
       #endif

       #ifdef PRISM_MOUSE_SUPPORT
        static inline Pm_Input_Device *MouseDevice() { return mpMouseDevice;}
       #endif

        static void SetEventManager(Pm_Event_Manager *pe)
        {
            mpEventManager = pe;
        }

        static inline Pm_Screen_Manager *ScreenManager()
        {
            return mpScreenManager;
        }

       #ifdef PRISM_KEYPAD_SUPPORT
        static void SetKeypadDevice(Pm_Input_Device *kd) {mpKeypadDevice = kd;}
       #endif

       #ifdef PRISM_MOUSE_SUPPORT
        static void SetMouseDevice(Pm_Input_Device *md) {mpMouseDevice = md;}
       #endif

        static void SetPaintEnginePtr(Pm_Paint_Engine *pe) {mpPaintEngine = pe;}

        static void SetScreenManagerPtr(Pm_Screen_Manager *pm)
        {
            mpScreenManager = pm;
        }
        static void SetTimerManager(Pm_Timer_Manager *ps) {mpTimerManager = ps;}

       #ifdef PRISM_TOUCH_SUPPORT
        static void SetTouchDevice(Pm_Input_Device *td) {mpTouchDevice = td;}
       #endif

        static inline Pm_Timer_Manager *TimerManager(void) {return mpTimerManager;}

       #ifdef PRISM_TOUCH_SUPPORT
        static inline Pm_Input_Device *TouchDevice() { return mpTouchDevice;}
       #endif

    protected:

        static Pm_Screen_Manager *mpScreenManager;
        static Pm_Event_Manager  *mpEventManager;
        static Pm_Paint_Engine   *mpPaintEngine;
        static Pm_Timer_Manager  *mpTimerManager;

       #ifdef PRISM_TOUCH_SUPPORT
        static Pm_Input_Device *mpTouchDevice;
       #endif

       #ifdef PRISM_KEYPAD_SUPPORT
        static Pm_Input_Device *mpKeypadDevice;
       #endif

       #ifdef PRISM_MOUSE_SUPPORT
        static Pm_Input_Device *mpMouseDevice;
       #endif
};

///////////////////////////////////////////////////////////////////////////////
inline void Bitmap(pm_point_t Where, pm_bitmap_t *pGetmap)
{
    Pm_Framework::Engine()->Bitmap(Where, pGetmap);
}

///////////////////////////////////////////////////////////////////////////////
inline void Bitmap(pm_point_t Where, pm_uint_t BitmapId)
{
    Pm_Framework::Engine()->Bitmap(Where, PBR(BitmapId));
}

///////////////////////////////////////////////////////////////////////////////
inline void BitmapFill(Pm_Region &Size, pm_bitmap_t *pGetmap)
{
    Pm_Framework::Engine()->BitmapFill(Size, pGetmap);
}

///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_FULL_GRAPHICS)
inline void Circle(pm_int_t xCenter, pm_int_t yCenter,
                    pm_int_t Rad, Pm_Brush &Brush)
{
    Pm_Framework::Engine()->Circle(xCenter, yCenter, Rad, Brush);
}
#endif

///////////////////////////////////////////////////////////////////////////////
inline void CloseCanvas(void)  {Pm_Framework::Engine()->CloseCanvas();}

///////////////////////////////////////////////////////////////////////////////
inline void DestroyBitmap(pm_bitmap_t *pMap)
{
    Pm_Framework::Engine()->DestroyBitmap(pMap);
}

///////////////////////////////////////////////////////////////////////////////
inline void DestroyCanvas(pm_int_t Canvas)
{
    Pm_Framework::Engine()->DestroyCanvas(Canvas);
}

///////////////////////////////////////////////////////////////////////////////
inline Pm_Event_Manager *EventManager() {return Pm_Framework::EventManager();}


///////////////////////////////////////////////////////////////////////////////
inline void Line(pm_int_t XStart, pm_int_t YStart, pm_int_t XEnd, 
                         pm_int_t YEnd, Pm_Brush &Brush)
{
    Pm_Framework::Engine()->Line(XStart, YStart, XEnd, YEnd, Brush);
}

///////////////////////////////////////////////////////////////////////////////
inline void PaintText(pm_point_t Where, const pm_char_t *pText,
                  Pm_Brush &Brush, pm_int_t Count = -1)
{
    Pm_Framework::Engine()->PaintText(Where, pText, Brush, Count);
}

///////////////////////////////////////////////////////////////////////////////
inline void PutPixel(pm_int_t x, pm_int_t y, pm_color_t Color)
{
    Pm_Framework::Engine()->PutPixel(x, y, Color);
}

///////////////////////////////////////////////////////////////////////////////
inline void Rectangle(const Pm_Region &Size, Pm_Brush &Brush)
{
    Pm_Framework::Engine()->Rectangle(Size, Brush);
}

///////////////////////////////////////////////////////////////////////////////
inline pm_bool_t RegionMove(Pm_Region Get, Pm_Region ClipTo,
    pm_int_t xShift, pm_int_t yShift)
{
    return (Pm_Framework::Engine()->RegionMove(Get, ClipTo, xShift, yShift));
}

///////////////////////////////////////////////////////////////////////////////
inline pm_bitmap_t *ResizeImage(pm_bitmap_t *pMap,
    pm_int_t Width, pm_int_t Height)
{
    return Pm_Framework::Engine()->ResizeImage(pMap, Width, Height);
}

///////////////////////////////////////////////////////////////////////////////
inline Pm_Screen_Manager *ScreenManager()
{
    return Pm_Framework::ScreenManager();
}

///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_MOUSE_SUPPORT)
inline void SetPointerType(pm_uint_t Type)
{
    Pm_Framework::Engine()->SetPointerType(Type);
}
#else
inline void SetPointerType(pm_uint_t Type) {}
#endif

///////////////////////////////////////////////////////////////////////////////
inline pm_int_t TextHeight(pm_uint_t FontId)
{
    return Pm_Framework::Engine()->TextHeight(FontId);
}

///////////////////////////////////////////////////////////////////////////////
inline pm_int_t TextHeight(const pm_font_t *pFont)
{
    return Pm_Framework::Engine()->TextHeight(pFont);
}

///////////////////////////////////////////////////////////////////////////////
inline pm_int_t TextWidth(const pm_char_t *pText, pm_uint_t FontId)
{
    return Pm_Framework::Engine()->TextWidth(pText, FontId);
}

///////////////////////////////////////////////////////////////////////////////
inline pm_int_t TextWidth(const pm_char_t *pText, const pm_font_t *pFont)
{
    return Pm_Framework::Engine()->TextWidth(pText, pFont);
}

END_NAMESPACE_PRISM

#endif