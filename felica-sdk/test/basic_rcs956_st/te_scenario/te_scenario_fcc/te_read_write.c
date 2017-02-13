
#include "testmain.h"

UINT te_read_write(void);

/**
 * This function is test program
 * for te_read_write.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT te_read_write(void)
{
    /* initialize */
    UINT rc;
    UINT32 cnt;
    UINT8 num_of_service;
    UINT16 service_code_list[16];
    UINT8 num_of_blocks;
    UINT8 w_block_data[240];
    UINT8 r_block_data1[240];
    UINT8 r_block_data2[240];
    UINT8 block_list[45];
    felica_card_t card;
    UINT32 timeout = g_timeout;
    UINT8 status_flag1;
    UINT8 status_flag2;

    /* function number */
    g_funcnum = STNO_READ_WRITE;

    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0100N */
    g_testnum = 0;

    TESTPR("TSS00-0100N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = 1;
    service_code_list[0] = 0x1009;
    num_of_blocks = 1;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00", 2);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0103N */
    g_testnum = 3;

    TESTPR("TSS00-0103N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 1;
    service_code_list[0] = 0x1009;
    num_of_blocks = 8;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x80\x01\x80\x02"
                                         "\x80\x03\x80\x04\x80\x05"
                                         "\x80\x06\x80\x07", 16);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0101N */
    g_testnum = 1;

    TESTPR("TSS00-0101N ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = 1;
    service_code_list[0] = 0x0c09;
    num_of_blocks = 2;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x80\x01", 4);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0104N */
    g_testnum = 4;

    TESTPR("TSS00-0104N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 2;
    service_code_list[0] = 0x0c09;
    service_code_list[1] = 0x0c49;
    num_of_blocks = 12;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x80\x01\x80\x02"
                                         "\x80\x03\x80\x04\x80\x05"
                                         "\x80\x06\x80\x07"
                                         "\x81\x00\x81\x01\x81\x02"
                                         "\x81\x03", 24);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], 8);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            8, block_list, w_block_data,
                                            &status_flag1,&status_flag2,
                                            timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], 4);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            4, &block_list[16],
                                            &w_block_data[128], &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0102N */
    g_testnum = 2;

    TESTPR("TSS00-0102N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = 1;
    service_code_list[0] = 0x0c09;
    num_of_blocks = 5;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x80\x01\x80\x02"
                                         "\x80\x03\x80\x04", 10);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0105N */
    g_testnum = 5;

    TESTPR("TSS00-0105N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 2;
    service_code_list[0] = 0x0c09;
    service_code_list[1] = 0x0c49;
    num_of_blocks = 15;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x80\x01\x80\x02"
                                         "\x80\x03\x80\x04\x80\x05"
                                         "\x80\x06\x80\x07"
                                         "\x81\x00\x81\x01\x81\x02"
                                         "\x81\x03\x81\x04\x81\x05"
                                         "\x81\x06", 30);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], 13);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            13, block_list, w_block_data,
                                            &status_flag1, &status_flag2,
                                            timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], 2);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            2, &block_list[26],
                                            &w_block_data[208], &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0106N */
    g_testnum = 6;

    TESTPR("TSS00-0106N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 1;
    service_code_list[0] = 0x0c09;
    num_of_blocks = 7;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x80\x01\x80\x02"
                                         "\x80\x03\x80\x04\x80\x05"
                                         "\x80\x06", 14);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    num_of_blocks = 1;
    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    num_of_blocks = 3;
    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    num_of_blocks = 7;
    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0107N */
    g_testnum = 7;

    TESTPR("TSS00-0107N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 1;
    service_code_list[0] = 0x0c09;
    num_of_blocks = 15;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x80\x01\x80\x02"
                                         "\x80\x03\x80\x04\x80\x05"
                                         "\x80\x06\x80\x07"
                                         "\x80\x00\x80\x01\x80\x02"
                                         "\x80\x03\x80\x04\x80\x05"
                                         "\x80\x06", 30);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    num_of_blocks = 11;
    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    num_of_blocks = 13;
    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    num_of_blocks = 15;
    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], 13);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            13, block_list, w_block_data,
                                            &status_flag1, &status_flag2,
                                            timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], 2);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            2, &block_list[26],
                                            &w_block_data[208], &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0108N */
    g_testnum = 8;

    TESTPR("TSS00-0108N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 9;
    service_code_list[0] = 0x0c09;
    service_code_list[1] = 0x0c49;
    service_code_list[2] = 0x0c91;
    service_code_list[3] = 0x0ccd;
    service_code_list[4] = 0x0d09;
    service_code_list[5] = 0x0d49;
    service_code_list[6] = 0x0d91;
    service_code_list[7] = 0x0dcd;
    service_code_list[8] = 0x0c09;

    num_of_blocks = 6;
    utl_memcpy(block_list, (const UINT8*)"\x83\x00\x84\x00\x85\x00"
                                         "\x86\x00\x87\x00\x88\x01", 12);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    num_of_blocks = 6;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x81\x00\x82\x00"
                                         "\x83\x00\x84\x00\x85\x00", 12);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* TSS00-0109N */
    g_testnum = 9;

    TESTPR("TSS00-0109N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = 13;
    service_code_list[0] = 0x0c09;
    service_code_list[1] = 0x0c49;
    service_code_list[2] = 0x0c91;
    service_code_list[3] = 0x0ccd;
    service_code_list[4] = 0x0d09;
    service_code_list[5] = 0x0d49;
    service_code_list[6] = 0x0d91;
    service_code_list[7] = 0x0dcd;
    service_code_list[8] = 0x0c09;
    service_code_list[9] = 0x0c49;
    service_code_list[10] = 0x0c91;
    service_code_list[11] = 0x0ccd;
    service_code_list[12] = 0x0d09;

    num_of_blocks = 10;
    utl_memcpy(block_list, (const UINT8*)"\x80\x00\x81\x00\x82\x00"
                                         "\x83\x00\x84\x00\x85\x00"
                                         "\x86\x00\x87\x00\x88\x00"
                                         "\x89\x00", 20);
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution & results judgement */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    utl_memcpy(block_list, (const UINT8*)"\x83\x00\x84\x00\x85\x00"
                                         "\x86\x00\x87\x00\x88\x00"
                                         "\x89\x00\x8a\x00\x8b\x00"
                                         "\x8c\x00", 20);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data1, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)(r_block_data1[cnt] + 1);
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            num_of_blocks, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data2, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data2,
                             w_block_data, (UINT)(num_of_blocks * 16)));
    T_CHECK_NE(0, utl_memcmp(r_block_data1,
                             r_block_data2, (UINT)(num_of_blocks * 16)));

    /* cleanup */
    utl_memset(r_block_data1, 0xcc, sizeof(r_block_data1));
    utl_memset(r_block_data2, 0xcc, sizeof(r_block_data2));
    status_flag1 = 0xcc;
    status_flag2 = 0xcc;

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
