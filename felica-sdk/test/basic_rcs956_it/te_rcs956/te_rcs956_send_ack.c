/**
 * \brief    Test Program for rcs956_send_ack
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_send_ack(void);

/**
 * This function is test program
 * for rcs956_send_ack.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_send_ack(void)
{
    UINT32 timeout;
    UINT32 command_timeout;
    UINT rc;
    UINT8 command[RCS956_MAX_FELICA_COMMAND_LEN];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[RCS956_MAX_FELICA_RESPONSE_LEN];
    UINT32 response_len;
    felica_card_t card;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_RCS956_SEND_ACK;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0700N */
    g_testnum = 0;

    TESTPR("TSJ01-0700N ...\n");

    /* build condition */
    utl_memcpy(command, (const UINT8*)"\xd4\x10\x00", 3);
    command_len = 3;
    max_response_len = 10;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_msleep(10);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0701N */
    g_testnum = 1;

    TESTPR("TSJ01-0701N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_RELEASE_CHIP();

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
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memcpy(command, (const UINT8*)"\xd4\x42", 2);
    command_len = 2;
    max_response_len = 20;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0702N */
    g_testnum = 2;

    TESTPR("TSJ01-0702N ...\n");

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
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    utl_msleep(10);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0703N */
    g_testnum = 3;

    TESTPR("TSJ01-0703N ...\n");

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
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    utl_msleep(10);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-0704E */
    g_testnum = 4;

    TESTPR("TSJ01-0704E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */

    /*********************************/
    /* TSJ01-0705E */
    g_testnum = 5;

    TESTPR("TSJ01-0705E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_send_ack(&bad_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_send_ack(void)
{
    UINT32 timeout;
    UINT32 command_timeout;
    UINT rc;
    UINT8 command[RCS956_MAX_FELICA_COMMAND_LEN];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[RCS956_MAX_FELICA_RESPONSE_LEN];
    UINT32 response_len;
    felica_card_t card;

    /* function number */
    g_funcnum = JTNO_RCS956_SEND_ACK;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0600N */
    g_testnum = 0;

    TESTPR("TSJ02-0600N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_RELEASE_CHIP();

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
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memcpy(command, (const UINT8*)"\xd4\x42", 2);
    command_len = 2;
    max_response_len = 20;

    rc = rcs956_execute_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               g_timeout);
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0601N */
    g_testnum = 1;

    TESTPR("TSJ02-0601N ...\n");

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
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    utl_msleep(10);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0602N */
    g_testnum = 2;

    TESTPR("TSJ02-0602N ...\n");

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
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* cleanup */
    utl_msleep(10);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0603E */
    g_testnum = 3;

    TESTPR("TSJ02-0603E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_send_ack(&g_rcs956_dev, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
