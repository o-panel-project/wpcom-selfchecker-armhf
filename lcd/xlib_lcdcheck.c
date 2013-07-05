/* 
 * calibration_app.c 
 *
 * written by Omoikane Inc.
 */

#include	<getopt.h>
#include	<fcntl.h>		/* 20110822VACS */
#include	<sys/ioctl.h>	/* 20110822VACS */
#include	<sys/stat.h>	/* 20110822VACS */
#include	<sys/types.h>	/* 20110822VACS */
#include	<unistd.h>		/* 20110822VACS */
#include	<time.h>		/* 20120403VACS */
#include	<signal.h>		/* 20120403VACS */

#include	"../tsc10-calibrator/calibration_app.h"
#include	"../PLD_access/PLD_IOacess.h"
#include	"lcd_jpg_search.h"
#include	"common.h"	/* 20110822VACS */
#include	"wpcio.h"	/* 20110822VACS	*/

void	lcdDisplay_Pattern(int pattern);
void	play_dots(void);
void	play_line_pixel_gap(int gap, int thickness);
void	play_background_color(char *backgndColor);
void	play_gradient(void);
void	play_dots2(void);
void	play_dots3(void);
/*	background: white, foreground: bkack	*/
void	set_back_fore_color(void);
/*	background: black, foreground: white	*/
void	set_back_fore_color2(void);
/*	transferred to xlib_lcdcontrol.c - 20061204	*/
#if	0
void	play_lcd_switch(unsigned long sw);
void	play_backlight_switch(unsigned long sw);
void	play_lvds_control(unsigned long sw);
#endif

int		display_click(struct itimerspec *timer);
void	timer_signal(int);

#ifdef DEBUG
#define PRINTMSG(msg...) do {printf(msg);} while (0)
#else
#define PRINTMSG(msg...) do {} while (0)
#endif
/** Log entry point for debugging */
#define ENTER()	\
do {			\
	PRINTMSG("Enter: %s\n", __FUNCTION__);	\
} while (0)

/** Log exit point for debugging */
#define LEAVE()	\
do {			\
	PRINTMSG("Leave: %s\n", __FUNCTION__);	\
} while (0)

#ifdef DIPSW_DUMMY
int g_pattern = 0;
#endif

/*=================================================================== 
 * Global Variable 
 *==================================================================*/
/*	Window parts	*/
static	Window	lcdCheckWindow;			/*	ルートウィンドウ		*/

/*	Window info	*/
static	Display	*lcdCheckDisplay;
static	GC		lcdCheckgc, clear_gc;	/*	描画用GC、クリア用GC	*/

static	int		fd_wpcio=-1;			/*	20110822VACS			*/
char	*grad_color[]={
			"rgb:00/00/00","rgb:0a/0a/0a","rgb:14/14/14","rgb:1e/1e/1e","rgb:28/28/28",
			"rgb:32/32/32","rgb:3c/3c/3c","rgb:46/46/46","rgb:50/50/50","rgb:5a/5a/5a",
			"rgb:64/64/64","rgb:6e/6e/6e","rgb:78/78/78","rgb:82/82/82","rgb:8c/8c/8c",
			"rgb:96/96/96","rgb:a0/a0/a0","rgb:aa/aa/aa","rgb:b4/b4/b4","rgb:be/be/be",
			"rgb:c8/c8/c8","rgb:d2/d2/d2","rgb:dc/dc/dc","rgb:e6/e6/e6","rgb:f0/f0/f0"
};

int click_flag=0;						/*	20120403VACS			*/
timer_t tid;							/*	20120403VACS			*/

/*=================================================================== 
 * Prototype 
 *==================================================================*/

/*=================================================================== 
 * Functions
 *==================================================================*/
/*-------------------------------------------------------------------
 * 名前で色の値を取得
 * "yellow", "red", "black", "white"など
 *------------------------------------------------------------------*/
static unsigned long 
lcdCheckColor(Display *display, char *color)
{
	ENTER();
	Colormap cmap;
	XColor c0, c1;
	cmap = DefaultColormap(display, 0);

	XAllocNamedColor(display, cmap, color, &c1, &c0);
	LEAVE();
	return (c1.pixel);
}

/*-------------------------------------------------------------------
 * 資源開放処理 
 *------------------------------------------------------------------*/
static void
free_resources(void)
{
	ENTER();
	XFreeGC(lcdCheckDisplay, lcdCheckgc);
	XFreeGC(lcdCheckDisplay, clear_gc);
	XDestroyWindow(lcdCheckDisplay, lcdCheckWindow);
	XCloseDisplay(lcdCheckDisplay);
	LEAVE();
}

/*-------------------------------------------------------------------
 * 初期化処理
 *------------------------------------------------------------------*/
static void 
init_display(void)
{
	ENTER();
	/* 変数定義: オート変数にするとフォルトする */
	static XWindowAttributes attribs;
	static XGCValues gcvals;
	static XSizeHints lcdCheckHint;
	static unsigned long lcdCheckForeground, lcdCheckBackground;
	static XSetWindowAttributes lcdCheckAttribs;	/* for window creation */
	static int lcdCheckScreen;
	static int screenwidth, screenheight;

	if (!getenv("DISPLAY")) {
		fprintf(stderr, "DISPLAY not set.\n");
		LEAVE();
		exit(1);
	}
	if (!(lcdCheckDisplay = XOpenDisplay(getenv("DISPLAY")))) {
		fprintf(stderr, "Unable to open display.\n");
		LEAVE();
		exit(1);
	}
	XFlush(lcdCheckDisplay);

	lcdCheckBackground = lcdCheckColor(lcdCheckDisplay, "white");
	lcdCheckForeground = lcdCheckColor(lcdCheckDisplay, "black");
	lcdCheckScreen = DefaultScreen(lcdCheckDisplay);
       	screenwidth = DisplayWidth(lcdCheckDisplay, lcdCheckScreen);
       	screenheight = DisplayHeight(lcdCheckDisplay, lcdCheckScreen);
	lcdCheckHint.width = WINDOW_SIZE_X;
	lcdCheckHint.height = WINDOW_SIZE_Y;
	lcdCheckHint.max_width = WINDOW_SIZE_X;
	lcdCheckHint.max_height = WINDOW_SIZE_Y;
	lcdCheckHint.min_width = WINDOW_SIZE_X;
	lcdCheckHint.min_width = WINDOW_SIZE_Y;
	lcdCheckHint.x = 0;
	lcdCheckHint.y = 0;
	lcdCheckHint.flags = PSize | PMinSize | PMaxSize | PPosition;
	lcdCheckAttribs.event_mask = ButtonPressMask |  
		VisibilityChangeMask | EnterWindowMask | KeyPressMask;

	lcdCheckAttribs.background_pixel = lcdCheckBackground;
	lcdCheckAttribs.override_redirect = True;

	lcdCheckWindow = XCreateWindow(lcdCheckDisplay, DefaultRootWindow(lcdCheckDisplay),
		lcdCheckHint.x,lcdCheckHint.y, lcdCheckHint.width, lcdCheckHint.height, 0,
		attribs.depth, InputOutput, attribs.visual,
		CWEventMask | CWBackPixel | CWOverrideRedirect, &lcdCheckAttribs);

	lcdCheckAttribs.event_mask = ButtonPressMask | ButtonReleaseMask;

	/* 画面描画用GC */ 
	gcvals.foreground = lcdCheckForeground;
	gcvals.background = lcdCheckBackground;
	lcdCheckgc = XCreateGC(lcdCheckDisplay, lcdCheckWindow, GCForeground | GCBackground,
		&gcvals);
	/* 画面クリア用GC */ 
	gcvals.foreground = lcdCheckBackground;
	gcvals.background = lcdCheckBackground;
	clear_gc = XCreateGC(lcdCheckDisplay, lcdCheckWindow, GCForeground | GCBackground,
		&gcvals);

	/* 親ウィンドウを表示 */
	XMapWindow(lcdCheckDisplay, lcdCheckWindow);
	LEAVE();
}

#if 0
/*-------------------------------------------------------------------
 * キャリブレーションの情報を出力する。
 *------------------------------------------------------------------*/
static void
print_calibration_info(int number, int x, int y)
{
	printf(" %d %d", x, y);
}
#endif

/*-------------------------------------------------------------------
 * 画面をクリアする
 *------------------------------------------------------------------*/
static void
clear_all_screen(void)
{
	ENTER();
	XFillRectangle(lcdCheckDisplay, lcdCheckWindow, clear_gc, 0, 0, 
			WINDOW_SIZE_X, WINDOW_SIZE_Y);
	LEAVE();
}

Window xlib_lcdcheck_do_pattern(Display *disp, Window prev, int n, char *jpeg_path)
{
	char pathbuf[1024];
	Window win=0;
	
	ENTER();
	clear_all_screen();
	
	if(n<=8){
		lcdDisplay_Pattern(n);
	}else{
		if(0&&n==9){
			XDestroyWindow(lcdCheckDisplay, lcdCheckWindow);
		}else if(prev){
			XDestroyWindow(lcdCheckDisplay, prev);
			XSync(lcdCheckDisplay, False);
		}
	
		sprintf(pathbuf, "%s/IMAGE%02d.JPG", jpeg_path, n-8);
		show_jpeg_with_imlib(disp, lcdCheckWindow, pathbuf);
	}
	LEAVE();
	return win;
}

int
xlib_lcdcheck(int use_timer, int usec0, char *jpeg_path)
{
	int usec=usec0, pattern=0;
	
	ENTER();
	// Hiding cursor/pointer codes begin:
    Pixmap bm_no;
    Colormap cmap;
    Cursor no_ptr;
    XColor black, dummy;
    static char bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};
	XEvent ev;
	Window win;
    
    cmap = DefaultColormap(lcdCheckDisplay, DefaultScreen(lcdCheckDisplay));
    XAllocNamedColor(lcdCheckDisplay, cmap, "black", &black, &dummy);
    bm_no = XCreateBitmapFromData(lcdCheckDisplay, lcdCheckWindow, bm_no_data, 8, 8);
    no_ptr = XCreatePixmapCursor(lcdCheckDisplay, bm_no, bm_no, &black, &black, 0, 0);

    XDefineCursor(lcdCheckDisplay, lcdCheckWindow, no_ptr);
    XFreeCursor(lcdCheckDisplay, no_ptr);
    if (bm_no != None) {
       XFreePixmap(lcdCheckDisplay, bm_no);
    }
    XFreeColors(lcdCheckDisplay, cmap, &black.pixel, 1, 0);
	// Hiding cursor/pointer codes end:
	
	/* 最初のテストをスクリーンに表示 */
	win=xlib_lcdcheck_do_pattern(lcdCheckDisplay, 0, pattern, jpeg_path);

	/* Main event loop */
	ev.type=0;
	while(pattern<18){
		if(use_timer){
			if(0<usec){
		        usleep(100000);
				usec-=100000;
				continue;
			}else{
				usec=usec0;
				pattern++;
				win=xlib_lcdcheck_do_pattern(lcdCheckDisplay, win, pattern, jpeg_path);
			}
		}else{
			switch(ev.type){
			case ButtonPress:
				pattern++;
				win=xlib_lcdcheck_do_pattern(lcdCheckDisplay, win, pattern, jpeg_path);
				break;
					
			default:
				break;
			}
			XNextEvent(lcdCheckDisplay, &ev);
		}
	}
	
	if(use_timer){
		while(0<usec){
			usleep(100000);
			usec-=100000;
		}
	}
	
	free_resources();
	LEAVE();
	return 0;
}

/*	20110822VACS	*/
int
wpcio_open_dipsw(int n, char *tag)
{
int	i, fd;

	ENTER();
	for(i=0;i<n;i++){
		fd=open("/dev/wpcio", O_RDWR);
		if(0<=fd)	break;
		usleep(50000);
	}
	if(i==n){
#if	1
		printf("%s : Could not open /dev/wpcio (%d).\n", tag, getpid());
#else
		debug_printf(3, "%s : Could not open /dev/wpcio (%d).\n", tag, getpid());
#endif
	}

	LEAVE();
	return	fd;
}

/*	20110822VACS	*/
static	int
open_dipsw_wpcio(void)
{
	ENTER();
	fd_wpcio=wpcio_open_dipsw(WPCIO_OPEN_RETRY, "open_dipsw_wpcio");
	LEAVE();
	return (fd_wpcio<0) ? 1 : 0;
}

/*	20110822VACS				*/
/*								*/
/*	S4-S1=OFF	: white		(0)	*/
/*	S1=ON		: red		(2)	*/
/*	S2=ON		: green		(4)	*/
/*	S3=ON		: blue		(3)	*/
/*	S4=ON		: black		(1)	*/
/*	other		: white		(0)	*/
static	int
get_dipsw_wpcio(void)
{
#ifdef DIPSW_DUMMY
	return g_pattern % 5;
#else
int	r, d, data;
int	pat=-1;

	ENTER();
	r=ioctl(fd_wpcio, WPC_GET_DIPSW, &d);
	if(r<0){
#if	1
		printf("Cannot get DIP Switch, error code = %d\n", r);
#else
		debug_printf(3, "Cannot get DIP Switch, error code = %d\n", r);
#endif
		LEAVE();
		return	-1;
	}

	data=(((d & 0x0f) ^ 0x0f) & 0x0f);
#if	0
printf("dip-sw    (%02x)(%02x)\n", d, (d & 0x0f));/*debug*/
#endif
#if	0
printf("dip-sw xor(%02x)\n", data);/*debug*/
#endif

	/*	white		*/
	pat=0;
	if((data & 0x0f) == WPC_DIPSW_1){
		/*	red		*/
#if	0
printf("lcd-red\n");/*debug*/
#endif
		pat=2;
	}
	if((data & 0x0f) == WPC_DIPSW_2){
		/*	green	*/
#if	0
printf("lcd-green\n");/*debug*/
#endif
		pat=4;
	}
	if((data & 0x0f) == WPC_DIPSW_3){
		/*	blue	*/
#if	0
printf("lcd-blue\n");/*debug*/
#endif
		pat=3;
	}
	if((data & 0x0f) == WPC_DIPSW_4){
		/*	black	*/
#if	0
printf("lcd-black\n");/*debug*/
#endif
		pat=1;
	}

	LEAVE();
	return pat;
#endif
}

/*	20110822VACS				*/
/*								*/
/*	S4-S1=OFF	: white		(0)	*/
/*	S1=ON		: red		(2)	*/
/*	S2=ON		: green		(4)	*/
/*	S3=ON		: blue		(3)	*/
/*	S4=ON		: black		(1)	*/
/*	other		: white		(0)	*/int
xlib_lcdcheck_Inspect(int use_timer, int usec0, char *jpeg_path)
{
int			usec=usec0, pattern=0;
int			loop=1, old_pattern=-1;
// Hiding cursor/pointer codes begin:
Pixmap		bm_no;
Colormap	cmap;
Cursor		no_ptr;
XColor		black, dummy;
static	char	bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};
XEvent		ev;
Window		win=0;

struct sigaction act;
struct timespec ts,rem;
struct itimerspec timer;

	ENTER();
    ts.tv_sec=0;
    ts.tv_nsec=100000000;	/*100ms*/
    timer.it_value.tv_sec=1;
    timer.it_value.tv_nsec=0;
    timer.it_interval.tv_sec=0;
    timer.it_interval.tv_nsec=0;

    memset(&act, 0, sizeof(act));
    act.sa_handler=timer_signal;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);
    timer_create(CLOCK_REALTIME, NULL, &tid);

    cmap = DefaultColormap(lcdCheckDisplay, DefaultScreen(lcdCheckDisplay));
    XAllocNamedColor(lcdCheckDisplay, cmap, "black", &black, &dummy);
    bm_no = XCreateBitmapFromData(lcdCheckDisplay, lcdCheckWindow, bm_no_data, 8, 8);
    no_ptr = XCreatePixmapCursor(lcdCheckDisplay, bm_no, bm_no, &black, &black, 0, 0);

    XDefineCursor(lcdCheckDisplay, lcdCheckWindow, no_ptr);
    XFreeCursor(lcdCheckDisplay, no_ptr);
    if (bm_no != None) {
       XFreePixmap(lcdCheckDisplay, bm_no);
    }
    XFreeColors(lcdCheckDisplay, cmap, &black.pixel, 1, 0);
	/*	Hiding cursor/pointer codes end:	*/

	/*	wpcio open	*/
	if(!open_dipsw_wpcio()){
		/*	Main event loop	*/
		loop=1;
		old_pattern=-1;
		ev.type=0;
		while(loop){
			if(XCheckTypedEvent(lcdCheckDisplay, ButtonPress, &ev)){
				if(display_click(&timer)==1){
					loop=0;
				}
#ifdef DIPSW_DUMMY
				g_pattern++;
#endif
			}else{
				pattern=get_dipsw_wpcio();
				if(pattern!=-1){
					if(pattern!=old_pattern){
						printf("LCD draw(%d)\n", pattern);
						/*	Drawing	*/
						win=xlib_lcdcheck_do_pattern(lcdCheckDisplay,
										win,
										pattern,
										jpeg_path
										);
						old_pattern=pattern;
					}
				}else	loop=0;
			}

			if(loop==0){
				break;/*	finish	*/
			}
			nanosleep(&ts,&rem);
		}
	}else{
		printf("open_dipsw_wpcio() Error\n");
	}

	/*	wpcio close	*/
	close(fd_wpcio);
	free_resources();
	LEAVE();
	return loop;
}

int main(int argc, char *argv[])
{
	char jpeg_path[1024]="./";	
	int c, usec=0;
	int	flg=0;
	
	while(1){
		c=getopt(argc, argv, "j:I:a:");
		if(c==-1) break;
		
		switch(c){
		case 'j':
			strcpy(jpeg_path, optarg);
			break;
		case 'I':
			usec=atoi(optarg)*1000000;
			break;
		/*	20110822VACS	*/
		case 'a':
			flg=atoi(optarg);
			break;
		}
	}
	
	init_display();
	/*	20110822VACS	*/
	if(!flg){
		printf("[LCD Display] test\n");
		xlib_lcdcheck((0<usec), usec, jpeg_path);
	}
	if(flg){
		printf("[LCD Inspect] test\n");
		xlib_lcdcheck_Inspect((0<usec), usec, jpeg_path);
	}
	return 0;
}


/*****************************************
  NAME		: play_dots
  I/F		: void play_dots(ColorMap color)
  FUNCTION	: Display a dotted pattern
  ARGUMENTS : ColorMap color - the color of the dots
*****************************************/
void 
play_dots(void)
{
  int x=0,y=0;
  int start=0;
  
  ENTER();
  for(; y<WINDOW_SIZE_Y-1; y+=2){
     if(x%2==0) {
	   start++;
     }
     else {
	   start=0;
     }
     
     for(x=start; x<WINDOW_SIZE_X-1; x+=2) {
	    //putpixel(x,y,color);
	    XDrawPoint(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,x,y);
	 }
  }

   XFlush(lcdCheckDisplay);
   LEAVE();
}

/*****************************************
  NAME		: play_dots2
  I/F		: void play_dots2(void)
  FUNCTION	: Display a dotted pattern
  ARGUMENTS : None (void)
*****************************************/
// my added codes -dem
void 
play_dots2(void)
{
  int x=0,y=0;
  int start=1;
  
  ENTER();
  for(; y < WINDOW_SIZE_Y; y++){
     
     start = (start == 1) ? 0 : 1;
     
     for(x = start; x < WINDOW_SIZE_X; x += 2) {
	    //putpixel(x,y,color);
	    XDrawPoint(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,x,y);
	 }
  }

   XFlush(lcdCheckDisplay);
   LEAVE();
}

/*****************************************
  NAME		: play_dots3
  I/F		: void play_dots3(void)
  FUNCTION	: Display a dotted pattern
  ARGUMENTS : None (void)
*****************************************/
// my added codes -dem
void 
play_dots3(void)
{
  int x=0,y=0;
  int start=1;
  
  ENTER();
  for(; y < WINDOW_SIZE_Y; y += 2){
     
     start = (start == 2) ? 0 : 2;
     
     for(x = start; x < WINDOW_SIZE_X; x += 4) {
	    //putpixel(x,y,color);
	    XDrawPoint(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,x,y);
	    XDrawPoint(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,x+1,y);
	    XDrawPoint(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,x,y+1);
	    XDrawPoint(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,x+1,y+1);
	 }
  }

   XFlush(lcdCheckDisplay);
   LEAVE();
}

/*****************************************
  NAME		: play_line_pixel_gap
  I/F		: void play_line_pixel_gap(int gap, int thickness,ColorMap color)
  FUNCTION	: display pattern stripe
  ARGUMENTS : int gap        - gap of each line to be displayed
	        : int thickness  - thickness of the line
 		    : ColorMap color - the color fo the line 
*****************************************/
void 
play_line_pixel_gap(int gap, int thickness)
{
  int i,x;
  
  ENTER();
   if(gap > 2) {
	gap = gap*2;
   }
    for(x = 0; x < WINDOW_SIZE_X-1; x += gap) {
       for(i=0; i<thickness; i++){
//  	     line(x+i,0,x+i,vinfo.yres,color);
		XDrawLine(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,x+i,0,x+i,WINDOW_SIZE_Y-1);

       }
	}
   XFlush(lcdCheckDisplay);
   LEAVE();
}


// my added code
void 
play_background_color(char *backgndColor) 
{
	ENTER();
	XSetBackground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, backgndColor));
	XSetForeground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, backgndColor));

	XFillRectangle(lcdCheckDisplay, lcdCheckWindow, lcdCheckgc, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	XFlush(lcdCheckDisplay);
	LEAVE();
}

// background: white
// foreground: black
// my added code -dem
void 
set_back_fore_color(void) 
{
	ENTER();
	XSetBackground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, "white"));
	XSetForeground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, "black"));

	XFlush(lcdCheckDisplay);
	LEAVE();
}

// background: black
// foreground: white
// my added code -dem
void 
set_back_fore_color2(void) 
{
	ENTER();
	XSetBackground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, "black"));
	XSetForeground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, "white"));

	XFlush(lcdCheckDisplay);
	LEAVE();
}

void 
play_border(void) 
{
	ENTER();
	XSetBackground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, "black"));
	XSetForeground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, "black"));
	XFillRectangle(lcdCheckDisplay, lcdCheckWindow, lcdCheckgc, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	XSetForeground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, "white"));

	XDrawLine(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,0,0,WINDOW_SIZE_X-1,0);
	XDrawLine(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,WINDOW_SIZE_X-1,0,WINDOW_SIZE_X-1,WINDOW_SIZE_Y-1);
	XDrawLine(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,WINDOW_SIZE_X-1,WINDOW_SIZE_Y-1,0,WINDOW_SIZE_Y-1);
	XDrawLine(lcdCheckDisplay,lcdCheckWindow,lcdCheckgc,0,WINDOW_SIZE_Y-1,0,0);

	XFlush(lcdCheckDisplay);
	LEAVE();
}

void 
play_gradient(void)
{
	int grad=0;
	int xlocation=0;

	ENTER();
	for (grad=0,xlocation=0; grad < 25; grad++, xlocation+=25) {
		XSetForeground(lcdCheckDisplay, lcdCheckgc, lcdCheckColor(lcdCheckDisplay, grad_color[grad]));
		XFillRectangle(lcdCheckDisplay, lcdCheckWindow, lcdCheckgc, xlocation, 0, 25, WINDOW_SIZE_Y);	
	}
	LEAVE();
}
// transferred to xlib_lcdcontrol.c - 20061204
#if 0 
/**************************************************
  NAME		: play_lcd_switch
  I/F		: void play_lcd_switch(unsigned int sw)
  FUNCTION	: turn LCD on/off
  ARGUMENTS : int sw (0:off, 1:on)
***************************************************/
// my added codes -dem
void 
play_lcd_switch(unsigned long sw)
{
	unsigned long value = 0;
	unsigned long mask;

	if (sw == 0 || sw == 1) {
		read_PLD(P_ENABLE, &value);
		mask = 0x00000001 << O_LCD_SWITCH;
		value = ~(mask) & value;
		value = value | (sw << O_LCD_SWITCH);
		write_PLD(P_ENABLE, &value);
	}
}


/********************************************************
  NAME		: play_backlight_switch
  I/F		: void play_backlight_switch(unsigned int sw)
  FUNCTION	: turn backlight on/off
  ARGUMENTS : int sw (0:off, 1:on)
*********************************************************/
// my added codes -dem
void 
play_backlight_switch(unsigned long sw)
{
	unsigned long value = 0;
	unsigned long mask;

	if (sw == 0 || sw == 1) {
		read_PLD(BACKLIGHT, &value);
		mask = 0x00000001 << O_BACKLIGHT_SW;
		value = ~(mask) & value;
		value = value | (sw << O_BACKLIGHT_SW);
		write_PLD(BACKLIGHT, &value);
	}
}


/********************************************************
  NAME		: play_lvds_control
  I/F		: void play_lvds_control(unsigned int sw)
  FUNCTION	: turns LVDS on/off
  ARGUMENTS : int sw (0:off, 1:on)
*********************************************************/
// my added codes -dem
void 
play_lvds_control(unsigned long sw)
{
	unsigned long value = 0;
	unsigned long mask;

	if (sw == 0 || sw == 1) {
		read_PLD(I_ENABLE, &value);
		mask = 0x00000001 << O_LVDS_CONTROL;
		printf("initial LVDS = %x\n", value & mask);
		value = ~(mask) & value;
		value = value | (sw << O_LVDS_CONTROL);
		write_PLD(I_ENABLE, &value);
	}
}
#endif

void 
lcdDisplay_Pattern(int pattern)
{
	ENTER();
	switch (pattern) {
		case 0: 
			play_background_color("white");
			break;
		case 1:	
			play_background_color("black");
			break;
		case 2:	
			play_background_color("red");
			break;
		case 3: 
			play_background_color("blue");
			break;
		case 4: 
			play_background_color("green");
			break;
		case 5: 
			play_border();
			break;
		case 6: 
			play_gradient();
			break;
		case 7: 
			set_back_fore_color();
			play_dots2();
			break;
		case 8: 
			set_back_fore_color();
			play_dots3();
			break;
// transferred to xlib_lcdcontrol.c - 20061204
#if 0
		case 9: 
			printf("LCD off\n");
			play_lcd_switch(0);  //OFF
			break;
		case 10: 
			printf("LCD on\n");
			play_lcd_switch(1);  //ON
			break;
		case 11:
			printf("LVDS reset\n");
			play_lvds_control(1);  //RESET
			break;
		case 12:
			printf("LVDS release reset\n");
			play_lvds_control(0);  //RELEASE RESET
			break;
		case 13: 
			printf("Backlight off\n");
			play_backlight_switch(0);  //OFF
			break;
		case 14:
			printf("Backlight on\n");
			play_backlight_switch(1);  //ON
			break;
#endif
		default: 
			printf("Default...\n");
			break;
	}
	LEAVE();
}

int display_click (struct itimerspec *timer)
{
	Window root_return, child_return;
	int root_xreturn, root_yreturn, x = 0, y = 0;
	unsigned int mask_return;	
	int i = 0;

	ENTER();
	i = XQueryPointer(lcdCheckDisplay, lcdCheckWindow, &root_return,
		&child_return, &root_xreturn, &root_yreturn, &x, &y,
		&mask_return);
	
	
	if(click_flag==1 && x<=200 && y<= 200){
		timer_delete(tid);
		LEAVE();
		return 1;
	}
	if(click_flag==0 && x>=824 && y<=200){
		click_flag = 1;
		timer_settime(tid, 0, timer, NULL);
	}
	LEAVE();
	return 0;
}

void timer_signal(int signum)
{
	ENTER();
	click_flag=0;
	LEAVE();
	return;
}

