/*
 *   Common functions, vars defs
 *
 *					2011.6	written by Omoikane Inc.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

//#include <gtk/gtk.h>
#include <sys/time.h>

#define SC_TITLE   "New SelfChecker (j3/j35/o)"
#define SC_TITLE_2 "SelfChecker(j3/j35/o)"
#define SC_VERSION "v3.7.0"

#define	WPC_BOARD_TYPE_J35	35
#define	WPC_BOARD_TYPE_O	5

//
//	global vars and defs
//
extern int debug_level, pid_battery_logger;
extern int audio_pid, audio_is_loop;
extern int testtime, interval, udhcpc_kill_check;
extern int battery_sub_run;
extern char *base_path, *log_dir;
extern int pid_bat1_charge, pid_bat2_charge;	/*	20110728VACS	*/
extern int bl_toggle_charge;					/*	20110831VACS	*/
extern int cr_mount_ok;							/*	20110905VACS	*/

#define DEFAULT_WINDOW_WIDTH	1024
#define	DEFAULT_WINDOW_HEIGHT	600
#define MID_STR	2048							/*	20110826VACS	*/
#define SMALL_STR 1024
#define WPCIO_OPEN_RETRY 10
#define	AUDIO_VOLUME_MAX	63					/*	20110917VACS	*/
#define	AUDIO_VOLUME1_MAX	127	/* for J4 */
#define	AUDIO_VOLUME2_MAX	118	/* for J4 */
#define	AUDIO_VOLUME_MAX_O	255					/*	20110917VACS	*/

#define SYSCALL(call) while (((call) == -1) && (errno == EINTR))

//
//	common functions
//
#define max(x,y) ((x)<(y) ? (y) : (x))
#define min(x,y) ((x)<(y) ? (x) : (y))
extern void debug_set(int);
extern int debug_printf(int n, const char *fmt, ...);
extern int append_sprintf(char *dest, const char *fmt, ...);
extern void append_hex(char *dest, int n, char *s);
extern int time_diff_msec(struct timeval *now, struct timeval *past);
extern int time_passed(struct timeval *tv0, int msec);
extern int sc_time_set(char *buf);
extern void do_kill(int);
extern void do_killpg(int);
extern int have_proc_pid(int pid);
extern void battery_check_logger_stop();
extern int get_md5sum(char *f, unsigned char *digest);
extern int is_mountpoint(const char *path);
extern void sc_i2c_set_power(int x);
extern int wpcio_open(int n, char *tag);
extern int sc_get_board_type();
extern int sc_IsJ4();
extern void set_sc_window_width(int);
extern void set_sc_window_height(int);
extern int get_sc_window_width();
extern int get_sc_window_height();

#ifdef __GTK_H__

extern GtkWidget *tv_sidemenu, *lb_menutop;
extern GtkTreeIter iter_battery, iter_audio;
enum{
	COLUMN_LABEL, COLUMN_NAME_COLOR, N_COLUMNS
};


//
//	common widget subroutines
//
extern void sc_gtk_update();
extern void sc_gtk_update_n(int n);
extern void sc_bbox1_click(GtkWidget *widget, gpointer data);
extern gboolean sc_bbox1_click_func(GtkWidget *widget, GdkEvent *ev, gpointer data);
extern void sc_bbox1_click_noquit(GtkWidget *widget, gpointer data);
extern gboolean sc_bbox1_click_noquit_func(GtkWidget *widget, GdkEvent *ev, gpointer data);
extern GtkWidget *sc_bbox1(int *st, GtkWidget *b, void (*callback)(GtkWidget *, gpointer));
extern GtkWidget *sc_bbox2(int *st, GtkWidget *bsub, GtkWidget *b_quit, gboolean (*callback)(GtkWidget *, GdkEvent *,gpointer));
extern void sc_bbox2_remove(GtkWidget *bsub);
extern void sc_message(GtkMessageType type, GtkButtonsType buttons, const char *fmt, ...);
extern void sc_gtk_text_view_insert(GtkTextView *tv, char *buf, int n);
extern void sc_gtk_text_view_insert_ping(GtkTextView *tv, char *buf, int n);
extern void sc_gtk_text_view_append(GtkTextView *tv, char *s);
extern void sc_gtk_text_view_clear(GtkTextView *tv);
extern int sc_message_yesno(const char *fmt, ...);
extern void sc_table_attach1(GtkTable *table, GtkWidget *widget);
extern void sc_table_attach2(GtkTable *table, GtkWidget *widget);
extern GtkWidget *sc_table_new();
extern void sc_message_dialog(char *msg, char *btnlabel, gboolean ismarkup);
extern void sc_wait_dialog(char *msg, char *btnlabel, char *(*event)(void));

//
//	Panel / Subpanel subroutines
//
extern int strategy_main(GtkWidget *p, GtkWidget *bsub);
extern int sdrt_main(GtkWidget *p, GtkWidget *bsub);
extern int audio_main(GtkWidget *p, GtkWidget *bsub);
extern int movie_main(GtkWidget *p, GtkWidget *bsub);
extern int wifi_main(GtkWidget *p, GtkWidget *bsub);
extern int lcd_pwm_main(GtkWidget *p, GtkWidget *bsub);
extern int battery_main(GtkWidget *p, GtkWidget *bsub);
extern int led_main(GtkWidget *p, GtkWidget *bsub);
extern int dipsw_main(GtkWidget *p, GtkWidget *bsub);
//extern int tpfunc_main(GtkWidget *p, int ival);
extern int tpfunc_main(GtkWidget *p, GtkWidget* ival);
extern int tpoff_main(GtkWidget *p, GtkWidget *bsub);
extern int i2c_cradle_main(GtkWidget *p, GtkWidget *bsub);
extern int felica_main(GtkWidget *p, GtkWidget *bsub);
extern int usbmem_exwifi_main(GtkWidget *p, GtkWidget *bsub);
extern int usbmem_hub2mem_main(GtkWidget *p, GtkWidget *bsub);
extern int usbmem_cradle_main(GtkWidget *p, GtkWidget *bsub);
extern int date_main(GtkWidget *table, GtkWidget *bsub);

extern GtkWidget *sc_battery_box_new();
extern void *battery_sub_update(void *data);
extern gboolean battery_sub_update_step(gpointer point);

extern int get_serial_id(char *buf, int size);
extern int set_serial_id(char *buf);


//
//	updown widget set
//
struct updown;
typedef void (*updown_event)(struct updown *);
struct updown {
	GtkWidget *body;
	GtkWidget *align;
	GtkWidget *vbox;
	GtkWidget *caption;
	GtkWidget *upbtn;
	GtkWidget *valuelb;
	GtkWidget *event_box;
	GtkWidget *downbtn;
	int value;
	int max;
	int min;
	int accl1;
	int accl2;
	int up_timer_tag;
	int down_timer_tag;
	updown_event ude;
	int type;
};

extern int ctoi(int value, int type);
extern char itoc(int value, int type);

extern int get_updown_value(struct updown *ud);
extern void set_updown_value(struct updown *ud, int value);
extern void set_updown_max(struct updown *ud, int value);
extern void set_updown_min(struct updown *ud, int value);
extern void create_updown(struct updown *ud, char *caption, char *plus_caption, char *minus_caption,
	int defaultvalue, int min, int max, int accl1, int accl2,
	gfloat xalign, gfloat yalign, gfloat xscale, gfloat yscale, updown_event ude, int type);

#endif

#endif /* __COMMON_H__ */
