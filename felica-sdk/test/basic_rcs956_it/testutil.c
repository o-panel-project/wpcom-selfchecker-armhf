/**
 * \brief    The Utility Program for Test Program
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

/**
 * This function is initialization done before it tests.
 *
 * \retval Please look at ics_error.h.
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
 * When the test execution ends, this function is called. 
 */
void do_testterminate(void)
{
    rcs956_close(&g_rcs956_dev);
}


/**
 * To do RCS860 in polling, this function is called. 
 *
 * \param card            [OUT] card information.
 *
 * \retval Please look at ics_error.h.
 */
UINT32 do_polling_to_rcs860(felica_card_t *card)
{
    UINT32 rc;
    felica_card_option_t card_option;
    UINT8 parameter[4];

    parameter[0] = 0x00;
    parameter[1] = 0x18;
    parameter[2] = 0x00;
    parameter[3] = 0x00;

    rc = felica_cc_polling(&g_felica_dev,
                           parameter,
                           card,
                           &card_option,
                           g_timeout);
    return rc;
}

/**
 * To do RCX407 in polling, this function is called. 
 *
 * \param card            [OUT] card information.
 *
 * \retval Please look at ics_error.h.
 */
UINT32 do_polling_to_rcx407(felica_card_t *card)
{
    UINT32 rc;
    felica_card_option_t card_option;
    UINT8 parameter[4];

    parameter[0] = 0x00;
    parameter[1] = 0x18;
    parameter[2] = 0x00;
    parameter[3] = 0x00;

    rc = felica_cc_polling(&g_felica_dev,
                           parameter,
                           card,
                           &card_option,
                           g_timeout);
    return rc;
}

/**
 * To do SDB200 in polling, this function is called. 
 *
 * \param card            [OUT] card information.
 *
 * \retval Please look at ics_error.h.
 */
UINT32 do_polling_to_sdb200(felica_card_t *card)
{
    UINT32 rc;
    felica_card_option_t card_option;
    UINT8 parameter[4];

    parameter[0] = 0x00;
    parameter[1] = 0x18;
    parameter[2] = 0x00;
    parameter[3] = 0x00;

    rc = felica_cc_polling(&g_felica_dev,
                           parameter,
                           card,
                           &card_option,
                           g_timeout);
    return rc;
}

#define TIMEOUTMARGIN    100

static UINT32 start_time = 0;

/**
 * When the time measurement is started, this function is called. 
 */
void measurement_timeout_start(void)
{
    start_time = utl_get_time_msec();
}

/**
 * To do SDB200 in polling, this function is called. 
 *
 * \param api_timeout         [INT] API timeout.
 * \param exe_time            [OUT] Function execution time.
 *
 * \retval TRUE (API timeout + 100) > (Function execution time).
 * \retval FALSE (API timeout + 100) <= (Function execution time).
 */
BOOL compare_timeout(
    UINT32 api_timeout,
    UINT32* exe_time)
{
    UINT32 end_time;
    UINT32 time;

    end_time = utl_get_time_msec();
    time = (UINT32)(end_time - start_time);

    if (exe_time != NULL) {
        *exe_time = time;
    }

    if (time < (UINT32)(api_timeout + TIMEOUTMARGIN)) {
        TESTPR("API timeout    : %d [ms]\n", api_timeout);
        TESTPR("Execution time : %d [ms]\n", time);
        return TRUE;
    }

    TESTPR("API timeout    : %d [ms]\n", api_timeout);
    TESTPR("Execution time : %d [ms]\n", time);

    return FALSE;
}

/**
 * This function transmits Diagnose Command(ACK is not received). 
 *
 * \retval Please look at ics_error.h.
 */
UINT32 send_diagnose_command(void)
{
    UINT32 timeout;
    UINT32 time0;
    UINT32 rc;
    UINT8 buf[10];

    time0 = utl_get_time_msec();
    timeout = 100;

    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0xff;
    buf[3] = 0x03;
    buf[4] = 0xfd;
    buf[5] = 0xd4;
    buf[6] = 0x00;
    buf[7] = 0x01;
    buf[8] = 0x2B;
    buf[9] = 0x00;

    rc = rcs956_reset(&g_rcs956_dev, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }

#if defined(DRV_UART)
    rc = icsdrv_uart_write(g_rcs956_dev.handle, buf, sizeof(buf),
                           time0, timeout);
#else
    rc = rcs956_usb_raw_write(g_rcs956_dev.handle, buf, sizeof(buf),
                              time0, timeout);
#endif
    return rc;
}

/**
 * This function transmits GetFirmwareVersion Command(ACK is not received). 
 *
 * \retval Please look at ics_error.h.
 */
UINT32 send_get_firmware_version_command(void)
{
    UINT32 timeout;
    UINT32 time0;
    UINT32 rc;
    UINT8 buf[9];

    time0 = utl_get_time_msec();
    timeout = 100;

    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0xff;
    buf[3] = 0x02;
    buf[4] = 0xfe;
    buf[5] = 0xd4;
    buf[6] = 0x02;
    buf[7] = 0x2a;
    buf[8] = 0x00;

#if defined(DRV_UART)
    rc = icsdrv_uart_write(g_rcs956_dev.handle, buf, 5 + 2 + 2,
                           time0, timeout);
#else
    rc = rcs956_usb_raw_write(g_rcs956_dev.handle, buf, 5 + 2 + 2,
                              time0, timeout);
#endif
    if (rc != ICS_ERROR_SUCCESS) {
        return rc;
    }
    
    rc = utl_msleep(100);
    return rc;
}

