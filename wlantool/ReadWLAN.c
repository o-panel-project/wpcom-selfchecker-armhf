/*  --------------------------------------------------------------------  */
/*  Wireless LAN environment measurement tool                             */
/*  --------------------------------------------------------------------  */
/*  Function    : WLAN environment & performance examination.             */
/*  --------------------------------------------------------------------  */
/*  History     : 2012.02.06 (v1.0) -  First release                      */
/*  --------------------------------------------------------------------  */


#ifndef READWLAN_H
#define READWLAN_H

#include "ReadWLAN.h"

#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#if 1
#define PING_COUNT 1000
#define FTP_WAIT   120
#else
#define PING_COUNT 10
#define FTP_WAIT   10
#endif

int	search_token(char *token, int start, int num);
int	do_scan ();
int	get_scan ( int ch );


static	char	ssid[64]		= "@NoAP@";
static	char	association[32]	= "0";
static	char	wep[32]			= "0";
static	char	channel[32]		= "@NoCh@";
static	char	bssid[32]		= "XX:XX:XX:XX:XX:XX";
static	char	macadrs[32]		= "XX:XX:XX:XX:XX:XX";
static	int 	rssi			= -100;
static	int	throughput		= 0;
static	float	error_rate		= 100.00;
static	int	other_rssi[13]	= {-100,-50,-100,-30,-100,-80,-100,-60,-50,-50,-100,-100,-100};

static	int	processing_flag = 0;
static	pid_t	down_pid;
static	pid_t	net_pid;
static	pid_t	ftp_pid;
static	pid_t	scan_pid;

static	int		tmp_a;

char HOSTPC[256];       // 0
char RESULTFILE[256];   // 1
char TESTFILE1[256];    // 2
char TESTFILE2[256];    // 3
char TESTFILE3[256];    // 4
char TEMPFILE[256];     // 5
char FTPTEMPFILE[256];  // 6
char DOWNFILE[256];     // 7
char WL_ASSOC[256];     // 8
char WL_WEPSTAT[256];   // 9
char WL_RSSI[256];      // 10
char WL_PKTCNT[256];    // 11
char IWLIST_CH[256];    // 12
char WL_SCANRSLT[256];  // 13
char IFCONFIG[256];     // 14
char WGET[256];         // 15
char OPT_FTP[256];      // 16
char OPT_ERRRATE[256];  // 17
char OPT_PING[256];     // 18
char IFUP[256];         // 19
char IFDOWN[256];       // 20
char EXIT_BTN[256];     // 21


// -------------------------------------
// 	Function body
// -------------------------------------

// -- Getting WLAN environment information --
int	get_environment_info ( int mode )
{
	int		ret = 0;
	int		fp1;
	char	tmp[200];

	ret = ReadConfig(); 
	printf( "[get environment_info] start\n" );

	if ( mode == 0 ) {
		ret += get_basic_info();
		ret += get_throughput_info();
		ret += get_errorrate_info();
	}
	ret += get_otherchannel_info();	

	// file open
	fp1 = open(TESTFILE1,(O_WRONLY|O_CREAT|O_TRUNC),((mode_t)0777));
	if ( fp1 < 0 ) {	
		printf( "[get_environment_info] file cannot open\n" );
		test_error =3;                                   
		perror(NULL);
		return -1;
	}
	printf( "  > file open\n" );

	// file lock
	flock( fp1, LOCK_EX );
	printf( "  > file lock\n" );

	printf( "%s,%s,%s,%s,%s,%s,%d,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
		ssid, association, wep, channel,bssid, macadrs, rssi, throughput, error_rate,
		other_rssi[0], other_rssi[1], other_rssi[2], other_rssi[3], other_rssi[4],  other_rssi[5],
		other_rssi[6], other_rssi[7], other_rssi[8], other_rssi[9], other_rssi[10], other_rssi[11],
		other_rssi[12]);

	sprintf( tmp, "%s,%s,%s,%s,%s,%s,%d,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		ssid, association, wep, channel, bssid,macadrs, rssi, throughput, error_rate,
		other_rssi[0], other_rssi[1], other_rssi[2], other_rssi[3], other_rssi[4],  other_rssi[5],
		other_rssi[6], other_rssi[7], other_rssi[8], other_rssi[9], other_rssi[10], other_rssi[11],
		other_rssi[12]);

	// date write
	write( fp1, tmp, strlen(tmp) );
	printf( " > data write\n" );

	// file unlock
	flock( fp1, LOCK_UN );
	printf( " > file unlock\n" );

	// file close
	close( fp1 );
	printf( " > file close\n" );

	// error check
	if (ret < 0) {
		return -1;
	}

	return 0;
}


// -- Getting WLAN basic information --
int	get_basic_info()
{
	int		ret = 0;
	int		i = 0;
	char	cmdstr[256];
	
	printf( "[get_basic_info] start\n" );
	
	// j-panel : iwlist wlanX channel
	sprintf(cmdstr, "%s > %s", IWLIST_CH, TEMPFILE);
	printf(" > execute command=%s\n", cmdstr);
	ret = system(cmdstr);
	strcat(cmdstr, TEMPFILE);
	if (ret != 0) {
		goto error_path;
	}
	
	sprintf(cmdstr, "%s >> %s", WL_ASSOC, TEMPFILE);
	for (i=0;i<5;i++) {
		printf(" > execute command=%s\n", cmdstr);
		ret = system(cmdstr);
		if (ret != 0) {
			goto error_path;
		}
	}
	
	// j-panel : ifconfig
	sprintf(cmdstr, "%s >> %s", IFCONFIG, TEMPFILE);
	printf(" > execute command=%s\n", cmdstr);
	ret = system(cmdstr);
	strcat(cmdstr, TEMPFILE);
	if (ret != 0) {
		goto error_path;
	}
	
	// analysis a temporary file
	ret = file_analysis(FOR_BASIC,0);
	
	//	if (ret != 0) goto error_path;
	switch( ret ) {
		case -1 : goto error_path;
		case -2 : goto error_ssid;
		case -3 : goto error_channel;
		case -4 : goto error_bssid;
		case -5 : goto error_wepstatus;
		case -6 : goto error_rssi;
		case -7 : goto error_macadrs;
	}

	// finish point without error
	return 0;


// !!!! error route !!!!
error_path:
error_ssid:
	strcpy( ssid, "@NoAP@" );
	strcpy( association, "0" );

error_channel:
	strcpy( channel, "@NoCh@" );

error_bssid:
	strcpy( bssid, "XX:XX:XX:XX:XX:XX" );
        printf("\nBSSID: %s\n",bssid);
error_wepstatus:
	strcpy( wep, "0" );

error_rssi:
	rssi = -100;

error_macadrs:
	strcpy( macadrs, "XX:XX:XX:XX:XX:XX" );
         printf("\nMAC: %s\n",macadrs);
	throughput = 0;
	error_rate = 100.00;
	other_rssi[0] = -100;
	other_rssi[1] = -100;
	other_rssi[2] = -100;
	other_rssi[3] = -100;
	other_rssi[4] = -100;
	other_rssi[5] = -100;
	other_rssi[6] = -100;
	other_rssi[7] = -100;
	other_rssi[8] = -100;
	other_rssi[9] = -100;
	other_rssi[10] = -100;
	other_rssi[11] = -100;
	other_rssi[12] = -100;

	printf( "[get_basic_info] something had error\n" );

	// finish point with error
	return -1;
}


// -- Getting WLAN throughput information --
int	get_throughput_info()
{
#define		LOOP 3
	int		ret = 0;
	int		total = 0;
	int		i = 0;

	printf( "[get_throughput_info] start\n" );

	// check 3 times
	for (i=0;i<LOOP;i++)
	{
		// execute FTP receive
		ret = do_ftp();
		if (ret < 0) {
			printf( "[get_throughput_info] FTP command error occurred\n" );
			throughput = 0;
			return -1;
		} 
		total += ret;
	}

	// getting average
	throughput = total / LOOP; 
	if ( throughput > 100000 ) throughput = 100000;
	printf( "[get_throughput_info] total throughput=%d[Kbps]\n", throughput );

	return 0;
}


// -- body of execute FTP receive --
int	do_ftp()
{
	int				ret = 0;
	int				start_time, end_time, tmp_time;
	unsigned long	tmp_byte;
	int				ftpfp1, ftpfp2;
	int				i;
	char			cmdstr[256];
	FILE			*fp;
	int				status;
	int				child_pid = 0;
	
	printf( "[do_ftp] start\n" );
	
	// file initialize
	ftpfp1 = open( FTPTEMPFILE, (O_WRONLY|O_CREAT|O_TRUNC), ((mode_t)0777) );
	if ( ftpfp1 < 0 ) {
		printf( "[do_ftp] %s cannot open(1)\n", FTPTEMPFILE );
		perror(NULL);
		return -1;
	}
	
	ret = -1;
	
	ret = write( ftpfp1, &ret, sizeof(ret) );
	
	if ( ret == -1 ) { 
		printf( "[do_ftp] write error\n" );
		perror(NULL);
	}
	printf( "[do_ftp] initialze data write to ftptemp.dat 1\n" );
	
	close( ftpfp1 );
	printf( "[do_ftp] %s close(1)\n", FTPTEMPFILE );
	
	// execute FTP command
	ftp_pid = fork();
	
	switch( ftp_pid ) {
		
		// error
		case -1 : {
			printf( "[do_ftp] child process making error!\n" );
			perror(NULL);
			return -1;
			break;
		}
		
		// child process
		case 0 : {
			printf( "[do_ftp::child] child process start!\n" );
			
			// getting time of start [unit:msec]
			start_time = get_time();
			printf( " > [C1] getting start time = %d\n", start_time );

			strcpy( cmdstr, WGET );
			strcat( cmdstr, " ftp://" );
			strcat( cmdstr, HOSTPC );
			strcat( cmdstr, DOWNFILE );
			strcat( cmdstr, " " );
			strcat( cmdstr, OPT_FTP );
			strcat( cmdstr, " /tmp/dummy.dat" );
			printf( " > [C1] execute command=%s\n", cmdstr );
			ret = system( cmdstr );
			if ( ret != 0 ) {
				printf( "[do_ftp::child] system command error! %d\n", ret);
				perror(NULL); 
				exit(1);	//return -1;
		 	}

			// getting time of end [unit:msec]
			end_time = get_time();
			printf( " > [C1] getting end time = %d\n", end_time );

			// getting number of received bytes after FTP comunication
			{
				FILE *fp = fopen("/tmp/dummy.dat", "rb"); 
				if (fp == NULL) {
					exit(1);
				}
				fseek(fp, 0, SEEK_END);
				fgetpos(fp, &tmp_byte);
				fclose(fp);
			}
			tmp_time = diff_time( start_time, end_time );
			printf( " > [C1] getting diff_byte = %lu, diff_time = %d\n", tmp_byte, tmp_time );

			// error check
			if ( tmp_time < 1) {
				printf( " > [C1] error occured!\n" );
				exit(1);	//return -1;
			}

			// calculation of throughput [unit:Kbps]
//			ret = (( tmp_byte / (tmp_time/1000)) * 8 ) / 1000;
			ret = (((double)tmp_byte / ((double)tmp_time/1000)) * 8 ) / 1000;
			printf( " > [C1] tmp_byte=%lu tmp_time=%d ret=%d\n", tmp_byte, tmp_time, ret);
			
			// save the result to file
			ftpfp1 = open( FTPTEMPFILE, (O_WRONLY|O_CREAT|O_TRUNC), ((mode_t)0777) );
			if ( ftpfp1 < 0 ) {
				printf( "[do_ftp::child] %s cannot open(2)\n", FTPTEMPFILE );
				perror(NULL); 
				exit(1);
			}
			
			write( ftpfp1, &ret, sizeof(ret) );
			printf( " > [C1] initialze data write to ftptemp.dat 2\n" );
			
			close( ftpfp1 );
			printf( " > [C1] %s close(2)\n", FTPTEMPFILE );

			printf( "[do_ftp::child] child process is finish\n" );
			exit(0);	//return ret;
		}

		// parent process
		default : {
			printf( "[do_ftp::parent] parent process start!\n" );
			
			// Save FTP process ID.
			fp = fopen( "/tmp/ftppid.dat", "w" );
			if ( fp == 0 ) {
				printf( "[do_ftp::parent] /tmp/ftppid.dat file cannot open\n" );
				perror(NULL);
				return -1;
			}
			
			fprintf( fp, "%d", ftp_pid );
			fclose( fp );
			
			printf( " > [P1] child process number is %d\n", ftp_pid );
			
			// 120 second sleep
			for( i=0; i<FTP_WAIT; i++ ) {
				sleep( 1 );
				ftpfp2 = open( FTPTEMPFILE, (O_RDONLY|O_NONBLOCK), ((mode_t)0777) );
				if ( ftpfp2 < 0 ) {
					;
				}
				else {
					read( ftpfp2, &ret, sizeof(ret) );
					printf( " > [P1] return value is %d (%d)\n", ret, i );
					close( ftpfp2 );
					if ( ret != -1 ) {
						printf( " > [P1] waiting for child process stop\n" );
						while((child_pid=waitpid( ftp_pid, &status, WNOHANG )) == 0 ) {
							;
						}
						printf( " > [P1] child process did stop\n" );
						return ret;
					}
				}
			}
			
			// parent kill the child process
			printf( "[do_ftp::parent] kill a child process!\n" );
			ret = kill( ftp_pid, SIGKILL );
			while((child_pid=waitpid( ftp_pid, &status, WNOHANG )) == 0 ) {
				;
			}
			printf( "[do_ftp::parent] return = %d\n", ret );
			if ( ret != 0 ) {
				printf( "[do_ftp:parent] Failed a child process killing!\n" );
			}
			
			return -1;
		  }
	}
	
	return -1;
}


#if 1
int	get_errorrate_info()
{
	int  ret;
	char cmdstr[256];
	
	printf( "[get_errorrate_info] start\n" );
	
	// execute ping command
	sprintf(cmdstr, "%s %s > %s", OPT_ERRRATE, HOSTPC, TEMPFILE);
	printf( " > execute command=%s\n", cmdstr );
	ret = system( cmdstr );
	if ( ret != 0 ) {
		printf( "[get_errorrate_info] system command error!\n" );
		return -1;
	}
	
	// 出力
	file_analysis(FOR_ERROR_RATE, 0);
	error_rate = (float)tmp_a;
 	printf("Error rate = %f\n", error_rate);
	
	return 0;
}
#else
// -- Getting communication error rate --
int	get_errorrate_info()
{    

	int		ret = 0;
	int		good_tx1 = 0;
	int		bad_tx1 = 0;
	int		good_rx1 = 0;
	int		bad_rx1 = 0;
	int		good_tx2 = 0;
	int		bad_tx2 = 0;
	int		good_rx2 = 0;
	int		bad_rx2 = 0;
	char	cmdstr[256];
	
	printf( "[get_errorrate_info] start\n" );
	
	// getting number of tx & rx packet
	strcpy( cmdstr, WL_PKTCNT );
	strcat( cmdstr, " > " );
	strcat( cmdstr, TEMPFILE );
	printf( " > execute command=%s\n", cmdstr );
	ret = system( cmdstr );
	if ( ret != 0 ) return -1;
	ret = file_analysis(FOR_ERROR_RATE,0);
	good_tx1 = tmp_a;
	bad_tx1 = tmp_b;
	good_rx1 = tmp_c;
	bad_rx1 = tmp_d;
 	printf( "---------- start\n" );
 	printf( " > GOOD_TX=%d\n", good_tx1 );
 	printf( " > BAD_TX =%d\n", bad_tx1  );
 	printf( " > GOOD_RX=%d\n", good_rx1 );
 	printf( " > BAD_RX =%d\n", bad_rx1  );
	
	// execute ping command
	strcpy( cmdstr, OPT_ERRRATE );
	strcat( cmdstr, " " );
	strcat( cmdstr, HOSTPC );
	strcat( cmdstr, " > /dev/null" );
	printf( " > execute command=%s\n", cmdstr );
	ret = system( cmdstr );
	if ( ret != 0 ) {
		printf( "[get_errorrate_info] system command error!\n" );
		return -1;
	}
	
	// getting number of tx & rx packet after ping
	strcpy( cmdstr, WL_PKTCNT );
	strcat( cmdstr, " > " );
	strcat( cmdstr, TEMPFILE );
	printf( " > execute command=%s\n", cmdstr );
	ret = system( cmdstr );
	if ( ret != 0 ) return -1;
	ret = file_analysis(FOR_ERROR_RATE,0);    
	good_tx2 = tmp_a;
	bad_tx2 = tmp_b;
	good_rx2 = tmp_c;
	bad_rx2 = tmp_d;
 	printf( "---------- end\n" );
 	printf( " > GOOD_TX=%d\n", good_tx2 );
 	printf( " > BAD_TX =%d\n", bad_tx2  );
 	printf( " > GOOD_RX=%d\n", good_rx2 );
 	printf( " > BAD_RX =%d\n", bad_rx2  );
	
	// calcularion of error rate [unit:%]
	error_rate = ((float)(abs(good_tx2-good_tx1-PING_COUNT) + abs(good_rx2-good_rx1-PING_COUNT) + abs(bad_tx2-bad_tx1) + abs(bad_rx2-bad_rx1)) / (float)(PING_COUNT*2));
	error_rate = error_rate * 100.00;			// Change to %
	if ( error_rate > 100.00 ) error_rate = 100.00;
	
	return 0;
}
#endif

// -- Getting radio strength an other channels --
int	get_otherchannel_info()
{        

	int		loop = 0;
	int		child_pid = 0;
	int		status;
	FILE	*fp;
	int		sock;
		
	printf( "[get_otherchannel_info] start\n" );
	
	scan_pid = fork();
	
	switch ( scan_pid ) {
		
		// error
		case -1 : {
			printf( "[get_otherchannel_info] child process start!\n" );
			perror( NULL );
			return -1;
			break;
		}
		
		// child process
		case 0 : {
			printf( "[get_otherchannel_info::child] child process start!\n" );
			
			do_scan();
			other_rssi[0]  = get_scan(1);
			other_rssi[1]  = get_scan(2);
			other_rssi[2]  = get_scan(3);
			other_rssi[3]  = get_scan(4);
			other_rssi[4]  = get_scan(5);
			other_rssi[5]  = get_scan(6);
			other_rssi[6]  = get_scan(7);
			other_rssi[7]  = get_scan(8);
			other_rssi[8]  = get_scan(9);
			other_rssi[9]  = get_scan(10);
			other_rssi[10] = get_scan(11);
			other_rssi[11] = get_scan(12);
			other_rssi[12] = get_scan(13);
			
 			// This is for test
			sock = socket(AF_INET, SOCK_STREAM, 0);
			printf( "socket number=%d\n", sock );
			
			// file open and check
			fp = fopen( "/tmp/scantemp.dat", "w" );
			if ( fp < 0 ) {
				printf( "[get_otherchannel_info::child] /tmp/scantemp.dat file cannot open!\n" );
				exit( 1 );
			}
			
			// data write to file
			for ( loop=0; loop<13; loop++ ) {
				fprintf( fp, "%d\n", other_rssi[loop] );
			}
			
			// close file
			fclose( fp );
			exit( 0 );
			break;
		}
		
		// parent process
		default : {
			printf( "[get_otherchannel_info::parent] parent process start!\n" );
			printf( "[get_otherchannel_info::parent] child process number is %d\n", scan_pid );
			
			// waiting for finish of child process
			printf( " > Now waiting for finish of child process " );
			while((child_pid=waitpid( scan_pid, &status, WNOHANG )) == 0 ) {
				printf( "." );
				sleep( 1 );
			}
			printf( "\n" );
			
			fp = fopen( "/tmp/scantemp.dat", "r" );
			if ( fp < 0 ) {
				printf( "[get_otherchannel_info::child] /tmp/scantemp.dat file cannot open!\n" );
				return -1;
			}
			
			// data read from file
			for ( loop=0; loop<13; loop++ ) {
				fscanf( fp, "%d", &other_rssi[loop] );
				printf( " > otehr_rssi[%d] = %d\n", loop, other_rssi[loop] );
			}
			
			// close file
			fclose( fp );
			
		}
	}	
	
	return 0;
}


// -- RSSI scanning --
int	do_scan ()
{
	int		ret = 0;
	char	cmdstr[256];
	int		starttime = 0;
	int		nowtime = 0;

	printf( "[do_scan] start\n" );
	
	starttime = get_time();
	printf( " > *starttime=%d\n", starttime );
	do {
		nowtime = get_time();
		if ( starttime > nowtime ) nowtime = nowtime + 1000000;
	} while((nowtime-starttime) < 250 );
	printf( " > *nowtime=%d\n", nowtime );
	
	// making display command of for result of channel scan
	strcpy( cmdstr, WL_SCANRSLT );
	strcat( cmdstr, " > " );
	strcat( cmdstr, TEMPFILE );
	printf( " > execute command=%s\n", cmdstr );
	ret = system( cmdstr );
	if ( ret != 0 ) {
		printf( "[do_scan] system command error!\n" );
		return -120;
	}
	
	return 0;
}

int	get_scan ( int ch )
{
	int		ret = 0;
	
	// getting maximum rssi
	ret = file_analysis(FOR_RSSI, ch);
	if ( ret == -1 ) {
		printf( "[do_scan] file analysis error!\n" );
		return -120;
	}
	
  	if ( ret > -20 ) ret = -20;
	
	return ret;
}


#define MAX_READ_LINE 1024
#define MAX_CHARACTER 80
static char tmp[MAX_READ_LINE][MAX_CHARACTER];  // file_analysisで読み込んだデータを格納するバッファ

int	file_analysis( int mode, int ch )
{
	int  ret = 0;
	char *tmp3;
	int  i = 0;
	int  j = 0;
	int  max = 0;
	FILE *fp;
	int  current = 0;
	
	int rssibuf = 0;
	int line = 0;
	
	memset(tmp, 0x00, sizeof(tmp));
	
	switch (mode) {
	case FOR_BASIC:
		printf( "[file_analysis] analysis for basic information\n" );
		
		// open the temporary file
		fp = fopen( TEMPFILE, "r" );
		if (fp == 0) {
			printf( "[file_analysis] %s cannot open!\n", TEMPFILE );
			perror(NULL);
			test_error = 3;
			return -1;
		}
		printf( " > %s opened!\n", TEMPFILE );
		
		// getting information
		for (i=0;i<MAX_READ_LINE;i++){
			fscanf(fp,"%s",tmp[i]);
			printf( " > tmp[%d]=%s\n", i, tmp[i] );  // For test
			ret = feof(fp);
			if (ret != 0) {
				break;
			}
		}
		
		// close the temporary file
		fclose(fp);
		
		// start analysis
		
		// ESSID
		i = search_token( "ESSID:", 0, 0 );
		if (i == -1) {
			return -2;
		}
		strtok( tmp[i], "\"" );
		tmp3 = strtok( NULL, "\"" );
		if (tmp3 != NULL) {
			strcpy( ssid, tmp3 );
			strcpy( association, "1" );
			printf( " > (1) detect SSID:\n" );
		} else {
			return -2;
		}
		
		// Channel
		i = search_token( "Current", 0, 0 );
		if (i == -1) {
			return -3;
		}
		tmp3 = strtok( tmp[i+4], ")" );
		if (tmp3 != NULL) {
			strcpy( channel, tmp3 );
			printf( " > (2) detect Channel:\n" );
		} else {
			return -3;
		}
		
		// BSSID
		i = search_token( "Point:", 0, 0 );
		if (i == -1) {
			return -4;
		}
		strcpy( bssid, tmp[i+1] );
		printf( " > (3) detect BSSID:\n" );
		
		// WEPSTATUS
		i = search_token( "key:", 0, 0 );
		tmp3 = strchr( tmp[i], ':' );
		if (strcmp(tmp3+1,"off") == 0) {
			strcpy( wep, "0" );
		}
		else {
			strcpy( wep, "1" );
		}
		printf( " > (4) detect WEPSTATUS:\n" );
		
		// RSSI
		rssibuf = 0;
		i = 0;
		for( j=0;j<5;j++ ) {
			i = search_token( "Signal", i+1, 0 );
			tmp3 = strpbrk( tmp[i+1], "=:" );
			if (tmp3 != NULL) {
				rssibuf += atoi(tmp3+1);
			} else {
				return -5;
			}
		}
		rssi = rssibuf / 5;
		printf( " > (5) detect RSSI:\n" );
		
		// MAC Address
		i = search_token( "HWaddr", 0, 0 );
		strcpy( macadrs, tmp[i+1] );
		printf( " > (6) detect HWaddr\n" );
		
		printf ( " --------------------------------\n" );
		printf ( " > SSID=%s\n", ssid );
		printf ( " > Association=%s\n", association );
		printf ( " > WEP=%s\n", wep );
		printf ( " > Channel=%s\n", channel );
		printf ( " > BSSID=%s\n", bssid );
		printf ( " > RSSI(avg)=%d\n", rssi );
		printf ( " > MACadrs=%s\n", macadrs );
		
		return 0;
	
	case FOR_THROUGHPUT:
		printf( "[file_analysis] analysis for throughput\n" );
		
		// open the temporary file
		fp = fopen( TEMPFILE, "r" );
		if (fp == 0) {
			printf( "[file_analysis] %s cannot open!\n", TEMPFILE );
			perror(NULL);
			test_error = 3;
			return -1;
		}  
		printf( " > %s was opened!\n", TEMPFILE );
		
		// getting information
		for (i=0;i<MAX_READ_LINE;i++){
			fscanf(fp,"%s",tmp[i]);
//			printf( " > tmp[%d]=%s\n", i, tmp[i] );  // For test
			ret = feof(fp);
			if (ret != 0) {
				break;
			}
		}
		
		// close the temporary file
		fclose(fp);
		
		// start analysis
		if ( strncmp( tmp[0], "wlan", 4) == 0 ) {		// checking adapter
			printf( " > (1) detect wlan0:\n" );
		}
		else {
			printf(" > (1) error\n");
			return -1;
		}
		
		if ( strcmp( tmp[5], "inet" ) == 0 ) {		// check link status
			printf( " > (2) detect inet:\n" );
		}
		else {
			printf(" > (2) error\n");
			return -1;
		}
		
		i = search_token( "RX", 0, 1 );
		if (i == -1) {
			printf(" > (3) error\n");
			return -1;
		}
		tmp3 = strchr( tmp[i+1], ':' );
		ret = atoi(tmp3+1);
		
		printf( " > get_byte=%d\n", ret );
		
		return ret;
		
	case FOR_ERROR_RATE:
		printf( "[file_analysis] analysis for error rate\n" );
		
#if 1
		// open temporary file
		fp = fopen(TEMPFILE, "r");
		if (fp == NULL) {
			printf( "[file_analysis] tempfile.dat cannot open!\n" );
			perror(NULL);  
			test_error = 3;
			return -1;
		}
		
		// getting information
		for (i=0; i<MAX_READ_LINE; i++) {
			fscanf(fp, "%s", tmp[i]);
			printf(" > tmp[%d]=%s\n", i, tmp[i]);
			ret = feof(fp);
			if (ret != 0) {
				break;
			}
		}
		
		// close the temporary file
		fclose(fp);
		
		i = search_token("received,", 0, 0);
		if (i == -1) {
			return -1;
		}
		sscanf(tmp[i+1], "%d%%", &tmp_a);
#else
		// open temporary file
		fp = fopen( TEMPFILE, "r" );
		if (fp == 0) {
			printf( "[file_analysis] tempfile.dat cannot open!\n" );
			perror(NULL);  
			test_error = 3;
			return -1;
		}
		
		// getting information
		for (i=0;i<MAX_READ_LINE;i++){
			fscanf(fp,"%s",tmp[i]);
			printf( " > tmp[%d]=%s\n", i, tmp[i] );  // For test
			ret = feof(fp);
			if (ret != 0) {
				break;
			}
		}
		
		// close the temporary file
		fclose(fp);
		
		// start analysis
		i = search_token( "TX", 0, 0 );
		if (i == -1) {
			return -1;
		}
		
		// tx good identifier
		tmp3 = strchr( tmp[i+1], ':' );
		tmp_a = atoi(tmp3+1);
		printf( " > (1) detect good\n" );
		
		// tx bad identifier
		tmp3 = strchr( tmp[i+2], ':' );
		tmp_b = atoi(tmp3+1);
		printf( " > (2) detect bad\n" );
		
		i = search_token( "RX", 0, 0 );
		if (i == -1) {
			return -1;
		}
		
		// rx good identifier
		tmp3 = strchr( tmp[i+1], ':' );
		tmp_c = atoi(tmp3+1);
		printf( " > (3) detect good\n" );
		
		// rx bad identifier
		tmp3 = strchr( tmp[i+2], ':' );
		tmp_d = atoi(tmp3+1);
		printf( " > (4) detect bad\n" );
#endif	
		return 0;
		
	case FOR_RSSI:
		printf( "[file_analysis] analysis for rssi of other channel\n" );
		
		// open the temporary file
		fp = fopen( TEMPFILE, "r" );
		if (fp == 0) {
			printf( "[file_analysis] %s cannot open!\n", TEMPFILE );
			perror(NULL);
			test_error = 3;
			return -1;
		}
		
		// getting information
		for (i=0;i<MAX_READ_LINE;i++){
			fscanf(fp,"%s",tmp[i]);
//			printf( " > tmp[%d]=%s\n", i, tmp[i] );  // For test
			ret = feof(fp);
			if (ret != 0) {
				break;
			}
		}
		line = i-1;
		
		// close the temporary file
		fclose(fp);
		
		i = 0;
		j = 0;
		max = -100;
		
		while (1) {
			i = search_token( "(Channel", i, 0 );
			printf( " %d",i );
			if (i == -1) {
				break;
			}
			tmp3 = strtok( tmp[i+1], ")" );
			ret = atoi(tmp3);
			if (ch == ret) {
				strtok( tmp[i+4], "=" );
				tmp3 = strtok( NULL, "=" );
printf( " >> %s\n", tmp3 );
#if 1
				if (tmp3 == NULL) {
					ret = 0;
				} else {
					ret = atoi(tmp3);
				}
#else
				ret = atoi(tmp3);
#endif
				current = ret;
				printf( " > this is different channel\n" );
				if ( current > max ) {
					max = current;
					printf( " > update maximum RSSI=%d\n", max );
				}
			}
			i++;
		}
		
		// if cannot detect RSSI
		if ( max == -100 ) {
			printf( "[file_analysis] It cannot detect RSSI!\n" );
		}
		
		return max;
	
	default :  // default case
			break;
	}
	
	return 0;
}


int	search_token(char *token, int start, int num)
{
	int i;
	int count = 0;
	int len;
	
	len = strlen(token);
	
	for (i=start;i<MAX_READ_LINE;i++) {
		if (strncmp(tmp[i],token,len) == 0) {
			if (count == num) {
				return i;
			}
			count++;
		}
	}
	return -1;
}


// -- function of start for downstream test --
int	start_downstream()
{
	int  ret = 0;
	char tmp[10];
	int  fp1, fp2;
	
	ret = ReadConfig();
	
	printf( "[start_downstream] start\n" );

	if ( processing_flag & 0x00000001 ) {
		test_error = 1;
			TstRunning = 1;
		return -1;		// if already running myself
	}

	down_pid = fork();

	switch( down_pid ) {

		// error
		case -1	: {
					printf( "[start_downstream] child process making error!\n" );
					test_error =1;  
					return -1;
					break;
				  }

		// child process
		case 0 	: {
			printf( "[start_donwstream::child] child process start!\n" );
			
			for (;;) {
				// ダウンロードストリーム
				ret = do_ftp();
				if (ret == -1) {
					printf( "[start_downstream::child] system function error!\n" );
					printf("> [C2] Error in Downstream test\n");
				} else {
					// file open
					fp1 = open( TESTFILE2, (O_WRONLY|O_CREAT), ((mode_t)0x0777) );
					if ( fp1 < 0 ) {		// If can't open a file
						printf( "[start_downstream::child] %s open error!\n", TESTFILE2 );
						perror(NULL); 
						exit(1);
						return -1;
					}

					// file lock
					flock( fp1, LOCK_EX );
					printf( " > [C2] file lock\n" );

					// data write
					sprintf(tmp, "%-6d", ret);
					write(fp1, tmp, strlen(tmp));
					
					printf( " > [C2] data write\n" );

					// file unlock
					flock( fp1, LOCK_UN );
					printf( " > [C2] file unlock\n" );

					// file close
					close( fp1 );
					printf( " > [C2] file close\n" );
				}
			}
			
			printf( "[start_downstream::child] child process end!\n" );
			break;
		}

		// parent process
		default	: {
                       
					printf( "[start_downstream::parent] parent process start!\n" );
					printf( "[start_downstream::parent] child process number is %d\n", ftp_pid );

					// network test running check
					if ( processing_flag < 2 ) {		// if network test not in running

						// file open
						fp2 = open( TESTFILE3, (O_WRONLY|O_CREAT|O_TRUNC), ((mode_t)0777) );
						if ( fp2 < 0 ) {				// If can't file open
							printf( "[start_downstream::parent] %s open error!\n", TESTFILE3 );
                          	perror(NULL);  
							test_error=1;
                            return -1;
						}
						else {

							// file lock
							flock( fp2, LOCK_EX );
							printf( " > [P2] file lock\n" );

							// write dummy data
							write( fp2, "0,0,0", strlen("0,0,0") );
							printf( " > [P2] data write\n" );

							// file unlock
							flock( fp2, LOCK_UN );
							printf( " > [P2] file unlock\n" );

							// file close
							close( fp2 );
							printf( " > [P2] file close\n" );
						}
					}

					// set flag with downstream test processing status
					processing_flag |= 0x00000001;
					printf( "[start_downstream::parent] processing_flag change to %d\n", processing_flag );
					printf( "[start_downstream::parent] parent process end!\n" );
                                                                          
					break;
				  }
	}
         
         
	return 0;
}


// -- function of end for downstream test --
int stop_downstream()
{
	int		ret = 0;
	int		pidftp = 0;
	FILE	*fp;
	int		child_pid = 0;
	int		status;

	printf("\nStopping Child process\n");
	
	// check the downstream test in running
	if ( processing_flag & 0x00000001 ) {

		fp = fopen( "/tmp/ftppid.dat", "r" );
		if ( fp == 0 ) {
			printf( "[stop_downstream] ftppid.dat file cannot open\n" );
			perror(NULL);
		}
		else {
			fscanf( fp, "%d", &pidftp );
			system( "rm /tmp/ftppid.dat" );
			fclose( fp );
		}
		
		printf( "[stop_downstream] DOWN=%d, FTP=%d\n", down_pid, pidftp );
		
		// When downstream child process was runnning.
		if ( down_pid != 0 ) {
			ret = kill( down_pid, SIGKILL );
			printf( "[stop_downstream] waiting for child process stop (ret=%d)\n", ret );
			while((child_pid=waitpid( down_pid, &status, WNOHANG )) == 0 ) {
				;
			}
			printf( "[stop_downstream] child process has stop\n" );
		}
		
		// When FTP process was running.
#if 1
		if (pidftp > down_pid) {
			int ret_temp;
			ret_temp = kill(pidftp, SIGKILL);
			if (errno != ESRCH) {
printf("[%s][%s][%d]\n",__FILE__,__FUNCTION__,__LINE__);
				ret += ret_temp;
			}
		}
#else
		if ( pidftp > down_pid ) {
			ret+= kill( pidftp, SIGKILL );
		}
#endif
		printf( "[stop_downstream] return = %d\n", ret );
		if ( ret != 0 ) {
			printf( "[stop_downstream] Failed a child process killing!\n" );
			perror(NULL);
			return -1;
		}
		
		// clear flag with downstream test processing status
		processing_flag &= 0xFFFFFFFE;
		printf( "[stop_downstream] processing_flag change to %d\n", processing_flag );
		printf( "[stop_downstream] child process killed!\n" );
	}
	// if downstream test not in running
	else {
		down_pid = 0;
		return -1;
	}

	sleep( 1 );
	down_pid = 0;
	return 0;
}


// -- function of start for network test --
int start_network()
{
	int  ret = 0;
	char tmp[32];
	
	int fp1, fp2;
	int point[10] = {0,0,0,0,0,0,0,0,0,0};
	int net_max = 0;
	int net_min = 10000000;
	int net_avg = 0;
	int net_avg_div = 1;
	int point_num;
	int i;
	
	point_num = sizeof(point)/sizeof(point[0]);  // 平均値計算配列の数
	memset(point, 0, sizeof(point));             // 平均値計算配列初期化
	
	ret = ReadConfig();
	
	if (processing_flag & 0x00000002) {
		test_error = 2;
		TstRunning = 2;
		return -1;		// if already running myself
	}
	
	net_pid = fork();
	
	switch( net_pid ) {

		// error
		case -1	: {
			printf( "[start_network] child process making error!\n" );
           	perror(NULL);          
            test_error=2;
            return -1;  
			break;
		}

		// child process
		case 0 	: {
			printf( "[start_network::child] child process start!\n" );
			for (;;) {
				// ネットワークディレイの取得 [msec]
				ret = do_ping();
				if ( ret == -1 ) {
					printf( "[start_network::child] system function error!\n" );
				} else {
					// 正常データのみを計算する
					
					// 平均値計算配列へのデータ送り出し
					for (i=0; i<point_num-1; i++) {
						point[i] = point[i+1];
					}
					point[point_num-1] = ret;
					
					// 平均値計算（取得データ数を考慮）
					net_avg = 0;
					for (i=0; i<point_num; i++) {
						net_avg += point[i];
					}
					net_avg /= net_avg_div;
					
					if (net_avg_div<point_num) {
						net_avg_div++;
					}
					
					// compare result and maximun
					net_max = ( net_max < ret ) ? ret : net_max;
					
					// compare result and minimum
					net_min = ( net_min > ret ) ? ret : net_min;
				}
				
				// dummy dispaly
				printf( " > [C3] now=%d,avg=%d,max=%d,min=%d\n", ret, net_avg, net_max, net_min );
				
				// file open
				fp1 = open(TESTFILE3, (O_WRONLY|O_CREAT), ((mode_t)0777));
				if ( fp1 < 0 ) {	// If can't open a file
					printf("[start_network::child]%s open error!\n",TESTFILE3);
					perror(NULL); 
					return -1;
				}
				
				// file lock
				flock( fp1, LOCK_EX );
				printf( " > [C3] file lock\n" );
				
				memset(tmp, 0, sizeof(tmp));
				if (ret == -1) {  // If network test error
//					sprintf(tmp, "%-6d,%-6d,%-6d", 10000, 10000, 10000);
//					sprintf(tmp, "  TOUT,%-6d,%-6d", 0, 0);
					sprintf(tmp, "%-6d,%-6d,%-6d", 0, 0, 10000);
				} else {
					sprintf(tmp, "%-6d,%-6d,%-6d", net_avg, net_min, net_max);
				}
				printf("[%s][%d] [%s] <- [%s]\n",__FUNCTION__,__LINE__, TESTFILE3, tmp);
				write(fp1, tmp, strlen(tmp));
				
				printf( " > [C3] data write\n" );
				
				// file unlock
				flock( fp1, LOCK_UN );
				printf( " > [C3] file unlock\n" );
				
				// file close
				close( fp1 );
				printf( " > [C3] file close\n" );
				
				if (ret == -1) {
					printf( "\nError in network test\n" );
					//exit(0);
					//return -1;
				}
				
				// waiting for selected seconds
				printf( " > [C3] sleep start (%d second)\n", net_interval );
				sleep( net_interval );
			}
			
			printf( "[start_network::child] child process end!\n" );
			break;
		}

		// parent process
		default	: {
			printf( "[start_network::parent] parent process start!\n" );
				printf( "[start_network::parent] child process number is %d\n", ftp_pid );

			// network test running check
  			if ( processing_flag == 0 || processing_flag == 2 ) {	// if downstream test not in running 

				// file open
				fp2 = open( TESTFILE2, (O_WRONLY|O_CREAT|O_TRUNC), ((mode_t)0777) );
				if ( fp2 < 0 ) {
					printf( " [start_network::parent] %s open error\n", TESTFILE2 );
                	perror(NULL);
					test_error = 2;
					return -1;
				} 
				else {							// file lock
					flock( fp2, LOCK_EX );
					printf( " > [P3] file lock\n" );

					// write dummy data
					write( fp2, "0", strlen("0") );
					printf( " > [P3] data write\n" );

					// file unlock
					flock( fp2, LOCK_UN );
					printf( " > [P3] file unlock\n" );

					// file close
					close( fp2 );
					printf( " > [P3] file close\n" );
				}
			}

			// set flag with network test processing status
			processing_flag |= 0x00000002;
			printf( "[start_network::parent] processing_flag change to %d\n", processing_flag );
			printf( "[start_network::parent] parent process end!\n" );
			break;
		}
	}

  	sleep( 1 );  
	return 0;
}

// -- function of stop for network test --
int	stop_network()
{
	int		ret = 0;
	int		child_pid = 0;
	int		status;   

	// check the downstream test in running
	if ( processing_flag & 0x00000002 ) {

		// When downstream child process was runnning.
		
		
		if ( net_pid != 0 ) {		
			ret = kill( net_pid, SIGKILL );
			printf( "[stop_network] waiting for child process stop (ret=%d)\n", ret );
			while((child_pid=waitpid( net_pid, &status, WNOHANG )) == 0 ) {
				;
			}
			printf( "[stop_network] child process has stop\n" );
		}
		
		printf( " > return = %d\n", ret );
		if ( ret != 0 ) {
			printf( "[stop_network] Failed a child process killing!\n" );
            perror(NULL);  
			return -1;
		}

		// clear flag with downstream test processing status
		processing_flag &= 0xFFFFFFFD;
		printf( "[stop_network] processing_flag change to %d\n", processing_flag );
		printf( "[stop_network] child process killed!\n" );
	}
	// if network test not in running
	else {
		return -1;
	}
	
	sleep( 1 );
	
	return 0;
}


// -- body of ping execute --
int	do_ping()
{
	int		ret;
	int		start_time, end_time;
   	char	cmdstr[256];     

	// getting time of start [unit:msec]
	start_time = get_time();

	// for debug
	printf( "[do_ping] start_time=%d\n", start_time );

	// execute ping command
	strcpy( cmdstr, OPT_PING );
	strcat( cmdstr, " " );
	strcat( cmdstr, HOSTPC );
	strcat( cmdstr, " > /dev/null" );
	printf( " > execute command=%s\n", cmdstr );
	ret = system( cmdstr );
	printf( " > return=%d\n", ret );
	if ( ret != 0 ) {
		printf( "[do_ping] system function error!\n" );
		return -1;
	}

	// getting time of end [unit:msec]
	end_time = get_time();

	// for debug
	printf( " > end_time=%d\n", end_time );

	// calculation of network delay time [unit:msec(max:999.999sec)]
	ret = diff_time( start_time, end_time );
	printf( " > diff_time=%d\n", ret );

	return ret;
}


// -- getting time (maximum:999.999sec) --
int	get_time()
{
	int				ret = 0;
	struct timeval	tv;

	ret = gettimeofday(&tv,NULL);
	if (ret != 0) return -1;
	ret = ((tv.tv_sec%1000)*1000)+(tv.tv_usec/1000);
	return ret;
}


// -- getting value of diffrence --
int	diff_time(int start, int end)
{
	int		ret = 0;

	if ( end < start ) {
		ret = (1000000 + end) - start;
	}
	else {
		ret = end - start;
	}
	return ret;
}


// -- interrupt handler --
void *int_handler( int signo )
{
	int		ret = 0;

	printf( "[Interrupt] SIGNO=%d\n", signo );
	ret = kill( down_pid, SIGKILL );
	ret = kill( net_pid, SIGKILL );
	ret = kill( ftp_pid, SIGKILL );
	ret = kill( scan_pid, SIGKILL );

	exit(1);
}


// -- dummy function (for test) --
int	dummy_func( int mode )
{
	int				ret = 0;
	int				i = 0;
	struct timeval	tv;

	switch( mode ) {

		// system function test
		case 0	: {
					ret = system( "ls -al" );
					printf( "[dummy_func] ret=%d\n", ret );
					break;
				  }

		// gettimeofday function test
		case 1	: {
					for( i=0;i<100;i++ ) {
						ret = gettimeofday(&tv,NULL);
						if (ret != 0) return -1;
						printf( "[dummy_func] sec=%d, usec=%d\n", (int)tv.tv_sec, (int)tv.tv_usec );
					}
					break;
				  }

		// default case
		default : {
					break;
				  }
	}

	return 0;
}


void removeLF(char *string)
{{
	int len;
		
	len= strlen(string);
		if(string[len-1] == '\n') string[len-1] = '\0';
}}


int ReadConfig()
{
	FILE *fp;
	
	fp = fopen( "./sokuteicfg", "r" );
	if ( fp == 0 ) {
		printf( "configration file cannot open\n" );
		perror(NULL);
		return -1;
	}
	
	fgets( HOSTPC,     256, fp );
	fgets( RESULTFILE, 256, fp );
	fgets( TESTFILE1,  256, fp );
	fgets( TESTFILE2,  256, fp );
	fgets( TESTFILE3,  256, fp );
	fgets( TEMPFILE,   256, fp );
	fgets( FTPTEMPFILE,256, fp );
	fgets( DOWNFILE,   256, fp );
	fgets( WL_ASSOC,   256, fp );
	fgets( WL_WEPSTAT, 256, fp );
	fgets( WL_RSSI,    256, fp );
	fgets( WL_PKTCNT,  256, fp );
	fgets( IWLIST_CH,  256, fp );
	fgets( WL_SCANRSLT,256, fp );
	fgets( IFCONFIG,   256, fp );
	fgets( WGET,       256, fp );
	fgets( OPT_FTP,    256, fp );
	fgets( OPT_ERRRATE,256, fp );
	fgets( OPT_PING,   256, fp );
	fgets( IFUP,       256, fp );
	fgets( IFDOWN,     256, fp );
	fgets( EXIT_BTN,   256, fp );

	fprintf( fp, "0" );
	fclose( fp );

	removeLF( HOSTPC );
	removeLF( RESULTFILE );
	removeLF( TESTFILE1 );     
	removeLF( TESTFILE2 );
	removeLF( TESTFILE3 );
	removeLF( TEMPFILE );
	removeLF( FTPTEMPFILE );
	removeLF( DOWNFILE );
	removeLF( WL_ASSOC );
	removeLF( WL_WEPSTAT );
	removeLF( WL_RSSI );
	removeLF( WL_PKTCNT );
	removeLF( IWLIST_CH );
	removeLF( WL_SCANRSLT );
	removeLF( IFCONFIG );
	removeLF( WGET );
	removeLF( OPT_FTP );
	removeLF( OPT_ERRRATE );
	removeLF( OPT_PING );
	removeLF( IFUP );
	removeLF( EXIT_BTN );

	printf( "Read configuration file.\n" );
	printf( " > HOSTPC     =%s\n"  , HOSTPC );
	printf( " > TESTFILE1  =%s\n"  , TESTFILE1 );
	printf( " > TESTFILE2  =%s\n"  , TESTFILE2 );
	printf( " > TESTFILE3  =%s\n"  , TESTFILE3 );
	printf( " > TEMPFILE   =%s\n"  , TEMPFILE );
	printf( " > FTPTEMPFILE=%s\n"  , FTPTEMPFILE );
	printf( " > DOWNFILE   =%s\n"  , DOWNFILE );
	printf( " > WL_ASSOC   =%s\n"  , WL_ASSOC );
	printf( " > WL_WEPSTAT =%s\n"  , WL_WEPSTAT );
	printf( " > WL_RSSI    =%s\n"  , WL_RSSI );
	printf( " > WL_PKTCNT  =%s\n"  , WL_PKTCNT );
	printf( " > IWLIST_CH  =%s\n"  , IWLIST_CH );
	printf( " > WL_SCANRSLT=%s\n"  , WL_SCANRSLT );
	printf( " > IFCONFIG   =%s\n"  , IFCONFIG );
	printf( " > WGET       =%s\n"  , WGET );
	printf( " > OPT_FTP    =%s\n"  , OPT_FTP );
	printf( " > OPT_ERRRATE=%s\n"  , OPT_ERRRATE );
	printf( " > OPT_PING   =%s\n"  , OPT_PING );
	printf( " > IFUP       =%s\n"  , IFUP );
	printf( " > EXIT_BTN   =%s\n\n", EXIT_BTN );
	
	return 0;
}


int ReadOneConfig(int n, char *str)
{
	int i;
	FILE *fp;
	char *ret;
	
	fp = fopen(CONFIG_FILE, "r");
	if (fp == 0) {
		printf("configration file cannot open\n");
		perror(NULL);
		return -1;
	}
	
	for (i=0; i<n; i++) {
		ret = fgets(str, 256, fp);
		if (ret == NULL) {
			fclose(fp);
			return -1;
		}
	}
	removeLF(str);
	
	fclose(fp);
	
	return 0;
}


int IsGetIP()
{
	FILE *fp;
	char buf[256];
	char command[256];
	int num;
	char *p;
	char str[256];
	
	ReadOneConfig(15, command);  // ifconfigコマンド取得
	sprintf(str, "LANG=C %s", command);
	printf("%s\n", str);
	fp = popen(str, "r");
	if (fp == NULL) {
		return 0;
	}
	
	memset(buf, 0, sizeof(buf));
	num = fread(buf, sizeof(buf[0]), sizeof(buf), fp);
	
	p = strstr(buf, "inet addr:");
	if (p == NULL) {
		pclose(fp);
		return 0;
	}
	
	p = strstr(buf, "UP ");
	if (p == NULL) {
		pclose(fp);
		return 0;
	}
	
	pclose(fp);
	
	return 1;
}

