/*
 *		WiFi 
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
#include "md5.h"

static int iw_log_run, ping_log_run, use_crypt;
static int pid_ping=0, pid_iw=0;
GtkWidget *v_main, *b_ping_start, *b_ping_stop, *lb_lq;
GtkWidget *lb_rssi;

struct log_check_info
{
	GtkWidget *w;
	char *log;
	int *flag;
	int *pid;
	int n;
	char buf[SMALL_STR];
};
static struct log_check_info log_check_ping, log_check_iw;


//
//	extract link quality
//
static void log_checker_extract_lq(char *log, char *buf, int n)
{
	char *p0, *p1, tmps[SMALL_STR];
	
	if(strcmp(log, "/tmp/iwconfig.log")!=0) return;
	p0=strstr(buf, "Link Quality");
	if(!p0) return;
	p1=strchr(p0+15, ' ');
	if(!p1) return;
	if(30<p1-p0) return;
	*p1='\0';
	sprintf(tmps, "Link Quality : %s", p0+13);
	gtk_label_set_text(GTK_LABEL(lb_lq), tmps);
}
// notice: buf[] may be modified.


//
//	extract rssi
//
static void iwconfig_checker_extract_rssi(char *log, char *buf, int n)
{
	char *p0, *p1, tmps[SMALL_STR];
	char target[] = "Signal level";
	
	p0 = strstr(buf, target);
	if (!p0) {
		return;
	}
	
	p1 = strtok(p0+strlen(target)+1, " ");
	if (!p1) {
		return;
	}
	
	sprintf(tmps, "RSSI : %s dBm", p1);
	gtk_label_set_text(GTK_LABEL(lb_rssi), tmps);
}


gboolean iwconfig_checker_step(gpointer point)
{
	GtkTextBuffer *tb;
	struct log_check_info *c = (struct log_check_info*)point;
	
	tb = gtk_text_view_get_buffer((GtkTextView *)c->w);
	gtk_text_buffer_set_text(tb, c->buf, -1);
	
	iwconfig_checker_extract_rssi(c->log, c->buf, c->n);
	log_checker_extract_lq(c->log, c->buf, c->n);
	
	return FALSE;
}


//
//	iwconfig checker
//
static void *iwconfig_checker(void *a)
{
	FILE *fp;
	char buf[256];
	struct log_check_info *c;
	
	c = (struct log_check_info*)a;
	
	while (*(c->flag)) {
		fp = popen("/sbin/iwconfig $SC_WLAN_IFACE 2>&1 | sed 's/    //'", "r");
		if (fp == NULL) {
			return NULL;
		}
		
		memset(buf, 0, sizeof(buf));
		c->n = fread(c->buf, sizeof(c->buf[0]), sizeof(c->buf), fp);
		
		pclose(fp);
		
		g_idle_add_full(G_PRIORITY_HIGH, iwconfig_checker_step, c, NULL);
		
		sleep(1);
	}
	
	return NULL;
}


gboolean log_checker_step(gpointer point)
{
	struct log_check_info *c = (struct log_check_info*)point;
	
	sc_gtk_text_view_insert_ping((GtkTextView *)c->w, c->buf, c->n);
	
	return FALSE;
}


//
//	log checker
//
static void *log_checker(void *a)
{
	int fd=0;
	struct log_check_info *c;
	
	c=(struct log_check_info *)a;
	c->n = 0;
	
restart:
	while(fd<=0){
		if(!*(c->flag)) return NULL;
		fd=open(c->log, O_RDONLY|O_NONBLOCK);
		usleep(200000);
	}
	
	while (*(c->flag)) {
		if (0 < (c->n)) {
			g_idle_add_full(G_PRIORITY_HIGH, log_checker_step, c, NULL);
		} else if (c->pid && !have_proc_pid(*(c->pid))) {
			close(fd);
			fd=-1;
			unlink(c->log);
			goto restart;
		}
		usleep(100000);
		c->n = read(fd, c->buf, SMALL_STR);
	}
	close(fd);
	
	return NULL;
}

//
//	wifi interface checker
//
static void check_wlan()
{
	char tmps[SMALL_STR];
	int i=0, r;
	
	for(i=0;i<9;i++){
		sprintf(tmps, "iwconfig wlan%d 2>&1 | grep 'No such device' >/dev/null", i);
		r=system(tmps);
		if(r!=-1&&WEXITSTATUS(r)==1){
			sprintf(tmps, "wlan%d", i);
			setenv("SC_WLAN_IFACE", strdup(tmps), 1);
			debug_printf(3, "Found wifi interface %s. (set to SC_WLAN_IFACE)\n", getenv("SC_WLAN_IFACE"));
			return;
		}
	}
	debug_printf(3, "ERROR: Could not find wlan device.\n");
}

static int iwconfig_start()
{
	char cmd[SMALL_STR];
	
	switch(pid_iw=fork()){
	case -1:
		pid_iw=0;
		return -1;
	
	case 0:
		setpgid(getpid(),getpid());
		sprintf(cmd, "while true ; do /sbin/iwconfig $SC_WLAN_IFACE 2>&1 | sed 's/    //' >> /tmp/iwconfig.log ; sleep 1 ; done");
		execl("/bin/sh", "sh", "-c", cmd, NULL);
		_exit(127);
		break;
	}
	return 0;
}

static void press_start_ping(GtkWidget *widget, gpointer data)
{
	char tmps[SMALL_STR];
	
	switch(pid_ping=fork()){
	case -1:
		pid_ping=0;
		return;
	
	case 0:
		setpgid(getpid(),getpid());
		sprintf(tmps, "%s/%s", base_path, "script/wifi-ping");
		execl("/bin/sh", "sh", tmps, NULL);
		_exit(127);
		break;
	}
	gtk_widget_set_sensitive(b_ping_start, FALSE);
	gtk_widget_set_sensitive(b_ping_stop, TRUE);
}

static void press_stop_ping(GtkWidget *widget, gpointer data)
{
	int s;
	
	if(!pid_ping) return;
	gtk_widget_set_sensitive(b_ping_stop, FALSE);
	do_killpg(pid_ping);
	waitpid(pid_ping, &s, WNOHANG);
	pid_ping=0;
	gtk_widget_set_sensitive(b_ping_start, TRUE);
}

static void press_browser(GtkWidget *widget, gpointer data)
{
	char tmps[SMALL_STR];
	sprintf(tmps, "%s/%s", base_path, "script/wifi-browser");
	gtk_widget_set_sensitive(v_main, FALSE);
	sc_gtk_update();
	system(tmps);
	gtk_widget_set_sensitive(v_main, TRUE);
}

static void press_crypt(GtkWidget *widget, gpointer data)
{
	use_crypt=use_crypt ? 0 : 1;
}

int error_check(GtkTextView *tv)
{
	char buf[SMALL_STR];
	int n;
	FILE *fp;
	
	fp=fopen("/tmp/sc-error.log", "r");
	if(!fp) return 0;
	
	while((n=fread(buf, 1, SMALL_STR, fp))>0){
		sc_gtk_text_view_insert(tv, buf, n);
	}
	fclose(fp);
	unlink("/tmp/sc-error.log");
	return 1;
}

void press_download(GtkWidget *widget, gpointer data)
{
	GtkWidget *p;
	char tmps[SMALL_STR];
	unsigned char buf[SMALL_STR], digest[16];
	int n, msec;
	long long int sum;
	FILE *fp;
	struct MD5Context ctx;
	struct timeval tv0, tv1;

	p=(GtkWidget *)data;
	unlink("/tmp/sc-error.log");
	strcpy(tmps, "\nStarted download ...\n");
	sc_gtk_text_view_insert_ping((GtkTextView *)log_check_ping.w, tmps, strlen(tmps));
	gtk_widget_set_sensitive(p, FALSE);
	sc_gtk_update();
	
	MD5Init(&ctx);
	gettimeofday(&tv0,NULL);
	sprintf(tmps, "%s%s", base_path, "/script/wifi-download");
	fp=popen(tmps, "r");
	if(!fp){
		printf("Could not start wifi-download");
		return;
	}
	sum=0;
	while((n=fread(buf, 1, SMALL_STR, fp))>0){
		MD5Update(&ctx, buf, n);
		sum+=n;
	}
	gettimeofday(&tv1,NULL);
	MD5Final(digest, &ctx);
	pclose(fp);
	
	msec=time_diff_msec(&tv1, &tv0);
	
	error_check((GtkTextView *)log_check_ping.w);
	sprintf(tmps, "Download %lld bytes in %.2f sec, average %lld bytes/sec.\n",
					sum, (1.0*msec)/1000.0, sum*1000/msec);
	strcat(tmps, "md5sum : ");
	append_hex(tmps, 16, (char *)digest);
	strcat(tmps, "\n");
	sc_gtk_text_view_insert_ping((GtkTextView *)log_check_ping.w, tmps, strlen(tmps));
	gtk_widget_set_sensitive(p,TRUE);
	return;
}

void press_upload(GtkWidget *widget, gpointer data)
{
	int i, n, msec;
	long long int sum;
	GtkWidget *p;
	FILE *fp_in, *fp_out;
	struct MD5Context ctx;
	struct timeval tv0, tv1;
	char tmps[SMALL_STR];
	unsigned char buf[SMALL_STR], digest[16];
	
	p=(GtkWidget *)data;
	gtk_widget_set_sensitive(p, FALSE);
	
	sc_message(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
				"Uploading is about to start.\n"
				"It is required that the remote host is ready to accept incoming data.\n"
				"For example, command like below have to be run.\n\n"
				"  nc -lp 10000 >/tmp/got.dat");
	
	unlink("/tmp/sc-error.log");
	strcpy(tmps, "\nStarted upload ...\n");
	sc_gtk_text_view_insert_ping((GtkTextView *)log_check_ping.w, tmps, strlen(tmps));
	sc_gtk_update();
	
	MD5Init(&ctx);
	sprintf(tmps, "%s%s", base_path, "/data/txdata");
	fp_in=fopen(tmps, "r");
	if(!fp_in){
		printf("Could not open input data.");
		return;
	}
	gettimeofday(&tv0,NULL);
	sprintf(tmps, "%s%s 2>>/tmp/ping.log", base_path, "/script/wifi-upload");
	fp_out=popen(tmps, "w");
	if(!fp_out){
		printf("Could not start wifi-download");
		return;
	}
	sum=0;
	while((n=fread(buf, 1, SMALL_STR, fp_in))>0){
		MD5Update(&ctx, buf, n);
		fwrite(buf, 1, n, fp_out);
		sum+=n;
	}
	MD5Final(digest, &ctx);
	pclose(fp_out);
	fclose(fp_in);
	gettimeofday(&tv1,NULL);
	msec=time_diff_msec(&tv1, &tv0);
	
	error_check((GtkTextView *)log_check_ping.w);
	sprintf(tmps, "Upload %lld bytes in %.2f sec, average %lld bytes/sec.\n",
					sum, (1.0*msec)/1000.0, sum*1000/msec);
	append_sprintf(tmps, "md5sum : ");
	for(i=0;i<16;i++) append_sprintf(tmps, "%02x", digest[i]);
	append_sprintf(tmps, "\n");
	sc_gtk_text_view_insert_ping((GtkTextView *)log_check_ping.w, tmps, strlen(tmps));
	
	gtk_widget_set_sensitive(p, TRUE);
}

void press_configure(GtkWidget *widget, gpointer data)
{
	char tmps[SMALL_STR];
	
	udhcpc_kill_check=1;
	unlink("/tmp/sc-error.log");
	sprintf(tmps, "%s/script/wifi-set-", base_path);
	switch((int)data){
	case 0:
		strcat(tmps, "infra-dhcp");
		break;
	case 1:
		strcat(tmps, "infra-static");
		break;
	case 2:
		strcat(tmps, "adhoc");
		break;
	}
	if(!use_crypt)
		strcat(tmps, " nocrypt");
	
	system(tmps);
	error_check((GtkTextView *)log_check_ping.w);
}


//
//	menu of WiFi
//
int wifi_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	pthread_t th_ping, th_iw;
	GtkWidget *a0, *tbl, *bb;
	GtkWidget *v00, *v01, *v10, *v11;
	GtkWidget *a00, *a01, *a10, *a11;
	GtkWidget *sc0, *sc1, *tv0, *tv1;
	GtkWidget *b00, *b01, *b02, *b10, *b11, *b12, *cb0;
	GtkWidget *h2;
	
	ping_log_run=1;
	iw_log_run=1;
	unlink("/tmp/iwconfig.log");
	unlink("/tmp/ping.log");
	check_wlan();
	
	v_main=gtk_vbox_new(FALSE, 10);
	a0=gtk_alignment_new(0.5, 0.5, 0, 0);
	tbl=gtk_table_new(10, 2, FALSE);
	gtk_container_add(GTK_CONTAINER(a0), tbl);
	gtk_container_add(GTK_CONTAINER(v_main), a0);
	bb=sc_bbox2(&button_no, bsub, gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click);
	gtk_box_pack_start(GTK_BOX(v_main), bb, FALSE, FALSE, 0);
	
	// top-left part
	v00=gtk_vbox_new(FALSE, 10);
	a00=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	b00=gtk_button_new_with_label("Configure Infrastructure - DHCP");
	g_signal_connect(b00, "clicked", G_CALLBACK(press_configure), (gpointer)0);
	b01=gtk_button_new_with_label("Configure Infrastructure - Static IP");
	g_signal_connect(b01, "clicked", G_CALLBACK(press_configure), (gpointer)1);
	b02=gtk_button_new_with_label("Configure Adhoc - Static IP");
	g_signal_connect(b02, "clicked", G_CALLBACK(press_configure), (gpointer)2);
	cb0=gtk_check_button_new_with_label("Use Encryption");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb0), TRUE);
	use_crypt=1;
	g_signal_connect(cb0, "toggled", G_CALLBACK(press_crypt), (gpointer)0);
	
	gtk_container_add(GTK_CONTAINER(v00), b00);
	gtk_container_add(GTK_CONTAINER(v00), b01);
	gtk_container_add(GTK_CONTAINER(v00), b02);
	gtk_container_add(GTK_CONTAINER(v00), cb0);
	gtk_container_add(GTK_CONTAINER(a00), v00);
	gtk_table_attach(GTK_TABLE(tbl), a00, 0, 4, 0, 1, 0, 0, 5, 20);
	
	// top-right part = iwconfig info 
	tv0=gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(tv0), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(tv0), FALSE);
	gtk_widget_set_usize(tv0, 410, 150);
	
	a01=gtk_alignment_new(0.5, 0.5, 1, 0.8);
	v01=gtk_vbox_new(FALSE, 10);
	lb_lq = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(v01), lb_lq);
	lb_rssi = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(v01), lb_rssi);
	gtk_container_add(GTK_CONTAINER(v01), tv0);
	gtk_container_add(GTK_CONTAINER(a01), v01);
	gtk_table_attach(GTK_TABLE(tbl), a01, 4, 10, 0, 1, 0, 0, 5, 20);
	
	// bottom-left part
	b10=gtk_button_new_with_label("Download File");
	g_signal_connect(b10, "clicked", G_CALLBACK(press_download), (gpointer)table);
	b11=gtk_button_new_with_label("Upload File");
	g_signal_connect(b11, "clicked", G_CALLBACK(press_upload), (gpointer)table);
	b12=gtk_button_new_with_label("Invoke Browser");
	g_signal_connect(b12, "clicked", G_CALLBACK(press_browser), (gpointer)0);
	v10=gtk_vbox_new(FALSE, 10);
	a10=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	gtk_container_add(GTK_CONTAINER(v10), b10);
	gtk_container_add(GTK_CONTAINER(v10), b11);
	gtk_container_add(GTK_CONTAINER(v10), b12);
	gtk_container_add(GTK_CONTAINER(a10), v10);
	gtk_table_attach(GTK_TABLE(tbl), a10, 0, 4, 1, 2, 0, 0, 5, 20);
	
	// bottom-right part = ping info
	sc1=gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sc1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_usize(sc1, 410, 120);
	tv1=gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(tv1), FALSE);
	gtk_container_add(GTK_CONTAINER(sc1), tv1);
	a11=gtk_alignment_new(0.5, 0.5, 1, 0.8);
	v11=gtk_vbox_new(FALSE, 10);
	a11=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	h2=gtk_hbox_new(FALSE, 10);
	b_ping_start=gtk_button_new_with_label("Start Ping");
	g_signal_connect(b_ping_start, "clicked", G_CALLBACK(press_start_ping), (gpointer)0);
	b_ping_stop=gtk_button_new_with_label("Stop Ping");
	g_signal_connect(b_ping_stop, "clicked", G_CALLBACK(press_stop_ping), (gpointer)0);
	gtk_widget_set_sensitive(b_ping_stop, FALSE);
	gtk_container_add(GTK_CONTAINER(h2), b_ping_start);
	gtk_container_add(GTK_CONTAINER(h2), b_ping_stop);
	gtk_container_add(GTK_CONTAINER(v11), sc1);
	gtk_container_add(GTK_CONTAINER(v11), h2);
	gtk_container_add(GTK_CONTAINER(a11), v11);
	gtk_table_attach(GTK_TABLE(tbl), a11, 4, 10, 1, 2, 0, 0, 5, 20);
	
	log_check_iw.w=tv0;
	log_check_iw.log="/tmp/iwconfig.log";
	log_check_iw.flag=&ping_log_run;
	log_check_iw.pid=&pid_iw;
	pthread_create(&th_iw, NULL, iwconfig_checker, (void *)&log_check_iw);
//	pthread_create(&th_iw, NULL, log_checker, (void *)&log_check_iw);
//	iwconfig_start();
	
	log_check_ping.w=tv1;
	log_check_ping.log="/tmp/ping.log";
	log_check_ping.flag=&ping_log_run;
	log_check_ping.pid=&pid_ping;
	pthread_create(&th_ping, NULL, log_checker, (void *)&log_check_ping);
	
	sc_table_attach2(GTK_TABLE(table), v_main);
	gtk_widget_show_all(table);
	
	gtk_main();
	
	ping_log_run=0;
	iw_log_run=0;
	do_killpg(pid_ping);
	do_killpg(pid_iw);
	sc_gtk_update();
	gdk_threads_leave();
	pthread_join(th_ping, NULL);
	pthread_join(th_iw, NULL);
	gdk_threads_enter();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v_main);
	
	return 0;
}
