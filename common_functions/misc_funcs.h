/***************************************************************************/
/*                                                                         */
/*                   LINUX SELFCHECK TEST PROGRAM                          */
/*            Linux Selfcheck Test Program for j-panel                     */
/*                                                                         */
/***************************************************************************/
/***************************************************************************/
/* SPECIFICATIONS                                                          */
/*   FILE       = misc_funcs.h                                             */
/*   FUNCTION   = Common functions used throughout the test program        */
/*                                                                         */
/*   DEVELOPERS = VACS                                                     */
/*                                                                         */
/*   ATTRIBUTE                                                             */
/*   HISTORY                                                               */
/*     ID    Date      Name           Description                          */
/*     ----  --------  -------------  -----------------------------------  */
/*                                                                         */
/***************************************************************************/

#ifndef __MFUNC__
#define __MFUNC__

/*****************************************
  NAME		: set_main_window_btn
  I/F		: void set_main_window_btn(WSCbool state);
  FUNCTION	: Sets the sensitivity property of the test buttons in the main window
  ARGUMENTS	: WSCbool state - boolean state of the buttons
  			: True - allow button activate
  			: False - not allow button activate (grayed out)
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_main_window_btn(WSCbool state);
#else
#define set_main_window_btn(x)	{printf("set_main_window_btn()\n");}
#endif

/*****************************************
  NAME		: di_sdram_button
  I/F		: void di_sdram_button(void);
  FUNCTION	: 
  ARGUMENTS	: none
  RETURN	: none
*****************************************/
void	di_sdram_button(void);

/*****************************************
  NAME		: setMovWND_stop
  I/F		: void setMovWND_stop();
  FUNCTION	: Sets the sensitivity property of the movie test buttons after movie animation
  ARGUMENTS	: none
  RETURN	: none
*****************************************/
void setMovWND_stop();

/*****************************************
  NAME		: set_color_orange
  I/F		: void set_color_orange(WSCbase* object)
  FUNCTION	: set background color of widestudio object to orange
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_orange(WSCbase* object);
#else
#define set_color_orange(x) {printf("set_color_orange\n");}
#endif

/*****************************************
  NAME		: set_color_grey
  I/F		: void set_color_grey(WSCbase* object)
  FUNCTION	: set background color of widestudio object to grey
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_grey(WSCbase* object);
#else
#define set_color_gray(x) {printf("set_color_gray\n");}
#endif

/*****************************************
  NAME		: set_color_red
  I/F		: void set_color_red(WSCbase* object)
  FUNCTION	: set background color of widestudio object to red
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_red(WSCbase* object);
#else
#define set_color_red(x) {printf("set_color_red\n");}
#endif

/*****************************************
  NAME		: set_color_blue
  I/F		: void set_color_blue(WSCbase* object)
  FUNCTION	: set background color of widestudio object to blue
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_blue(WSCbase* object);
#else
#define set_color_blue(x) {printf("set_color_blue\n");}
#endif

/*****************************************
  NAME		: set_color_yellow
  I/F		: void set_color_yellow(WSCbase* object)
  FUNCTION	: set background color of widestudio object to yellow
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_yellow(WSCbase* object);
#else
#define set_color_yellow(x) {printf("set_color_yellow\n");}
#endif

/*****************************************
  NAME		: set_color_green
  I/F		: void set_color_green(WSCbase* object)
  FUNCTION	: set background color of widestudio object to green
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_green(WSCbase* object);
#else
#define set_color_green(x) {printf("set_color_green\n");}
#endif

/*****************************************
  NAME		: set_color_ygreen
  I/F		: void set_color_ygreen(WSCbase* object)
  FUNCTION	: set background color of widestudio object to yellowish green
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_ygreen(WSCbase* object);
#else
#define set_color_ygreen(x) {printf("set_color_ygreen\n");}
#endif

/*****************************************
  NAME		: set_color_black
  I/F		: void set_color_black(WSCbase* object)
  FUNCTION	: set background color of widestudio object to black
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_black(WSCbase* object);
#else
#define set_color_black(x) {printf("set_color_black\n");}
#endif

/*****************************************
  NAME		: set_color_white
  I/F		: void set_color_white(WSCbase* object)
  FUNCTION	: set background color of widestudio object to white
  ARGUMENTS	: WSCbase* object - pointer to a widestudio object
  RETURN	: none
*****************************************/
#ifdef HAVE_WS
void set_color_white(WSCbase* object);
#else
#define set_color_white(x) {printf("set_color_white\n");}
#endif

/*****************************************
  NAME		: 
  I/F		: 
  FUNCTION	: 
  ARGUMENTS	: 
  RETURN	: 
*****************************************/
void set_sublcd_start();

/*****************************************
  NAME		: 
  I/F		: 
  FUNCTION	: 
  ARGUMENTS	: 
  RETURN	: 
*****************************************/
void set_sublcd_stop();

#endif
