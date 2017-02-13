/**
 * \brief    Test Program for rcs956_open
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_open(void);

/**
 * This function is test program
 * for rcs956_open.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_open(void)
{
    ICS_HW_DEVICE rcs956;
    ICS_HW_DEVICE bad_rcs956;
    UINT rc;
    const char* port_name;

    /* function number */
    g_funcnum = JTNO_RCS956_OPEN;

    /* initialize */
    rc = rcs956_initialize(&g_rcs956_dev, &icsdrv_uart_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_initialize(&rcs956, &icsdrv_uart_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    /*********************************/
    /* TSJ01-0100N */
    g_testnum = 0;

    TESTPR("TSJ01-0100N ...\n");

    /* build condition */

    /* set parameters */
    port_name = g_port_name;

    /* test execution */
    rc = rcs956_open(&g_rcs956_dev, port_name);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), g_speed);
    T_CHECK_EQ(g_rcs956_dev.priv_value, 0);

    /* cleanup */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0101N */
    g_testnum = 1;

    TESTPR("TSJ01-0101N ...\n");

    /* build condition */
    port_name = g_port2_name;
    rc = rcs956_open(&rcs956, port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    port_name = g_port_name;

    /* test execution */
    rc = rcs956_open(&g_rcs956_dev, port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), g_speed);
    T_CHECK_EQ(g_rcs956_dev.priv_value, 0);
    T_CHECK_EQ(TRUE, (g_rcs956_dev.handle != rcs956.handle));

    /* cleanup */
    rc = rcs956_close(&rcs956);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0102E */
    g_testnum = 2;

    TESTPR("TSJ01-0102E ...\n");

    /* build condition */

    /* set parameters */
    port_name = "ABCD";

    /* test execution */
    rc = rcs956_open(&g_rcs956_dev, port_name);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-0103E */
    g_testnum = 3;

    TESTPR("TSJ01-0103E ...\n");

    /* build condition */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    port_name = g_port_name;
    utl_memcpy(&bad_rcs956, &g_rcs956_dev, sizeof(g_rcs956_dev));

    /* test execution */
    rc = rcs956_open(&bad_rcs956, port_name);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#elif defined(ARCH_WINXP)
    T_CHECK_EQ(ICS_ERROR_PERMISSION, rc);
#elif defined(ARCH_WINCE)
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0104E */
    g_testnum = 4;

        TESTPR("TSJ01-0104E ...\n");

    /* build condition */

    /* set parameters */
    port_name = "";

    utl_memcpy(&bad_rcs956, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956.priv_data = NULL;

    /* test execution */
    rc = rcs956_open(&bad_rcs956, port_name);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_open(void)
{
    ICS_HW_DEVICE rcs956;
    ICS_HW_DEVICE bad_rcs956;
    UINT rc;
    const char* port_name;

    /* function number */
    g_funcnum = JTNO_RCS956_OPEN;

    /* initialize */
    rc = rcs956_initialize(&g_rcs956_dev, &rcs956_usb_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_initialize(&rcs956, &rcs956_usb_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0100N */
    g_testnum = 0;

    TESTPR("TSJ02-0100N ...\n");

    /* build condition */

    /* set parameters */
    port_name = g_port_name;

    /* test execution */
    rc = rcs956_open(&g_rcs956_dev, port_name);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), g_speed);
    T_CHECK_EQ(g_rcs956_dev.priv_value, 0);

    /* cleanup */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0101N */
    g_testnum = 1;

    TESTPR("TSJ02-0101N ...\n");

    /* build condition */
    port_name = g_port2_name;
    rc = rcs956_open(&rcs956, port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    port_name = g_port_name;

    /* test execution */
    rc = rcs956_open(&g_rcs956_dev, port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), g_speed);
    T_CHECK_EQ(g_rcs956_dev.priv_value, 0);
    T_CHECK_EQ(TRUE, (g_rcs956_dev.handle != rcs956.handle));

    /* cleanup */
    rc = rcs956_close(&rcs956);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0102N */
    g_testnum = 2;

    TESTPR("TSJ02-0102E ...\n");

    /* build condition */

    /* set parameters */
    port_name = "ABCD";

    /* test execution */
    rc = rcs956_open(&g_rcs956_dev, port_name);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-0103E */
    g_testnum = 3;

    TESTPR("TSJ02-0103E ...\n");

    /* build condition */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    port_name = g_port_name;
    utl_memcpy(&bad_rcs956, &g_rcs956_dev, sizeof(g_rcs956_dev));

    /* test execution */
    rc = rcs956_open(&bad_rcs956, port_name);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#else
    T_CHECK_EQ(ICS_ERROR_PERMISSION, rc);
#endif

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
