/*
 *    I2C Cradle
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "sc_i2c.h"
#include "common.h"

int i2c_main_auto_menu = 0;	/* RH */

GtkWidget *lb_result, *lb_write, *lb_read;
GtkWidget *lb_error_rate;
GtkWidget *b_single, *b_cycle_wv, *b_cycle_read, *b_quit;

static guint i2c_cradle_timer_tag = (guint)0;	/*	20110818VACS	*/

int tatal_count, error_count;

//
//	Label update
//
static void update_result(int is_error, char *mes)
{
	char tmps[SMALL_STR];
	char tmps2[SMALL_STR];
	
	// テスト結果文字列表示
	sprintf(tmps, "<span size='xx-large'>Result : %s%s%s</span>", is_error ? "<span color='red'>" : "",
					mes, is_error ? "</span>" : "");
	gtk_label_set_markup(GTK_LABEL(lb_result), tmps);
	
	// エラーレート文字列表示
	sprintf(tmps2, "<span size='xx-large'>Total = %d : Error = %d : Error rate = %d %%</span>", 
		tatal_count, error_count, (tatal_count==0)? 0 : (int)((double)error_count/tatal_count*100));
	gtk_label_set_markup(GTK_LABEL(lb_error_rate), tmps2);
}

// 
//  read test
//
static void read_cradle()
{
	int r;
	unsigned char rbuf[I2C_DATA_SIZE];
	char tmps[SMALL_STR];
	
	switch((r=sc_i2c_read_cradle(rbuf))){
	case 1:
	case 2:
		gtk_label_set_text(GTK_LABEL(lb_read), "");
		gtk_label_set_text(GTK_LABEL(lb_write), "");
		error_count++;
		update_result(1, r==1 ? "Device Open ERROR" : "Read IOCTL ERROR");
		return;
	}
	gtk_label_set_text(GTK_LABEL(lb_write), "");
	sprintf(tmps, "<span size='xx-large'>Read : 0x%02x%02x%02x%02x(%c%c%c%c)</span>",
							rbuf[0], rbuf[1], rbuf[2], rbuf[3], 
							rbuf[0], rbuf[1], rbuf[2], rbuf[3]);
	gtk_label_set_markup(GTK_LABEL(lb_read), tmps);
	update_result(0, "Reading");
}

// 
//  write test
//
int write_verify_cradle()
{
	int r;
	char tmps[SMALL_STR];
	unsigned char wbuf[I2C_DATA_SIZE], rbuf[I2C_DATA_SIZE];
	
	// write i2c
	r=sc_i2c_write_cradle(wbuf);
	if(r==1){
		error_count++;
		update_result(1, "Device Open ERROR");
		return r;
	}
	
	sprintf(tmps, "<span size='xx-large'>Write : 0x%02x%02x%02x%02x(%c%c%c%c)</span>",
							wbuf[0], wbuf[1], wbuf[2], wbuf[3], 
							wbuf[0], wbuf[1], wbuf[2], wbuf[3]);
	gtk_label_set_text(GTK_LABEL(lb_read), "");
	gtk_label_set_markup(GTK_LABEL(lb_write), tmps);
	
	if(r==2){
		error_count++;
		update_result(1, "Write IOCTL ERROR");
		return r;
	}
	
	// read i2c for verify
	switch((r=sc_i2c_read_cradle(rbuf))){
	case 1:
	case 2:
		gtk_label_set_text(GTK_LABEL(lb_read), "");
		gtk_label_set_text(GTK_LABEL(lb_write), "");
		error_count++;
		update_result(1, r==1 ? "Device Open ERROR" : "Read IOCTL ERROR");
		return r;
	}
	
	if(memcmp(wbuf, rbuf, I2C_DATA_SIZE)!=0){
		error_count++;
		update_result(1, "FAIL");
	}else{
		sprintf(tmps,
			"<span size='xx-large'>"
			"Read : 0x%02x%02x%02x%02x(%c%c%c%c)"
			"</span>",
			rbuf[0], rbuf[1], rbuf[2], rbuf[3],
			rbuf[0], rbuf[1], rbuf[2], rbuf[3]);
		gtk_label_set_markup(GTK_LABEL(lb_read), tmps);
		update_result(0, "PASS");
	}
	
	return r;
}

// 
//  Single write test
//
static void press_single(GtkWidget *widget, gpointer data)
{
	int ret;
	char *result_dialog_text;
	
	error_count = 0;
	tatal_count = 1;
	ret = write_verify_cradle();
	switch (ret)
	{
	case 1:
		result_dialog_text = "<span foreground='red' font='Serif 40px'><b>Device Open ERROR</b></span>";
		break;
	case 2:
		result_dialog_text = "<span foreground='red' font='Serif 40px'><b>Write IOCTL ERROR</b></span>";
		break;
	default:
		result_dialog_text = "<span font='Serif 40px'><b>PASS</b></span>";
		break;
	}
	
	//sc_message_dialog(result_dialog_text, "OK", TRUE);
}
static gboolean press_single_func(
		GtkWidget *widget, GdkEvent *event,gpointer data)
{
	press_single(widget, data);
	return FALSE;
}

//
//	dispacher by timer
//
static int test_type=0;
static gboolean loop_test(gpointer data)
{
	switch(test_type){
	case 1:
		tatal_count++;
		write_verify_cradle();
		break;
	
	case 2:
		tatal_count++;
		read_cradle();
		break;
	}
	return TRUE;
}

static void press_cycle_wv(GtkWidget *widget, gpointer data)
{
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))==TRUE){
		gtk_widget_set_sensitive(b_single, FALSE);
		gtk_widget_set_sensitive(b_cycle_read, FALSE);
		gtk_widget_set_sensitive(b_quit, FALSE);
		error_count = 0;
		tatal_count = 0;
		test_type=1;
	}else{
		gtk_widget_set_sensitive(b_single, TRUE);
		gtk_widget_set_sensitive(b_cycle_read, TRUE);
		gtk_widget_set_sensitive(b_quit, TRUE);
		test_type=0;
	}
}
static gboolean press_cycle_wv_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gboolean state;
	state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_toggle_button_set_active(
			GTK_TOGGLE_BUTTON(widget), state ? FALSE : TRUE); /* toggled */
	press_cycle_wv(widget, data);
	return FALSE;
}

static void press_cycle_read(GtkWidget *widget, gpointer data)
{
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))==TRUE){
		gtk_widget_set_sensitive(b_single, FALSE);
		gtk_widget_set_sensitive(b_cycle_wv, FALSE);
		gtk_widget_set_sensitive(b_quit, FALSE);
		error_count = 0;
		tatal_count = 0;
		test_type=2;
	}else{
		gtk_widget_set_sensitive(b_single, TRUE);
		gtk_widget_set_sensitive(b_cycle_wv, TRUE);
		gtk_widget_set_sensitive(b_quit, TRUE);
		test_type=0;
	}
}
static gboolean press_cycle_read_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gboolean state;
	state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_toggle_button_set_active(
			GTK_TOGGLE_BUTTON(widget), state ? FALSE : TRUE); /* toggled */
	press_cycle_read(widget, data);
	return FALSE;
}

int i2c_cradle_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *v0, *bb;
	GtkWidget *a1, *v1;
	printf("%s():auto menuno=%d\n",__func__,i2c_main_auto_menu);
	// ボタン部分格納用hbox
	GtkWidget *h0;
	
	tatal_count = 0;
	error_count = 0;
	
	sc_i2c_set_power(1);
	a1=gtk_alignment_new(1,1,1,1);
	
	lb_result=gtk_label_new("");
	lb_error_rate=gtk_label_new("");
	lb_write=gtk_label_new("");
	lb_read=gtk_label_new("");
	
	b_single=gtk_button_new_with_label("Single Test");
	b_cycle_wv=gtk_toggle_button_new_with_label("Cyclic Test");
	b_cycle_read=gtk_toggle_button_new_with_label("Cyclic Read");
	
	v1=gtk_vbox_new(FALSE, 7);
	
	gtk_container_add(GTK_CONTAINER(v1), lb_result);
	gtk_container_add(GTK_CONTAINER(v1), lb_error_rate);
	gtk_container_add(GTK_CONTAINER(v1), lb_write);
	gtk_container_add(GTK_CONTAINER(v1), lb_read);
	
	h0=gtk_hbox_new(FALSE, 7);
	
	gtk_container_add(GTK_CONTAINER(v1), h0);
	gtk_container_add(GTK_CONTAINER(h0), b_single);
	
	g_signal_connect(b_single, "clicked", G_CALLBACK(press_single), (gpointer)0);
	g_signal_connect(b_single, "button-release-event", G_CALLBACK(press_single_func), (gpointer)0);
	gtk_container_add(GTK_CONTAINER(h0), b_cycle_wv);
	g_signal_connect(b_cycle_wv, "toggled", G_CALLBACK(press_cycle_wv), (gpointer)0);
	//g_signal_connect(b_cycle_wv, "button-release-event", G_CALLBACK(press_cycle_wv_func), (gpointer)0);
	gtk_container_add(GTK_CONTAINER(h0), b_cycle_read);
	g_signal_connect(b_cycle_read, "toggled", G_CALLBACK(press_cycle_read), (gpointer)0);
	//g_signal_connect(b_cycle_read, "button-release-event", G_CALLBACK(press_cycle_read_func), (gpointer)0);
	gtk_container_add(GTK_CONTAINER(a1), v1);
	
	v0=gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(&button_no, bsub, b_quit, sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	/*	20110818VACS	*/
	i2c_cradle_timer_tag = g_timeout_add(500, loop_test, 0);

	switch (i2c_main_auto_menu) {
	case 1:
		gtk_button_clicked(GTK_BUTTON(b_single));
		break;
	case 2:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b_cycle_wv), TRUE);
		break;
	case 3:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b_cycle_read), TRUE);
		break;
	default:
		break;
	}

	gtk_main();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	sc_i2c_set_power(0);

	/*	20110818VACS	*/
	if(i2c_cradle_timer_tag){
		g_source_remove(i2c_cradle_timer_tag);
		i2c_cradle_timer_tag = (guint)0;
	}

	return 0;
}
