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
//  engine_common_16bpp.h - Prism drawing engine for Windows applications
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_WIN_ENGINE_
#define _PRISM_WIN_ENGINE_

#include "prism_pe_base_32bpp.h"  


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


typedef struct 
{
    BITMAPINFOHEADER bmhead;
    DWORD bmiColors[3];
} BMHEAD;

class Pm_Panel;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class PrismWinPaintEngine : public Pm_Paint_Engine32Bit
{
    public:
        PrismWinPaintEngine(const Pm_Region &);
        virtual ~PrismWinPaintEngine();

        virtual pm_int_t OpenCanvas(Pm_Widget *Caller, const Pm_Region &Invalid,
            pm_int_t Canvas);
        virtual void CloseCanvas(void);

        HWND GetHWnd(void) {return mHWnd;}
        virtual uint8_t *GetPalette(uint32_t *pPutSize);
        virtual void ResetPalette(void);

       #ifndef PRISM_SIZE_TO_WIN_CLIENT
        void SetNewWindowsWinSize(pm_int_t Width, pm_int_t Height)
        {
            mWinRectXOffset = (Width - mHRes) / 2;
            mWinRectYOffset = (Height - mVRes) / 2;            
        }
       #else
        void SetNewWindowsWinSize(pm_int_t Width, pm_int_t Height);
       #endif

        virtual void SetPalette(pm_int_t First, pm_int_t Count, 
                                const uint8_t *pPal);
        virtual void SetPointerType(pm_uint_t Type);
        void SetHWnd(HWND hWnd);
        void SetWindowTitle(pm_char_t *pText);

        pm_int_t mWinRectXOffset;
        pm_int_t mWinRectYOffset;
        pm_int_t mWinRectMouseXOffset;
        pm_int_t mWinRectMouseYOffset;

    protected:

        virtual void MemoryToVisible(void);

        pm_color_t *mpVidMem;
        HWND mHWnd;
        HPALETTE mhPalette;
        HDC  mHdc;

    private:
        
       #if defined(PRISM_MULT_SW_CANVASES)
        uint8_t *mpCompositeBuffer;
       #endif
};


#endif

