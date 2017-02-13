
#include "testmain.h"

#define RETRYCNT    100
#define COMMAND_TIME_OUT    10000


/* For examination that uses RC-S860(Sample5N) */
const static UINT16 node_code_list_rcs860[] =
{
    0x1008, 0x1009, 0x100a, 0x100b, 0x100c, 0x100d, 0x100e, 0x100f,
    0x1010, 0x1011, 0x1012, 0x1013, 0x1014, 0x1015, 0x1016, 0x1017,
    0x1108, 0x110c, 0x1108, 0x110c, 0x1110, 0x1112, 0x1114, 0x1116,
    0x1209, 0x1008, 0x1009, 0x100a, 0x100b, 0x100c, 0x100d, 0x100e
};

const static UINT16 Key_version_list_rcs860[] =
{
    0x0001, 0x0000, 0xfffe, 0x0000, 0x100c, 0x0000, 0x00ff, 0x0000,
    0xff00, 0x0000, 0x1012, 0x0000, 0x1014, 0x0000, 0x1016, 0x0000,
    0x1108, 0x110c, 0x1108, 0x110c, 0x1110, 0x1112, 0x1114, 0x1116,
    0x0000, 0x0001, 0x0000, 0xfffe, 0x0000, 0x100c, 0x0000, 0x00ff
};

/* For examination that uses SDB-200(FCF-006) */
static const UINT16 service_code_list_SDB200[] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0c91, 0x0cce, 0x0ccf,
    0x0c90, 0x0c97, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c49
};

UINT te_retry_fcc(void);
static UINT32 execute_scenario(void);

/**
 * This function is test program
 * for te_retry_fcc.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT te_retry_fcc(void)
{
    /* initialize */
    UINT rc;
    UINT32 cnt = 0;
    felica_card_t card;
    UINT32 timeout;
    UINT8 mode;
    UINT8 num_of_service;
    UINT16 service_code_list[FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX];
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 w_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 r_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;
    UINT16 service_index;
    UINT8 max_system_codes;
    UINT8 num_of_system_codes;
    UINT8 system_code_list[4];

    /* function number */
    g_funcnum = STNO_RETRY_FCC;

    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSS00-0300N */
    /* Obsolete */

    /*********************************/
    /* TSS00-0301N */
    g_testnum = 1;

    TESTPR("TSS00-0301N ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    retry_result_init();

    /* set parameters */
    service_index = 0x0000;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);

    /* test execution */
    PAUSE_RELEASE_CHIP();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry felica_cc_request_response >>>>> \n");


    for (cnt = 0; ; cnt++) {
        rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    T_CHECK_EQ(mode, 0x00);

    retry_result_print();

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSS00-0302N */
    g_testnum = 2;

    TESTPR("TSS00-0302N ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_SDB200,
               sizeof(service_code_list_SDB200));

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02\x80\x03"
                             "\x80\x04\x80\x05\x80\x06\x80\x07"
                             "\x8f\x00\x8f\x01\x8f\x02\x8f\x03"
                             "\x8f\x04\x8f\x05\x8f\x06",
                             (UINT)(num_of_blocks * 2));

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    for (cnt = 0;
        cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            8, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            4, &block_list[16],
                                            &w_block_data[128], &status_flag1,
                                            &status_flag2, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    retry_result_init();

    /* test execution */
    PAUSE_RELEASE_CHIP();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry felica_cc_read_without_encryption >>>>> \n");

    for (cnt = 0; ; cnt++) {
        rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                               num_of_service, service_code_list,
                                               num_of_blocks, block_list,
                                               r_block_data, &status_flag1,
                                               &status_flag2, timeout);

        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);
    T_CHECK_EQ(0, utl_memcmp(r_block_data, w_block_data, 16));

    retry_result_print();

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0303N */
    g_testnum = 3;

    TESTPR("TSS00-0303N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_service = 4;
    service_code_list[0] = 0x0c09;
    service_code_list[1] = 0x0d08;
    service_code_list[2] = 0x0d09;
    service_code_list[3] = 0x0c49;

    num_of_blocks = 13;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02\x80\x03"
                             "\x80\x04\x80\x05\x80\x06\x80\x07"
                             "\x83\x00\x83\x01\x83\x02\x83\x03"
                             "\x83\x04", (UINT)(num_of_blocks * 2));

    for (cnt = 0;
        cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    retry_result_init();

    /* test execution */
    PAUSE_RELEASE_CHIP();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry felica_cc_write_without_encryption >>>>> \n");

    for (cnt = 0; ; cnt++) {
        rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                                num_of_service, service_code_list,
                                                num_of_blocks, block_list,
                                                w_block_data, &status_flag1,
                                                &status_flag2, timeout);

        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    retry_result_print();

    /* cleanup */
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0304N */
    /* Obsolete */

    /*********************************/
    /* TSS00-0305N */
    g_testnum = 5;

    TESTPR("TSS00-0305N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = FELICA_CC_REQUEST_SYSTEM_CODE_MAX_NUM_OF_SYSTEM_CODES_MAX;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);

    retry_result_init();

    /* test execution */
    PAUSE_RELEASE_CHIP();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry felica_cc_request_system_code >>>>> \n");

    for (cnt = 0; ; cnt++) {
        rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                           max_system_codes, &num_of_system_codes,
                                           system_code_list, timeout);

        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    T_CHECK_EQ(num_of_system_codes, 0x01);
    T_CHECK_EQ(system_code_list[0], 0x00);
    T_CHECK_EQ(system_code_list[1], 0x18);

    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS00-0306N */
    g_testnum = 6;

    TESTPR("TSS00-0306N ...\n");

    PAUSE_SET_AND_RELEASE_CHIP_RCS860();

    retry_result_init();

    TESTPR("\n retry start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n" );
    for (cnt = 1; cnt <= 500; cnt++) {
        rc = execute_scenario();
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
            if (rc == ICS_ERROR_TIMEOUT) {
                TESTPR(".");
            } else {
                TESTPR("x");
            }
        } else {
            TESTPR("o");
        }

        if (cnt % 50 == 0) {
            TESTPR("\n");
        }
        fflush(stdout);
    }
    TESTPR("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< retry end\n");

    retry_result_print();

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);

}

static UINT32 execute_scenario(void)
{
    UINT32 rc = ICS_ERROR_SUCCESS;
    UINT32 timeout;
    int i;

    /* felica_cc_polling paramaters */
    felica_card_t card;
    felica_card_option_t card_option;
    UINT8 parameter[4] = {0xff, 0xff, 0x00, 0x00};

    /* felica_cc_request_service paramaters */
    UINT8 num_of_nodes;
    UINT16 node_code_list[FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX];
    UINT16 node_key_version_list[FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX];

    /* felica_cc_request_system_code paramaters */
    UINT8 max_system_codes;
    UINT8 num_of_system_codes;
    UINT8 system_code_list[4];

    /* felica_cc_request_response paramaters */
    UINT8 mode;

    /* felica_cc_write_without_encryption  paramaters */
    UINT8 rw_num_of_service;
    UINT16 rw_service_code_list[FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX];
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 2];
    UINT8 write_block_data[FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1, status_flag2;

    /* felica_cc_read_without_encryption  paramaters */
    UINT8 read_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];

    /* do felica_cc_polling */
    rc = felica_cc_polling(&g_felica_dev,
                           parameter,
                           &card,
                           &card_option,
                           POLLING_TIMEOUT_AT_DEVICE);
    if (ICS_ERROR_SUCCESS != rc) {
        return rc;
    }

    /* do felica_cc_request_service */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX;
    for (i = 0; i < 4; i++) {
        node_code_list[0 + (i * 8)] = 0xffff;
        node_code_list[1 + (i * 8)] = 0x0000;
        node_code_list[2 + (i * 8)] = 0x1008;
        node_code_list[3 + (i * 8)] = 0x100A;
        node_code_list[4 + (i * 8)] = 0x100C;
        node_code_list[5 + (i * 8)] = 0x100E;
        node_code_list[6 + (i * 8)] = 0x1010;
        node_code_list[7 + (i * 8)] = 0x1012;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[2], num_of_nodes);
    rc = felica_cc_request_service(&g_felica_dev, 
                                   &card,
                                   num_of_nodes,
                                   node_code_list,
                                   node_key_version_list,
                                   timeout);
    if (ICS_ERROR_SUCCESS != rc) {
        return rc;
    }

    /* do felica_cc_request_system_code */
    max_system_codes = 1;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);
    rc = felica_cc_request_system_code(&g_felica_dev,
                                       &card,
                                       max_system_codes,
                                       &num_of_system_codes,
                                       system_code_list,
                                       timeout);
    if (ICS_ERROR_SUCCESS != rc) {
        return rc;
    }

    /* do felica_cc_request_response */
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);
    rc = felica_cc_request_response(&g_felica_dev,
                                    &card,
                                    &mode,
                                    timeout);
    if (ICS_ERROR_SUCCESS != rc) {
        return rc;
    }

    /* do felica_cc_write_without_encryption */
    rw_num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    for (i = 0; i < 2; i++) {
        rw_service_code_list[0 + (i * 8)] = 0x1009;
        rw_service_code_list[1 + (i * 8)] = 0x100b;
        rw_service_code_list[2 + (i * 8)] = 0x100d;
        rw_service_code_list[3 + (i * 8)] = 0x100f;
        rw_service_code_list[4 + (i * 8)] = 0x1011;
        rw_service_code_list[5 + (i * 8)] = 0x1013;
        rw_service_code_list[6 + (i * 8)] = 0x1015;
        rw_service_code_list[7 + (i * 8)] = 0x1209;
    }
    num_of_blocks = 8;
    for (i = 0; i < num_of_blocks; i++) {
        block_list[0 + (i * 2)]  = 0x80;
        block_list[1 + (i * 2)]  = (UINT8)i;
    }
    for (i = 0; i < num_of_blocks * 16; i++) {
         write_block_data[i] = (UINT8)i;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);
    rc = felica_cc_write_without_encryption(&g_felica_dev,
                                            &card,
                                            rw_num_of_service,
                                            rw_service_code_list,
                                            num_of_blocks,
                                            block_list,
                                            write_block_data,
                                            &status_flag1,
                                            &status_flag2,
                                            timeout);
    if (ICS_ERROR_SUCCESS != rc) {
        return rc;
    }

    if ((status_flag1 != 0) || (status_flag2 != 0) ) {
        return 0xffff;
    }

    /* do felica_cc_read_without_encryption(data verify) */
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);
    rc = felica_cc_read_without_encryption(&g_felica_dev,
                                           &card,
                                           rw_num_of_service,
                                           rw_service_code_list,
                                           num_of_blocks,
                                           block_list,
                                           read_block_data,
                                           &status_flag1,
                                           &status_flag2,
                                           timeout);
    if (ICS_ERROR_SUCCESS != rc) {
        return rc;
    }

    if ((status_flag1 != 0) || (status_flag2 != 0) ) {
        return 0xffff;
    }

    for (i = 0; i < (8 * 16); i++) {
        if (read_block_data[i] != (UINT8)i) {
            break;
        }
    }
    if (i != (8 * 16)) {
        return 0xffff;
    }

    return ICS_ERROR_SUCCESS;
}
