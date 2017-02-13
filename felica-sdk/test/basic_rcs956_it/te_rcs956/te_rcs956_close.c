/**
 * \brief    Test Program for rcs956_close
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_close(void);

/**
 * This function is test program
 * for rcs956_close.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_close(void)
{
    UINT rc;
    ICS_HANDLE handle;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_CLOSE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0300N */
    g_testnum = 0;

    TESTPR("TSJ01-0300N ...\n");

    /* build condition */

    /* set parameters */
    handle = (ICS_HANDLE)-1;

    /* test execution */
    rc = rcs956_close(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(TRUE, (handle == g_rcs956_dev.handle));

    /* cleanup */

    /*********************************/
    /* TSJ01-0301E */
    g_testnum = 1;

    TESTPR("TSJ01-0301E ...\n");

    /* build condition */
    rc = rcs956_initialize(&g_rcs956_dev, &icsdrv_uart_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_close(&g_rcs956_dev);

    /* results judgement */

#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#else
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif

    /* cleanup */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0302E */
    g_testnum = 2;

    TESTPR("TSJ01-0302E ...\n");

    /* build condition */

    /* set parameters */
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_close(&bad_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_close(void)
{
    UINT rc;
    ICS_HANDLE handle;

    /* function number */
    g_funcnum = JTNO_RCS956_CLOSE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0300N */
    g_testnum = 0;

    TESTPR("TSJ02-0300N ...\n");

    /* build condition */

    /* set parameters */
    handle = (ICS_HANDLE)-1;

    /* test execution */
    rc = rcs956_close(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(TRUE, (handle == g_rcs956_dev.handle));

    /* cleanup */

    /*********************************/
    /* TSJ02-0301E */
    g_testnum = 1;

    TESTPR("TSJ02-0301E ...\n");

    /* build condition */
    rc = rcs956_initialize(&g_rcs956_dev, &rcs956_usb_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_close(&g_rcs956_dev);

    /* results judgement */

#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#elif defined(ARCH_WINXP)
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#elif defined(ARCH_WINCE)
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
