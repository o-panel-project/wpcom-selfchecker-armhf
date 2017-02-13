/**
 * \brief    Test Program for rcs956_initialize
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

UINT32 te_rcs956_initialize(void);

/**
 * This function is test program
 * for rcs956_initialize.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_rcs956_initialize(void)
{
    UINT rc;

    /* function number */
    g_funcnum = JTNO_RCS956_INITIALIZE;

    /* initialize */
#if defined(DRV_UART)
    /*********************************/
    /* TSJ01-0000N */
    g_testnum = 0;

    TESTPR("TSJ01-0000N ...\n");

    /* build condition */

    /* set parameters */

    /* test execution */

    rc = rcs956_initialize(&g_rcs956_dev, &icsdrv_uart_raw_func);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(TRUE, (&icsdrv_uart_raw_func == g_rcs956_dev.priv_data));

    /* cleanup */
#else
    /*********************************/
    /* TSJ02-0000N */
    g_testnum = 0;

    TESTPR("TSJ02-0000N ...\n");

    /* build condition */

    /* set parameters */

    /* test execution */

    rc = rcs956_initialize(&g_rcs956_dev, &rcs956_usb_raw_func);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(TRUE, (&rcs956_usb_raw_func == g_rcs956_dev.priv_data));

    /* cleanup */
#endif

    return (0);
}
