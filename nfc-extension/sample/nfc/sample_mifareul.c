/*
 * sample_mifareul.c - Sample code reading MIFARE UltraLight.
 *
 * Copyright 2009 Sony Corporation
 */
#include <stdio.h>  /* printf, fprintf */
#include <stdlib.h> /* strtoul */
#include <rcs956_typea.h>
#include <utl.h>
#include <icsdrv.h>
#include <ics_error.h>

#ifndef DEFAULT_PORT_NAME
#define DEFAULT_PORT_NAME "/dev/ttyS0" /* may be overwritten by Makefile */
#endif
#ifndef DEFAULT_SPEED
#define DEFAULT_SPEED 115200
#endif
#ifndef DEFAULT_TIMEOUT
#define DEFAULT_TIMEOUT 400 /* ms */
#endif
#ifndef DEFAULT_COMMAND_MAX_RETRY_TIMES
#define DEFAULT_COMMAND_MAX_RETRY_TIMES 2
#endif
#ifndef DEFAULT_POLLING_MAX_RETRY_TIMES
#define DEFAULT_POLLING_MAX_RETRY_TIMES 9
#endif
#ifndef DEFAULT_POLLING_INTERVAL
#define DEFAULT_POLLING_INTERVAL 500 /* ms */
#endif
#ifndef DEFAULT_READ_ADDR
#define DEFAULT_READ_ADDR 0
#endif

/* These functions are defined in another file. */
extern const icsdrv_basic_func_t* g_drv_func;

static const char* s_port_name = DEFAULT_PORT_NAME;
static UINT32 s_speed = DEFAULT_SPEED;
static UINT32 s_timeout = DEFAULT_TIMEOUT;
static UINT32 s_command_max_retry_times = DEFAULT_COMMAND_MAX_RETRY_TIMES;
static UINT32 s_polling_max_retry_times = DEFAULT_POLLING_MAX_RETRY_TIMES;
static UINT32 s_polling_interval = DEFAULT_POLLING_INTERVAL;
static UINT8 s_read_addr = DEFAULT_READ_ADDR;

static void print_usage(const char* argv0)
{
    fprintf(stderr, "Usage: %s [options]\n", argv0);
    fprintf(stderr, "  -p port\n");
    fprintf(stderr, "  -b UART speed (bps)\n");
    fprintf(stderr, "  -t timeout (ms)\n");
    fprintf(stderr, "  -R max retry times of command (default: 2)\n");
    fprintf(stderr, "  -a read address (0-15) (default: 0)\n");

    return;
}

static int initialize(
    ICS_HW_DEVICE* dev)
{
    UINT32 rc;

    printf("start initialization ...\n");

    printf("  calling open(%s) ...\n", s_port_name);
    rc = g_drv_func->open(dev, s_port_name);
    if (rc != ICS_ERROR_SUCCESS) {
        fprintf(stderr, "    failure in open():%u\n", rc);
        return -1;
    }

    if (g_drv_func->set_speed != NULL) {
        printf("  calling set_speed(%u) ...\n", s_speed);
        rc = g_drv_func->set_speed(dev, s_speed);
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in set_speed():%u\n", rc);
            return -1;
        }
    }

    if (g_drv_func->initialize_device != NULL) {
        printf("  calling initialize_device() ...\n");
        rc = g_drv_func->initialize_device(dev, s_timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in initialize_device():%u\n", rc);
            return -1;
        }
    }

    return 0;
}

static int finalize(
    ICS_HW_DEVICE* dev)
{
    UINT32 rc;

    printf("start finalization ...\n");

    if (g_drv_func->cancel_command != NULL) {
        printf("  calling cancel_command() ...\n");
        rc = g_drv_func->cancel_command(dev);
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in cancel_command():%u\n", rc);
            /* Note: continue */
        }
    }

    if (g_drv_func->reset != NULL) {
        printf("  calling reset() ...\n");
        rc = g_drv_func->reset(dev, s_timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in reset():%u\n", rc);
            /* Note: continue */
        }
    }

    if (g_drv_func->rf_off != NULL) {
        printf("  calling rf_off() ...\n");
        rc = g_drv_func->rf_off(dev, s_timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in rf_off():%u\n", rc);
            /* Note: continue */
        }
    }

    printf("  calling close() ...\n");
    rc = g_drv_func->close(dev);
    if (rc != ICS_ERROR_SUCCESS) {
        fprintf(stderr, "    failure in close():%u\n", rc);
        return -1;
    }

    return 0;
}

int main(
    int argc,
    char* argv[])
{
    int res;
    UINT32 rc;
    int i;
    UINT32 nretries;
    ICS_HW_DEVICE dev;
    typea_card_t card;

    printf("MIFARE UltraLight read sample.\n");

    /* parse options */
    for (i = 1; i < argc; i++) {
        if ((utl_strcmp(argv[i], "-p") == 0) && ((i + 1) < argc)) {
            i++;
            s_port_name = argv[i];
        } else if ((utl_strcmp(argv[i], "-b") == 0) && ((i + 1) < argc)) {
            i++;
            s_speed = strtoul(argv[i], NULL, 0);
        } else if ((utl_strcmp(argv[i], "-t") == 0) && ((i + 1) < argc)) {
            i++;
            s_timeout = strtoul(argv[i], NULL, 0);
        } else if ((utl_strcmp(argv[i], "-R") == 0) && ((i + 1) < argc)) {
            i++;
            s_command_max_retry_times = strtoul(argv[i], NULL, 0);
        } else if ((utl_strcmp(argv[i], "-a") == 0) && ((i + 1) < argc)) {
            i++;
            s_read_addr = (UINT8)strtoul(argv[i], NULL, 0);
        } else {
            print_usage(argv[0]);
            return -1;
        }
    }
    if (s_read_addr >= 16) {
        print_usage(argv[0]);
        return -1;
    }

    res = initialize(&dev);
    if (res != 0) {
        return -1;
    }

    /*
     * Polling
     */
    {
        printf("start Type A Polling ...\n");

        for (nretries = 0; nretries <= s_polling_max_retry_times; nretries++) {
            printf("  calling rcs956_typea_polling() ...\n");
            rc = rcs956_typea_polling(&dev,
                                      NULL, 0,
                                      &card, NULL,
                                      s_timeout);
            if (rc != ICS_ERROR_TIMEOUT) {
                break;
            }
            fprintf(stderr, "    rcs956_typea_polling(): timeout.\n");
            utl_msleep(s_polling_interval);
        }
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in rcs956_typea_polling():%u\n", rc);
            finalize(&dev);
            return -1;
        }

        printf("    ATQA: %02x%02x\n", card.atqa[0], card.atqa[1]);
        printf("    SAK: %02x\n", card.sak);
        printf("    UID: ");
        for (i = 0; i < (int)card.uid_len; i++) {
            printf("%02x", card.uid[i]);
        }
        printf("\n");
    }

    /*
     * Read
     */
    {
        UINT8 command[2];
        UINT8 response[16];
        UINT32 response_len;

        command[0] = 0x30;
        command[1] = s_read_addr;

        printf("start Read ...\n");

        for (nretries = 0; nretries <= s_command_max_retry_times; nretries++) {
            printf("  calling rcs956_typea_command() ...\n");
            rc = rcs956_typea_command(&dev, &card,
                                      command, 2,
                                      sizeof(response),
                                      response, &response_len,
                                      s_timeout, s_timeout);
            if ((rc != ICS_ERROR_TIMEOUT) &&
                (rc != ICS_ERROR_FRAME_CRC)) {
                break;
            }
        }
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr,
                    "    failure in rcs956_typea_command():%u\n", rc);
            finalize(&dev);
            return -1;
        }

        printf("    Read data: ");
        for (i = 0; i < (int)response_len; i++) {
            printf("%02x", response[i]);
        }
        printf("\n");
    }

    finalize(&dev);

    return 0;
}
