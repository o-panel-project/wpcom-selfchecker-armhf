#include <stdio.h>
#include <string.h>
#include <Imlib2.h>

#ifdef HAVE_WS
#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
#include	<WSCwindow.h>
#endif
#include	"../common_functions/misc_funcs.h"
#include	"lcd_jpg_search.h"
#include	"lcd_jpg_file.h"

int show_jpeg_with_imlib(Display *disp, Window win, char *imgfile)
{
	Visual  *vis;
	Colormap cm;
	int      depth;
	int w, h;
	
	XEvent ev;
	Imlib_Updates updates, current_update;
	Imlib_Image buffer;
	Imlib_Image image;
	
	/* get default visual , colormap etc. you could ask imlib2 for what it */
	/* thinks is the best, but this example is intended to be simple */
	vis   = DefaultVisual(disp, DefaultScreen(disp));
	depth = DefaultDepth(disp, DefaultScreen(disp));
	cm    = DefaultColormap(disp, DefaultScreen(disp));
   
	/* create a window */
	win = XCreateSimpleWindow(disp, DefaultRootWindow(disp), 0, 0, 1024, 600, 0, 0, 0);

	/* tell X what events we are interested in */
	XSelectInput(disp, win, ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ExposureMask);
	/* show the window */
	XMapWindow(disp, win);
	/* set our cache to 2 Mb so it doesn't have to go hit the disk as long as */
	/* the images we use use less than 2Mb of RAM (that is uncompressed) */
	imlib_set_cache_size(4096 * 1024);
	
	/* set the maximum number of colors to allocate for 8bpp and less to 128 */
	imlib_set_color_usage(128);
	/* dither for depths < 24bpp */
	imlib_context_set_dither(1);
	/* set the display , visual, colormap and drawable we are using */
	imlib_context_set_display(disp);
	imlib_context_set_visual(vis);
	imlib_context_set_colormap(cm);
	imlib_context_set_drawable(win);
	/* infinite event loop */
	do{
		/* init our updates to empty */
		updates = imlib_updates_init();
		
        /* while there are events form X - handle them */
		while(XPending(disp)){
			XNextEvent(disp, &ev);
			switch (ev.type){
			case Expose:
				updates = imlib_update_append_rect(updates, ev.xexpose.x, ev.xexpose.y, ev.xexpose.width, ev.xexpose.height);
				break;
			//case ButtonPress:
			//	return 0;
			//	break;
			default:
				break;
			}
		}
        usleep(100000);
        
        updates = imlib_updates_merge_for_rendering(updates, 1024, 600);
		for (current_update = updates; current_update; current_update = imlib_updates_get_next(current_update)){
			int up_x, up_y, up_w, up_h;
			
			/* find out where the first update is */
			imlib_updates_get_coordinates(current_update, &up_x, &up_y, &up_w, &up_h);
			
			/* create our buffer image for rendering this update */
			buffer = imlib_create_image(up_w, up_h);
			
			/* draw an icon centered around the mouse position */
			image = imlib_load_image(imgfile);
			imlib_context_set_image(image);
			w = imlib_image_get_width();
			h = imlib_image_get_height();
			imlib_context_set_image(buffer);
			if (image) {
				imlib_blend_image_onto_image(image, 0, 0, 0, w, h, 0, 0, w, h);
				imlib_context_set_image(image);
                imlib_free_image();
			}else{
				printf("Could not open image.\n");
			}

			/* set the buffer image as our current image */
			imlib_context_set_image(buffer);
             
			/* render the image at 0, 0 */
			imlib_render_image_on_drawable(up_x, up_y);
			
			/* don't need that temporary buffer image anymore */
			imlib_free_image();
		}
		
        if(updates) imlib_updates_free(updates);
	}while(XPending(disp));
	XRaiseWindow(disp, win);
	XSync(disp, False);
	return win;
}

/*****************************************
  NAME		: lcd_jpg_check
  I/F		: int	lcd_jpg_check(void)
  FUNCTION	: LCD Display JPG file check
  ARGUMENTS	: void
  RETURN	: int	status
*****************************************/
int	lcd_jpg_check(char *jpeg_path)
{
int		status;
int		max;
long	first_val;

#ifdef HAVE_WS
	extern	WSCwindow		*WNDwind_LCD_jpg;
#endif

	/* File MAX */
	max = IMAGE_MAX;

	status = 0;
	first_val = (long)IMAGE_INIT;
#ifdef HAVE_WS
	WNDwind_LCD_jpg->setProperty(WSNuserValue,(long)IMAGE_INIT);
	WNDwind_LCD_jpg->setVisible(False);
#endif

	/* JPG File Search & flag setting */
	first_val = JPG_search(max, jpeg_path);

#if	0	/* TEST */
printf("[JPG]First[%ld]\n",first_val);
#endif

	if(first_val >= (long)0){
#ifdef HAVE_WS
		WSCvariant variant(first_val);
		WNDwind_LCD_jpg->setProperty(WSNuserValue,variant);

		WNDwind_LCD_jpg->execProcedure(WSEV_MOUSE_PRESS);
#endif
		status = 1;		/* find */
	} else {
		/* Finish */
		set_main_window_btn(True);
	}

	return(status);
}

/*****************************************
  NAME		: JPG_search
  I/F		: long	JPG_search(int	max)
  FUNCTION	: LCD Display JPG file search
  ARGUMENTS	: int	max
  RETURN	: long	first_val
*****************************************/
long JPG_search(int max, char *jpeg_path)
{
	int		i;
	int		array_cnt;
	int		status;
	long	fst_no;
	FILE	*fp;
	char pathbuf[1024];

	array_cnt = 0;
	fst_no = (long)IMAGE_INIT;
	memset(&image_find_array[0],(long)IMAGE_INIT,sizeof(image_find_array));

#if	0	/* TEST */
printf("[JPG]max[%d]\n",max);
#endif

	for(i=0;i<max;i++){
		status = IMAGE_NON;
		IMAGE_FNAME_Table[i].flag = IMAGE_NON;

		/* Capital letter check */
#if	0	/* TEST */
printf("[JPG]file[%s]\n",&IMAGE_FNAME_Table[i].image_name[0]);
#endif
		sprintf(pathbuf, "%s/%s", jpeg_path, IMAGE_FNAME_Table[i].image_name);
		fp = fopen(pathbuf, "r");
		if(fp){
			fclose(fp);
			status |= IMAGE_ON;
		}

#if	0	/* TEST */
printf("[JPG]status[%d]\n",status);
#endif

		if(status > IMAGE_NON && array_cnt < max){
			IMAGE_FNAME_Table[i].flag = status;
			image_find_array[array_cnt] = (long)i;
			if(fst_no==(long)IMAGE_INIT){
				/* First no */
				fst_no = (long)array_cnt;	/* [0] */
#if	0	/* TEST */
printf("[JPG]fst_no[%ld]\n",fst_no);
#endif
			}
			array_cnt++;
		}
#if	0
		usleep(100000);				/* 100ms */
#endif
	}

#if	1	/* TEST */
printf("[JPG]a cnt[%d]\n",array_cnt);
for(i=0;i<max;i++){
printf("[JPG]array[%d]:[%ld]\n",i,image_find_array[i]);
}
#endif

	return(fst_no);
}

