/**
 * \brief    Test Program for rcs956_uart_initialize_device
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

#if defined(DRV_UART)
UINT32 te_rcs956_uart_initialize_device(void);

/**
 * This function is test program
 * for rcs956_uart_initialize_device.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_rcs956_uart_initialize_device(void)
{
    UINT32 timeout;
    UINT rc;
    felica_card_t card;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_UART_INITIALIZE_DEVICE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0500N */
    g_testnum = 0;

    TESTPR("TSJ01-0500N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);

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
    /* TSJ01-0501N */
    g_testnum = 1;

    TESTPR("TSJ01-0501N ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);

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
    /* TSJ01-0502N */
    g_testnum = 2;

    TESTPR("TSJ01-0502N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);

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
    /* TSJ01-0503N */
    g_testnum = 3;

    TESTPR("TSJ01-0503N ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);

    /* results judgement */
    if (rc == ICS_ERROR_INVALID_RESPONSE) {
        rc = rcs956_cancel_command(&g_rcs956_dev);
        T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

        rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);
    }
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0504E */
    g_testnum = 4;

    TESTPR("TSJ01-0504E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0505E */
    g_testnum = 5;

    TESTPR("TSJ01-0505E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif
    /* cleanup */

    /*********************************/
    /* TSJ01-0506E */
    g_testnum = 6;

    TESTPR("TSJ01-0506E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    g_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
