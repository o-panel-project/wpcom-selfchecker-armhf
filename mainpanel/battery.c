/*
 *		Battery
 *
 *					2011.7	written by Omoikane Inc.
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
#include <sys/ioctl.h>
#include <gtk/gtk.h>
#include "common.h"
#include "wpcio.h"
#include "sc_battery.h"

#define LABEL_LOGGING_OFF "Logging : off"
#define LABEL_LOGGING_ON "Logging : on "

/*	20110728VACS	*/
#define LABEL_BAT1_CHARGE_OFF "bat1 charge : off"
#define LABEL_BAT1_CHARGE_ON "bat1 charge : on"
#define LABEL_BAT2_CHARGE_OFF "bat2 charge : off"
#define LABEL_BAT2_CHARGE_ON "bat2 charge : on"

static GtkWidget *lb_adc;
static GdkPixbuf *indicator_on, *indicator_off;
static GtkWidget *bat1fast, *bat1full, *bat1fault;
static GtkWidget *bat2fast, *bat2full, *bat2fault;
static GtkWidget *lb_logging, *b_start, *b_stop, *b_quit;

/* 20170110 wpc */
extern const guint8 indicator_on_inline[];
extern const guint8 indicator_off_inline[];
extern const guint8 indicator_on_small_inline[];
extern const guint8 indicator_off_small_inline[];

/*	20110729VACS	*/
static	GtkWidget	*lb_charge_b1, *b_charge_b1_on, *b_charge_b1_off;
static	GtkWidget	*lb_charge_b2, *b_charge_b2_on, *b_charge_b2_off;

/* 20170111 wpc */
extern int g_board_type;

static void set_img(GtkWidget *w, int is_on, GdkPixbuf *p_on, GdkPixbuf *p_off)
{
	gtk_image_set_from_pixbuf(GTK_IMAGE(w), is_on ? p_on : p_off);
}

static int bat1_stat, bat2_stat;
static int bat_v0, bat_v1, bat_v2;
static int bat_r0=-1, bat_r1=-1, bat_r2=-1, bat_r3=-1, bat_r4=-1;

// guard time for wpcio in usec.
//#define WPCIO_IOCTL_GUARD 5000
//#define WPCIO_CLOSE_GUARD 10000

static pthread_t bat_disp_thread;
static int bat_disp_thread_run = 0;


static void get_value()
{
#if 0
	int fd, dc, bat1, bat2;
	
	fd = wpcio_open(WPCIO_OPEN_RETRY, "battery");
	if (fd < 0) {
		return;
	}
	
	bat_r0 = ioctl(fd, WPC_GET_BAT1_CHARGING_STAT, &bat1_stat);
	if (bat_r0 < 0) {
		printf("Could not get battery 1 charging status, error code = %d\n", bat_r0);
	}
	usleep(WPCIO_IOCTL_GUARD);
	
	bat_r1 = ioctl(fd, WPC_GET_BAT2_CHARGING_STAT, &bat2_stat);
	if (bat_r1<0) {
		printf("Could not get battery 2 charging status, error code = %d\n", bat_r1);
	}
	usleep(WPCIO_IOCTL_GUARD);
	
	bat_r2 = ioctl(fd, WPC_GET_DC_LEVEL, &dc);
	if (bat_r2 < 0) {
		printf("Could not get adc value(0), error code = %d\n", bat_r2);
	}
	
	bat_r3 = ioctl(fd, WPC_GET_BAT1_LEVEL, &bat1);
	if (bat_r3 < 0) {
		printf("Could not get adc value(1), error code = %d\n", bat_r3);
	}
	
	bat_r4 = ioctl(fd, WPC_GET_BAT2_LEVEL, &bat2);
	if (bat_r4 < 0) {
		printf("Could not get adc value(2), error code = %d\n", bat_r4);
	}
	
	close(fd);
	
	usleep(WPCIO_CLOSE_GUARD);
	
	bat_v0 = dc   * (DC_RL + DC_RH)   / DC_RL;
	bat_v1 = bat1 * (BAT_RL + BAT_RH) / BAT_RL;
	bat_v2 = bat2 * (BAT_RL + BAT_RH) / BAT_RL;

#endif
	sc_get_battery_status("battery", &bat_v0, &bat_v1, &bat1_stat, &bat_v2, &bat2_stat);
	bat_r0 = bat1_stat;
	bat_r1 = bat2_stat;
	bat_r2 = bat_v0;
	bat_r3 = bat_v1;
	bat_r4 = bat_v2;
	if (g_board_type != WPC_BOARD_TYPE_J) {
		bat_v2 = sc_get_cradle_battery_level();
		if (bat_v2 == CRADLE_BAT_OPEN_ERROR || bat_v2 == CRADLE_BAT_MODE_ERROR){
			/* cradle detach? */
			bat_v2 = -1;
		} else {
			if (bat_v2 < 500)	/* battery attach threshold 0.5V */
				bat_v2 = 0;
		}
	}
}


static void update_image_gen(GdkPixbuf *p_on, GdkPixbuf *p_off,
							GtkWidget *b1fast, GtkWidget *b1full, GtkWidget *b1fault, 
							GtkWidget *b2fast, GtkWidget *b2full, GtkWidget *b2fault)
{
	if(0<=bat_r0&&b1fast){
		set_img(b1fast, bat1_stat & WPC_CHARGING_FAST, p_on, p_off);
		set_img(b1full, bat1_stat & WPC_CHARGING_FULL, p_on, p_off);
		set_img(b1fault, bat1_stat & WPC_CHARGING_FAULT, p_on, p_off);
	}
	if (g_board_type != WPC_BOARD_TYPE_J)
		return;
	if(0<=bat_r1&&b2fast){
		set_img(b2fast, bat2_stat & WPC_CHARGING_FAST, p_on, p_off);
		set_img(b2full, bat2_stat & WPC_CHARGING_FULL, p_on, p_off);
		set_img(b2fault, bat2_stat & WPC_CHARGING_FAULT, p_on, p_off);
	}
}

#if 0	/* not use */
static void get_value_timer()
{
	char tmps[SMALL_STR];

	update_image_gen(indicator_on, indicator_off,
					bat1fast, bat1full, bat1fault, bat2fast, bat2full, bat2fault);
	if(0<=bat_r2&&0<=bat_r3&&0<=bat_r4&&lb_adc){
		sprintf(tmps, "DC level = %dmV\n", bat_v0);
		append_sprintf(tmps, "BAT1 level = %dmV\n", bat_v1);
		append_sprintf(tmps, "BAT2 level = %dmV\n", bat_v2);
		gtk_label_set_text(GTK_LABEL(lb_adc), tmps);
	}
}
#endif


gboolean get_value_thread_step(gpointer point)
{
	char tmps[SMALL_STR];
	
	update_image_gen(indicator_on, indicator_off,
		bat1fast, bat1full, bat1fault, bat2fast, bat2full, bat2fault);
	
	if (0<=bat_r2 && 0<=bat_r3 && 0<=bat_r4 && lb_adc) {
		sprintf(tmps, "DC level = %dmV\n", bat_v0);
		append_sprintf(tmps, "BAT1 level = %dmV\n", bat_v1);
		if (g_board_type == WPC_BOARD_TYPE_J) {
		append_sprintf(tmps, "BAT2 level = %dmV\n", bat_v2);
		} else {
			append_sprintf(tmps, "BatteryCradle level = %dmV\n", bat_v2);
		}
		gtk_label_set_text(GTK_LABEL(lb_adc), tmps);
	}
	
	return FALSE;
}


static void *get_value_thread(void *data)
{
//	char tmps[SMALL_STR];

	while (bat_disp_thread_run) {
/*
		gdk_threads_enter();
		update_image_gen(indicator_on, indicator_off,
			bat1fast, bat1full, bat1fault, bat2fast, bat2full, bat2fault);
		if(0<=bat_r2&&0<=bat_r3&&0<=bat_r4&&lb_adc){
			sprintf(tmps, "DC level = %dmV\n", bat_v0);
			append_sprintf(tmps, "BAT1 level = %dmV\n", bat_v1);
			append_sprintf(tmps, "BAT2 level = %dmV\n", bat_v2);
			gtk_label_set_text(GTK_LABEL(lb_adc), tmps);
		}
		gdk_threads_leave();
*/
		usleep(1000*1000);
		g_idle_add_full(G_PRIORITY_HIGH, get_value_thread_step, NULL, NULL);
	}
	
	return NULL;
}

static int setup_img()
{
	char tmps[SMALL_STR];
	
	indicator_on = gdk_pixbuf_new_from_inline(
			-1, indicator_on_inline, FALSE, NULL);
	if(!indicator_on){
		printf("Could not open %s\n", tmps);
		return 1;
	}
	
	indicator_off = gdk_pixbuf_new_from_inline(
			-1, indicator_off_inline, FALSE, NULL);
	if(!indicator_off){
		printf("Could not open %s\n", tmps);
		return 1;
	}
	
	g_object_ref(indicator_on);
	g_object_ref(indicator_off);
	bat1fast=gtk_image_new_from_pixbuf(indicator_off);
	bat1full=gtk_image_new_from_pixbuf(indicator_off);
	bat1fault=gtk_image_new_from_pixbuf(indicator_off);
	if (g_board_type == WPC_BOARD_TYPE_J) {
	bat2fast=gtk_image_new_from_pixbuf(indicator_off);
	bat2full=gtk_image_new_from_pixbuf(indicator_off);
	bat2fault=gtk_image_new_from_pixbuf(indicator_off);
	} else {
		bat2fast = NULL;
		bat2full = NULL;
		bat2fault = NULL;
	}
	
	return 0;
}

#if 0	/* not use */
static void start_timer()
{
	struct itimerval itimer;
	struct sigaction sa = {
		.sa_sigaction = get_value_timer,
		.sa_flags = SA_RESTART | SA_SIGINFO,
	};

	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
	
	itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = 1;
	itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &itimer, NULL);
}

static void stop_timer()
{
	struct itimerval itimer;
	
	itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = 0;
	itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &itimer, NULL);
}
#endif

/*	20110729VACS	*/
static int press_charge_on(GtkWidget *widget, gpointer data)
{
int fd;
int err;
int bat_no;

	printf("battery charger on start = %d\n",(int)data);

	bat_no = (int)data % 2;

	fd=wpcio_open(WPCIO_OPEN_RETRY, "battery");
	if(fd<0){
		printf("BAT%d device open error = %d\n", bat_no + 1, fd);
		return 0;
	}

	switch(bat_no){
	case 0:
		/*	BAT1 charge on, GPIO65	*/
		err = ioctl(fd, WPC_SET_GPIO_OUTPUT_HIGH, 65);
		break;
	case 1:
		/*	BAT2 charge on, GPIO54	*/
		err = ioctl(fd, WPC_SET_GPIO_OUTPUT_HIGH, 54);
		break;
	default:
		printf("BAT select error = %d\n", bat_no);
		err = -1;
		break;
	}
	if (err < 0) {
		printf("BAT%d charge on, error code = %d\n", bat_no + 1, err);
	}	else	{
		printf("BAT%d charge on\n", bat_no + 1);

		switch(bat_no){
			case 0:
				/*	BAT1 charge on, GPIO65	*/
				pid_bat1_charge = 1;

				if((int)data == 0 || (int)data == 1){
					gtk_widget_set_sensitive(b_charge_b1_on, FALSE);
					gtk_widget_set_sensitive(b_charge_b1_off, TRUE);
					gtk_label_set_text(GTK_LABEL(lb_charge_b1), LABEL_BAT1_CHARGE_ON);
				}
				break;
			case 1:
				/*	BAT2 charge on, GPIO54	*/
				pid_bat2_charge = 1;

				if((int)data == 0 || (int)data == 1){
					gtk_widget_set_sensitive(b_charge_b2_on, FALSE);
					gtk_widget_set_sensitive(b_charge_b2_off, TRUE);
					gtk_label_set_text(GTK_LABEL(lb_charge_b2), LABEL_BAT2_CHARGE_ON);
				}
				break;
		}
		err = 1;
	}

	close(fd);
	return err;
}

/*	20110729VACS	*/
static int press_charge_off(GtkWidget *widget, gpointer data)
{
int fd;
int err;
int bat_no;

	printf("battery charger off start = %d\n",(int)data);

	bat_no = (int)data % 2;

	fd=wpcio_open(WPCIO_OPEN_RETRY, "battery");
	if(fd<0){
		printf("BAT%d device open error = %d\n", bat_no + 1, fd);
		return 0;
	}

	switch(bat_no){
	case 0:
		/*	BAT1 charge off, GPIO65	*/
		err = ioctl(fd, WPC_SET_GPIO_OUTPUT_LOW, 65);
		break;
	case 1:
		/*	BAT1 charge off, GPIO54	*/
		err = ioctl(fd, WPC_SET_GPIO_OUTPUT_LOW, 54);
		break;
	default:
		printf("BAT select error = %d\n", bat_no);
		err = -1;
		break;
	}
	if (err < 0) {
		printf("BAT%d charge off, error code = %d\n", bat_no + 1, err);
	}	else	{
		printf("BAT%d charge off\n", bat_no + 1);

		switch(bat_no){
			case 0:
				/*	BAT1 charge off, GPIO65	*/
				pid_bat1_charge = 0;

				if((int)data == 0 || (int)data == 1){
					gtk_widget_set_sensitive(b_charge_b1_on, TRUE);
					gtk_widget_set_sensitive(b_charge_b1_off, FALSE);
					gtk_label_set_text(GTK_LABEL(lb_charge_b1), LABEL_BAT1_CHARGE_OFF);
				}
				break;
			case 1:
				/*	BAT2 charge off, GPIO54	*/
				pid_bat2_charge = 0;

				if((int)data == 0 || (int)data == 1){
					gtk_widget_set_sensitive(b_charge_b2_on, TRUE);
					gtk_widget_set_sensitive(b_charge_b2_off, FALSE);
					gtk_label_set_text(GTK_LABEL(lb_charge_b2), LABEL_BAT2_CHARGE_OFF);
				}
				break;
		}
		err = 1;
	}

	close(fd);
	return err;
}

static int press_start(GtkWidget *widget, gpointer data)
{
	GtkTreeStore *store;
	char cmd[SMALL_STR], log[SMALL_STR];
	
	switch(pid_battery_logger=fork()){
	case 0:
		setpgid(getpid(),getpid());
		sprintf(cmd, "%s/%s", base_path, "bin/log-battery");
		sprintf(log, "%s/battery.log", log_dir);
		execl(cmd, "log-battery", log, NULL);
		_exit(127);
		break;
	}
	
	if(pid_battery_logger<0){
		sc_message(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
					"Could not start battery logger(1) : %s %s", cmd, log);
		pid_battery_logger=0;
		return 0;
	}
	
	usleep(100000);
	if(!have_proc_pid(pid_battery_logger)){
		sc_message(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
					"Could not start battery logger(2) : %s %s", cmd, log);
		pid_battery_logger=0;
		return 0;
	}
	
	gtk_widget_set_sensitive(b_start, FALSE);
	gtk_widget_set_sensitive(b_stop, TRUE);
	gtk_label_set_text(GTK_LABEL(lb_logging), LABEL_LOGGING_ON);
	
	store=GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tv_sidemenu)));
	gtk_tree_store_set(store, &iter_battery, COLUMN_LABEL, "Battery Level", COLUMN_NAME_COLOR, "Orange", -1);
	
//	start_timer();
	return 1;
}

static void press_stop(GtkWidget *widget, gpointer data)
{
	GtkTreeStore *store;
	
//	stop_timer();
	battery_check_logger_stop();
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_widget_set_sensitive(b_start, TRUE);
	gtk_label_set_text(GTK_LABEL(lb_logging), LABEL_LOGGING_OFF);
	
	store=GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tv_sidemenu)));
	gtk_tree_store_set(store, &iter_battery, COLUMN_LABEL, "Battery Level", COLUMN_NAME_COLOR, NULL, -1);
}

static void battery_main_interval_mode(GtkWidget *v0)
{
	time_t t_fin=0;

	if(!press_start(NULL,0)) return;
//	start_timer();
	gtk_widget_set_sensitive(v0, FALSE);
	t_fin=time(NULL)+testtime;
	while(time(NULL)<t_fin){
		sleep(1);
		sc_gtk_update();
	}
}

int battery_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	GtkWidget *v0, *bb;
	GtkWidget *a1;
	GtkWidget *a2, *tbl, *lb10, *lb11, *lb12, *lb13, *lb20, *lb21, *lb22, *lb23;
	GtkWidget *a3, *h3;

	/*	20110728VACS	*/
	GtkWidget	*a_bat1, *h_bat1;
	GtkWidget	*a_bat2, *h_bat2;

	char tmps[SMALL_STR];

	/*	20110730VACS	*/
	/*	BAT1 charge on, GPIO65	*/
	if(!press_charge_on(NULL, (gpointer)2))	return 1;
	usleep(100000);
	/*	BAT2 charge on, GPIO54	*/
	if(!press_charge_on(NULL, (gpointer)3))	return 1;
	usleep(100000);

	lb_adc=gtk_label_new("");
	strcpy(tmps, "DC level = 0mV\n");
	strcat(tmps, "BAT1 level = 0mV\n");
	if (g_board_type == WPC_BOARD_TYPE_J) {
		strcat(tmps, "BAT2 level = 0mV\n");
	} else {
		strcat(tmps, "BatteryCradle level = 0mV\n");
	}
	gtk_label_set_text(GTK_LABEL(lb_adc), tmps);

	a1=gtk_alignment_new(0.5, 0.9, 0.5, 0.5);
	gtk_container_add(GTK_CONTAINER(a1), lb_adc);
	
	a2=gtk_alignment_new(0.8, 0.5, 0.6, 0.5);
	tbl=gtk_table_new(7, 2, FALSE);
	if(setup_img()) return 1;
	
	lb10=gtk_label_new("Battery 1 :    ");
	lb11=gtk_label_new("Fast        ");
	lb12=gtk_label_new("Full        ");
	lb13=gtk_label_new("Fault");
	lb20=gtk_label_new("Battery 2 :    ");
	lb21=gtk_label_new("Fast        ");
	lb22=gtk_label_new("Full        ");
	lb23=gtk_label_new("Fault");
	
	gtk_table_attach(GTK_TABLE(tbl), lb10, 0, 1, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb11, 2, 3, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb12, 4, 5, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb13, 6, 7, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), bat1fast, 1, 2, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), bat1full, 3, 4, 0, 1, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), bat1fault, 5, 6, 0, 1, 0, 0, 5, 20);
	if (g_board_type == WPC_BOARD_TYPE_J) {
	gtk_table_attach(GTK_TABLE(tbl), lb20, 0, 1, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb21, 2, 3, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb22, 4, 5, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), lb23, 6, 7, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), bat2fast, 1, 2, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), bat2full, 3, 4, 1, 2, 0, 0, 5, 20);
	gtk_table_attach(GTK_TABLE(tbl), bat2fault, 5, 6, 1, 2, 0, 0, 5, 20);
	}
	gtk_container_add(GTK_CONTAINER(a2), tbl);
	
	a3=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);
	h3=gtk_hbox_new(FALSE, 10);
	lb_logging=gtk_label_new(pid_battery_logger ? LABEL_LOGGING_ON : LABEL_LOGGING_OFF);
	b_start=gtk_button_new_with_label("Start");
	g_signal_connect(b_start, "clicked", G_CALLBACK(press_start), (gpointer)0);
	if(pid_battery_logger) gtk_widget_set_sensitive(b_start, FALSE);
	
	b_stop=gtk_button_new_with_label("Stop");
	g_signal_connect(b_stop, "clicked", G_CALLBACK(press_stop), (gpointer)0);
	if(!pid_battery_logger) gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_container_add(GTK_CONTAINER(h3), lb_logging);
	gtk_container_add(GTK_CONTAINER(h3), b_start);
	gtk_container_add(GTK_CONTAINER(h3), b_stop);
	gtk_container_add(GTK_CONTAINER(a3), h3);
	

	/*	20110728VACS	*/
	/*	battery1	*/
	a_bat1=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);
	h_bat1=gtk_hbox_new(FALSE, 10);
	lb_charge_b1=gtk_label_new(pid_bat1_charge ? LABEL_BAT1_CHARGE_ON : LABEL_BAT1_CHARGE_OFF);
	b_charge_b1_on=gtk_button_new_with_label("Charge on");
	g_signal_connect(b_charge_b1_on, "clicked", G_CALLBACK(press_charge_on), (gpointer)0);
	if(pid_bat1_charge){
		gtk_widget_set_sensitive(b_charge_b1_on, FALSE);
	}	else	{
		gtk_widget_set_sensitive(b_charge_b1_on, TRUE);
	}

	b_charge_b1_off=gtk_button_new_with_label("Charge off");
	g_signal_connect(b_charge_b1_off, "clicked", G_CALLBACK(press_charge_off), (gpointer)0);
	if(!pid_bat1_charge){
		gtk_widget_set_sensitive(b_charge_b1_off, FALSE);
	}	else	{
		gtk_widget_set_sensitive(b_charge_b1_off, TRUE);
	}
	gtk_container_add(GTK_CONTAINER(h_bat1), lb_charge_b1);
	gtk_container_add(GTK_CONTAINER(h_bat1), b_charge_b1_on);
	gtk_container_add(GTK_CONTAINER(h_bat1), b_charge_b1_off);
	gtk_container_add(GTK_CONTAINER(a_bat1), h_bat1);

	/*	battery2	*/
	if (g_board_type == WPC_BOARD_TYPE_J) {
	a_bat2=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);
	h_bat2=gtk_hbox_new(FALSE, 10);
	lb_charge_b2=gtk_label_new(pid_bat2_charge ? LABEL_BAT2_CHARGE_ON : LABEL_BAT2_CHARGE_OFF);
	b_charge_b2_on=gtk_button_new_with_label("Charge on");
	g_signal_connect(b_charge_b2_on, "clicked", G_CALLBACK(press_charge_on), (gpointer)1);
	if(pid_bat2_charge){
		gtk_widget_set_sensitive(b_charge_b2_on, FALSE);
	}	else	{
		gtk_widget_set_sensitive(b_charge_b2_on, TRUE);
	}

	b_charge_b2_off=gtk_button_new_with_label("Charge off");
	g_signal_connect(b_charge_b2_off, "clicked", G_CALLBACK(press_charge_off), (gpointer)1);
	if(!pid_bat2_charge){
		gtk_widget_set_sensitive(b_charge_b2_off, FALSE);
	}	else	{
		gtk_widget_set_sensitive(b_charge_b2_off, TRUE);
	}
	gtk_container_add(GTK_CONTAINER(h_bat2), lb_charge_b2);
	gtk_container_add(GTK_CONTAINER(h_bat2), b_charge_b2_on);
	gtk_container_add(GTK_CONTAINER(h_bat2), b_charge_b2_off);
	gtk_container_add(GTK_CONTAINER(a_bat2), h_bat2);
	}

	v0=gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	gtk_container_add(GTK_CONTAINER(v0), a2);
	gtk_container_add(GTK_CONTAINER(v0), a3);
	gtk_container_add(GTK_CONTAINER(v0), a_bat1);
	if (g_board_type == WPC_BOARD_TYPE_J) {
	gtk_container_add(GTK_CONTAINER(v0), a_bat2);
	}

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
	
	bat_disp_thread_run = 1;
	pthread_create(&bat_disp_thread, NULL, get_value_thread, 0);
	if(testtime){
		battery_main_interval_mode(v0);
	}else{
//		start_timer();
		gtk_main();
	}

	bat_disp_thread_run = 0;
	pthread_join(bat_disp_thread, NULL);
//	stop_timer();
	
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	lb_adc=bat1fast=bat2fast=NULL;
	
	return 0;
}


//
//	sub area battery status 
//

static GdkPixbuf *batsub_img_on, *batsub_img_off;
static GtkWidget *imgsub_bat1fast, *imgsub_bat1full, *imgsub_bat1fault;
static GtkWidget *imgsub_bat2fast, *imgsub_bat2full, *imgsub_bat2fault;
static GtkWidget *lb_bat0, *lb_batsub1v, *lb_batsub2v;

GtkWidget *sc_battery_box_new()
{
	char tmps[SMALL_STR];
	GtkWidget *h, *lb_bat1, *lb_bat2;
	GtkWidget *lb_fast1, *lb_full1, *lb_fault1;
	GtkWidget *lb_fast2, *lb_full2, *lb_fault2;
	
	batsub_img_on = gdk_pixbuf_new_from_inline(
			-1, indicator_on_small_inline, FALSE, NULL);
	if(!batsub_img_on){
		printf("Could not open %s\n", tmps);
		return NULL;
	}
	
	batsub_img_off = gdk_pixbuf_new_from_inline(
			-1, indicator_off_small_inline, FALSE, NULL);
	if(!batsub_img_off){
		printf("Could not open %s\n", tmps);
		return NULL;
	}
	
	g_object_ref(batsub_img_on);
	g_object_ref(batsub_img_off);
	imgsub_bat1fast=gtk_image_new_from_pixbuf(batsub_img_off);
	imgsub_bat1full=gtk_image_new_from_pixbuf(batsub_img_off);
	imgsub_bat1fault=gtk_image_new_from_pixbuf(batsub_img_off);
	if (g_board_type == WPC_BOARD_TYPE_J) {
	imgsub_bat2fast=gtk_image_new_from_pixbuf(batsub_img_off);
	imgsub_bat2full=gtk_image_new_from_pixbuf(batsub_img_off);
	imgsub_bat2fault=gtk_image_new_from_pixbuf(batsub_img_off);
	} else {
		imgsub_bat2fast = NULL;
		imgsub_bat2full = NULL;
		imgsub_bat2fault = NULL;
	}
	
	lb_bat1=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(lb_bat1), "<span size=\"x-small\">Bat1:</span>");
	lb_batsub1v=gtk_label_new("");
	
	lb_bat2=gtk_label_new("");
	if (g_board_type == WPC_BOARD_TYPE_J) {
	gtk_label_set_markup(GTK_LABEL(lb_bat2), "<span size=\"x-small\">  Bat2:</span>");
	} else {
		gtk_label_set_markup(GTK_LABEL(lb_bat2),
				"<span size=\"x-small\">  BatteryCradle:</span>");
	}
	lb_batsub2v=gtk_label_new("");
	
	lb_bat0=gtk_label_new("");
	
	lb_fast1=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(lb_fast1), "<span size=\"x-small\">Fast</span>");
	lb_full1=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(lb_full1), "<span size=\"x-small\">Full</span>");
	lb_fault1=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(lb_fault1), "<span size=\"x-small\">Fault</span>");
	
	lb_fast2=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(lb_fast2), "<span size=\"x-small\">Fast</span>");
	lb_full2=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(lb_full2), "<span size=\"x-small\">Full</span>");
	lb_fault2=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(lb_fault2), "<span size=\"x-small\">Fault</span>");
	
	h=gtk_hbox_new(FALSE, 0);
	g_object_ref(h);
	
	gtk_container_add(GTK_CONTAINER(h), lb_bat1);
	gtk_container_add(GTK_CONTAINER(h), lb_batsub1v);
	gtk_container_add(GTK_CONTAINER(h), imgsub_bat1fast);
	gtk_container_add(GTK_CONTAINER(h), lb_fast1);
	gtk_container_add(GTK_CONTAINER(h), imgsub_bat1full);
	gtk_container_add(GTK_CONTAINER(h), lb_full1);
	gtk_container_add(GTK_CONTAINER(h), imgsub_bat1fault);
	gtk_container_add(GTK_CONTAINER(h), lb_fault1);
	
	gtk_container_add(GTK_CONTAINER(h), lb_bat2);
	gtk_container_add(GTK_CONTAINER(h), lb_batsub2v);
	if (g_board_type == WPC_BOARD_TYPE_J) {
	gtk_container_add(GTK_CONTAINER(h), imgsub_bat2fast);
	gtk_container_add(GTK_CONTAINER(h), lb_fast2);
	gtk_container_add(GTK_CONTAINER(h), imgsub_bat2full);
	gtk_container_add(GTK_CONTAINER(h), lb_full2);
	gtk_container_add(GTK_CONTAINER(h), imgsub_bat2fault);
	gtk_container_add(GTK_CONTAINER(h), lb_fault2);
	}
	
	gtk_container_add(GTK_CONTAINER(h), lb_bat0);
	return h;
}


gboolean battery_sub_update_step(gpointer point)
{
	char tmps[SMALL_STR];
	
	get_value();
	
	if (gtk_widget_get_visible(imgsub_bat1fast) == FALSE) {
		return FALSE;
	}
	
	update_image_gen(batsub_img_on, batsub_img_off,
					imgsub_bat1fast, imgsub_bat1full, imgsub_bat1fault,
					imgsub_bat2fast, imgsub_bat2full, imgsub_bat2fault);
	
	if (0<=bat_r2 && 0<=bat_r3 && 0<=bat_r4) {
		sprintf(tmps, "<span size=\"x-small\">  DC:%dmV</span>", bat_v0);
		gtk_label_set_markup(GTK_LABEL(lb_bat0), tmps);
		
		sprintf(tmps, "<span size=\"x-small\">%dmV</span>", bat_v1);
		gtk_label_set_markup(GTK_LABEL(lb_batsub1v), tmps);
		
		sprintf(tmps, "<span size=\"x-small\">%dmV</span>", bat_v2);
		gtk_label_set_markup(GTK_LABEL(lb_batsub2v), tmps);
	}
	
	return FALSE;
}


void *battery_sub_update(void *data)
{
	guint tag;
	
	while (battery_sub_run) {

		tag=g_idle_add_full(G_PRIORITY_HIGH, battery_sub_update_step, NULL, NULL);
		usleep(1000*1000);
		/*g_source_remove(tag);*/
		/* if function returns FALSE, tag is automatically removed */
	}	
	return NULL;
}

