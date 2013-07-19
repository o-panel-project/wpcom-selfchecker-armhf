
#ifndef _PRISM_X11_SCREEN_
#define _PRISM_X11_SCREEN_

#include "prism_pe_base_16bpp.h"  // include the 16 bit template class

#if defined(PRISM_MULT_SW_CANVASES)
///////////////////////////////////////////////////////////////////////////////
// We also need a forward reference to Pm_Panel for the method prototypes
///////////////////////////////////////////////////////////////////////////////
class Pm_Panel;
#endif  // PRISM_MULT_SW_CANVASES

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class PrismX11PaintEngine : public Pm_Paint_Engine16Bit
{
    public:

        PrismX11PaintEngine(const Pm_Region &);
        virtual ~PrismX11PaintEngine();

        XVisualInfo* GetXVisualInfo(void) const { return mpVisual; }

       #if defined(PRISM_SIZE_TO_WIN_CLIENT)
        void SetNewX11WinSize(pm_uint_t wWidth, pm_uint_t wHeight);
       #endif

       #if defined(PRISM_MOUSE_SUPPORT)
        virtual void SetPointerType(pm_uint_t Type);
       #endif  // PRISM_MOUSE_SUPPORT
        void SetWindowTitle(char *pText);

    protected:
        virtual pm_bool_t AllocateXScreen(pm_uint_t Width, pm_uint_t Height);
        virtual pm_bool_t GetPreferredXVisual(pm_uint_t Depth, pm_int_t Class,
                XVisualInfo *pVisualInfo);

        virtual void MemoryToVisible(void);

        pm_color_t *mpImageMem;        
        pm_color_t *mpVidMem;
        pm_color_t** mpImageScanPointers;
        XImage* mpXImage;
        XVisualInfo* mpVisual;
        Cursor mtCursor;

        uint32_t mwNumPalEntries;

       #if defined(PRISM_MULT_SW_CANVASES)
        uint8_t *mpCompositeBuffer;
       #endif
};

#endif  // X11SCR16_HPP_


