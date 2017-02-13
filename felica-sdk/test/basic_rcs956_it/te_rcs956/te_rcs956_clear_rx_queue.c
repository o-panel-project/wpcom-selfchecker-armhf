/**
 * \brief    Test Program for rcs956_clear_rx_queue
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_clear_rx_queue(void);

/**
 * This function is test program
 * for rcs956_clear_rx_queue.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_clear_rx_queue(void)
{
    UINT rc;
    UINT32 time0;
    UINT8 buf[1];
    ICS_HW_DEVICE bad_rcs956;

    /* function number */
    g_funcnum = JTNO_RCS956_CLEAR_RX_QUEUE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1700N */
    g_testnum = 0;

    TESTPR("TSJ01-1700N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_clear_rx_queue(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    time0 = utl_get_time_msec();

    rc = icsdrv_uart_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                          NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1701N */
    g_testnum = 1;

    TESTPR("TSJ01-1701N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_clear_rx_queue(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    time0 = utl_get_time_msec();

    rc = icsdrv_uart_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                          NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1702E */
    g_testnum = 2;

    TESTPR("TSJ01-1702E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_clear_rx_queue(&g_rcs956_dev);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif

    /* cleanup */

    /*********************************/
    /* TSJ01-1703E */
    g_testnum = 3;

    TESTPR("TSJ01-1703E ...\n");

    /* build condition */

    /* set parameters */
    utl_memcpy(&bad_rcs956, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956.priv_data = NULL;

    /* test execution */
    rc = rcs956_clear_rx_queue(&bad_rcs956);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_clear_rx_queue(void)
{
    UINT rc;

    /* function number */
    g_funcnum = JTNO_RCS956_CLEAR_RX_QUEUE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1400N */
    g_testnum = 0;

    TESTPR("TSJ02-1400N ...\n");

    /* build condition */

    /* set parameters */

    /* test execution */
    rc = rcs956_clear_rx_queue(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-1401E */
    g_testnum = 1;

    TESTPR("TSJ02-1401E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_clear_rx_queue(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
