/**
 * \brief    Test Program for reliability test
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef TESTPR
#include <stdio.h>
#define TESTPR testpr

#endif
#include "felica_card.h"
#include "felica_cc.h"
#include "felica_cc_stub.h"
#include "felica_cc_stub_rcs956.h"

#include "rcs956.h"
#if defined DRV_UART
#include "rcs956_uart.h"
#include "icsdrv_uart.h"
const icsdrv_basic_func_t* s_drv_func = &rcs956_uart_basic_func;
#elif defined DRV_USB
#include "rcs956_usb.h"
const icsdrv_basic_func_t* s_drv_func = &rcs956_usb_basic_func;
#endif

#include "ics_types.h"
#include "ics_error.h"
#include "ics_hwdev.h"
#include "icsdrv.h"
#include "icslib_chk.h"
#include "icslog.h"

#include "arch_ics_types.h"
#include "arch_icslog.h"
#include "arch_utl.h"

#include "utl.h"

/* total time[sec](from test metrics) */
#define TOTALTIMESEC      360000

/* max recovery time */
#define MAX_RECOVERY_COUNT  5

/* waiting time */
#define WAIT_POWER_DOWN    10

/* check macro */
#define T_CHECK_EQ(first, second) { \
    if ((first) != (second)) { \
        TESTPR("#error:%d: %s != %s(%d != %d)\n", \
                __LINE__, #first, #second, (UINT)first, (UINT)second); \
        return (0xffff); \
    } \
}

#define PAUSE(msg) { \
    TESTPR((msg)); \
    while (getchar() != '\n'){} \
}

#define PAUSE_SET_RCS860() { \
    PAUSE("Set felica chip: [RC-S860] ( >> Push 'Enter')\n"); \
}

#define ERR_RECOVERY() { \
    TESTPR("The error occurred while recovering\n"); \
}

#define ERROR_LOG(error_no) { \
        TESTPR("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); \
        TESTPR("#error:%d: %s (%d)\n", \
               __LINE__, #error_no, (UINT)error_no); \
        TESTPR("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n"); \
        s_fail++;                                     \
}

#define FELICA_CC_CALC_PROC_TIME(x, nblocks) \
        (FELICA_CC_CALC_TIMEOUT(x, nblocks) + 16)

#define DEFAULT_PORTNAME  "COM1"
#define DEFAULT_SPEED 115200
#define DEFAULT_TIMEOUT 1000
#define DEFAULT_INITIAL_SPEED 115200
#define PING_TIMEOUT 50
#define TE_ERROR (UINT32)0xabcd

#define DEFAULT_FILENAME  "te_reliability.txt"

static const char* s_port_name = DEFAULT_PORTNAME;
static const char* s_logfilename = DEFAULT_FILENAME;
static UINT32 s_timeout = DEFAULT_TIMEOUT;
static UINT32 s_speed = DEFAULT_SPEED;
static UINT32 s_initial_speed = DEFAULT_INITIAL_SPEED;
static UINT32 s_recovery_cnt = 0;
static UINT32 s_start_time = 0;
static UINT16 s_fail = 0;
static FILE *s_fp = NULL;

typedef struct scenario_paramater_t {

    ICS_HW_DEVICE rcs956_dev;
    felica_cc_devf_t felica_dev;
    felica_card_t card;
    
    /* input paramaters */
    UINT8 num_of_nodes;
    UINT16 node_code_list[FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX];
    UINT8 max_system_codes;
    UINT16 service_index;
    UINT8 rw_num_of_service;
    UINT16 rw_service_code_list[FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX];
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 2];
    UINT8 write_block_data[FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX * 16];

    /* output paramaters */
    UINT16 node_key_version_list[FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX];
    UINT8 num_of_system_codes;
    UINT8 system_code_list[4];
    UINT8 read_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];

} scenario_paramater_t;


UINT32 do_testinitialize(scenario_paramater_t* param);
void do_testterminate(scenario_paramater_t *param);
void set_scenario_paramaters(scenario_paramater_t *param);
void execute_scenario(scenario_paramater_t *param);
UINT32 te_reliability(void);

static void testpr(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    vfprintf(s_fp, format, ap);
    va_end(ap);
}

/**
 * This function is initialization done before it tests.
 * \param param     [IN] parameters.
 */
UINT32 do_testinitialize(scenario_paramater_t* param)
{
    UINT32 rc = TE_ERROR;

    if (s_drv_func->open != NULL) {
        rc = s_drv_func->open(&param->rcs956_dev, s_port_name);
        if (rc != ICS_ERROR_SUCCESS) {
            return rc;
        }
    }

    if ((s_drv_func->set_speed != NULL) &&
        (s_drv_func->set_dev_speed != NULL) &&
        (s_speed != s_initial_speed)) {
        rc = s_drv_func->set_speed(&param->rcs956_dev, s_initial_speed);
        if (rc != ICS_ERROR_SUCCESS) {
            return rc;
        }
        if (s_drv_func->ping != NULL) {
            rc = s_drv_func->ping(&param->rcs956_dev, PING_TIMEOUT);
            if (rc != ICS_ERROR_SUCCESS) {
                return rc;
            }
        }
        if (rc == ICS_ERROR_SUCCESS) {
            rc = s_drv_func->set_dev_speed(&param->rcs956_dev, s_speed, s_timeout);
            if (rc != ICS_ERROR_SUCCESS) {
                return rc;
            }
        }
    }
    if (s_drv_func->set_speed != NULL) {
        rc = s_drv_func->set_speed(&param->rcs956_dev, s_speed);
        if (rc != ICS_ERROR_SUCCESS) {
            return rc;
        }
    }

    if (s_drv_func->initialize_device != NULL) {
        rc = s_drv_func->initialize_device(&param->rcs956_dev, s_timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            return rc;
        }
    }

    rc = felica_cc_stub_rcs956_initialize(&param->felica_dev, &param->rcs956_dev);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }

    s_start_time = utl_get_time_msec();

    return rc;
}

/**
 * When the test execution ends, this function is called. 
 * \param param     [IN] parameters.
 */
void do_testterminate(scenario_paramater_t *param)
{
    rcs956_close(&param->rcs956_dev);
}

/**
 * This function sets the parameter used by execute_scenario function. 
 *
 * \param param    [IN/OUT] parameters.
 *
 */
void set_scenario_paramaters(scenario_paramater_t *param)
{
    int i = 0;        

    /* for 'Request Service' */
    param->num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX;
    for (i = 0; i < 4; i++) {
        param->node_code_list[0 + (i * 8)] = 0xffff;
        param->node_code_list[1 + (i * 8)] = 0x0000;
        param->node_code_list[2 + (i * 8)] = 0x1008;
        param->node_code_list[3 + (i * 8)] = 0x100A;
        param->node_code_list[4 + (i * 8)] = 0x100C;
        param->node_code_list[5 + (i * 8)] = 0x100E;
        param->node_code_list[6 + (i * 8)] = 0x1010;
        param->node_code_list[7 + (i * 8)] = 0x1012;
    }

    /* for 'Request System Code' */
    param->max_system_codes = 1;

    /* for 'Search Service Code' */
    param->service_index = 0x0000;

    /* for 'Read/Write Without Encryption' */
    param->rw_num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    for (i = 0; i < 2; i++) {
        param->rw_service_code_list[0 + (i * 8)] = 0x1009;
        param->rw_service_code_list[1 + (i * 8)] = 0x100B;
        param->rw_service_code_list[2 + (i * 8)] = 0x100D;
        param->rw_service_code_list[3 + (i * 8)] = 0x100F;
        param->rw_service_code_list[4 + (i * 8)] = 0x1011;
        param->rw_service_code_list[5 + (i * 8)] = 0x1013;
        param->rw_service_code_list[6 + (i * 8)] = 0x1015;
        param->rw_service_code_list[7 + (i * 8)] = 0x1209;
    }
    param->num_of_blocks = 12;
    for (i = 0; i < (param->num_of_blocks / 2); i++) {
        param->block_list[0 + (i * 2)]  = 0x80;
        param->block_list[1 + (i * 2)]  = (UINT8)i;
    }
    for (i = 0; i < param->num_of_blocks / 2; i++) {
        param->block_list[12 + (i * 2)]  = 0x87;
        param->block_list[13 + (i * 2)]  = (UINT8)i;
    }
    for (i = 0; i < 8 * 16; i++) {
         param->write_block_data[i] = (UINT8)i;
    }
}

/**
 * This function recovers used by execute_scenario function. 
 *
 * \param param    [IN/OUT] parameters.
 *
 */
static UINT32 recovery_error(scenario_paramater_t* param)
{
    UINT32 rc = 0;

    rc = rcs956_cancel_command(&param->rcs956_dev);
    if (rc != ICS_ERROR_SUCCESS) {
        ERR_RECOVERY();
    }

    rc = utl_msleep(WAIT_POWER_DOWN);
    if (rc != ICS_ERROR_SUCCESS) {
        ERR_RECOVERY();
    }

    rc = rcs956_ping(&param->rcs956_dev, s_timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ERR_RECOVERY();
    }
    
    s_recovery_cnt++;

    return rc;
}

/**
 * This function executes the examination scenario  for reliability test. 
 *
 * \param param     [IN/OUT] parameters.
 *
 * \retval Please look at ics_error.h.
 */
void execute_scenario(scenario_paramater_t *param)
{
    UINT32 rc = ICS_ERROR_SUCCESS;
    UINT32 timeout;
    UINT8 mode;
    UINT8 num_of_blocks;
    UINT8 status_flag1, status_flag2;
    felica_card_option_t card_option;
    UINT8 parameter[4] = {0xff, 0xff, 0x00, 0x00};
    UINT8 ic_type;
    UINT16 version;
    UINT32 recovery;
    int i;

    /* do rcs956_get_firmware_version */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_get_firmware_version(&param->rcs956_dev, &ic_type, &version, s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_get_firmware_version / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
        
    }
    if (recovery < MAX_RECOVERY_COUNT) {
        TESTPR("o : rcs956_get_firmware_version\n");
    } else {
        ERROR_LOG(rc);
    }
    
#if defined DRV_UART
    /* do rcs956_set_dev_speed */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_set_dev_speed(&param->rcs956_dev, s_speed, s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_set_dev_speed / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (recovery < MAX_RECOVERY_COUNT) {
        TESTPR("o : rcs956_set_dev_speed\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do rcs956_set_speed */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_set_speed(&param->rcs956_dev, s_speed);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_set_speed / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (recovery < MAX_RECOVERY_COUNT) {
        TESTPR("o : rcs956_set_speed\n");
    } else {
        ERROR_LOG(rc);
    }
#endif

    /* do rcs956_rf_on */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_rf_on(&param->rcs956_dev, s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_rf_on / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (recovery < MAX_RECOVERY_COUNT) {
        TESTPR("o : rcs956_rf_on\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do felica_cc_polling */
    memset(param->card.pmm, 0xff, 8);
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = felica_cc_polling(&param->felica_dev,
                               parameter,
                               &param->card,
                               &card_option,
                               s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            if (0 != memcmp(param->card.pmm, "\xff\xff\xff\xff\xff\xff\xff\xff", 8)) {
                break;
            }
        }
        TESTPR("x : felica_cc_polling / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : felica_cc_polling\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do rcs956_set_rf_speed */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_set_rf_speed(&param->rcs956_dev, 0x01, s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_set_rf_speed / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : rcs956_set_rf_speed\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do felica_cc_request_service */
    timeout = FELICA_CC_CALC_PROC_TIME(param->card.pmm[2], param->num_of_nodes);
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = felica_cc_request_service(&param->felica_dev, 
                                       &param->card,
                                       param->num_of_nodes,
                                       param->node_code_list,
                                       param->node_key_version_list,
                                       timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : felica_cc_request_service / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : felica_cc_request_service\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do felica_cc_request_system_code */
    timeout = FELICA_CC_CALC_PROC_TIME(param->card.pmm[3], 0);
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = felica_cc_request_system_code(&param->felica_dev,
                                           &param->card,
                                           param->max_system_codes,
                                           &param->num_of_system_codes,
                                           param->system_code_list,
                                           timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : felica_cc_request_system_code / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : felica_cc_request_system_code\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do felica_cc_request_response */
    timeout = FELICA_CC_CALC_PROC_TIME(param->card.pmm[3], 0);
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = felica_cc_request_response(&param->felica_dev,
                                        &param->card,
                                        &mode,
                                        timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : felica_cc_request_response / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : felica_cc_request_response\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do felica_cc_read_without_encryption */
    timeout = FELICA_CC_CALC_PROC_TIME(param->card.pmm[5], param->num_of_blocks);
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = felica_cc_read_without_encryption(&param->felica_dev,
                                               &param->card,
                                               param->rw_num_of_service,
                                               param->rw_service_code_list,
                                               param->num_of_blocks,
                                               param->block_list,
                                               param->read_block_data,
                                               &status_flag1,
                                               &status_flag2,
                                               timeout);
        if ((ICS_ERROR_SUCCESS == rc) && (status_flag1 == 0) && (status_flag2 == 0) ) {
            break;
        }
        TESTPR("x : felica_cc_read_without_encryption / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        TESTPR("status_flag1:[%02x], status_flag2:[%02x]\n", status_flag1, status_flag2);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : felica_cc_read_without_encryption\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do felica_cc_write_without_encryption */
    num_of_blocks = 8;
    timeout = FELICA_CC_CALC_PROC_TIME(param->card.pmm[6], num_of_blocks);
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = felica_cc_write_without_encryption(&param->felica_dev,
                                                &param->card,
                                                param->rw_num_of_service,
                                                param->rw_service_code_list,
                                                num_of_blocks,
                                                param->block_list,
                                                param->write_block_data,
                                                &status_flag1,
                                                &status_flag2,
                                                timeout);
        if ((ICS_ERROR_SUCCESS == rc) && (status_flag1 == 0) && (status_flag2 == 0) ) {
            break;
        }
        TESTPR("x : felica_cc_write_without_encryption / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        TESTPR("status_flag1:[%02x], status_flag2:[%02x]\n", status_flag1, status_flag2);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : felica_cc_write_without_encryption\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do felica_cc_read_without_encryption(data verify) */
    timeout = FELICA_CC_CALC_PROC_TIME(param->card.pmm[5], param->num_of_blocks);
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
            rc = felica_cc_read_without_encryption(&param->felica_dev,
                                                   &param->card,
                                                   param->rw_num_of_service,
                                                   param->rw_service_code_list,
                                                   param->num_of_blocks,
                                                   param->block_list,
                                                   param->read_block_data,
                                                   &status_flag1,
                                                   &status_flag2,
                                                   timeout);
        if ((ICS_ERROR_SUCCESS == rc) && (status_flag1 == 0) && (status_flag2 == 0) ) {
            for (i = 0; i < (8 * 16); i++) {
                if (param->read_block_data[i] != (UINT8)i) {
                    printf("%d\n", i);
                    break;
                }
            }
            if (i == (8 * 16)) {
                break;
            }
        }
        TESTPR("x : felica_cc_read_without_encryption / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        TESTPR("status_flag1:[%02x], status_flag2:[%02x]\n", status_flag1, status_flag2);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : felica_cc_read_without_encryption(data verify)\n");
    } else {
        ERROR_LOG(rc);
    }

    /* do rcs956_reset */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_reset(&param->rcs956_dev, s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_reset / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : rcs956_reset\n");
    } else {
        ERROR_LOG(rc);
    }

#if defined DRV_UART
    /* do rcs956_power_down */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_power_down(&param->rcs956_dev, 0x10, s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_power_down / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : rcs956_power_down\n");
        utl_msleep(WAIT_POWER_DOWN);
    } else {
        ERROR_LOG(rc);
    }

    /* do rcs956_wake_up */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_wake_up(&param->rcs956_dev, s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_wake_up / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : rcs956_wake_up\n");
    } else {
        ERROR_LOG(rc);
    }

#endif
    /* do rcs956_rf_off */
    for (recovery = 0; recovery < MAX_RECOVERY_COUNT; recovery++) {
        rc = rcs956_rf_off(&param->rcs956_dev, s_timeout);
        if (ICS_ERROR_SUCCESS == rc) {
            break;
        }
        TESTPR("x : rcs956_rf_off / err : [%ld], Retry : [%d]\n", rc, recovery + 1);
        recovery_error(param);
    }
    if (ICS_ERROR_SUCCESS == rc) {
        TESTPR("o : rcs956_rf_off\n");
    } else {
        ERROR_LOG(rc);
    }
}


/**
 * This function is test program for reliability test
 * for reliability test.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_reliability(void)
{
    UINT32 rc = 0;
    static scenario_paramater_t param;
    UINT32 cnt = 0;
    INT32 rest_timeout;

    set_scenario_paramaters(&param);

    PAUSE_SET_RCS860();

    rc = do_testinitialize(&param);
    T_CHECK_EQ(rc, ICS_ERROR_SUCCESS);

    TESTPR("\ntest start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    /* test execution */
    do {
        cnt++;
        rest_timeout = (TOTALTIMESEC - ((utl_get_time_msec() - s_start_time) / 1000));
        TESTPR("\n*********************************************\n");
        TESTPR("The loop count [%d] / remaining [%ld] sec / Fail Count : [%d] / Retry : [%d]\n",
               cnt, rest_timeout, s_fail, s_recovery_cnt);
        TESTPR("*********************************************\n");
        utl_msleep(500);
        execute_scenario(&param);

    } while (rest_timeout > 0);
    
    TESTPR("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< test end\n");
    TESTPR("Total Fail Count = %d\n", s_fail);
    TESTPR("Total Recovery Count = %d\n", s_recovery_cnt);
    TESTPR("Total Loop Count = %d\n", cnt);

    do_testterminate(&param);

    return rc;
}

/**
 * This function displays the Help menu.
 *
 * \param argv0               [IN] Input character string from command line.
 */
static void print_usage(
    const char* argv0)
{
    TESTPR("Usage: %s [options] testcases...\n", argv0);
    TESTPR("  -p port\n");
    TESTPR("  -b UART speed\n");
    TESTPR("  -i initial UART speed\n");
    TESTPR("  -t timeout\n");

    return;
}

/**
 * It initiates the processing from this function. 
 *
 * \param argc          [IN] Input character string length from command line.
 * \param argv          [IN] Input character string from command line.
 *
 * \retval Number of Test Failures.
 */
int main(
    int argc,
    char* argv[])
{
    int i;
    UINT32 rc = 0;

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
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            i++;
            s_logfilename = argv[i];
        } else if (argv[i][0] == '-') {
            print_usage(argv[0]);
            return -1;
        } else {
            break;
        }
    }

    s_fp = fopen(s_logfilename, "a+");
    if (s_fp == NULL) {
        return -1;
    }

    rc = te_reliability();

    fclose(s_fp);

    return rc;
}
