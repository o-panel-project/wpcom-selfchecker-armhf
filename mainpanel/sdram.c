/*
 *		SDRAM stuff
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
#include "sdram_olddef.h"
#include "common.h"

#define SCRIPT_MOUNT_POINT "/mnt/tmp"

static GtkWidget *lb_testresult, *pb;
static GtkWidget *lb_error_rate;
static GtkWidget *b_start, *b_stop, *b_loop, *b_quit;
static int time_log_run=0;
static time_t t_fin=0;
static int script_exitcode=-1, script_count=0;

#define SDRAM_SHELL_PATH "/mnt1/sdram_test"
#define SDRAM_W_NAME SDRAM_SHELL_PATH "/sdram_result.txt"

int error_count;

//
//	a file to string
//
int sdrt_get_file(char *fname, char *buf, int len)
{
	FILE *fp;
	int r;
	
	if(access(fname,0)==-1) return 0;
	fp=fopen(fname,"r");
	if(!fp){
		sprintf(buf, "open error [%s]", fname);
		return -1;
	}
	
	if(fgets(buf, len-1,fp)==NULL){
		r=0;
	}else{
		r=strlen(buf);
	}
	fclose(fp);
	return r;
}

void sdrt_result_color(char *c, char *s)
{
	char tmps[SMALL_STR];
	
	sprintf(tmps, "<span color=\"%s\">%s</span>", c, s);
	gtk_label_set_markup(GTK_LABEL(lb_testresult), tmps);
}

#define sdrt_result_red(s)  sdrt_result_color("red", s)
#define sdrt_result_blue(s) sdrt_result_color("blue", s)

static int SdramFilewrite(char *fname, char *wbuf)
{
	int ret, length;
	FILE *fp;
	
	ret=0;
	/* error file found */
	fp=fopen(fname,"w+");
	if(fp==NULL){
		debug_printf(5, "[file write]open error [%s]\n", fname);
		return -1;
	}
	/* file write */
	if(fprintf(fp, "%s\n", wbuf)<=0){
		debug_printf(5, "[file write]write error [%s]\n", fname);
		ret=-1;
	}else{
		length=strlen(wbuf);
		ret=length;
		debug_printf(5,"[file write]write Success [%s][%d]\n", fname, ret);
	}
	fclose(fp);
	return ret;
}

static int pid_script=0;

//
//	execute test script
//
static int sdrt_test_start()
{
	switch(pid_script=fork()){
	case -1:
		pid_script=0;
		return -1;
	
	case 0:
		setpgid(getpid(),getpid());
		execl("/bin/sh", "sh", SDRAM_SHELL_PATH "/sdram_test.sh", NULL);
		_exit(127);
		break;
	}
	return 0;
}

static int sdrt_loop_run=0; // 0:stop, 1:single run, 2:loop run
static int fd_progress=-1;
static int progress_count=0;
static int progress_total=0;

//
//	progress bar updater
//
void update_progress()
{
	int n;
	char tmps[SMALL_STR];
	
	if(0<(n=read(fd_progress, tmps, sizeof(tmps)-1))){
		tmps[n]='\0';
		debug_printf(5, "[SDRAM:update_progress] got(%d) : %s", n, tmps);
		if(!progress_total){
			progress_total=atoi(tmps);
			progress_count=0;
		}else{
			progress_count+=atoi(tmps);
			gtk_progress_bar_update(GTK_PROGRESS_BAR(pb),
									min(1.0, 1.0*progress_count/progress_total));
			if(script_count){
				sprintf(tmps, "Loop Test #%d", script_count);
				gtk_progress_bar_set_text(GTK_PROGRESS_BAR(pb), tmps);
			}
		}
	}
}

static void press_stop(GtkWidget *widget, gpointer data)
{
	gtk_widget_set_sensitive(b_stop, FALSE);
	sdrt_loop_run=0;
	if(pid_script){
		do_killpg(pid_script);
		pid_script=0;
		sdrt_result_red("*** Test Aborted ***");
		progress_count=progress_total=0;
	}
	if(0<fd_progress){
		close(fd_progress);
		fd_progress=-1;
	}
	time_log_run=0;
	script_count=0;
	gtk_widget_set_sensitive(b_start, TRUE);
	gtk_widget_set_sensitive(b_loop, TRUE);
	gtk_widget_set_sensitive(b_quit, TRUE);
}
static gboolean press_stop_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_stop(widget, data);
	return FALSE;
}

int sdrt_check_timeup()
{
	if(t_fin && t_fin<time(NULL)){
		debug_printf(3, "Loop test time up.\n");
		press_stop(NULL, 0);
		gtk_main_quit();
		return 1;
	}
	return 0;
}

void sdrt_exitcode_check()
{
	int st;
	
	if(script_exitcode<0 && pid_script==waitpid(pid_script, &st, WNOHANG)){
		if(WIFEXITED(st))
			script_exitcode=WEXITSTATUS(st);
	}
}

// phase1 : need to start test script
static gboolean sdrt_loop_phase1()
{
	char tmps[SMALL_STR];

	script_exitcode=-1;	
	gtk_progress_bar_update(GTK_PROGRESS_BAR(pb), 0.0);
	debug_printf(5, "[SDRAM:START]Thd start%s\n", sdrt_loop_run==1 ? "" : "(loop)");
	sprintf(tmps, "*** Normal Test Start ***%s", sdrt_loop_run==1 ? "" : "(loop)");
	gtk_label_set_text(GTK_LABEL(lb_testresult), tmps);
	sdrt_test_start();
	usleep(200000);
	if(have_proc_pid(pid_script)) return TRUE;
	
	sdrt_result_red("[ SD-RAM shell error ]");
	debug_printf(5, "[SDRAM:THD]SD-RAM shell error\n");
	
	return FALSE;
}

// phase2 : script not alive.
static gboolean sdrt_loop_phase2()
{
	sdrt_exitcode_check();
	
	if(fd_progress<0)
		fd_progress=open("/tmp/progress.log", O_RDONLY|O_NONBLOCK);
	else
		update_progress();
	
	return TRUE;
}

// phase3 : script finished
static gboolean sdrt_loop_phase3()
{
	int r;
	char tmps[SMALL_STR], buf[SMALL_STR], *result_text;
	char tmps2[SMALL_STR];
	char *result_dialog_text;
	
	if(0<fd_progress){
		update_progress();
		close(fd_progress);
		fd_progress=-1;
		progress_count=progress_total=0;
	}
	
	sdrt_exitcode_check();
	if(script_exitcode!=0){
		if(0<script_exitcode){
			sprintf(tmps, "[ SD-RAM shell error (%d) ]", script_exitcode);
		}else{
			sprintf(tmps, "[ SD-RAM shell unknown error ]");
		}
		sdrt_result_red(tmps);
		return FALSE;
	}
	
	r=sdrt_get_file(SDRAM_SHELL_PATH "/finish.txt", buf, sizeof(buf));
	if(r<=0){
		sdrt_result_red("[ Finish file read error ]");
		debug_printf(5, "[SDRAM:THD]Finish file read error\n");
		return FALSE;
	}
	
	r=sdrt_get_file(SDRAM_SHELL_PATH "/result.log", buf, sizeof(buf));
	if(r<=0){
		sdrt_result_red("[ Result file read error ]");
		debug_printf(5, "[SDRAM:THD]Result file read error\n");
		return FALSE;
	}
	
	if(strstr(buf, "NG")){
		result_dialog_text = "<span foreground='red' font='Serif 40px'><b>SD-RAM NG</b></span>";
		result_text="[ SD-RAM NG ]";
		sdrt_result_red(result_text);
		error_count++;
	}else{
		result_dialog_text = "<span font='Serif 40px'><b>SD-RAM OK</b></span>";
		result_text="[ SD-RAM OK ]";
		sdrt_result_blue(result_text);
	}
	
	
	gtk_progress_bar_update(GTK_PROGRESS_BAR(pb), 1.0);
	
	debug_printf(5, "[SDRAM:THD]Result File Write\n", result_text);
	sprintf(tmps, "SD-RAM Test Result : %s", result_text);
	debug_printf(5, "[SDRAM:THD]Result File Write Text[%s][%s] \n", SDRAM_W_NAME, result_text);
	SdramFilewrite(SDRAM_W_NAME, tmps);
	
	pid_script=0;
	if(sdrt_loop_run==2){ // continue to next loop
		sprintf(tmps2, "Total = %d : Error = %d : Error rate = %d %%", 
			script_count, error_count, (script_count==0)? 0 : (int)((double)error_count/script_count*100));
		gtk_label_set_text(GTK_LABEL(lb_error_rate), tmps2);
		
		script_count++;
		umount(SCRIPT_MOUNT_POINT);
		return TRUE;
	} else {
		sc_message_dialog(result_dialog_text, "OK", TRUE);
	}
	
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_widget_set_sensitive(b_start, TRUE);
	gtk_widget_set_sensitive(b_loop, TRUE);
	gtk_widget_set_sensitive(b_quit, TRUE);
	
	sdrt_loop_run=0; // eof single script
	return FALSE;
}

//
//	main loop : watching child test program
//
static gboolean sdrt_loop(gpointer data)
{
	if(!sdrt_loop_run)
		return FALSE;
	
	if(sdrt_check_timeup())
		return FALSE;
	
	// phase1 : need to start test script
	if(!pid_script)
		return sdrt_loop_phase1();
	
	// phase2 : script alive?
	if(have_proc_pid(pid_script))
		return sdrt_loop_phase2();
	
	// phase3 : script finished
	return sdrt_loop_phase3();
}

static gboolean time_log(gpointer data)
{
	if(!time_log_run) return FALSE;
	debug_printf(5, "[SDRAM AGING:GET TIME] timGetTime[%ld]\n", time(NULL));
	
	if(t_fin && t_fin<time(NULL)){
		debug_printf(3, "Loop test time up.\n");
		press_stop(NULL, 0);
		gtk_main_quit();
		script_count=0;
		return FALSE;
	}
	return TRUE;
}

static void start_common(int mode)
{
	gtk_widget_set_sensitive(b_loop, FALSE);
	gtk_widget_set_sensitive(b_start, FALSE);
	gtk_widget_set_sensitive(b_quit, FALSE);
	sdrt_loop_run=mode;
	g_timeout_add(200, sdrt_loop, 0);
	gtk_widget_set_sensitive(b_stop, TRUE);
}

static void press_single(GtkWidget *widget, gpointer data)
{
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(pb), "");
	script_count=0;
	start_common(1);
}
static gboolean press_single_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_single(widget, data);
	return FALSE;
}

static void press_loop(GtkWidget *widget, gpointer data)
{
	char tmps2[SMALL_STR];
	
	start_common(2);
	time_log_run=1;
	script_count=1;
	error_count = 0;
	
	sprintf(tmps2, "Total = %d : Error = %d : Error rate = %d %%", 0, 0, 0);
	gtk_label_set_text(GTK_LABEL(lb_error_rate), tmps2);
	
	g_timeout_add(16*100000, time_log, 0);
	debug_printf(5, "[SDRAM:START]Start Timer(loop)\n");
}
static gboolean press_loop_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_loop(widget, data);
	return FALSE;
}

//
//	menu for 'SDRAM'
//
int sdrt_main(GtkWidget *table, GtkWidget *bsub)
{
	GtkWidget *v0, *v1, *v2, *a1, *a2, *bb;
	
	// panel setup
	v0=gtk_vbox_new(FALSE, 10);
	
	// status area
	v1=gtk_vbox_new(FALSE, 10);
	a1=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	lb_testresult=gtk_label_new("");
	pb=gtk_progress_bar_new();
	gtk_widget_set_usize(pb, 480, 32);
	gtk_container_add(GTK_CONTAINER(v1), lb_testresult);
	
	lb_error_rate=gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(v1), lb_error_rate);
	
	gtk_container_add(GTK_CONTAINER(v1), pb);
	gtk_container_add(GTK_CONTAINER(a1), v1);
	
	// buttons
	v2=gtk_vbox_new(FALSE, 10);
	a2=gtk_alignment_new(0.5, 0.5, 0.5, 0.3);
	b_start=gtk_button_new_with_label("SD-RAM\nStart");
	g_signal_connect(b_start, "button-release-event", G_CALLBACK(press_single_func), (gpointer)2);
	b_stop=gtk_button_new_with_label("SD-RAM\nStop");
	g_signal_connect(b_stop, "button-release-event", G_CALLBACK(press_stop_func), (gpointer)3);
	b_loop=gtk_button_new_with_label("SD-RAM\nLoop");
	g_signal_connect(b_loop, "button-release-event", G_CALLBACK(press_loop_func), (gpointer)4);
	
	gtk_container_add(GTK_CONTAINER(v2), b_start);
	gtk_container_add(GTK_CONTAINER(v2), b_stop);
	gtk_container_add(GTK_CONTAINER(v2), b_loop);
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_container_add(GTK_CONTAINER(a2), v2);
	
	gtk_container_add(GTK_CONTAINER(v0), a1);
	gtk_container_add(GTK_CONTAINER(v0), a2);
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(NULL, bsub, b_quit, sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
	
	if(testtime){
		t_fin=time(NULL)+testtime;
		time_log_run=1;
		press_loop(NULL, 0);
	}
	gtk_main();
	
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	umount(SCRIPT_MOUNT_POINT);
	return 0;
}

