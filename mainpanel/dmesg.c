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
#include <errno.h>
#include "common.h"

static void dmesg_view(GtkTextBuffer *buffer)
{
	int fd;
	struct stat st;
	char *p;
	int rc;

	gtk_text_buffer_set_text(buffer, "", 0);

	system("dmesg > /tmp/sc_dmesg.txt");

	sleep(1);
	if ((fd = open("/tmp/sc_dmesg.txt", O_RDONLY)) < 0) {
		gtk_text_buffer_set_text(buffer, "dmesg open error, try again.", -1);
		return;
	}
	fstat(fd, &st);
	p = (char*)malloc(st.st_size + 1);
	SYSCALL(rc = read(fd, p, st.st_size));
	close(fd);
	if (rc != st.st_size) {
		gtk_text_buffer_set_text(buffer, "dmesg read error, try again.", -1);
		free(p);
		return;
	}
	printf("dmesg %d bytes readn\n", rc);
	p[rc] = '\0';
	if (!g_utf8_validate(p, -1, NULL)) {
		gtk_text_buffer_set_text(buffer,
				"dmesg validate error, try again.", -1);
		free(p);
		return;
	}
	gtk_text_buffer_set_text(buffer, p, -1);
	free(p);
}

int dmesg_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *vbox, *view, *scrolledwindow;
	GtkWidget *bb;
	GtkTextBuffer *buffer;
	PangoFontDescription *font_desc;

	vbox = gtk_vbox_new(FALSE, 10);
	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	view = gtk_text_view_new();
	font_desc = pango_font_description_from_string("DejaVu Sans Mono bold 10");
	gtk_widget_modify_font(view, font_desc);
	pango_font_description_free(font_desc);
	sc_gtk_text_view_clear(GTK_TEXT_VIEW(view));
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	dmesg_view(buffer);
	gtk_container_add(GTK_CONTAINER(scrolledwindow), view);
	gtk_container_add(GTK_CONTAINER(vbox), scrolledwindow);

	bb = sc_bbox2(&button_no, bsub,
			gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(vbox), bb, FALSE, FALSE, 0);

	sc_table_attach2(GTK_TABLE(table), vbox);
	gtk_widget_show_all(table);

	gtk_main();

	sc_bbox2_remove(bsub);
	gtk_widget_destroy(vbox);

	return 0;
}
