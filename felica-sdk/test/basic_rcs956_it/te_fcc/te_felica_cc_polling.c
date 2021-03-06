/**
 * \brief    Test Program for felica_cc_polling
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_felica_cc_polling(void);

/**
 * This function is test program
 * for felica_cc_polling.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_felica_cc_polling(void)
{
    UINT8 polling_param[4];
    UINT32 timeout;
    UINT rc;
    UINT32 i;
    felica_card_t card;
    felica_card_option_t card_option;
    felica_cc_devf_t bad_devf;
    ICS_HW_DEVICE bad_rcs956_dev;
    UINT8 cmp_cardinfo[] = "\x00\x01\x02\x03\x04\x05\x06\x07";

    /* function number */
    g_funcnum = JTNO_FELICA_CC_POLLING;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    /*********************************/
    /* TSJ00-0000N */
    g_testnum = 0;

    TESTPR("TSJ00-0000N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    /*********************************/
    /* TSJ00-0003N */
    g_testnum = 3;

    TESTPR("TSJ00-0003N ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0xff;
    polling_param[1] = 0x18;
    polling_param[2] = 0x0f;
    polling_param[3] = 0x07;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    /*********************************/
    /* TSJ00-0006N */
    g_testnum = 6;

    TESTPR("TSJ00-0006N ...\n");

    /* build condition */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0009N */
    g_testnum = 9;

    TESTPR("TSJ00-0009N ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x03;
    polling_param[3] = 0x03;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0012N */
    g_testnum = 12;

    TESTPR("TSJ00-0012N ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x07;
    polling_param[3] = 0x07;
    timeout = 0xffffffff;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0015N */
    g_testnum = 15;

    TESTPR("TSJ00-0015N ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0xff;
    polling_param[2] = 0x0f;
    polling_param[3] = 0x0f;
    timeout = 0xffffffff;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0001N */
    g_testnum = 1;

    TESTPR("TSJ00-0001N ...\n");

    /* build condition */
    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x01;
    polling_param[3] = 0x01;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(2, card_option.option_len);
    T_CHECK_EQ(0x00, card_option.option[0]);
    T_CHECK_EQ(0x18, card_option.option[1]);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;
    utl_memset(card_option.option, 0xff, sizeof(card_option.option));

    /*********************************/
    /* TSJ00-0004N */
    g_testnum = 4;

    TESTPR("TSJ00-0004N ...\n");

    /* build condition */
    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0xff;
    polling_param[2] = 0xff;
    polling_param[3] = 0x0f;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    /*********************************/
    /* TSJ00-0007N */
    g_testnum = 7;

    TESTPR("TSJ00-0007N ...\n");

    /* build condition */
    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x01;
    polling_param[3] = 0x01;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(2, card_option.option_len);
    T_CHECK_EQ(0x00, card_option.option[0]);
    T_CHECK_EQ(0x18, card_option.option[1]);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;
    utl_memset(card_option.option, 0xff, sizeof(card_option.option));

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0010N */
    g_testnum = 10;

    TESTPR("TSJ00-0010N ...\n");

    /* build condition */
    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x04;
    polling_param[3] = 0x04;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0013N */
    g_testnum = 13;

    TESTPR("TSJ00-0013N ...\n");

    /* build condition */
    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x08;
    polling_param[3] = 0x08;
    timeout = 0xffffffff;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0016N */
    g_testnum = 16;

    TESTPR("TSJ00-0016N ...\n");

    /* build condition */
    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0xff;
    polling_param[1] = 0x18;
    polling_param[2] = 0x10;
    polling_param[3] = 0x10;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0002N */
    g_testnum = 2;

    TESTPR("TSJ00-0002N ...\n");

    /* build condition */
    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    PAUSE_SET_SDB200();

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x02;
    polling_param[3] = 0x03;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(2, card_option.option_len);
    T_CHECK_EQ(0x00, card_option.option[0]);
    T_CHECK_EQ(0x83, card_option.option[1]);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;
    utl_memset(card_option.option, 0xff, sizeof(card_option.option));

    /*********************************/
    /* TSJ00-0008N */
    g_testnum = 8;

    TESTPR("TSJ00-0008N ...\n");

    /* build condition */
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x02;
    polling_param[3] = 0x02;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(2, card_option.option_len);
    T_CHECK_EQ(0x00, card_option.option[0]);
    T_CHECK_EQ(0x83, card_option.option[1]);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;
    utl_memset(card_option.option, 0xff, sizeof(card_option.option));

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0011N */
    g_testnum = 11;

    TESTPR("TSJ00-0011N ...\n");

    /* build condition */
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_on(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x06;
    polling_param[3] = 0x06;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0014N */
    g_testnum = 14;

    TESTPR("TSJ00-0014N ...\n");

    /* build condition */
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x0e;
    polling_param[3] = 0x0e;
    timeout = 0xffffffff;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0017N */
    g_testnum = 17;

    TESTPR("TSJ00-0017N ...\n");

    /* build condition */
    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0xff;
    polling_param[1] = 0xff;
    polling_param[2] = 0xff;
    polling_param[3] = 0xff;
    timeout = 0xffffffff;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, card_option.option_len);

    PR_IDM(card.idm);
    PR_PMM(card.pmm);

    T_CHECK_NE(0, utl_memcmp(card.idm, cmp_cardinfo, sizeof(card.idm)));
    T_CHECK_NE(0, utl_memcmp(card.pmm, cmp_cardinfo, sizeof(card.pmm)));

    /* cleanup */
    utl_memcpy(card.idm, cmp_cardinfo, sizeof(card.idm));
    utl_memcpy(card.pmm, cmp_cardinfo, sizeof(card.pmm));
    card_option.option_len = 0xff;

    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0018E */
    g_testnum = 18;

    TESTPR("TSJ00-0018E ...\n");

    /* build condition */
    PAUSE_RELEASE_CHIP();

    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = felica_cc_stub_rcs956_initialize(&bad_devf, &bad_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&bad_devf, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0019E */
    g_testnum = 19;

    TESTPR("TSJ00-0019E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = rcs956_initialize(&bad_rcs956_dev, g_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&bad_devf, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0020E */
    g_testnum = 20;

    TESTPR("TSJ00-0020E ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    /* set parameters */
    polling_param[0] = 0x01;
    polling_param[1] = 0x23;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = 0;

    measurement_timeout_start();

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0021E */
    g_testnum = 21;

    TESTPR("TSJ00-0021E ...\n");

    /* build condition */
    PAUSE_RELEASE_CHIP();

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = 0x7d0;

    measurement_timeout_start();

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0022E */
    g_testnum = 22;

    TESTPR("TSJ00-0022E ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    /* set parameters */
    polling_param[0] = 0x01;
    polling_param[1] = 0x23;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = 0x2bf20;

    TESTPR("Wait for 3 minutes.\n");
    measurement_timeout_start();

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    rc = compare_timeout(timeout, NULL);
    T_CHECK_EQ(TRUE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0023E */
    g_testnum = 23;

    TESTPR("TSJ00-0023E ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x01;
    polling_param[1] = 0x23;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0024E */
    g_testnum = 24;

#if defined(DRV_UART)
    TESTPR("TSJ00-0024E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    /*********************************/
    /* TSJ00-0025E */
    g_testnum = 25;

    TESTPR("TSJ00-0025E ...\n");

    /* build condition */
    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    g_felica_dev.polling_func = NULL;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */
    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ00-0026E */
    g_testnum = 26;

    TESTPR("TSJ00-0026E ...\n");

    /* build condition */
    rc = felica_cc_stub_rcs956_initialize(&g_felica_dev, &g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x00;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    timeout = g_timeout;

    /* test execution */
    for (i = 0; i <= 0xff; i++) {
        polling_param[3] = (UINT8)i;
        if ((polling_param[3] == 0x00) ||
            (polling_param[3] == 0x01) ||
            (polling_param[3] == 0x03) ||
            (polling_param[3] == 0x07) ||
            (polling_param[3] == 0x0f)) {
                continue;
        }
        rc = felica_cc_polling(&g_felica_dev, polling_param,
                               &card, &card_option, timeout);
        /* results judgement */
        T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);
    }

    /* cleanup */

    /*********************************/
    /* TSJ00-0027E */
    g_testnum = 27;

    TESTPR("TSJ00-0027E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    polling_param[0] = 0x01;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_polling(&g_felica_dev, polling_param,
                           &card, &card_option, timeout);

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
    /* TSJ00-0028E */
    g_testnum = 28;

    TESTPR("TSJ00-0028E ...\n");

    /* build condition */

    /* set parameters */
    polling_param[0] = 0x01;
    polling_param[1] = 0x18;
    polling_param[2] = 0x00;
    polling_param[3] = 0x00;
    timeout = g_timeout;

    utl_memcpy(&bad_devf, &g_felica_dev, sizeof(bad_devf));
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_devf.dev = &bad_rcs956_dev;
    bad_rcs956_dev.handle= (ICS_HANDLE)1023;

    /* test execution */
    rc = felica_cc_polling(&bad_devf, polling_param,
                           &card, &card_option, timeout);

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
