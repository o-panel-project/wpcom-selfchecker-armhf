/*  --------------------------------------------------------------------  */
/*  Wireless LAN environment measurement tool                             */
/*  --------------------------------------------------------------------  */
/*  Function    : WLAN environment & performance examination.             */
/*  --------------------------------------------------------------------  */
/*  History     : 2012.02.06 (v1.0) -  First release                      */
/*                2013.08.21 (v3.0) -  for j3                             */
/*  --------------------------------------------------------------------  */


#ifndef READLAN_H
#define READLAN_H

// -- include --
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


// -- defined value --
#define FOR_BASIC      0
#define FOR_THROUGHPUT 1
#define FOR_ERROR_RATE 2
#define FOR_RSSI       3
#define FOR_RSSI_PRE   4

#define CONFIG_FILE "./sokuteicfg"

#define   ON         1
#define   OFF        0
#define   NET_ERROR  2
#define   DOWN_ERROR 1
#define   NO_ERROR   0

// -------------------------------------
// 	Function prototype
// -------------------------------------

int  get_environment_info(int mode);
int  get_basic_info();
int  get_throughput_info();
int  get_errorrate_info();
int  get_otherchannel_info();

int  start_downstream();
int  stop_downstream();
int  start_network();
int  stop_network();

int  do_ftp();
int  do_ping();
int  file_analysis(int mode, int ch);
int  get_time();
int  diff_time(int start, int end);
void *int_handler();
int  dummy_func(int mode);
void removeLF(char *string);
int  ReadConfig();
int  ReadOneConfig(int n, char *str);
int  IsGetIP();

extern int net_interval;

extern int test_error;
extern int TstRunning;

extern char HOSTPC[256];       // 0
extern char RESULTFILE[256];   // 1
extern char TESTFILE1[256];    // 2
extern char TESTFILE2[256];    // 3
extern char TESTFILE3[256];    // 4
extern char TEMPFILE[256];     // 5
extern char FTPTEMPFILE[256];  // 6
extern char DOWNFILE[256];     // 7
extern char WL_ASSOC[256];     // 8
extern char WL_WEPSTAT[256];   // 9
extern char WL_RSSI[256];      // 10
extern char WL_PKTCNT[256];    // 11
extern char IWLIST_CH[256];    // 12
extern char WL_SCANRSLT[256];  // 13
extern char IFCONFIG[256];     // 14
extern char WGET[256];         // 15
extern char OPT_FTP[256];      // 16
extern char OPT_ERRRATE[256];  // 17
extern char OPT_PING[256];     // 18
extern char IFUP[256];         // 19
extern char IFDOWN[256];       // 20
extern char EXIT_BTN[256];     // 21


#endif

