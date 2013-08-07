/*
 * Display dmesg text
 * 2013/08/07 wpc
 */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "common.h"

static void dmesg_view(GtkTextBuffer *buffer)
{
	int fd;
	struct stat st;
	char *p;

	system("dmesg > /tmp/sc_dmesg.txt");

	fd = open("/tmp/sc_dmesg.txt", O_RDONLY);
	fstat(fd, &st);
	p = (char*)malloc(st.st_size);
	read(fd, p, st.st_size);
	close(fd);
	gtk_text_buffer_set_text(buffer, p, -1);
	free(p);
}

int dmesg_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *vbox, *view, *scrolledwindow;
	GtkWidget *bb;
	GtkTextBuffer *buffer;

	vbox = gtk_vbox_new(FALSE, 10);
	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	view = gtk_text_view_new();
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	dmesg_view(buffer);
	gtk_container_add(GTK_CONTAINER(scrolledwindow), view);
	gtk_container_add(GTK_CONTAINER(vbox), scrolledwindow);

	bb = sc_bbox2(&button_no, bsub,
			gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click);
	gtk_box_pack_start(GTK_BOX(vbox), bb, FALSE, FALSE, 0);

	sc_table_attach2(GTK_TABLE(table), vbox);
	gtk_widget_show_all(table);

	gtk_main();

	sc_bbox2_remove(bsub);
	gtk_widget_destroy(vbox);

	return 0;
}
