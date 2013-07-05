/**
 * \brief    Test Program for rcs956_get_ack_time
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_get_ack_time(void);

/**
 * This function is test program
 * for rcs956_get_ack_time.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_get_ack_time(void)
{
    UINT32 ack_time;
    UINT32 ack_time_esc;
    UINT rc;

    /* function number */
    g_funcnum = JTNO_RCS956_GET_ACK_TIME;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1800N */
    g_testnum = 0;

    TESTPR("TSJ01-1800N...\n");

    /* build condition */
    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    ack_time_esc = g_rcs956_dev.priv_value;

    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_get_ack_time(&g_rcs956_dev, &ack_time);

    /* results judgement */
    T_CHECK_EQ(ack_time, ack_time_esc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    T_CHECK_NE(g_rcs956_dev.priv_value, ack_time_esc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1801N */
    g_testnum = 1;

    TESTPR("TSJ01-1801N ...\n");

    /* build condition */
    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_get_ack_time(&g_rcs956_dev, &ack_time_esc);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(100);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_get_ack_time(&g_rcs956_dev, &ack_time);

    /* results judgement */
    T_CHECK_EQ(TRUE, (ack_time != ack_time_esc));

    /* cleanup */
    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_get_ack_time(void)
{
    UINT32 ack_time;
    UINT32 ack_time_esc;
    UINT rc;

    /* function number */
    g_funcnum = JTNO_RCS956_GET_ACK_TIME;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1500N */
    g_testnum = 0;

    TESTPR("TSJ02-1500N...\n");

    /* build condition */
    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    ack_time_esc = g_rcs956_dev.priv_value;

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_get_ack_time(&g_rcs956_dev, &ack_time);

    /* results judgement */
    T_CHECK_NE(ack_time, ack_time_esc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
