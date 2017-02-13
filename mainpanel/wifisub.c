/*
 *		WiFi Sub 
 *
 *		2014.7	written by WPC Inc.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include "common.h"

//
// check if wireless
//
static int check_wireless(const char* ifname, char* protocol)
{
	int sock = -1;
	struct iwreq pwrq;
	memset(&pwrq, 0, sizeof(pwrq));
	strncpy(pwrq.ifr_name, ifname, IFNAMSIZ-1);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 0;
	}

	if (ioctl(sock, SIOCGIWNAME, &pwrq) != -1) {
		if (protocol) strncpy(protocol, pwrq.u.name, IFNAMSIZ-1);
		close(sock);
		return 1;
	}

	close(sock);
	return 0;
}

//
//	wifi interface checker
//
void check_wlan()
{
	struct ifaddrs *ifaddr, *ifa;

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		debug_printf(3, "ERROR: Could not find wlan device.\n");
		return;
	}

	/* Walk through linked list, maintaining head pointer so we
	   can free list later */
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		char protocol[IFNAMSIZ] = {0};
		if (ifa->ifa_addr == NULL ||
			ifa->ifa_addr->sa_family != AF_PACKET) continue;
		if (check_wireless(ifa->ifa_name, protocol)) {
			setenv("SC_WLAN_IFACE", ifa->ifa_name, 1);
			debug_printf(3, "Found wifi interface %s(%s). "
					"(set to SC_WLAN_IFACE)\n",
					getenv("SC_WLAN_IFACE"), protocol);
			freeifaddrs(ifaddr);
			return;
		}
	}
	freeifaddrs(ifaddr);
	debug_printf(3, "ERROR: Could not find wlan device.\n");
}
