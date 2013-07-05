
#include "testmain.h"

UINT te_pmm(void);

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

static const UINT16 service_code_list_rcs860[] =
{
    0x1009, 0x1008, 0x100a, 0x100b, 0x100c, 0x100d, 0x100e, 0x100f,
    0x1010, 0x1011, 0x1012, 0x1013, 0x1014, 0x1015, 0x1016, 0x1209,
};

/* For examination that uses RCX-407(FCF-003) */
const static UINT16 node_code_list_rcx407[] =
{
    0x0c08, 0x0c09, 0x0c0a, 0x0c0b, 0x0c48, 0x0c49, 0x0c4a, 0x0c90,
    0x0c90, 0x0c91, 0x0c91, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c97,
    0x0ccc, 0x0ccd, 0x0cce, 0x0ccf, 0x0d08, 0x0d09, 0x0d0a, 0x0d0b,
    0x0d48, 0x0d49, 0x0d4a, 0x0d4a, 0x0d90, 0x0d91, 0x0d91, 0x0d93
};

const static UINT16 Key_version_list_rcx407[] =
{
    0x0c08, 0x0000, 0x0c0a, 0x0000, 0x0c48, 0x0000, 0x0c4a, 0x0c90,
    0x0c90, 0x0000, 0x0000, 0x0000, 0x0c94, 0x0000, 0x0c96, 0x0000,
    0x0ccc, 0x0000, 0x0cce, 0x0000, 0x0d08, 0x0000, 0x0d0a, 0x0000,
    0x0d48, 0x0000, 0x0d4a, 0x0d4a, 0x0d90, 0x0000, 0x0000, 0x0000
};

static const UINT16 service_code_list_rcx407[] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0c48, 0x0c97, 0x0c4a, 0x0c4b,
    0x0c90, 0x0c91, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c49
};

/* For examination that uses SDB-200(FCF-006) */
const static UINT16 node_code_list_SDB200[] =
{
    0x0c08, 0x0c09, 0x0c0a, 0x0c0b, 0x0c48, 0x0c49, 0x0c4a, 0x0d94,
    0x0c90, 0x0c91, 0x0d95, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c97,
    0x0ccc, 0x0ccd, 0x0cce, 0x0ccf, 0x0d08, 0x0d09, 0x0d0a, 0x0d0b,
    0x0d48, 0x0d49, 0x0d4a, 0x0d96, 0x0d90, 0x0d91, 0x0d97, 0x0d93
};

const static UINT16 Key_version_list_SDB200[] =
{
    0x0c08, 0x0000, 0x0c0a, 0x0000, 0x0c48, 0x0000, 0x0c4a, 0x0d94,
    0x0c90, 0x0000, 0x0000, 0x0000, 0x0c94, 0x0000, 0x0c96, 0x0000,
    0x0ccc, 0x0000, 0x0cce, 0x0000, 0x0d08, 0x0000, 0x0d0a, 0x0000,
    0x0d48, 0x0000, 0x0d4a, 0x0d96, 0x0d90, 0x0000, 0x0000, 0x0000
};

static const UINT16 service_code_list_SDB200[] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0c91, 0x0cce, 0x0ccf,
    0x0c90, 0x0c97, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c49
};
/**
 * This function is test program
 * for te_pmm.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT te_pmm(void)
{
    /* initialize */
    UINT rc;
    UINT32 cnt = 0;
    felica_card_t card;
    UINT32 timeout;
    UINT8 num_of_nodes;
    UINT16 node_code_list[FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX];
    UINT16 node_key_version_list[FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX];
    UINT8 mode;
    UINT8 num_of_service;
    UINT16 service_code_list[FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX];
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 w_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 r_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;
    UINT8 max_system_codes;
    UINT8 num_of_system_codes;
    UINT8 system_code_list[4];

    /* function number */
    g_funcnum = STNO_PMM;

    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    mode = 0xff;

    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    for (cnt = 0;
        cnt < (UINT32)(FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    num_of_system_codes = 0xff;
    utl_memset(system_code_list, 0xff, sizeof(system_code_list));

    /*********************************/
    /* TSS00-0200N */
    g_testnum = 0;

    TESTPR("TSS00-0200N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1008;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[2], num_of_nodes);

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(node_key_version_list[0], 0x0001);

    /* cleanup */
    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSS00-0201N */
    g_testnum = 1;

    TESTPR("TSS00-0201N ...\n");

    /* build condition */

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX;
    utl_memcpy(node_code_list,
               node_code_list_rcs860,
               sizeof(node_code_list_rcs860));


    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[2], num_of_nodes);

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(node_key_version_list,
                             Key_version_list_rcs860,
                             sizeof(Key_version_list_rcs860)));

    /* cleanup */
    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSS00-0202N */
    g_testnum = 2;

    TESTPR("TSS00-0202N ...\n");

    /* build condition */

    /* set parameters */
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSS00-0203N */
    g_testnum = 3;

    TESTPR("TSS00-0203N ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;

    /* build condition */
    for (cnt = 0;
        cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
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

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0204N */
    g_testnum = 4;

    TESTPR("TSS00-0204N ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_rcs860,
               sizeof(service_code_list_rcs860));

    num_of_blocks = 12;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02\x80\x03"
                             "\x80\x04\x80\x05\x80\x06\x80\x07"
                             "\x8f\x00\x8f\x01\x8f\x02\x8f\x03"
                             ,(UINT)(num_of_blocks * 2));

    /* build condition */
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

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);

   /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0205N */
    g_testnum = 5;

    TESTPR("TSS00-0205N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x1009;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;

    for (cnt = 0;
        cnt < (UINT32)(FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

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
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0206N */
    g_testnum = 6;

    TESTPR("TSS00-0206N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_rcs860,
               sizeof(service_code_list_rcs860));

    num_of_blocks = 8;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02\x80\x03"
                             "\x80\x04\x80\x05\x80\x06\x80\x07"
                             ,(UINT)(num_of_blocks * 2));

    for (cnt = 0;
        cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

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
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0207N */
    /* Obsolete */

    /*********************************/
    /* TSS00-0208N */
    g_testnum = 8;

    TESTPR("TSS00-0208N ...\n");

    /* build condition */

    /* set parameters */
    max_system_codes = FELICA_CC_REQUEST_SYSTEM_CODE_MAX_NUM_OF_SYSTEM_CODES_MAX;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(num_of_system_codes, 0x01);
    T_CHECK_EQ(system_code_list[0], 0x00);
    T_CHECK_EQ(system_code_list[1], 0x18);

    /* cleanup */
    num_of_system_codes = 0xff;
    utl_memset(system_code_list, 0xff, sizeof(system_code_list));

    /*********************************/
    /* TSS00-0209N */
    g_testnum = 9;

    TESTPR("TSS00-0209N ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x0c08;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[2], num_of_nodes);

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(node_key_version_list[0], 0x0c08);

    /* cleanup */
    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSS00-0210N */
    g_testnum = 10;

    TESTPR("TSS00-0210N ...\n");

    /* build condition */

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX;
    utl_memcpy(node_code_list,
               node_code_list_rcx407,
               sizeof(node_code_list_rcx407));


    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[2], num_of_nodes);

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(node_key_version_list,
                             Key_version_list_rcx407,
                             sizeof(Key_version_list_rcx407)));

    /* cleanup */
    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSS00-0211N */
    g_testnum = 11;

    TESTPR("TSS00-0211N ...\n");

    /* build condition */

    /* set parameters */
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSS00-0212N */
    g_testnum = 12;

    TESTPR("TSS00-0212N ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x0c09;
    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;

    /* build condition */
    for (cnt = 0;
        cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
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

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0213N */
    g_testnum = 13;

    TESTPR("TSS00-0213N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_rcx407,
               sizeof(service_code_list_rcx407));

    num_of_blocks = 12;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02\x80\x03"
                             "\x80\x04\x80\x05"
                             "\x8f\x00\x8f\x01\x8f\x02\x8f\x03"
                             "\x8f\x04\x8f\x05"
                             ,(UINT)(num_of_blocks * 2));

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

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);
   /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0214N */
    g_testnum = 14;

    TESTPR("TSS00-0214N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x0c09;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;

    for (cnt = 0;
        cnt < (UINT32)(FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

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
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0215N */
    g_testnum = 15;

    TESTPR("TSS00-0215N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(service_code_list,
               service_code_list_rcx407,
               sizeof(service_code_list_rcx407));

    num_of_blocks = 8;
    utl_memcpy(block_list,
               (const UINT8*)"\x80\x00\x80\x01\x80\x02\x80\x03"
                             "\x80\x04\x80\x05\x80\x06\x80\x07"
                             ,(UINT)(num_of_blocks * 2));

    for (cnt = 0;
        cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

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
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0216N */
    /* Obsolete */

    /*********************************/
    /* TSS00-0217N */
    g_testnum = 17;

    TESTPR("TSS00-0217N ...\n");

    /* build condition */

    /* set parameters */
    max_system_codes = FELICA_CC_REQUEST_SYSTEM_CODE_MAX_NUM_OF_SYSTEM_CODES_MAX;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(num_of_system_codes, 0x01);
    T_CHECK_EQ(system_code_list[0], 0x00);
    T_CHECK_EQ(system_code_list[1], 0x18);

    /* cleanup */
    num_of_system_codes = 0xff;
    utl_memset(system_code_list, 0xff, sizeof(system_code_list));

    /*********************************/
    /* TSS00-0218N */
    g_testnum = 18;

    TESTPR("TSS00-0218N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x0c09;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[2], num_of_nodes);

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(node_key_version_list[0], 0x0000);

    /* cleanup */
    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSS00-0219N */
    g_testnum = 19;

    TESTPR("TSS00-0219N ...\n");

    /* build condition */

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX;
    utl_memcpy(node_code_list,
               node_code_list_SDB200,
               sizeof(node_code_list_SDB200));


    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[2], num_of_nodes);

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(node_key_version_list,
                             Key_version_list_SDB200,
                             sizeof(Key_version_list_SDB200)));

    /* cleanup */
    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSS00-0220N */
    g_testnum = 20;

    TESTPR("TSS00-0220N ...\n");

    /* build condition */

    /* set parameters */
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSS00-0221N */
    g_testnum = 21;

    TESTPR("TSS00-0221N ...\n");

    /* set parameters */
    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x0c09;
    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* build condition */
    for (cnt = 0;
        cnt < (UINT32)(FELICA_CC_READ_WE_NUM_OF_BLOCKS_MIN * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
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

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0222N */
    g_testnum = 22;

    TESTPR("TSS00-0222N ...\n");

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

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* build condition */
    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            5, &block_list[20],
                                            &w_block_data[160], &status_flag1,
                                            &status_flag2, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* test execution */
    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);
   /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0223N */
    g_testnum = 23;

    TESTPR("TSS00-0223N ...\n");

    /* build condition */

    /* set parameters */
    num_of_service = FELICA_CC_WRITE_WE_NUM_OF_SERVICES_MIN;
    service_code_list[0] = 0x0c09;
    num_of_blocks = FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN;
    block_list[0] = 0x80;
    block_list[1] = 0x00;

    for (cnt = 0;
        cnt < (UINT32)(FELICA_CC_WRITE_WE_NUM_OF_BLOCKS_MIN * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

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
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0224N */
    g_testnum = 24;

    TESTPR("TSS00-0224N ...\n");

    /* build condition */

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
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS00-0225N */
    /* Obsolete */

    /*********************************/
    /* TSS00-0226N */
    g_testnum = 26;

    TESTPR("TSS00-0226N ...\n");

    /* build condition */

    /* set parameters */
    max_system_codes =
        FELICA_CC_REQUEST_SYSTEM_CODE_MAX_NUM_OF_SYSTEM_CODES_MAX;
    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[3], 0);

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(num_of_system_codes, 0x02);
    T_CHECK_EQ(system_code_list[0], 0x00);
    T_CHECK_EQ(system_code_list[1], 0x18);
    T_CHECK_EQ(system_code_list[2], 0xfe);
    T_CHECK_EQ(system_code_list[3], 0x0f);

    /* cleanup */
    num_of_system_codes = 0xff;
    utl_memset(system_code_list, 0xff, sizeof(system_code_list));

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
