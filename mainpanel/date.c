#include <memory.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "common.h"


struct updown ud_year;
struct updown ud_month;
struct updown ud_day;
struct updown ud_hour;
struct updown ud_minute;
struct updown ud_second;

struct updown ud_serial1;
struct updown ud_serial2;
struct updown ud_serial3;
struct updown ud_serial4;
struct updown ud_serial5;
struct updown ud_serial6;
struct updown ud_serial7;
struct updown ud_serial8;
struct updown ud_serial9;
struct updown ud_serial10;
GtkWidget *label_serialid1;
GtkWidget *label_serialid2;

char defaultid[16];

static void updown_event_day_adjust(struct updown *ud)
{
	int month_table[]  = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int month_table2[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	if ((ud_year.value%4) == 0) {
		set_updown_max(&ud_day, month_table2[ud_month.value]);
	} else {
		set_updown_max(&ud_day, month_table[ud_month.value]);
	}
}


static void btn_date_ok_clicked(GtkWidget *button, gpointer user_data)
{
	struct tm *next_tm;
	struct timespec tp;
	
	clock_gettime(CLOCK_REALTIME, &tp);
	next_tm = localtime(&tp.tv_sec);
	
	next_tm->tm_sec  = ud_second.value;
	next_tm->tm_min  = ud_minute.value;
	next_tm->tm_hour = ud_hour.value;
	next_tm->tm_mday = ud_day.value;
	next_tm->tm_mon  = ud_month.value - 1;
	next_tm->tm_year = ud_year.value - 1900;
	
	tp.tv_sec  = mktime(next_tm);
	tp.tv_nsec = 0;
	
	clock_settime(CLOCK_REALTIME, &tp);
}


char ntpdate_end_msg[256];
static char *exec_ntpdate(void)
{
	FILE *fp;
	char str[256];
	int ret;
	struct timespec tp;
	struct tm *cur_tm;
	
	ret = system("ntpdate ntp.nict.jp");
	if (ret == 0) {
		fp = popen("date", "r");
		if (fp == NULL) {
			return NULL;
		}
		
		memset(ntpdate_end_msg, 0, sizeof(ntpdate_end_msg));
		fread(str, sizeof(ntpdate_end_msg[0]), sizeof(ntpdate_end_msg), fp);
		pclose(fp);
		
		clock_gettime(CLOCK_REALTIME, &tp);
		cur_tm = localtime(&tp.tv_sec);
		
		set_updown_value(&ud_second, cur_tm->tm_sec);
		set_updown_value(&ud_minute, cur_tm->tm_min);
		set_updown_value(&ud_hour,   cur_tm->tm_hour);
		set_updown_value(&ud_day,    cur_tm->tm_mday);
		set_updown_value(&ud_month,  cur_tm->tm_mon+1);
		set_updown_value(&ud_year,   cur_tm->tm_year+1900);
		
		sprintf(ntpdate_end_msg, "online setup complete : %s", str);
	} else {
		sprintf(ntpdate_end_msg, "online setup failure");
	}
	
	return ntpdate_end_msg;
}


static void btn_date_net_set_clicked(GtkWidget *button, gpointer user_data)
{
	sc_wait_dialog("Please wait for a minute ...\n", "OK", exec_ntpdate);
}


static void btn_date_default_clicked(GtkWidget *button, gpointer user_data)
{
	struct timespec tp;
	struct tm *cur_tm;
	
	clock_gettime(CLOCK_REALTIME, &tp);
	cur_tm = localtime(&tp.tv_sec);
	
	set_updown_value(&ud_second, cur_tm->tm_sec);
	set_updown_value(&ud_minute, cur_tm->tm_min);
	set_updown_value(&ud_hour,   cur_tm->tm_hour);
	set_updown_value(&ud_day,    cur_tm->tm_mday);
	set_updown_value(&ud_month,  cur_tm->tm_mon+1);
	set_updown_value(&ud_year,   cur_tm->tm_year+1900);
}


static void btn_serialid_ok_clicked(GtkWidget *button, gpointer user_data)
{
printf("[%s][%s][%d]\n",__FILE__,__FUNCTION__,__LINE__);
	char buf[16];

	sprintf(buf, "F%d%d%cD%c%d%d%d%d%d%d\n",
		get_updown_value(&ud_serial1),
		get_updown_value(&ud_serial2),

		itoc(get_updown_value(&ud_serial3), 1),
		itoc(get_updown_value(&ud_serial4), 2),

		get_updown_value(&ud_serial5),
		get_updown_value(&ud_serial6),
		get_updown_value(&ud_serial7),
		get_updown_value(&ud_serial8),
		get_updown_value(&ud_serial9),
		get_updown_value(&ud_serial10)
		);
	set_serial_id(buf);
}


static void btn_serialid_default_clicked(GtkWidget *button, gpointer user_data)
{

printf("[%s][%s][%d]\n",__FILE__,__FUNCTION__,__LINE__);
	set_updown_value(&ud_serial1, defaultid[1]-'0');
	set_updown_value(&ud_serial2, defaultid[2]-'0');
	
	set_updown_value(&ud_serial3, ctoi(defaultid[3], 1));
	set_updown_value(&ud_serial4, ctoi(defaultid[5], 2));
	
	set_updown_value(&ud_serial5, defaultid[6]-'0');
	set_updown_value(&ud_serial6, defaultid[7]-'0');
	set_updown_value(&ud_serial7, defaultid[8]-'0');
	set_updown_value(&ud_serial8, defaultid[9]-'0');
	set_updown_value(&ud_serial9, defaultid[10]-'0');
	set_updown_value(&ud_serial10, defaultid[11]-'0');
}

#define SERIAL_ID_FILE "/mnt1/var/serialid"
int get_serial_id(char *buf, int size)
{
	FILE *fp;
	
	fp = fopen(SERIAL_ID_FILE, "r");
	if (fp == NULL) {
		return 1;
	}
	
	fgets(buf, size, fp);
	
	fclose(fp);

	return 0;
}


int set_serial_id(char *buf)
{
	FILE *fp;
	
	fp = fopen(SERIAL_ID_FILE, "w");
	if (fp == NULL) {
		return 1;
	}
	
	fputs(buf, fp);
	
	fclose(fp);
	
	return 0;
}


#define DATE_PLUS_CAPTION  "   +   "
#define DATE_MINUS_CAPTION "-"
#define ID_PLUS_CAPTION    "  +  "
#define ID_MINUS_CAPTION   "-"

//
//	a menu of date & serial ID
//
int date_main(GtkWidget *table, GtkWidget *bsub)
{
	gfloat xalign = 0;
	gfloat yalign = 0;
	gfloat xscale = 1;
	gfloat yscale = 0;
	int accl1 = 400;
	int accl2 = 40;
	int st_exit=0;
	char *plus;
	char *minus;
	struct tm *cur_tm;
	struct timespec tp;
	int ret;
	
	GtkWidget *v1;
	GtkWidget *bb;
	
	GtkWidget *vbox_date_all;
	GtkWidget *hbox_date;
	GtkWidget *hbox_date_btn;
	GtkWidget *frame_date;
	GtkWidget *btn_date_ok;
	GtkWidget *btn_date_net_set;
	GtkWidget *btn_date_default_set;
	GtkWidget *a_date;
	
	GtkWidget *vbox_serialid_all;
	GtkWidget *hbox_serialid;
	GtkWidget *hbox_serialid_btn;
	GtkWidget *frame_serialid;
	GtkWidget *btn_serialid_ok;
	GtkWidget *btn_serialid_default_set;
	GtkWidget *a_serialid;
	
	GdkColor color;
	GtkWidget *event_box1;
	
	// 時刻設定のボックス
	hbox_date = gtk_hbox_new(FALSE, 10);
	
	// 時刻設定
	plus  = DATE_PLUS_CAPTION;
	minus = DATE_MINUS_CAPTION;
	clock_gettime(CLOCK_REALTIME, &tp);
	cur_tm = localtime(&tp.tv_sec);
	create_updown(&ud_year,     "Year", plus, minus, cur_tm->tm_year+1900, 1970, 2038, accl1, accl2, xalign, yalign, xscale, yscale, updown_event_day_adjust, 0);
	gtk_box_pack_start(GTK_BOX(hbox_date), ud_year.body,   TRUE, TRUE, 0);
	create_updown(&ud_month,   "Month", plus, minus, cur_tm->tm_mon+1,        1,   12, accl1, accl2, xalign, yalign, xscale, yscale, updown_event_day_adjust, 0);
	gtk_box_pack_start(GTK_BOX(hbox_date), ud_month.body,  TRUE, TRUE, 0);
	create_updown(&ud_day,       "Day", plus, minus, cur_tm->tm_mday,         1,   31, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_date), ud_day.body,    TRUE, TRUE, 0);
	create_updown(&ud_hour,     "Hour", plus, minus, cur_tm->tm_hour,         0,   23, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_date), ud_hour.body,   TRUE, TRUE, 0);
	create_updown(&ud_minute, "Minute", plus, minus, cur_tm->tm_min,          0,   59, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_date), ud_minute.body, TRUE, TRUE, 0);
	create_updown(&ud_second, "Second", plus, minus, cur_tm->tm_sec,          0,   59, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_date), ud_second.body, TRUE, TRUE, 0);
	
	hbox_date_btn = gtk_hbox_new(FALSE, 10);
	
	// デフォルトボタン
	btn_date_default_set = gtk_button_new_with_label("Get System Date");
	g_signal_connect(G_OBJECT(btn_date_default_set), "clicked", G_CALLBACK(btn_date_default_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(hbox_date_btn), btn_date_default_set, TRUE, TRUE, 0);
	
	// ntpdateボタン
	btn_date_net_set = gtk_button_new_with_label("Online Setup");
	g_signal_connect(G_OBJECT(btn_date_net_set), "clicked", G_CALLBACK(btn_date_net_set_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(hbox_date_btn), btn_date_net_set, TRUE, TRUE, 0);
	
	// 設定ボタン
	btn_date_ok = gtk_button_new_with_label("  Set Date  ");
	g_signal_connect(G_OBJECT(btn_date_ok), "clicked", G_CALLBACK(btn_date_ok_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(hbox_date_btn), btn_date_ok, TRUE, TRUE, 0);
	
	vbox_date_all = gtk_vbox_new(FALSE, 1);
	gtk_box_pack_start(GTK_BOX(vbox_date_all), hbox_date, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_date_all), hbox_date_btn, TRUE, TRUE, 0);
	
	a_date = gtk_alignment_new(0.5, 0, 0.9, 0.8);
	gtk_container_add(GTK_CONTAINER(a_date), vbox_date_all);
	
	frame_date = gtk_frame_new("Date Configuration");
	gtk_container_add(GTK_CONTAINER(frame_date), a_date);
	
	
	// シリアルIDのボックス
	hbox_serialid = gtk_hbox_new(FALSE, 15);
	
	// シリアルID取得
	memset(defaultid, '0', sizeof(defaultid));
	get_serial_id(defaultid, sizeof(defaultid));
	ret = strlen(defaultid);  // FXXXDXXXXXXX
	if (ret != 13) {
		memset(defaultid, '0', sizeof(defaultid));
	}
	
	// シリアルID
	plus  = ID_PLUS_CAPTION;
	minus = ID_MINUS_CAPTION;
	label_serialid1 = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(label_serialid1), "<b>F</b>");
	gtk_box_pack_start(GTK_BOX(hbox_serialid), label_serialid1, TRUE, TRUE, 0);
	create_updown(&ud_serial1, NULL, plus, minus, defaultid[1]-'0', 1, 2, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial1.body, TRUE, TRUE, 0);
	create_updown(&ud_serial2, NULL, plus, minus, defaultid[2]-'0', 0, 9, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial2.body, TRUE, TRUE, 0);
	
	create_updown(&ud_serial3, NULL, plus, minus, ctoi((int)defaultid[3], 1), 1, 12, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 1);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial3.body, TRUE, TRUE, 0);

	label_serialid2 = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(label_serialid2), "<b>D</b>");
	gtk_box_pack_start(GTK_BOX(hbox_serialid), label_serialid2, TRUE, TRUE, 0);

	create_updown(&ud_serial4, NULL, plus, minus, ctoi((int)defaultid[5], 2), 0, 3, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 2);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial4.body, TRUE, TRUE, 0);

	create_updown(&ud_serial5, NULL, plus, minus, defaultid[6]-'0', 0, 9, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial5.body, TRUE, TRUE, 0);
	create_updown(&ud_serial6, NULL, plus, minus, defaultid[7]-'0', 0, 9, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial6.body, TRUE, TRUE, 0);
	create_updown(&ud_serial7, NULL, plus, minus, defaultid[8]-'0', 0, 9, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial7.body, TRUE, TRUE, 0);
	create_updown(&ud_serial8, NULL, plus, minus, defaultid[9]-'0', 0, 9, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial8.body, TRUE, TRUE, 0);
	create_updown(&ud_serial9, NULL, plus, minus, defaultid[10]-'0', 0, 9, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial9.body, TRUE, TRUE, 0);
	create_updown(&ud_serial10, NULL, plus, minus, defaultid[11]-'0', 0, 9, accl1, accl2, xalign, yalign, xscale, yscale, NULL, 0);
	gtk_box_pack_start(GTK_BOX(hbox_serialid), ud_serial10.body, TRUE, TRUE, 0);
	
	hbox_serialid_btn = gtk_hbox_new(FALSE, 10);
	
	// デフォルトボタン
	btn_serialid_default_set = gtk_button_new_with_label("Default Serial ID");
	g_signal_connect(G_OBJECT(btn_serialid_default_set), "clicked", G_CALLBACK(btn_serialid_default_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(hbox_serialid_btn), btn_serialid_default_set, TRUE, TRUE, 0);
	
	// 設定ボタン
	btn_serialid_ok = gtk_button_new_with_label("Set Serial ID");
	g_signal_connect(G_OBJECT(btn_serialid_ok), "clicked", G_CALLBACK(btn_serialid_ok_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(hbox_serialid_btn), btn_serialid_ok, TRUE, TRUE, 0);
	
	vbox_serialid_all = gtk_vbox_new(FALSE, 1);
	gtk_box_pack_start(GTK_BOX(vbox_serialid_all), hbox_serialid, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_serialid_all), hbox_serialid_btn, TRUE, TRUE, 0);
	
	a_serialid = gtk_alignment_new(0.5, 0, 0, 0.8);
	gtk_container_add(GTK_CONTAINER(a_serialid), vbox_serialid_all);
	
	frame_serialid = gtk_frame_new("serial ID configuration");
	gtk_container_add(GTK_CONTAINER(frame_serialid), a_serialid);
	
	// バッテリー
	bb = sc_bbox2(&st_exit, bsub, gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click);
	
	// 全体のボックス
	v1 = gtk_vbox_new(FALSE, 5);
	
	gtk_container_add(GTK_CONTAINER(v1), frame_date);
	gtk_container_add(GTK_CONTAINER(v1), frame_serialid);
	gtk_box_pack_start(GTK_BOX(v1), bb, FALSE, FALSE, 0);
	
	sc_table_attach2(GTK_TABLE(table), v1);
	gtk_widget_show_all(v1);
	
	gtk_main();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v1);
	
	return 0;
}

