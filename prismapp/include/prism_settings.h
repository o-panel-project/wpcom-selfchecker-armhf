///////////////////////////////////////////////////////////////////////////////
//
// prism_settings.h- Prism Runtime Framework build settings.
//
///////////////////////////////////////////////////////////////////////////////

/* This file has been generated using the Prism Insight Program            */

// This file should NOT be edited by hand. To make changes, run the 
// Prism Insight application.
//
#if !defined(PRISM_DESKTOP_BUILD) && !defined(PRISM_TARGET_BUILD)
#define PRISM_TARGET_BUILD
#endif

#define BEGIN_NAMESPACE_PRISM
#define END_NAMESPACE_PRISM


///////////////////////////////////////////////////////////////////////////////
// Desktop Simulator
///////////////////////////////////////////////////////////////////////////////

#ifdef PRISM_DESKTOP_BUILD
#define PRISM_HARDWARE_CURSOR
#define PRISM_MOUSE_SUPPORT
//#define PRISM_FOR_X11
#define PRISM_FOR_WIN32
#define PRISM_SIZE_TO_WIN_CLIENT

///////////////////////////////////////////////////////////////////////////////
// DeskTop Paint Engine Settings
///////////////////////////////////////////////////////////////////////////////
#define PRISM_PAINT_ENGINE  "drivers/graphics/prism_pe_windows_32bpp.h"
#endif 



///////////////////////////////////////////////////////////////////////////////
// Target Operating System
///////////////////////////////////////////////////////////////////////////////

#ifdef PRISM_TARGET_BUILD
#define PRISM_MOUSE_SUPPORT
#define PRISM_FOR_X11
#define PRISM_THREADS_SUPPORT

///////////////////////////////////////////////////////////////////////////////
// Target Paint Engine Settings
///////////////////////////////////////////////////////////////////////////////
#define PRISM_PAINT_ENGINE  "drivers/graphics/prism_pe_x11_24bpp.h"
#define PRISM_TOUCH_SUPPORT
#define PRISM_TOUCH_INCLUDE     "drivers/touch/prism_atmel_9263_touch.h"
#endif 

///////////////////////////////////////////////////////////////////////////////
// Operating System
///////////////////////////////////////////////////////////////////////////////
#define PRISM_TASK_PRIORITY               0
#define PRISM_HEAP_SIZE               16384
#define PRISM_STACK_SIZE               4096
#define PRISM_TICS_SEC                   20
#define RTOS_TICS_SEC                   100


///////////////////////////////////////////////////////////////////////////////
// General Paint Engine Settings
///////////////////////////////////////////////////////////////////////////////


// #define PRODUCT_KEY_HANDLER ProductKeyHandler


#if defined(PRISM_FOR_X11)
extern int PRISM_DEFAULT_YRES;
extern int PRISM_DEFAULT_XRES;
#define PRISM_XVISUAL_DEPTH              24
#else
#define PRISM_DEFAULT_YRES              600
#define PRISM_DEFAULT_XRES             1024
#endif
#define PRISM_MULT_SW_CANVASES
#define PRISM_DOUBLE_BUFFER
#define PRISM_HARDWARE_CURSOR
#define PRISM_LCD_MODE
#define PRISM_PAINT_STACK_DEPTH          20

///////////////////////////////////////////////////////////////////////////////
// Native or Rotated Screen Orientation
///////////////////////////////////////////////////////////////////////////////

#define PRISM_ORIENTATION  PRISM_ORIENTATION_NATIVE


///////////////////////////////////////////////////////////////////////////////
// Target CPU Settings
///////////////////////////////////////////////////////////////////////////////
#define PRISM_CPU_ARM9


///////////////////////////////////////////////////////////////////////////////
// Color Format Settings
///////////////////////////////////////////////////////////////////////////////
#define PRISM_COLOR_FORMAT  PM_CF_32BPP_ARGB
#if PRISM_COLOR_FORMAT == PM_CF_16Bpp_RGB
#define PRISM_565_FORMAT
#endif
#define PRISM_NUM_PAL_ENTRIES           248
#define PRISM_GRAYSCALE_PAL_START       232


///////////////////////////////////////////////////////////////////////////////
// Standard Color Defines
///////////////////////////////////////////////////////////////////////////////
#if PRISM_COLOR_FORMAT > PM_CF_16BPP_LIMIT
#define PRISM_DEF_TRANSPARENCY   0x00000000
#define PRISM_DEF_BLACK           0xff000000
#define PRISM_DEF_RED             0xffbf0000
#define PRISM_DEF_GREEN           0xff00bf00
#define PRISM_DEF_BROWN           0xffbfbf00
#define PRISM_DEF_BLUE            0xff0000bf
#define PRISM_DEF_MAGENTA         0xffbf00bf
#define PRISM_DEF_CYAN            0xff00bfbf
#define PRISM_DEF_LIGHTGRAY       0xffc0c0c0
#define PRISM_DEF_DARKGRAY        0xff808080
#define PRISM_DEF_LIGHTRED        0xffff0000
#define PRISM_DEF_LIGHTGREEN      0xff00ff00
#define PRISM_DEF_YELLOW          0xffffff00
#define PRISM_DEF_LIGHTBLUE       0xff0000ff
#define PRISM_DEF_LIGHTMAGENTA    0xffff00ff
#define PRISM_DEF_LIGHTCYAN       0xff00ffff
#define PRISM_DEF_WHITE           0xffffffff
#else
#define PRISM_DEF_TRANSPARENCY    0x0000
#define PRISM_DEF_BLACK           0x0000
#define PRISM_DEF_RED             0xb800
#define PRISM_DEF_GREEN           0x05e0
#define PRISM_DEF_BROWN           0xbde0
#define PRISM_DEF_BLUE            0x0017
#define PRISM_DEF_MAGENTA         (PRISM_DEF_RED | PRISM_DEF_BLUE)
#define PRISM_DEF_CYAN            (PRISM_DEF_GREEN | PRISM_DEF_BLUE)
#define PRISM_DEF_LIGHTGRAY       0xc618
#define PRISM_DEF_DARKGRAY        0x8410
#define PRISM_DEF_LIGHTRED        0xf800
#define PRISM_DEF_LIGHTGREEN      0x07e0
#define PRISM_DEF_YELLOW          (PRISM_DEF_RED | PRISM_DEF_GREEN)
#define PRISM_DEF_LIGHTBLUE       0x001f
#define PRISM_DEF_LIGHTMAGENTA    0xf81f
#define PRISM_DEF_LIGHTCYAN       0x07ff
#define PRISM_DEF_WHITE           0xffff
#endif


///////////////////////////////////////////////////////////////////////////////
// Input Device Settings
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Painting Options
///////////////////////////////////////////////////////////////////////////////
#define PRISM_PORTALS
#define PRISM_FULL_CLIPPING
#define PRISM_FULL_GRAPHICS
#define PRISM_ALIASED_LINES
#define PRISM_TRUE_ALIAS
#define PRISM_GRADIENT_SUPPORT
#define PRISM_GRADIENT_BUTTONS
#define PRISM_TITLE_BAR
#define PRISM_GRAPHIC_SCALING
#define PRISM_DT_BORDER                   6
#define PRISM_DT_HEADER_SPACE             0
#define PRISM_DT_CELL_PADDING             8
#define PRISM_DT_CELL_SPACING             0
#define PRISM_DT_SEPARATOR_WIDTH          4
#define PRISM_DT_MAX_HEADER_LINES         0

#define PRISM_TBUTTON_PADDING             3
#define PRISM_BTN_TIMER1                  8
#define PRISM_BTN_TIMER2                  4
#define PRISM_RBUTTON_SPACING            10
#define PRISM_CBOX_SPACING               10
#define PRISM_BMP_TXT_PADDING             2
#define PRISM_MENU_CHK_WIDTH              8
#define PRISM_TB_WHITESPACE               2
#define PRISM_SYS_BTN_SIZE               16
#define PRISM_TOOLBAR_PAD_HEIGHT          2
#define PRISM_THICK_FRAME                 5


///////////////////////////////////////////////////////////////////////////////
// Widget Options
///////////////////////////////////////////////////////////////////////////////
#define PRISM_SCROLL_SUPPORT
#define PRISM_DATA_TABLE_SUPPORT


///////////////////////////////////////////////////////////////////////////////
// Runtime Graphic Reader Options
///////////////////////////////////////////////////////////////////////////////
#define PRISM_RUNTIME_GRAPHIC_READERS
#define PRISM_RUNTIME_JPG_READER
#define PRISM_RUNTIME_PNG_READER
#define PRISM_RUNTIME_GIF_READER
#define PRISM_RUNTIME_BMP_READER
#define PRISM_GRAPHIC_READER_FILEIO
#define PRISM_GRAPHIC_SCALING
#define BIR_INLINE_RESIZE
#define PRISM_ZIP	
#define PRISM_UNZIP


///////////////////////////////////////////////////////////////////////////////
// File I/O Options
///////////////////////////////////////////////////////////////////////////////
#define PRISM_FILEIO_ABSTRACTION
#define PRISM_FILESYS_ANSI_C


///////////////////////////////////////////////////////////////////////////////
// Font Settings
///////////////////////////////////////////////////////////////////////////////
#define PRISM_ALIASED_TEXT
#define PRISM_SJIS_CONVERSION

#ifdef PRISM_TARGET_BUILD
#define	TTF3	"/usr/share/fonts/truetype/dfphsg3.ttf"
#define	TTF5	"/usr/share/fonts/truetype/dfphsg5.ttf"
#define	TTF7	"/usr/share/fonts/truetype/dfphsg7.ttf"
#endif

#ifdef PRISM_FOR_WIN32
#define TTF3	"..\\bin\\dfphsg3.ttf"
#define TTF5	"..\\bin\\dfphsg5.ttf"
#define TTF7	"..\\bin\\dfphsg7.ttf"
#endif

///////////////////////////////////////////////////////////////////////////////
// String Handling Options
///////////////////////////////////////////////////////////////////////////////
#define PRISM_STRING_LIBRARY_REPLACEMENTS
#define PRISM_LTOA_REPLACEMENT


///////////////////////////////////////////////////////////////////////////////
// Miscellaneous Settings
///////////////////////////////////////////////////////////////////////////////
#define PRISM_MAX_LINE_OFFSETS          100
#define PRISM_MAX_PTR_CAPTURE_NESTING     8
#define PM_PAINT_STACK_DEPTH             20
#define PRISM_SCREEN_STACK_DEPTH         20
#define PRISM_USER_PTR_TYPES              5
#define PRISM_BTN_INITIAL_REPEAT         20
#define PRISM_BTN_REPEAT_RATE             5
#define PRISM_DEF_ML_MARKER            0x7c

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PRISM_PORTAL_BLOCK_SIZE          32



///////////////////////////////////////////////////////////////////////////////
// User Defined Settings
///////////////////////////////////////////////////////////////////////////////
#define PRISM_FLICK_GESTURE_SUPPORT
#define PRISM_FREETYPE_SUPPORT
#if defined(PRISM_FLICK_GESTURE_SUPPORT) && defined(PRISM_FOR_X11)
# if !defined(__i386__)
//#  define PRISM_OVG_ANIMATION
#  define PRISM_OGL_ANIMATION
# if defined PRISM_OVG_ANIMATION && defined PRISM_OGL_ANIMATION
#  error "PRISM_OVG_ANIMATION and PRISM_OGL_ANIMATION are mutually exclusive"
# endif
# endif
#endif

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
