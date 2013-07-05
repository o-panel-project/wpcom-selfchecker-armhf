/**
 * \brief    Sleep routines (Linux)
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#undef ICSLOG_MODULE
#define ICSLOG_MODULE "usl"

#include "icslog.h"
#include "ics_error.h"
#include "utl.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>

/**
 * This function sleeps for the specified time.
 *
 * \param  msec                   [IN] Sleep time. (millisecond)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_IO                Any device error.
 */
UINT32 utl_msleep(UINT32 msec)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "utl_msleep"
    int res;
    struct timeval tm;
    ICSLOG_FUNC_BEGIN;

    ICSLOG_DBG_UINT(msec);

    tm.tv_sec = (msec / 1000);
    tm.tv_usec = ((msec % 1000) * 1000);
    res = select(0, NULL, NULL, NULL, &tm);
    if (res == -1) {
        ICSLOG_ERR_STR(errno, "select()");
        return ICS_ERROR_IO;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}
