/*
 * sample_powerdown.c - Sample code of powerdown.
 *
 *   This program uses the following RC-S956 command.
 *     - PowerDown
 *
 * Copyright 2006,2007,2008 Sony Corporation
 */
#include <stdio.h>  /* printf, fprintf */
#include <stdlib.h> /* strtoul */
#include <utl.h>
#include <icsdrv.h>
#include <ics_error.h>
#include <rcs956.h>

#ifndef DEFAULT_PORT_NAME
#define DEFAULT_PORT_NAME "/dev/ttyS0" /* may be overwritten by Makefile */
#endif
#ifndef DEFAULT_SPEED
#define DEFAULT_SPEED 115200
#endif
#ifndef DEFAULT_TIMEOUT
#define DEFAULT_TIMEOUT 400 /* ms */
#endif
#ifndef DEFAULT_POWERDOWN_PERIOD
#define DEFAULT_POWERDOWN_PERIOD 5000 /* ms */
#endif

/* This function is defined in another file. */
extern const icsdrv_basic_func_t* g_drv_func;

static const char* s_port_name = DEFAULT_PORT_NAME;
static UINT32 s_speed = DEFAULT_SPEED;
static UINT32 s_timeout = DEFAULT_TIMEOUT;
static UINT32 s_powerdown_period = DEFAULT_POWERDOWN_PERIOD;

static void print_usage(const char* argv0)
{
    fprintf(stderr, "Usage: %s [options]\n", argv0);
    fprintf(stderr, "  -p port\n");
    fprintf(stderr, "  -b UART speed (bps)\n");
    fprintf(stderr, "  -t timeout for UART/USB communication (ms)\n");
    fprintf(stderr, "  -w powerdown period (ms)\n");
    fprintf(stderr, "  -rf (wait for external RF)\n");

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
    ICS_HW_DEVICE dev;
    int wait_rf;
    int rf_detected;

    printf("RC-S956 PowerDown sample.\n");

    wait_rf = 0;

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
        } else if ((utl_strcmp(argv[i], "-w") == 0) && ((i + 1) < argc)) {
            i++;
            s_powerdown_period = strtoul(argv[i], NULL, 0);
        } else if (utl_strcmp(argv[i], "-rf") == 0) {
            wait_rf = 1;
        } else {
            print_usage(argv[0]);
            return -1;
        }
    }

    res = initialize(&dev);
    if (res != 0) {
        return -1;
    }

    /*
     * PowerDown
     */
    {
        UINT8 wake_up;

        printf("start PowerDown ...\n");

        if (wait_rf) {
            wake_up = (RCS956_WAKEUP_RF_DETECT | RCS956_WAKEUP_UART);
        } else {
            wake_up = RCS956_WAKEUP_UART;
        }

        printf("  calling rcs956_power_down() ...\n");
        rc = rcs956_power_down(&dev, wake_up, s_timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in rcs956_power_down():%u\n", rc);
            finalize(&dev);
            return -1;
        }
    }

    rf_detected = 0;
    if (wait_rf) {
        printf("waiting for external RF (%u msec) ...\n", s_powerdown_period);

        printf("  calling rcs956_wait_for_wake_up() ...\n");
        rc = rcs956_wait_for_wake_up(&dev, s_powerdown_period);
        if (rc == ICS_ERROR_SUCCESS) {
            printf("  detected external RF\n");
            rf_detected = 1;
        } else if (rc == ICS_ERROR_TIMEOUT) {
            printf("  timeout\n");
        } else {
            fprintf(stderr,
                    "    failure in rcs956_wait_for_wake_up():%u\n", rc);
            finalize(&dev);
            return -1;
        }
    } else {
        printf("sleep %u msec ...\n", s_powerdown_period);

        utl_msleep(s_powerdown_period);
    }

    /*
     * resume
     */
    if (!rf_detected) {
        fprintf(stderr, "wake RC-S956 up ...\n");

        printf("  calling rcs956_wake_up() ...\n");
        rc = rcs956_wake_up(&dev, s_timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr, "    failure in rcs956_wake_up():%u\n", rc);
            finalize(&dev);
            return -1;

        }
    }

    printf("Resumed.\n");

    finalize(&dev);

    return 0;
}
