/**
 * \brief    Test Program for felica_cc_request_response
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

static UINT16 service_code_list_SDB200[] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0ccd, 0x0cce, 0x0ccf,
    0x0c90, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c97, 0x0c49
};

UINT32 te_felica_cc_communication_test(void);

/**
 * This function is test program
 * for felica_cable_transmission_rate_switch.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_felica_cc_communication_test(void)
{
    UINT8 mode;
    UINT32 timeout;
    UINT32 rc;
    UINT8 num_of_service;
    UINT16 service_code_list[FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX];
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 w_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 r_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;
    UINT32 read_len;
    UINT8  data[280];
    UINT32 time0;
    felica_card_t card;

    /* function number */
    g_funcnum = JTNO_FELICA_CC_COMMUNICATION_TEST;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#if defined(DRV_UART)
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0900N */
    g_testnum = 0;

    TESTPR("TSJ00-0900N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    mode = 0xff;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */

    /*********************************/
    /* TSJ00-0901N */
    g_testnum = 1;

    TESTPR("TSJ00-0901N ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list_SDB200));

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));
    timeout = g_timeout;

    /* build condition */

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0902N */
    g_testnum = 2;

    TESTPR("TSJ00-0902N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 7;
    service_code_list[0] = 0x0c08;
    service_code_list[1] = 0x0c09;
    service_code_list[2] = 0x0c0a;
    service_code_list[3] = 0x0c0b;
    service_code_list[4] = 0x0c49;
    service_code_list[5] = 0x0d08;
    service_code_list[6] = 0x0d09;

    num_of_blocks = 12;
    utl_memcpy(block_list,
               (const UINT8*)"\x01\x00\x00\x01\x01\x00\x01\x02\x00"
               "\x01\x03\x00\x04\x00\x00\x04\x01\x00\x04\x02\x00"
               "\x04\x03\x00\x06\x00\x00\x06\x01\x00\x06\x02\x00"
               "\x06\x03\x00",
               (UINT)(num_of_blocks * 3));

    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0903N */
    g_testnum = 3;

    TESTPR("TSJ00-0903N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#if defined(DRV_UART)
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 38400, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 38400);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    mode = 0xff;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */

    /*********************************/
    /* TSJ00-0904N */
    g_testnum = 4;

    TESTPR("TSJ00-0904N ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list_SDB200));

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));
    timeout = g_timeout;

    /* build condition */

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0905N */
    g_testnum = 5;

    TESTPR("TSJ00-0905N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 7;
    service_code_list[0] = 0x0c08;
    service_code_list[1] = 0x0c09;
    service_code_list[2] = 0x0c0a;
    service_code_list[3] = 0x0c0b;
    service_code_list[4] = 0x0c49;
    service_code_list[5] = 0x0d08;
    service_code_list[6] = 0x0d09;

    num_of_blocks = 12;
    utl_memcpy(block_list,
               (const UINT8*)"\x01\x00\x00\x01\x01\x00\x01\x02\x00"
               "\x01\x03\x00\x04\x00\x00\x04\x01\x00\x04\x02\x00"
               "\x04\x03\x00\x06\x00\x00\x06\x01\x00\x06\x02\x00"
               "\x06\x03\x00",
               (UINT)(num_of_blocks * 3));

    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0906N */
    g_testnum = 6;

    TESTPR("TSJ00-0906N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#if defined(DRV_UART)
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 57600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 57600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    mode = 0xff;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */

    /*********************************/
    /* TSJ00-0907N */
    g_testnum = 7;

    TESTPR("TSJ00-0907N ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list_SDB200));

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));
    timeout = g_timeout;

    /* build condition */

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0908N */
    g_testnum = 8;

    TESTPR("TSJ00-0908N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 7;
    service_code_list[0] = 0x0c08;
    service_code_list[1] = 0x0c09;
    service_code_list[2] = 0x0c0a;
    service_code_list[3] = 0x0c0b;
    service_code_list[4] = 0x0c49;
    service_code_list[5] = 0x0d08;
    service_code_list[6] = 0x0d09;

    num_of_blocks = 12;
    utl_memcpy(block_list,
               (const UINT8*)"\x01\x00\x00\x01\x01\x00\x01\x02\x00"
               "\x01\x03\x00\x04\x00\x00\x04\x01\x00\x04\x02\x00"
               "\x04\x03\x00\x06\x00\x00\x06\x01\x00\x06\x02\x00"
               "\x06\x03\x00",
               (UINT)(num_of_blocks * 3));

    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0909E */
    g_testnum = 9;

    TESTPR("TSJ00-0909E ...\n");

    /* set parameters */
    timeout = 0;

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#if defined(DRV_UART)
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_RELEASE_CHIP();

    mode = 0xff;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSJ00-0910E */
    g_testnum = 10;

    TESTPR("TSJ00-0910E ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list_SDB200));

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));
    timeout = 0;

    /* build condition */

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0911E */
    g_testnum = 11;

    TESTPR("TSJ00-0911E ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 7;
    service_code_list[0] = 0x0c08;
    service_code_list[1] = 0x0c09;
    service_code_list[2] = 0x0c0a;
    service_code_list[3] = 0x0c0b;
    service_code_list[4] = 0x0c49;
    service_code_list[5] = 0x0d08;
    service_code_list[6] = 0x0d09;

    num_of_blocks = 12;
    utl_memcpy(block_list,
               (const UINT8*)"\x01\x00\x00\x01\x01\x00\x01\x02\x00"
               "\x01\x03\x00\x04\x00\x00\x04\x01\x00\x04\x02\x00"
               "\x04\x03\x00\x06\x00\x00\x06\x01\x00\x06\x02\x00"
               "\x06\x03\x00",
               (UINT)(num_of_blocks * 3));

    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));

    /*********************************/
    /* TSJ00-0912E */
    g_testnum = 12;

    TESTPR("TSJ00-0912E ...\n");

    /* set parameters */
    timeout = 0;

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#if defined(DRV_UART)
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 38400, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 38400);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_RELEASE_CHIP();

    mode = 0xff;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSJ00-0913E */
    g_testnum = 13;

    TESTPR("TSJ00-0913E ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list_SDB200));

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));
    timeout = 0;

    /* build condition */

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0914E */
    g_testnum = 14;

    TESTPR("TSJ00-0914E ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 7;
    service_code_list[0] = 0x0c08;
    service_code_list[1] = 0x0c09;
    service_code_list[2] = 0x0c0a;
    service_code_list[3] = 0x0c0b;
    service_code_list[4] = 0x0c49;
    service_code_list[5] = 0x0d08;
    service_code_list[6] = 0x0d09;

    num_of_blocks = 12;
    utl_memcpy(block_list,
               (const UINT8*)"\x01\x00\x00\x01\x01\x00\x01\x02\x00"
               "\x01\x03\x00\x04\x00\x00\x04\x01\x00\x04\x02\x00"
               "\x04\x03\x00\x06\x00\x00\x06\x01\x00\x06\x02\x00"
               "\x06\x03\x00",
               (UINT)(num_of_blocks * 3));

    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0915E */
    g_testnum = 15;

    TESTPR("TSJ00-0915E ...\n");

    /* set parameters */
    timeout = 0;

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#if defined(DRV_UART)
    rc = rcs956_set_dev_speed(&g_rcs956_dev, 57600, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, 57600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_RELEASE_CHIP();

    mode = 0xff;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSJ00-0916E */
    g_testnum = 16;

    TESTPR("TSJ00-0916E ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list_SDB200));

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));
    timeout = 0;

    /* build condition */

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0917E */
    g_testnum = 17;

    TESTPR("TSJ00-0917E ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 7;
    service_code_list[0] = 0x0c08;
    service_code_list[1] = 0x0c09;
    service_code_list[2] = 0x0c0a;
    service_code_list[3] = 0x0c0b;
    service_code_list[4] = 0x0c49;
    service_code_list[5] = 0x0d08;
    service_code_list[6] = 0x0d09;

    num_of_blocks = 12;
    utl_memcpy(block_list,
               (const UINT8*)"\x01\x00\x00\x01\x01\x00\x01\x02\x00"
               "\x01\x03\x00\x04\x00\x00\x04\x01\x00\x04\x02\x00"
               "\x04\x03\x00\x06\x00\x00\x06\x01\x00\x06\x02\x00"
               "\x06\x03\x00",
               (UINT)(num_of_blocks * 3));

    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    time0 = utl_get_time_msec();

    rc = g_raw_func->read(g_rcs956_dev.handle, 1, sizeof(data),
                          data, &read_len, time0, g_timeout);

    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

#if defined(DRV_UART)
    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
