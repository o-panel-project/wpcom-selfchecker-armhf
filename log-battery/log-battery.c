/*
 *		Battery Logger
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
#include <sys/ioctl.h>
//#include <gtk/gtk.h>
#include "common.h"
#include "wpcio.h"
#include "sc_battery.h"

static int fd_log=0;
static int get_value_counter=0;
static int machine_type = WPC_BOARD_TYPE_J3;

static void get_value()
{
#if 0
	int fd_wpcio;
	int x0, x1, r0, r1, r2, r3, r4;
	int dc, bat1, bat2, v0, v1, v2, bc0;
#endif
	int x0, x1;
	int v0, v1, v2, bc0;
	char tmps[2048];
	
	get_value_counter++;
	if(!(get_value_counter&1)) return;
#if 0
	fd_wpcio=wpcio_open(WPCIO_OPEN_RETRY, "log-battery");
	if(fd_wpcio<0) return;
	
	r0=ioctl(fd_wpcio, WPC_GET_BAT1_CHARGING_STAT, &x0);
	if(r0<0){
		printf("Could not get battery 1 charging status, error code = %d\n", r0);
	}
/*
	r1=ioctl(fd_wpcio, WPC_GET_BAT2_CHARGING_STAT, &x1);
	if(r1<0){
		printf("Could not get battery 2 charging status, error code = %d\n", r1);
	}
*/
	r2=ioctl(fd_wpcio, WPC_GET_DC_LEVEL, &dc);
	if(r2<0){
		printf("Could not get adc value(0), error code = %d\n", r2);
	}
	r3=ioctl(fd_wpcio, WPC_GET_BAT1_LEVEL, &bat1);
	if(r3<0){
		printf("Could not get adc value(1), error code = %d\n", r3);
	}
/*
	r4=ioctl(fd_wpcio, WPC_GET_BAT2_LEVEL, &bat2);
	if(r4<0){
		printf("Could not get adc value(2), error code = %d\n", r4);
	}
*/
	close(fd_wpcio);
	
	v0=dc * (DC_RL + DC_RH) / DC_RL;
	v1=bat1 * (BAT_RL + BAT_RH) / BAT_RL;
	v2=bat2 * (BAT_RL + BAT_RH) / BAT_RL;
#endif
	if (machine_type == WPC_BOARD_TYPE_O)
		sc_get_battery_status_o("log-battery", &v0, &v1, &x0, &v2, &x1);
	else
	sc_get_battery_status("log-battery", &v0, &v1, &x0, &v2, &x1);
	bc0 = sc_get_cradle_battery_level(machine_type);
	if (bc0 == CRADLE_BAT_OPEN_ERROR || bc0 == CRADLE_BAT_MODE_ERROR) {
		/* cradle detach? */
		bc0 = -1;
	} else {
		if (bc0 < 500)	/* battery attach threshold 0.5V */
		bc0 = 0;
	}

	sc_time_set(tmps);
	append_sprintf(tmps, ",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", v0, v1, v2,
					(0<(x0 & WPC_CHARGING_FAST)), (0<(x0 & WPC_CHARGING_FULL)), (0<(x0 & WPC_CHARGING_FAULT)),
					(0<(x1 & WPC_CHARGING_FAST)), (0<(x1 & WPC_CHARGING_FULL)), (0<(x1 & WPC_CHARGING_FAULT)), bc0);
	write(fd_log, tmps, strlen(tmps));
	fsync(fd_log);
}

static void start_timer()
{
	struct itimerval itimer;
	struct sigaction sa = {
		.sa_sigaction = get_value,
		.sa_flags = SA_RESTART | SA_SIGINFO,
	};

	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
	
	itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = 1;
	itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &itimer, NULL);
}

int main(int argc, char *argv[])
{
	machine_type = sc_get_board_type();
	fd_log=open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if(fd_log<0){
		printf("Could not open log file : %s.", argv[1]);
		return 0;
	}
	start_timer();
	while(1)sleep(1);
}
