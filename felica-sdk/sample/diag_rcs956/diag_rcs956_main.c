/**
 * \brief    Self-Diagnosing Sample Main
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diag_rcs956.h"

#ifndef DEFAULT_PORT_NAME
#define DEFAULT_PORT_NAME "/dev/ttyS0" /* may be overwritten by Makefile */
#endif

static void print_usage(
    const char* argv0);

static const char* s_port_name = DEFAULT_PORT_NAME;
static UINT32 s_timeout = 1000;
static const UINT16 s_versions[] = {0x0130};

int main(
    int argc,
    char* argv[])
{
    ICS_HW_DEVICE dev;
    int rc;
    int i;
    UINT32 num_ver;

    /* parse options */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            i++;
            s_port_name = argv[i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            i++;
            s_timeout = strtoul(argv[i], NULL, 0);
        } else if (argv[i][0] == '-') {
            print_usage(argv[0]);
            return -1;
        } else {
            break;
        }
    }

    rc = diag_rcs956_init_device(&dev, s_port_name, s_timeout);
    if (rc != DIAG_SUCCESS) {
        printf("Error: Initialize Device(%d)\n", rc);
        return rc;
    }

    num_ver = (sizeof(s_versions) / sizeof(s_versions[0]));

    rc = diag_rcs956_check_firmware_version(&dev, s_versions,
                                            num_ver,
                                            s_timeout);
    if (rc != DIAG_SUCCESS) {
        printf("Error: Check Firmware Version(%d)\n", rc);
        return rc;
    }

    rc = diag_rcs956_communication_line_test(&dev, s_timeout);
    if (rc != DIAG_SUCCESS) {
        printf("Error: Communication Line Test(%d)\n", rc);
        return rc;
    }

    rc = diag_rcs956_rom_test(&dev, s_timeout);
    if (rc != DIAG_SUCCESS) {
        printf("Error: ROM Test(%d)\n", rc);
        return rc;
    }

    rc = diag_rcs956_ram_test(&dev, s_timeout);
    if (rc != DIAG_SUCCESS) {
        printf("Error: RAM Test(%d)\n", rc);
        return rc;
    }

    rc = diag_rcs956_eeprom_test(&dev, s_timeout);
    if (rc != DIAG_SUCCESS) {
        printf("Error: EEPROM Test(%d)\n", rc);
        return rc;
    }

    rc = diag_rcs956_polling_test_to_card(&dev, s_timeout);
    if (rc != DIAG_SUCCESS) {
        printf("Error: Polling Test to Card(%d)\n", rc);
        return rc;
    }

    printf("Success\n");

    return 0;
}

static void print_usage(
    const char* argv0)
{
    fprintf(stderr, "Usage: %s [options] testcases...\n", argv0);
    fprintf(stderr, "  -p port\n");
    fprintf(stderr, "  -t timeout\n");

    return;
}
