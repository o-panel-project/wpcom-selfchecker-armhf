/*
 *		Dip SW
 *
 *					2011.6	written by Omoikane Inc.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <gtk/gtk.h>
#include "common.h"
#include "wpcio.h"


static int fd_wpcio=-1, run_loop=0;
static GdkPixbuf *indicator_on, *indicator_off;
static GtkWidget *img0, *img1, *img2, *img3;

/* 20170110 wpc */
extern const guint8 indicator_on_inline[];
extern const guint8 indicator_off_inline[];

static void set_img(GtkWidget *w, int on)
{
	gtk_image_set_from_pixbuf(GTK_IMAGE(w), on ? indicator_on : indicator_off);
}

static gboolean dipsw_loop(gpointer x)
{
	int r, data;
	
	if(!run_loop) return FALSE;
	r=ioctl(fd_wpcio, WPC_GET_DIPSW, &data);
	if(r<0){
		debug_printf(3, "Cannot get DIP Switch, error code = %d\n", r);
		return FALSE;
	}
	set_img(img0, !(data & WPC_DIPSW_1));
	set_img(img1, !(data & WPC_DIPSW_2));
	set_img(img2, !(data & WPC_DIPSW_3));
	set_img(img3, !(data & WPC_DIPSW_4));
	
	return TRUE;
}

static int setup_img()
{
	char tmps[SMALL_STR];
	
	indicator_on = gdk_pixbuf_new_from_inline(
			-1, indicator_on_inline, FALSE, NULL);
	if(!indicator_on){
		printf("Could not open %s\n", tmps);
		return 1;
	}
	
	indicator_off = gdk_pixbuf_new_from_inline(
			-1, indicator_off_inline, FALSE, NULL);
	if(!indicator_off){
		printf("Could not open %s\n", tmps);
		return 1;
	}
	
	g_object_ref(indicator_on);
	g_object_ref(indicator_off);
	
	img0=gtk_image_new_from_pixbuf(indicator_off);
	img1=gtk_image_new_from_pixbuf(indicator_off);
	img2=gtk_image_new_from_pixbuf(indicator_off);
	img3=gtk_image_new_from_pixbuf(indicator_off);
	
	return 0;
}

static int init_dipsw_wpcio()
{
	fd_wpcio=wpcio_open(WPCIO_OPEN_RETRY, "init_dipsw_wpcio");
	return (fd_wpcio<0) ? 1 : 0;
}


int dipsw_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *v0, *bb;
	GtkWidget *a1, *tbl, *lb0, *lb1, *lb2, *lb3;
	
	if(init_dipsw_wpcio()) return 1;
	
	tbl=gtk_table_new(4, 2, FALSE);
	if(setup_img()) return 1;
	
	lb0=gtk_label_new(" Dip SW1 ");
	lb1=gtk_label_new(" Dip SW2 ");
	lb2=gtk_label_new(" Dip SW3 ");
	lb3=gtk_label_new(" Dip SW4 ");
	
	gtk_table_attach(GTK_TABLE(tbl), lb0, 0, 1, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb1, 1, 2, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb2, 2, 3, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb3, 3, 4, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), img0, 0, 1, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), img1, 1, 2, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), img2, 2, 3, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), img3, 3, 4, 1, 2, 0, 0, 5, 20);
	
	a1=gtk_alignment_new(1, 0.8, 0.6, 0.5);
	gtk_container_add(GTK_CONTAINER(a1), tbl);
	
	v0=gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	
	bb=sc_bbox2(&button_no, bsub, gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	run_loop=1;
	g_timeout_add(500, dipsw_loop, 0);
	gtk_main();
	run_loop=0;
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	close(fd_wpcio);
	return 0;
}
