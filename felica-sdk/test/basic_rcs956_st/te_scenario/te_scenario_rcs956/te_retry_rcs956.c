
#include "testmain.h"

#if defined(DRV_UART)
static UINT16 read_service_code_list_SDB200_sub[16] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0c49, 0x0cce, 0x0ccf,
    0x0c90, 0x0c97, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c91
};

static UINT8 read_service_code_list_SDB200[] = "\x09\x0c\x08\x0c\x0a\x0c\x0b\x0c"
                                               "\xcc\x0c\x49\x0c\xce\x0c\xcf\x0c"
                                               "\x90\x0c\x97\x0c\x92\x0c\x93\x0c"
                                               "\x94\x0c\x95\x0c\x96\x0c\x91\x0c";

static const UINT16 service_code_list_SDB200[] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0c91, 0x0cce, 0x0ccf,
    0x0c90, 0x0c97, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c49
};
#endif

UINT te_retry_rcs956(void);

/**
 * This function is test program
 * for te_retry_rcs956.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT te_retry_rcs956(void)
{

#if defined(DRV_UART)

    /* initialize */
    UINT32 timeout;
    UINT32 command_timeout;
    UINT rc;
    UINT32 cnt = 0;
    felica_card_t card;
    felica_card_option_t card_option;
    UINT8 polling_param[4];
    UINT8 command[RCS956_MAX_FELICA_COMMAND_LEN];
    UINT32 command_len;
    UINT32 max_response_len;
    UINT8 response[RCS956_MAX_FELICA_RESPONSE_LEN];
    UINT32 response_len;
    UINT8 num_of_service;
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 w_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;
    UINT8 ic_type;
    UINT16 version;
    UINT8 cmp_cardinfo[] = "\x00\x01\x02\x03\x04\x05\x06\x07";
    UINT16 service_code_list[FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX];
    UINT8 r_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];

    /* function number */
    g_funcnum = STNO_RETRY_RCS956;

    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(command, 0x00, sizeof(command));
    utl_memset(response, 0x00, sizeof(response));

    ic_type = 0xff;
    version = 0xffff;

    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    /*********************************/
    /* TSS01-0300N */
    g_testnum = 0;

    TESTPR("TSS01-0300N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_initialize_device >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_initialize_device(&g_rcs956_dev, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS01-0301N */
    g_testnum = 1;

    TESTPR("TSS01-0301N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_uart_initialize_device >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_uart_initialize_device(&g_rcs956_dev, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS01-0302N */
    g_testnum = 2;

    TESTPR("TSS01-0302N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    TESTPR("Please leave putting ...\n");

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    num_of_service = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;

    num_of_blocks = FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX;
    utl_memcpy(block_list,
               (const UINT8*)"\x00\x00\x00\x00\x01\x00\x00\x02\x00"
               "\x00\x03\x00\x00\x04\x00\x00\x05\x00\x00\x06\x00"
               "\x00\x07\x00\x05\x00\x00\x05\x01\x00\x05\x02\x00"
               "\x05\x03\x00\x05\x04\x00\x05\x05\x00\x0f\x00\x00",
               (UINT)(num_of_blocks * 3));

    utl_memcpy(service_code_list,
               read_service_code_list_SDB200_sub, 
               sizeof(read_service_code_list_SDB200_sub));

    for (cnt = 0;
        cnt < (UINT32)(FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16); cnt++) {
        w_block_data[cnt] = (UINT8)cnt;
    }

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[6], num_of_blocks);

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            service_code_list,
                                            10, block_list,
                                            w_block_data, &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    status_flag1 = 0xff;
    status_flag2 = 0xff;

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service,
                                            service_code_list,
                                            5, &block_list[10 * 3],
                                            &w_block_data[10 * 16],
                                            &status_flag1,
                                            &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* set parameters */
    command[0] = 0x06;
    utl_memcpy(command + 1, card.idm, 8);

    command[9] = FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX;
    utl_memcpy(command + 10,
               read_service_code_list_SDB200,
               sizeof(read_service_code_list_SDB200) - 1);

    command[10 + sizeof(read_service_code_list_SDB200) - 1] = 0x0f;
    utl_memcpy(command + 11 + sizeof(read_service_code_list_SDB200) - 1,
               block_list, (UINT)(num_of_blocks * 3));

    command_len = 88;
    max_response_len = 252;
    command_timeout = 1000;
    timeout = g_timeout;

    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_felica_command >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_felica_command(&g_rcs956_dev, command, command_len,
                                   max_response_len, response, &response_len,
                                   command_timeout, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    T_CHECK_EQ(0, memcmp(response, "\x07", 1));
    T_CHECK_EQ(0, memcmp(response + 1, card.idm, 8));
    T_CHECK_EQ(0, memcmp(response + 9, "\x00\x00", 2));

    T_CHECK_EQ(0x0f, response[11]);
    T_CHECK_EQ(0, utl_memcmp(response + 12,
                             w_block_data, 219));
    T_CHECK_EQ(252, response_len);

    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS01-0303N */
    g_testnum = 3;

    TESTPR("TSS01-0303N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_rf_off >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_rf_off(&g_rcs956_dev, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS01-0304N */
    g_testnum = 4;

    TESTPR("TSS01-0304N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_rf_on >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_rf_on(&g_rcs956_dev, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    retry_result_print();

    /* cleanup */
    rc = rcs956_rf_off(&g_rcs956_dev, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSS01-0305N */
    g_testnum = 5;

    TESTPR("TSS01-0305N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_reset >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_reset(&g_rcs956_dev, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS01-0306N */
    g_testnum = 6;

    TESTPR("TSS01-0306N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_set_dev_speed >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_set_dev_speed(&g_rcs956_dev, 9600, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    retry_result_print();

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_dev_speed(&g_rcs956_dev, g_speed, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSS01-0307N */
    g_testnum = 7;

    TESTPR("TSS01-0307N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_get_firmware_version >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_get_firmware_version(&g_rcs956_dev,
                                         &ic_type, &version, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    T_CHECK_EQ(0x33, ic_type);
    T_CHECK_EQ(0x0130, version);

    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS01-0308N */
    g_testnum = 8;

    TESTPR("TSS01-0308N ...\n");

    /* set parameters */
    timeout = g_timeout;

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry rcs956_set_rf_speed >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x02, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    rc = rcs956_set_rf_speed(&g_rcs956_dev, 0x01, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS01-0309N */
    g_testnum = 9;

    TESTPR("TSS01-0309N ...\n");

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x01;
    polling_param[3] = 0x01;

    timeout = g_timeout;

    /* build condition */
    PAUSE_SET_SDB200();

    TESTPR("Please leave putting ...\n");

    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry felica_cc_polling >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
        rc = felica_cc_polling(&g_felica_dev, polling_param,
                               &card, &card_option, timeout);
        retry_result_cntup(rc);
        if (rc != ICS_ERROR_SUCCESS) {
            retry_recovery();
        } else {
            break;
        }
    }

    /* results judgement */
    T_CHECK_EQ(2, card_option.option_len);
    T_CHECK_EQ(0x00, card_option.option[0]);
    T_CHECK_EQ(0x18, card_option.option[1]);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    retry_result_print();

    /* cleanup */

    /*********************************/
    /* TSS01-0310N */
    g_testnum = 10;

    TESTPR("TSS01-0310N ...\n");

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

    TESTPR("Please leave putting ...\n");

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

    status_flag1 = 0xff;
    status_flag2 = 0xff;

    rc = felica_cc_write_without_encryption(&g_felica_dev, &card,
                                            num_of_service, service_code_list,
                                            4, &block_list[16],
                                            &w_block_data[128], &status_flag1,
                                            &status_flag2, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    PAUSE_DISCONNECT();

    TESTPR("Please wait in 5 seconds and set felica chip. \n");
    TESTPR("Retry felica_cc_read_without_encryption >>>>> \n");

    retry_result_init();

    /* test execution */
    for ( cnt = 0; ; cnt++ ) {
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
#endif
    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
