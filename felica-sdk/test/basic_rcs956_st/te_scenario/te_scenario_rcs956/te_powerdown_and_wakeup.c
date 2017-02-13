
#include "testmain.h"

static const UINT16 service_code_list_SDB200[] =
{
    0x0c09, 0x0c08, 0x0c0a, 0x0c0b, 0x0ccc, 0x0c91, 0x0cce, 0x0ccf,
    0x0c90, 0x0c97, 0x0c92, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c49
};

UINT te_powerdown_and_wakeup(void);

/**
 * This function is test program
 * for te_powerdown_and_wakeup.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT te_powerdown_and_wakeup(void)
{

#if defined(DRV_UART)

    /* initialize */
    UINT32 timeout;
    UINT rc;
    felica_card_t card;
    UINT8 num_of_service;
    UINT16 service_code_list[FELICA_CC_READ_WE_NUM_OF_SERVICES_MAX];
    UINT8 num_of_blocks;
    UINT8 block_list[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 3];
    UINT8 r_block_data[FELICA_CC_READ_WE_NUM_OF_BLOCKS_MAX * 16];
    UINT8 status_flag1;
    UINT8 status_flag2;

    /* function number */
    g_funcnum = STNO_POWERDOWN_AND_WAKEUP;

    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSS01-0000N */
    g_testnum = 0;

    TESTPR("TSS01-0000N ...\n");

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

    timeout = TE_FELICA_CC_CALC_TIMEOUT(card.pmm[5], num_of_blocks);

    /* test execution */
    rc = rcs956_power_down(&g_rcs956_dev, 0x10, timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = utl_msleep(100);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* results judgement */
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

    /* cleanup */
    utl_memset(r_block_data, 0xff, sizeof(r_block_data));
    status_flag1 = 0xff;
    status_flag2 = 0xff;

    /*********************************/
    /* terminate */
    do_testterminate();

#endif

    return (0);
}
