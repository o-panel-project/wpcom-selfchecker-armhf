/**
 * \brief    Test Program for felica_cc_request_system_code
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_felica_cc_request_system_code(void);

/**
 * This function is test program
 * for felica_cc_request_system_code.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT32 te_felica_cc_request_system_code(void)
{
    UINT8 max_system_codes;
    UINT8 num_of_system_codes;
    UINT8 system_code_list[4];
    UINT32 timeout;
    UINT rc;
    felica_card_t card;
    felica_cc_devf_t bad_devf;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_FELICA_CC_REQUEST_SYSTEM_CODE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    num_of_system_codes = 0xff;
    utl_memset(system_code_list, 0xff, sizeof(system_code_list));

    /*********************************/
    /* TSU00-0700N */
    g_testnum = 0;

    TESTPR("TSJ00-0700N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 1;
    timeout = g_timeout;

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
    /* TSU00-0701N */
    g_testnum = 1;

    TESTPR("TSJ00-0701N ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 2;
    timeout = g_timeout;

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
    /* TSU00-0702N */
    g_testnum = 2;

    TESTPR("TSJ00-0702N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = FELICA_CC_REQUEST_SYSTEM_CODE_MAX_NUM_OF_SYSTEM_CODES_MAX;
    timeout = 0xffffffff;

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
    /* TSU00-0703E */
    g_testnum = 3;

    TESTPR("TSJ00-0703E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = felica_cc_stub_rcs956_initialize(&bad_devf, &bad_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 1;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_system_code(&bad_devf, &card,
                                        max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSU00-0704E */
    g_testnum = 4;

    TESTPR("TSJ00-0704E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = rcs956_initialize(&bad_rcs956_dev, g_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 1;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_system_code(&bad_devf, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSU00-0705E */
    g_testnum = 5;

    TESTPR("TSJ00-0705E ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    card.idm[7] += 1;

    /* set parameters */
    max_system_codes = 1;
    timeout = 0x8000;

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSU00-0706E */
    g_testnum = 6;

    TESTPR("TSJ00-0706E ...\n");

    /* build condition */
    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 1;
    timeout = 1000;

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSU00-0707E */
    g_testnum = 7;

#if defined(DRV_UART)
    TESTPR("TSJ00-0707E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 1;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    /*********************************/
    /* TSU00-0708E */
    g_testnum = 8;

    TESTPR("TSJ00-0708E ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 1;
    timeout = 0xffffffff;

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */

    /*********************************/
    /* TSU00-0709E */
    g_testnum = 9;

    TESTPR("TSJ00-0709E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 1;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

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
    /* TSU00-0710E */
    g_testnum = 10;

    TESTPR("TSJ00-0710E ...\n");

    /* build condition */

    /* set parameters */
    max_system_codes = 1;
    timeout = g_timeout;

    utl_memcpy(&bad_devf, &g_felica_dev, sizeof(bad_devf));
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_devf.dev = &bad_rcs956_dev;
    bad_rcs956_dev.handle= (ICS_HANDLE)1023;

    /* test execution */
    rc = felica_cc_request_system_code(&bad_devf, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

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
    /* TSU00-0711E */
    g_testnum = 11;

    TESTPR("TSJ00-0711E ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 0;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);
    T_CHECK_EQ(num_of_system_codes, 0x01);

    /* cleanup */

    /*********************************/
    /* TSU00-0712E */
    g_testnum = 12;

    TESTPR("TSJ00-0712E ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    max_system_codes = 1;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_system_code(&g_felica_dev, &card,
                                       max_system_codes, &num_of_system_codes,
                                       system_code_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_BUF_OVERFLOW, rc);
    T_CHECK_EQ(num_of_system_codes, 0x02);
    T_CHECK_EQ(system_code_list[0], 0x00);
    T_CHECK_EQ(system_code_list[1], 0x18);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
