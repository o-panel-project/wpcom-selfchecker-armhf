/**
 * \brief    Test Program for rcs956_set_rf_speed
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_set_rf_speed(void);

/**
 * This function is test program
 * for rcs956_set_rf_speed.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_set_rf_speed(void)
{
    UINT32 timeout;
    UINT rc;
    felica_card_t card;
    UINT8 mode;
    UINT8 polling_param[4];
    felica_card_option_t card_option;
    felica_cc_devf_t thru_devf;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_SET_RF_SPEED;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_stub_rcs956_initialize(&thru_devf, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;

    thru_devf.polling_func = NULL;

    /*********************************/
    /* TSJ01-2100N */
    g_testnum = 0;

    TESTPR("TSJ01-2100N ...\n");
    PAUSE_SET_SDB200();

    /* build condition */
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();
    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-2101N */
    g_testnum = 1;

    TESTPR("TSJ01-2101N ...\n");
    PAUSE_SET_RCX407();

    /* build condition */
    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_SDB200();
    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-2102E */
    g_testnum = 2;

    TESTPR("TSJ01-2102E ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_SDB200();
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-2103N */
    g_testnum = 3;

    TESTPR("TSJ01-2103N ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-2104N */
    g_testnum = 4;

    TESTPR("TSJ01-2104N ...\n");
    PAUSE_SET_SDB200();

    /* build condition */
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-2105E */
    g_testnum = 5;

    TESTPR("TSJ01-2105E ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 9600);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);

    /*********************************/
    /* TSJ01-2106E */
    g_testnum = 6;

    TESTPR("TSJ01-2106E ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-2107E */
    g_testnum = 7;

    TESTPR("TSJ01-2107E ...\n");

    /* build condition */
    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2108E */
    g_testnum = 8;

    TESTPR("TSJ01-2108E ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-2109E */
    g_testnum = 9;

    TESTPR("TSJ01-2109E ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif
    /* cleanup */

    /*********************************/
    /* TSJ01-2110E */
    g_testnum = 10;

    TESTPR("TSJ01-2110E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_set_rf_speed(&bad_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_set_rf_speed(void)
{
    UINT32 timeout;
    UINT rc;
    felica_card_t card;
    UINT8 mode;
    UINT8 polling_param[4];
    felica_card_option_t card_option;
    felica_cc_devf_t thru_devf;

    /* function number */
    g_funcnum = JTNO_RCS956_SET_RF_SPEED;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_stub_rcs956_initialize(&thru_devf, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    thru_devf.polling_func = NULL;

    /*********************************/
    /* TSJ02-1800N */
    g_testnum = 0;

    TESTPR("TSJ02-1800N ...\n");
    PAUSE_SET_SDB200();

    /* build condition */
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();
    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-1801N */
    g_testnum = 1;

    TESTPR("TSJ02-1801N ...\n");
    PAUSE_SET_RCX407();

    /* build condition */
    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_SDB200();
    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-1802N */
    g_testnum = 2;

    TESTPR("TSJ02-1802N ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_SDB200();
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();
    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-1803N */
    g_testnum = 3;

    TESTPR("TSJ02-1803N ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-1804N */
    g_testnum = 4;

    TESTPR("TSJ02-1804N ...\n");
    PAUSE_SET_SDB200();

    /* build condition */
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_polling(&thru_devf, polling_param,
                           &card, &card_option, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

#if 0

    /*********************************/
    /* TSJ02-1805E */
    g_testnum = 5;

    TESTPR("TSJ02-1805E ...\n");

    /* build condition */
    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#endif

    /*********************************/
    /* TSJ02-1806E */
    g_testnum = 6;

    TESTPR("TSJ02-1806E ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-1807E */
    g_testnum = 7;

    TESTPR("TSJ02-1807E ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1808E */
    g_testnum = 8;

    TESTPR("TSJ02-1808E ...\n");

    /* build condition */

    /* set parameters */
    timeout = 0;

    /* test execution */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
