/**
 * \brief    Test Program for ( felica_cc_write_without_encryption
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

static UINT16 service_code_list_SDB200[16] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0ccd, 0x0cce, 0x0ccf,
    0x0c90, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c97, 0x0c49
};

UINT32 te_felica_cc_write_without_encryption(void);

/**
 * This function is test program
 * for felica_cc_write_without_encryption.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT32 te_felica_cc_write_without_encryption(void)
{
    UINT8 num_of_service;
    UINT16 service_code_list[FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX];
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 w_block_data[FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 r_block_data[FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;
    UINT32 timeout;
    UINT rc;
    UINT32 cnt = 0;
    felica_card_t card;
    felica_cc_devf_t bad_devf;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_FELICA_CC_WRITE_WITHOUT_ENCRYPTION;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    for (cnt = 0;
         cnt < (UINT32)(FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    /*********************************/
    /* TSJ00-0500N */
    g_testnum = 0;

    TESTPR("TSJ00-0500N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0501N */
    g_testnum = 1;

    TESTPR("TSJ00-0501N ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = 2;
    service_code_list[0] = 0x0c08;
    service_code_list[1] = 0x0c09;
    num_of_blocks = 2;
    utl_memcpy(block_list,
               (const UINT8*)"\x81\x00\x81\x01",
               (UINT)(num_of_blocks * 2));
    block_list[0] = 0x81;
    block_list[1] = 0x00;
    block_list[2] = 0x81;
    block_list[3] = 0x01;
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
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0502N */
    g_testnum = 2;

    TESTPR("TSJ00-0502N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list_SDB200));
    num_of_blocks = 11;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x80\x04\x80\x05\x80\x06\x80\x07"
               "\x8f\x00\x8f\x01\x8f\x02",
               (UINT)(num_of_blocks * 2));
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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0503N */
    g_testnum = 3;

    TESTPR("TSJ00-0503N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 4;
    service_code_list[0] = 0x0c09;
    service_code_list[1] = 0x0c49;
    service_code_list[2] = 0x0d09;
    service_code_list[3] = 0x0c91;

    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02"
               "\x80\x03\x81\x00\x81\x01\x81\x02\x81\x03"
               "\x82\x00\x82\x01\x82\x02\x82\x03\x83\x00",
               (UINT)(num_of_blocks * 2));

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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0504N */
    g_testnum = 4;

    TESTPR("TSJ00-0504N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;

    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x00;
    block_list[1] = 0x00;
    block_list[2] = 0x00;
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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0505N */
    g_testnum = 5;

    TESTPR("TSJ00-0505N ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = 2;
    service_code_list[0] = 0x0c08;
    service_code_list[1] = 0x0c09;

    num_of_blocks = 2;
    utl_memcpy(block_list,
               (const UINT8*)"\x01\x00\x00\x01\x01\x00",
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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0506N */
    g_testnum = 6;

    TESTPR("TSJ00-0506N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list));

    num_of_blocks = 10;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x0f\x00\x00\x0f\x01\x00",
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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0507N */
    g_testnum = 7;

    TESTPR("TSJ00-0507N ...\n");

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

    timeout = 0xffffffff;

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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0508N */
    g_testnum = 8;

    TESTPR("TSJ00-0508N ...\n");

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
               (const UINT8*)"\x01\x00\x00\x81\x01\x01\x02\x00"
               "\x81\x03\x84\x00\x04\x01\x00\x84\x02\x04\x03\x00"
               "\x06\x00\x00\x86\x01\x06\x02\x00\x86\x03",
               (UINT)((3 * 6) + (2 * 6)));

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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0509N */
    g_testnum = 9;

    TESTPR("TSJ00-0509N ...\n");

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
               (const UINT8*)"\x81\x00\x01\x01\x00\x81\x02"
               "\x01\x03\x00\x04\x00\x00\x84\x01\x04\x02\x00\x84\x03"
               "\x86\x00\x06\x01\x00\x86\x02\x06\x03\x00",
               (UINT)((2 * 6) + (3 * 6)));

    timeout = 0xffffffff;

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

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data,
                             w_block_data, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0510E */
    g_testnum = 10;

    TESTPR("TSJ00-0510E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = felica_cc_stub_rcs956_initialize(&bad_devf, &bad_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_write_without_encryption(&bad_devf, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0511E */
    g_testnum = 11;

    TESTPR("TSJ00-0511E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = rcs956_initialize(&bad_rcs956_dev, g_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_write_without_encryption(&bad_devf, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0512E */
    g_testnum = 12;

    TESTPR("TSJ00-0512E ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    card.idm[7] += 1;

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = 0x8000;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0513E */
    g_testnum = 13;

    TESTPR("TSJ00-0513E ...\n");

    /* build condition */
    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0514E */
    g_testnum = 14;

#if defined(DRV_UART)
    TESTPR("TSJ00-0514E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    /*********************************/
    /* TSJ00-0515E */
    g_testnum = 15;

    TESTPR("TSJ00-0515E ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = 0xffffffff;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0516E */
    g_testnum = 16;

    TESTPR("TSJ00-0516E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
#if (defined(ARCH_LINUX) && defined(DRV_UART))
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */
    rc = rcs956_open(&g_rcs956_dev, g_port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0517E */
    g_testnum = 17;

    TESTPR("TSJ00-0517E ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = g_timeout;

    utl_memcpy(&bad_devf, &g_felica_dev, sizeof(bad_devf));
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_devf.dev = &bad_rcs956_dev;
    bad_rcs956_dev.handle= (ICS_HANDLE)1023;
    /* test execution */
    rc = felica_cc_read_without_encryption(&bad_devf, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
#if (defined(ARCH_LINUX) && defined(DRV_UART))
#if !defined(OBS266)
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif
#else
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#endif

    /* cleanup */

    /*********************************/
    /* TSJ00-0518E */
    g_testnum = 18;

    TESTPR("TSJ00-0518E ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x8888;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x00;
    block_list[1] = 0x00;
    block_list[2] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_STATUS_FLAG1, rc);
    T_CHECK_NE(status_flag1, 0x00);
    T_CHECK_NE(status_flag2, 0x00);

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0519E */
    g_testnum = 19;

    TESTPR("TSJ00-0519E ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list));

    num_of_blocks = 10;
    for (cnt = 0; cnt < (UINT32)num_of_blocks; cnt++) {
        block_list[(cnt * 3) + 0] = 0x01;
        block_list[(cnt * 3) + 1] = 0x00;
        block_list[(cnt * 3) + 2] = 0x00;
    }

    block_list[27] = 0x0f;

    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_STATUS_FLAG1, rc);
    T_CHECK_NE(status_flag1, 0x00);
    T_CHECK_NE(status_flag2, 0x00);

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0520E */
    g_testnum = 20;

    TESTPR("TSJ00-0520E ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x0d;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_STATUS_FLAG1, rc);
    T_CHECK_NE(status_flag1, 0x00);
    T_CHECK_NE(status_flag2, 0x00);

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSJ00-0521E */
    g_testnum = 21;

    TESTPR("TSJ00-0521E ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = 4;
    service_code_list[0] = 0x0c09;
    service_code_list[1] = 0x0c49;
    service_code_list[2] = 0x0d09;
    service_code_list[3] = 0x0c91;

    num_of_blocks = 13;
    for (cnt = 0; cnt < (UINT32)num_of_blocks; cnt++) {
        block_list[(cnt * 2) + 0] = 0x80;
        block_list[(cnt * 2) + 1] = 0x00;
    }
    block_list[(cnt * 2) - 1] = 0x85;

    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, g_timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_STATUS_FLAG1, rc);
    T_CHECK_NE(status_flag1, 0x00);
    T_CHECK_NE(status_flag2, 0x00);

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
