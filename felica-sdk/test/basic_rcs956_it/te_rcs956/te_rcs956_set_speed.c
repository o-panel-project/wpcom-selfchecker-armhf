/**
 * \brief    Test Program for rcs956_set_speed
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_set_speed(void);

/**
 * This function is test program
 * for rcs956_set_speed.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_rcs956_set_speed(void)
{
#if defined(DRV_UART)
    UINT rc;
    UINT32 timeout;

    /* function number */
    g_funcnum = JTNO_RCS956_SET_SPEED;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1600N */
    g_testnum = 0;

    TESTPR("TSJ01-1600N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), 9600);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1601N */
    g_testnum = 1;

    TESTPR("TSJ01-1601N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 19200, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, 19200);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), 19200);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1602N */
    g_testnum = 2;

    TESTPR("TSJ01-1602N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 38400, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, 38400);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), 38400);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1603N */
    g_testnum = 3;

    TESTPR("TSJ01-1603N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 57600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, 57600);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), 57600);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1604N */
    g_testnum = 4;

    TESTPR("TSJ01-1604N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), g_speed);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */


#if 0
    /*********************************/
    /* TSJ01-1605N */
    g_testnum = 5;

    TESTPR("TSJ01-1605N ...\n");

    /* build condition */

    /* set parameters */

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, 230400);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), 230400);

    /* cleanup */

    /*********************************/
    /* TSJ01-1606N */
    g_testnum = 6;

    TESTPR("TSJ01-1606N ...\n");

    /* build condition */

    /* set parameters */

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, 460800);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), 460800);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#endif

    /*********************************/
    /* TSJ01-1607N */
    g_testnum = 7;

    TESTPR("TSJ01-1607N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), g_speed);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1608N */
    g_testnum = 8;

    TESTPR("TSJ01-1608N ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), 9600);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1609E */
    g_testnum = 9;

    TESTPR("TSJ01-1609E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1610E */
    g_testnum = 10;

    TESTPR("TSJ01-1610E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    g_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();
#endif
    return (0);
}
