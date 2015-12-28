/*
 *    main construction and starting point for New SelfChecker
 *
 *					2011.7	written by Omoikane Inc.
 *					2013.7	For j3 and refactoring, wpc.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include "common.h"
#include "wpcio.h"
#include <sys/ioctl.h>
#include <linux/hidraw.h>
#include <errno.h>

extern const char *sc_version_str;
extern void check_wlan();	/* wifisub.c */
extern int get_mac_addr(unsigned char *);	/* wifi.c */

static int ignore_count=0;
extern char usbmem_first_device;	/* RH */
extern int i2c_main_auto_menu;	/* RH */
int g_board_type = WPC_BOARD_TYPE_J;

int demo_main(GtkWidget *table, GtkWidget *bsub);
int version_main(GtkWidget *table, GtkWidget *bsub);
int wlantool_main(GtkWidget *table, GtkWidget *bsub);
int display_main(GtkWidget *table, GtkWidget *bsub);
int lcdinspect_main(GtkWidget *table, GtkWidget *bsub);
int dmesg_main(GtkWidget *table, GtkWidget *bsub);
int reset_main(GtkWidget *table, GtkWidget *bsub);

struct side_menu_list_st {
	int enable;
	int level;
	const char *menu_label;
	const char *title_label;
	const char *tree_id;
	int (*func)(GtkWidget *, GtkWidget *);
} side_menu_list[] = {
	{ 1, 1, "Wlan Tool", "Wlan Tool", "0", wlantool_main },
	{ 1, 1, "---------------", NULL, NULL/*1*/, NULL },	/* separator */
	{ 1, 1, "Demo", "Demo", "2", demo_main },
	{ 1, 1, "Strategy File", "Strategy File", "3", strategy_main },
	{ 1, 1, "Date", "Date", "4", date_main },
	{ 1, 1, "LCD", NULL, NULL, NULL },
	{ 1, 2, "Display", "LCD - Display", "5:0", display_main },
	{ 1, 2, "PWM, On/Off", "LCD - PWM, On/Off", "5:1", lcd_pwm_main },
	{ 1, 1, "TP", NULL, NULL, NULL },
	{ 1, 2, "TP Function", "Touch Pad Function", "6:0", tpfunc_main },
	{ 1, 2, "On/Off", "Touch Pad On/Off", "6:1", tpoff_main },
	{ 1, 1, "Memory", NULL, NULL, NULL },
	{ 1, 2, "SDRAM", "SDRAM", "7:0", sdrt_main },
	{ 0, 2, "NAND", "NAND", "7:1", NULL },
	{ 0, 2, "iNAND", "iNAND", "7:2", NULL },
	{ 1, 1, "Audio", "Audio", "8", audio_main },
	{ 1, 1, "Battery Level", "Battery", "9", battery_main },
	{ 0, 1, "uSD", "uSD", "10", NULL },
	{ 1, 1, "LED(GPIO)", "LED(GPIO)", "11", led_main },
	{ 1, 1, "SW(GPIO)", "SW(GPIO)", "12", dipsw_main },
	{ 1, 1, "I2C", NULL, NULL, NULL },
	{ 0, 2, "Bus1:PMIC", "i2c1(PMIC)", "13:0", NULL },
	{ 1, 2, "Bus2:Cradle", "i2c2(Cradle)", "13:1", i2c_cradle_main },
	{ 0, 2, "Bus3:LCD/TP", "i2c3(LCD/TP)", "13:2", NULL },
	{ 1, 1, "WiFi", "WiFi", "14", wifi_main },
	{ 1, 1, "HSUSB2", NULL, NULL, NULL },
	{ 1, 2, "1:Cradle", "HUSB2-1:Cradle", "15:0", usbmem_cradle_main },
	{ 1, 2, "2:Memory", "HUSB2-2:Memory", "15:1", usbmem_hub2mem_main },
	{ 1, 2, "3:FeliCa", "HUSB2-3:FeliCa", "15:2", felica_main },
	{ 1, 2, "4:ExWiFi", "HUSB2-4:ExWiFi", "15:3", usbmem_exwifi_main },
	{ 1, 1, "Movie Play", "Movie Play", "16", movie_main },
	{ 1, 1, "LCD Inspect", "LCD Inspect", "17", lcdinspect_main },
	{ 1, 1, "Version", "Version", "18", version_main },
	{ 1, 1, "dmesg", "boot message", "19", dmesg_main },
	{ 1, 1, "---------------", NULL, NULL/*20*/, NULL },	/* separator */
	{ 1, 1, "Soft Reset", "Soft Reset", "21", reset_main },
	{ 0, 0, NULL, NULL, NULL, NULL },
};

//
//	Main top : showing version
//
void sc_mainpanel(GtkWidget *w, const char *t, GtkWidget *f, GtkWidget *table)
{
	gtk_window_set_title(GTK_WINDOW(w), t);
	gtk_container_set_border_width(GTK_CONTAINER(w), 5);
	gtk_widget_set_usize(f, 216, -1);
	
	sc_table_attach1(GTK_TABLE(table), f);
	gtk_container_add(GTK_CONTAINER(w), table);
}

//
//	side tree format(model)
//
GtkWidget* sc_sidetree_model()
{
	GtkWidget *treeview;
	GtkTreeStore *treestore;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	
	treestore = gtk_tree_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);
	treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(treestore));
	g_object_unref (treestore);
	column = gtk_tree_view_column_new();
	
	gtk_tree_view_column_set_title(column, "SelfCheck Menu");
	
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", COLUMN_LABEL);
	gtk_tree_view_column_add_attribute(column, renderer, "background", COLUMN_NAME_COLOR);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
	return treeview;
}

//
//	side tree data
//
void sc_sidetree_add_data(GtkTreeView *treeview)
{
	GtkTreeStore *store;
	GtkTreeIter iter, child_iter;
	int i;

	store=GTK_TREE_STORE(gtk_tree_view_get_model(treeview));

	// RH
	for (i = 0; side_menu_list[i].menu_label != NULL; i++) {

		/* j3 unsupported */
		if (g_board_type != WPC_BOARD_TYPE_J) {
			if (side_menu_list[i].func == usbmem_cradle_main) {
				side_menu_list[i].enable = 0;
				side_menu_list[i].func = NULL;
			}
			if (side_menu_list[i].func == usbmem_exwifi_main) {
				side_menu_list[i].enable = 0;
				side_menu_list[i].func = NULL;
			}
		}

		if (side_menu_list[i].enable) {
			if (side_menu_list[i].level == 1) {
				gtk_tree_store_append(store, &iter, NULL);
				gtk_tree_store_set(store, &iter, COLUMN_LABEL,
						side_menu_list[i].menu_label, -1);
			}
			if (side_menu_list[i].level == 2) {
				gtk_tree_store_append(store, &child_iter , &iter);
				gtk_tree_store_set(store, &child_iter, COLUMN_LABEL,
						side_menu_list[i].menu_label, -1);
			}
		} else {
			if (side_menu_list[i].level == 1) {
				gtk_tree_store_append(store, &iter, NULL);
				gtk_tree_store_set(store, &iter, COLUMN_LABEL,
				side_menu_list[i].menu_label, COLUMN_NAME_COLOR, "Gray", -1);
			}
			if (side_menu_list[i].level == 2) {
				gtk_tree_store_append(store, &child_iter , &iter);
				gtk_tree_store_set(store, &child_iter, COLUMN_LABEL,
				side_menu_list[i].menu_label, COLUMN_NAME_COLOR, "Gray", -1);
			}
		}
		if (side_menu_list[i].func == audio_main)
			iter_audio = iter;
		if (side_menu_list[i].func == battery_main)
			iter_battery = iter;
	}

	gtk_tree_view_expand_all(treeview);
}

//
//	click call back
//
static void sc_sidetree_clicked(GtkWidget *widget, char **menupath)
{
	GtkTreeIter iter;
	GtkWidget *treeview;
	GtkTreePath *path;
	char *s;
	int i;
	
	if(ignore_count){ // for some buggy gtk environment. ignoring first event by option(-g)
		ignore_count=0;
		goto fin;
	}
	
	gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), (GtkTreeModel **)&treeview, &iter);
	if(!iter.user_data) goto fin; // avoid double invocation
	path=gtk_tree_model_get_path(GTK_TREE_MODEL(treeview), &iter);
	if(!path) goto fin;
	s=strdup(gtk_tree_path_to_string(path));
	if(!s) goto fin;

	// RH
	for (i = 0; side_menu_list[i].menu_label != NULL; i++) {
		/* non support function, separator skip */
//		if (!side_menu_list[i].tree_id)
//			goto fin;
		if (side_menu_list[i].tree_id &&
			(strcmp(s, side_menu_list[i].tree_id) == 0) &&
			(!side_menu_list[i].enable))
			goto fin;
	}

	if(*menupath==NULL)
		*menupath=s;
	gtk_main_quit();

fin:
	gtk_tree_selection_unselect_all(GTK_TREE_SELECTION(widget));
}

//
//	all side tree
//
GtkWidget *sc_sidetree(char **menupath)
{
	GtkWidget *sc, *sel;
	
	tv_sidemenu=sc_sidetree_model();
	sc_sidetree_add_data((GtkTreeView *)tv_sidemenu);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tv_sidemenu), FALSE);
	
	sel=(GtkWidget *)gtk_tree_view_get_selection(GTK_TREE_VIEW(tv_sidemenu));
	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(sel), GTK_SELECTION_SINGLE);
	g_signal_connect(sel, "changed", G_CALLBACK(sc_sidetree_clicked), (gpointer)menupath);
	
	sc=gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sc), tv_sidemenu);
	return sc;
}

//
//	kernel version fetcher
//
void fetch_kver(char *kver)
{
	int fd, n;
	char *p;
	
	fd=open("/proc/version", O_RDONLY);
	if(fd<0){
		strcpy(kver, "Unknown (proc unmounted?)");
		return;
	}
	if((n=read(fd,kver,SMALL_STR))<=0){
		strcpy(kver, "Unknown (could not read proc)");
		goto fin;
	}
	kver[n]='\0';

	// Trim : put a '\n' before first '#'
	p=strchr(kver, '#');
	if(p&&kver<p&&*(p-1)==' ')
		*(p-1)='\n';
	
fin:	
	close(fd);
}

//	20110827VACS
//
//	kernel version fetcher No.2
//
void
fetch_kver2(char *kver1, char *kver2)
{
int		fd, n;
char	*p;

	kver1[0]='\0'; kver2[0]='\0';

	fd=open("/proc/version", O_RDONLY);
	if(fd<0){
		strcpy(kver1, "Unknown (proc unmounted?)");
		return;
	}
	if((n=read(fd,kver1,SMALL_STR))<=0){
		strcpy(kver1, "Unknown (could not read proc)");
		goto fin;
	}
	kver1[n]='\0';

	p=strstr(kver1, "(gcc");
	if(*p!='\0'){
		sprintf(kver2, "%s", p);
		*p='\0';

		// Trim : put a '\n' before first '#'
		p=strchr(kver2, '#');
		if(p&&kver2<p&&*(p-1)==' ')
			*(p-1)='\0';
	}

fin:
	close(fd);
}

//	20110826VACS
//
//	root filesystem version fetcher
//
void
fetch_rfsver(char *kver)
{
int		fd, n, i;
char	*p;
char	tmps[SMALL_STR];

	tmps[0]='\0'; kver[0]='\0';

	printf("%s()\n", __func__);
	fd=open("/rootfs-version", O_RDONLY);
	if(fd<0){
		strcpy(tmps, "Unknown (proc unmounted?)");
		return;
	}
	if((n=read(fd,tmps,SMALL_STR))<=0){
		strcpy(tmps, "Unknown (could not read file)");
		goto fin;
	}
	tmps[n]='\0';

	p=strstr(tmps, "VERSION:");
	if((p != NULL) && (*p!='\0')){
		sprintf(kver, "%s", p);
		for(i=0;i<strlen(kver);i++){
			if((kver[i]==0x0d) || (kver[i]==0x0a)){
				kver[i]='\0';
				break;
			}
		}
	}else{
		strcpy(kver, "Unknown root filesystem version");
	}

fin:
	close(fd);
}

const static char *mms_ctrl_dev = "/dev/mms_ts";
#define IOCTL_GET_VERSION   _IOR('W', 0xA5, u_char *)

static void fetch_tpver_melfas(char *fwver)
{
	int fd;
	u_char ver[6];
	int ret;

	fwver[0] = '\0';

	fd = open(mms_ctrl_dev, O_RDWR);
	if (fd < 0) {
		perror("open");
		return;
	}
	SYSCALL(ret = ioctl(fd, IOCTL_GET_VERSION, (u_char *)ver));
	if (ret < 0) {
		perror("ioctl");
		close(fd);
		return;
	}
	close(fd);
	sprintf(fwver, "%02x:%02x:%02x:%02x:%02x:%02x",
			ver[0], ver[1], ver[2], ver[3], ver[4], ver[5]);
	fprintf(stdout, "%s\n", fwver);
}

const static char *ega_ctrl_dev = "/dev/hidraw0";

static void fetch_tpver_egalax(char *fwver)
{
	int fd;
	fd_set rfds;
	struct timeval tv;
	int retval;
	struct hidraw_devinfo dinfo;
	char buf[SMALL_STR];
	char *p;

	fwver[0] = '\0';

    fd = open(ega_ctrl_dev, O_RDWR|O_NONBLOCK);
	if (fd < 0) {
		perror("open");
		return;
	}

	if (ioctl(fd, HIDIOCGRAWINFO, &dinfo) < 0) {
		perror("ioctl");
		close(fd);
		return;
	}
	printf("hidraw0: bustype=%d vendor=0x%04x product=0x%04x\n",
			dinfo.bustype, dinfo.vendor, dinfo.product);

	buf[0] = '\0';
	buf[1] = '\3';
	buf[2] = '\1';
	buf[3] = 'D';

	SYSCALL(retval = write(fd, buf, 4));
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	SYSCALL(retval = select(fd+1, &rfds, NULL, NULL, &tv));
	if (retval > 0) {
		SYSCALL(retval = read(fd, buf, SMALL_STR));
		if (retval > 0) {
			printf("%02x,%02x,%02x,%02x,%02x,...\n",
				buf[0], buf[1], buf[2], buf[3], buf[4]);
			p = strchr(buf, 'D');
			if (p)
				strcpy(fwver, p+1);
		}
	}
	close(fd);
}

void fetch_egver(char *);
//	20121002VACS
//
//	tpfirmware version fetcher
//
void
fetch_tpver(char *kver)
{
	char buf[SMALL_STR];
	char egb[SMALL_STR];

	sleep(2);
	if (g_board_type == WPC_BOARD_TYPE_J) {
		fetch_tpver_egalax(buf);
		fetch_egver(egb);
		strcat(buf, "\n");
		strcat(buf, egb);
	} else if (g_board_type == WPC_BOARD_TYPE_J3 ||
			g_board_type == WPC_BOARD_TYPE_J4)
		fetch_tpver_melfas(buf);
	else
		strcpy(buf, "unknown");
	sprintf(kver, "TP FW Version:%s", buf);
	return;
}

//	20121002VACS
//
//	eGesture version fetcher
//
void
fetch_egver(char *kver)
{
	FILE *fp;
	char *cmd = "eGestured -v";
	char buf[SMALL_STR];

	if ( (fp=popen(cmd,"r")) ==NULL) {
		sprintf(kver, "eGestured version Unknown");
		pclose(fp);

		return;
	}
	while(fgets(buf, SMALL_STR, fp)!=NULL){
		if(strstr(buf,"eGestured version")){
			sprintf(kver, "%s", buf+1);
			pclose(fp);

			return;
		}
	}
	pclose(fp);
	sprintf(kver, "eGestured version Unknown");
}

/*
 * selfcheck package version
 */
void fetch_pkgver(char *kver)
{
	int		fd, n, i;
	char	*p;
	char	tmps[SMALL_STR];

	tmps[0]='\0'; kver[0]='\0';

	printf("%s()\n", __func__);
	fd = open("/mnt1/usb-version", O_RDONLY);
	if (fd < 0) {
		printf("%s() open error\n", __func__);
		strcpy(kver, "Unknown (could not open file)");
		return;
	}
	if ((n = read(fd, tmps, SMALL_STR)) <= 0) {
		printf("%s() read error\n", __func__);
		strcpy(kver, "Unknown (could not read file)");
		goto fin;
	}
	tmps[n]='\0';

	p = strstr(tmps, "VERSION :");
	if ((p != NULL) && (*p != '\0')) {
		snprintf(kver, SMALL_STR, "%s", p);
		for (i = 0; i < strlen(kver); i++) {
			if ((kver[i] == 0x0d) || (kver[i] == 0x0a)) {
				kver[i]='\0';
				break;
			}
		}
	} else {
		printf("%s() format error\n", __func__);
		strcpy(kver, "Unknown selfcheck package version");
	}

fin:
	close(fd);
}


//
//	a menu of version
//
int version_main(GtkWidget *table, GtkWidget *bsub)
{
/*	20110826VACS	*/
char	tmps[MID_STR], kver[SMALL_STR],  kver21[SMALL_STR], kver22[SMALL_STR], kver3[SMALL_STR], kver4[SMALL_STR];
char	tpver[SMALL_STR], egver[SMALL_STR];
GtkWidget	*v0, *bb, *a1, *lb;
struct	utsname	u;

	if(uname(&u)<0){
		strcpy(kver, "Unknown (failed uname())");
	}else{
		strcpy(kver, u.version);
	}

	/*	20110826VACS	*/
	fetch_kver2(kver21, kver22);
	fetch_rfsver(kver3);
	fetch_pkgver(kver4);

	/*	20121002VACS	*/
	fetch_tpver(tpver);
//	fetch_egver(egver);

	v0=gtk_vbox_new(FALSE, 10);
	a1=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	lb=gtk_label_new("");

	/*	20110827VACS	*/
	/* RH */
	sprintf(tmps, "%s\n"
				  "usb package %s\n\n\n"
				  "%s\n%s\n%s\n\n"
				  "root filesystem %s\n%s\n",
			sc_version_str, kver4, kver21, kver, kver22, kver3, tpver);

	gtk_label_set_markup(GTK_LABEL(lb), tmps);
	gtk_container_add(GTK_CONTAINER(a1), lb);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	bb=sc_bbox2(NULL, bsub, gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(v0);
	
	gtk_main();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	return 0;
}

#if	0	/*	20111011VACS	*/
//	20110805VACS
//
//	not Support
//
int notsupport_main(GtkWidget *p, GtkWidget *bsub)
{
	char tmps[SMALL_STR];
	GtkWidget *v0, *bb, *a1, *lb;
	
	v0=gtk_vbox_new(FALSE, 10);
	a1=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	lb=gtk_label_new("");
	sprintf(tmps, "This test is not supported. Please push the Quit key.");
	gtk_label_set_markup(GTK_LABEL(lb), tmps);
	gtk_container_add(GTK_CONTAINER(a1), lb);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	bb=sc_bbox2(NULL, bsub, gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(v0);
	gtk_main();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	return 0;
}
#endif

//	20110804VACS
//
//	demo application
//
int demo_main(GtkWidget *table, GtkWidget *bsub)
{
	char tmps[SMALL_STR];

	/*	script start	*/
	strcpy(tmps,"/mnt1/bin/newsc_demoapp.sh");
	system(tmps);

	return 0;
}

//	20110819VACS
//
//	wlantool application
//
int wlantool_main(GtkWidget *table, GtkWidget *bsub)
{
	char tmps[SMALL_STR];

	/*	script start	*/
	strcpy(tmps,"/mnt1/bin/newsc_wlantool.sh");
	system(tmps);

	return 0;
}

/* RH */
int display_main(GtkWidget *table, GtkWidget *bsub)
{
	char tmps[SMALL_STR];
	int flg;

	sprintf(tmps, "xlib_lcdcheck -j %s/jpg", base_path);
	if (interval)
		append_sprintf(tmps, " -I %d", interval);
	flg = 0;								/*	[LCD Display] test	*/
	append_sprintf(tmps, " -a %d", flg);
	system(tmps);
	return 0;
}

/* RH */
int lcdinspect_main(GtkWidget *table, GtkWidget *bsub)
{
	char tmps[SMALL_STR];
	int flg;

	sprintf(tmps, "xlib_lcdcheck -j %s/jpg", base_path);
	if (interval)
		append_sprintf(tmps, " -I %d", interval);
	flg=1;								/*	[LCD Inspect] test	*/
	append_sprintf(tmps, " -a %d", flg);
	system(tmps);
	return 0;
}

//
//	mouse cursor setup (unused)
//
void set_mouse_cursor()
{
	Display *d;
	Cursor c;
	
	d=XOpenDisplay(NULL);
	c=XCreateFontCursor(d, XC_arrow);
	XDefineCursor(d, 0, c);
}

//	20121002VACS
//
//	battery charge on
//
void
bat_charge_on()
{
	int fd;
	int err;

	fd = wpcio_open(WPCIO_OPEN_RETRY, "bat");
	if(fd<0){
		printf("BAT device open error = %d\n", fd);
		return;
	}

	/*	BAT1 charge on	*/
	err = ioctl(fd, WPC_SET_GPIO_OUTPUT_HIGH, 65);
	if(err<0){
		printf("BAT1 charge on error, code = %d\n", err);
	}
	usleep(100000);

	if (g_board_type != WPC_BOARD_TYPE_J) {
		close(fd);
		return;
	}

	/*	BAT2 charge on	*/
	err = ioctl(fd, WPC_SET_GPIO_OUTPUT_HIGH, 54);
	if(err<0){
		printf("BAT2 charge on error, code = %d\n", err);
	}
	usleep(100000);
	close(fd);
}

static int Base_path_device(char *path)
{
	struct stat stp, stc;
	char buf[256];
	int r;

	if (stat(path, &stc) != 0)
		return 0;
	if (!S_ISDIR(stc.st_mode))
		return 0;
	memset(buf, 0, sizeof(buf));
	strncpy(buf, path, sizeof(buf) - 4);
	strcat(buf, "/..");
	if (stat(buf, &stp) != 0)
		return 0;
	r = (stc.st_dev != stp.st_dev) ||
		(stc.st_dev == stp.st_dev && stc.st_ino == stp.st_ino);
	if (!r)
		return 0;
	return major(stc.st_dev);
}

//
//	the starting point
//
int main(int argc, char *argv[])
{
	int c, gargc=1, st_exit=0, menu_by_arg=0, ival=-1;
//	int	flg=0;	/*	20110822VACS	*/
	GtkWidget *window, *v0, *v1, *lb, *a, *bb, *tr, *lb_mac, *al_mac;
	GtkWidget *table;
	GtkWidget *vx, *lb_top, *fr, *batsub;
	int i;
	char macstr[SMALL_STR];
	unsigned char macaddr[SMALL_STR];
	
	char *menupath=NULL;//, tmps[SMALL_STR];
	pthread_t th_bsub;
	GdkScreen *gscr;
	
	if (sc_IsJ4()) {
		bl_toggle_charge = 0;
	}
	
	base_path="/mnt1";
	log_dir="/media/sda1/log";
	while(1){
		c=getopt(argc, argv, "M:T:I:L:b:d:gf:t:i:");
		if(c==-1) break;
		switch(c){
		case 'I':
			interval=atoi(optarg);
			break;
		case 'M':
			menupath=strdup(optarg);
			menu_by_arg=1;
			break;
		case 'T':
			testtime=atoi(optarg);
			if(testtime==0)
				testtime=INT_MAX-time(NULL)-100;
			break;
		case 'L':
			log_dir=strdup(optarg);
			break;
		case 'b':
			base_path=strdup(optarg);
			break;
		case 'd':
			debug_level=atoi(optarg);
			break;
		case 'g':
			ignore_count=1;
			break;
		case 't':
			ival=atoi(optarg);
			break;
		case 'i':
			i2c_main_auto_menu = atoi(optarg);
			break;
		}
	}
	if(menupath&&ignore_count){
		printf("Canceld -g option\n");
		ignore_count=0;
	}

	g_board_type = sc_get_board_type();
	bat_charge_on();

	if (Base_path_device(base_path) == 179) {	/* RH */
		/* selfcheck on SD */
		usbmem_first_device = 'a';
	}

	//set_mouse_cursor();
	
	g_thread_init(NULL);
	gdk_threads_init();
	gdk_threads_enter();
	
	gtk_init(&gargc, &argv);

	gscr = gdk_screen_get_default();
	gint w = gdk_screen_get_width(gscr);
	gint h = gdk_screen_get_height(gscr);
	printf("Selfcheck screen size %dx%d\n", w, h);
	set_sc_window_width(w);
	set_sc_window_height(h);

	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(window, w, h);
	
	table = sc_table_new();
	
	vx=gtk_vbox_new(FALSE, 8);
	v0=gtk_vbox_new(FALSE, 10);
	
	lb_top=lb_menutop=gtk_label_new("");
	fr=gtk_frame_new(NULL);
	gtk_widget_set_usize(fr, 190, 40);
	gtk_container_add(GTK_CONTAINER(fr), lb_top);
	gtk_box_pack_start(GTK_BOX(vx), fr, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vx), v0);
	
	sc_mainpanel(window, sc_version_str, vx, table);
	
	tr=sc_sidetree(&menupath);
	gtk_box_pack_start(GTK_BOX(v0), tr, TRUE, TRUE, 0);
	
	batsub=sc_battery_box_new();
	battery_sub_run=1;
	pthread_create(&th_bsub, NULL, battery_sub_update, 0);
	
	check_wlan(); /* wifisub.c *//* moved from wifi.c*/

	macstr[0] = '\0';
	if (get_mac_addr(macaddr) == 0)
		sprintf(macstr, "MAC Address : <span font_desc=\"monospace bold 20.0\""
				" foreground=\"red\">%02X-%02X-%02X-%02X-%02X-%02X</span>",
				macaddr[0], macaddr[1], macaddr[2],
				macaddr[3], macaddr[4], macaddr[5]);

	while(1){
		gtk_label_set_text(GTK_LABEL(lb_top), SC_TITLE);
		lb=gtk_label_new(sc_version_str);
		a=gtk_alignment_new(0.5, 0.9, 0.0, 0.0);
		lb_mac = gtk_label_new("");
		gtk_label_set_markup(GTK_LABEL(lb_mac), macstr);
		al_mac = gtk_alignment_new(0.5, 0.2, 0.0, 0.0);
		bb=sc_bbox2(&st_exit, batsub, gtk_button_new_from_stock("gtk-quit"), sc_bbox1_click);
		v1=gtk_vbox_new(FALSE, 10);
		
		gtk_widget_set_sensitive(tr, TRUE);
		gtk_container_add(GTK_CONTAINER(a), lb);
		gtk_container_add(GTK_CONTAINER(al_mac), lb_mac);
		gtk_container_add(GTK_CONTAINER(v1), a);
		gtk_container_add(GTK_CONTAINER(v1), al_mac);
		gtk_box_pack_start(GTK_BOX(v1), bb, FALSE, FALSE, 0);
		sc_table_attach2(GTK_TABLE(table), v1);
		if (!menu_by_arg)
		gtk_widget_show_all(window);
		
		if(!menupath){
			gtk_main();
			if(st_exit){
				if(sc_message_yesno("Are you OK to leave SelfChecker?")==1)
					goto fin;
				else{
					sc_bbox2_remove(batsub);
					gtk_widget_destroy(v1);
					continue;
				}
			}
		}
		sc_bbox2_remove(batsub);
		gtk_widget_destroy(v1);
		gtk_widget_set_sensitive(tr, FALSE);
		sc_gtk_update();
		if(!menupath){
			printf("ERROR : Could not get menupath.\n");
			exit(1);
		} else {
		printf("menupath=[%s].\n", menupath);
			// RH
			for (i = 0; side_menu_list[i].menu_label != NULL; i++) {
				if (side_menu_list[i].tree_id == NULL)
					continue;
				if (!side_menu_list[i].enable)
					continue;
				if (side_menu_list[i].func == NULL)
					continue;
				if (strcmp(menupath, side_menu_list[i].tree_id) == 0) {
					if (side_menu_list[i].title_label)
						gtk_label_set_text(GTK_LABEL(lb_top),
								side_menu_list[i].title_label);
					if (side_menu_list[i].func == tpfunc_main) {
						side_menu_list[i].func(table, (GtkWidget*)ival);
					} else
						side_menu_list[i].func(table, batsub);
					break;
				}
			}
		}

		menupath=NULL;
		testtime=0;
		if(menu_by_arg) break;
	}
	
fin:	
	gdk_threads_leave();
	battery_check_logger_stop();
	battery_sub_run=0;
	if(udhcpc_kill_check){
		system("killall udhcpc");
		system("killall wpa_supplicant");
		usleep(200000);
		system("killall -9 udhcpc");
		system("killall -9 wpa_supplicant");
	}
	pthread_join(th_bsub, NULL);
    return 0;
}
