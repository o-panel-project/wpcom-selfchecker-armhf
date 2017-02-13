/**
 * \brief    Test Program for rcs956_set_dev_speed
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_set_dev_speed(void);

/**
 * This function is test program
 * for rcs956_set_dev_speed.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_set_dev_speed(void)
{
    UINT32 timeout;
    UINT rc;
    felica_card_t card;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_SET_DEV_SPEED;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1500N */
    g_testnum = 0;

    TESTPR("TSJ01-1500N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1501N */
    g_testnum = 1;

    TESTPR("TSJ01-1501N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 19200, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 19200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1502N */
    g_testnum = 2;

    TESTPR("TSJ01-1502N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 38400, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 38400);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1503N */
    g_testnum = 3;

    TESTPR("TSJ01-1503N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 57600, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 57600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1504N */
    g_testnum = 4;

    TESTPR("TSJ01-1504N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);


    /*********************************/
    /* TSJ01-1505N */
    g_testnum = 5;

    TESTPR("TSJ01-1505N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 230400, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    PAUSE_RW_REACTIVATE()

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1506N */
    g_testnum = 6;

    TESTPR("TSJ01-1506N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 460800, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    PAUSE_RW_REACTIVATE()

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1507N */
    g_testnum = 7;

    TESTPR("TSJ01-1507N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1508N */
    g_testnum = 8;

    TESTPR("TSJ01-1508N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1509N */
    g_testnum = 9;

    TESTPR("TSJ01-1509N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1510E */
    g_testnum = 10;

    TESTPR("TSJ01-1510E ...\n");

    /* build condition */
    timeout = g_timeout;
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1511E */
    g_testnum = 11;

    TESTPR("TSJ01-1511E ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1512E */
    g_testnum = 12;

    TESTPR("TSJ01-1512E ...\n");

    /* build condition */
    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1513E */
    g_testnum = 13;

    TESTPR("TSJ01-1513E ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1514E */
    g_testnum = 14;

    TESTPR("TSJ01-1514E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */

    /*********************************/
    /* TSJ01-1515E */
    g_testnum = 15;

    TESTPR("TSJ01-1515E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_set_dev_speed(&bad_rcs956_dev, g_speed, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_set_dev_speed(void)
{
    UINT32 timeout;
    UINT rc;

    /* function number */
    g_funcnum = JTNO_RCS956_SET_DEV_SPEED;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1300E */
    g_testnum = 0;

    TESTPR("TSJ02-1300E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 115200, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
