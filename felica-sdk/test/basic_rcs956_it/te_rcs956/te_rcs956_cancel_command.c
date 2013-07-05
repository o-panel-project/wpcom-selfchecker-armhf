/**
 * \brief    Test Program for rcs956_cancel_command
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_cancel_command(void);

/**
 * This function is test program
 * for rcs956_cancel_command.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_cancel_command(void)
{
    UINT32 command_timeout;
    UINT rc;
    UINT8 command[RCS956_MAX_FELICA_COMMAND_LEN];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[RCS956_MAX_FELICA_RESPONSE_LEN];
    UINT32 response_len;
    UINT8 buf[1];
    UINT32 time0;
    felica_card_t card;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_CANCEL_COMMAND;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1000N */
    g_testnum = 0;

    TESTPR("TSJ01-1000N ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_RELEASE_CHIP();

    utl_memcpy(command, (const UINT8*)"\x00\x00\x18\x00\x00", 5);
    command_len = 5;
    max_response_len = 17;
    command_timeout = 20000;

    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    time0 = utl_get_time_msec();

    rc = icsdrv_uart_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                          NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1001N */
    g_testnum = 1;

    TESTPR("TSJ01-1001N ...\n");

    /* build condition */

    utl_memcpy(command, (const UINT8*)"\x00\x00\x18\x00\x00", 5);
    command_len = 5;
    max_response_len = 17;
    command_timeout = 30000;

    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    time0 = utl_get_time_msec();

    rc = icsdrv_uart_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                          NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1002N */
    g_testnum = 2;

    TESTPR("TSJ01-1002N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x32\x05\x00\x00\xff", 6);
    command_len = 6;
    max_response_len = 2;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memcpy(command, (const UINT8*)"\xd4\x4A\x01\x01\x00"
                                      "\xff\xff\x00\x00", 9);
    command_len = 9;
    max_response_len = 20;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();

    utl_memcpy(command, (const UINT8*)"\xd4\x42", 2);
    command_len = 2;
    max_response_len = 20;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = icsdrv_uart_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                          NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1003N */
    g_testnum = 3;

    TESTPR("TSJ01-1003N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    time0 = utl_get_time_msec();

    rc = icsdrv_uart_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                          NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1004E */
    g_testnum = 4;

    TESTPR("TSJ01-1004E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1005E */
    g_testnum = 5;

    TESTPR("TSJ01-1005E ...\n");

    /* build condition */

    /* set parameters */
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_cancel_command(&bad_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_cancel_command(void)
{
    UINT32 command_timeout;
    UINT rc;
    UINT8 command[RCS956_MAX_FELICA_COMMAND_LEN];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[RCS956_MAX_FELICA_RESPONSE_LEN];
    UINT32 response_len;
    UINT8 buf[1];
    UINT32 time0;
    felica_card_t card;

    /* function number */
    g_funcnum = JTNO_RCS956_CANCEL_COMMAND;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0800N */
    g_testnum = 0;

    TESTPR("TSJ02-0800N ...\n");
    PAUSE_SET_RCS860();

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_RELEASE_CHIP();

    utl_memcpy(command, (const UINT8*)"\x00\x00\x18\x00\x00", 5);
    command_len = 5;
    max_response_len = 17;
    command_timeout = 20000;

    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    time0 = utl_get_time_msec();

    rc = rcs956_usb_raw_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                             NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0801N */
    g_testnum = 1;

    TESTPR("TSJ02-0801N ...\n");

    /* build condition */

    utl_memcpy(command, (const UINT8*)"\x00\x00\x18\x00\x00", 5);
    command_len = 5;
    max_response_len = 17;
    command_timeout = 30000;

    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    time0 = utl_get_time_msec();

    rc = rcs956_usb_raw_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                             NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0802N */
    g_testnum = 2;

    TESTPR("TSJ02-0802N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x32\x05\x00\x00\xff", 6);
    command_len = 6;
    max_response_len = 2;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memcpy(command, (const UINT8*)"\xd4\x4A\x01\x01\x00"
                                      "\xff\xff\x00\x00", 9);
    command_len = 9;
    max_response_len = 20;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();

    utl_memcpy(command, (const UINT8*)"\xd4\x42", 2);
    command_len = 2;
    max_response_len = 20;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = rcs956_usb_raw_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                             NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0803N */
    g_testnum = 3;

    TESTPR("TSJ02-0803N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */

    /* test execution */
    rc = rcs956_cancel_command(&g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    time0 = utl_get_time_msec();

    rc = rcs956_usb_raw_read(g_rcs956_dev.handle, sizeof(buf), sizeof(buf), buf,
                             NULL, time0, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0804E */
    /* Deprecated */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
