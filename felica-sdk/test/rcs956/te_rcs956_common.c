/*
 * Copyright 2006,2008 Sony Corporation
 */

#include <stdlib.h>
#include <string.h>
#include <utl.h>
#include <icsdrv.h>
#include <ics_error.h>
#include <rcs956.h>

#include "testtool.h"

#ifndef DEFAULT_PORT_NAME
#define DEFAULT_PORT_NAME "/dev/ttyS0"
#endif

#define DEFAULT_TIMEOUT 400

extern const icsdrv_basic_func_t* g_drv_func;

static const char* s_port_name = DEFAULT_PORT_NAME;
static UINT32 s_timeout = DEFAULT_TIMEOUT;

static int te_rcs956_open(void)
{
    UINT32 rc;
    ICS_HW_DEVICE rcs956;

    rc = g_drv_func->open(&rcs956, s_port_name);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->close(&rcs956);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    return 0;
}

static int te_rcs956_execute_command(void)
{
    UINT32 rc;
    ICS_HW_DEVICE rcs956;
    UINT8 response[254];
    UINT32 response_len;

    rc = g_drv_func->open(&rcs956, s_port_name);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    /* initialize the device */
    rc = g_drv_func->initialize_device(&rcs956, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    /* Get Firmware Version command */
    rc = g_drv_func->execute_command(&rcs956,
                                     (const UINT8*)"\xd4\x02", 2,
                                     6, response, &response_len,
                                     s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(response_len == 6);
    T_CHECK(response[0] == 0xd5);
    T_CHECK(response[1] == 0x03);
    T_CHECK(response[2] == 0x33);
    T_CHECK(response[3] == 0x01);

    /* InListPassiveTarget */
    rc = g_drv_func->execute_command(&rcs956,
                                     (const UINT8*)"\xd4\x4a\x01\x01"
                                     "\x00\xff\xff\x00\x00", 9,
                                     22, response, &response_len,
                                     s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(response_len == 22);
    T_CHECK(response[0] == 0xd5);
    T_CHECK(response[1] == 0x4b);
    T_CHECK(response[2] == 0x01);
    T_CHECK(response[3] == 0x01);

    rc = g_drv_func->rf_off(&rcs956, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->close(&rcs956);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    return 0;
}

static int te_rcs956_felica_command(void)
{
    UINT32 rc;
    ICS_HW_DEVICE rcs956;
    UINT8 command[254];
    UINT8 response[254];
    UINT32 response_len;

    rc = g_drv_func->open(&rcs956, s_port_name);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    /* initialize the device */
    rc = g_drv_func->initialize_device(&rcs956, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    /* Get Firmware Version command */
    rc = g_drv_func->execute_command(&rcs956,
                                     (const UINT8*)"\xd4\x02", 2,
                                     6, response, &response_len,
                                     s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(response_len == 6);
    T_CHECK(response[0] == 0xd5);
    T_CHECK(response[1] == 0x03);
    T_CHECK(response[2] == 0x33);
    T_CHECK(response[3] == 0x01);

    /* InListPassiveTarget */
    rc = g_drv_func->execute_command(&rcs956,
                                     (const UINT8*)"\xd4\x4a\x01\x01"
                                     "\x00\xff\xff\x00\x00", 9,
                                     22, response, &response_len,
                                     s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(response_len == 22);
    T_CHECK(response[0] == 0xd5);
    T_CHECK(response[1] == 0x4b);
    T_CHECK(response[2] == 0x01);
    T_CHECK(response[3] == 0x01);

    /* FeliCa Polling */
    rc = g_drv_func->felica_command(&rcs956,
                                    (const UINT8*)"\x00\xff\xff\x00\x00", 5,
                                    17, response, &response_len,
                                    s_timeout, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(response_len == 17);
    T_CHECK(response[0] == 0x01);

    /* FeliCa Echo */
    {
        UINT i;

        command[0] = 0xf0;
        command[1] = 0x00;
        for (i = 2; i < sizeof(command); i++) {
            command[i] = (UINT8)(i + 5);
        }

        rc = g_drv_func->felica_command(&rcs956,
                                        command, sizeof(command),
                                        sizeof(response),
                                        response, &response_len,
                                        s_timeout, s_timeout);
        T_CHECK(rc == ICS_ERROR_SUCCESS);
        T_CHECK(response_len == sizeof(command));
        T_CHECK(response[0] == 0xf0);
        T_CHECK(response[1] == 0x00);
        for (i = 2; i < sizeof(command); i++) {
            T_CHECK(response[i] == (UINT8)(i + 5));
        }
    }

    rc = g_drv_func->rf_off(&rcs956, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->close(&rcs956);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    return 0;
}

static int te_rcs956_cancel_command(void)
{
    UINT32 rc;
    ICS_HW_DEVICE rcs956;
    UINT8 response[254];
    UINT32 response_len;

    rc = g_drv_func->open(&rcs956, s_port_name);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    /* InListPassiveTarget */
    rc = g_drv_func->execute_command(&rcs956,
                                     (const UINT8*)"\xd4\x4a\x01\x01"
                                     "\x00\xff\xff\x00\x00", 9,
                                     22, response, &response_len,
                                     0);
    T_CHECK(rc == ICS_ERROR_TIMEOUT);

    /* cancel command */
    rc = g_drv_func->cancel_command(&rcs956);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->rf_off(&rcs956, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->close(&rcs956);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    return 0;
}

static int te_rcs956_reset(void)
{
    UINT32 rc;
    ICS_HW_DEVICE rcs956;

    rc = g_drv_func->open(&rcs956, s_port_name);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->reset(&rcs956, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->close(&rcs956);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    return 0;
}

static int te_rcs956_get_firmware_version(void)
{
    UINT32 rc;
    UINT8 ic_type;
    UINT16 version;
    ICS_HW_DEVICE rcs956;

    rc = g_drv_func->open(&rcs956, s_port_name);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = rcs956_get_firmware_version(&rcs956, &ic_type, &version,
                                     s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->rf_off(&rcs956, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = g_drv_func->close(&rcs956);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    return 0;
}


static testcase_t te_rcs956_tc[] = {
    TESTCASE(te_rcs956_open),
    TESTCASE(te_rcs956_execute_command),
    TESTCASE(te_rcs956_felica_command),
    TESTCASE(te_rcs956_cancel_command),
    TESTCASE(te_rcs956_reset),
    TESTCASE(te_rcs956_get_firmware_version),
};

static void print_usage(
    const char* argv0)
{
    fprintf(stderr, "Usage: %s [options] testcases...\n", argv0);
    fprintf(stderr, "  -p port\n");
    fprintf(stderr, "  -t timeout\n");

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
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            i++;
            s_port_name = argv[i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            i++;
            s_timeout = strtoul(argv[i], NULL, 0);
        } else if (argv[i][0] == '-') {
            print_usage(argv[0]);
            fprintf(stderr, "testcases:\n");
            print_testcases(te_rcs956_tc,
                            TESTTOOL_NCASES(te_rcs956_tc));
            return -1;
        } else {
            break;
        }
    }

    if (i < argc) {
        res = do_specified_testcases(te_rcs956_tc,
                                     TESTTOOL_NCASES(te_rcs956_tc),
                                     argv + i, argc - i);
    } else {
        res = do_testcases(te_rcs956_tc,
                           TESTTOOL_NCASES(te_rcs956_tc));
    }

    return res;
}
