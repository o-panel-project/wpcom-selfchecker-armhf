/*
 *		LED
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


static GtkWidget *button_g;
static GtkWidget *button_r;
static GtkWidget *button_o;
static GtkWidget *b20, *b21, *b_quit;
static int led_stat = 0;  // 0:off, 1:green, 2:red, 3:orange
static int fd_wpcio;
static time_t t_fin=0;
int led_color = 0;


static void on_led(int color)
{
	int ret;
	
	led_color &= ~color;
	
	ret = ioctl(fd_wpcio, WPC_SET_LED, led_color);
	if (ret < 0) {
		debug_printf(3, "Cannot set LED, error code = %d\n", ret);
	}
}


static void off_led(int color)
{
	int ret;
	
	led_color |= color;
	
	ret = ioctl(fd_wpcio, WPC_SET_LED, led_color);
	if (ret < 0) {
		debug_printf(3, "Cannot set LED, error code = %d\n", ret);
	}
}


static void press_led(GtkWidget *widget, gpointer data)
{
	int color = *(int*)data;
	gboolean state;
	
	state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	if (state == TRUE) {
		on_led(color);
	} else {
		off_led(color);
	}
}


static int loop_run = 0;
static void press_stop(GtkWidget *widget, gpointer data)
{
	loop_run = 0;
	gtk_widget_set_sensitive(button_g, TRUE);
	gtk_widget_set_sensitive(button_r, TRUE);
	gtk_widget_set_sensitive(button_o, TRUE);
	gtk_widget_set_sensitive(b20, TRUE);
	gtk_widget_set_sensitive(b_quit, TRUE);
	gtk_widget_set_sensitive(b21, FALSE);
}


void led_check_timeup()
{
	if (t_fin && t_fin<time(NULL)) {
		debug_printf(3, "Loop test time up.\n");
		press_stop(NULL, 0);
		gtk_main_quit();
	}
}


static gboolean led_loop(gpointer data)
{
	if (!loop_run) {
		return FALSE;
	}
	
	switch(led_stat)
	{
	case 0:
		off_led(WPC_LED_GREEN);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_g), FALSE);
		off_led(WPC_LED_RED);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_r), FALSE);
		off_led(WPC_LED_ORANGE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_o), FALSE);
		led_stat++;
		break;
	case 1:
		on_led(WPC_LED_GREEN);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_g), TRUE);
		off_led(WPC_LED_RED);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_r), FALSE);
		off_led(WPC_LED_ORANGE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_o), FALSE);
		led_stat++;
		break;
	case 2:
		off_led(WPC_LED_GREEN);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_g), FALSE);
		on_led(WPC_LED_RED);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_r), TRUE);
		off_led(WPC_LED_ORANGE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_o), FALSE);
		led_stat++;
		break;
	case 3:
		off_led(WPC_LED_GREEN);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_g), FALSE);
		off_led(WPC_LED_RED);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_r), FALSE);
		on_led(WPC_LED_ORANGE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_o), TRUE);
		led_stat = 0;
		break;
	default:
		led_stat = 0;
		break;
	}
	
	led_check_timeup();
	
	return TRUE;
}


static void press_start(GtkWidget *widget, gpointer data)
{
	loop_run = 1;
	gtk_widget_set_sensitive(button_g, FALSE);
	gtk_widget_set_sensitive(button_r, FALSE);
	gtk_widget_set_sensitive(button_o, FALSE);
	gtk_widget_set_sensitive(b20, FALSE);
	gtk_widget_set_sensitive(b_quit, FALSE);
	gtk_widget_set_sensitive(b21, TRUE);
	g_timeout_add(500, led_loop, 0);
}


static int init_led_wpcio()
{
	int x;
	
	fd_wpcio=wpcio_open(WPCIO_OPEN_RETRY, "init_led_wpcio");
	if(fd_wpcio<0) return 1;
	
	if(ioctl(fd_wpcio, WPC_GET_LED, &x)<0){
		debug_printf(3, "led ioctl failure.");
		return 1;
	}
	led_stat = 0; //min((x==4 ? 3 : x), 4);
	
	return 0;
}

int led_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *v0, *bb;
	GtkWidget *a1;
	GtkWidget *a2, *h2;
	int color_g = WPC_LED_GREEN;
	int color_r = WPC_LED_RED;
	int color_o = WPC_LED_ORANGE;
	GtkWidget *t0;
	
	if (init_led_wpcio()) {
		return 1;
	}
	
	v0=gtk_vbox_new(FALSE, 10);
	
	button_g = gtk_toggle_button_new_with_label("LED Green");
	g_signal_connect(G_OBJECT(button_g), "toggled", G_CALLBACK(press_led), &color_g);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_g), FALSE);
	off_led(color_g);
	
	button_r = gtk_toggle_button_new_with_label("LED Red");
	g_signal_connect(G_OBJECT(button_r), "toggled", G_CALLBACK(press_led), &color_r);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_r), FALSE);
	off_led(color_r);

	button_o = gtk_toggle_button_new_with_label("LED Orange");
	g_signal_connect(G_OBJECT(button_o), "toggled", G_CALLBACK(press_led), &color_o);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_o), FALSE);
	off_led(color_o);

	t0=gtk_table_new(2, 2, FALSE);

	gtk_table_attach(GTK_TABLE(t0), button_o, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 3, 3);
	gtk_table_attach(GTK_TABLE(t0), button_g, 1, 2, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 3, 3);
	gtk_table_attach(GTK_TABLE(t0), button_r, 1, 2, 1, 2, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 3, 3);
	
	a1=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	gtk_container_add(GTK_CONTAINER(a1), t0);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	
	h2=gtk_hbox_new(FALSE, 10);
	b20=gtk_button_new_with_label("Start Loop");
	g_signal_connect(b20, "clicked", G_CALLBACK(press_start), (gpointer)0);
	b21=gtk_button_new_with_label("Stop Loop");
	g_signal_connect(b21, "clicked", G_CALLBACK(press_stop), (gpointer)0);
	gtk_widget_set_sensitive(b21, FALSE);

	gtk_container_add(GTK_CONTAINER(h2), b20);
	gtk_container_add(GTK_CONTAINER(h2), b21);
	
	a2=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);
	gtk_container_add(GTK_CONTAINER(a2), h2);
	gtk_container_add(GTK_CONTAINER(v0), a2);
	
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
	
	if (testtime) {
		t_fin = time(NULL) + testtime;
		press_start(NULL, 0);
	}
	
	gtk_main();
	
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	close(fd_wpcio);
	
	return 0;
}

