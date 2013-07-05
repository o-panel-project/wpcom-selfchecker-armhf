/*
 *   Common functions, vars defs
 *
 *					2011.6	written by Omoikane Inc.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
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

//
//	i2c subroutines
//

#define CRADLE_ADDR 0x55
#define I2C_DEVICE "/dev/i2c-2"

int sc_i2c_check_open(int *fd)
{
	if(0<*fd) return *fd;
	
	*fd=open(I2C_DEVICE, O_RDWR);
	if(*fd<0) return *fd;
	
	if(ioctl(*fd, I2C_SLAVE, CRADLE_ADDR)<0){
		debug_printf(3, "Error: Could not set address : %s\n", strerror(errno));
		return -errno;
	}
	return 0;
}

//
//	Cyclic selectin of write data
//

static char *write_data[]={ "aaaa", "bbbb", "1234", "1a2b", "cccc" };
static int write_data_pointer=0;

static char *sc_i2c_pick_write_data()
{
	write_data_pointer++;
	if(4<write_data_pointer) write_data_pointer=0;
	return write_data[write_data_pointer];
}

// i2c read
// 0 on success, 1 on open error, 2 on ioctl failure
int sc_i2c_read_cradle(unsigned char *buf)
{
	int fd=0, i, r;

	bzero(buf, I2C_DATA_SIZE);
	
	if(sc_i2c_check_open(&fd)<0)
		return 1;
	
	for(i=0;i<I2C_DATA_SIZE;i++){
		usleep(I2C_GUARDTIME_USEC);
		r=i2c_smbus_read_byte_data(fd, i);
		if(r<0){
			close(fd);
			return 2;
		}
		buf[i]=r;
	}
	close(fd);
	return 0;
}

// i2c write
// 0 on success, 1 on open error, 2 on ioctl failure
int sc_i2c_write_cradle(unsigned char *buf)
{
	int fd=0, i;
	char *data;
	
	if(sc_i2c_check_open(&fd)<0)
		return 1;
	
	data=sc_i2c_pick_write_data();
	memcpy(buf, data, I2C_DATA_SIZE);
	
	for(i=0;i<I2C_DATA_SIZE;i++){
		usleep(I2C_GUARDTIME_USEC);
		if(i2c_smbus_write_byte_data(fd, i, buf[i])<0){
			close(fd);
			return 2;
		}
	}
	close(fd);
	return 0;
}

int wpcio_open(int n, char *tag)
{
	int i, fd;
	
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
