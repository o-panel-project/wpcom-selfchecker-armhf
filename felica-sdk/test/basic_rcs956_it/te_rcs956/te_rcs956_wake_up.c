/**
 * \brief    Test Program for rcs956_wake_up
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_wake_up(void);

/**
 * This function is test program
 * for rcs956_wake_up.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_rcs956_wake_up(void)
{
#if defined(DRV_UART)
    UINT32 timeout;
    UINT rc;

    /* function number */
    g_funcnum = JTNO_RCS956_WAKE_UP;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0900N */
    g_testnum = 0;

    TESTPR("TSJ01-0900N ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_wake_up(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-0901N */
    g_testnum = 1;

    TESTPR("TSJ01-0901N ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x08, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_wake_up(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_EXTERNAL_RF();
    PAUSE_RELEASE_RF();

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-0902N */
    g_testnum = 2;

    TESTPR("TSJ01-0902N ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x18, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_wake_up(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-0903N */
    g_testnum = 3;

    TESTPR("TSJ01-0903N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_wake_up(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-9004E */
    g_testnum = 4;

    TESTPR("TSJ01-0904E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_wake_up(&g_rcs956_dev, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */

    /*********************************/
    /* TSJ01-9005E */
    g_testnum = 5;

    TESTPR("TSJ01-0905E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    g_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_wake_up(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();
#endif
    return (0);
}
