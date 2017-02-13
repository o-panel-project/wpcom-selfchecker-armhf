/**
 * \brief    Test Program for rcs956_usb_open
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

#if defined(DRV_USB)
UINT32 te_rcs956_usb_open(void);

/**
 * This function is test program
 * for rcs956_usb_open.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */
UINT32 te_rcs956_usb_open(void)
{
    ICS_HW_DEVICE rcs956;
    UINT rc;
    const char* port_name;
    ICS_HW_DEVICE bad_rcs956;

    /* function number */
    g_funcnum = JTNO_RCS956_USB_OPEN;

    /* initialize */

    /*********************************/
    /* TSJ02-0200N */
    g_testnum = 0;

    TESTPR("TSJ02-0200N ...\n");

    /* build condition */

    /* set parameters */
    port_name = g_port_name;

    /* test execution */
    rc = rcs956_usb_open(&g_rcs956_dev, port_name);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), g_speed);
    T_CHECK_EQ(g_rcs956_dev.priv_value, 0);

    /* cleanup */
    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0201N */
    g_testnum = 1;

    TESTPR("TSJ02-0201N ...\n");

    /* build condition */
    port_name = g_port2_name;
    rc = rcs956_usb_open(&rcs956, port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* set parameters */
    port_name = g_port_name;

    /* test execution */
    rc = rcs956_usb_open(&g_rcs956_dev, port_name);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);
    T_CHECK_EQ(RCS956_SPEED(&g_rcs956_dev), g_speed);
    T_CHECK_EQ(g_rcs956_dev.priv_value, 0);
    T_CHECK_EQ(TRUE, (g_rcs956_dev.handle != rcs956.handle));

    /* cleanup */
    rc = rcs956_close(&rcs956);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    rc = rcs956_close(&g_rcs956_dev);
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSJ02-0202E */
    g_testnum = 2;

    TESTPR("TSJ02-0202E ...\n");

    /* build condition */

    /* set parameters */
    port_name = "ABCD";

    /* test execution */
    rc = rcs956_usb_open(&g_rcs956_dev, port_name);

    /* results judgement */
    T_CHECK_EQ(ICS_ERROR_IO, rc);

    /* cleanup */

    /*********************************/
    /* TSJ02-0203E */
    g_testnum = 3;

    TESTPR("TSJ02-0203E ...\n");

    /* build condition */
    rc = rcs956_usb_open(&g_rcs956_dev, g_port_name);

    /* set parameters */
    port_name = g_port_name;
    utl_memcpy(&bad_rcs956, &g_rcs956_dev, sizeof(g_rcs956_dev));

    /* test execution */
    rc = rcs956_usb_open(&bad_rcs956, port_name);

    /* results judgement */
#if defined(ARCH_LINUX)
    T_CHECK_EQ(ICS_ERROR_IO, rc);
#else
    T_CHECK_EQ(ICS_ERROR_PERMISSION, rc);
#endif

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}
#endif
