/*
 * sample_sample5n.c - Sample code using Sample5N card.
 *
 *   This program uses the following FeliCa commands.
 *     - Polling
 *     - Request Service
 *     - Write Without Encryption
 *     - Read Without Encryption
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
#define DEFAULT_SYSTEM_CODE 0x0018 /* Sample5N */
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
#ifndef DEFAULT_POLLING_OPTION
#define DEFAULT_POLLING_OPTION 0
#endif
#ifndef DEFAULT_POLLING_TIMESLOT
#define DEFAULT_POLLING_TIMESLOT 0
#endif

#ifndef FELICA_COMMAND_TIMEOUT_ADD
#define FELICA_COMMAND_TIMEOUT_ADD 16 /* ms */
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
static UINT32 s_command_max_retry_times = DEFAULT_COMMAND_MAX_RETRY_TIMES;
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
    fprintf(stderr, "  -R max retry times of command (default: 2)\n");

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
    UINT32 command_timeout;

    printf("FeliCa read/write access sample.\n");

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
    }

    /*
     * Request Service
     */
    {
        const UINT16 node_code_list[3] = {
            0x0000,
            0x1009,
            0x100b,
        };
        UINT16 node_key_version_list[3];

        command_timeout =
            FELICA_CC_CALC_TIMEOUT(card.pmm[FELICA_CC_PMM_REQUEST_SERVICE], 3);
        command_timeout += FELICA_COMMAND_TIMEOUT_ADD;

        printf("start Request Service ...\n");

        for (nretries = 0; nretries <= s_command_max_retry_times; nretries++) {
            printf("  calling felica_cc_request_service() ...\n");
            rc = felica_cc_request_service(&devf,
                                           &card,
                                           3,
                                           node_code_list,
                                           node_key_version_list,
                                           command_timeout);
            if ((rc != ICS_ERROR_TIMEOUT) &&
                (rc != ICS_ERROR_FRAME_CRC)) {
                break;
            }
        }
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr,
                    "    failure in felica_cc_request_service():%u\n", rc);
            finalize(&dev);
            return -1;
        }

        for (i = 0; i < 3; i++) {
            printf("    Node: %04x, KeyVersion: %04x\n",
                   node_code_list[i], node_key_version_list[i]);
        }
    }

    /*
     * Write Without Encryption
     */
    {
        const UINT16 service_code_list[1] = {
            0x1009
        };
        const UINT8 block_list[2 * 4] = {
            0x80, 0x00, /* service code list #0, block #0 */
            0x80, 0x01, /* service code list #0, block #1 */
            0x80, 0x02, /* service code list #0, block #2 */
            0x80, 0x03, /* service code list #0, block #3 */
        };
        UINT8 block_data[4 * 16];
        UINT8 status_flag1;
        UINT8 status_flag2;

        /* make write data */
        for (i = 0; i < sizeof(block_data); i++) {
            block_data[i] = (UINT8)(i + 1);
        }

        command_timeout =
            FELICA_CC_CALC_TIMEOUT(
                card.pmm[FELICA_CC_PMM_WRITE_WITHOUT_ENCRYPTION],
                4);
        command_timeout += FELICA_COMMAND_TIMEOUT_ADD;

        printf("start Write Without Encryption ...\n");

        for (nretries = 0; nretries <= s_command_max_retry_times; nretries++) {
            printf("  calling felica_cc_write_without_encryptioin() ...\n");
            rc = felica_cc_write_without_encryption(&devf,
                                                    &card,
                                                    1,
                                                    service_code_list,
                                                    4,
                                                    block_list,
                                                    block_data,
                                                    &status_flag1,
                                                    &status_flag2,
                                                    command_timeout);
            if ((rc != ICS_ERROR_TIMEOUT) &&
                (rc != ICS_ERROR_FRAME_CRC)) {
                break;
            }
        }
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr,
                    "    failure in felica_cc_read_without_encryption():%u\n",
                    rc);
            if (rc == ICS_ERROR_STATUS_FLAG1) {
                fprintf(stderr, "    status_flag1 = %02x\n", status_flag1);
                fprintf(stderr, "    status_flag2 = %02x\n", status_flag2);
            }
            finalize(&dev);
            return -1;
        }

        printf("    status_flag1 = %02x\n", status_flag1);
        printf("    status_flag2 = %02x\n", status_flag2);
    }

    /*
     * Read Without Encryption
     */
    {
        const UINT16 service_code_list[1] = {
            0x100b
        };
        const UINT8 block_list[2 * 4] = {
            0x80, 0x00, /* service code list #0, block #0 */
            0x80, 0x01, /* service code list #0, block #1 */
            0x80, 0x02, /* service code list #0, block #2 */
            0x80, 0x03, /* service code list #0, block #3 */
        };
        UINT8 block_data[4 * 16];
        UINT8 status_flag1;
        UINT8 status_flag2;

        command_timeout =
            FELICA_CC_CALC_TIMEOUT(
                card.pmm[FELICA_CC_PMM_READ_WITHOUT_ENCRYPTION],
                4);
        command_timeout += FELICA_COMMAND_TIMEOUT_ADD;

        printf("start Read Without Encryption ...\n");

        for (nretries = 0; nretries <= s_command_max_retry_times; nretries++) {
            printf("  calling felica_cc_read_without_encryptioin() ...\n");
            rc = felica_cc_read_without_encryption(&devf,
                                                   &card,
                                                   1,
                                                   service_code_list,
                                                   4,
                                                   block_list,
                                                   block_data,
                                                   &status_flag1,
                                                   &status_flag2,
                                                   command_timeout);
            if ((rc != ICS_ERROR_TIMEOUT) &&
                (rc != ICS_ERROR_FRAME_CRC)) {
                break;
            }
        }
        if (rc != ICS_ERROR_SUCCESS) {
            fprintf(stderr,
                    "    failure in felica_cc_read_without_encryption():%u\n",
                    rc);
            if (rc == ICS_ERROR_STATUS_FLAG1) {
                fprintf(stderr, "    status_flag1 = %02x\n", status_flag1);
                fprintf(stderr, "    status_flag2 = %02x\n", status_flag2);
            }
            finalize(&dev);
            return -1;
        }

        printf("    status_flag1 = %02x\n", status_flag1);
        printf("    status_flag2 = %02x\n", status_flag2);
        printf("    Read data:\n");
        for (i = 0; i < sizeof(block_data); i++ ) {
            if ((i % 16) == 0) {
                printf("      ");
            }
            printf("%02x", block_data[i]);
            if ((i % 16) == 15) {
                printf("\n");
            }
        }
    }

    finalize(&dev);

    return 0;
}
