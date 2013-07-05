/*
 * Copyright 2006,2007,2008 Sony Corporation
 */

#include <stdlib.h>
#include <string.h>
#include <felica_cc.h>
#include <utl.h>
#include <icsdrv.h>
#include <ics_error.h>

#include "testtool.h"

#ifndef DEFAULT_PORT_NAME
#define DEFAULT_PORT_NAME "/dev/ttyS0"
#endif

#define DEFAULT_TIMEOUT 400
#ifndef DEFAULT_SPEED
#define DEFAULT_SPEED 115200
#endif
#ifndef DEFAULT_INITIAL_SPEED
#define DEFAULT_INITIAL_SPEED 115200
#endif
#define PING_TIMEOUT 50

extern const icsdrv_basic_func_t* g_drv_func;
extern UINT32 (*g_felica_cc_stub_initialize_func)(
    felica_cc_devf_t* devf,
    ICS_HW_DEVICE* dev);

static const char* s_port_name = DEFAULT_PORT_NAME;
static UINT32 s_timeout = DEFAULT_TIMEOUT;
static UINT32 s_initial_speed = DEFAULT_INITIAL_SPEED;
static UINT32 s_speed = DEFAULT_SPEED;

static int te_felica_cc_polling(void)
{
    UINT32 rc;
    ICS_HW_DEVICE dev;
    felica_cc_devf_t devf;
    felica_card_t card;
    felica_card_t card2;

    /* initialize */
    {
        if (g_drv_func->open != NULL) {
            rc = g_drv_func->open(&dev, s_port_name);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if ((g_drv_func->set_speed != NULL) &&
            (g_drv_func->set_dev_speed != NULL) &&
            (s_speed != s_initial_speed)) {
            rc = g_drv_func->set_speed(&dev, s_initial_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
            if (g_drv_func->ping != NULL) {
                rc = g_drv_func->ping(&dev, PING_TIMEOUT);
            }
            if (rc == ICS_ERROR_SUCCESS) {
                rc = g_drv_func->set_dev_speed(&dev, s_speed, s_timeout);
                T_CHECK(rc == ICS_ERROR_SUCCESS);
            }
        }
        if (g_drv_func->set_speed != NULL) {
            rc = g_drv_func->set_speed(&dev, s_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->initialize_device != NULL) {
            rc = g_drv_func->initialize_device(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        rc = g_felica_cc_stub_initialize_func(&devf, &dev);
        T_CHECK(rc == ICS_ERROR_SUCCESS);
    }

    memset(&card, 0xa5, sizeof(card));
    rc = felica_cc_polling(&devf, (const UINT8*)"\xff\xff\x01\x00",
                           &card, NULL, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    memset(&card2, 0x5a, sizeof(card2));
    rc = felica_cc_polling(&devf, (const UINT8*)"\xff\xff\x01\x00",
                           &card2, NULL, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(memcmp(&card, &card2, sizeof(card)) == 0);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    /* finalize */
    {
        if (g_drv_func->rf_off != NULL) {
            rc = g_drv_func->rf_off(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->close != NULL) {
            rc = g_drv_func->close(&dev);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }
    }

    return 0;
}

static int te_felica_cc_request_service(void)
{
    UINT32 rc;
    ICS_HW_DEVICE dev;
    felica_cc_devf_t devf;
    felica_card_t card;
    const UINT16 node_code_list[3] = {0x0000, 0xf800, 0x1008};
    UINT16 node_key_version_list[3];

    /* initialize */
    {
        if (g_drv_func->open != NULL) {
            rc = g_drv_func->open(&dev, s_port_name);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if ((g_drv_func->set_speed != NULL) &&
            (g_drv_func->set_dev_speed != NULL) &&
            (s_speed != s_initial_speed)) {
            rc = g_drv_func->set_speed(&dev, s_initial_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
            if (g_drv_func->ping != NULL) {
                rc = g_drv_func->ping(&dev, PING_TIMEOUT);
            }
            if (rc == ICS_ERROR_SUCCESS) {
                rc = g_drv_func->set_dev_speed(&dev, s_speed, s_timeout);
                T_CHECK(rc == ICS_ERROR_SUCCESS);
            }
        }
        if (g_drv_func->set_speed != NULL) {
            rc = g_drv_func->set_speed(&dev, s_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->initialize_device != NULL) {
            rc = g_drv_func->initialize_device(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        rc = g_felica_cc_stub_initialize_func(&devf, &dev);
        T_CHECK(rc == ICS_ERROR_SUCCESS);
    }

    rc = felica_cc_polling(&devf, (const UINT8*)"\xff\xff\x01\x00",
                           &card, NULL, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = felica_cc_request_service(&devf, &card, 3, node_code_list,
                                   node_key_version_list,
                                   s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(node_key_version_list[0] == 0x0123);
    T_CHECK(node_key_version_list[1] == 0xffff);
    T_CHECK(node_key_version_list[2] != 0xffff);

    /* finalize */
    {
        if (g_drv_func->rf_off != NULL) {
            rc = g_drv_func->rf_off(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->close != NULL) {
            rc = g_drv_func->close(&dev);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }
    }

    return 0;
}

static int te_felica_cc_request_response(void)
{
    UINT32 rc;
    ICS_HW_DEVICE dev;
    felica_cc_devf_t devf;
    felica_card_t card;
    UINT8 mode;

    /* initialize */
    {
        if (g_drv_func->open != NULL) {
            rc = g_drv_func->open(&dev, s_port_name);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if ((g_drv_func->set_speed != NULL) &&
            (g_drv_func->set_dev_speed != NULL) &&
            (s_speed != s_initial_speed)) {
            rc = g_drv_func->set_speed(&dev, s_initial_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
            if (g_drv_func->ping != NULL) {
                rc = g_drv_func->ping(&dev, PING_TIMEOUT);
            }
            if (rc == ICS_ERROR_SUCCESS) {
                rc = g_drv_func->set_dev_speed(&dev, s_speed, s_timeout);
                T_CHECK(rc == ICS_ERROR_SUCCESS);
            }
        }
        if (g_drv_func->set_speed != NULL) {
            rc = g_drv_func->set_speed(&dev, s_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->initialize_device != NULL) {
            rc = g_drv_func->initialize_device(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        rc = g_felica_cc_stub_initialize_func(&devf, &dev);
        T_CHECK(rc == ICS_ERROR_SUCCESS);
    }

    rc = felica_cc_polling(&devf, (const UINT8*)"\xff\xff\x01\x00",
                           &card, NULL, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = felica_cc_request_response(&devf, &card, &mode, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(mode == 0);

    /* finalize */
    {
        if (g_drv_func->rf_off != NULL) {
            rc = g_drv_func->rf_off(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->close != NULL) {
            rc = g_drv_func->close(&dev);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }
    }

    return 0;
}

static int te_felica_cc_request_system_code(void)
{
    UINT32 rc;
    ICS_HW_DEVICE dev;
    felica_cc_devf_t devf;
    felica_card_t card;
    UINT8 num_of_system_codes;
    UINT8 system_code_list[2 * 8];

    /* initialize */
    {
        if (g_drv_func->open != NULL) {
            rc = g_drv_func->open(&dev, s_port_name);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if ((g_drv_func->set_speed != NULL) &&
            (g_drv_func->set_dev_speed != NULL) &&
            (s_speed != s_initial_speed)) {
            rc = g_drv_func->set_speed(&dev, s_initial_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
            if (g_drv_func->ping != NULL) {
                rc = g_drv_func->ping(&dev, PING_TIMEOUT);
            }
            if (rc == ICS_ERROR_SUCCESS) {
                rc = g_drv_func->set_dev_speed(&dev, s_speed, s_timeout);
                T_CHECK(rc == ICS_ERROR_SUCCESS);
            }
        }
        if (g_drv_func->set_speed != NULL) {
            rc = g_drv_func->set_speed(&dev, s_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->initialize_device != NULL) {
            rc = g_drv_func->initialize_device(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        rc = g_felica_cc_stub_initialize_func(&devf, &dev);
        T_CHECK(rc == ICS_ERROR_SUCCESS);
    }

    rc = felica_cc_polling(&devf, (const UINT8*)"\xff\xff\x01\x00",
                           &card, NULL, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    rc = felica_cc_request_system_code(&devf, &card, 8,
                                       &num_of_system_codes,
                                       system_code_list,
                                       s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    /* finalize */
    {
        if (g_drv_func->rf_off != NULL) {
            rc = g_drv_func->rf_off(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->close != NULL) {
            rc = g_drv_func->close(&dev);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }
    }

    return 0;
}

static int te_felica_cc_write_read_without_encryption(void)
{
    UINT32 rc;
    ICS_HW_DEVICE dev;
    felica_cc_devf_t devf;
    felica_card_t card;
    const UINT16 service_code_list[2] = {0x1009, 0x100b};
    UINT8 block_list[5];
    UINT8 block_data[2 * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;

    /* initialize */
    {
        if (g_drv_func->open != NULL) {
            rc = g_drv_func->open(&dev, s_port_name);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if ((g_drv_func->set_speed != NULL) &&
            (g_drv_func->set_dev_speed != NULL) &&
            (s_speed != s_initial_speed)) {
            rc = g_drv_func->set_speed(&dev, s_initial_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
            if (g_drv_func->ping != NULL) {
                rc = g_drv_func->ping(&dev, PING_TIMEOUT);
            }
            if (rc == ICS_ERROR_SUCCESS) {
                rc = g_drv_func->set_dev_speed(&dev, s_speed, s_timeout);
                T_CHECK(rc == ICS_ERROR_SUCCESS);
            }
        }
        if (g_drv_func->set_speed != NULL) {
            rc = g_drv_func->set_speed(&dev, s_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->initialize_device != NULL) {
            rc = g_drv_func->initialize_device(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        rc = g_felica_cc_stub_initialize_func(&devf, &dev);
        T_CHECK(rc == ICS_ERROR_SUCCESS);
    }

    rc = felica_cc_polling(&devf, (const UINT8*)"\xff\xff\x01\x00",
                           &card, NULL, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    block_list[0] = 0x80;
    block_list[1] = 0x00;
    block_list[2] = 0x00;
    block_list[3] = 0x01;
    block_list[4] = 0x00;
    {
        UINT i;
        for (i = 0; i < sizeof(block_data); i++) {
            block_data[i] = 5 + i;
        }
    }
    rc = felica_cc_write_without_encryption(&devf, &card,
                                            2, service_code_list,
                                            2, block_list,
                                            block_data,
                                            &status_flag1, &status_flag2,
                                            s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(status_flag1 == 0x00);
    T_CHECK(status_flag2 == 0x00);

    block_list[0] = 0x81;
    block_list[1] = 0x00;
    block_list[2] = 0x00;
    block_list[3] = 0x01;
    block_list[4] = 0x00;
    utl_memset(block_data, 0xa5, sizeof(block_data));
    rc = felica_cc_read_without_encryption(&devf, &card,
                                           2, service_code_list,
                                           2, block_list,
                                           block_data,
                                           &status_flag1, &status_flag2,
                                           s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(status_flag1 == 0x00);
    T_CHECK(status_flag2 == 0x00);
    {
        UINT i;
        for (i = 0; i < sizeof(block_data); i++) {
            T_CHECK(block_data[i] == (UINT8)(5 + i));
        }
    }

    /* finalize */
    {
        if (g_drv_func->rf_off != NULL) {
            rc = g_drv_func->rf_off(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->close != NULL) {
            rc = g_drv_func->close(&dev);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }
    }

    return 0;
}

static int te_felica_cc_write_without_encryption_error(void)
{
    UINT32 rc;
    ICS_HW_DEVICE dev;
    felica_cc_devf_t devf;
    felica_card_t card;
    const UINT16 service_code_list[2] = {0x0009, 0x9909};
    UINT8 block_list[5];
    UINT8 block_data[2 * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;

    /* initialize */
    {
        if (g_drv_func->open != NULL) {
            rc = g_drv_func->open(&dev, s_port_name);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if ((g_drv_func->set_speed != NULL) &&
            (g_drv_func->set_dev_speed != NULL) &&
            (s_speed != s_initial_speed)) {
            rc = g_drv_func->set_speed(&dev, s_initial_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
            if (g_drv_func->ping != NULL) {
                rc = g_drv_func->ping(&dev, PING_TIMEOUT);
            }
            if (rc == ICS_ERROR_SUCCESS) {
                rc = g_drv_func->set_dev_speed(&dev, s_speed, s_timeout);
                T_CHECK(rc == ICS_ERROR_SUCCESS);
            }
        }
        if (g_drv_func->set_speed != NULL) {
            rc = g_drv_func->set_speed(&dev, s_speed);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->initialize_device != NULL) {
            rc = g_drv_func->initialize_device(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        rc = g_felica_cc_stub_initialize_func(&devf, &dev);
        T_CHECK(rc == ICS_ERROR_SUCCESS);
    }

    rc = felica_cc_polling(&devf, (const UINT8*)"\xff\xff\x01\x00",
                           &card, NULL, s_timeout);
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    block_list[0] = 0x80;
    block_list[1] = 0x00;
    block_list[2] = 0x00;
    block_list[3] = 0x01;
    block_list[4] = 0x00;
    {
        UINT i;
        for (i = 0; i < sizeof(block_data); i++) {
            block_data[i] = 5 + i;
        }
    }
    rc = felica_cc_write_without_encryption(&devf, &card,
                                            2, service_code_list,
                                            2, block_list,
                                            block_data,
                                            &status_flag1, &status_flag2,
                                            s_timeout);
    T_CHECK((rc == ICS_ERROR_TIMEOUT) ||
            (rc == ICS_ERROR_STATUS_FLAG1));

    /* finalize */
    {
        if (g_drv_func->rf_off != NULL) {
            rc = g_drv_func->rf_off(&dev, s_timeout);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }

        if (g_drv_func->close != NULL) {
            rc = g_drv_func->close(&dev);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }
    }

    return 0;
}


static testcase_t te_felica_cc_tc[] = {
    TESTCASE(te_felica_cc_polling),
    TESTCASE(te_felica_cc_request_service),
    TESTCASE(te_felica_cc_request_response),
    TESTCASE(te_felica_cc_request_system_code),
    TESTCASE(te_felica_cc_write_read_without_encryption),
    TESTCASE(te_felica_cc_write_without_encryption_error),
};

static void print_usage(
    const char* argv0)
{
    fprintf(stderr, "Usage: %s [options] testcases...\n", argv0);
    fprintf(stderr, "  -p port\n");
    fprintf(stderr, "  -b UART speed\n");
    fprintf(stderr, "  -i initial UART speed\n");
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
        } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            i++;
            s_speed = strtoul(argv[i], NULL, 0);
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            i++;
            s_initial_speed = strtoul(argv[i], NULL, 0);
        } else if (argv[i][0] == '-') {
            print_usage(argv[0]);
            fprintf(stderr, "testcases:\n");
            print_testcases(te_felica_cc_tc,
                            TESTTOOL_NCASES(te_felica_cc_tc));
            return -1;
        } else {
            break;
        }
    }

    if (i < argc) {
        res = do_specified_testcases(te_felica_cc_tc,
                                     TESTTOOL_NCASES(te_felica_cc_tc),
                                     argv + i, argc - i);
    } else {
        res = do_testcases(te_felica_cc_tc,
                           TESTTOOL_NCASES(te_felica_cc_tc));
    }

    return res;
}
