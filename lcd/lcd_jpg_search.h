/***************************************************************************/
/*                                                                         */
/*              I-Panel LINUX SELFCHECK TEST PROGRAM                       */
/*            Linux Selfcheck Test Program for j-panel                     */
/*                                                                         */
/***************************************************************************/
/***************************************************************************/
/* SPECIFICATIONS                                                          */
/*   FILE       = lcd_jpg_search.h                                         */
/*   DEVELOPERS = VACS                                                     */
/*   ATTRIBUTE                                                             */
/*   HISTORY                                                               */
/*     ID    Date      Name           Description                          */
/*     ----  --------  -------------  -----------------------------------  */
/*                                                                         */
/***************************************************************************/
#ifndef __LCD_H__

 #define __LCD_H__

#if defined __cplusplus
	extern "C" {
#endif

/*==============================================================
 * header include
 *=============================================================*/

/*==============================================================
 * constant value
 *=============================================================*/
/* Window Size */
#define	WINDOW_SIZE_X	1024		/* pixel */
#define	WINDOW_SIZE_Y	600		/* pixel */

/* JPG File MAX */
#define	IMAGE_MAX		10

/* JPG File Name */
#define	IMAGE_NAME_MAX	20+1

#define	IMAGE01			"IMAGE01.JPG"
#define	IMAGE02			"IMAGE02.JPG"
#define	IMAGE03			"IMAGE03.JPG"
#define	IMAGE04			"IMAGE04.JPG"
#define	IMAGE05			"IMAGE05.JPG"
#define	IMAGE06			"IMAGE06.JPG"
#define	IMAGE07			"IMAGE07.JPG"
#define	IMAGE08			"IMAGE08.JPG"
#define	IMAGE09			"IMAGE09.JPG"
#define	IMAGE10			"IMAGE10.JPG"

#define	IMAGE_INIT		-1
#define	IMAGE_NON		0
#define	IMAGE_ON		IMAGE_NON + 1

/*==============================================================
 * Structure
 *=============================================================*/
struct	IMAGE_FNAME_t{
	int		flag;													/* 0:non	*/
	char	image_name[IMAGE_NAME_MAX];
};

/*==============================================================
 * Prototype
 *=============================================================*/
/*****************************************
  NAME		: lcd_jpg_check
  I/F		: int	lcd_jpg_check(void)
  FUNCTION	: LCD Display JPG file check
  ARGUMENTS	: void
  RETURN	: int	status
*****************************************/
int	lcd_jpg_check(char *jpeg_path);

/*****************************************
  NAME		: JPG_search
  I/F		: long	JPG_search(int	max)
  FUNCTION	: LCD Display JPG file search
  ARGUMENTS	: int	max
  RETURN	: long	first_val
*****************************************/
long	JPG_search(int	max, char *jpeg_path);

extern int show_jpeg_with_imlib(Display *disp, Window win, char *imgfile);

#if defined __cplusplus
	}
#endif

#endif
