/**
 * \brief    Test Program for rcs956_get_firmware_version
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_get_firmware_version(void);

/**
 * This function is test program
 * for rcs956_get_firmware_version.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_get_firmware_version(void)
{
    UINT32 timeout;
    UINT32 time0;
    UINT rc;
    felica_card_t card;
    UINT8 ic_type;
    UINT16 version;
    UINT8 buf[11];
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_GET_FIRMWARE_VERSION;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1900N */
    g_testnum = 0;

    TESTPR("TSJ01-1900N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0x33, ic_type);
    T_CHECK_EQ(0x0130, version);

    /* cleanup */

    /*********************************/
    /* TSJ01-1901N */
    g_testnum = 1;

    TESTPR("TSJ01-1901N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0x33, ic_type);
    T_CHECK_EQ(0x0130, version);

    /* cleanup */

    /*********************************/
    /* TSJ01-1902N */
    g_testnum = 2;

    TESTPR("TSJ01-1902N ...\n");

    /* build condition */
    time0 = utl_get_time_msec();

    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0xff;
    buf[3] = 0x04;
    buf[4] = 0xfc;
    buf[5] = 0xd4;
    buf[6] = 0x32;
    buf[7] = 0x80;
    buf[8] = 0x01;
    buf[9] = 0x79;
    buf[10] = 0x00;

    rc = icsdrv_uart_write(g_rcs956_dev.handle, buf, sizeof(buf),
                           time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(100);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0x33, ic_type);
    T_CHECK_EQ(0x0130, version);

    /* cleanup */

    /*********************************/
    /* TSJ01-1903E */
    g_testnum = 3;

    TESTPR("TSJ01-1903N ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1904E */
    g_testnum = 4;

    TESTPR("TSJ01-1904N ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1905E */
    g_testnum = 5;

    TESTPR("TSJ01-1905E ...\n");

    /* build condition */
    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1906E */
    g_testnum = 6;

    TESTPR("TSJ01-1906E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif
    /* cleanup */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1907E */
    g_testnum = 7;

    TESTPR("TSJ01-1907E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_get_firmware_version(&bad_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_get_firmware_version(void)
{
    UINT32 timeout;
    UINT32 time0;
    UINT rc;
    felica_card_t card;
    UINT8 ic_type;
    UINT16 version;
    UINT8 buf[11];

    /* function number */
    g_funcnum = JTNO_RCS956_GET_FIRMWARE_VERSION;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1600N */
    g_testnum = 0;

    TESTPR("TSJ02-1600N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0x33, ic_type);
    T_CHECK_EQ(0x0130, version);

    /* cleanup */

    /*********************************/
    /* TSJ02-1601N */
    g_testnum = 1;

    TESTPR("TSJ02-1601N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0x33, ic_type);
    T_CHECK_EQ(0x0130, version);

    /* cleanup */

    /*********************************/
    /* TSJ02-1602N */
    g_testnum = 2;

    TESTPR("TSJ02-1602N ...\n");

    /* build condition */
    time0 = utl_get_time_msec();

    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0xff;
    buf[3] = 0x04;
    buf[4] = 0xfc;
    buf[5] = 0xd4;
    buf[6] = 0x32;
    buf[7] = 0x80;
    buf[8] = 0x01;
    buf[9] = 0x79;
    buf[10] = 0x00;

    rc = rcs956_usb_raw_write(g_rcs956_dev.handle, buf, sizeof(buf),
                              time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(100);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0x33, ic_type);
    T_CHECK_EQ(0x0130, version);

    /* cleanup */

#if 0

    /*********************************/
    /* TSJ02-1603E */
    g_testnum = 3;

    TESTPR("TSJ02-1603E ...\n");

    /* build condition */
    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#endif

    /*********************************/
    /* TSJ02-1604E */
    g_testnum = 4;

    TESTPR("TSJ02-1604E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-1605E */
    g_testnum = 5;

    TESTPR("TSJ02-1605E ...\n");

    /* build condition */

    /* set parameters */
    timeout = 0;

    /* test execution */
    rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                     &ic_type, &version, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
