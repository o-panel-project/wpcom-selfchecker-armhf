
#include "testmain.h"

static const UINT16 service_code_list_rcs860[] =
{
    0x1009, 0x1008, 0x100a, 0x100b, 0x100c, 0x100d, 0x100e, 0x100f,
    0x1010, 0x1011, 0x1012, 0x1013, 0x1014, 0x1015, 0x1016, 0x1209,
};

static const UINT16 service_code_list_SDB200[] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0c91, 0x0cce, 0x0ccf,
    0x0c90, 0x0c97, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c49
};

UINT te_chg_rwmode(void);


/**
 * This function is test program
 * for te_chg_rwmode.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT te_chg_rwmode(void)
{
    /* initialize */
    UINT32 timeout;
    UINT rc;
    UINT32 cnt = 0;
    felica_card_t card;
    UINT8 num_of_service;
    UINT16 service_code_list[FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX];
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 r_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 w_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;

    /* function number */
    g_funcnum = STNO_CHG_RWMODE;

    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSS01-0200N */
    g_testnum = 0;

    TESTPR("TSS01-0200N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

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

    /* test execution */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    /* results judgement */

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* TSS01-0201N */
    g_testnum = 1;

    TESTPR("TSS01-0201N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

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
                                            7, &block_list[16],
                                            &w_block_data[128], &status_flag1,
                                            &status_flag2, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    /* test execution */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_read_without_encryption(&g_felica_dev, &card,
                                           num_of_service, service_code_list,
                                           num_of_blocks, block_list,
                                           r_block_data, &status_flag1,
                                           &status_flag2, timeout);

    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(status_flag1, 0x00);
    T_CHECK_EQ(status_flag2, 0x00);

    for (cnt = 0; cnt < (UINT32)(num_of_blocks * 16); cnt++) {
        T_CHECK_EQ(r_block_data[cnt], (UINT8)cnt);
    }

    /* results judgement */

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
