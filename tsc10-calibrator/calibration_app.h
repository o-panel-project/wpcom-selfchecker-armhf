/* 
 * calibration_app.h
 *
 */

#ifndef __CALIBRATION_APP_H__
#define __CALIBRATION_APP_H__

/*==============================================================
 * header include
 *=============================================================*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Intrinsic.h>
#include <X11/cursorfont.h>
#include <X11/Xmd.h>
#include <X11/Xmu/WinUtil.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/xtestext1.h>
#include <X11/Xlocale.h>

extern int errno;

/*==============================================================
 * constant value 
 *=============================================================*/
#ifdef NOT_USE_THESE_VALUES
/* ウィンドウサイズ */
#define WINDOW_SIZE_X	1024		/* pixel */
#define WINDOW_SIZE_Y	600		/* pixel */

#define CROSS_TARGET_DISTANCE	40
#define SUB_LCD_CROSS_TARGET_DISTANCE	50	//add 2006/11/15

/* キャリブレーションテストのクロスターゲット表示位置 */
#define CALIBRATION_P00_X	CROSS_TARGET_DISTANCE
#define CALIBRATION_P00_Y	CROSS_TARGET_DISTANCE

#define CALIBRATION_P01_X	(WINDOW_SIZE_X / 2)
#define CALIBRATION_P01_Y	CROSS_TARGET_DISTANCE

#define CALIBRATION_P02_X	(WINDOW_SIZE_X-CROSS_TARGET_DISTANCE)
#define CALIBRATION_P02_Y	CROSS_TARGET_DISTANCE

#define CALIBRATION_P10_X	CROSS_TARGET_DISTANCE
#define CALIBRATION_P10_Y	(WINDOW_SIZE_Y / 2)

#define CALIBRATION_PCT_X	(WINDOW_SIZE_X / 2)
#define CALIBRATION_PCT_Y	(WINDOW_SIZE_Y / 2)

#define CALIBRATION_P12_X	(WINDOW_SIZE_X-CROSS_TARGET_DISTANCE)
#define CALIBRATION_P12_Y	(WINDOW_SIZE_Y / 2)

#define CALIBRATION_P20_X	CROSS_TARGET_DISTANCE
#define CALIBRATION_P20_Y	(WINDOW_SIZE_Y-CROSS_TARGET_DISTANCE)

#define CALIBRATION_P21_X	(WINDOW_SIZE_X / 2)
#define CALIBRATION_P21_Y	(WINDOW_SIZE_Y-CROSS_TARGET_DISTANCE)

#define CALIBRATION_P22_X	(WINDOW_SIZE_X-CROSS_TARGET_DISTANCE)
#define CALIBRATION_P22_Y	(WINDOW_SIZE_Y-CROSS_TARGET_DISTANCE)

/* SUB-LCD calibration points -- add 2006/11/15 */
#define SUB_LCD_CALIBRATION_P00_X	SUB_LCD_CROSS_TARGET_DISTANCE
#define SUB_LCD_CALIBRATION_P00_Y	SUB_LCD_CROSS_TARGET_DISTANCE

#define SUB_LCD_CALIBRATION_P01_X	(WINDOW_SIZE_X / 2)
#define SUB_LCD_CALIBRATION_P01_Y	SUB_LCD_CROSS_TARGET_DISTANCE

#define SUB_LCD_CALIBRATION_P02_X	(WINDOW_SIZE_X-SUB_LCD_CROSS_TARGET_DISTANCE)
#define SUB_LCD_CALIBRATION_P02_Y	SUB_LCD_CROSS_TARGET_DISTANCE

#define SUB_LCD_CALIBRATION_P10_X	SUB_LCD_CROSS_TARGET_DISTANCE
#define SUB_LCD_CALIBRATION_P10_Y	(WINDOW_SIZE_Y / 2)

#define SUB_LCD_CALIBRATION_PCT_X	(WINDOW_SIZE_X / 2)
#define SUB_LCD_CALIBRATION_PCT_Y	(WINDOW_SIZE_Y / 2)

#define SUB_LCD_CALIBRATION_P12_X	(WINDOW_SIZE_X-SUB_LCD_CROSS_TARGET_DISTANCE)
#define SUB_LCD_CALIBRATION_P12_Y	(WINDOW_SIZE_Y / 2)

#define SUB_LCD_CALIBRATION_P20_X	SUB_LCD_CROSS_TARGET_DISTANCE
#define SUB_LCD_CALIBRATION_P20_Y	(WINDOW_SIZE_Y-SUB_LCD_CROSS_TARGET_DISTANCE)

#define SUB_LCD_CALIBRATION_P21_X	(WINDOW_SIZE_X / 2)
#define SUB_LCD_CALIBRATION_P21_Y	(WINDOW_SIZE_Y-SUB_LCD_CROSS_TARGET_DISTANCE)

#define SUB_LCD_CALIBRATION_P22_X	(WINDOW_SIZE_X-SUB_LCD_CROSS_TARGET_DISTANCE)
#define SUB_LCD_CALIBRATION_P22_Y	(WINDOW_SIZE_Y-SUB_LCD_CROSS_TARGET_DISTANCE)

/* クロスターゲットの表示サイズ */
#define CROSS_TARGET_SIZE	35

/*==============================================================
 * Structure 
 *=============================================================*/

struct calibration_point_t {
	int x; 
	int y;
};
#endif

#endif /* __CALIBRATION_APP_H__ */

