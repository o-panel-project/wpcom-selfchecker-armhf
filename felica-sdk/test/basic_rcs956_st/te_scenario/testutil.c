/**
 * \brief    The Utility Program for Test Program
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

/**
 * This function is initialization done before it tests.
 */
UINT32 do_testinitialize(void)
{
    UINT32 rc;

#if defined(DRV_UART)
    rc = rcs956_initialize(&g_rcs956_dev, &icsdrv_uart_raw_func);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }
#else
    rc = rcs956_initialize(&g_rcs956_dev, &rcs956_usb_raw_func);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }
#endif

    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }

    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }

    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }

    return rc;
}

/**
 * This function is initialization done before it tests.
 */
void do_testterminate(void)
{
    utl_msleep(WAIT_BEFOR_CANCEL);
    rcs956_cancel_command(&g_rcs956_dev);
    rcs956_ping(&g_rcs956_dev, g_timeout);
    rcs956_reset(&g_rcs956_dev, g_timeout);
    rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    rcs956_close(&g_rcs956_dev);
}

#define POLLING_RETRY 10

UINT32 do_polling_to_rcs860(felica_card_t *card)
{
    UINT32 rc = 0, cnt = 0;
    felica_card_option_t card_option;
    UINT8 parameter[4];

    parameter[0] = 0x00;
    parameter[1] = 0x18;
    parameter[2] = 0x00;
    parameter[3] = 0x00;

    for (cnt = 0; cnt < POLLING_RETRY; cnt++) {
        rc = felica_cc_polling(&g_felica_dev,
                               parameter,
                               card,
                               &card_option,
                               g_timeout);
        if (rc == ICS_ERROR_SUCCESS) {
            break;
        }
    }

    return rc;
}

UINT32 do_polling_to_rcx407(felica_card_t *card)
{
    UINT32 rc = 0, cnt = 0;
    felica_card_option_t card_option;
    UINT8 parameter[4];

    parameter[0] = 0x00;
    parameter[1] = 0x18;
    parameter[2] = 0x00;
    parameter[3] = 0x00;

    for (cnt = 0; cnt < POLLING_RETRY; cnt++) {
        rc = felica_cc_polling(&g_felica_dev,
                               parameter,
                               card,
                               &card_option,
                               g_timeout);
        if (rc == ICS_ERROR_SUCCESS) {
            break;
        }
    }

    return rc;
}

UINT32 do_polling_to_sdb200(felica_card_t *card)
{
    UINT32 rc = 0, cnt = 0;
    felica_card_option_t card_option;
    UINT8 parameter[4];

    parameter[0] = 0x00;
    parameter[1] = 0x18;
    parameter[2] = 0x00;
    parameter[3] = 0x00;

    for (cnt = 0; cnt < POLLING_RETRY; cnt++) {
        rc = felica_cc_polling(&g_felica_dev,
                               parameter,
                               card,
                               &card_option,
                               g_timeout);
        if (rc == ICS_ERROR_SUCCESS) {
            break;
        }
    }

    return rc;
}

static UINT32 start_time = 0;

void measurement_timeout_start(void)
{
    start_time = utl_get_time_msec();
}

static UINT32 result[ICS_ERROR_DEVICE + 2];

void retry_result_init(void) {
    utl_memset(result, 0, sizeof(result));
}

void retry_recovery(void)
{
    rcs956_cancel_command(&g_rcs956_dev);
}

void retry_result_cntup(UINT32 rc)
{
    if (ICS_ERROR_DEVICE < rc) {
        result[ICS_ERROR_DEVICE + 1]++;
    } else {
        result[rc]++;
    }
}

void retry_result_print(void)
{
    UINT32 count;
    UINT8 idx;

    for (count = 0, idx = 0; idx < ICS_ERROR_DEVICE + 2; idx++) {
        count = count + result[idx];
    }

    TESTPR("=====================================\n");
    TESTPR("RETRY CNT : %d\n", count );
    TESTPR("-------------------------------------\n");
    if ( result[ICS_ERROR_SUCCESS] != 0 ) {
        TESTPR("ICS_ERROR_SUCCESS           : %d\n",
               result[ICS_ERROR_SUCCESS]);
    }
    if ( result[ICS_ERROR_NOT_SUPPORTED] != 0 ) {
        TESTPR("ICS_ERROR_NOT_SUPPORTED     : %d\n",
               result[ICS_ERROR_NOT_SUPPORTED]);
    }
    if ( result[ICS_ERROR_NOT_IMPLEMENTED] != 0 ) {
        TESTPR("ICS_ERROR_NOT_IMPLEMENTED   : %d\n",
               result[ICS_ERROR_NOT_IMPLEMENTED]);
    }
    if ( result[ICS_ERROR_NOT_INITIALIZED] != 0 ) {
        TESTPR("ICS_ERROR_NOT_INITIALIZED   : %d\n",
                result[ICS_ERROR_NOT_INITIALIZED]);
    }
    if ( result[ICS_ERROR_NOT_OPENED] != 0 ) {
        TESTPR("ICS_ERROR_NOT_OPENED        : %d\n",
               result[ICS_ERROR_NOT_OPENED]);
    }
    if ( result[ICS_ERROR_ALREADY_OPENED] != 0 ) {
        TESTPR("ICS_ERROR_ALREADY_OPENED    : %d\n",
                result[ICS_ERROR_ALREADY_OPENED]);
    }
    if ( result[ICS_ERROR_INVALID_PARAM] != 0 ) {
        TESTPR("ICS_ERROR_INVALID_PARAM     : %d\n",
               result[ICS_ERROR_INVALID_PARAM]);
    }
    if ( result[ICS_ERROR_ILLEGAL_MODE] != 0 ) {
        TESTPR("ICS_ERROR_ILLEGAL_MODE      : %d\n",
               result[ICS_ERROR_ILLEGAL_MODE]);
    }
    if ( result[ICS_ERROR_FATAL] != 0 ) {
        TESTPR("ICS_ERROR_FATAL             : %d\n",
                result[ICS_ERROR_FATAL]);
    }
    if ( result[ICS_ERROR_IO] != 0 ) {
        TESTPR("ICS_ERROR_IO                : %d\n",
                result[ICS_ERROR_IO]);
    }
    if ( result[ICS_ERROR_NO_RESOURCES] != 0 ) {
        TESTPR("ICS_ERROR_NO_RESOURCES      : %d\n",
                result[ICS_ERROR_NO_RESOURCES]);
    }
    if ( result[ICS_ERROR_BUSY] != 0 ) {
        TESTPR("ICS_ERROR_BUSY              : %d\n",
               result[ICS_ERROR_BUSY]);
    }
    if ( result[ICS_ERROR_PERMISSION] != 0 ) {
        TESTPR("ICS_ERROR_PERMISSION        : %d\n",
               result[ICS_ERROR_PERMISSION]);
    }
    if ( result[ICS_ERROR_TIMEOUT] != 0 ) {
        TESTPR("ICS_ERROR_TIMEOUT           : %d\n",
               result[ICS_ERROR_TIMEOUT]);
    }
    if ( result[ICS_ERROR_FRAME_CRC] != 0 ) {
        TESTPR("ICS_ERROR_FRAME_CRC         : %d\n",
               result[ICS_ERROR_FRAME_CRC]);
    }
    if ( result[ICS_ERROR_INVALID_RESPONSE] != 0 ) {
        TESTPR("ICS_ERROR_INVALID_RESPONSE  : %d\n",
               result[ICS_ERROR_INVALID_RESPONSE]);
    }
    if ( result[ICS_ERROR_SYNTAX] != 0 ) {
        TESTPR("ICS_ERROR_SYNTAX            : %d\n",
               result[ICS_ERROR_SYNTAX]);
    }
    if ( result[ICS_ERROR_BUF_OVERFLOW] != 0 ) {
        TESTPR("ICS_ERROR_BUF_OVERFLOW      : %d\n",
               result[ICS_ERROR_BUF_OVERFLOW]);
    }
    if ( result[ICS_ERROR_DATA_TRANS_START] != 0 ) {
        TESTPR("ICS_ERROR_DATA_TRANS_START  : %d\n",
               result[ICS_ERROR_DATA_TRANS_START]);
    }
    if ( result[ICS_ERROR_DATA_TRANS_END] != 0 ) {
        TESTPR("ICS_ERROR_DATA_TRANS_END    : %d\n",
               result[ICS_ERROR_DATA_TRANS_END]);
    }
    if ( result[ICS_ERROR_NOT_STARTED] != 0 ) {
        TESTPR("ICS_ERROR_NOT_STARTED       : %d\n",
               result[ICS_ERROR_NOT_STARTED]);
    }
    if ( result[ICS_ERROR_ALREADY_STARTED] != 0 ) {
        TESTPR("ICS_ERROR_ALREADY_STARTED   : %d\n",
               result[ICS_ERROR_ALREADY_STARTED]);
    }
    if ( result[ICS_ERROR_SEQUENCE] != 0 ) {
        TESTPR("ICS_ERROR_SEQUENCE          : %d\n",
               result[ICS_ERROR_SEQUENCE]);
    }
    if ( result[ICS_ERROR_DESELECTED] != 0 ) {
        TESTPR("ICS_ERROR_DESELECTED        : %d\n",
               result[ICS_ERROR_DESELECTED]);
    }
    if ( result[ICS_ERROR_RELEASED] != 0 ) {
        TESTPR("ICS_ERROR_RELEASED          : %d\n",
               result[ICS_ERROR_RELEASED]);
    }
    if ( result[ICS_ERROR_RF_OFF] != 0 ) {
        TESTPR("ICS_ERROR_RF_OFF            : %d\n",
               result[ICS_ERROR_RF_OFF]);
    }
    if ( result[ICS_ERROR_NOT_EXIST] != 0 ) {
        TESTPR("ICS_ERROR_NOT_EXIST         : %d\n",
               result[ICS_ERROR_NOT_EXIST]);
    }
    if ( result[ICS_ERROR_ALREADY_EXIST] != 0 ) {
        TESTPR("ICS_ERROR_ALREADY_EXIST     : %d\n",
               result[ICS_ERROR_ALREADY_EXIST]);
    }
    if ( result[ICS_ERROR_IGNORE] != 0 ) {
        TESTPR("ICS_ERROR_IGNORE            : %d\n",
               result[ICS_ERROR_IGNORE]);
    }
    if ( result[ICS_ERROR_STATUS_FLAG1] != 0 ) {
        TESTPR("ICS_ERROR_STATUS_FLAG1      : %d\n",
               result[ICS_ERROR_STATUS_FLAG1]);
    }
    if ( result[ICS_ERROR_STATUS_FLAG] != 0 ) {
        TESTPR("ICS_ERROR_STATUS_FLAG       : %d\n",
               result[ICS_ERROR_STATUS_FLAG]);
    }
    if ( result[ICS_ERROR_SN_OVERFLOW] != 0 ) {
        TESTPR("ICS_ERROR_SN_OVERFLOW       : %d\n",
               result[ICS_ERROR_SN_OVERFLOW]);
    }
    if ( result[ICS_ERROR_INVALID_DATA] != 0 ) {
        TESTPR("ICS_ERROR_INVALID_DATA      : %d\n",
               result[ICS_ERROR_INVALID_DATA]);
    }
    if ( result[ICS_ERROR_DISCONNECTED] != 0 ) {
        TESTPR("ICS_ERROR_DISCONNECTED      : %d\n",
               result[ICS_ERROR_DISCONNECTED]);
    }
    if ( result[ICS_ERROR_SHUTDOWN] != 0 ) {
        TESTPR("ICS_ERROR_SHUTDOWN          : %d\n",
               result[ICS_ERROR_SHUTDOWN]);
    }
    if ( result[ICS_ERROR_MANY_ERRORS] != 0 ) {
        TESTPR("ICS_ERROR_MANY_ERRORS       : %d\n",
               result[ICS_ERROR_MANY_ERRORS]);
    }
    if ( result[ICS_ERROR_NOT_CONNECTED] != 0 ) {
        TESTPR("ICS_ERROR_NOT_CONNECTED     : %d\n",
               result[ICS_ERROR_NOT_CONNECTED]);
    }
    if ( result[ICS_ERROR_DEV_BUSY] != 0 ) {
        TESTPR("ICS_ERROR_DEV_BUSY          : %d\n",
               result[ICS_ERROR_DEV_BUSY]);
    }
    if ( result[ICS_ERROR_DEVICE] != 0 ) {
        TESTPR("ICS_ERROR_DEVICE            : %d\n",
               result[ICS_ERROR_DEVICE]);
    }
    if ( result[ICS_ERROR_DEVICE + 1] != 0 ) {
        TESTPR("UNKNOWN    ERROR!!!         : %d\n",
               result[ICS_ERROR_DEVICE + 1]);
    }
    TESTPR("=====================================\n");
}
