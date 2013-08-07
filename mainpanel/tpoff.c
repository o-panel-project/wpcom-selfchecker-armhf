/*
 *     Touch Pad off
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


static void press_execute(GtkWidget *widget, gpointer data)
{
	int i, fd_wpcio, r;
	GtkWidget *w, *lb;
	char tmps[SMALL_STR];
	
	w=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(w, 1024, 600);
	gtk_window_set_position(GTK_WINDOW(w), GTK_WIN_POS_CENTER_ALWAYS);
	
	lb=gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(w), lb);
	gtk_widget_show_all(w);
	
	fd_wpcio=wpcio_open(WPCIO_OPEN_RETRY, "tpoff");
	if(fd_wpcio<0) return;
	
	r=ioctl(fd_wpcio, WPC_SET_TP_ONOFF, 0);
	if(r<0){
		printf("Connect/Disconnect SDIO WIFI, error code = %d\n", r);
		close(fd_wpcio);
		return;
	}
	
	for(i=5;0<=i;i--){
		sprintf(tmps, "Now touch pad is OFF.\n"
						"Confirm that touch pad is not working. \n"
						"%d second(s) to restart Xorg.",
						i);
		gtk_label_set_text(GTK_LABEL(lb), tmps);
		sc_gtk_update();
		sleep(1);
	}
	r=ioctl(fd_wpcio, WPC_SET_TP_ONOFF, 1);
	if(r<0){
		printf("Connect/Disconnect SDIO WIFI, error code = %d\n", r);
		close(fd_wpcio);
		return;
	}
	exit(100);
}

int tpoff_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *v0, *bb;
	GtkWidget *a0, *lb;
	GtkWidget *a1, *b0;

	if(pid_battery_logger){
		sc_message(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
					"Battery Logger is running.\n"
					"Stop battery logger by 'Battery Level' menu before entering 'Touch Pad OFF'.");
		return 1;
	}
	
	a0=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	lb=gtk_label_new("'Touch Pad OFF' test. \n"
					"Press 'Execute' and touch pad will be stopped for 5 seconds.\n"
					"After that, both self checker and Xorg will be restarted.");
	gtk_container_add(GTK_CONTAINER(a0), lb);

	a1=gtk_alignment_new(0.5, 0.5, 0.3, 0.2);
	b0=gtk_button_new_with_label("Execute");
	g_signal_connect(b0, "clicked", G_CALLBACK(press_execute), (gpointer)0);
	gtk_container_add(GTK_CONTAINER(a1), b0);
	
	v0=gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(v0), a0);
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
	
	gtk_main();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	return 0;
}


int tpfunc_main(GtkWidget *table, GtkWidget *i)
{
	GtkWidget *w, *lb;
	char tmps[SMALL_STR];
	int ival = (int)i;
	
	w=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(w, 1024, 600);
	gtk_window_set_position(GTK_WINDOW(w), GTK_WIN_POS_CENTER_ALWAYS);
	
	lb=gtk_label_new("Starting full-screen program ...\n"
						"Note that full-screen program will be finished by double-clicking any place. ");
	gtk_container_add(GTK_CONTAINER(w), lb);
	gtk_widget_show_all(w);
	sc_gtk_update();
	
	sprintf(tmps, "tp-grid -b %s", base_path);
	if(0<ival)
		append_sprintf(tmps, " -t %d", ival);
	
	system(tmps);
	gtk_widget_destroy(w);
	return 0;
}

