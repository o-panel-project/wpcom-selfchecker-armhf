/**
 * \brief    Test Program for rcs956_felica_command
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_felica_command(void);

static UINT16 write_service_code_list_SDB200_sub[16] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0ccd, 0x0cce, 0x0ccf,
    0x0c90, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c97, 0x0c49
};

static UINT8 write_service_code_list_SDB200[] = "\x09\x0c\x08\x0c\x0a\x0c\x0b\x0c"
                                                "\xcc\x0c\xcd\x0c\xce\x0c\xcf\x0c"
                                                "\x90\x0c\x92\x0c\x93\x0c\x94\x0c"
                                                "\x95\x0c\x96\x0c\x97\x0c\x49\x0c";

static UINT16 read_service_code_list_SDB200_sub[16] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0c49, 0x0cce, 0x0ccf,
    0x0c90, 0x0c97, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c91
};

static UINT8 read_service_code_list_SDB200[] = "\x09\x0c\x08\x0c\x0a\x0c\x0b\x0c"
                                               "\xcc\x0c\x49\x0c\xce\x0c\xcf\x0c"
                                               "\x90\x0c\x97\x0c\x92\x0c\x93\x0c"
                                               "\x94\x0c\x95\x0c\x96\x0c\x91\x0c";

/**
 * This function is test program
 * for rcs956_felica_command.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
#if defined(DRV_UART)
UINT32 te_rcs956_felica_command(void)
{
    UINT32 timeout;
    UINT32 command_timeout;
    UINT rc;
    felica_card_t card;
    UINT8 command[RCS956_MAX_FELICA_COMMAND_LEN];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[RCS956_MAX_FELICA_RESPONSE_LEN];
    UINT32 response_len;
    UINT32 cnt;
    UINT8 num_of_service;
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 w_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 r_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;
    ICS_HW_DEVICE bad_rcs956_dev;
    UINT8 mode;

    /* function number */
    g_funcnum = JTNO_RCS956_FELICA_COMMAND;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    for (cnt = 0;
         cnt < (UINT32)(FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    /*********************************/
    /* TSJ01-1100N */
    g_testnum = 0;

    TESTPR("TSJ01-1100N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 10;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1101N */
    g_testnum = 1;

    TESTPR("TSJ01-1101N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x08;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               write_service_code_list_SDB200,
               sizeof(write_service_code_list_SDB200) - 1);

    command[10 + sizeof(write_service_code_list_SDB200) - 1] = 11;
    utl_memcpy(command + 11 + sizeof(write_service_code_list_SDB200) - 1,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02", 22);

    utl_memcpy(command + 33 + sizeof(write_service_code_list_SDB200) - 1,
               w_block_data, 16 * 11);

    command_len = 241;
    max_response_len = 15;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x09", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(11, response_len);

    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service,
                                           write_service_code_list_SDB200_sub,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1102N */
    g_testnum = 2;

    TESTPR("TSJ01-1102N ...\n");

    /* build condition */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));

    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1103N */
    g_testnum = 3;

    TESTPR("TSJ01-1103N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 19200, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 19200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 10;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1104N */
    g_testnum = 4;

    TESTPR("TSJ01-1104N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x08;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               write_service_code_list_SDB200,
               sizeof(write_service_code_list_SDB200) - 1);

    command[10 + sizeof(write_service_code_list_SDB200) - 1] = 11;
    utl_memcpy(command + 11 + sizeof(write_service_code_list_SDB200) - 1,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02", 22);

    utl_memcpy(command + 33 + sizeof(write_service_code_list_SDB200) - 1,
               w_block_data, 16 * 11);

    command_len = 241;
    max_response_len = 15;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x09", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(11, response_len);

    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service,
                                           write_service_code_list_SDB200_sub,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1105N */
    g_testnum = 5;

    TESTPR("TSJ01-1105N ...\n");

    /* build condition */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));

    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1106N */
    g_testnum = 6;

    TESTPR("TSJ01-1106N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 38400, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 38400);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 10;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1107N */
    g_testnum = 7;

    TESTPR("TSJ01-1107N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x08;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               write_service_code_list_SDB200,
               sizeof(write_service_code_list_SDB200) - 1);

    command[10 + sizeof(write_service_code_list_SDB200) - 1] = 11;
    utl_memcpy(command + 11 + sizeof(write_service_code_list_SDB200) - 1,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02", 22);

    utl_memcpy(command + 33 + sizeof(write_service_code_list_SDB200) - 1,
               w_block_data, 16 * 11);

    command_len = 241;
    max_response_len = 15;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x09", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(11, response_len);

    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service,
                                           write_service_code_list_SDB200_sub,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1108N */
    g_testnum = 8;

    TESTPR("TSJ01-1108N ...\n");

    /* build condition */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));

    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1109N */
    g_testnum = 9;

    TESTPR("TSJ01-1109N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 57600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 57600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 10;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1110N */
    g_testnum = 10;

    TESTPR("TSJ01-1110N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x08;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               write_service_code_list_SDB200,
               sizeof(write_service_code_list_SDB200) - 1);

    command[10 + sizeof(write_service_code_list_SDB200) - 1] = 11;
    utl_memcpy(command + 11 + sizeof(write_service_code_list_SDB200) - 1,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02", 22);

    utl_memcpy(command + 33 + sizeof(write_service_code_list_SDB200) - 1,
               w_block_data, 16 * 11);

    command_len = 241;
    max_response_len = 15;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x09", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(11, response_len);

    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service,
                                           write_service_code_list_SDB200_sub,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1111N */
    g_testnum = 11;

    TESTPR("TSJ01-1111N ...\n");

    /* build condition */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));

    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1112N */
    g_testnum = 12;

    TESTPR("TSJ01-1112N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 115200, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 10;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1113N */
    g_testnum = 13;

    TESTPR("TSJ01-1113N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x08;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               write_service_code_list_SDB200,
               sizeof(write_service_code_list_SDB200) - 1);

    command[10 + sizeof(write_service_code_list_SDB200) - 1] = 11;
    utl_memcpy(command + 11 + sizeof(write_service_code_list_SDB200) - 1,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02", 22);

    utl_memcpy(command + 33 + sizeof(write_service_code_list_SDB200) - 1,
               w_block_data, 16 * 11);

    command_len = 241;
    max_response_len = 15;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x09", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(11, response_len);

    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service,
                                           write_service_code_list_SDB200_sub,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1114N */
    g_testnum = 14;

    TESTPR("TSJ01-1114N ...\n");

    /* build condition */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));

    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

#if 0

    /*********************************/
    /* TSJ01-1115N */
    g_testnum = 15;

    TESTPR("TSJ01-1115N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 230400, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 230400);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 10;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1116N */
    g_testnum = 16;

    TESTPR("TSJ01-1116N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x08;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               write_service_code_list_SDB200,
               sizeof(write_service_code_list_SDB200) - 1);

    command[10 + sizeof(write_service_code_list_SDB200) - 1] = 11;
    utl_memcpy(command + 11 + sizeof(write_service_code_list_SDB200) - 1,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02", 22);

    utl_memcpy(command + 33 + sizeof(write_service_code_list_SDB200) - 1,
               w_block_data, 16 * 11);

    command_len = 241;
    max_response_len = 15;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x09", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(11, response_len);

    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service,
                                           write_service_code_list_SDB200_sub,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1117N */
    g_testnum = 17;

    TESTPR("TSJ01-1117N ...\n");

    /* build condition */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));

    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1118N */
    g_testnum = 18;

    TESTPR("TSJ01-1118N ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 460800, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 460800);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 10;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1119N */
    g_testnum = 19;

    TESTPR("TSJ01-1119N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x08;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               write_service_code_list_SDB200,
               sizeof(write_service_code_list_SDB200) - 1);

    command[10 + sizeof(write_service_code_list_SDB200) - 1] = 11;
    utl_memcpy(command + 11 + sizeof(write_service_code_list_SDB200) - 1,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02", 22);

    utl_memcpy(command + 33 + sizeof(write_service_code_list_SDB200) - 1,
               w_block_data, 16 * 11);

    command_len = 241;
    max_response_len = 15;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x09", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(11, response_len);

    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service,
                                           write_service_code_list_SDB200_sub,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ01-1120N */
    g_testnum = 20;

    TESTPR("TSJ01-1120N ...\n");

    /* build condition */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));

    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

#endif

    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, 115200, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1121N */
    g_testnum = 21;

    TESTPR("TSJ01-1121N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 0;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, NULL, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x00", 1));
    T_CHECK_EQ(0, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1122N */
    g_testnum = 22;

    TESTPR("TSJ01-1122N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ01-1123E */
    g_testnum = 23;

    TESTPR("TSJ01-1123E ...\n");

    /* build condition */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /* set parameters */
    command_len = 0;
    max_response_len = sizeof(response);
    response_len = 0;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, NULL, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1124E */
    g_testnum = 24;

    TESTPR("TSJ01-1124E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1125E */
    g_testnum = 25;

    TESTPR("TSJ01-1125E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 0;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1126E */
    g_testnum = 26;

    TESTPR("TSJ01-1126E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 2;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1127E */
    g_testnum = 27;

    TESTPR("TSJ01-1127E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 180000;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1128E */
    g_testnum = 28;

    TESTPR("TSJ01-1128E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0xffffffff;
    timeout = 0;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1129E */
    g_testnum = 29;

    TESTPR("TSJ01-1129E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0xffffffff;
    timeout = 2;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1130E */
    g_testnum = 30;

    TESTPR("TSJ01-1130E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0xffffffff;
    timeout = 180000;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1131E */
    g_testnum = 31;

    TESTPR("TSJ01-1131E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 115200);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1132E */
    g_testnum = 32;

    TESTPR("TSJ01-1132E ...\n");

    /* build condition */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(WAIT_POWER_DOWN);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1133E */
    g_testnum = 33;

    TESTPR("TSJ01-1133E ...\n");

    /* build condition */
    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ01-1134E */
    g_testnum = 34;

    TESTPR("TSJ01-1134E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 0;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1135E */
    g_testnum = 35;

    TESTPR("TSJ01-1135E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 9;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ01-1136E */
    g_testnum = 36;

    TESTPR("TSJ01-1136E ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 251;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /*********************************/
    /* TSJ01-1137E */
    g_testnum = 37;

    TESTPR("TSJ01-1137E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 9;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */

    /*********************************/
    /* TSJ01-1138E */
    g_testnum = 38;

    TESTPR("TSJ01-1138E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 9;
    command_timeout = 1000;
    timeout = 1000;
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_rcs956_dev.priv_data = NULL;

    /* test execution */
    rc = rcs956_felica_command(&bad_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#else
UINT32 te_rcs956_felica_command(void)
{
    UINT32 timeout;
    UINT32 command_timeout;
    UINT rc;
    felica_card_t card;
    UINT8 command[RCS956_MAX_FELICA_COMMAND_LEN];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[RCS956_MAX_FELICA_RESPONSE_LEN];
    UINT32 response_len;
    UINT32 cnt;
    UINT8 num_of_service;
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 w_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 r_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;
    UINT8 mode;

    /* function number */
    g_funcnum = JTNO_RCS956_FELICA_COMMAND;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    for (cnt = 0;
         cnt < (UINT32)(FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    /*********************************/
    /* TSJ02-0900N */
    g_testnum = 0;

    TESTPR("TSJ02-0900N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 10;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ02-0901N */
    g_testnum = 1;

    TESTPR("TSJ02-0901N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x08;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               write_service_code_list_SDB200,
               sizeof(write_service_code_list_SDB200) - 1);

    command[10 + sizeof(write_service_code_list_SDB200) - 1] = 11;
    utl_memcpy(command + 11 + sizeof(write_service_code_list_SDB200) - 1,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02", 22);

    utl_memcpy(command + 33 + sizeof(write_service_code_list_SDB200) - 1,
               w_block_data, 16 * 11);

    command_len = 241;
    max_response_len = 15;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x09", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(11, response_len);

    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service,
                                           write_service_code_list_SDB200_sub,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */

    /*********************************/
    /* TSJ02-0902N */
    g_testnum = 2;

    TESTPR("TSJ02-0902N ...\n");

    /* build condition */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            read_service_code_list_SDB200_sub,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00\x00", 2));
    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    /* cleanup */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /*********************************/
    /* TSJ02-0903N */
    g_testnum = 3;

    TESTPR("TSJ02-0903N ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 0;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, NULL, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x00", 1));
    T_CHECK_EQ(0, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ02-0904N */
    g_testnum = 4;

    TESTPR("TSJ02-0904N ...\n");

    /* build condition */
    rc = send_get_firmware_version_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(response, "\x05", 1));
    T_CHECK_EQ(0, utl_memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, utl_memcmp(response + 9, "\x00", 1));
    T_CHECK_EQ(10, response_len);

    /* cleanup */

    /*********************************/
    /* TSJ02-0905E */
    g_testnum = 5;

    TESTPR("TSJ02-0905E ...\n");

    /* build condition */
    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    /* set parameters */
    command_len = 0;
    max_response_len = sizeof(response);
    response_len = 0;
    command_timeout = 1000;
    timeout = g_timeout;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, NULL, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-0906E */
    g_testnum = 6;

    TESTPR("TSJ02-0906E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0907E */
    g_testnum = 7;

    TESTPR("TSJ02-0907E ...\n");

    /* build condition */
    PAUSE_RELEASE_CHIP()

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 0;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

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
    /* TSJ02-0908E */
    g_testnum = 8;

    TESTPR("TSJ02-0908E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 2;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

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
    /* TSJ02-0909E */
    g_testnum = 9;

    TESTPR("TSJ02-0909E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0xffff;
    timeout = 0xffff;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

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
    /* TSJ02-0910E */
    g_testnum = 10;

    TESTPR("TSJ02-0910E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0xffffffff;
    timeout = 0;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

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
    /* TSJ02-0911E */
    g_testnum = 11;

    TESTPR("TSJ02-0911E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0xffffffff;
    timeout = 2;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

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
    /* TSJ02-0912E */
    g_testnum = 12;

    TESTPR("TSJ02-0912E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 0xffffffff;
    timeout = 180000;

    measurement_timeout_start();

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#if 0

    /*********************************/
    /* TSJ02-0913E */
    g_testnum = 13;

    TESTPR("TSJ02-0913E ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = send_diagnose_command();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 14;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = rcs956_cancel_command(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#endif

    /*********************************/
    /* TSJ02-0914E */
    g_testnum = 14;

    TESTPR("TSJ02-0914E ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 0;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-0915E */
    g_testnum = 15;

    TESTPR("TSJ02-0915E ...\n");

    /* build condition */

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 9;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-0916E */
    g_testnum = 16;

    TESTPR("TSJ02-0916E ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 251;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);

    /*********************************/
    /* TSJ02-0917E */
    g_testnum = 17;

    TESTPR("TSJ02-0917E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    command[0] = 0x04;
    utl_memcpy(command + 1, card.idm, 8);
    command_len = 9;
    max_response_len = 9;
    command_timeout = 1000;
    timeout = 1000;

    /* test execution */
    rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                               max_response_len, response, &response_len,
                               command_timeout, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
