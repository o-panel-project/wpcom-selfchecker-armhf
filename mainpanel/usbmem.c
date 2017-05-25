/*
 *		USB memory test
 *
 *					2011.6	written by Omoikane Inc.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <gtk/gtk.h>
#include "common.h"
#include "wpcio.h"
#include "sc_i2c.h"


/*
 * If selfcheck is on uSD, then usbmem_first_device is set to 'a'
 * at main()@mainpanel.c
 */
char usbmem_first_device = 'b';	/* RH */

static char *dest_dir=NULL;
static char src_path[SMALL_STR], dest_path[SMALL_STR];
static GtkWidget *lb_result, *lb_i2cstat, *pb, *fc;
static GtkWidget *lb_error_rate;
static GtkWidget *b_start, *b_stop, *b_quit, *b_mount;
static int time_log_run=0;
static time_t t_fin=0;
static int script_exitcode=-1, script_count=1;	/*	20110915VACS	*/
static int pid_script=0;

static int usbmem_loop_run=0; // 0:stop, 1:single run, 2:loop run
static int fd_progress=-1;
static int progress_count=0;
static int progress_total=0;

/*	20110907VACS	*/
static int confirm_run=0;
static int confirm_stop=0;
static int cr_i2c_ok_old=0;

/*	20110920VACS	*/
static	int test_mode=0;
static	int	file_cnt=1;
static char file_cnt_name[SMALL_STR];

static	int loop_testing=0;										/*	20110927VACS	*/
static	int	press_mount_cradle_ing=0;							/*	20110929VACS	*/

static	int tatal_count, error_count;

#define MOUNT_BUTTON_MOUNT "Mount USB Memory"
#define MOUNT_BUTTON_UNMOUNT "Unmount USB Memory"
#define MOUNT_BUTTON_MOUNT30 "Mount 30sec"
#define MOUNT_BUTTON_UNMOUNT30 "Unmount 30sec"					/*	20110908VACS	*/
#define	CRADLE_TEST_FILE_CNT_NAME	"/mnt1/var/cradle_testfile_cnt"	/*	20110921VACS	*/
#define	MAIN_TEST_FILE_CNT_NAME		"/mnt1/var/usbmem_testfile_cnt"	/*	20110921VACS	*/
#define	EXWIFI_TEST_FILE_CNT_NAME	"/mnt1/var/exwifi_testfile_cnt"	/*	20110921VACS	*/
#define	TEST_FILE_CNT_MAX			999							/*	20110921VACS	*/

//	20110824VACS, RH
static	void
usbmem_init_test_path_usbmem(void)
{
	sprintf(src_path, "%s/data/usbmem-test.dat", base_path);
	dest_dir="/mnt1/usbmem";
	sprintf(dest_path, "%s/testfile%03d", dest_dir, file_cnt);	/*	20110920VACS	*/
	debug_printf(5, "main test file:(%s)\n", dest_path);		/*	20110921VACS	*/
}

//	20110831VACS
static	void
usbmem_init_test_path_exwifi(void)
{
	sprintf(src_path, "%s/data/usbmem-test.dat", base_path);
	dest_dir="/mnt1/exwifi";
	sprintf(dest_path, "%s/testfile%03d", dest_dir, file_cnt);	/*	20110920VACS	*/
	debug_printf(5, "exwifi test file:(%s)\n", dest_path);		/*	20110921VACS	*/
}

//	201109011VACS
static	void
usbmem_init_test_path_cradle(void)
{
	sprintf(src_path, "%s/data/usbmem-test.dat", base_path);
	dest_dir="/mnt1/cradle";
	sprintf(dest_path, "%s/testfile%03d", dest_dir, file_cnt);	/*	20110920VACS	*/
	debug_printf(5, "cradle test file:(%s)\n", dest_path);		/*	20110921VACS	*/
}

//	20110920VACS
void
test_file_unlink(void)
{

	if(access(dest_path, 0)==0){
		unlink(dest_path);
		sync();
		debug_printf(5, "unlink(%s)\n", dest_path);				/*	20110921VACS	*/
	}
	usleep(200000);
	return;
}

//	20110920VACS
void
test_file_cnt_read(void)
{
int		iwork;
char	str[SMALL_STR];
FILE	*fp;

	file_cnt=1;
	switch(test_mode){
		case	1:	/*	cradle		*/
			strcpy(file_cnt_name, CRADLE_TEST_FILE_CNT_NAME);	break;
		case	2:	/*	main body	*/
			strcpy(file_cnt_name, MAIN_TEST_FILE_CNT_NAME);		break;
		case	3:	/*	exwifi		*/
			strcpy(file_cnt_name, EXWIFI_TEST_FILE_CNT_NAME);	break;
		default:
			strcpy(file_cnt_name, MAIN_TEST_FILE_CNT_NAME);		break;
	}

	debug_printf(5, "read:file_cnt_name(%s)\n", file_cnt_name);	/*	20110921VACS	*/
	if(access(file_cnt_name, 0)==0){
		if((fp=fopen(file_cnt_name, "r"))==NULL){
			debug_printf(5, "read:open error\n");				/*	20110921VACS	*/
			return;
		}

		while(!feof(fp)){
			fgets(str, SMALL_STR, fp);
			iwork=atoi(str);
			if(iwork>0 && iwork<=TEST_FILE_CNT_MAX){
				file_cnt=iwork;
				break;
			}
		}
		fclose(fp);
	}

	debug_printf(5, "read:file cnt(%d)\n", file_cnt);			/*	20110921VACS	*/
	return;
}

//	20110920VACS
void
test_file_cnt_check(void)
{
int		i;
int		flg;
char	str[SMALL_STR];

	flg=0;
	for(i=file_cnt; i<=TEST_FILE_CNT_MAX; i++){
		switch(test_mode){
			case	1:	/*	cradle		*/
				sprintf(str, "/mnt1/cradle/testfile%03d", file_cnt);	break;
			case	2:	/*	main body	*/
				sprintf(str, "/mnt1/usbmem/testfile%03d", file_cnt);	break;
			case	3:	/*	exwifi		*/
				sprintf(str, "/mnt1/exwifi/testfile%03d", file_cnt);	break;
			default:
				sprintf(str, "/mnt1/usbmem/testfile%03d", file_cnt);	break;
		}

#if	0
printf("check:str(%d)(%s)\n", test_mode, str);	/*debug*/
#endif

		if(access(str, 0)==0){
			file_cnt++;
		}else{
			flg=1;
			break;
		}

		usleep(100000);
	}

	if(!flg)	file_cnt=1;
	debug_printf(5, "check:file cnt(%d)\n", file_cnt);					/*	20110921VACS	*/

	switch(test_mode){
		case	1:	/*	cradle		*/
			usbmem_init_test_path_cradle();	break;
		case	2:	/*	main body	*/
			usbmem_init_test_path_usbmem();	break;
		case	3:	/*	exwifi		*/
			usbmem_init_test_path_exwifi();	break;
		default:
			usbmem_init_test_path_usbmem();	break;
	}

	debug_printf(5, "check:dest_path(%d)(%s)\n", test_mode, dest_path);	/*	20110921VACS	*/

	return;
}

//	20110920VACS
void
test_file_cnt_write(void)
{
FILE	*fp;

	debug_printf(5, "write:file_cnt_name(%s)(%d)\n", file_cnt_name, file_cnt);	/*	20110921VACS	*/

	if((fp=fopen(file_cnt_name, "w+"))==NULL){
		debug_printf(5, "write:open error\n");									/*	20110921VACS	*/
		return;
	}

	if((fprintf(fp, "%d\n", file_cnt))<=0){
		debug_printf(5, "write:write error\n");									/*	20110921VACS	*/
	}
	fclose(fp);
	sync();

	return;
}

void usbmem_result_color(char *c, char *s)
{
	char tmps[SMALL_STR];
	
	sprintf(tmps, "<span color=\"%s\">%s</span>", c, s);
	gtk_label_set_markup(GTK_LABEL(lb_result), tmps);
}

#define usbmem_result_red(s)  usbmem_result_color("red", s)
#define usbmem_result_blue(s) usbmem_result_color("blue", s)

//
//	launch test program(=cpprogress)
//
static int usbmem_test_start()
{
	char tmps[SMALL_STR];
	
	switch(pid_script=fork()){
	case -1:
		pid_script=0;
		return -1;
	
	case 0:
		setpgid(getpid(),getpid());
		sprintf(tmps, "%s/bin/cpprogress 1048576 %s %s >/tmp/progress.log", base_path, src_path, dest_path);
		execl("/bin/sh", "sh", "-c", tmps, NULL);
		_exit(127);
		break;
	}
	progress_total=5;
	return 0;
}

void increment_progress(int n)
{
	char tmps[SMALL_STR];
	
	progress_count+=n;
	gtk_progress_bar_update(GTK_PROGRESS_BAR(pb),
							min(1.0, 1.0*progress_count/progress_total));
	if(script_count){
		sprintf(tmps, "Loop Test #%d", script_count);
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(pb), tmps);
	}
}

//
//	reflect output of test program to progress bar
//
void read_progress_log()
{
	int n;
	char tmps[SMALL_STR];
	
	if(0<(n=read(fd_progress, tmps, 2))){
		tmps[n]='\0';
		increment_progress(atoi(tmps));
	}
}

static void press_stop(GtkWidget *widget, gpointer data)
{
	gtk_widget_set_sensitive(b_stop, FALSE);
	usbmem_loop_run=0;
	if(pid_script){
		do_killpg(pid_script);
		pid_script=0;
		usbmem_result_red("*** Test Aborted ***");
		progress_count=progress_total=0;
	}
	if(0<fd_progress){
		close(fd_progress);
		fd_progress=-1;
	}
	time_log_run=0;
	script_count=1;		/*	20110915VACS	*/

	test_file_unlink();	/*	20110920VACS	*/

	/*	20111004VACS	*/
	if(test_mode==1){
		if(confirm_run)	gtk_widget_set_sensitive(b_start, TRUE);
	}else{
		gtk_widget_set_sensitive(b_start, TRUE);
	}

	gtk_widget_set_sensitive(b_quit, TRUE);

	/*	20111004VACS	*/
	if(test_mode==1){
			if(b_mount)		gtk_widget_set_sensitive(b_mount, TRUE);
	}else{
		if(b_mount)	gtk_widget_set_sensitive(b_mount, TRUE);
	}

	loop_testing=0;		/*	20110927VACS	*/
}
static gboolean press_stop_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_stop(widget, data);
	return FALSE;
}

int usbmem_check_timeup()
{
	if(t_fin && t_fin<time(NULL)){
		press_stop(NULL, 0);
		gtk_main_quit();
		return 1;
	}
	return 0;
}

void usbmem_exitcode_check()
{
	int st;
	
	if(script_exitcode<0 && pid_script==waitpid(pid_script, &st, WNOHANG)){
		if(WIFEXITED(st))
			script_exitcode=WEXITSTATUS(st);
	}
}

// phase1 : need to start test program
static gboolean usbmem_loop_phase1()
{
	char tmps[SMALL_STR];

	script_exitcode=-1;	
	gtk_progress_bar_update(GTK_PROGRESS_BAR(pb), 0.0);
	sprintf(tmps, "*** Normal Test Start ***%s", usbmem_loop_run==1 ? "" : "(loop)");
	gtk_label_set_text(GTK_LABEL(lb_result), tmps);
	usbmem_test_start();
	usleep(200000);
	if(have_proc_pid(pid_script))
		return TRUE;
	
	usbmem_result_red("[ USB Memory script error ]");
	return FALSE;
}

// phase2 : script not alive.
static gboolean usbmem_loop_phase2()
{
	usbmem_exitcode_check();
	
	if(fd_progress<0)
		fd_progress=open("/tmp/progress.log", O_RDONLY|O_NONBLOCK);
	else
		read_progress_log();
	
	return TRUE;
}

// phase3 : script finished
static gboolean usbmem_loop_phase3()
{
	char tmps[SMALL_STR], *result_text;
	unsigned char digest_src[16], digest_dest[16];
	
	if(0<fd_progress){
		read_progress_log();
		close(fd_progress);
		fd_progress=-1;
		progress_count=progress_total=0;
	}
	
	usbmem_exitcode_check();
	if(script_exitcode!=0){
		if(0<script_exitcode){
			sprintf(tmps, "[ USB Memory script error (%d) ]", script_exitcode);
		}else{
			sprintf(tmps, "[ USB Memory script unknown error ]");
		}
		usbmem_result_red(tmps);
		return FALSE;
	}
	
	// md5sum comparison here
	if(!get_md5sum(src_path, digest_src)){
		usbmem_result_red("[ Data file open error(0) ]");
		return FALSE;
	}
	if(!get_md5sum(dest_path, digest_dest)){
		usbmem_result_red("[ Data file open error(1) ]");
		return FALSE;
	}
	increment_progress(1);
	
	strcpy(tmps, "usbmem md5sum : ");
	append_hex(tmps, 16, (char *)digest_src);
	strcat(tmps, ", ");
	append_hex(tmps, 16, (char *)digest_dest);
	debug_printf(3, "%s\n", tmps);
	
	if(memcmp((void *)digest_src, (void *)digest_dest, 16)!=0){
		result_text="[ MD5SUM Error ]";
		usbmem_result_red(result_text);
		return FALSE;
	}else{
		result_text="[ OK ]";
		usbmem_result_blue(result_text);
	}
	
	gtk_progress_bar_update(GTK_PROGRESS_BAR(pb), 1.0);
	
	pid_script=0;
	if(usbmem_loop_run==2){ // continue to next loop
		script_count++;
		return TRUE;
	}
	
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_widget_set_sensitive(b_start, TRUE);
	gtk_widget_set_sensitive(b_quit, TRUE);
	
	usbmem_loop_run=0; // eof single script
	return FALSE;
}

//
//	main loop : watching child test program
//
static gboolean usbmem_loop(gpointer data)
{
	gboolean ret;
	char tmps2[SMALL_STR];
	
	if(!usbmem_loop_run)
		return FALSE;
	
	if(usbmem_check_timeup())
		return FALSE;
	
	// phase1 : need to start test script
	if(!pid_script)
		return usbmem_loop_phase1();
	
	// phase2 : script alive?
	if(have_proc_pid(pid_script))
		return usbmem_loop_phase2();
	
	// phase3 : script finished
	
	tatal_count++;
	ret = usbmem_loop_phase3();
	if (ret == FALSE) {
		error_count++;
	}
	sprintf(tmps2, "Total = %d : Error = %d : Error rate = %d %%", 
		tatal_count, error_count, (tatal_count==0)? 0 : (int)((double)error_count/tatal_count*100));
	gtk_label_set_text(GTK_LABEL(lb_error_rate), tmps2);
	
	return ret;
}

static gboolean time_log(gpointer data)
{
	if(!time_log_run) return FALSE;
	
	if(t_fin && t_fin<time(NULL)){
		press_stop(NULL, 0);
		gtk_main_quit();
		script_count=1;		/*	20110915VACS	*/
		return FALSE;
	}
	return TRUE;
}

static void start_common(int mode)
{
	gtk_widget_set_sensitive(b_start, FALSE);
	gtk_widget_set_sensitive(b_quit, FALSE);

	/*	20111004VACS	*/
	if(b_mount)	gtk_widget_set_sensitive(b_mount, FALSE);

	usbmem_loop_run=mode;
	g_timeout_add(200, usbmem_loop, 0);
	gtk_widget_set_sensitive(b_stop, TRUE);
}

#if	0
static void press_single(GtkWidget *widget, gpointer data)
{
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(pb), "");
	script_count=0;
	start_common(1);
}
#endif

static void press_loop(GtkWidget *widget, gpointer data)
{
	error_count  = 0;
	tatal_count  = 0;

	if((!cr_mount_ok) && (test_mode==1))	return;	/*	20111004VACS	*/

	test_file_cnt_check();							/*	20110920VACS	*/

	start_common(2);
	time_log_run = 1;
	script_count = 1;
	g_timeout_add(16*100000, time_log, 0);

	if(test_mode==1)	loop_testing=1;				/*	20111004VACS	*/
}
static gboolean press_loop_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_loop(widget, data);
	return FALSE;
}


static void press_b_start(GtkWidget *widget, gpointer data)
{
	error_count  = 0;
	tatal_count  = 0;
	press_loop(widget, data);
}
static gboolean press_b_start_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_b_start(widget, data);
	return FALSE;
}


static char basedrive_letter='\0';
static char targetdrive_letter='\0';

//	20110824VACS
//
//	find main body USB memory which includes selfchecker itself
//
static	int
usbmem_search_basedrive_main_body(void)
{
int		ret=0;
char	d='1', path[SMALL_STR];

	for(d='1';d<'9';d++){
		sprintf(path, "/mnt%c", d);
		if(is_mountpoint(path)){
			if(basedrive_letter){
				debug_printf(3, "Error: base drive could not determin (mnt%c and mnt%c)\n", basedrive_letter, d);
				ret=0;
				continue;
			}
			basedrive_letter=d;
			debug_printf(3, "usbmem : base drive found (mnt%c)\n", basedrive_letter);
			ret=1;
		}
	}
	if(!ret)
		debug_printf(3, "usbmem : No base drive was set. (mnt?)\n");
	return	ret;
}

static	int
usbmem_search_basedrive_usbmem(void)
{
	int		ret = 0;
	char	d = '1', path[SMALL_STR];

	strcpy(path, "/mnt1/usbmem");
	if (is_mountpoint(path)) {
		if (basedrive_letter) {
			debug_printf(3,
				"Error: base drive could not determin (mnt1/usbmem)\n");
			ret = 0;
		} else {
			basedrive_letter = d;
			debug_printf(3, "usbmem : base drive found (mnt1/usbmem)\n");
			ret = 1;
		}
	}
	if (!ret)
		debug_printf(3, "usbmem : No base drive was set. (mnt1/usbmem)\n");
	return	ret;
}

//	20110831VACS
//
//	find ExWifi USB memory which includes selfchecker itself
//
static	int
usbmem_search_basedrive_exwifi(void)
{
int		ret=0;
char	d='1', path[SMALL_STR];

	strcpy(path, "/mnt1/exwifi");
	if(is_mountpoint(path)){
		if(basedrive_letter){
			debug_printf(3, "Error: base drive could not determin (mnt1/exwifi)\n");
			ret=0;
		}else{
			basedrive_letter=d;
			debug_printf(3, "usbmem : base drive found (mnt1/exwifi)\n");
			ret=1;
		}
	}
	if(!ret)
		debug_printf(3, "usbmem : No base drive was set. (mnt1/exwifi)\n");
	return	ret;
}

//	20110902VACS
//
//	find Cradle USB memory which includes selfchecker itself
//
static	int
usbmem_search_basedrive_cradle(void)
{
int		ret=0;
char	d='1', path[SMALL_STR];

	strcpy(path, "/mnt1/cradle");
	if(is_mountpoint(path)){
		if(basedrive_letter){
			debug_printf(3, "Error: base drive could not determin (mnt1/cradle)\n");
			ret=0;
		}else{
			basedrive_letter=d;
			debug_printf(3, "usbmem : base drive found (mnt1/cradle)\n");
			ret=1;
		}
	}
	if(!ret)
		debug_printf(3, "usbmem : No base drive was set. (mnt1/cradle)\n");
	return	ret;
}

/* RH */
static int usbmem_search_newdrive(const char *path)
{
	int		ret;
	char	d, device[SMALL_STR];

	if (is_mountpoint(path))
		return	1;

	for (d = usbmem_first_device; d < 'f'; d++) {
		sprintf(device, "/dev/sd%c1", d);
		sleep(1);
		ret = mount(device, path, "vfat", 0, NULL);
		if (ret == 0)
			return	1;
	}
	return	0;
}

/* RH */
static int wait_for_newdrive(const char *path, int n)
{
	int	i;

	for (i = 0; i < n; i++) {
		if (usbmem_search_newdrive(path))
			return	1;
	}
	return	0;
}
#if 0
//	20110831VACS, RH
static	int
usbmem_search_newdrive_exwifi(void)
{
	return usbmem_search_newdrive("/mnt1/exwifi");
}
#endif
//	20110831VACS, RH
static	int
wait_for_newdrive_exwifi(int n)
{
	return wait_for_newdrive("/mnt1/exwifi", n);
}
#if 0
//	20110902VACS, RH
static	int
usbmem_search_newdrive_cradle(void)
{
	return usbmem_search_newdrive("/mnt1/cradle");
}
#endif
//	20110902VACS, RH
static	int
wait_for_newdrive_cradle(int n)
{
	return wait_for_newdrive("/mnt1/cradle", n);
}
#if 0
//	RH
static	int
usbmem_search_newdrive_usbmem(void)
{
	return usbmem_search_newdrive("/mnt1/usbmem");
}
#endif
//	RH
static	int
wait_for_newdrive_usbmem(int n)
{
	return wait_for_newdrive("/mnt1/usbmem", n);
}

//
//	power on usb memory port
//
static void usbmem_usbmem_set_power(int x)
{
	int fd;

	fd = wpcio_open(WPCIO_OPEN_RETRY, "usbmem_usbmem_set_power");
	if (fd < 0) return;

	if (ioctl(fd, WPC_CONNECT_USB_MEMORY, x) < 0)
		debug_printf(3, "WPC_CONNECT_USB_MEMORY ioctl failure.");

	close(fd);
}

//
//	power on usb exwifi port
//
static void usbmem_exwifi_set_power(int x)
{
	int fd;
	
	fd=wpcio_open(WPCIO_OPEN_RETRY, "usbmem_exwifi_set_power");
	if(fd<0) return;
	
	if(ioctl(fd, WPC_CONNECT_USB_WIFI, x)<0)
		debug_printf(3, "WPC_CONNECT_USB_WIFI ioctl failure.");
	
	close(fd);
}

//	20110824VACS, RH
//
//	set appropriate main body directory for filelist.
//
static	void
set_fc_drive_main_body(char d)
{
char	path[SMALL_STR];

	if (usbmem_first_device == 'a') {
		/* selfcheck on uSD */
		if (d)
			strcpy(path, "/mnt1/usbmem");
		else
			strcpy(path, "/mnt1");
	} else {
		if(d)
			strcpy(path, "/mnt1");
		else
			strcpy(path, "/");
	}

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fc), path);
}

static void set_fc_drive_usbmem(char d)
{
	set_fc_drive_main_body(d);
}

//	20110831VACS
//
//	set appropriate ExWifi directory for filelist
//
static	void
set_fc_drive_exwifi(char d)
{
char	path[SMALL_STR];

	if(d)
		strcpy(path, "/mnt1/exwifi");
	else
		strcpy(path, "/");

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fc), path);
}

//	20110901VACS
//
//	set appropriate Cradle directory for filelist
//
static	void
set_fc_drive_cradle(char d)
{
char	path[SMALL_STR];

	if(d)
		strcpy(path, "/mnt1/cradle");
	else
		strcpy(path, "/");

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fc), path);
}

//	20110831VACS
static	void
press_mount_hub4(GtkWidget *widget, gpointer data)
{
int		ret;
char	path[SMALL_STR];

	gtk_widget_set_sensitive(widget, FALSE);
	sc_gtk_update();
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))==TRUE){
		debug_printf(5, "usbmem mount start... (/mnt1/exwifi)\n");
		gtk_widget_set_sensitive(b_quit, FALSE);	/*	20111004VACS	*/

		gtk_label_set_text(GTK_LABEL(lb_result), "Confirming USB memory...");
		sc_gtk_update();
		usbmem_exwifi_set_power(1);
		sleep(1);
		ret=wait_for_newdrive_exwifi(3);
		if(!ret){
			debug_printf(5, "usbmem mount failure. (/mnt1/exwifi)\n");
			usbmem_result_red("Could not confirm USB memory");
			usbmem_exwifi_set_power(0);
			gtk_button_set_label(GTK_BUTTON(b_mount), MOUNT_BUTTON_MOUNT);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE);
		}else{
			debug_printf(5, "usbmem mount OK. (/mnt1/exwifi)\n");
			set_fc_drive_exwifi(1);
			gtk_widget_set_sensitive(fc, TRUE);
			usbmem_result_blue(" ");
			gtk_button_set_label(GTK_BUTTON(b_mount), MOUNT_BUTTON_UNMOUNT);
			gtk_widget_set_sensitive(b_start, TRUE);
		}
		gtk_widget_set_sensitive(b_quit, TRUE);		/*	20111004VACS	*/
	}else{
		test_file_unlink();							/*	20110920VACS	*/
		gtk_widget_set_sensitive(b_quit, FALSE);	/*	20111004VACS	*/

		debug_printf(5, "usbmem unmount (/mnt1/exwifi)\n");
		gtk_button_set_label(GTK_BUTTON(b_mount), MOUNT_BUTTON_MOUNT);
		gtk_widget_set_sensitive(b_start, FALSE);
		set_fc_drive_exwifi(1);
		gtk_widget_set_sensitive(fc, FALSE);
		strcpy(path, "/mnt1/exwifi");
		if(is_mountpoint(path)&&(umount(path)!=0)){
			gtk_widget_set_sensitive(b_quit, TRUE);	/*	20111004VACS	*/
			usbmem_result_red("umount error");
			debug_printf(5, "umount error (/mnt1/exwifi)\n");
			return;
		}
		gtk_widget_set_sensitive(b_quit, TRUE);		/*	20111004VACS	*/
		targetdrive_letter='\0';
		usbmem_exwifi_set_power(0);
	}
	gtk_widget_set_sensitive(widget, TRUE);
}
static gboolean press_mount_hub4_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gboolean state;
	state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_toggle_button_set_active(
		GTK_TOGGLE_BUTTON(widget), state ? FALSE : TRUE); /* toggled */
	press_mount_hub4(widget, data);
	return FALSE;
}

//
//	menu for USB(HUB-4:ExWiFi)
//
int usbmem_exwifi_main(GtkWidget *table, GtkWidget *bsub)
{
	GtkWidget *v0, *v1, *h2, *a1, *a2, *bb;

	loop_testing=0;								/*	20110927VACS	*/
	press_mount_cradle_ing=0;					/*	20110929VACS	*/

	/*	20110920VACS	*/
	test_mode=3;
	test_file_cnt_read();

	usbmem_init_test_path_exwifi();				/*	20110831VACS	*/

	v0=gtk_vbox_new(FALSE, 5);
	
	// status area
	v1=gtk_vbox_new(FALSE, 20);
	a1=gtk_alignment_new(0.5, 0.2, 0.5, 0.01);
	lb_result=gtk_label_new(" ");
	pb=gtk_progress_bar_new();
	gtk_widget_set_usize(pb, 480, 32);
	gtk_container_add(GTK_CONTAINER(v1), lb_result);

	lb_error_rate = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(v1), lb_error_rate);

	gtk_container_add(GTK_CONTAINER(v1), pb);
	
	// button
	h2=gtk_hbox_new(FALSE, 20);
	a2=gtk_alignment_new(0.5, 0.2, 0.5, 0.15);
	b_start=gtk_button_new_with_label("Test Loop Start");
	g_signal_connect(b_start, "button-release-event", G_CALLBACK(press_loop_func), 0);
	gtk_container_add(GTK_CONTAINER(h2), b_start);
	gtk_widget_set_sensitive(b_start, FALSE);
	b_stop=gtk_button_new_with_label("Test Loop Stop");
	g_signal_connect(b_stop, "button-release-event", G_CALLBACK(press_stop_func), 0);
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_container_add(GTK_CONTAINER(h2), b_stop);
	b_mount=gtk_toggle_button_new_with_label(MOUNT_BUTTON_MOUNT);
	gtk_container_add(GTK_CONTAINER(h2), b_mount);
	g_signal_connect(b_mount, "toggled", G_CALLBACK(press_mount_hub4), 0);
	//g_signal_connect(b_mount, "button-release-event", G_CALLBACK(press_mount_hub4_func), 0);
	gtk_widget_set_sensitive(b_mount, FALSE);
	
	gtk_container_add(GTK_CONTAINER(a2), h2);
	gtk_container_add(GTK_CONTAINER(v1), a2);
	gtk_container_add(GTK_CONTAINER(a1), v1);
	
	// filelist
	fc=gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_OPEN);
	
	set_fc_drive_exwifi(1);						/*	20110831VACS	*/
	
	gtk_widget_set_usize(fc, 640, 340);
	gtk_container_add(GTK_CONTAINER(v1), fc);
	gtk_widget_set_sensitive(fc, FALSE);
	
	gtk_container_add(GTK_CONTAINER(v0), a1);
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(NULL, bsub, b_quit, sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	gtk_widget_set_sensitive(b_quit, FALSE);	/*	20111004VACS	*/

	basedrive_letter=targetdrive_letter='\0';
	usbmem_exwifi_set_power(0);

	usbmem_search_basedrive_exwifi();			/*	20110831VACS	*/

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b_mount), TRUE);
	
	if(testtime){
		t_fin=time(NULL)+testtime;
		time_log_run=1;
		press_loop(NULL, 0);
	}
	gtk_main();
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b_mount))==TRUE)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b_mount), FALSE);

	test_file_cnt_write();						/*	20110920VACS	*/

	sc_gtk_update();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);

	test_mode=0;								/*	20111004VACS	*/
	return 0;
}

//
//	menu for USB(HUB-2:Memory)
//
static int usbmem_sdmem_main(GtkWidget *, GtkWidget *);
static int usbmem_usbmem_main(GtkWidget *, GtkWidget *);
int usbmem_hub2mem_main(GtkWidget *table, GtkWidget *bsub)
{
	if (usbmem_first_device == 'a')	/* selfcheck on uSD */
		return usbmem_usbmem_main(table, bsub);
	return usbmem_sdmem_main(table, bsub);
}

static int usbmem_sdmem_main(GtkWidget *table, GtkWidget *bsub)
{
	GtkWidget *v0, *v1, *h2, *a1, *a2, *bb;

	loop_testing=0;								/*	20110927VACS	*/
	press_mount_cradle_ing=0;					/*	20110929VACS	*/

	/*	20110920VACS	*/
	test_mode=2;
	test_file_cnt_read();

	usbmem_init_test_path_usbmem();			/*	20110824VACS	*/

	v0=gtk_vbox_new(FALSE, 5);
	
	// status area
	v1=gtk_vbox_new(FALSE, 20);
	a1=gtk_alignment_new(0.5, 0.2, 0.5, 0.01);
	lb_result=gtk_label_new(" ");
	pb=gtk_progress_bar_new();
	gtk_widget_set_usize(pb, 480, 32);
	gtk_container_add(GTK_CONTAINER(v1), lb_result);

	lb_error_rate = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(v1), lb_error_rate);
	
	gtk_container_add(GTK_CONTAINER(v1), pb);
	
	// button
	h2=gtk_hbox_new(FALSE, 20);
	a2=gtk_alignment_new(0.5, 0.2, 0.5, 0.15);
	b_start=gtk_button_new_with_label("Test Loop Start");
	g_signal_connect(b_start, "button-release-event", G_CALLBACK(press_loop_func), 0);
	gtk_container_add(GTK_CONTAINER(h2), b_start);
	gtk_widget_set_sensitive(b_start, FALSE);
	b_stop=gtk_button_new_with_label("Test Loop Stop");
	g_signal_connect(b_stop, "button-release-event", G_CALLBACK(press_stop_func), 0);
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_container_add(GTK_CONTAINER(h2), b_stop);
	b_mount=NULL;
	gtk_container_add(GTK_CONTAINER(a2), h2);
	gtk_container_add(GTK_CONTAINER(v1), a2);
	gtk_container_add(GTK_CONTAINER(a1), v1);
	
	// filelist
	fc=gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_OPEN);

	set_fc_drive_main_body(0);					/*	20110831VACS	*/

	gtk_widget_set_usize(fc, 640, 340);
	gtk_container_add(GTK_CONTAINER(v1), fc);
	gtk_widget_set_sensitive(fc, FALSE);
	
	gtk_container_add(GTK_CONTAINER(v0), a1);
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(NULL, bsub, b_quit, sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	usbmem_exwifi_set_power(0);
	basedrive_letter='\0';

	usbmem_search_basedrive_main_body();		/*	20110824VACS	*/

	if(!basedrive_letter){
		sc_message(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Could not find target drive.");
		sc_bbox2_remove(bsub);					/*	20110824VACS	*/
		gtk_widget_destroy(v0);
		return 1;
	}
	set_fc_drive_main_body(basedrive_letter);	/*	20110824VACS	*/

	gtk_widget_set_sensitive(fc, TRUE);
	gtk_widget_set_sensitive(b_start, TRUE);
	
	if(testtime){
		t_fin=time(NULL)+testtime;
		time_log_run=1;
		press_loop(NULL, 0);
	}
	gtk_main();

	test_file_unlink();							/*	20110920VACS	*/

	test_file_cnt_write();						/*	20110920VACS	*/

	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);

	test_mode=0;								/*	20111004VACS	*/
	return 0;
}

static void press_mount_usbmem(GtkWidget *widget, gpointer data)
{
	int		ret;
	char	path[SMALL_STR];

	gtk_widget_set_sensitive(widget, FALSE);
	sc_gtk_update();
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) == TRUE) {
		debug_printf(5, "usbmem mount start... (/mnt1/usbmem)\n");
		gtk_widget_set_sensitive(b_quit, FALSE);

		gtk_label_set_text(GTK_LABEL(lb_result), "Confirming USB memory...");
		sc_gtk_update();
		usbmem_usbmem_set_power(1);
		sleep(1);
		ret = wait_for_newdrive_usbmem(3);
		if (!ret) {
			debug_printf(5, "usbmem mount failure. (/mnt1/usbmem)\n");
			usbmem_result_red("Could not confirm USB memory");
			usbmem_exwifi_set_power(0);
			gtk_button_set_label(GTK_BUTTON(b_mount), MOUNT_BUTTON_MOUNT);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE);
		} else {
			debug_printf(5, "usbmem mount OK. (/mnt1/usbmem)\n");
			set_fc_drive_usbmem(1);
			gtk_widget_set_sensitive(fc, TRUE);
			usbmem_result_blue(" ");
			gtk_button_set_label(GTK_BUTTON(b_mount), MOUNT_BUTTON_UNMOUNT);
			gtk_widget_set_sensitive(b_start, TRUE);
		}
		gtk_widget_set_sensitive(b_quit, TRUE);
	} else {
		test_file_unlink();
		gtk_widget_set_sensitive(b_quit, FALSE);

		debug_printf(5, "usbmem unmount (/mnt1/usbmem)\n");
		gtk_button_set_label(GTK_BUTTON(b_mount), MOUNT_BUTTON_MOUNT);
		gtk_widget_set_sensitive(b_start, FALSE);
		set_fc_drive_usbmem(1);
		gtk_widget_set_sensitive(fc, FALSE);
		strcpy(path, "/mnt1/usbmem");
		if (is_mountpoint(path) && (umount(path) != 0)) {
			gtk_widget_set_sensitive(b_quit, TRUE);
			usbmem_result_red("umount error");
			debug_printf(5, "umount error (/mnt1/usbmem)\n");
			return;
		}
		gtk_widget_set_sensitive(b_quit, TRUE);
		targetdrive_letter='\0';
		usbmem_usbmem_set_power(0);
	}
	gtk_widget_set_sensitive(widget, TRUE);
}
static gboolean press_mount_usbmem_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gboolean state;
	state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_toggle_button_set_active(
		GTK_TOGGLE_BUTTON(widget), state ? FALSE : TRUE); /* toggled */
	press_mount_usbmem(widget, data);
	return FALSE;
}

//
//	menu for USB(HUB-2:Memory)
//
static int usbmem_usbmem_main(GtkWidget *table, GtkWidget *bsub)
{
	GtkWidget *v0, *v1, *h2, *a1, *a2, *bb;

	loop_testing = 0;
	press_mount_cradle_ing = 0;

	test_mode=2;
	test_file_cnt_read();

	usbmem_init_test_path_usbmem();

	v0=gtk_vbox_new(FALSE, 5);

	// status area
	v1 = gtk_vbox_new(FALSE, 20);
	a1 = gtk_alignment_new(0.5, 0.2, 0.5, 0.01);
	lb_result = gtk_label_new(" ");
	pb = gtk_progress_bar_new();
	gtk_widget_set_usize(pb, 480, 32);
	gtk_container_add(GTK_CONTAINER(v1), lb_result);

	lb_error_rate = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(v1), lb_error_rate);

	gtk_container_add(GTK_CONTAINER(v1), pb);

	// button
	h2 = gtk_hbox_new(FALSE, 20);
	a2 = gtk_alignment_new(0.5, 0.2, 0.5, 0.15);
	b_start = gtk_button_new_with_label("Test Loop Start");
	g_signal_connect(b_start, "button-release-event", G_CALLBACK(press_loop_func), 0);
	gtk_container_add(GTK_CONTAINER(h2), b_start);
	gtk_widget_set_sensitive(b_start, FALSE);
	b_stop = gtk_button_new_with_label("Test Loop Stop");
	g_signal_connect(b_stop, "button-release-event", G_CALLBACK(press_stop_func), 0);
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_container_add(GTK_CONTAINER(h2), b_stop);
	b_mount = gtk_toggle_button_new_with_label(MOUNT_BUTTON_MOUNT);
	gtk_container_add(GTK_CONTAINER(h2), b_mount);
	g_signal_connect(b_mount, "toggled", G_CALLBACK(press_mount_usbmem), 0);
	//g_signal_connect(b_mount, "button-release-event", G_CALLBACK(press_mount_usbmem_func), 0);
	gtk_widget_set_sensitive(b_mount, FALSE);

	gtk_container_add(GTK_CONTAINER(a2), h2);
	gtk_container_add(GTK_CONTAINER(v1), a2);
	gtk_container_add(GTK_CONTAINER(a1), v1);

	// filelist
	fc = gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_OPEN);

	set_fc_drive_usbmem(1);

	gtk_widget_set_usize(fc, 640, 340);
	gtk_container_add(GTK_CONTAINER(v1), fc);
	gtk_widget_set_sensitive(fc, FALSE);

	gtk_container_add(GTK_CONTAINER(v0), a1);
	b_quit = gtk_button_new_from_stock("gtk-quit");
	bb = sc_bbox2(NULL, bsub, b_quit, sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);

	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);

	gtk_widget_set_sensitive(b_quit, FALSE);

	basedrive_letter = targetdrive_letter = '\0';
	usbmem_usbmem_set_power(0);

	usbmem_search_basedrive_usbmem();

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b_mount), TRUE);

	if (testtime) {
		t_fin = time(NULL) + testtime;
		time_log_run = 1;
		press_loop(NULL, 0);
	}
	gtk_main();
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b_mount)) == TRUE)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b_mount), FALSE);

	test_file_cnt_write();

	sc_gtk_update();
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);

	test_mode = 0;
	return 0;
}

static int do_confirm_i2c()
{
	unsigned char wbuf[I2C_DATA_SIZE], rbuf[I2C_DATA_SIZE];
	
	if(sc_i2c_write_cradle(wbuf)!=0) return 0;
	if(sc_i2c_read_cradle(rbuf)!=0) return 0;
	
	return memcmp(wbuf, rbuf, I2C_DATA_SIZE)==0;
}

//
//	switching cradle mode by I2C
//
static void usbmem_cradle_switch(int is_30sec)
{
	// set termininal
	system("i2cset -y 2 0x1d 0x02 0x00");
	usleep(I2C_GUARDTIME_USEC);
	
	// set I/O
	system("i2cset -y 2 0x1d 0x03 0xf1");
	usleep(I2C_GUARDTIME_USEC);
	
	// send switch command
	if(is_30sec)
		system("i2cset -y 2 0x1d 0x01 0x0e");
	else
		system("i2cset -y 2 0x1d 0x01 0x06");
}

//	20111005VACS
static	void
mount_umount_cradle(GtkWidget *widget, int is_30sec, int mflg)
{
int		ret;
char	path[SMALL_STR];


	if(mflg){
		debug_printf(5, "mount_umount_cradle():usbmem mount start... (/mnt1/cradle)\n");
		gtk_label_set_text(GTK_LABEL(lb_result), "Confirming USB memory...");
		sc_i2c_set_power(1);
		usleep(50000);

		ret=1;
		usbmem_cradle_switch((int)is_30sec);
		usleep(1500000);

		if(!cr_mount_ok){
			debug_printf(5, "mount_umount_cradle():mount cradle\n");
			ret=wait_for_newdrive_cradle(3);
		}
		if(!ret){
			debug_printf(5, "mount_umount_cradle():usbmem mount failure. (/mnt1/cradle)\n");
			usbmem_result_red("Could not confirm USB memory");
			gtk_button_set_label(GTK_BUTTON(widget), is_30sec ? MOUNT_BUTTON_MOUNT30 : MOUNT_BUTTON_MOUNT);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE);

			gtk_widget_set_sensitive(b_start, FALSE);
			gtk_widget_set_sensitive(b_stop, FALSE);
			gtk_widget_set_sensitive(b_mount, TRUE);
			gtk_widget_set_sensitive(widget, FALSE);
			gtk_widget_set_sensitive(b_quit, TRUE);
			confirm_run=0;
			cr_mount_ok=0;
		}else{
			debug_printf(5, "mount_umount_cradle():usbmem mount OK. (/mnt1/cradle)\n");
			set_fc_drive_cradle(1);
			gtk_widget_set_sensitive(fc, TRUE);
			usbmem_result_blue(" ");
			gtk_button_set_label(GTK_BUTTON(widget), is_30sec ? MOUNT_BUTTON_UNMOUNT30 : MOUNT_BUTTON_UNMOUNT);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);

			if(loop_testing){
				gtk_widget_set_sensitive(b_stop, TRUE);
			}else{
				gtk_widget_set_sensitive(b_start, TRUE);
				gtk_widget_set_sensitive(b_quit, TRUE);
			}
			gtk_widget_set_sensitive(b_mount, FALSE);
			gtk_widget_set_sensitive(widget, TRUE);
			confirm_run=1;
			cr_mount_ok=1;
		}
	}else{
		debug_printf(5, "mount_umount_cradle():usbmem unmount (/mnt1/cradle)\n");
		gtk_button_set_label(GTK_BUTTON(widget), is_30sec ? MOUNT_BUTTON_MOUNT30 : MOUNT_BUTTON_MOUNT);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE);
		gtk_widget_set_sensitive(b_start, FALSE);
		set_fc_drive_cradle(1);
		gtk_widget_set_sensitive(fc, FALSE);
		strcpy(path, "/mnt1/cradle");

		gtk_widget_set_sensitive(b_mount, TRUE);

		if(!loop_testing)	gtk_widget_set_sensitive(b_quit, TRUE);
		if(is_mountpoint(path)&&(umount(path)!=0)){
			usbmem_result_red("umount error");
			debug_printf(5, "mount_umount_cradle():umount error (/mnt1/cradle)\n");
			return;
		}
		targetdrive_letter='\0';
		confirm_run=0;
		cr_mount_ok=0;
	}
}

//	20110930VACS
//
//	interval I2C check
//
static	gboolean
confirm_i2c(gpointer data)
{
int	r;
int	cr_i2c_ok=0;
int	cr_i2c_ok_new=0;

	if(confirm_stop)			return	FALSE;

	r=do_confirm_i2c();
	gtk_label_set_text(GTK_LABEL(lb_i2cstat), r ? "I2C connected : " : "I2C disconnected : ");

	if(!confirm_run){
		if(r){
			gtk_widget_set_sensitive(b_mount, TRUE);
		}else{
			gtk_widget_set_sensitive(b_mount, FALSE);
		}
		return	TRUE;
	}

	if(r)								cr_i2c_ok_new=1;
	else								cr_i2c_ok_new=0;
	if(!cr_i2c_ok_old && cr_i2c_ok_new)	cr_i2c_ok=0;
	else								cr_i2c_ok=1;
	cr_i2c_ok_old=cr_i2c_ok_new;

	if(press_mount_cradle_ing)	return	TRUE;
	press_mount_cradle_ing=2;

	if(!cr_i2c_ok && cr_mount_ok){
		gtk_widget_set_sensitive(b_start, FALSE);
		gtk_widget_set_sensitive(b_stop, FALSE);
		gtk_widget_set_sensitive(b_mount, FALSE);
		gtk_widget_set_sensitive(b_quit, FALSE);
		gtk_widget_set_sensitive(fc, FALSE);
		usleep(50000);

		mount_umount_cradle(b_mount, 0, 0);
		usleep(500000);
		mount_umount_cradle(b_mount, 0, 1);
		debug_printf(5, "Re-mount\n");
		if(loop_testing){
			press_stop(NULL, 0);
			usleep(1500000);
			press_loop(NULL, 0);
		}
	}
	press_mount_cradle_ing=0;

	return	TRUE;
}

static	void
press_mount_cradle(GtkWidget *widget, gpointer is_30sec)
{
int		ret;
char	path[SMALL_STR];

	if(press_mount_cradle_ing)	return;
	press_mount_cradle_ing=1;					/*	20110929VACS	*/

	gtk_widget_set_sensitive(b_mount, FALSE);
	gtk_widget_set_sensitive(b_quit, FALSE);	/*	20110928VACS	*/
	sc_gtk_update();
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))==TRUE){
		debug_printf(5, "usbmem mount start... (/mnt1/cradle)\n");
		gtk_label_set_text(GTK_LABEL(lb_result), "Confirming USB memory...");
		sc_gtk_update();
		sc_i2c_set_power(1);
		usleep(500000);

		ret=1;
		usbmem_cradle_switch((int)is_30sec);
		usleep(1500000);

		if(!cr_mount_ok){
			debug_printf(5, "mount cradle\n");	/*	20110921VACS	*/
			ret=wait_for_newdrive_cradle(3);
		}
		if(!ret){
			debug_printf(5, "usbmem mount failure. (/mnt1/cradle)\n");
			usbmem_result_red("Could not confirm USB memory");
			gtk_button_set_label(GTK_BUTTON(widget), is_30sec ? MOUNT_BUTTON_MOUNT30 : MOUNT_BUTTON_MOUNT);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE);

			/*	20110930VACS	*/
			gtk_widget_set_sensitive(b_start, FALSE);
			gtk_widget_set_sensitive(b_stop, FALSE);
			gtk_widget_set_sensitive(b_mount, TRUE);
			gtk_widget_set_sensitive(widget, FALSE);
			gtk_widget_set_sensitive(b_quit, TRUE);
			confirm_run=0;
			cr_mount_ok=0;
		}else{
			debug_printf(5, "usbmem mount OK. (/mnt1/cradle)\n");
			set_fc_drive_cradle(1);
			gtk_widget_set_sensitive(fc, TRUE);
			usbmem_result_blue(" ");
			gtk_button_set_label(GTK_BUTTON(widget), is_30sec ? MOUNT_BUTTON_UNMOUNT30 : MOUNT_BUTTON_UNMOUNT);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);	/*	20111003VACS	*/

			/*	20110930VACS	*/
			if(loop_testing){
				gtk_widget_set_sensitive(b_stop, TRUE);
			}else{
				gtk_widget_set_sensitive(b_start, TRUE);
				gtk_widget_set_sensitive(b_quit, TRUE);
			}
			gtk_widget_set_sensitive(b_mount, FALSE);
			gtk_widget_set_sensitive(widget, TRUE);
			confirm_run=1;
			cr_mount_ok=1;
		}
	}else{
		test_file_unlink();					/*	20110920VACS	*/

		debug_printf(5, "usbmem unmount (/mnt1/cradle)\n");
		gtk_button_set_label(GTK_BUTTON(widget), is_30sec ? MOUNT_BUTTON_MOUNT30 : MOUNT_BUTTON_MOUNT);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE);	/*	20111001VACS	*/
		gtk_widget_set_sensitive(b_start, FALSE);
		set_fc_drive_cradle(1);
		gtk_widget_set_sensitive(fc, FALSE);
		strcpy(path, "/mnt1/cradle");

		/*	20110928VACS	*/
		gtk_widget_set_sensitive(b_mount, TRUE);
		if(!loop_testing)	gtk_widget_set_sensitive(b_quit, TRUE);

		if(is_mountpoint(path)&&(umount(path)!=0)){
			usbmem_result_red("umount error");
			debug_printf(5, "umount error (/mnt1/cradle)\n");
			press_mount_cradle_ing=0;		/*	20110929VACS	*/
			return;
		}
		targetdrive_letter='\0';
		confirm_run=0;						/*	20111001VACS	*/
		cr_mount_ok=0;
	}

	press_mount_cradle_ing=0;				/*	20110929VACS	*/
}
static gboolean press_mount_cradle_func(
		GtkWidget *widget, GdkEvent *event, gpointer is_30sec)
{
	gboolean state;
	state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_toggle_button_set_active(
		GTK_TOGGLE_BUTTON(widget), state ? FALSE : TRUE); /* toggled */
	press_mount_cradle(widget, is_30sec);
	return FALSE;
}

//
//	menu for USB(HUB-1:Cradle)
//
int usbmem_cradle_main(GtkWidget *table, GtkWidget *bsub)
{
	GtkWidget *v0, *v1, *h1, *h2, *a1, *a2, *bb;

	loop_testing=0;					/*	20110927VACS	*/
	press_mount_cradle_ing=0;		/*	20110929VACS	*/

	/*	20110920VACS	*/
	test_mode=1;
	test_file_cnt_read();

	sc_i2c_set_power(1);

	usbmem_init_test_path_cradle();	/*	20110901VACS	*/
	v0=gtk_vbox_new(FALSE, 5);
	
	// status area
	v1=gtk_vbox_new(FALSE, 20);
	a1=gtk_alignment_new(0.5, 0.2, 0.5, 0.01);
	h1=gtk_hbox_new(FALSE, 10);
	lb_i2cstat=gtk_label_new(" ");
	gtk_container_add(GTK_CONTAINER(h1), lb_i2cstat);
	lb_result=gtk_label_new(" ");
	gtk_container_add(GTK_CONTAINER(h1), lb_result);
	gtk_container_add(GTK_CONTAINER(v1), h1);
	
	lb_error_rate = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(v1), lb_error_rate);
	
	pb=gtk_progress_bar_new();
	gtk_widget_set_usize(pb, 480, 32);
	gtk_container_add(GTK_CONTAINER(v1), pb);
	
	// button
	h2=gtk_hbox_new(FALSE, 20);
	a2=gtk_alignment_new(0.5, 0.2, 0.5, 0.15);
	b_start=gtk_button_new_with_label("Test Loop Start");

	g_signal_connect(b_start, "button-release-event", G_CALLBACK(press_b_start_func), 0);
	
	gtk_container_add(GTK_CONTAINER(h2), b_start);
	gtk_widget_set_sensitive(b_start, FALSE);
	b_stop=gtk_button_new_with_label("Test Loop Stop");
	g_signal_connect(b_stop, "button-release-event", G_CALLBACK(press_stop_func), 0);
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_container_add(GTK_CONTAINER(h2), b_stop);
	
	b_mount=gtk_toggle_button_new_with_label(MOUNT_BUTTON_MOUNT);
	gtk_container_add(GTK_CONTAINER(h2), b_mount);
	g_signal_connect(b_mount, "toggled", G_CALLBACK(press_mount_cradle), (gpointer)0);
	//g_signal_connect(b_mount, "button-release-event", G_CALLBACK(press_mount_cradle_func), (gpointer)0);
	gtk_widget_set_sensitive(b_mount, TRUE);	/*	20110905VACS	*/

	gtk_container_add(GTK_CONTAINER(a2), h2);
	gtk_container_add(GTK_CONTAINER(v1), a2);
	gtk_container_add(GTK_CONTAINER(a1), v1);
	
	// filelist
	fc=gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_OPEN);
	set_fc_drive_cradle(1);						/*	20110901VACS	*/
	
	gtk_widget_set_usize(fc, 640, 340);
	gtk_container_add(GTK_CONTAINER(v1), fc);
	gtk_widget_set_sensitive(fc, FALSE);
	
	gtk_container_add(GTK_CONTAINER(v0), a1);
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(NULL, bsub, b_quit, sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	basedrive_letter=targetdrive_letter='\0';
	usbmem_exwifi_set_power(0);
	usbmem_search_basedrive_cradle();			/*	20110902VACS	*/
	
	if(testtime){
		t_fin=time(NULL)+testtime;
		time_log_run=1;
		press_loop(NULL, 0);					/*	20110905VACS	*/
	}
	confirm_stop=0;								/*	20110906VACS	*/

	g_timeout_add(500, confirm_i2c, 0);
	gtk_main();
	confirm_stop=1;								/*	20110906VACS	*/
	confirm_run=0;

	/*	20110903VACS	*/
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b_mount))==TRUE)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b_mount), FALSE);


	test_file_cnt_write();						/*	20110920VACS	*/

	sc_gtk_update();

	sc_i2c_set_power(0);
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);

	test_mode=0;								/*	20111004VACS	*/
	return 0;
}
