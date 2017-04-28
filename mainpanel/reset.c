/*
 * Soft reset
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <gtk/gtk.h>
#include "common.h"
#include "wpcio.h"

static void press_execute(GtkWidget *widget, gpointer data)
{
	int i, fd_wpcio, r;
	GtkWidget *w, *lb;
	char tmps[SMALL_STR];

	w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(w, get_sc_window_width(), get_sc_window_height());
	gtk_window_set_position(GTK_WINDOW(w), GTK_WIN_POS_CENTER_ALWAYS);

	lb = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(w), lb);
	gtk_widget_show_all(w);

	fd_wpcio = wpcio_open(WPCIO_OPEN_RETRY, "reset");
	if (fd_wpcio < 0) return;

	for (i = 5; 0 <= i; i--) {
		sprintf(tmps, "%d second(s) to reboot terminal.", i);
		gtk_label_set_text(GTK_LABEL(lb), tmps);
		sc_gtk_update();
		sleep(1);
	}
	printf("reset: timer expired.\n");
	sync();sync();sync();
	printf("reset: fs synching.\n");
	r = ioctl(fd_wpcio, WPC_SET_GPIO_OUTPUT_HIGH, 116);
	if (r < 0) {
		printf("GPIO-116 output high failed.\n");
		close(fd_wpcio);
		return;
	}
	/* not reached */
	close(fd_wpcio);
	exit(100);
}

int reset_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *v0, *bb;
	GtkWidget *a0, *lb;
	GtkWidget *a1, *b0;

	if (pid_battery_logger) {
		sc_message(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
			"Battery Logger is running.\n"
			"Stop battery logger by 'Battery Level' menu "
			"before entering 'Soft Reset'.");
		return 1;
	}

	a0 = gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	lb = gtk_label_new("'Soft Reset' test. \n"
			"Press 'Execute' and terminal reboot after 5 seconds.");
	gtk_container_add(GTK_CONTAINER(a0), lb);

	a1 = gtk_alignment_new(0.5, 0.5, 0.3, 0.2);
	b0 = gtk_button_new_with_label("Execute");
	g_signal_connect(b0, "clicked", G_CALLBACK(press_execute), (gpointer)0);
	gtk_container_add(GTK_CONTAINER(a1), b0);

	v0 = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(v0), a0);
	gtk_container_add(GTK_CONTAINER(v0), a1);

	bb = sc_bbox2(&button_no, bsub,
			gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);

	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);

	gtk_main();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	return 0;
}
