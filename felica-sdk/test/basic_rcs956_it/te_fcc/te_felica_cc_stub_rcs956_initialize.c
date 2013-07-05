/**
 * \brief    Test Program for felica_cc_stub_rcs956_initialize
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_felica_cc_stub_rcs956_initialize(void);

/**
 * This function is test program
 * for felica_cc_stub_rcs956_initialize.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT32 te_felica_cc_stub_rcs956_initialize(void)
{
    UINT32 rc;
    felica_cc_devf_t devf;
    ICS_HW_DEVICE bad_rcs956_dev;

    /* function number */
    g_funcnum = JTNO_FELICA_CC_STUB_RCS956_INITIALIZE;

    /* initialize */
    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    utl_memset(&devf, 0, sizeof(felica_cc_devf_t));

    /*********************************/
    /* TSU00-0800N */
    g_testnum = 0;

    TESTPR("TSJ00-0800N ...\n");

    /* build condition */

    /* set parameters */

    /* test execution */
    rc = felica_cc_stub_rcs956_initialize(&devf, &g_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(TRUE, (devf.dev == &g_rcs956_dev));
    T_CHECK_EQ(TRUE, (devf.polling_func != NULL));
    T_CHECK_EQ(TRUE, (devf.thru_func != NULL));

    /* cleanup */
    utl_memset(&devf, 0, sizeof(felica_cc_devf_t));

    /*********************************/
    /* TSU00-0801N */
    g_testnum = 1;

    TESTPR("TSJ00-0801N ...\n");

    /* build condition */
    utl_memset(&bad_rcs956_dev, 0x00, sizeof(ICS_HW_DEVICE));
    utl_memset(&devf, 0x00, sizeof(felica_cc_devf_t));

    /* set parameters */

    /* test execution */
    rc = felica_cc_stub_rcs956_initialize(&devf, &bad_rcs956_dev);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(TRUE, (devf.dev == &bad_rcs956_dev));
    T_CHECK_EQ(TRUE, (devf.polling_func != NULL));
    T_CHECK_EQ(TRUE, (devf.thru_func != NULL));

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
