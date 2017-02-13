/**
 * \brief    Test Program for rcs956_power_down
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_power_down(void);

/**
 * This function is test program
 * for rcs956_power_down.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_power_down(void)
{
    UINT32 timeout;
    UINT rc;
    felica_card_t card;
    UINT8 mode;

    /* function number */
    g_funcnum = JTNO_RCS956_POWER_DOWN;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2000N */
    g_testnum = 0;

    TESTPR("TSJ01-2000N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x08, timeout);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_wait_for_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    PAUSE_SET_EXTERNAL_RF();
    PAUSE_RELEASE_RF();

    rc = rcs956_wait_for_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2001N */
    g_testnum = 1;

    TESTPR("TSJ01-2001N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_EXTERNAL_RF();
    PAUSE_RELEASE_RF();

    rc = rcs956_wait_for_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2002N */
    g_testnum = 2;

    TESTPR("TSJ01-2002N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x18, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2003N */
    g_testnum = 3;

    TESTPR("TSJ01-2003N ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x08, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    PAUSE_SET_EXTERNAL_RF();
    PAUSE_RELEASE_RF();

    rc = rcs956_wait_for_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-2004N */
    g_testnum = 4;

    TESTPR("TSJ01-2004N ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_EXTERNAL_RF();
    PAUSE_RELEASE_RF();

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_NE(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2005N */
    g_testnum = 5;

    TESTPR("TSJ01-2005N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x18, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_EXTERNAL_RF();
    PAUSE_RELEASE_RF();

    rc = rcs956_wait_for_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2006N */
    g_testnum = 6;

    TESTPR("TSJ01-2006N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2007E */
    g_testnum = 7;

    TESTPR("TSJ01-2007E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2008E */
    g_testnum = 8;

    TESTPR("TSJ01-2008E ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-2009E */
    g_testnum = 9;

    TESTPR("TSJ01-2009E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */

    /*********************************/
    /* TSJ01-2010E */
    g_testnum = 10;

    TESTPR("TSJ01-2010E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    g_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_power_down(void)
{
    UINT32 timeout;
    UINT32 time0;
    UINT rc;
    UINT8 buf[10];
    /* function number */
    g_funcnum = JTNO_RCS956_POWER_DOWN;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1700N */
    g_testnum = 0;

    TESTPR("TSJ02-1700N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    time0 = utl_get_time_msec();

    rc = rcs956_usb_raw_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                             NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    T_CHECK_EQ(0, utl_memcmp(buf, "\x00\x00\xff\x03\xfd\xd5\x17\x26\xee\x00", 10));

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
