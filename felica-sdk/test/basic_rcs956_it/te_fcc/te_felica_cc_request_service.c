/**
 * \brief    Test Program for felica_cc_request_service
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_felica_cc_request_service(void);

const static UINT16 node_code_list_SDB200[32] =
{
    0x0c08, 0x0c09, 0x0c0a, 0x0c0b, 0x0c48, 0x0c49, 0x0c4a, 0x0d94,
    0x0c90, 0x0c91, 0x0d95, 0x0c93, 0x0c94, 0x0c95, 0x0c96, 0x0c97,
    0x0ccc, 0x0ccd, 0x0cce, 0x0ccf, 0x0d08, 0x0d09, 0x0d0a, 0x0d0b,
    0x0d48, 0x0d49, 0x0d4a, 0x0d96, 0x0d90, 0x0d91, 0x0d97, 0x0d93
};

const static UINT16 Key_version_list_SDB200[32] =
{
    0x0c08, 0x0000, 0x0c0a, 0x0000, 0x0c48, 0x0000, 0x0c4a, 0x0d94,
    0x0c90, 0x0000, 0x0000, 0x0000, 0x0c94, 0x0000, 0x0c96, 0x0000,
    0x0ccc, 0x0000, 0x0cce, 0x0000, 0x0d08, 0x0000, 0x0d0a, 0x0000,
    0x0d48, 0x0000, 0x0d4a, 0x0d96, 0x0d90, 0x0000, 0x0000, 0x0000
};

/**
 * This function is test program
 * for felica_cc_request_service.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT32 te_felica_cc_request_service(void)
{
    UINT8 num_of_nodes;
    UINT16 node_code_list[FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX];
    UINT16 node_key_version_list[FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX];
    UINT32 timeout;
    UINT rc;
    felica_card_t card;
    felica_cc_devf_t bad_devf;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_FELICA_CC_REQUEST_SERVICE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSJ00-0200N */
    g_testnum = 0;

    TESTPR("TSJ00-0200N ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = g_timeout;

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
    /* TSJ00-0201N */
    g_testnum = 1;

    TESTPR("TSJ00-0201N ...\n");

    /* build condition */
    PAUSE_SET_RCX407();

    rc = do_polling_to_rcx407(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = 2;
    node_code_list[0] = 0x0c08;
    node_code_list[1] = 0x1111;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(node_key_version_list[0], 0x0c08);
    T_CHECK_EQ(node_key_version_list[1], 0xffff);

    /* cleanup */
    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSJ00-0202N */
    g_testnum = 2;

    TESTPR("TSJ00-0202N ...\n");

    /* build condition */
    PAUSE_SET_SDB200();

    rc = do_polling_to_sdb200(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MAX;
    utl_memcpy(node_code_list, node_code_list_SDB200, sizeof(node_code_list));
    timeout = 0xffffffff;

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(0, utl_memcmp(node_key_version_list,
                             Key_version_list_SDB200,
                             sizeof(node_key_version_list)));

    /* cleanup */
    utl_memset(node_key_version_list, 0xff, sizeof(node_key_version_list));

    /*********************************/
    /* TSJ00-0203E */
    g_testnum = 3;

    TESTPR("TSJ00-0203E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = felica_cc_stub_rcs956_initialize(&bad_devf, &bad_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_service(&bad_devf, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0204E */
    g_testnum = 4;

    TESTPR("TSJ00-0204E ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&bad_devf, 0x00, sizeof(felica_cc_devf_t));

    rc = rcs956_initialize(&bad_rcs956_dev, g_raw_func);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_service(&bad_devf, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_PARAM, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0205E */
    g_testnum = 5;

    TESTPR("TSJ00-0205E ...\n");

    /* build condition */
    PAUSE_RELEASE_CHIP();

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = 0;

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0206E */
    g_testnum = 6;

    TESTPR("TSJ00-0206E ...\n");

    /* build condition */
    PAUSE_SET_RCS860();

    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0207E */
    g_testnum = 7;

#if defined(DRV_UART)
    TESTPR("TSJ00-0207E ...\n");

    /* build condition */
    rc = rcs956_set_speed(&g_rcs956_dev, 9600);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_TIMEOUT, rc);

    /* cleanup */
    rc = rcs956_set_speed(&g_rcs956_dev, g_speed);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
#endif
    /*********************************/
    /* TSJ00-0208E */
    g_testnum = 8;

    TESTPR("TSJ00-0208E ...\n");

    /* build condition */
    rc = do_polling_to_rcs860(&card);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_INVALID_RESPONSE, rc);

    /* cleanup */

    /*********************************/
    /* TSJ00-0209E */
    g_testnum = 9;

    TESTPR("TSJ00-0209E ...\n");

    /* build condition */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = g_timeout;

    /* test execution */
    rc = felica_cc_request_service(&g_felica_dev, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

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
    /* TSJ00-0210E */
    g_testnum = 10;

    TESTPR("TSJ00-0210E ...\n");

    /* build condition */

    /* set parameters */
    num_of_nodes = FELICA_CC_REQUEST_SERVICE_NUM_OF_NODES_MIN;
    node_code_list[0] = 0x1009;
    timeout = g_timeout;

    utl_memcpy(&bad_devf, &g_felica_dev, sizeof(bad_devf));
    utl_memcpy(&bad_rcs956_dev, &g_rcs956_dev, sizeof(g_rcs956_dev));
    bad_devf.dev = &bad_rcs956_dev;
    bad_rcs956_dev.handle= (ICS_HANDLE)1023;

    /* test execution */
    rc = felica_cc_request_service(&bad_devf, &card,
                                   num_of_nodes, node_code_list,
                                   node_key_version_list, timeout);

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
