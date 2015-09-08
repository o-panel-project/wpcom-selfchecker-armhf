/*
 * $Id: rcs956scan.c 82 2011-11-17 08:56:46Z r-hara $
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>

#include "common.h"

#include "ics_types.h"
#include "utl.h"

#define	RCS956_VENDOR_ID	(0x054c)
#define	RCS956_PRODUCT_ID	(0x02e1)

#define LOGERR(args...)     debug_printf(5,args)
#define LOGWARNING(args...) debug_printf(5,args)
#define LOGNOTICE(args...)  debug_printf(5,args)
#define LOGINFO(args...)    debug_printf(5,args)
#define LOGDEBUG(args...)   debug_printf(5,args)

static int m_iIsJ4 = 0;
static int nfind;
static char *pfind[5];	/* max 5 device */
static int compare_str(const void *a, const void *b)
{
	return utl_strcmp(*(char**)a, *(char**)b);
}

/*
 * Coverts CPU-endian byte order to UINT16
 */
static UINT16 rcs956_cpub_to_uint16(const UINT8 x[2])
{
	UINT16 out;
	utl_memcpy(&out, (const void*)x, 2);
	return out;
}

/*
 * Search Vendor/Prod in /proc/bus/usb/BBB/DDD
 */
static int find_rcs956(const char *fpath, const struct stat *sb, int typeflags)
{
	int fd, res;
	UINT8 desc[18];	/* usb_device_descriptor */
	char *rcs956_proc_path;

	if (m_iIsJ4) {
		if (typeflags != S_IFCHR)
			return 0;
	} else {
		if (typeflags != S_IFREG)
			return 0;
	}

	/* ignore /proc/bus/usb/{devices,drivers} */
	if (strrchr(fpath, '/')[1] == 'd')
		return 0;
	fd = open(fpath, O_RDWR);
	if (fd == -1)
		return 0;
	LOGINFO("usb device descriptor path = %s\r\n", fpath);
	/* read descriptors */
	res = read(fd, desc, sizeof(desc));
	close(fd);
	if (res != sizeof(desc)) {
		return 0;
	}
	/* check the device descriptor */
	if ((rcs956_cpub_to_uint16(desc + 8) == RCS956_VENDOR_ID) &&
		(rcs956_cpub_to_uint16(desc + 10) == RCS956_PRODUCT_ID)) {
		rcs956_proc_path = strdup(fpath);
		return (int)rcs956_proc_path;
	}
	return 0;
}

static int scan_dir(const char *dirpath)
{
	struct dirent *pdent;
	struct stat st;
	DIR *dir;
	char buff[NAME_MAX + 1];
	int ret;

	dir = opendir(dirpath);
	if (dir == NULL)
		return -1;

	while ((pdent = readdir(dir))) {
		if ((pdent->d_name[0] == '.') ||	/* ignore '.','..' */
			(pdent->d_name[0] == 'd'))		/* ignore 'devices','drivers' */
			continue;
		sprintf(buff, "%s/%s", dirpath, pdent->d_name);
		if (lstat(buff, &st) == -1) {
			continue;
		}
		if (S_ISDIR(st.st_mode))
			scan_dir(buff);
		else {
			if (m_iIsJ4) {
				if (S_ISCHR(st.st_mode)) {
					ret = find_rcs956(buff, &st, S_IFCHR);
					if (ret != 0) {
						/* found */
						if (nfind < 5) {
							pfind[nfind] = (char *)ret;
							nfind++;
						}
					}
				}
			} else {
				if (S_ISREG(st.st_mode)) {
					ret = find_rcs956(buff, &st, S_IFREG);
					if (ret != 0) {
						/* found */
						if (nfind < 5) {
							pfind[nfind] = (char *)ret;
							nfind++;
						}
					}
				}
			}
		}
	}
	closedir(dir);
	return 0;
}

/*
 * Search rcs956
 * return port_name used by rcs956_usb_open()
 */
char *scan_rcs956()
{
	int ret, cnt, i;
	struct timespec ts;
	char *str;

	ts.tv_sec	= 0;
	ts.tv_nsec	= 200000000;

	m_iIsJ4 = sc_IsJ4();

	for (cnt = 1; cnt <= 15; cnt++ ) {	// 3sec
		LOGWARNING("%s():rcs956 scan start. cnt=%d\r\n", __func__, cnt);
		nfind = 0;
		if (m_iIsJ4) {
		LOGWARNING("%s():Scanning \"/dev/bus/usb\"\n", __func__);
		ret = scan_dir("/dev/bus/usb");
		} else {
		LOGWARNING("%s():Scanning \"/proc/bus/usb\"\n", __func__);
		ret = scan_dir("/proc/bus/usb");
		}
		if (ret == -1) {
			LOGERR("%s():ftw() error.\r\n", __func__);
			return NULL;
		}
		if (nfind == 0) {
			nanosleep(&ts, NULL);
			continue;
		}
		if (nfind > 1)
			qsort(pfind, nfind, sizeof(char*), compare_str);
		str = strdup(pfind[0]);
		for (i = 0; i < nfind; i++) {
			LOGNOTICE("%s():rcs956 found (%s).\r\n", __func__, pfind[i]);
			free(pfind[i]);
		}
		LOGNOTICE("%s():rcs956 (%s) selected.\r\n", __func__, str);
		return str;	/* must do free() */
	}
	LOGWARNING("%s():rcs956 not found. cnt=%d\r\n", __func__, cnt);
	return NULL;
}

#if 0
/*
 * test main
 * Reference Implementation Basic Suite for RC-S956 API 仕様書
 * 9.2.3 参照
 */
#include "ics_error.h"
#include "felica_cc.h"
#include "stub/felica_cc_stub_rcs956.h"
int main(int argc, char *argv[])
{
	ICS_HW_DEVICE rcs956;
	UINT32 ret;
	felica_cc_devf_t devf;
	felica_card_t card;
	felica_card_option_t card_option;
	UINT8 polling_param[4];

	char *port_name = scan_rcs956();
	if (!port_name) {
		return 0;
	}

	ret = rcs956_usb_open(&rcs956, port_name);
	if (ret == ICS_ERROR_SUCCESS) {
		fprintf(stdout, "rcs956 port name = %s\r\n", port_name);
	} else {
		fprintf(stderr, "rcs956 port open error %d\r\n", ret);
		free(port_name);
		return 0;
	}
	free(port_name);
	ret = rcs956_initialize_device(&rcs956, 400);
	ret = felica_cc_stub_rcs956_initialize(&devf, &rcs956);
	polling_param[0] = (UINT8)(0xff);
	polling_param[1] = (UINT8)(0xff);
	polling_param[2] = 0;
	polling_param[3] = 0;
	ret = felica_cc_polling(&devf, polling_param, &card, &card_option, 400);
	if (ret == ICS_ERROR_SUCCESS) {
		int idx;
		fprintf(stdout, "IDm:");
		for (idx = 0; idx < 8; idx++)
			fprintf(stdout, "%02x", card.idm[idx] & 0xff);
		fprintf(stdout, "\r\n");
	}
	ret = rcs956_reset(&rcs956, 400);
	ret = rcs956_rf_off(&rcs956, 400);
	ret = rcs956_close(&rcs956);

	return 0;
}
#endif
