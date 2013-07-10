/*
 *		LCD - PWM
 *
 *					2011.6	written by Omoikane Inc.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <gtk/gtk.h>
#include "common.h"
#include "wpcio.h"

static time_t t_fin=0;
static int brightness=70, loop_run=0;
static GtkWidget *b_start, *b_stop, *b_quit, *slider, *aj;

#define SYSFS_BACKLIGHT_IF_DIR	\
	"/sys/devices/platform/omapdss/generic-bl/backlight/omap3evm-bklight"
#define SYSFS_BACKLIGHT_POWER	SYSFS_BACKLIGHT_IF_DIR"/bl_power"
#define SYSFS_BACKLIGHT_BRIGHT	SYSFS_BACKLIGHT_IF_DIR"/brightness"
#define SYSFS_DISPLAY_IF_DIR	\
	"/sys/devices/omapdss/display0"
#define SYSFS_DISPLAY_ENABLE    SYSFS_DISPLAY_IF_DIR"/enabled"

#define DEV_BL			SYSFS_BACKLIGHT_POWER
#define DEV_BRIGHTNESS	SYSFS_BACKLIGHT_BRIGHT

static int wpcio_lvds_shutdown(int onoff)
{
	int fd, req;

	fd = wpcio_open(WPCIO_OPEN_RETRY, "lvds_shutdown(89)");
	if (fd < 0) return -1;

	if (onoff == 0)
		req = WPC_SET_GPIO_OUTPUT_LOW;
	else
		req = WPC_SET_GPIO_OUTPUT_HIGH;
	if (ioctl(fd, req, 89) < 0) {
		debug_printf(3, "lvds_shutdown(89) ioctl failure.");
		onoff = -1;
	}
	close(fd);

	return onoff;
}

static int wpcio_backlight_en(int onoff)
{
	int fd, req;

	fd = wpcio_open(WPCIO_OPEN_RETRY, "backlight_en(88)");
	if (fd < 0) return -1;

	if (onoff == 0)
		req = WPC_SET_GPIO_OUTPUT_LOW;
	else
		req = WPC_SET_GPIO_OUTPUT_HIGH;
	if (ioctl(fd, req, 88) < 0) {
		debug_printf(3, "backlight_en(88) ioctl failure.");
		onoff = -1;
	}
	close(fd);

	return onoff;
}

static inline int sysfs_backlight_read(const char *f)
{
	FILE *fp;
	int val;

	if ((fp = fopen(f, "r")) == NULL) {
		perror("fopen");
		return -1;
	}

	if (fscanf(fp, "%d\n", &val) == EOF) {
		perror("fscanf");
		val = -1;
	}

	fclose(fp);
	return val;
}

static inline void sysfs_backlight_write(const char *f, char *d, size_t s)
{
	int fd, written;

	if ((fd = open(f, O_WRONLY)) < 0) {
		perror("open");
		return;
	}

	written = write(fd, d, s);
	if (s != written) {
		perror("write");
	}

	close(fd);
}

static void brightness_set(int n)
{
	char tmps[SMALL_STR];
	sprintf(tmps, "%d\n", n);
	sysfs_backlight_write(DEV_BRIGHTNESS, (char*)tmps, strlen(tmps));
	brightness=n;
}

static void bl_set(int n)
{
	char tmps[SMALL_STR];
	sprintf(tmps, "%d\n", n);
	sysfs_backlight_write(DEV_BL, (char*)tmps, strlen(tmps));
}

static int int_get(char *path)
{
	return sysfs_backlight_read(path);
}

#define brightness_get() int_get(DEV_BRIGHTNESS)
#define bl_get() int_get(DEV_BL)

static gboolean bright_dim_loop(gpointer data)
{
	if(!loop_run) return FALSE;
	
	gtk_adjustment_set_value(GTK_ADJUSTMENT(aj), brightness*1.0);
	brightness_set(brightness);
	brightness-=10;
	if(brightness<0) brightness=100;
	
	if(t_fin && t_fin<time(NULL)){
		gtk_main_quit();
		return FALSE;
	}
	
	return TRUE;
}

static void loop_start(GtkWidget *widget, gpointer data)
{
	loop_run=1;
	gtk_widget_set_sensitive(b_start, FALSE);
	gtk_widget_set_sensitive(slider, FALSE);
	gtk_widget_set_sensitive(b_quit, FALSE);
	gtk_widget_set_sensitive(b_stop, TRUE);
	g_timeout_add(500, bright_dim_loop, 0);
}

static void loop_stop(GtkWidget *widget, gpointer data)
{
	loop_run=0;
	gtk_widget_set_sensitive(b_start, TRUE);
	gtk_widget_set_sensitive(slider, TRUE);
	gtk_widget_set_sensitive(b_quit, TRUE);
	gtk_widget_set_sensitive(b_stop, FALSE);
//	gtk_adjustment_set_value(GTK_ADJUSTMENT(aj), 70.0);
//	brightness=70;
}

static void slider_set(GtkWidget *widget, gpointer data)
{
	brightness_set(gtk_adjustment_get_value(GTK_ADJUSTMENT(widget)));
}

static void bl_toggle(GtkWidget *widget, gpointer data)
{
	bl_set(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))==FALSE);
}

//	20110831VACS
static	void
bl_toggle2(GtkWidget *widget, gpointer data)
{
	bl_toggle_charge ? wpcio_lvds_shutdown(0) : wpcio_lvds_shutdown(1);
	if(bl_toggle_charge)		bl_toggle_charge=0;
	else if(!bl_toggle_charge)	bl_toggle_charge=1;

	gtk_button_set_label(GTK_BUTTON(widget),bl_toggle_charge ? "\n\n\n\n                              LVDS_SHUTDOWN# Off                              \n\n\n\n" : "\n\n\n\n                              LVDS_SHUTDOWN# On                               \n\n\n\n");
}

//	20121002VACS
static void
reset_bri(GtkWidget *widget, gpointer data)
{
	gtk_adjustment_set_value(GTK_ADJUSTMENT(aj), 70.0);
	brightness=70;
}

int lcd_pwm_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *v0, *bb;
	GtkWidget *a1, *h0, *lb0;
	GtkWidget *a2, *lb1, *a3, *cb;
	GtkWidget *b0, *t0;
	
	lb0=gtk_label_new("Bright-Dim Loop:");
	b_start=gtk_button_new_with_label("Start");
	g_signal_connect(b_start, "clicked", G_CALLBACK(loop_start), (gpointer)0);
	
	b_stop=gtk_button_new_with_label("Stop");
	gtk_widget_set_sensitive(b_stop, FALSE);
	g_signal_connect(b_stop, "clicked", G_CALLBACK(loop_stop), (gpointer)0);
	
	h0=gtk_hbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(h0), lb0);
	gtk_container_add(GTK_CONTAINER(h0), b_start);
	gtk_container_add(GTK_CONTAINER(h0), b_stop);
	a1=gtk_alignment_new(0.5, 0.8, 0.5, 0.13);
	gtk_container_add(GTK_CONTAINER(a1), h0);
	
	lb1=gtk_label_new("Brightness %: ");

	brightness=max(brightness_get(),0);
	aj=(GtkWidget *)gtk_adjustment_new(brightness, 0, 101, 10, 10, 1);
	slider=gtk_hscale_new((GtkAdjustment *)aj);
	g_signal_connect(aj, "value-changed", G_CALLBACK(slider_set), (gpointer)0);

	b0=gtk_button_new_with_label("Reset");
	g_signal_connect(b0, "clicked", G_CALLBACK(reset_bri), (gpointer)0);

	t0=gtk_table_new(2, 2, FALSE);

	gtk_table_attach_defaults(GTK_TABLE(t0), lb1, 0, 1, 0, 2);
	gtk_table_attach_defaults(GTK_TABLE(t0), slider, 1, 2, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(t0), b0, 1, 2, 1, 2);

	a2=gtk_alignment_new(0.5, 0.5, 0.6, 0.5);
	gtk_container_add(GTK_CONTAINER(a2), t0);
	
#if	1	/*	20110831VACS	*/
	a3=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	cb=gtk_button_new_with_label(bl_toggle_charge ? "\n\n\n\n                              LVDS_SHUTDOWN# Off                              \n\n\n\n" : "\n\n\n\n                              LVDS_SHUTDOWN# On                               \n\n\n\n");
	g_signal_connect(cb, "clicked", G_CALLBACK(bl_toggle2), (gpointer)0);
#else
	a3=gtk_alignment_new(0.5, 0.5, 0.2, 0.5);
	cb=gtk_check_button_new_with_label("Back Light Power");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb), bl_get()==0 ? TRUE : FALSE);
	g_signal_connect(cb, "toggled", G_CALLBACK(bl_toggle), (gpointer)0);
#endif
	gtk_container_add(GTK_CONTAINER(a3), cb);
	
	v0=gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	gtk_container_add(GTK_CONTAINER(v0), a2);
	gtk_container_add(GTK_CONTAINER(v0), a3);
	
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(&button_no, bsub, b_quit, sc_bbox1_click);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	if(testtime){
		t_fin=time(NULL)+testtime;
		loop_start(NULL, 0);
	}
	gtk_main();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	return 0;
}
