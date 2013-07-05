#include <gtk/gtk.h>
#include "common.h"
#include <string.h>

#define CH_TABLE1 "0123456789XYZ"
#define CH_TABLE2 "RJKL"

int ctoi(int value, int type)
{
	char *ch_table1 = CH_TABLE1;
	char *ch_table2 = CH_TABLE2;
	char *ret;

	if(type==1){
		ret = strchr(ch_table1, value);
		if(ret!=NULL){
			return ret - ch_table1;
		}
	}else if(type==2){
		ret = strchr(ch_table2, value);
		if(ret!=NULL){
			return ret - ch_table2;
		}
	}

	return 0;
}

char itoc(int value, int type)
{
	char *ch_table1 = CH_TABLE1;
	char *ch_table2 = CH_TABLE2;

	if(type==1){
		return ch_table1[value];
	}else if(type==2){
		return ch_table2[value];
	}

	return value;
}

static void updown_up(GtkWidget *button, gpointer user_data)
{
	struct updown *ud = (struct updown*)user_data;
	char str[16];
	
	ud->value++;
	if (ud->max < ud->value) {
		ud->value = ud->min;
	}

	if(ud->type==1){
		sprintf(str, "%c", itoc(ud->value, 1));
	}else if(ud->type==2){
		sprintf(str, "%c", itoc(ud->value, 2));
	}else{
		sprintf(str, "%d", ud->value);
	}

	gtk_label_set_text(GTK_LABEL(ud->valuelb), str);

	if (ud->ude) {
		ud->ude(ud);
	}
}


static gboolean timer_updown_up(gpointer point)
{
	struct updown *ud = (struct updown*)point;
	
	g_source_remove(ud->up_timer_tag);
	ud->up_timer_tag = g_timeout_add(ud->accl2, timer_updown_up, ud);
	
	updown_up(ud->upbtn, point);
	return TRUE;
}


static void updown_upbtn_pressed(GtkWidget *button, gpointer user_data)
{
	struct updown *ud = (struct updown*)user_data;
	
	updown_up(button, user_data);
	ud->up_timer_tag = g_timeout_add(ud->accl1, timer_updown_up, user_data);
}


static void updown_upbtn_leave(GtkWidget *button, gpointer user_data)
{
	struct updown *ud = (struct updown*)user_data;
	
	if (ud->up_timer_tag != 0) {
		g_source_remove(ud->up_timer_tag);
		ud->up_timer_tag = 0;
	}
}


static void updown_upbtn_released(GtkWidget *button, gpointer user_data)
{
	struct updown *ud = (struct updown*)user_data;
	
	if (ud->up_timer_tag != 0) {
		g_source_remove(ud->up_timer_tag);
		ud->up_timer_tag = 0;
	}
}


static void updown_down(GtkWidget *button, gpointer user_data)
{
	struct updown *ud = (struct updown*)user_data;
	char str[16];
	
	ud->value--;
	if (ud->min > ud->value) {
		ud->value = ud->max;
	}

	if(ud->type==1){
		sprintf(str, "%c", itoc(ud->value, 1));
	}else if(ud->type==2){
		sprintf(str, "%c", itoc(ud->value, 2));
	}else{
		sprintf(str, "%d", ud->value);
	}

	gtk_label_set_text(GTK_LABEL(ud->valuelb), str);
	
	if (ud->ude) {
		ud->ude(ud);
	}
}


static gboolean timer_updown_down(gpointer point)
{
	struct updown *ud = (struct updown*)point;
	
	g_source_remove(ud->down_timer_tag);
	ud->down_timer_tag = g_timeout_add(ud->accl2, timer_updown_down, ud);
	
	updown_down(ud->downbtn, point);
	
	return TRUE;
}


static void updown_downbtn_pressed(GtkWidget *button, gpointer user_data)
{
	struct updown *ud = (struct updown*)user_data;
	
	updown_down(button, user_data);
	ud->down_timer_tag = g_timeout_add(ud->accl1, timer_updown_down, user_data);
}


static void updown_downbtn_leave(GtkWidget *button, gpointer user_data)
{
	struct updown *ud = (struct updown*)user_data;
	
	if (ud->down_timer_tag != 0) {
		g_source_remove(ud->down_timer_tag);
		ud->down_timer_tag = 0;
	}
}


static void updown_downbtn_released(GtkWidget *button, gpointer user_data)
{
	struct updown *ud = (struct updown*)user_data;
	
	if (ud->down_timer_tag != 0) {
		g_source_remove(ud->down_timer_tag);
		ud->down_timer_tag = 0;
	}
}


int get_updown_value(struct updown *ud)
{
	return ud->value;
}


void set_updown_value(struct updown *ud, int value)
{
	char str[16];
	char ch;

	ud->value = value;
	if (ud->max < ud->value) {
		ud->value = ud->min;
	}
	if (ud->min > ud->value) {
		ud->value = ud->max;
	}


	if(ud->type==1){
		ch = itoc(ud->value, 1);
		sprintf(str, "%c", ch);
	}else if(ud->type==2){
		ch = itoc(ud->value, 2);
		sprintf(str, "%c", ch);
	}else{
		sprintf(str, "%d", ud->value);
	}

	gtk_label_set_text(GTK_LABEL(ud->valuelb), str);
}


void set_updown_max(struct updown *ud, int value)
{
	char str[16];
	
	ud->max = value;
	if (ud->max < ud->value) {
		ud->value = ud->max;
		sprintf(str, "%d", ud->value);
		gtk_label_set_text(GTK_LABEL(ud->valuelb), str);
	}
}


void set_updown_min(struct updown *ud, int value)
{
	char str[16];
	
	ud->min = value;
	if (ud->min > ud->value) {
		ud->value = ud->min;
		sprintf(str, "%d", ud->value);
		gtk_label_set_text(GTK_LABEL(ud->valuelb), str);
	}
}


void create_updown(struct updown *ud, char *caption, char *plus_caption, char *minus_caption,
	int defaultvalue, int min, int max, int accl1, int accl2,
	gfloat xalign, gfloat yalign, gfloat xscale, gfloat yscale, updown_event ude, int type)
{
	GdkColor color;
	char str[16];
	
	ud->vbox = gtk_vbox_new(FALSE, 0);
	
	if (caption != NULL) {
		ud->caption = gtk_label_new(caption);
		gtk_box_pack_start(GTK_BOX(ud->vbox), ud->caption, FALSE, FALSE, 0);
	}
	
	ud->upbtn = gtk_button_new_with_label(plus_caption);
	gtk_box_pack_start(GTK_BOX(ud->vbox), ud->upbtn, TRUE, TRUE, 0);
	g_signal_connect(G_OBJECT(ud->upbtn), "pressed",  G_CALLBACK(updown_upbtn_pressed),  ud);
	g_signal_connect(G_OBJECT(ud->upbtn), "leave",    G_CALLBACK(updown_upbtn_leave),    ud);
	g_signal_connect(G_OBJECT(ud->upbtn), "released", G_CALLBACK(updown_upbtn_released), ud);
	
	ud->event_box = gtk_event_box_new();
	gdk_color_parse("white", &color);
	gtk_widget_modify_bg(ud->event_box, GTK_STATE_NORMAL, &color);
	gtk_box_pack_start(GTK_BOX(ud->vbox), ud->event_box, TRUE, TRUE, 0);
	
	ud->value = defaultvalue;
	
	if(type==1 || type==2){
		sprintf(str, "%c", itoc(ud->value, type));
	}else{
		sprintf(str, "%d", ud->value);
	}
	
	ud->valuelb = gtk_label_new(str);
	gtk_container_add(GTK_CONTAINER(ud->event_box), ud->valuelb);
	
	ud->downbtn = gtk_button_new_with_label(minus_caption);
	gtk_box_pack_start(GTK_BOX(ud->vbox), ud->downbtn, TRUE, TRUE, 0);
	g_signal_connect(G_OBJECT(ud->downbtn), "pressed",  G_CALLBACK(updown_downbtn_pressed),  ud);
	g_signal_connect(G_OBJECT(ud->downbtn), "leave",    G_CALLBACK(updown_downbtn_leave),    ud);
	g_signal_connect(G_OBJECT(ud->downbtn), "released", G_CALLBACK(updown_downbtn_released), ud);
	
	ud->body = gtk_alignment_new(xalign, yalign, xscale, yscale);
	gtk_container_add(GTK_CONTAINER(ud->body), ud->vbox);
	
	ud->min = min;
	ud->max = max;
	ud->ude = ude;
	ud->accl1 = accl1;
	ud->accl2 = accl2;
	ud->up_timer_tag = 0;
	ud->down_timer_tag = 0;
	ud->type = type;
}
