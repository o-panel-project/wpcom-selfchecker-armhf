/**
 * \brief    Test Program for felica_cc_request_response
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_felica_cc_request_response(void);

/**
 * This function is test program
 * for felica_cc_request_response.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_felica_cc_request_response(void)
{
    UINT8 mode;
    UINT32 timeout;
    UINT32 rc;
    felica_card_t card;
    felica_cc_devf_t bad_devf;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_FELICA_CC_REQUEST_RESPONSE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    mode = 0xff;

    /*********************************/
    /* TSJ00-0300N */
    g_testnum = 0;

    TESTPR("TSJ00-0300N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSJ00-0301N */
    g_testnum = 1;

    TESTPR("TSJ00-0301N ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSJ00-0302N */
    g_testnum = 2;

    TESTPR("TSJ00-0302N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = 0xffffffff;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(mode, 0x00);

    /* cleanup */
    mode = 0xff;

    /*********************************/
    /* TSJ00-0303E */
    g_testnum = 3;

    TESTPR("TSJ00-0303E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = felica_cc_stub_rcs956_initialize(&bad_devf, &bad_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_response(&bad_devf, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0304E */
    g_testnum = 4;

    TESTPR("TSJ00-0304E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = rcs956_initialize(&bad_rcs956_dev, g_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_response(&bad_devf, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0305E */
    g_testnum = 5;

    TESTPR("TSJ00-0305E ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    card.idm[7] += 1;

    /* set parameters */
    timeout = 0x8000;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0306E */
    g_testnum = 6;

    TESTPR("TSJ00-0306E ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = 1000;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0307E */
    g_testnum = 7;

#if defined(DRV_UART)
    TESTPR("TSJ00-0307E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    /*********************************/
    /* TSJ00-0308E */
    g_testnum = 8;

    TESTPR("TSJ00-0308E ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = 0xffffffff;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0309E */
    g_testnum = 9;

    TESTPR("TSJ00-0309E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_response(&g_felica_dev, &card, &mode, timeout);

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
    /* TSJ00-0310E */
    g_testnum = 10;

    TESTPR("TSJ00-0310E ...\n");

    /* build condition */

    /* set parameters */
    timeout = g_timeout;

    utl_memcpy(&bad_devf, &g_felica_dev, sizeof(bad_devf));
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_devf.dev = &bad_rcs956_dev;
    bad_rcs956_dev.handle= (ICS_HANDLE)1023;

    /* test execution */
    rc = felica_cc_request_response(&bad_devf, &card, &mode, timeout);

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
    /* terminate */
    do_testterminate();

    return (0);
}
