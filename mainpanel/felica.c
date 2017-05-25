/*
 *		USB FeliCa
 *
 *					2011.6	written by Omoikane Inc.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <gtk/gtk.h>
#include "common.h"
#include "wpcio.h"

#include "felica_cc.h"
#include "utl.h"
#include "icsdrv.h"
#include "ics_error.h"
#include "rcs956.h"
#include "rcs956_usb.h"
#include "rcs956_typea.h"
#include "rcs956_typeb.h"
#include "stub/felica_cc_stub_rcs956.h"

//#include "felica-former/misc.h"
#include "felica-former/i2c_usb.h"
#include "felica-former/USB_FeliCatest.h"

//#include	"i2c_usb/i2c_usb_test_cradle.h"

// 1. felica_cc_polling()/rcs956_typea_polling()/ rcs956_typeb_polling() を繰り返す。
// 2. 検出内容およびエラーについて表示する。
// 3. 同内容をログに書き込む。
// 4. UI としてはログのテキストエリアで表示し、スタート・ストップボタンで制御をおこなう。

static GtkWidget *lb_status, *b_start, *b_stop, *b_quit, *cb;
static GtkWidget *lb_error_rate;
static GtkWidget *tv_result;
static ICS_HW_DEVICE rcs956dev;
static felica_cc_devf_t rcs956devf;
const static icsdrv_basic_func_t *g_drv_func = &rcs956_usb_basic_func;
static UINT32 (*g_felica_cc_stub_initialize_func)(felica_cc_devf_t* devf, ICS_HW_DEVICE* dev) = felica_cc_stub_rcs956_initialize;
static guint felica_timer_tag = (guint)0;	/*	20110818VACS	*/

int tatal_count, error_count;

extern char *scan_rcs956();

//
//	error interpreter
//	(imported from former version)
//
int	get_ics_error_message(int errorno, char *mes)
{
	switch(errorno){
	case ICS_ERROR_NOT_SUPPORTED:
		strcpy(mes,ICS_ERROR_NOT_SUPPORTED_MESSAGE);
		break;
	case ICS_ERROR_NOT_IMPLEMENTED:
		strcpy(mes,ICS_ERROR_NOT_IMPLEMENTED_MESSAGE);
		break;
	case ICS_ERROR_NOT_INITIALIZED:
		strcpy(mes,ICS_ERROR_NOT_INITIALIZED_MESSAGE);
		break;
	case ICS_ERROR_NOT_OPENED:
		strcpy(mes,ICS_ERROR_NOT_OPENED_MESSAGE);
		break;
	case ICS_ERROR_ALREADY_OPENED:
		strcpy(mes,ICS_ERROR_ALREADY_OPENED_MESSAGE);
		break;
	case ICS_ERROR_INVALID_PARAM:
		strcpy(mes,ICS_ERROR_INVALID_PARAM_MESSAGE);
		break;
	case ICS_ERROR_ILLEGAL_MODE:
		strcpy(mes,ICS_ERROR_ILLEGAL_MODE_MESSAGE);
		break;
	case ICS_ERROR_FATAL:
		strcpy(mes,ICS_ERROR_FATAL_MESSAGE);
		break;
	case ICS_ERROR_IO:
		strcpy(mes,ICS_ERROR_IO_MESSAGE);
		break;
	case ICS_ERROR_NO_RESOURCES:
		strcpy(mes,ICS_ERROR_NO_RESOURCES_MESSAGE);
		break;
	case ICS_ERROR_BUSY:
		strcpy(mes,ICS_ERROR_BUSY_MESSAGE);
		break;
	case ICS_ERROR_PERMISSION:
		strcpy(mes,ICS_ERROR_PERMISSION_MESSAGE);
		break;
	case ICS_ERROR_TIMEOUT:
		strcpy(mes,ICS_ERROR_TIMEOUT_MESSAGE);
		break;
	case ICS_ERROR_FRAME_CRC:
		strcpy(mes,ICS_ERROR_FRAME_CRC_MESSAGE);
		break;
	case ICS_ERROR_INVALID_RESPONSE:
		strcpy(mes,ICS_ERROR_INVALID_RESPONSE_MESSAGE);
		break;
	case ICS_ERROR_SYNTAX:
		strcpy(mes,ICS_ERROR_SYNTAX_MESSAGE);
		break;
	case ICS_ERROR_BUF_OVERFLOW:
		strcpy(mes,ICS_ERROR_BUF_OVERFLOW_MESSAGE);
		break;
	case ICS_ERROR_DATA_TRANS_START:
		strcpy(mes,ICS_ERROR_DATA_TRANS_START_MESSAGE);
		break;
	case ICS_ERROR_DATA_TRANS_END:
		strcpy(mes,ICS_ERROR_DATA_TRANS_END_MESSAGE);
		break;
	case ICS_ERROR_NOT_STARTED:
		strcpy(mes,ICS_ERROR_NOT_STARTED_MESSAGE);
		break;
	case ICS_ERROR_ALREADY_STARTED:
		strcpy(mes,ICS_ERROR_ALREADY_STARTED_MESSAGE);
		break;
	case ICS_ERROR_SEQUENCE:
		strcpy(mes,ICS_ERROR_SEQUENCE_MESSAGE);
		break;
	case ICS_ERROR_DESELECTED:
		strcpy(mes,ICS_ERROR_DESELECTED_MESSAGE);
		break;
	case ICS_ERROR_RELEASED:
		strcpy(mes,ICS_ERROR_RELEASED_MESSAGE);
		break;
	case ICS_ERROR_RF_OFF:
		strcpy(mes,ICS_ERROR_RF_OFF_MESSAGE);
		break;
	case ICS_ERROR_NOT_EXIST:
		strcpy(mes,ICS_ERROR_NOT_EXIST_MESSAGE);
		break;
	case ICS_ERROR_ALREADY_EXIST:
		strcpy(mes,ICS_ERROR_ALREADY_EXIST_MESSAGE);
		break;
	case ICS_ERROR_IGNORE:
		strcpy(mes,ICS_ERROR_IGNORE_MESSAGE);
		break;
	case ICS_ERROR_STATUS_FLAG1:
		strcpy(mes,ICS_ERROR_STATUS_FLAG1_MESSAGE);
		break;
	case ICS_ERROR_STATUS_FLAG:
		strcpy(mes,ICS_ERROR_STATUS_FLAG_MESSAGE);
		break;
	case ICS_ERROR_SN_OVERFLOW:
		strcpy(mes,ICS_ERROR_SN_OVERFLOW_MESSAGE);
		break;
	case ICS_ERROR_INVALID_DATA:
		strcpy(mes,ICS_ERROR_INVALID_DATA_MESSAGE);
		break;
	case ICS_ERROR_DISCONNECTED:
		strcpy(mes,ICS_ERROR_DISCONNECTED_MESSAGE);
		break;
	case ICS_ERROR_SHUTDOWN:
		strcpy(mes,ICS_ERROR_SHUTDOWN_MESSAGE);
		break;
	case ICS_ERROR_MANY_ERRORS:
		strcpy(mes,ICS_ERROR_MANY_ERRORS_MESSAGE);
		break;
	case ICS_ERROR_NOT_CONNECTED:
		strcpy(mes,ICS_ERROR_NOT_CONNECTED_MESSAGE);
		break;
	case ICS_ERROR_DEV_BUSY:
		strcpy(mes,ICS_ERROR_DEV_BUSY_MESSAGE);
		break;
	case ICS_ERROR_DEVICE:
		strcpy(mes,ICS_ERROR_DEVICE_MESSAGE);
		break;
	default:
		mes[0]='\0';
		break;
	}
	return strlen(mes);
}

//
//	port finder
//	(imported from former version)
//
static int felica_find_device()
{
	int i, cnt, device_no, work_device_no, read_device_no;
	char str[USBFS_DEVICES_WORK_SIZE];
	char read_work[LOCAL_WORK_SIZE128];
	char *sp, check_str[]="Dev#=", check_end[]="Manufacturer=Sony";
	int internal_usb_access_device_no[256], internal_usb_access_device_no_cnt;
	FILE *fp;

	if((fp = fopen("/proc/bus/usb/devices", "r")) == NULL){
		debug_printf(5, "[RCS956] cannot open usbfs device file\n");
		device_no = RCS956_PORT_NO_GET_NG;
		return(device_no);
    }
	debug_printf(5, "[RCS956] open usbfs device file\n");

	device_no = RCS956_PORT_NO_GET_NG;

	/*	internal USB memory access devices no init			*/
	for(i = 0;i < INTERNAL_DEVICE_NO_SIZE;i++){
		internal_usb_access_device_no[i] = INTERNAL_USB_ACCESS_DEVICE_INIT_NO;
	}
	internal_usb_access_device_no_cnt	=	0;

	cnt = 0;
	memset(str,0,sizeof(str));
	while(!feof(fp)){
		fgets(str,USBFS_DEVICES_WORK_SIZE,fp);
		
		/*	"Dev#=" search									*/
		sp = strstr(str,check_str);
		if(sp != NULL){
			/*	Discovery "Dev#="							*/
			memset(read_work,0x00,sizeof(read_work));
			memcpy(read_work,(sp + USBFS_DEVICES_START_POINTER),USBFS_DEVICES_PORTNO_CHAR_NUMBER);
			work_device_no = atoi(read_work);

			/*	target 2 < 127								*/
			if((work_device_no >= RCS956_DEFAULT_PORT_START_DEVICE_NAME) &&
				(work_device_no <= RCS956_DEFAULT_PORT_END_DEVICE_NAME)){
				read_device_no = work_device_no;
				debug_printf(5, "[RCS956] read deviceno[%d]\n",read_device_no);
				/*	internal USB memory access get devices no		*/
				internal_usb_access_device_no[cnt] = read_device_no;
				cnt++;
			}
		}

		/*	"Manufacturer=Sony" search						*/
		/*	end string point								*/
		sp = strstr(str,check_end);
		if(sp != NULL){
			/*	Discovery "Manufacturer=Sony"				*/
			debug_printf(5, "[RCS956] <%s> discovery\n",check_end);
			device_no = read_device_no;

			/*	internal USB access deviceno get			*/
			for(i = 0;((i < cnt) && (i < INTERNAL_DEVICE_NO_SIZE));i++){
				if(internal_usb_access_device_no[i] == device_no){
					internal_usb_access_device_no[i] = INTERNAL_USB_ACCESS_DEVICE_INIT_NO;
					if(cnt > 0)	cnt--;
					debug_printf(5, "[RCS956] device_no check[%d][%d] [%d]->[%d]\n",i,cnt,device_no,internal_usb_access_device_no[i]);
				}
				if(internal_usb_access_device_no[i] == INTERNAL_USB_ACCESS_DEVICE_INIT_NO){
					break;
				}
			}
			break;
		}
	}
	
	/* internal USB access device no cnt set */
	internal_usb_access_device_no_cnt = cnt;

	debug_printf(5, "[RCS956] device_no[%d]\n",device_no);
	printf("[RCS956] internal USB get device no[%d]\n",internal_usb_access_device_no_cnt);
	for(i = 0;i < ((internal_usb_access_device_no_cnt) && (i < INTERNAL_DEVICE_NO_SIZE));i++)
		printf(" [%d] ",internal_usb_access_device_no[i]);

	printf("\n");

	fclose(fp);
	return(device_no);
}


static char message_buf[SMALL_STR];
static int message_type, message_dirty=0;

void write_message_buf(int n, char *s)
{
	while(message_dirty)
		usleep(100000);
	
	message_type=n;
	strcpy(message_buf, s);
	message_dirty=1;
}

int polling_keeper_felica(int *r)
{
	char tmps[SMALL_STR];
	UINT16 s_system_code=RCS956_DEFAULT_SYSTEM_CODE;
	UINT8 polling_param[4];
	felica_card_t card;
	felica_card_option_t card_option;
	UINT8 s_polling_option=RCS956_DEFAULT_POLLING_OPTION;
	UINT8 s_polling_timeslot=RCS956_DEFAULT_POLLING_TIMESLOT;

	polling_param[0] = (UINT8)((s_system_code >> 8) & 0xff);
	polling_param[1] = (UINT8)((s_system_code >> 0) & 0xff);
	polling_param[2] = s_polling_option;
	polling_param[3] = s_polling_timeslot;
	
	*r=felica_cc_polling(&rcs956devf, polling_param, &card, &card_option, RCS956_DEFAULT_TIMEOUT);
	switch(*r){
	case ICS_ERROR_TIMEOUT:
		return 0;
		
	case ICS_ERROR_SUCCESS:
		strcpy(tmps, "[IDm]");
		append_hex(tmps, IDM_STRING_BYTE, (char *)card.idm);
		write_message_buf(RCS956_ICS_SUCCESS, tmps);
		return 1;
	}
	return 2;
}

int polling_keeper_typea(int *r)
{
	char tmps[SMALL_STR];
	typea_card_t ta_card;
	
	*r=rcs956_typea_polling(&rcs956dev, NULL, 0, &ta_card, NULL, RCS956_DEFAULT_TIMEOUT);
	switch(*r){
	case ICS_ERROR_TIMEOUT:
		return 0;
		
	case ICS_ERROR_SUCCESS:
		strcpy(tmps, "[UID]");
		append_hex(tmps, ta_card.uid_len, (char *)ta_card.uid);
		write_message_buf(RCS956_TYPEA_ICS_SUCCESS, tmps);
		return 1;
	}
	return 2;
}

int polling_keeper_typeb(int *r)
{
	int r_off;
	char tmps[SMALL_STR];
	typeb_card_t tb_card;
	typeb_card_attrib_t tb_attrib;
	
	bzero((void *)&tb_card, sizeof(tb_card));
	bzero((void *)&tb_attrib, sizeof(tb_attrib));
	
	*r=rcs956_typeb_polling(&rcs956dev, 0, &tb_card, &tb_attrib, RCS956_DEFAULT_TIMEOUT);
	r_off=g_drv_func->rf_off(&rcs956dev, RCS956_DEFAULT_TIMEOUT);
	if(r_off!=ICS_ERROR_SUCCESS)
		debug_printf(5, "[RCS956 THD] failure in rf_off():%u\n", r_off);
	
	switch(*r){
	case ICS_ERROR_TIMEOUT:
		if(r_off!=ICS_ERROR_SUCCESS){
			debug_printf(5, "[RCS956 THD] timeout failure in rf_off():%u\n", r);
			get_ics_error_message(r_off, tmps);
			write_message_buf(RCS956_RF_OFF_ICS_ERROR, tmps);
			return -1;
		}
		return 0;
	
	case ICS_ERROR_SUCCESS:
		strcpy(tmps, "[PUPI]");
		append_hex(tmps, PUPI_STRING_BYTE, (char *)tb_card.pupi);
		write_message_buf(RCS956_TYPEA_ICS_SUCCESS, tmps);
		return 1;
	}
	return 2;
}

int felica_rcs956finalize(ICS_HW_DEVICE *dev)
{
	UINT32	rc;

	debug_printf(5, "[RCS956] start finalization ...\n");

	if(g_drv_func->cancel_command){
		debug_printf(5, "[RCS956] calling cancel_command() ...\n");
		rc = g_drv_func->cancel_command(dev);
		if(rc != ICS_ERROR_SUCCESS)
			debug_printf(5, "[RCS956] failure in cancel_command():%u\n", rc);
	}

	if(g_drv_func->reset){
		debug_printf(5, "[RCS956] calling reset() ...\n");
		rc = g_drv_func->reset(dev, RCS956_DEFAULT_TIMEOUT);
		if(rc != ICS_ERROR_SUCCESS)
			debug_printf(5, "[RCS956] failure in reset():%u\n", rc);
	}

	if(g_drv_func->rf_off){
		debug_printf(5, "[RCS956] calling rf_off() ...\n");
		rc = g_drv_func->rf_off(dev, RCS956_DEFAULT_TIMEOUT);
		if(rc != ICS_ERROR_SUCCESS)
			debug_printf(5, "[RCS956] failure in rf_off():%u\n", rc);
	}
	rc = g_drv_func->close(dev);
    if(rc!=ICS_ERROR_SUCCESS){
		debug_printf(5, "[RCS956] failure in close():%u\n", rc);
		return(RCS956_FINALIZE_NG);
	}
	
	debug_printf(5, "[RCS956] finish finalization ...\n");
	return(RCS956_FINALIZE_OK);
}

static int polling_run=0, polling_thread_run=0;
static void do_polling()
{
	char tmps[SMALL_STR];
	int r, polling_type=-1, ics_error=0;
	int ret;
	int success_flag = 0;
	
	while(polling_run){
		polling_type++;
		if(polling_type==3) {
			tatal_count++;
			if (success_flag == 0) {
				error_count++;
			}
			success_flag = 0;
			polling_type = 0;
		}
		switch (polling_type)
		{
		case 0:
#if 1
			ret = polling_keeper_felica(&r);
			switch (ret)
			{
			case 0:  // タイムアウト
				continue;
			case 1:  // 成功
				success_flag = 1;
				break;
			default:  // その他のエラー
				break;
			}
#else
			if(polling_keeper_felica(&r)==0) continue;
#endif
			break;
		
		case 1:
#if 1
			ret = polling_keeper_typea(&r);
			switch (ret)
			{
			case 0:  // タイムアウト
				continue;
			case 1:  // 成功
				success_flag = 1;
				break;
			default:  // その他のエラー
				break;
			}
#else
			if(polling_keeper_typea(&r)==0) continue;
#endif
			break;
		
		default:
#if 1
			ret = polling_keeper_typeb(&r);
			switch (ret)
			{
			case 0:  // タイムアウト
				continue;
			case -1:  // ？
				goto fin;
			case 1:  // 成功
				success_flag = 1;
				break;
			default:  // その他のエラー
				break;
			}
#else
			switch(polling_keeper_typeb(&r)){
			case 0:
				continue;
			case -1:
				goto fin;
			}
#endif
			break;
		}
		// common error message
		if(r!=ICS_ERROR_SUCCESS){
			debug_printf(5, "[RCS956 THD] failure in felica_cc_polling():%u\n", r);
			ics_error=1;
			get_ics_error_message(r, tmps);
			write_message_buf(RCS956_ICS_ERROR, tmps);
			break;
		}
		usleep(100000);
	}
	
fin:
	if(ics_error){
		debug_printf(5, "[RCS956 THD] ICS_ERROR restart: start \n");
		felica_rcs956finalize(&rcs956dev);
		debug_printf(5, "[RCS956 THD] ICS_ERROR restart: finish \n");
	}
}

static void *polling_keeper(void *data)
{
	while(polling_thread_run){
		do_polling();
		usleep(100000);
	}
	return NULL;
}


#define FELICA_LOG_SINGLE 3600

static FILE *felica_log_fp=NULL;
static time_t felica_log_time;
static int felica_log_counter;

void felica_log_renew()
{
	char tmps[SMALL_STR];
	
	if(felica_log_fp) fclose(felica_log_fp);
	sprintf(tmps, "%s/felica-%04d.log", log_dir, felica_log_counter);
	felica_log_time+=FELICA_LOG_SINGLE;
	felica_log_fp=fopen(tmps, "w");
}

void felica_log_write(char *s)
{
	char tmps[SMALL_STR];

	if(!felica_log_fp){
		debug_printf(1, "Could not write log\n%s\n", s);
		return;
	}
	fprintf(felica_log_fp, "%s", s);
	fflush(felica_log_fp);
	
	// 1hour check
	if(felica_log_time+FELICA_LOG_SINGLE<time(NULL)){
		felica_log_counter++;
		felica_log_renew();
		if(!felica_log_fp){
			debug_printf(0, "Could not open new log file : %s\n", tmps);
			return;
		}
		felica_log_time+=FELICA_LOG_SINGLE;
	}
}

static gboolean update_display(gpointer data)
{
	char tmps[SMALL_STR];
	char tmps2[SMALL_STR];
	int	disp_on=1, logging_on=1;
	
#if 1	
	// エラーレート文字列
	sprintf(tmps2, "Total = %d : Success = %d : Success rate = %d %%", 
		tatal_count, tatal_count-error_count, (tatal_count==0)? 0 : (int)((double)(tatal_count-error_count)/tatal_count*100));
	gtk_label_set_text(GTK_LABEL(lb_error_rate), tmps2);
#endif
	
	gtk_label_set_text(GTK_LABEL(lb_menutop), "FeliCa");
	if(!message_dirty) return TRUE;
	
	switch(message_type){
	case RCS956_ICS_ERROR: /* ICS ERRROR */
		debug_printf(5, "[RCS956 DISP] %s (%s)\n", RCS956_ICS_ERROR_MESSAGE, message_buf);
		disp_on=logging_on=0;
		break;
	
	case RCS956_RF_OFF_ICS_ERROR: /* rf off ICS ERRROR */
		debug_printf(5, "[RCS956 DISP] %s (%s)\n", RCS956_RF_OFF_ICS_ERROR_MESSAGE, message_buf);
		disp_on=logging_on=0;
		break;
	
	case RCS956_OPEN_ERROR: /* RCS956 open error */
		strcpy(tmps, RCS956_OPEN_NG_MESSAGE);
		break;
	
	case RCS956_PORT_NO_GET_ERROR: /* RCS956 usbfs portno get error */
		strcpy(tmps, RCS956_PORT_NO_GET_NG_MESSAGE);
		break;
	
	case RCS956_ICS_SUCCESS: /*	FeliCa ICS SUCCESS */
	case RCS956_TYPEA_ICS_SUCCESS: /* TYPEA ICS SUCCESS	*/
	case RCS956_TYPEB_ICS_SUCCESS: /* TYPEB ICS SUCCESS	*/
		strcpy(tmps, message_buf);
		break;
	
	default: /*	other error */
		strcpy(tmps, RCS956_ICS_OTHER_ERROR_MESSAGE);
		disp_on=logging_on=0;
		break;
	}
	
	sc_time_set(tmps);
	strcat(tmps, " ");
	strcat(tmps, message_buf);
	strcat(tmps, "\n");
	message_dirty=0;
	
	if(disp_on){
		sc_gtk_text_view_insert((GtkTextView *)tv_result, tmps, strlen(tmps));
		gtk_label_set_markup(GTK_LABEL(lb_menutop), "<span bgcolor=\"orange\">FeliCa</span>");
		
	}
	
	if(logging_on)
		felica_log_write(tmps);
	
	return TRUE;
}

int	felica_rcs956initialize(ICS_HW_DEVICE *dev, felica_cc_devf_t *devf)
{
	int r, devn;
	char felica_port[SMALL_STR];
	char *p;

//	devn=felica_find_device();
//	sprintf(felica_port, "/proc/bus/usb/002/%03d", devn);
	p = scan_rcs956();
	if (p) {
		strcpy(felica_port, p);
		free(p);
	} else {
		debug_printf(5, "[RCS956] device not found.\n");
		return 0;
   	}
	
	r=g_drv_func->open(dev, felica_port);
	if(r!=ICS_ERROR_SUCCESS){
		debug_printf(5, "[RCS956] failure in open %s (%u) ...\n", felica_port, r);
		return 0;
   	}
	debug_printf(5, "[RCS956] open success[%s]\n", felica_port);
	
	if(g_drv_func->set_speed!=NULL){
		debug_printf(5, "[RCS956] calling set_speed(%u) ...\n", RCS956_DEFAULT_SPEED);
		r=g_drv_func->set_speed(dev, RCS956_DEFAULT_SPEED);
		if(r!=ICS_ERROR_SUCCESS){
			debug_printf(5, "[RCS956] failure in set_speed(%u) ...\n", r);
			return 0;
		}
	}

	if(g_drv_func->initialize_device != NULL){
		debug_printf(5, "[RCS956] calling initialize_device(%u) ...\n",  RCS956_DEFAULT_TIMEOUT);
		r=g_drv_func->initialize_device(dev, RCS956_DEFAULT_TIMEOUT);
		if(r!=ICS_ERROR_SUCCESS){
			debug_printf(5, "[RCS956] failure in initialize_device(%u) ...\n", r);
			return 0;
		}
	}
	
	r=(*g_felica_cc_stub_initialize_func)(devf, dev);
	if(r!=ICS_ERROR_SUCCESS){
		debug_printf(5, "[RCS956] failure in felica_cc_stub_initialize(%u) ...\n",r);
		return 0;
	}
	
	debug_printf(5, "[RCS956] initialization completed.\n");
	return 1;
}

static char RCS956_open_error[]="<span color=\"red\">RCS956 open error</span>";
static char RCS956_open_success[]="RCS956 open success";

static void press_start(GtkWidget *widget, gpointer data)
{
	sc_gtk_text_view_clear(GTK_TEXT_VIEW(tv_result));
	error_count = 0;
	tatal_count = 0;
	
	if(!felica_rcs956initialize(&rcs956dev, &rcs956devf)){
		debug_printf(5, "[RCS956 START] rcs956 open error\n");
		felica_rcs956finalize(&rcs956dev);
		gtk_label_set_markup(GTK_LABEL(lb_status), RCS956_open_error);
		return;
	}
	felica_log_time=time(NULL);
	polling_run=1;
	gtk_label_set_text(GTK_LABEL(lb_status), RCS956_open_success);
	gtk_widget_set_sensitive(b_start, FALSE);
	gtk_widget_set_sensitive(cb, FALSE);
	gtk_widget_set_sensitive(b_stop, TRUE);
	gtk_widget_set_sensitive(b_quit, FALSE);
	debug_printf(5, "[RCS956 START] polling start out\n");
}
static gboolean press_start_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_start(widget, data);
	return FALSE;
}

static void press_stop(GtkWidget *widget, gpointer data)
{
	gtk_widget_set_sensitive(b_stop, FALSE);
	polling_run=0;
	debug_printf(5, "[RCS956 STOP] rcs956 finish\n");
	felica_rcs956finalize(&rcs956dev);
	gtk_label_set_text(GTK_LABEL(lb_status), "");
	gtk_widget_set_sensitive(cb, TRUE);
	gtk_widget_set_sensitive(b_start, TRUE);
	gtk_widget_set_sensitive(b_quit, TRUE);
}
static gboolean press_stop_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	press_stop(widget, data);
	return FALSE;
}

static void felica_set_power(int x)
{
	int fd;
	
	fd=wpcio_open(WPCIO_OPEN_RETRY, "felica_set_power");
	if(fd<0) return;
	
	if(ioctl(fd, WPC_CONNECT_USB_FELICA, x)<0)
		debug_printf(3, "WPC_CONNECT_USB_FELICA ioctl failure.");
	
	close(fd);
}

static void felica_toggle(GtkWidget *widget, gpointer data)
{
	int is_on;
	
	is_on=(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))==TRUE);
	gtk_widget_set_sensitive(b_start, is_on ? TRUE : FALSE);
	felica_set_power(is_on);
}
static gboolean felica_toggle_func(
		GtkWidget *widget, GdkEvent *event, gpointer data)
{
	int is_on;
	is_on=(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))==TRUE);
	gtk_toggle_button_set_active(
			GTK_TOGGLE_BUTTON(widget), is_on ? FALSE : TRUE);
	felica_toggle(widget, data);
	return FALSE;
}


int felica_main(GtkWidget *table, GtkWidget *bsub)
{
	int button_no;
	pthread_t th;
	GtkWidget *v0, *bb;
	GtkWidget *a1, *v1, *sc;
	GtkWidget *a2, *h2;
	GtkWidget *a3;
	
	error_count = 0;
	tatal_count = 0;
	
	felica_log_counter=0;
	felica_log_renew();
	
	v0 = gtk_vbox_new(FALSE, 10);
	
	a1 = gtk_alignment_new(0.5, 0.5, 0, 0);
	
	v1 = gtk_vbox_new(FALSE, 10);
	
	lb_status = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(v1), lb_status);
	
	lb_error_rate = gtk_label_new("lb_error_rate");
	gtk_container_add(GTK_CONTAINER(v1), lb_error_rate);
	
	sc = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sc), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gint scw = (gint)((double)get_sc_window_width() * 0.391);
	gint sch = (gint)((double)get_sc_window_height() * 0.300);
	gtk_widget_set_usize(sc, scw, sch);
	tv_result=gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(tv_result), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(tv_result), FALSE);
	gtk_container_add(GTK_CONTAINER(sc), tv_result);
	gtk_container_add(GTK_CONTAINER(v1), sc);
	gtk_container_add(GTK_CONTAINER(a1), v1);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	
	h2=gtk_hbox_new(FALSE, 10);
	a2=gtk_alignment_new(0.5, 0.5, 0, 0);
	b_start=gtk_button_new_with_label("Start Polling");
	g_signal_connect(b_start, "button-release-event", G_CALLBACK(press_start_func), (gpointer)0);
	gtk_widget_set_sensitive(b_start, FALSE);
	b_stop=gtk_button_new_with_label("Stop Polling");
	g_signal_connect(b_stop, "button-release-event", G_CALLBACK(press_stop_func), (gpointer)0);
	gtk_widget_set_sensitive(b_stop, FALSE);
	gtk_container_add(GTK_CONTAINER(h2), b_start);
	gtk_container_add(GTK_CONTAINER(h2), b_stop);
	gtk_container_add(GTK_CONTAINER(a2), h2);
	gtk_container_add(GTK_CONTAINER(v0), a2);
	
	a3=gtk_alignment_new(0.5, 0.5, 0.2, 0.5);
	cb=gtk_check_button_new_with_label("Turn on FeliCa Unit");
	felica_set_power(0);
	//g_signal_connect(cb, "toggled", G_CALLBACK(felica_toggle), (gpointer)0);
	g_signal_connect(cb, "button-release-event", G_CALLBACK(felica_toggle_func), (gpointer)0);
	
	gtk_container_add(GTK_CONTAINER(a3), cb);
	gtk_container_add(GTK_CONTAINER(v0), a3);
	
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(&button_no, bsub, b_quit, sc_bbox1_click_func);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	polling_thread_run=1;
	pthread_create(&th, NULL, polling_keeper, 0);
	gtk_widget_show_all(v0);

	/*	20110818VACS	*/
	felica_timer_tag = g_timeout_add(100, update_display, 0);

	gtk_main();
	message_dirty=0;
	polling_run=polling_thread_run=0;
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	usleep(200000);
	if(felica_log_fp){
		fclose(felica_log_fp);
		felica_log_fp=NULL;
	}
	pthread_join(th, NULL);
	felica_set_power(0);

	/*	20110818VACS	*/
	if(felica_timer_tag){
		g_source_remove(felica_timer_tag);
		felica_timer_tag = (guint)0;
	}

	return 0;
}
