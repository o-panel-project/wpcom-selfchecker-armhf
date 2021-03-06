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
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>
#include "common.h"
#include "md5.h"
#include "tctlprof.h"

static int iw_log_run, ping_log_run;
static int use_wifi_config = 0;
static int pid_ping=0, pid_iw=0;
GtkWidget *v_main, *b_ping_start, *b_ping_stop, *lb_lq, *lb_ip, *lb_mac;
GtkWidget *lb_rssi;

struct log_check_info
{
	GtkWidget *w;
	char *log;
	int *flag;
	int *pid;
	int n;
	char buf[MID_STR];
};
static struct log_check_info log_check_ping, log_check_iw;

struct _wifi_config {
	char label[SMALL_STR];
	char essid[SMALL_STR];
	char wepkey[SMALL_STR];
	char ip_config[SMALL_STR];
	in_addr_t ping_target;
	in_addr_t static_ip;
	in_addr_t static_mask;
	in_addr_t static_gw;
};
#define WIFI_MAX_CONFIG 5
static struct _wifi_config wifi_config[WIFI_MAX_CONFIG];
static int wifi_config_entry;


//
//  extract wifi config
//
static void wifi_config_setup()
{
	int i, count, ret;
	char sect[SMALL_STR], key[SMALL_STR];
	struct in_addr ip;

	comn_ProfileSetup("/tmp/selfcheck.ini");

	for (i = 1, count = 0; i <= WIFI_MAX_CONFIG; i++) {
		sprintf(sect, "wifi_config%d", i);
		ret = comn_GetProfileString(sect, "label", "", key, sizeof(key), NULL);
		if (ret == 0) continue;
		strcpy(wifi_config[count].label, key);
		comn_GetProfileString(sect, "essid",
				"MeniukunNet", key, sizeof(key), NULL);
		strcpy(wifi_config[count].essid, key);
		comn_GetProfileString(sect, "wepkey",
				"worldpicom001", key, sizeof(key), NULL);
		strcpy(wifi_config[count].wepkey, key);
		comn_GetProfileString(sect, "ip_config",
				"dhcp", key, sizeof(key), NULL);
		strcpy(wifi_config[count].ip_config, key);
		comn_GetProfileString(sect, "ping_target",
				"8.8.8.8", key, sizeof(key), NULL);
		ret = inet_aton(key, &ip);
		if (ret != 0)
			wifi_config[count].ping_target = ip.s_addr;
		else
			wifi_config[count].ping_target = INADDR_NONE;

		if (strcmp(wifi_config[count].ip_config, "static")) {
			/* not static configuration */
			wifi_config[count].static_ip = INADDR_NONE;
			wifi_config[count].static_mask = INADDR_NONE;
			wifi_config[count].static_gw = INADDR_NONE;
			count++;
			continue;
		}

		/* static configuration */
		ret = comn_GetProfileString(sect, "ip_config_ip",
				"", key, sizeof(key), NULL);
		if (ret > 0)  ret = inet_aton(key, &ip);
		if (ret != 0) wifi_config[count].static_ip = ip.s_addr;
		else          wifi_config[count].static_ip = INADDR_NONE;
		ret = comn_GetProfileString(sect, "ip_config_mask",
				"", key, sizeof(key), NULL);
		if (ret > 0)  ret = inet_aton(key, &ip);
		if (ret != 0) wifi_config[count].static_mask = ip.s_addr;
		else          wifi_config[count].static_mask = INADDR_NONE;
		ret = comn_GetProfileString(sect, "ip_config_gw",
				"", key, sizeof(key), NULL);
		if (ret > 0)  ret = inet_aton(key, &ip);
		if (ret != 0) wifi_config[count].static_gw = ip.s_addr;
		else          wifi_config[count].static_gw = INADDR_NONE;

		count++;
	}

	comn_ProfileTerm();

	wifi_config_entry = count;
	if (wifi_config_entry == 0)
		debug_printf(3, "Wifi configuration not found\n");
	for(i = 0; i < wifi_config_entry; i++) {
		debug_printf(3, "%s() (%d) label      [%s]\n",
				__func__, i+1, wifi_config[i].label);
		debug_printf(3, "%s() (%d) essid      [%s]\n",
				__func__, i+1, wifi_config[i].essid);
		debug_printf(3, "%s() (%d) wepkey     [%s]\n",
				__func__, i+1, wifi_config[i].wepkey);
		debug_printf(3, "%s() (%d) ip config  [%s]\n",
				__func__, i+1, wifi_config[i].ip_config);
		debug_printf(3, "%s() (%d) ping       [%08x]\n",
				__func__, i+1, htonl(wifi_config[i].ping_target));
		if (strcmp(wifi_config[i].ip_config, "static"))
			continue;
		debug_printf(3, "%s() (%d) static ip  [%08x]\n",
				__func__, i+1, htonl(wifi_config[i].static_ip));
		debug_printf(3, "%s() (%d) static mask[%08x]\n",
				__func__, i+1, htonl(wifi_config[i].static_mask));
		debug_printf(3, "%s() (%d) static gw  [%08x]\n",
				__func__, i+1, htonl(wifi_config[i].static_gw));
	}
}

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

//
//	get ip addr
//
unsigned long get_ip_addr()
{
	int fd, ret_ip;
	struct ifreq ifr_ip;
	char *p;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return 0;
	}

	p = getenv("SC_WLAN_IFACE");
	if (!p) {
		close(fd);
		return 0;
	}

	ifr_ip.ifr_addr.sa_family = AF_INET;
	strncpy(ifr_ip.ifr_name, p, IFNAMSIZ-1);
	ret_ip = ioctl(fd, SIOCGIFADDR, &ifr_ip);
	close(fd);
	if (ret_ip < 0)
		return 0;

	return ((struct sockaddr_in*)&ifr_ip.ifr_addr)->sin_addr.s_addr;
}

//
//	get mac addr
//
int get_mac_addr(unsigned char *mac_addr)
{
	int fd, ret_mac;
	struct ifreq ifr_mac;
	char *p;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return -1;
	}

	p = getenv("SC_WLAN_IFACE");
	if (!p) {
		close(fd);
		return -1;
	}

	ifr_mac.ifr_addr.sa_family = AF_INET;
	strncpy(ifr_mac.ifr_name, p, IFNAMSIZ-1);
	ret_mac = ioctl(fd, SIOCGIFHWADDR, &ifr_mac);
	close(fd);
	if (ret_mac < 0)
		return -1;

	mac_addr[0] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0];
	mac_addr[1] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1];
	mac_addr[2] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2];
	mac_addr[3] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3];
	mac_addr[4] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4];
	mac_addr[5] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5];

	return 0;
}

//
//	display ip addr
//
static void display_ip_addr()
{
	char ipstr[SMALL_STR], macstr[SMALL_STR];
	unsigned long ipaddr;
	unsigned char macaddr[SMALL_STR];
	struct in_addr ia;

	ipstr[0] = '\0';
	macstr[0] = '\0';

	if (get_mac_addr(macaddr) == 0)
		sprintf(macstr, "MAC Address : <span font_desc=\"monospace bold\""
				" size=\"x-large\""
				" foreground=\"red\">%02X-%02X-%02X-%02X-%02X-%02X</span>",
			macaddr[0], macaddr[1], macaddr[2],
			macaddr[3], macaddr[4], macaddr[5]);

	ipaddr = get_ip_addr();
	if (ipaddr != 0) {
		ia.s_addr = ipaddr;
		sprintf(ipstr, "ip addr : %s", inet_ntoa(ia));
	}

	gtk_label_set_markup(GTK_LABEL(lb_mac), macstr);
	gtk_label_set_text(GTK_LABEL(lb_ip), ipstr);
}

gboolean iwconfig_checker_step(gpointer point)
{
	GtkTextBuffer *tb;
	struct log_check_info *c = (struct log_check_info*)point;
	
	tb = gtk_text_view_get_buffer((GtkTextView *)c->w);
	gtk_text_buffer_set_text(tb, "", 0);
	usleep(100000);//g_print("%s()\n", __func__);
	gtk_text_buffer_set_text(tb, c->buf, c->n);
	
	iwconfig_checker_extract_rssi(c->log, c->buf, c->n);
	log_checker_extract_lq(c->log, c->buf, c->n);
	display_ip_addr();
	
	return FALSE;
}


//
//	iwconfig checker
//
static void *iwconfig_checker(void *a)
{
	FILE *fp;
//	char buf[256];
	struct log_check_info *c;
	
	c = (struct log_check_info*)a;
	
	while (*(c->flag)) {
	//	fp = popen("/sbin/iwconfig $SC_WLAN_IFACE 2>&1 | sed 's/    //'", "r");
		fp = popen("/sbin/iwconfig $SC_WLAN_IFACE 2>&1", "r");
		if (fp == NULL) {
			return NULL;
		}
		
	//	memset(buf, 0, sizeof(buf));
		c->n = fread(c->buf, sizeof(c->buf[0]), sizeof(c->buf), fp);
		
		pclose(fp);
		
		g_idle_add_full(G_PRIORITY_HIGH, iwconfig_checker_step, c, NULL);
		
		sleep(2);
	}
	g_print("%s() thread finish.\n", __func__);
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
		if(!*(c->flag)) break;//return NULL;
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
	g_print("%s() thread finish.\n", __func__);
	return NULL;
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
static gboolean press_start_ping_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_start_ping(widget, data);
	return FALSE;
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
static gboolean press_stop_ping_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_stop_ping(widget, data);
	return FALSE;
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
static gboolean press_browser_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_browser(widget, data);
	return FALSE;
}

static void wifi_config_changed(GtkComboBox *combo, gpointer data)
{
	gchar *string = gtk_combo_box_get_active_text(combo);
	use_wifi_config = gtk_combo_box_get_active(combo);
	g_print("%s() %s (%d) selected.\n", __func__, string, use_wifi_config);
	g_free(string);
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
static gboolean press_download_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_download(widget, data);
	return FALSE;
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
static gboolean press_upload_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_upload(widget, data);
	return FALSE;
}

static void wifi_set_config()
{
	FILE *fp;
	struct in_addr ip;

	fp = fopen("/tmp/wifi-set-config", "w");
	if (!fp) return;
	fprintf(fp, "ESSID=%s\n", wifi_config[use_wifi_config].essid);
	fprintf(fp, "KEY=%s\n", wifi_config[use_wifi_config].wepkey);
	if (wifi_config[use_wifi_config].ping_target != INADDR_NONE) {
		ip.s_addr = wifi_config[use_wifi_config].ping_target;
		fprintf(fp, "PING=%s\n", inet_ntoa(ip));
	}
	if (wifi_config[use_wifi_config].static_ip != INADDR_NONE) {
		ip.s_addr = wifi_config[use_wifi_config].static_ip;
		fprintf(fp, "IP=%s\n", inet_ntoa(ip));
	}
	if (wifi_config[use_wifi_config].static_mask != INADDR_NONE) {
		ip.s_addr = wifi_config[use_wifi_config].static_mask;
		fprintf(fp, "NM=%s\n", inet_ntoa(ip));
	}
	if (wifi_config[use_wifi_config].static_gw != INADDR_NONE) {
		ip.s_addr = wifi_config[use_wifi_config].static_gw;
		fprintf(fp, "GW=%s\n", inet_ntoa(ip));
	}
	fclose(fp);
}

void press_configure(GtkWidget *widget, gpointer data)
{
	char tmps[SMALL_STR];

	g_print("%s() start\n", __func__);
	udhcpc_kill_check=1;
	unlink("/tmp/wifi-set-config");
	unlink("/tmp/sc-error.log");
	wifi_set_config();
	sprintf(tmps, "%s/script/wifi-set-", base_path);
	if (strcmp(wifi_config[use_wifi_config].ip_config, "dhcp") == 0)
		strcat(tmps, "infra-dhcp");
	else if (strcmp(wifi_config[use_wifi_config].ip_config, "misdhcp") == 0)
		strcat(tmps, "infra-dhcp misdhcp");
	else if (strcmp(wifi_config[use_wifi_config].ip_config, "static") == 0)
		strcat(tmps, "infra-static");
	else if (strcmp(wifi_config[use_wifi_config].ip_config, "adhoc") == 0)
		strcat(tmps, "adhoc");	/* not supported */
	else if (strcmp(wifi_config[use_wifi_config].ip_config, "pskdhcp") == 0)
		strcat(tmps, "psk-dhcp");
	else {
		g_print("%s() ip_config failed\n", __func__);
		sprintf(tmps,
			"echo \"%s() ip_config failed\n\" > /tmp/sc-error.log" , __func__);
	}

	system(tmps);
	error_check((GtkTextView *)log_check_ping.w);
}
static gboolean press_configure_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_configure(widget, data);
	return FALSE;
}


//
//	menu of WiFi
//
int wifi_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no, i;
	pthread_t th_ping, th_iw;
	GtkWidget *a0, *tbl, *bb;
	GtkWidget *v00, *v01, *v10, *v11;
	GtkWidget *a00, *a01, *a10, *a11;
	GtkWidget *sc1, *tv0, *tv1;
	GtkWidget *b10, *b11, *b12;
	GtkWidget *h2;
	GtkWidget *combo, *cb_lbl, *b_go;
	
	ping_log_run=1;
	iw_log_run=1;
	unlink("/tmp/iwconfig.log");
	unlink("/tmp/ping.log");
	/* check_wlan();		move to mainpanel.c */
	wifi_config_setup();
	
	v_main=gtk_vbox_new(FALSE, 10);
	a0=gtk_alignment_new(0.5, 0.5, 0, 0);
	tbl=gtk_table_new(2, 2, FALSE);
	gtk_container_add(GTK_CONTAINER(a0), tbl);
	gtk_container_add(GTK_CONTAINER(v_main), a0);
	bb=sc_bbox2(&button_no, bsub, gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(v_main), bb, FALSE, FALSE, 0);
	
	// top-left part
	v00=gtk_vbox_new(FALSE, 10);
	a00=gtk_alignment_new(0.5, 0.5, 0.0, 0.0);

	cb_lbl = gtk_label_new("Select wifi configuration");
	combo = gtk_combo_box_new_text();
	if (wifi_config_entry == 0)
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "no config");
	else
		for (i = 0; i < wifi_config_entry; i++) {
			gtk_combo_box_append_text(
					GTK_COMBO_BOX(combo), wifi_config[i].label);
		}
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), use_wifi_config);
	g_signal_connect(G_OBJECT(combo), "changed",
			G_CALLBACK(wifi_config_changed), NULL);
	b_go = gtk_button_new_with_label("Configure");
	g_signal_connect(G_OBJECT(b_go), "button-release-event",
			G_CALLBACK(press_configure_func), (gpointer)0);

	gtk_container_add(GTK_CONTAINER(v00), cb_lbl);
	gtk_container_add(GTK_CONTAINER(v00), combo);
	gtk_container_add(GTK_CONTAINER(v00), b_go);
	gtk_container_add(GTK_CONTAINER(a00), v00);
	gtk_table_attach_defaults(GTK_TABLE(tbl), a00, 0, 1, 0, 1);
	
	// top-right part = iwconfig info 
	v01=gtk_vbox_new(FALSE, 10);
	a01=gtk_alignment_new(0.5, 0.5, 0.0, 0.0);

	tv0=gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(tv0), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(tv0), FALSE);
    gint scw = (gint)((double)get_sc_window_width() * 0.488);
	gint sch = (gint)((double)get_sc_window_height() * 0.240);
	gtk_widget_set_usize(tv0, scw, sch);
	lb_lq = gtk_label_new("");
	lb_rssi = gtk_label_new("");
	lb_mac = gtk_label_new("");
	lb_ip = gtk_label_new("");

	gtk_container_add(GTK_CONTAINER(v01), lb_lq);
	gtk_container_add(GTK_CONTAINER(v01), lb_rssi);
	gtk_container_add(GTK_CONTAINER(v01), tv0);
	gtk_container_add(GTK_CONTAINER(v01), lb_mac);
	gtk_container_add(GTK_CONTAINER(v01), lb_ip);
	gtk_container_add(GTK_CONTAINER(a01), v01);
	gtk_table_attach(GTK_TABLE(tbl), a01, 1, 2, 0, 1, 0, 0, 30, 10);
	
	// bottom-left part
	v10=gtk_vbox_new(FALSE, 10);
	a10=gtk_alignment_new(0.5, 0.5, 0.0, 0.0);

	b10=gtk_button_new_with_label("Download File");
	g_signal_connect(b10, "button-release-event", G_CALLBACK(press_download_func), (gpointer)table);
	b11=gtk_button_new_with_label("Upload File");
	g_signal_connect(b11, "button-release-event", G_CALLBACK(press_upload_func), (gpointer)table);
	b12=gtk_button_new_with_label("Invoke Browser");
	g_signal_connect(b12, "button-release-event", G_CALLBACK(press_browser_func), (gpointer)0);

	gtk_container_add(GTK_CONTAINER(v10), b10);
	gtk_container_add(GTK_CONTAINER(v10), b11);
	gtk_container_add(GTK_CONTAINER(v10), b12);
	gtk_container_add(GTK_CONTAINER(a10), v10);
	gtk_table_attach_defaults(GTK_TABLE(tbl), a10, 0, 1, 1, 2);
	
	gtk_widget_set_sensitive(b10, FALSE);
	gtk_widget_set_sensitive(b11, FALSE);
	gtk_widget_set_sensitive(b12, FALSE);

	// bottom-right part = ping info
	v11=gtk_vbox_new(FALSE, 10);
	a11=gtk_alignment_new(0.5, 0.5, 0.0, 0.0);

	sc1=gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sc1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    scw = (gint)((double)get_sc_window_width() * 0.488);
	sch = (gint)((double)get_sc_window_height() * 0.240);
	gtk_widget_set_usize(sc1, scw, sch);
	tv1=gtk_text_view_new();
	sc_gtk_text_view_insert_ping(NULL, NULL, 0);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(tv1), FALSE);
	h2=gtk_hbox_new(FALSE, 10);
	b_ping_start=gtk_button_new_with_label("Start Ping");
	g_signal_connect(b_ping_start, "button-release-event", G_CALLBACK(press_start_ping_func), (gpointer)0);
	b_ping_stop=gtk_button_new_with_label("Stop Ping");
	g_signal_connect(b_ping_stop, "button-release-event", G_CALLBACK(press_stop_ping_func), (gpointer)0);
	gtk_widget_set_sensitive(b_ping_stop, FALSE);

	gtk_container_add(GTK_CONTAINER(sc1), tv1);
	gtk_container_add(GTK_CONTAINER(h2), b_ping_start);
	gtk_container_add(GTK_CONTAINER(h2), b_ping_stop);
	gtk_container_add(GTK_CONTAINER(v11), sc1);
	gtk_container_add(GTK_CONTAINER(v11), h2);
	gtk_container_add(GTK_CONTAINER(a11), v11);
	gtk_table_attach(GTK_TABLE(tbl), a11, 1, 2, 1, 2, 0, 0, 30, 20);
	
	log_check_iw.w=tv0;
	log_check_iw.log="/tmp/iwconfig.log";
	log_check_iw.flag=&iw_log_run;
	log_check_iw.pid=&pid_iw;
	pthread_create(&th_iw, NULL, iwconfig_checker, (void *)&log_check_iw);
	
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
