/**
 * \brief    Test Program for rcs956_execute_command
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_execute_command(void);

/**
 * This function is test program
 * for rcs956_execute_command.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_execute_command(void)
{
    UINT32 timeout;
    UINT rc;
    UINT8 command[265];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[265];
    UINT32 response_len;
    UINT32 cnt;
    felica_card_t card;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_EXECUTE_COMMAND;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0600N */
    g_testnum = 0;

    TESTPR("TSJ01-0600N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 3;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(3, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0601N */
    g_testnum = 0;

    TESTPR("TSJ01-0601N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0602N */
    g_testnum = 2;

    TESTPR("TSJ01-0602N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 265;

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(0, utl_memcmp(response + 2, command + 3, 262));
    T_CHECK_EQ(264, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0603N */
    g_testnum = 3;

    TESTPR("TSJ01-0603N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 19200, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 19200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 3;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(3, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0604N */
    g_testnum = 4;

    TESTPR("TSJ01-0604N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0605N */
    g_testnum = 5;

    TESTPR("TSJ01-0605N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 265;

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(0, utl_memcmp(response + 2, command + 3, 262));
    T_CHECK_EQ(264, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0606N */
    g_testnum = 6;

    TESTPR("TSJ01-0606N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 38400, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 38400);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 3;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(3, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0607N */
    g_testnum = 7;

    TESTPR("TSJ01-0607N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0608N */
    g_testnum = 8;

    TESTPR("TSJ01-0608N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 265;

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(0, utl_memcmp(response + 2, command + 3, 262));
    T_CHECK_EQ(264, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0609N */
    g_testnum = 9;

    TESTPR("TSJ01-0609N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 57600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 57600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 3;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(3, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0610N */
    g_testnum = 10;

    TESTPR("TSJ01-0610N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0611N */
    g_testnum = 11;

    TESTPR("TSJ01-0611N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 265;

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(0, utl_memcmp(response + 2, command + 3, 262));
    T_CHECK_EQ(264, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0612N */
    g_testnum = 12;

    TESTPR("TSJ01-0612N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 3;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(3, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0613N */
    g_testnum = 13;

    TESTPR("TSJ01-0613N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0614N */
    g_testnum = 14;

    TESTPR("TSJ01-0614N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 265;

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(0, utl_memcmp(response + 2, command + 3, 262));
    T_CHECK_EQ(264, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

#if 0
    /*********************************/
    /* TSJ01-0615N */
    g_testnum = 15;

    TESTPR("TSJ01-0615N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 230400, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 230400);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 3;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(3, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0616N */
    g_testnum = 16;

    TESTPR("TSJ01-0616N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0617N */
    g_testnum = 17;

    TESTPR("TSJ01-0617N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 265;

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(0, utl_memcmp(response + 2, command + 3, 262));
    T_CHECK_EQ(264, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0618N */
    g_testnum = 18;

    TESTPR("TSJ01-0618N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 460800, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 460800);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 3;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(3, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0619N */
    g_testnum = 19;

    TESTPR("TSJ01-0619N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0620N */
    g_testnum = 20;

    TESTPR("TSJ01-0620N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 265;

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(0, utl_memcmp(response + 2, command + 3, 262));
    T_CHECK_EQ(264, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

#endif
    /*********************************/
    /* TSJ01-0621N */
    g_testnum = 21;

    TESTPR("TSJ01-0621N ...\n");

    /* build condition */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0622N */
    g_testnum = 22;

    TESTPR("TSJ01-0622N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x10\x00", 3);
    command_len = 3;
    max_response_len = 2;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(1, response_len);
    T_CHECK_EQ(0x7f, response[0]);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0623N */
    g_testnum = 23;

    TESTPR("TSJ01-0623N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(1, response_len);
    T_CHECK_EQ(0x7f, response[0]);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-0624E */
    g_testnum = 24;

    TESTPR("TSJ01-0624E ...\n");

    /* build condition */

    /* set parameters */
    timeout = 0;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0\x3e\x08", 4);
    command_len = 4;
    max_response_len = 6;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0625E */
    g_testnum = 25;

    TESTPR("TSJ01-0625E ...\n");

    /* build condition */

    /* set parameters */
    timeout = 2;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0\x3e\x08", 4);
    command_len = 4;
    max_response_len = 6;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0626E */
    g_testnum = 26;

    TESTPR("TSJ01-0626E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = 0x2bf20;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0\xff\xff", 4);
    command_len = 4;
    max_response_len = 6;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0627E */
    g_testnum = 27;

    TESTPR("TSJ01-0627E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0628E */
    g_testnum = 28;

    TESTPR("TSJ01-0628E ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    utl_memcmp(response, "\xd5\x17\x00", response_len);

    /* cleanup */
    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0629E */
    g_testnum = 29;

    TESTPR("TSJ01-0629E ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0\x3e\x08", 4);
    command_len = 4;
    max_response_len = 0;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-0630E */
    g_testnum = 30;

    TESTPR("TSJ01-0630E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 2;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-0631E */
    g_testnum = 31;

    TESTPR("TSJ01-0631E ...\n");

    /* build condition */

    /* set parameters */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 263;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-0632E */
    g_testnum = 32;

    TESTPR("TSJ01-0632E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

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
    /* TSJ01-0633E */
    g_testnum = 33;

    TESTPR("TSJ01-0633E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_execute_command(&bad_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_execute_command(void)
{
    UINT32 timeout;
    UINT rc;
    UINT8 command[265];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[265];
    UINT32 response_len;
    UINT32 cnt;
    felica_card_t card;

    /* function number */
    g_funcnum = JTNO_RCS956_EXECUTE_COMMAND;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0500N */
    g_testnum = 0;

    TESTPR("TSJ02-0500N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 3;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(3, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0501N */
    g_testnum = 1;

    TESTPR("TSJ02-0501N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0502N */
    g_testnum = 2;

    TESTPR("TSJ02-0502N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 265;

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x01\x00", 3));
    T_CHECK_EQ(0, utl_memcmp(response + 2, command + 3, 262));
    T_CHECK_EQ(264, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0503N */
    g_testnum = 3;

    TESTPR("TSJ02-0503N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\xd5\x03\x33\x01\x30\x07", 6));
    T_CHECK_EQ(6, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0504N */
    g_testnum = 4;

    TESTPR("TSJ02-0504N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x10\x00", 3);
    command_len = 3;
    max_response_len = 2;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(1, response_len);
    T_CHECK_EQ(0x7f, response[0]);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0505N */
    g_testnum = 5;

    TESTPR("TSJ02-0505N ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(1, response_len);
    T_CHECK_EQ(0x7f, response[0]);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0506E */
    g_testnum = 6;

    TESTPR("TSJ02-0506E ...\n");

    /* build condition */

    /* set parameters */
    timeout = 0;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0\x3e\x08", 4);
    command_len = 4;
    max_response_len = 6;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0507E */
    g_testnum = 7;

    TESTPR("TSJ02-0507E ...\n");

    /* build condition */

    /* set parameters */
    timeout = 2;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0\x3e\x08", 4);
    command_len = 4;
    max_response_len = 6;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0508E */
    g_testnum = 8;

    TESTPR("TSJ02-0508E ...\n");

    /* build condition */
    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = 0x2bf20;
    utl_memcpy(command, (const UINT8*)"\xd4\x42", 2);
    command_len = 2;
    max_response_len = 3;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0509E */
    g_testnum = 9;

    TESTPR("TSJ02-0509E ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\xa0\x3e\x08", 4);
    command_len = 4;
    max_response_len = 0;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-0510E */
    g_testnum = 10;

    TESTPR("TSJ02-0510E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x01", 3);
    command_len = 3;
    max_response_len = 2;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-0511E */
    g_testnum = 11;

    TESTPR("TSJ02-0511E ...\n");

    /* build condition */

    /* set parameters */
    utl_memcpy(command, (const UINT8*)"\xd4\x00\x00", 3);
    for (cnt = 0; cnt < 262; cnt++) {
        command[3 + cnt] = (UINT8)cnt;
    }

    command_len = 265;
    max_response_len = 263;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-0512E */
    g_testnum = 12;

    TESTPR("TSJ02-0512E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;
    utl_memcpy(command, (const UINT8*)"\xd4\x02", 2);
    command_len = 2;
    max_response_len = 6;

    /* test execution */
    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                                max_response_len, response, &response_len,
                                timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
