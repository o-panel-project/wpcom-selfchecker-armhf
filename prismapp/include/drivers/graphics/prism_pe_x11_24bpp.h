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
//  prism_pe_x11_24bpp.h - Prism paint engine for Linux/X11, true color mode.
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_X11_SCREEN_
#define _PRISM_X11_SCREEN_

#include "prism_pe_base_32bpp.h" 

#if defined(PRISM_MULT_SW_CANVASES)
///////////////////////////////////////////////////////////////////////////////
// We also need a forward reference to Pm_Panel for the method prototypes
///////////////////////////////////////////////////////////////////////////////
class Pm_Panel;
#endif  // PRISM_MULT_SW_CANVASES

#if defined(PRISM_OVG_ANIMATION) || defined(PRISM_OGL_ANIMATION)
#if defined(PRISM_OVG_ANIMATION)
#include <VG/openvg.h>
#endif // PRISM_OVG_ANIMATION
#if defined(PRISM_OGL_ANIMATION)
#include <GLES/gl.h>
#endif // PRISM_OGL_ANIMATION
#include <EGL/egl.h>

#if defined(PRISM_OGL_ANIMATION)
struct ogl_image_t
{
	GLuint      texture;
	GLsizei     width;
	GLsizei     height;
	GLsizei     pow2Width;
	GLsizei     pow2Height;
	pm_color_t  *data;		// XXX Used for copy images
};
#endif // PRISM_OGL_ANIMATION

struct oxx_animwin_context_t
{
	pm_uint_t   Id;
	EGLSurface  eglSurface;
	EGLContext  eglContext;
#if defined(PRISM_OVG_ANIMATION)
	VGImage     ovgImage_curr;	// flick work
	VGImage     ovgImage_next;	// flick work
	VGImage     ovgImage_zoom;	// zoom work
	Window      x11Window;
	GC          x11Gc;
	void*       px11ShmInfo;
	XImage*     px11XImage;
#endif // PRISM_OVG_ANIMATION
#if defined(PRISM_OGL_ANIMATION)
	ogl_image_t *oglImage_curr;	// flick work
	ogl_image_t *oglImage_next;	// flick work
	ogl_image_t *oglImage_zoom;	// zoom work
#endif // PRISM_OGL_ANIMATION
	Pm_Region   animWin;
	uint32_t    animWinStatus;
	Pm_Region   animWinClip;
	pm_int_t    flickType;
#if defined(PRISM_OVG_ANIMATION)
	VGfloat     zoomRatio_last;
	VGfloat     zoomRatio_curr;
	VGfloat     zoomRatio_next;
#endif // PRISM_OVG_ANIMATION
#if defined(PRISM_OGL_ANIMATION)
	GLfloat     zoomRatio_last;
	GLfloat     zoomRatio_curr;
	GLfloat     zoomRatio_next;
#endif // PRISM_OGL_ANIMATION
	pm_point_t  zoomPos_last;
	oxx_animwin_context_t *pPrev;
	oxx_animwin_context_t *pNext;
	Pm_Region   damage;
	pm_bool_t   bDamage;
	pm_int_t    damage_bytes_per_line;
	pm_int_t    damage_xoffset;
	pm_int_t    damage_yoffset;
};

enum Pm_AnimWin_AnimType {
	PM_ANIMWIN_FLICK_CONT = 1,
	PM_ANIMWIN_FLICK_FLIP,
	PM_ANIMWIN_FLICK_TURN
};
#endif // PRISM_OVG_ANIMATION || PRISM_OGL_ANIMATION

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class PrismX11PaintEngine : public Pm_Paint_Engine32Bit
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

       #if defined(PRISM_OVG_ANIMATION) || defined(PRISM_OGL_ANIMATION)
		pm_uint_t oxx_AnimWin_CreateContext(const Pm_Region &);
		void oxx_AnimWin_set_region_Damage(pm_uint_t, const Pm_Region &);
		void oxx_AnimWin_DestroyContext(pm_uint_t);
		void oxx_AnimWin_show(pm_uint_t);
		void oxx_AnimWin_hide(pm_uint_t);
		void oxx_AnimWin_sync(Pm_Region &);
		void oxx_AnimWin_AddStatus(pm_uint_t, uint32_t);
		void oxx_AnimWin_RemoveStatus(pm_uint_t, uint32_t);
		void oxx_AnimWin_flickType(pm_uint_t, int itype);
		void oxx_AnimWin_draw_bitmap_scale(pm_uint_t,
								pm_bitmap_t *, pm_point_t &, pm_int_t);
		void oxx_AnimWin_draw_zoom(pm_uint_t, pm_point_t &, float);
		void oxx_AnimWin_zoom_reset(pm_uint_t);
		void oxx_flick_run(pm_uint_t, pm_bitmap_t *, int, int);
		void InitHardwareCanvas(pm_int_t Width, pm_int_t Height,
            pm_canvas_t *pCanvas, pm_int_t HardLayer);
		pm_bitmap_t *oxx_AnimWin_capturePrism(pm_uint_t);
       #endif // PRISM_OVG_ANIMATION || PRISM_OGL_ANIMATION
    protected:
        virtual pm_bool_t AllocateXScreen(pm_uint_t Width, pm_uint_t Height);
        virtual pm_bool_t GetPreferredXVisual(pm_uint_t Depth, pm_int_t Class,
                XVisualInfo *pVisualInfo);

        virtual void MemoryToVisible(void);

        pm_color_t *mpImageMem;        
        //pm_color_t *mpVidMem;
        pm_color_t** mpImageScanPointers;
        XImage* mpXImage;
        XVisualInfo* mpVisual;
        Cursor mtCursor;

        uint32_t mwNumPalEntries;

       #if defined(PRISM_MULT_SW_CANVASES)
        uint8_t *mpCompositeBuffer;
       #endif
       #if defined(PRISM_OVG_ANIMATION) || defined(PRISM_OGL_ANIMATION)
		void oxx_AnimWin_Initialize(void);
		oxx_animwin_context_t *oxx_AnimWin_FindContext(pm_uint_t);
		void oxx_AnimWin_draw_bitmap(oxx_animwin_context_t *,
								pm_bitmap_t *, pm_point_t &);
		void oxx_AnimWin_draw_context(oxx_animwin_context_t *,
								pm_point_t &, pm_point_t &);
		pm_bitmap_t *oxx_AnimWin_capturePrism(oxx_animwin_context_t *);
		pm_bitmap_t *oxx_AnimWin_capturePrism(const Pm_Region &);
		pm_bool_t oxx_AnimWin_StatusIs(oxx_animwin_context_t *, uint32_t);
		void oxx_flick_run_cont(oxx_animwin_context_t *, int, int);
		void oxx_flick_run_flip(oxx_animwin_context_t *, int, int);
       #if defined(PRISM_OVG_ANIMATION)
		void ovg_flick_run_turn(oxx_animwin_context_t *, int, VGImage);
       #endif // PRISM_OVG_ANIMATION
       #if defined(PRISM_OGL_ANIMATION)
		void ogl_flick_run_turn(oxx_animwin_context_t *, int, ogl_image_t *);
       #endif // PRISM_OGL_ANIMATION
		EGLDisplay  eglDisplay;
		EGLConfig   eglConfig;
		pm_bool_t   eglInitDone;
		oxx_animwin_context_t *mpOxxAnimWinList;
		void oxx_AnimWin_receive_Damage(oxx_animwin_context_t *);
		void oxx_AnimWin_transfer_Damage(oxx_animwin_context_t *);
       #if defined(PRISM_OGL_ANIMATION)
		ogl_image_t *ogl_AnimWin_CreateImage(GLsizei, GLsizei);
		void ogl_AnimWin_ImageSubData(ogl_image_t *, const GLvoid *,
								GLint, GLint, GLsizei, GLsizei);
		void ogl_AnimWin_DrawImage(ogl_image_t *);
		void ogl_AnimWin_CopyImage(ogl_image_t *, GLint, GLint, ogl_image_t *,
								GLint, GLint, GLsizei, GLsizei);
		void ogl_AnimWin_DestroyImage(ogl_image_t *);
		void ogl_AnimWin_InitScene(oxx_animwin_context_t *);
       #endif // PRISM_OGL_ANIMATION
	   #endif // PRISM_OVG_ANIMATION || PRISM_OGL_ANIMATION
};

#endif  

