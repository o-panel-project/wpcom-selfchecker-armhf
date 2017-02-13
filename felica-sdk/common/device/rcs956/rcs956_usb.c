/**
 * \brief    RC-S956 USB Driver
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#undef ICSLOG_MODULE
#define ICSLOG_MODULE "DGB"

#include "ics_types.h"
#include "ics_error.h"
#include "icslib_chk.h"
#include "icslog.h"
#include "utl.h"

#include "rcs956_usb.h"

/* --------------------------------
 * Function
 * -------------------------------- */

/* ------------------------
 * Exported
 * ------------------------ */

/**
 * This function opens a port to the device.
 *
 * \param  rcs956                [OUT] Handle to access the port.
 * \param  port_name              [IN] The port name to open.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_BUSY              Device busy.
 * \retval ICS_ERROR_PERMISSION        Permission denied.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_usb_open(
    ICS_HW_DEVICE* rcs956,
    const char* port_name)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_usb_open"
    UINT32 rc;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(port_name, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_STR(port_name);

    rc = rcs956_initialize(rcs956, &rcs956_usb_raw_func);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_initialize()");
        return rc;
    }

    rc = rcs956_open(rcs956, port_name);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_open()");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}
