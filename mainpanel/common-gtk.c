#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

#include "common.h"

GtkTreeIter iter_battery, iter_audio;
GtkWidget *tv_sidemenu, *lb_menutop;


//
//	common gtk functions
//

void sc_gtk_update()
{
	while(gtk_events_pending()==TRUE) gtk_main_iteration();
}

void sc_gtk_update_n(int n)
{
	int i;
	
	for(i=0;i<n&&gtk_events_pending()==TRUE;i++)
		gtk_main_iteration();
}

void sc_bbox1_click(GtkWidget *widget, gpointer data)
{
	if(data) *((int *)data)=1;
	gtk_main_quit();
}

void sc_bbox1_click_noquit(GtkWidget *widget, gpointer data)
{
	if(data) *((int *)data)=1;
}

GtkWidget *sc_bbox1(int *st, GtkWidget *b, void (*callback)(GtkWidget *, gpointer))
{
	GtkWidget *f, *bb;

	f=gtk_frame_new(NULL);
	bb=gtk_hbutton_box_new();
	gtk_container_set_border_width(GTK_CONTAINER(bb), 5);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bb), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing(bb, 20);
	gtk_button_box_set_child_size(GTK_BUTTON_BOX(bb), 85, 20);
	gtk_container_add(GTK_CONTAINER(f), bb);
    gtk_container_add(GTK_CONTAINER(bb), b);
    
    if(st)*st=0;
	g_signal_connect(G_OBJECT(b), "clicked", G_CALLBACK(callback), (gpointer)st);
	gtk_widget_set_usize(f, 160*0, 72);
	return f;
}

static GtkWidget *sc_bbox2_cur_hbox=NULL;

GtkWidget *sc_bbox2(int *st, GtkWidget *bsub, GtkWidget *b_quit, void (*callback)(GtkWidget *, gpointer))
{
	GtkWidget *f, *bb, *h;

	f=gtk_frame_new(NULL);
	bb=gtk_hbutton_box_new();
	gtk_container_set_border_width(GTK_CONTAINER(bb), 5);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bb), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing(bb, 20);
	gtk_button_box_set_child_size(GTK_BUTTON_BOX(bb), 85, 20);
    gtk_container_add(GTK_CONTAINER(bb), b_quit);
	
	sc_bbox2_cur_hbox=h=gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(h), bsub);
	gtk_container_add(GTK_CONTAINER(h), bb);
	gtk_container_add(GTK_CONTAINER(f), h);
    
    if(st)*st=0;
	g_signal_connect(G_OBJECT(b_quit), "clicked", G_CALLBACK(callback), (gpointer)st);
	gtk_widget_set_usize(f, 160*0, 72);
	return f;
}

void sc_bbox2_remove(GtkWidget *bsub)
{
	gtk_container_remove(GTK_CONTAINER(sc_bbox2_cur_hbox), bsub);
}

void sc_message(GtkMessageType type, GtkButtonsType button, const char *fmt, ...)
{
	char *s;
	GtkWidget *d;
	va_list ap;
	va_start(ap, fmt);
	
	s=malloc(strlen(fmt)*4);
	vsprintf(s, fmt, ap);
	d=gtk_message_dialog_new((GtkWindow *)NULL, GTK_DIALOG_DESTROY_WITH_PARENT, type, button, s);
	g_signal_connect_swapped(d, "response", G_CALLBACK(gtk_widget_destroy), d);
	gtk_window_set_position(GTK_WINDOW(d), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_dialog_run(GTK_DIALOG(d));
	free(s);
}

static int sc_message_select_stat;

void sc_message_select_press(GtkWidget *widget, gpointer data)
{
	sc_message_select_stat=(int)data;
	gtk_main_quit();
}

int sc_message_yesno(const char *fmt, ...)
{
	GtkWidget *w, *lb, *b0, *b1, *img;
	GtkWidget *a0, *a1, *a2, *v0, *h0, *h1;
	char *s;
	va_list ap;
	va_start(ap, fmt);
	
	s=malloc(strlen(fmt)*4);
	vsprintf(s, fmt, ap);
	
	img=gtk_image_new_from_stock(GTK_STOCK_INFO, GTK_ICON_SIZE_LARGE_TOOLBAR);
	lb=gtk_label_new(s);
	h0=gtk_hbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(h0), img);
	gtk_container_add(GTK_CONTAINER(h0), lb);
	
	b0=gtk_button_new_from_stock(GTK_STOCK_YES);
	b1=gtk_button_new_from_stock(GTK_STOCK_NO);
	h1=gtk_hbox_new(FALSE, 40);
	gtk_container_add(GTK_CONTAINER(h1), b0);
	g_signal_connect(b0, "clicked", G_CALLBACK(sc_message_select_press), (gpointer)1);
	gtk_container_add(GTK_CONTAINER(h1), b1);
	g_signal_connect(b1, "clicked", G_CALLBACK(sc_message_select_press), (gpointer)2);
	sc_message_select_stat=0;
	
	v0=gtk_vbox_new(FALSE, 10);
	a1=gtk_alignment_new(0.5, 0.5, 0.2, 0.2);
	gtk_container_add(GTK_CONTAINER(a1), h0);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	
	a2=gtk_alignment_new(0.5, 0.5, 0.8, 0.5);
	gtk_container_add(GTK_CONTAINER(a2), h1);
	gtk_container_add(GTK_CONTAINER(v0), a2);
	
	a0=gtk_alignment_new(0.5, 0.5, 0.3, 0.1);
	gtk_container_add(GTK_CONTAINER(a0), v0);
	
	w=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_add(GTK_CONTAINER(w), a0);
	gtk_widget_set_usize(w, get_sc_window_width(), get_sc_window_height());
	gtk_window_set_position(GTK_WINDOW(w), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_widget_show_all(w);
	gtk_main();
	gtk_widget_destroy(w);
	
	//debug_printf(3, "sc_message_select_stat:%d¥n", sc_message_select_stat);
	free(s);
	
	return sc_message_select_stat;
}


GList *list = NULL;
void sc_gtk_text_view_insert_ping(GtkTextView *tv, char *buf, int n)
{
	GtkTextIter   it;
	GtkTextIter   it0;
	GtkTextIter   it1;
	GtkTextBuffer *tb;
	GList         *l;
	guint len;

	if (tv == NULL) {
		if (list != NULL) {
			g_list_foreach(list, (GFunc)g_free, NULL);
			g_list_free(list);
			list = NULL;
			g_print("%s() clear\n",__func__);
		}
		return;
	}

	list = g_list_append(list, g_strndup((gchar*)buf,n));
	len = g_list_length(list);
	if (len >= 7) {
		list=g_list_delete_link(list, g_list_first(list));
	}
	
	tb = gtk_text_view_get_buffer(tv);
	gtk_text_buffer_get_start_iter(tb, &it0);
	gtk_text_buffer_get_end_iter(tb, &it1);
	gtk_text_buffer_delete(tb, &it0, &it1);
	
	for (l=list; l; l=g_list_next(l)) {
		gtk_text_buffer_get_end_iter(tb, &it);
		gtk_text_buffer_insert(tb, &it, l->data, strlen(l->data));
	}
}


void sc_gtk_text_view_insert(GtkTextView *tv, char *buf, int n)
{
	GtkTextIter   it, sit, eit;
	GtkTextBuffer *tb;
	GtkTextMark   *tm;
	
	tb = gtk_text_view_get_buffer(tv);
	gtk_text_buffer_get_end_iter(tb, &it);
	
	gtk_text_buffer_insert(tb, &it, buf, n);
	gtk_text_buffer_get_end_iter(tb, &it);
	tm=gtk_text_buffer_get_insert(tb);
	gtk_text_buffer_place_cursor(tb, &it);
	gtk_text_view_scroll_to_mark(tv, tm, 0, FALSE, 0, 1.0);

	if(gtk_text_buffer_get_line_count(tb)>100){
		gtk_text_buffer_get_start_iter(tb, &sit);
		eit = sit;
		gtk_text_iter_forward_line(&eit);
		gtk_text_buffer_delete(tb, &sit, &eit);
	}

}


void sc_gtk_text_view_append(GtkTextView *tv, char *s)
{
	sc_gtk_text_view_insert(tv, s, strlen(s));
}

void sc_gtk_text_view_clear(GtkTextView *tv)
{
	GtkTextIter it0, it1;
	GtkTextBuffer *tb;
	GtkTextMark *tm;

	tb=gtk_text_view_get_buffer(tv);
	gtk_text_buffer_get_start_iter(tb, &it0);
	gtk_text_buffer_get_end_iter(tb, &it1);
	gtk_text_buffer_delete(tb, &it0, &it1);
	tm=gtk_text_buffer_get_insert(tb);
	gtk_text_view_scroll_mark_onscreen(tv, tm);
}


GtkWidget *sc_table_new()
{
	return gtk_table_new(1, 100, TRUE);
}


void sc_table_attach1(GtkTable *table, GtkWidget *widget)
{
	gtk_table_attach_defaults(table, widget, 0, 22, 0, 1);
}


void sc_table_attach2(GtkTable *table, GtkWidget *widget)
{
	gtk_table_attach_defaults(table, widget, 23, 100, 0, 1);
}


/**
 *	警告ダイアログクローズイベント
 */
static void btn_msg_dlg_close(GtkWidget *button, gpointer user_data)
{
	GtkWidget *window = (GtkWidget*)user_data;
	gtk_widget_destroy(window);
}


/**
 *	警告ダイアログ
 */
void sc_message_dialog(char *msg, char *btnlabel, gboolean ismarkup)
{
	GtkWidget *window;
	GtkWidget *frame;
	GtkWidget *label;
	GtkWidget *btn_close;
	GtkWidget *table;
	
	// ウインドウ作成
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_resize(GTK_WINDOW(window), 600, 250);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	
	// 装飾枠
	frame = gtk_frame_new("");
	gtk_frame_set_label_align(GTK_FRAME(frame), 0.0, 0.0);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
	gtk_container_add(GTK_CONTAINER(window), frame);
	
	// 配置用テーブル
	table = gtk_table_new(10, 30, TRUE);
	gtk_container_add(GTK_CONTAINER(frame), table);
	
	// ラベル
	label = gtk_label_new(msg);
	if (ismarkup == TRUE) {
		gtk_label_set_markup(GTK_LABEL(label), msg);
	} else {
		gtk_label_set_text(GTK_LABEL(label), msg);
	}
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 28, 1, 4);
	
	// ボタン
	btn_close = gtk_button_new_with_label(btnlabel);
	g_signal_connect(G_OBJECT(btn_close), "clicked", G_CALLBACK(btn_msg_dlg_close), window);
	gtk_table_attach_defaults(GTK_TABLE(table), btn_close, 21, 29, 5, 9);
	
	gtk_widget_show_all(window);
}


/**
 *	待ちダイアログクローズイベント
 */
static void btn_wate_dlg_close(GtkWidget *button, gpointer user_data)
{
	GtkWidget *window = (GtkWidget*)user_data;
	gtk_widget_destroy(window);
}


GtkWidget *wait_dlg_btn_close;
GtkWidget *wait_dlg_spinner;
GtkWidget *wait_dlg_label;
char wait_dlg_end_msg[256];

gboolean my_useraction(gpointer point)
{
	char *str = (char*)point;
	
	gtk_spinner_stop(GTK_SPINNER(wait_dlg_spinner));
	gtk_widget_set_sensitive(wait_dlg_btn_close, TRUE);
	gtk_label_set_text(GTK_LABEL(wait_dlg_label), str);
	
	return FALSE;
}

void *th_my_useraction(void *data)
{
	char *(*event)(void) = (char *(*)(void))data;
	char *str;
	
	str = event();
	g_idle_add_full(G_PRIORITY_HIGH, my_useraction, str, NULL);
	return NULL;
}

/**
 *	待ちダイアログ
 */
void sc_wait_dialog(char *msg, char *btnlabel, char *(*event)(void))
{
	GtkWidget *window;
	GtkWidget *frame;
	GtkWidget *table;
	pthread_t th;
	
	// ウインドウ作成
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_resize(GTK_WINDOW(window), 700, 250);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	
	// 装飾枠
	frame = gtk_frame_new("");
	gtk_frame_set_label_align(GTK_FRAME(frame), 0.0, 0.0);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
	gtk_container_add(GTK_CONTAINER(window), frame);
	
	// 配置用テーブル
	table = gtk_table_new(10, 30, TRUE);
	gtk_container_add(GTK_CONTAINER(frame), table);
	
	
	wait_dlg_spinner = gtk_spinner_new();
	gtk_table_attach_defaults(GTK_TABLE(table), wait_dlg_spinner, 10, 20, 0, 2);
	
	// ラベル
	wait_dlg_label = gtk_label_new(msg);
	gtk_label_set_text(GTK_LABEL(wait_dlg_label), msg);
	gtk_table_attach_defaults(GTK_TABLE(table), wait_dlg_label, 1, 29, 3, 5);
	
	// ボタン
	wait_dlg_btn_close = gtk_button_new_from_stock (GTK_STOCK_OK);
	gtk_button_set_label(GTK_BUTTON(wait_dlg_btn_close), btnlabel);
	g_signal_connect(G_OBJECT(wait_dlg_btn_close), "clicked", G_CALLBACK(btn_wate_dlg_close), window);
	gtk_table_attach_defaults(GTK_TABLE(table), wait_dlg_btn_close, 21, 29, 5, 9);
	gtk_widget_set_sensitive(wait_dlg_btn_close, FALSE);
	
	gtk_widget_show_all(window);
	
	gtk_spinner_start(GTK_SPINNER(wait_dlg_spinner));
	pthread_create(&th, NULL, th_my_useraction, event);
}

