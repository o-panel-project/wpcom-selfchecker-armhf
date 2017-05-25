/*
 *		LCD-Display (grid touch test)
 *
 *					2011.6	written by Omoikane Inc.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <gtk/gtk.h>
#include "common.h"

static gboolean delete_event( GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    gtk_main_quit();
    return TRUE;
}

static void destroy( GtkWidget *widget, gpointer   data )
{
    gtk_main_quit();
}

static int *flags;
GtkWidget  **button_image;
GdkPixbuf *pix_orange, *pix_green;

/* 20170111 wpc */
extern const guint8 p16green_inline[];
extern const guint8 p16orange_inline[];

static struct timeval tv0;
static const double nx_weight = (1024.0/36.0);
static const double ny_weight = (600.0/17.0);
static int nx=36, ny=17;
static int nx_arg=0, ny_arg=0;

// click interval for exit in usec
static long long ival=280000;

static void check_time()
{
	unsigned long long t;
	struct timeval tv1;
	
	gettimeofday(&tv1,NULL);
	t=(tv1.tv_sec-tv0.tv_sec)*1000000+tv1.tv_usec-tv0.tv_usec;
	if(t<ival) exit(0);
	tv0=tv1;
}

static void click_func(GtkWidget *widget, gpointer data)
{
	int n;
	
	check_time();
	
	n=(unsigned int)data;
	flags[n]++;
	
	gtk_image_set_from_pixbuf(GTK_IMAGE(button_image[n]), (flags[n]&1) ? pix_orange : pix_green);
}

static gboolean button_release_func(
		GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	g_print("Touch (x, y)=(%.1f, %.1f)\n", event->x_root, event->y_root);
	click_func(widget, data);
	return FALSE;
}

void usage()
{
	printf("usage : tp-grid <opt>\n");
	printf("-x <n>: number of button in x-direction (default 50)\n");
	printf("-y <n>: number of button in y-direction (default 30)\n");
	printf("-t <n>: double click interval for exit in usec (default 280)\n");
}

int main(int argc, char *argv[])
{
	int c, i, j, gargc=0, n;
	GtkWidget *window, *button, *tbl;
	char *base_path;
	
	base_path="/mnt1";
	
	while(1){
		c=getopt(argc, argv, "x:y:t:b:");
		if(c==-1) break;
		switch(c){
		case 'x':
			nx_arg=atoi(optarg);
			break;
		case 'y':
			ny_arg=atoi(optarg);
			break;
		case 't':
			ival=1000*atoi(optarg);
			break;
		case 'b':
			base_path=strdup(optarg);
			break;
		}
	}
	
	gettimeofday(&tv0,NULL);
    
	gtk_init(&gargc, &argv);
    
	GdkScreen *gscr = gdk_screen_get_default();
	gint w = gdk_screen_get_width(gscr);
	gint h = gdk_screen_get_height(gscr);
	if (nx_arg > 0)
		nx = nx_arg;
	else
		nx = (int)((double)w / (double)nx_weight + 0.5);
	if (ny_arg > 0)
		ny = ny_arg;
	else
		ny = (int)((double)h / (double)ny_weight + 0.5);
	g_print("tp-grid matrix %dx%d\n", nx, ny);

	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER (window), 0);
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK (delete_event), NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK (destroy), NULL);
    
	tbl=gtk_table_new(nx,ny,FALSE);
	gtk_container_add (GTK_CONTAINER (window), tbl);
	gtk_container_set_border_width (GTK_CONTAINER (tbl), 0);
	gtk_widget_set_usize(window, w, h);
    
	flags=(int *)malloc(sizeof(unsigned int)*nx*ny);
	button_image=(GtkWidget **)malloc(sizeof(GtkWidget *)*nx*ny);
	
	pix_orange = gdk_pixbuf_new_from_inline(-1, p16orange_inline, FALSE, NULL);
	g_object_ref(pix_orange);
	pix_green = gdk_pixbuf_new_from_inline(-1, p16green_inline, FALSE, NULL);
	g_object_ref(pix_green);
    
	for(j=0;j<ny;j++){
		for(i=0;i<nx;i++){
			n=nx*j+i;
			flags[n]=0;
 			button=gtk_button_new_with_label("");
 			button_image[n]=gtk_image_new_from_pixbuf(pix_green);
			gtk_button_set_image(GTK_BUTTON(button), button_image[n]);
			gtk_table_attach(GTK_TABLE(tbl), button, i, i+1, j, j+1, GTK_FILL | GTK_EXPAND | GTK_SHRINK, GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
			g_signal_connect(G_OBJECT(button), "button-release-event", G_CALLBACK(button_release_func), (gpointer)n);
	    }
	}
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
