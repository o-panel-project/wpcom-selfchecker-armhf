	/*
 *		Strategy File
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
#include <gtk/gtk.h>
#include "common.h"

static void press_execute(GtkWidget *widget, gpointer data)
{
	GtkWidget *w, *lb;
	char f[SMALL_STR], tmps[SMALL_STR];
	
	sprintf(f, "%s/script/strategy-%d", base_path, (int)data);
	
	w=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(w, 1024, 600);
	gtk_window_set_position(GTK_WINDOW(w), GTK_WIN_POS_CENTER_ALWAYS);
	
	sprintf(tmps, "Executing %s ...", f);
	lb=gtk_label_new(tmps);
	
	gtk_container_add(GTK_CONTAINER(w), lb);
	gtk_widget_show_all(w);
	
	sc_gtk_update();
	
	system(f);
	gtk_widget_destroy(w);
}

static int check_exclude_battery()
{
	if(!pid_battery_logger) return 1;
	sc_message(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
				"Battery Logger is running.\n"
				"Stop battery logger by 'Battery Level' menu before entering 'Strategy File'.");
	return 0;
}

static int check_exclude_audio()
{
	if(!(audio_pid&&audio_is_loop)) return 1;
	sc_message(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
				"Audio test is running.\n"
				"Stop audio test by 'Audio' menu before entering 'Strategy File'.");
	return 0;
}

int strategy_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *v0, *bb;
	GtkWidget *a1, *v1, *lb1, *lb2, *lb3;
	
	if(!check_exclude_battery()) return 1;
	if(!check_exclude_audio()) return 1;
	
	v1=gtk_vbox_new(FALSE, 10);
	a1=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);
	
	lb1=gtk_button_new_with_label("Excute Strategy File 1");
	g_signal_connect(lb1, "clicked", G_CALLBACK(press_execute), (gpointer)1);
	lb2=gtk_button_new_with_label("Excute Strategy File 2");
	g_signal_connect(lb2, "clicked", G_CALLBACK(press_execute), (gpointer)2);
	lb3=gtk_button_new_with_label("Excute Strategy File 3");
	g_signal_connect(lb3, "clicked", G_CALLBACK(press_execute), (gpointer)3);
	
	gtk_container_add(GTK_CONTAINER(v1), lb1);
	gtk_container_add(GTK_CONTAINER(v1), lb2);
	gtk_container_add(GTK_CONTAINER(v1), lb3);
	gtk_container_add(GTK_CONTAINER(a1), v1);
	
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
	
	button_no=0;
	gtk_main();
	
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	return 0;
}
