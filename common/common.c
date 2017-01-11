/*
 *   Common functions, vars defs
 *
 *					2011.6	written by Omoikane Inc.
 */
#include <stdio.h>
#include <stdarg.h>
/* define this to get getpgid() */
#define __USE_XOPEN_EXTENDED
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include "md5.h"
#include "common.h"
#include "wpcio.h"

int debug_level=0, pid_battery_logger=0;
int testtime=0, interval=0, udhcpc_kill_check=0;
char *base_path, *log_dir, *felica_port;
int battery_sub_run=0;

int pid_bat1_charge=0, pid_bat2_charge=0;	/*	20110728VACS	*/
int bl_toggle_charge=1;						/*	20110831VACS	*/
int	cr_mount_ok=0;							/*	20110905VACS	*/

static int m_sc_window_height = DEFAULT_WINDOW_HEIGHT;
static int m_sc_window_width = DEFAULT_WINDOW_WIDTH;
void set_sc_window_height(int h) {m_sc_window_height = h;}
void set_sc_window_width(int w) {m_sc_window_width = w;}
int get_sc_window_height() {return m_sc_window_height;}
int get_sc_window_width() {return m_sc_window_width;}

void debug_set(int n)
{
	debug_level=n;
}

int debug_printf(int n, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	
	return (debug_level<=n) ? vprintf(fmt, ap) : 0;
}

int append_sprintf(char *dest, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	
	return vsprintf(dest+strlen(dest), fmt, ap);
}

void append_hex(char *dest, int n, char *s)
{
	int i;
	
	for(i=0;i<n;i++)
		append_sprintf(dest, "%02x", s[i]);
}

int time_diff_msec(struct timeval *now, struct timeval *past)
{
	int n;
	
	n=now->tv_sec-past->tv_sec;
	n*=1000;
	n+=(now->tv_usec-past->tv_usec)/1000;
	return n;
}

int time_passed(struct timeval *tv0, int msec)
{
	struct timeval tv1;
	
	gettimeofday(&tv1,NULL);
	return msec < time_diff_msec(&tv1, tv0);
}

int sc_time_set(char *buf)
{
	time_t t;
	struct tm *tm;
	
	t=time(NULL);
	tm=localtime(&t);
	sprintf(buf, "%d/%02d/%02d %02d:%02d:%02d",
				1900+tm->tm_year, 1+tm->tm_mon, tm->tm_mday,
				tm->tm_hour, tm->tm_min, tm->tm_sec);
	return strlen(buf);
}

void do_kill(int pid)
{
	int s;
	
	if(!pid) return;
	kill(pid, SIGTERM);
	usleep(100000);
	waitpid(pid, &s, WNOHANG);
	usleep(200000);
	kill(pid, SIGKILL);
}

void do_killpg(int pid)
{
	int s, pg;
	
	if(!pid) return;
	pg=getpgid(pid);
	if(!pg) return;
	killpg(pg, SIGTERM);
	usleep(100000);
	waitpid(pid, &s, WNOHANG);
	usleep(200000);
	kill(pid, SIGKILL);
	killpg(pg, SIGKILL);
}

int have_proc_pid(int pid)
{
	char tmps[SMALL_STR];
	struct stat st;
	
	sprintf(tmps, "/proc/%d", pid);
	return stat(tmps, &st)==0;
}

void battery_check_logger_stop()
{
	if(pid_battery_logger)
		do_killpg(pid_battery_logger);
	pid_battery_logger=0;
}

int get_md5sum(char *f, unsigned char *digest)
{
	struct MD5Context ctx;
	FILE *fp;
	unsigned char buf[SMALL_STR];
	int n;
	
	fp=fopen(f, "r");
	if(!fp) return 0;
	
	MD5Init(&ctx);
	while((n=fread(buf, 1, SMALL_STR, fp))>0){
		MD5Update(&ctx, buf, n);
	}
	MD5Final(digest, &ctx);
	fclose(fp);
	return 1;
}

int is_mountpoint(const char *path)
{
	char *ppath;
	struct stat st0, st1;
	
	if(stat(path, &st0)!=0) return 0;
	ppath=malloc(strlen(path)+10);
	strcpy(ppath, path);
	strcat(ppath, "/..");
	if(stat(ppath, &st1)!=0) return 0;
	
	return (st0.st_dev != st1.st_dev) || (st0.st_dev == st1.st_dev && st0.st_ino == st1.st_ino);
}

int wpcio_open(int n, char *tag)
{
	int i, fd = -1;
	
	for(i=0;i<n;i++){
		fd=open("/dev/wpcio", O_RDWR);
		if(0<=fd) break;
		usleep(50000);
	}
	if(i==n)
		debug_printf(3, "%s : Could not open /dev/wpcio (%d).\n", tag, getpid());
	
	return fd;
}

void sc_i2c_set_power(int x)
{
	int fd;
	
	fd=wpcio_open(WPCIO_OPEN_RETRY, "sc_i2c_set_power");
	if(fd<0) return;
	
	if(ioctl(fd, WPC_CONNECT_CRADLE, x)<0)
		debug_printf(3, "WPC_CONNECT_CRADLE ioctl failure.");
	
	close(fd);
}

int sc_get_board_type()
{
	int fd;
	int type;
	
	fd = wpcio_open(WPCIO_OPEN_RETRY, "sc_get_board_type");
	if (fd < 0)
		return WPC_BOARD_TYPE_J;

	if ((type = ioctl(fd, WPC_GET_BOARD_TYPE, 0)) < 0) {
		debug_printf(3, "WPC_GET_BOARD_TYPE ioctl failure.");
		type = WPC_BOARD_TYPE_J;
	}
	if (type == 0)
		type = WPC_BOARD_TYPE_J;

	close(fd);

	return type;
}

int sc_IsJ4()
{
	char name[32];

	if (gethostname(name, 32) == 0) {
		if (strcmp(name, "j4-panel") == 0)
			return 1;
	}
	return 0;
}
