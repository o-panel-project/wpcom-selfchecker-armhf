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
//  prism_includes.h - Prism master header file, includes all API header files
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_INCLUDES_
#define _PRISM_INCLUDES_

#include "prism_color_formats.h"

#if defined(PRISM_INSIGHT)
#include "prism_settings.ih"
#else
#include "prism_settings.h"      // Pull in user configuration settings
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PRISM_TRUE_ALIAS

#if defined(PRISM_BINARY_RESOURCES)

#if !defined(PRISM_FILEIO_ABSTRACTION)
#error Settings require Prism File I/O be enabled
#endif 

#if !defined(PRISM_UNZIP)
#error Settings require that PRISM_UNZIP be enabled
#endif 

#endif 

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if defined(PRISM_MULT_SW_CANVASES) || defined(PRISM_MULT_HW_CANVASES)
#define PRISM_MULTIPLE_CANVASES
#endif

#if defined(PRISM_FOR_SMX) && defined(WIN32)  
#define PRISM_FOR_WIN32
#endif

///////////////////////////////////////////////////////////////////////////////
#ifndef PRISM_BORDER_WIDTH
#define PRISM_BORDER_WIDTH 5
#endif

#ifndef PRISM_THICK_FRAME
#define PRISM_THICK_FRAME PRISM_FRAME_WIDTH
#endif

#define PRISM_MAX_ALPHA 255
#define PM_MAX_CACHED_GRADIENTS 32

#ifndef PRISM_SYS_BTN_SIZE
#define PRISM_SYS_BTN_SIZE PRISM_SCROLL_WIDTH  
#endif

#if defined(PRISM_TITLE_BAR)
#ifndef TITLE_GRADIENT_SIZE
#define TITLE_GRADIENT_SIZE 32
#endif
#endif

#define PRISM_UNZIP_CHUNK_SIZE 8192

#ifndef PRISM_TOOLBAR_PAD_HEIGHT
#define PRISM_TOOLBAR_PAD_HEIGHT 2
#endif

#ifndef PRISM_BMP_TXT_PADDING
#define PRISM_BMP_TXT_PADDING 2
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if defined(PRISM_MOUSE_SUPPORT)

#define PM_MOUSE_CANVAS_SIZE 32    
#if defined(PRISM_HARDWARE_CURSOR)

#define PM_MOUSE_LAYER        

#else

#define PM_SW_MOUSE

#if defined(PRISM_MULT_SW_CANVASES)
#define PM_MOUSE_LAYER            
#endif

#endif    
#endif  

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#if (PRISM_COLOR_FORMAT < PM_CF_2BPP_GRAY)
#define PRISM_INCLUDE_MONO_COLOR_TABLE
#elif (PRISM_COLOR_FORMAT < PM_CF_4BPP_GRAY)
#define PRISM_INCLUDE_4_GRAY_COLOR_TABLE
#elif (PRISM_COLOR_FORMAT < PM_CF_4BPP_VGA)
#define PRISM_INCLUDE_16_GRAY_COLOR_TABLE
#else
#define PRISM_INCLUDE_DEFAULT_COLOR_TABLE
#endif

// figure out if we are using our standalone Pm_Event_Manager
// implementation or are we using an RTOS specific version?

#if defined(PRISM_STANDALONE)

#define PRISM_STANDALONE_EVENT_MANAGER

#elif defined(PRISM_FOR_WIN32) && !defined(PRISM_THREADS_SUPPORT)

#define PRISM_STANDALONE_EVENT_MANAGER

#elif defined(PRISM_FOR_WINCE)

#define PRISM_STANDALONE_EVENT_MANAGER

#elif defined(PRISM_FOR_X11) && !defined(PRISM_THREADS_SUPPORT)

#define PRISM_STANDALONE_EVENT_MANAGER

#elif defined(PRISM_FOR_POSIX) && !defined(PRISM_THREADS_SUPPORT)

#define PRISM_STANDALONE_EVENT_MANAGER

#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_FOR_RTXC)
#undef NULL
#define NULL 0
#endif


///////////////////////////////////////////////////////////////////////////////

#if !defined(PRISM_FOR_SMX)  
#if defined(__BORLANDC__) && !defined(PRISM_LTOA_REPLACEMENT)
#define _ltoa(a, b, c)  ltoa(a, b, c)
#else
#define ltoa(a, b, c)   _ltoa(a, b, c)
#endif   
#endif  

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_FOR_SMX)  

#if !defined(SMX_H_)
#include "smx.h"
#endif

#if defined(PRISM_FOR_WIN32) 

#include "windows.h"

#undef  PRISM_DRAW_ENGINE
#define PRISM_DRAW_ENGINE "prism_pe_win_32bpp.h"  
#define PRISM_FULL_GRAPHICS
#ifdef  PRISM_FP_GRAPHICS
#undef  PRISM_FP_GRAPHICS
#endif

#define PRISM_GRAPHIC_READER
#define PRISM_THREADS_SUPPORT

#endif
                                                                
#elif defined(PRISM_FOR_THREADX)        

#ifndef TX_API

#include "tx_api.h"

#endif

#elif defined(PRISM_FOR_ETS)

#ifndef _WINDOWS_
#include "windows.h"
#endif

#elif defined(PRISM_FOR_RTXC)

extern "C" {
#include "rtxcapi.h"
#include "kproject.h"
#include "kcounter.h"
}

#elif defined(PRISM_FOR_KEIL_ARTX)

extern "C" {        
#include "RTL.h"
}

#elif defined(PRISM_FOR_NUCLEUS)        

#include "plus/nucleus.h" 

#if defined(PRISM_FOR_WIN32)    
#ifndef _WINDOWS_
#include "windows.h"
#endif
#endif

#elif defined(PRISM_FOR_FREE_RTOS)

#ifdef PRISM_FOR_WIN32
#undef PRISM_STANDALONE_EVENT_MANAGER
#endif

extern  "C" {
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
}

#elif defined(PRISM_FOR_TI_BIOS)

#include <std.h>

#include <clk.h>
#include <lck.h>
#include <sem.h>
#include <sys.h>
#include <tsk.h>
#include <trc.h>

#elif defined(PRISM_FOR_ITRON)

#include "itron.h"

#elif defined(PRISM_FOR_WIN32)            

#ifndef _WINDOWS_
#include "windows.h"
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if defined(_DEBUG)
#define REPORT_MEMORY_LEAKS 
#endif

#if defined REPORT_MEMORY_LEAKS

#include "stdlib.h"
#include "crtdbg.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


#elif defined(PRISM_FOR_WINCE)

#ifndef _WINDOWS_
#include "windows.h"
#endif

#elif defined(PRISM_FOR_X11)              

#if defined(PRISM_THREADS_SUPPORT)
#include <pthread.h>
#endif  

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#elif defined(PRISM_FOR_LINUX)         

#if defined(PRISM_THREADS_SUPPORT)
#include <pthread.h>
#endif  

#elif defined(PRISM_FOR_UNISON)
         
extern "C" {
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
}

#elif defined(PRISM_FOR_ECOS)           

#include <cyg/kernel/kapi.h>

#elif defined(PRISM_FOR_CMX) 

extern "C" {
#include "cxfuncs.h"
#include "cxextern.h"
};

#elif defined(PRISM_FOR_VDK)            

// these diagnostic suppressions are produced by VisualDSP- we
// have no idea what they do!

#pragma diag(push)
#pragma diag(suppress: 177,401,451,826,831,1462)

#include "VDK.h"

#pragma diag(pop)

#elif defined(PRISM_FOR_INTEGRITY) 

#include <INTEGRITY.h>

#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define NUM_PRISM_FREE_EVENTS   40    

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_FOR_SMX)               

#include "rtos/prism_for_smx.h"       
       
#elif defined(PRISM_FOR_THREADX)            

#if defined(PRISM_FOR_WIN32)
#include "rtos/prism_for_threadx_sim.h"   
#else
#include "rtos/prism_for_threadx.h" 
#endif

#elif defined(PRISM_FOR_CMX)
#include "rtos/prism_for_cmx.h"

#elif defined(PRISM_FOR_RTXC)

#include "rtos/prism_for_rtxc.h"

#elif defined(PRISM_FOR_ECOS)
#include "rtos/prism_for_ecos.h"

#elif defined(PRISM_FOR_KEIL_ARTX)

#include "rtos/prism_for_keil_rtx.h"

#elif defined(PRISM_FOR_LINUX)    
#include "rtos/prism_for_linux.h"

#elif defined(PRISM_FOR_UNISON)    
#include "rtos/prism_for_unison.h"

#elif defined(PRISM_FOR_VDK)  
#include "rtos/prism_for_vdk.h"

#elif defined(PRISM_FOR_FREE_RTOS)
#include "rtos/prism_for_free_rtos.h"

#elif defined(PRISM_FOR_TI_BIOS)
#include "rtos/prism_for_ti_bios.h"

#elif defined(PRISM_FOR_ETS)
#include "rtos/prism_for_ets.h"

#elif defined(PRISM_FOR_ITRON)

#include "rtos/prism_for_itron.h"

#elif defined(PRISM_FOR_X11)       

#include "rtos/prism_for_x11.h"

#elif defined(PRISM_FOR_WIN32) || defined(PRISM_FOR_WINCE) 

#include "rtos\prism_for_win32.h"

#else                             

#define PRISM_ENTER_CRITICAL        
#define PRISM_EXIT_CRITICAL
#define PRISM_LOCK_EVENTS
#define PRISM_UNLOCK_EVENTS
#define PRISM_LOCK_TIMERS 
#define PRISM_UNLOCK_TIMERS
#define ENQUEUE_TASK_EVENT(a, b)
#define PRISM_DELAY(a)

#if defined(_MSC_VER) && defined(_DEBUG)
 #include "crtdbg.h"
 #define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif

#endif

///////////////////////////////////////////////////////////////////////////////
#if !defined(PRISM_RUNTIME_GRAPHIC_READERS)

#if defined(PRISM_RUNTIME_BMP_READER) || defined(PRISM_RUNTIME_GIF_READER)
#define PRISM_RUNTIME_GRAPHIC_READERS
#elif defined(PRISM_RUNTIME_PNG_READER) || defined(PRISM_RUNTIME_JPG_READER)
#define PRISM_RUNTIME_GRAPHIC_READERS
#endif
#endif

#include "prism_data_types.h"

#if defined(PRISM_FREETYPE_SUPPORT)
#include "ft2build.h"
#include FT_FREETYPE_H
#endif

#if defined(PRISM_GRAPHICS_HEAP)
#include "prism_heap_mgr.h"
#endif

#include "prism_colors.h"
#include "prism_status.h"
#include "prism_point.h"
#include "prism_bitmap.h"
#include "prism_fp_math.h"
#include "prism_timer_mgr.h"
#include "prism_font_type.h"
#include "prism_brush.h"
#include "prism_string_lib.h"
#include "prism_key_defs.h"
#include "prism_region.h"
#include "prism_error.h"
#include "prism_snapshot.h"

#if !defined(PRISM_MICRO)
#include "prism_paint_gradient.h"
#endif

#if defined(PRISM_FILEIO_ABSTRACTION)
#include "prism_file_wrapper.h"
#endif

#if defined(PRISM_INSIGHT)
#if defined(PRISM_FOR_WIN32)
#include "..\..\insight\include\insight_res_mgr.h"
#else
#include "insight_res_mgr.h"
#endif
#else
#include "prism_resource_mgr.h"
#endif

#include "prism_event_mgr.h"

#include "prism_paint_engine.h"
#include PRISM_PAINT_ENGINE            

#include "prism_input_device.h"
#include "prism_framework.h"
#include "prism_widget.h"
#include "prism_widget_types.h"
#include "prism_text_widget.h"
#include "prism_math.h"
#include "prism_icon.h"
#include "prism_group.h"
#include "prism_toolbar.h"
#include "prism_prompt.h"

#include "prism_button.h"
#include "prism_icon_btn.h"
#include "prism_text_btn.h"
#include "prism_bmp_btn.h"
#include "prism_shadow_btn.h"
#include "prism_exclusive_btn.h"
#include "prism_toggle_btn.h"
#include "prism_txt_icon_btn.h"
#include "prism_bmp_text_btn.h"
#include "prism_spin_btn.h"
#include "prism_help_btn.h"

#if !defined(PRISM_MICRO) && defined(PRISM_BITMAP_ROTATE)
#include "prism_bmp_rotate.h"
#endif

#include "prism_scroll.h"
#include "prism_ml_text_btn.h"
#include "prism_slider.h"
#include "prism_progress_bar.h"

#include "prism_status_bar.h"
#include "prism_vprompt.h"
#include "prism_anim_prompt.h"
#include "prism_bitmap_prompt.h"
#include "prism_sprite.h"

#include "prism_panel.h"
#include "prism_menu.h"
#include "prism_title_bar.h"
#include "prism_dec_panel.h"
#include "prism_tab_panel.h"
#include "prism_tree_view.h"

#if defined(PRISM_INSIGHT)
#include "prism_data_table.h"
#endif

#include "prism_list.h"
#include "prism_virtual_list.h"
#include "prism_drop_box.h"
#include "prism_tab_panel.h"

#include "prism_screen_mgr.h"
#include "prism_text_input.h"
#include "prism_password.h"

#if defined(PRISM_ZIP) || defined(PRISM_UNZIP)
#include "prism_zip.h"
#endif

#if defined(PRISM_BINARY_RESOURCES)
#include "prism_res_file.h"
#endif

#if defined(PRISM_CHART_SUPPORT)
#include "prism_chart_base.h"
#include "prism_line_chart.h"
#endif

#if defined(PRISM_RUNTIME_GRAPHIC_READERS)

#include "prism_graphic_reader.h"

#if defined(PRISM_RUNTIME_JPG_READER)
#include "prism_jpg_reader.h"
#endif

#if defined(PRISM_RUNTIME_PNG_READER)
#include "prism_png_reader.h"
#endif

#if defined(PRISM_RUNTIME_GIF_READER)
#include "prism_gif_reader.h"
#endif

#if defined(PRISM_RUNTIME_BMP_READER)
#include "prism_bmp_reader.h"
#endif

#include "prism_image_widget.h"

#endif

#include "prism_ml_prompt.h"
#include "prism_ml_text_input.h"
#include "prism_popup.h"
#include "prism_ml_popup.h"
#include "prism_progress_panel.h"

#if defined(PRISM_KEYPAD_SUPPORT)
#include "drivers/keypad/prism_keypad_device.h"
#ifdef PRISM_KEYPAD_DEVICE
 #include PRISM_KEYPAD_DEVICE
#endif
#endif 

#if defined(PRISM_MOUSE_SUPPORT)
#include "drivers/mouse/prism_mouse_device.h"
#ifdef PRISM_MOUSE_INCLUDE
 #include PRISM_MOUSE_INCLUDE
#endif
#endif

#if defined(PRISM_TOUCH_SUPPORT)
#include "drivers/touch/prism_touch_device.h"

#ifdef PRISM_TOUCH_INCLUDE
#include PRISM_TOUCH_INCLUDE
#endif
#endif

#if defined(PRISM_FOR_ETS)
Pm_Paint_Engine *Ets_Paint_Engine(void);
#endif

///////////////////////////////////////////////////////////////////////////////

void PrismInitialize(const Pm_Region &Size);
void PrismTerminate(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_FOR_WIN32)

#define MAX_PRISM_COMMAND_LINE 512

typedef struct {
    pm_int_t xScreenSize;
    pm_int_t yScreenSize;
    pm_int_t xWinClientSize;
    pm_int_t yWinClientSize;
    pm_int_t xOffset;
    pm_int_t yOffset;
    HBITMAP ShellImage;
    char   CmdString[MAX_PRISM_COMMAND_LINE];
} CommandLineParams;

#elif defined(PRISM_FOR_X11)

#define MAX_PRISM_COMMAND_LINE 512
typedef struct {
    pm_int_t xScreenSize;
    pm_int_t yScreenSize;
    pm_int_t xWinClientSize;
    pm_int_t yWinClientSize;
    pm_int_t xOffset;
    pm_int_t yOffset;
    XImage *ShellImage;
    char   CmdString[MAX_PRISM_COMMAND_LINE];
} CommandLineParams;

#elif defined(PRISM_FOR_LINUX)

typedef struct {
    char   CmdString[MAX_PRISM_C0MMAND_LINE];
} CommandLineParams;

extern CommandLineParams CommandLine;

#endif

END_NAMESPACE_PRISM

#endif  



