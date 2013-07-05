/*
 * Copyright 2006,2007,2008 Sony Corporation
 */

#include <stdlib.h>
#include <string.h>
#include <felica_cc.h>
#include <utl.h>
#include <icsdrv.h>
#include <ics_error.h>

#ifndef TESTPR
#include <stdio.h>
#define TESTPR  printf
#endif

#define T_CHECK(cond) { \
    if (!(cond)) { \
        TESTPR("#error:%d: %s\n", __LINE__, #cond); \
        return -1; \
    } \
}

#ifndef DEFAULT_PORT_NAME
#define DEFAULT_PORT_NAME "/dev/ttyS0"
#endif

#define DEFAULT_TIMEOUT 400
#define DEFAULT_RETRY   3
#define DEFAULT_SYSTEM_CODE 0xffff
#define DEFAULT_NODE_CODE 0x0000

#ifndef DEFAULT_SPEED
#define DEFAULT_SPEED 115200
#endif
#ifndef DEFAULT_INITIAL_SPEED
#define DEFAULT_INITIAL_SPEED 115200
#endif
#define PING_TIMEOUT 50

#define MAX_NODES       256

extern const icsdrv_basic_func_t* g_drv_func;
extern UINT32 (*g_felica_cc_stub_initialize_func)(
    felica_cc_devf_t* devf,
    ICS_HW_DEVICE* dev);

static const char* s_port_name = DEFAULT_PORT_NAME;
static UINT32 s_timeout = DEFAULT_TIMEOUT;
static UINT32 s_retry = DEFAULT_RETRY;
static UINT16 s_system_code = DEFAULT_SYSTEM_CODE;
static UINT16 s_node_code = DEFAULT_NODE_CODE;

static UINT32 s_initial_speed = DEFAULT_INITIAL_SPEED;
static UINT32 s_speed = DEFAULT_SPEED;

static int felica_dump_syscode(void)
{
    UINT32 rc;
    ICS_HW_DEVICE dev;
    felica_cc_devf_t devf;
    felica_card_t card;
    felica_card_option_t card_option;
    UINT32 retry;
    UINT8 polling_param[4];
    UINT8 num_of_system_codes;
    UINT8 system_code_list[2 * 32];
    UINT8 i;

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

    polling_param[0] = (UINT8)((s_system_code >> 8) & 0xff);
    polling_param[1] = (UINT8)((s_system_code >> 0) & 0xff);
    polling_param[2] = 0x01;
    polling_param[3] = 0x00;

    retry = s_retry;
    do {
        rc = felica_cc_polling(&devf, polling_param,
                               &card, &card_option, s_timeout);
    } while ((rc != ICS_ERROR_SUCCESS) && (retry-- > 0));
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    TESTPR("IDm: %02x%02x%02x%02x%02x%02x%02x%02x\n",
           card.idm[0], card.idm[1], card.idm[2], card.idm[3],
           card.idm[4], card.idm[5], card.idm[6], card.idm[7]);
    TESTPR("PMm: %02x%02x%02x%02x%02x%02x%02x%02x\n",
           card.pmm[0], card.pmm[1], card.pmm[2], card.pmm[3],
           card.pmm[4], card.pmm[5], card.pmm[6], card.pmm[7]);

    if (card_option.option_len == 2) {
        TESTPR("System code: %02x%02x (current)\n",
               card_option.option[0],
               card_option.option[1]);
    }

    retry = s_retry;
    do {
        rc = felica_cc_request_system_code(&devf, &card,
                                           32, 
                                           &num_of_system_codes,
                                           system_code_list,
                                           s_timeout);
    } while ((rc != ICS_ERROR_SUCCESS) && (retry-- > 0));
    T_CHECK(rc == ICS_ERROR_SUCCESS);
    T_CHECK(num_of_system_codes <= 32);

    for (i = 0; i < num_of_system_codes; i++) {
        TESTPR("System code: %02x%02x\n",
               system_code_list[(i * 2) + 0],
               system_code_list[(i * 2) + 1]);
    }

    /* finalize */
    {
        if (g_drv_func->close != NULL) {
            rc = g_drv_func->close(&dev);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }
    }

    return 0;
}

static int felica_dump_read(void)
{
    UINT32 rc;
    ICS_HW_DEVICE dev;
    felica_cc_devf_t devf;
    felica_card_t card;
    felica_card_option_t card_option;
    UINT32 retry;
    UINT8 polling_param[4];
    UINT8 block_list[3];
    UINT8 block_data[16];
    UINT8 status_flag1;
    UINT8 status_flag2;
    UINT32 i;

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

    polling_param[0] = (UINT8)((s_system_code >> 8) & 0xff);
    polling_param[1] = (UINT8)((s_system_code >> 0) & 0xff);
    polling_param[2] = 0x01;
    polling_param[3] = 0x00;

    retry = s_retry;
    do {
        rc = felica_cc_polling(&devf, polling_param,
                               &card, &card_option, s_timeout);
    } while ((rc != ICS_ERROR_SUCCESS) && (retry-- > 0));
    T_CHECK(rc == ICS_ERROR_SUCCESS);

    TESTPR("IDm: %02x%02x%02x%02x%02x%02x%02x%02x\n",
           card.idm[0], card.idm[1], card.idm[2], card.idm[3],
           card.idm[4], card.idm[5], card.idm[6], card.idm[7]);
    TESTPR("PMm: %02x%02x%02x%02x%02x%02x%02x%02x\n",
           card.pmm[0], card.pmm[1], card.pmm[2], card.pmm[3],
           card.pmm[4], card.pmm[5], card.pmm[6], card.pmm[7]);

    TESTPR("Service: %04x\n", s_node_code);

    for (i = 0; i < 0x10000; i++) {
        if (i >= 0x100) {
            block_list[0] = 0x00;
            block_list[1] = (UINT8)((i >> 0) & 0xff);
            block_list[2] = (UINT8)((i >> 8) & 0xff);
        } else {
            block_list[0] = 0x80;
            block_list[1] = (UINT8)i;
        }

        retry = s_retry;
        do {
            rc = felica_cc_read_without_encryption(&devf, &card,
                                                   1, &s_node_code,
                                                   1, block_list,
                                                   block_data,
                                                   &status_flag1,
                                                   &status_flag2,
                                                   s_timeout);
        } while ((rc != ICS_ERROR_SUCCESS) &&
                 (rc != ICS_ERROR_STATUS_FLAG1) &&
                 (retry-- > 0));
        if ((rc == ICS_ERROR_STATUS_FLAG1) &&
            (status_flag2 == 0xa8)) {
            break;
        }
        T_CHECK(rc == ICS_ERROR_SUCCESS);

        TESTPR("%04x: %02x %02x %02x %02x %02x %02x %02x %02x  "
               "%02x %02x %02x %02x %02x %02x %02x %02x\n",
               i,
               block_data[0], block_data[1], block_data[2], block_data[3],
               block_data[4], block_data[5], block_data[6], block_data[7],
               block_data[8], block_data[9], block_data[10], block_data[11],
               block_data[12], block_data[13], block_data[14], block_data[15]);
    }

    /* finalize */
    {
        if (g_drv_func->close != NULL) {
            rc = g_drv_func->close(&dev);
            T_CHECK(rc == ICS_ERROR_SUCCESS);
        }
    }

    return 0;
}


static void print_usage(
    const char* argv0)
{
    fprintf(stderr, "Usage: %s [options] syscode|read\n", argv0);
    fprintf(stderr, "  -p port\n");
    fprintf(stderr, "  -b UART speed\n");
    fprintf(stderr, "  -i initial UART speed\n");
    fprintf(stderr, "  -t timeout\n");
    fprintf(stderr, "  -r retry\n");
    fprintf(stderr, "  -s system code\n");
    fprintf(stderr, "  -c node code\n");

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
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            i++;
            s_retry = strtoul(argv[i], NULL, 0);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            i++;
            s_system_code = (UINT16)strtoul(argv[i], NULL, 16);
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            i++;
            s_node_code = (UINT16)strtoul(argv[i], NULL, 16);
        } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            i++;
            s_speed = strtoul(argv[i], NULL, 0);
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            i++;
            s_initial_speed = strtoul(argv[i], NULL, 0);
        } else if (argv[i][0] == '-') {
            print_usage(argv[0]);
            return -1;
        } else {
            break;
        }
    }

    if ((i < argc) && (strcmp(argv[i], "syscode") == 0)) {
        res = felica_dump_syscode();
    } else if ((i < argc) && (strcmp(argv[i], "read") == 0)) {
        res = felica_dump_read();
    } else {
        print_usage(argv[0]);
        return -1;
    }

    return res;
}
