/**
 * \brief    Test Program for rcs956_reset
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_reset(void);

/**
 * This function is test program
 * for rcs956_reset.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_reset(void)
{
    UINT rc;
    UINT32 timeout;
    felica_card_t card;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_RESET;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1400N */
    g_testnum = 0;

    TESTPR("TSJ01-1400N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
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
    /* TSJ01-1401N */
    g_testnum = 1;

    TESTPR("TSJ01-1401N ...\n");

    /* build condition */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
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
    /* TSJ01-1402N */
    g_testnum = 2;

    TESTPR("TSJ01-1402N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
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
    /* TSJ01-1403E */
    g_testnum = 3;

    TESTPR("TSJ01-1403N ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1404E */
    g_testnum = 4;

    TESTPR("TSJ01-1404E ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1405E */
    g_testnum = 5;

    TESTPR("TSJ01-1405E ...\n");

    /* build condition */
    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1406E */
    g_testnum = 6;

    TESTPR("TSJ01-1406E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif
    /* cleanup */

    /*********************************/
    /* TSJ01-1407E */
    g_testnum = 7;

    TESTPR("TSJ01-1407E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_reset(&bad_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_reset(void)
{
    UINT rc;
    UINT32 timeout;
    felica_card_t card;

    /* function number */
    g_funcnum = JTNO_RCS956_RESET;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1200N */
    g_testnum = 0;

    TESTPR("TSJ02-1200N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-1201N */
    g_testnum = 1;

    TESTPR("TSJ02-1201N ...\n");

    /* build condition */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-1202N */
    g_testnum = 2;

    TESTPR("TSJ02-1202N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

#if 0

    /*********************************/
    /* TSJ02-1203E */
    g_testnum = 3;

    TESTPR("TSJ02-1203E ...\n");

    /* build condition */
    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#endif

    /*********************************/
    /* TSJ02-1204E */
    g_testnum = 4;

    TESTPR("TSJ02-1204E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1205E */
    g_testnum = 5;

    TESTPR("TSJ02-1205E ...\n");

    /* build condition */

    /* set parameters */
    timeout = 0;

    /* test execution */
    rc = rcs956_reset(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
