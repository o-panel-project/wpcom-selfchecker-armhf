/*
 * sample_polling.c -  FeliCa Polling sample.
 *
 * Copyright 2006,2007,2008 Sony Corporation
 */

#include <stdio.h>  /* printf, fprintf */
#include <stdlib.h> /* strtoul */
#include <felica_cc.h>
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
#ifndef DEFAULT_SYSTEM_CODE
#define DEFAULT_SYSTEM_CODE 0xffff
#endif
#ifndef DEFAULT_POLLING_MAX_RETRY_TIMES
#define DEFAULT_POLLING_MAX_RETRY_TIMES 9
#endif
#ifndef DEFAULT_POLLING_INTERVAL
#define DEFAULT_POLLING_INTERVAL 500 /* ms */
#endif
#ifndef DEFAULT_POLLING_OPTION
#define DEFAULT_POLLING_OPTION 0
#endif
#ifndef DEFAULT_POLLING_TIMESLOT
#define DEFAULT_POLLING_TIMESLOT 0
#endif

/* These functions are defined in another file. */
extern const icsdrv_basic_func_t* g_drv_func;
extern UINT32 (*g_felica_cc_stub_initialize_func)(
    felica_cc_devf_t* devf,
    ICS_HW_DEVICE* dev);

static const char* s_port_name = DEFAULT_PORT_NAME;
static UINT32 s_speed = DEFAULT_SPEED;
static UINT32 s_timeout = DEFAULT_TIMEOUT;
static UINT16 s_system_code = DEFAULT_SYSTEM_CODE;
static UINT32 s_polling_max_retry_times = DEFAULT_POLLING_MAX_RETRY_TIMES;
static UINT32 s_polling_interval = DEFAULT_POLLING_INTERVAL;
static UINT8 s_polling_option = DEFAULT_POLLING_OPTION;
static UINT8 s_polling_timeslot = DEFAULT_POLLING_TIMESLOT;

static void print_usage(const char* argv0)
{
    fprintf(stderr, "Usage: %s [options]\n", argv0);
    fprintf(stderr, "  -p port\n");
    fprintf(stderr, "  -b UART speed (bps)\n");
    fprintf(stderr, "  -t timeout for UART/USB communication (ms)\n");
    fprintf(stderr, "  -s system code (default: ffff)\n");
    fprintf(stderr, "  -r polling max retry times (default: 9)\n");
    fprintf(stderr, "  -I polling interval (ms)\n");
    fprintf(stderr, "  -o polling option (0, 1, 2)\n");

    return;
}

static int initialize(
    ICS_HW_DEVICE* dev,
    felica_cc_devf_t* devf)
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

    printf("  calling felica_cc_stub_initialize() ...\n");
    rc = (*g_felica_cc_stub_initialize_func)(devf, dev);
    if (rc != ICS_ERROR_SUCCESS) {
        fprintf(stderr, "    failure in felica_cc_stub_initialize():%u\n", rc);
        return -1;
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
    felica_cc_devf_t devf;
    felica_card_t card;
    felica_card_option_t card_option;

    printf("FeliCa Polling sample.\n");

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
        } else if ((utl_strcmp(argv[i], "-s") == 0) && ((i + 1) < argc)) {
            i++;
            s_system_code = (UINT16)strtoul(argv[i], NULL, 16);
        } else if ((utl_strcmp(argv[i], "-r") == 0) && ((i + 1) < argc)) {
            i++;
            s_polling_max_retry_times = strtoul(argv[i], NULL, 0);
        } else if ((utl_strcmp(argv[i], "-I") == 0) && ((i + 1) < argc)) {
            i++;
            s_polling_interval = strtoul(argv[i], NULL, 0);
        } else if ((utl_strcmp(argv[i], "-o") == 0) && ((i + 1) < argc)) {
            i++;
            s_polling_option = (UINT8)strtoul(argv[i], NULL, 0);
        } else {
            print_usage(argv[0]);
            return -1;
        }
    }

    res = initialize(&dev, &devf);
    if (res != 0) {
        return -1;
    }

    /*
     * Polling
     */
    {
        UINT8 polling_param[4];

        polling_param[0] = (UINT8)((s_system_code >> 8) & 0xff);
        polling_param[1] = (UINT8)((s_system_code >> 0) & 0xff);
        polling_param[2] = s_polling_option;
        polling_param[3] = s_polling_timeslot;

        printf("start Polling ...\n");

        for (nretries = 0; nretries <= s_polling_max_retry_times; nretries++) {
            printf("  calling felica_cc_polling() ...\n");
            rc = felica_cc_polling(&devf,
                                   polling_param,
                                   &card,
                                   &card_option,
                                   s_timeout);
            if (rc != ICS_ERROR_TIMEOUT) {
                break;
            }
            fprintf(stderr, "    felica_cc_polling(): polling timeout.\n");
            utl_msleep(s_polling_interval);
        }
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in felica_cc_polling():%u\n", rc);
            finalize(&dev);
            return -1;
        }

        printf("    IDm: %02x%02x%02x%02x%02x%02x%02x%02x\n",
               card.idm[0], card.idm[1], card.idm[2], card.idm[3],
               card.idm[4], card.idm[5], card.idm[6], card.idm[7]);
        printf("    PMm: %02x%02x%02x%02x%02x%02x%02x%02x\n",
               card.pmm[0], card.pmm[1], card.pmm[2], card.pmm[3],
               card.pmm[4], card.pmm[5], card.pmm[6], card.pmm[7]);
        printf("    Option: ");
        for (i = 0; i < (int)card_option.option_len; i++) {
            printf("%02x", card_option.option[i]);
        }
        printf("\n");
    }

    finalize(&dev);

    return 0;
}
