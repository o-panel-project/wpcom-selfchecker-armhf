/*
 * Copyright 2006,2007,2008 Sony Corporation
 */

#include <stdlib.h>
#include <string.h>
#include <utl.h>
#include <icsdrv.h>
#include <ics_error.h>

#ifndef TESTPR
#include <stdio.h>
#define TESTPR  printf
#endif
#ifndef TESTPR_FLUSH
#include <stdio.h>
#define TESTPR_FLUSH fflush(stdout)
#endif

#ifndef DEFAULT_PORT_NAME
#define DEFAULT_PORT_NAME "/dev/ttyS0"
#endif

#define DEFAULT_TIMEOUT                 100
#define DEFAULT_SPEED                   115200
#define DEFAULT_LOOP_TIMES              0 /* infinite */
#define DEFAULT_MAX_POLLING_TIMES       0 /* infinite */

#define N_POLLINGS_PER_CYCLE  50
#define N_CYCLES_PER_LOOP     20

#define POLLING_TIMEOUT_AT_DEVICE       10

#define MY_VERSION      "Polling for RC-S956 --- ver 1.0"
#define MY_COPYRIGHT    "Copyright 2006,2007 Sony Corporation"

extern const icsdrv_basic_func_t* g_drv_func;

static const char* s_port_name = DEFAULT_PORT_NAME;
static UINT32 s_timeout = DEFAULT_TIMEOUT;

static UINT32 s_speed = DEFAULT_SPEED;
static UINT32 s_loop_times = DEFAULT_LOOP_TIMES;
static UINT32 s_max_polling_times = DEFAULT_MAX_POLLING_TIMES;

static int polling_rcs956(void)
{
    UINT32 rc;
    ICS_HW_DEVICE rcs956;
    UINT32 n_pollings_in_cycle;
    UINT32 n_pollings_in_loop;
    UINT32 n_pollings_total;
    UINT32 n_successes_in_cycle;
    UINT32 n_successes_in_loop;
    UINT32 n_cycles;
    UINT32 n_loops;
    const UINT8 in_list_command[9] = {
        0xd4, 0x4a, 0x01, 0x01, 0x00, 0xff, 0xff, 0x00, 0x00,
    };
    UINT8 in_list_response[22];
    UINT32 response_len;
    UINT32 success_rate;

    TESTPR("\n<< %s %s >> \n\n", MY_VERSION, MY_COPYRIGHT);
    TESTPR_FLUSH;

    rc = g_drv_func->open(&rcs956, s_port_name);
    if (rc != ICS_ERROR_SUCCESS) {
        TESTPR("Cannot open the device(%s) (Error=%u)\n", s_port_name, rc);
        TESTPR_FLUSH;
        return -1;
    }

    if (g_drv_func->set_speed != NULL) {
        rc = g_drv_func->set_speed(&rcs956, s_speed);
        if (rc != ICS_ERROR_SUCCESS) {
            TESTPR("Cannot set speed(%u) (Error=%u)\n", s_speed, rc);
            TESTPR_FLUSH;
            return -1;
        }
    }

    if (g_drv_func->initialize_device != NULL) {
        rc = g_drv_func->initialize_device(&rcs956, s_timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            TESTPR("Cannot initialize the device(%s) (Error=%u)\n",
                   s_port_name, rc);
            TESTPR_FLUSH;
            return -1;
        }
    }

    n_pollings_total = 0;

    for (n_loops = 0;
         (s_loop_times == 0) || (n_loops < s_loop_times);
         n_loops++) {
        if ((s_max_polling_times > 0) &&
            (n_pollings_total >= s_max_polling_times)) {
            break;
        }

        n_pollings_in_loop = 0;
        n_successes_in_loop = 0;

        for (n_cycles = 0; n_cycles < N_CYCLES_PER_LOOP; n_cycles++) {
            if ((s_max_polling_times > 0) &&
                (n_pollings_total >= s_max_polling_times)) {
                break;
            }

            n_successes_in_cycle = 0;

            for (n_pollings_in_cycle = 0;
                 n_pollings_in_cycle < N_POLLINGS_PER_CYCLE;
                 n_pollings_in_cycle++) {
                if ((s_max_polling_times > 0) &&
                    (n_pollings_total >= s_max_polling_times)) {
                    break;
                }

                /* InListPassiveTarget */
                rc = g_drv_func->execute_command(&rcs956,
                                                 in_list_command,
                                                 sizeof(in_list_command),
                                                 sizeof(in_list_response),
                                                 in_list_response,
                                                 &response_len,
                                                 s_timeout);
                if (rc == ICS_ERROR_SUCCESS) {
                    if (in_list_response[2] == 0) {
                        TESTPR(".");
                        TESTPR_FLUSH;
                    } else {
                        TESTPR("o");
                        TESTPR_FLUSH;
                        n_successes_in_cycle++;
                    }
                } else {
                    if (rc == ICS_ERROR_TIMEOUT) {
                        TESTPR(".");
                        TESTPR_FLUSH;
                    } else {
                        TESTPR("X");
                        TESTPR_FLUSH;
                    }
                }
                n_pollings_total++;
            }

            TESTPR("    [%3u/%3u] %3u%%\n",
                   n_successes_in_cycle,
                   n_pollings_in_cycle,
                   (n_successes_in_cycle * 100) / n_pollings_in_cycle);
            TESTPR_FLUSH;

            n_pollings_in_loop += n_pollings_in_cycle;
            n_successes_in_loop += n_successes_in_cycle;
        }

        success_rate = ((n_successes_in_loop * 10000) / n_pollings_in_loop);
        TESTPR("(TOTAL) [%4u/%4u] ... %3u.%02u%%\n\n",
               n_successes_in_loop,
               n_pollings_in_loop,
               success_rate / 100,
               success_rate % 100);
        TESTPR_FLUSH;
    }

    if (g_drv_func->rf_off != NULL) {
        rc = g_drv_func->rf_off(&rcs956, s_timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            TESTPR("Cannot turn off RF power (Error=%u)\n", rc);
            TESTPR_FLUSH;
            return -1;
        }
    }

    if (g_drv_func->close != NULL) {
        rc = g_drv_func->close(&rcs956);
        if (rc != ICS_ERROR_SUCCESS) {
            TESTPR("Cannot close the device (Error=%u)\n", rc);
            TESTPR_FLUSH;
            return -1;
        }
    }

    return 0;
}


static void print_usage(
    const char* argv0)
{
    TESTPR("Usage: %s [options]\n", argv0);
    TESTPR("  -p port\n");
    TESTPR("   c port\n");
    TESTPR("  -b UART speed\n");
    TESTPR("   b UART speed\n");
    TESTPR("  -t timeout at controller\n");
    TESTPR("   t timeout at controller\n");
    TESTPR("  -m max polling times (0=infinite)\n");
    TESTPR("   m max polling times\n");
    TESTPR("  -l loop times (0=infinite)\n");
    TESTPR("   l loop times\n");

    return;
}

int main(
    int argc,
    char* argv[])
{
    int res;
    int i;

    /* parse options */
    for (i = 1; i < argc; i++) {
        if (((strcmp(argv[i], "-p") == 0) ||
             (strcmp(argv[i], "c") == 0)) && ((i + 1) < argc)) {
            i++;
            s_port_name = argv[i];
        } else if (((strcmp(argv[i], "-t") == 0) ||
                    (strcmp(argv[i], "t") == 0)) && ((i + 1) < argc)) {
            i++;
            s_timeout = strtoul(argv[i], NULL, 0);
        } else if (((strcmp(argv[i], "-b") == 0) ||
                    (strcmp(argv[i], "b") == 0)) && ((i + 1) < argc)) {
            i++;
            s_speed = strtoul(argv[i], NULL, 0);
        } else if (((strcmp(argv[i], "-m") == 0) ||
                    (strcmp(argv[i], "m") == 0)) && ((i + 1) < argc)) {
            i++;
            s_max_polling_times = strtoul(argv[i], NULL, 0);
        } else if (((strcmp(argv[i], "-l") == 0) ||
                    (strcmp(argv[i], "l") == 0)) && ((i + 1) < argc)) {
            i++;
            s_loop_times = strtoul(argv[i], NULL, 0);
        } else if (argv[i][0] == '-') {
            print_usage(argv[0]);
            return -1;
        } else {
            break;
        }
    }

    res = polling_rcs956();

    return res;
}
