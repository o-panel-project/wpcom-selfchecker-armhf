/**
 * \brief    the header file for ICS log facilities
 * \date     2008/10/14
 * \author   Copyright 2005,2007,2008 Sony Corporation
 */

#include "ics_types.h"
#include "utl.h"
#include "arch_icslog.h"

#ifndef ICSLOG_H_
#define ICSLOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ICSLOG_MAX_DUMP_LEN     320

#ifndef ICSLOG_PRINTF
#include <stdio.h>
#define ICSLOG_PRINTF printf
#endif

#define ICSLOG_EMERG   1
#define ICSLOG_ALERT   2
#define ICSLOG_CRIT    3
#define ICSLOG_ERR     4
#define ICSLOG_WARNING 5
#define ICSLOG_NOTICE  6
#define ICSLOG_INFO    7
#define ICSLOG_DEBUG   8

/*
 * Macros
 */

#ifdef ICSLOG_LEVEL

#define ICSLOG(level, arg) \
    do { \
        if ((level) <= ICSLOG_LEVEL) { \
            ICSLOG_PRINTF arg; \
        } \
    } while (0)

#define ICSLOG_ERR_STR(error, str) \
    do { \
        ICSLOG(ICSLOG_ERR, \
               ("E:%s:%011lu:%s:%d:%s: Error (%lu): %s\n", \
                ICSLOG_MODULE, \
                (unsigned long)utl_get_time_msec(), \
                __FILE__, __LINE__, ICSLOG_FUNC, \
                (unsigned long)(error), str)); \
    } while (0)

#define ICSLOG_ERR_PRINT(error, arg) \
    do { \
        ICSLOG(ICSLOG_ERR, \
               ("E:%s:%011lu:%s:%d:%s: Error (%lu): ", \
                ICSLOG_MODULE, \
                (unsigned long)utl_get_time_msec(), \
                __FILE__, __LINE__, ICSLOG_FUNC, \
                (unsigned long)(error))); \
        ICSLOG(ICSLOG_ERR, arg); \
    } while (0)

#define ICSLOG_DBG_PRINT(arg) \
    do { \
        ICSLOG(ICSLOG_DEBUG, \
               ("D:%s:%011lu:%s: ", ICSLOG_MODULE, \
                (unsigned long)utl_get_time_msec(), ICSLOG_FUNC)); \
        ICSLOG(ICSLOG_DEBUG, arg); \
    } while (0)

#define ICSLOG_DBG_STR(v) \
    do { \
        if ((v) != 0) { \
            ICSLOG_DBG_PRINT(("%s=%s\n", #v, v)); \
        } \
    } while (0)

#define ICSLOG_DBG_UINT(v) \
    do { \
        ICSLOG_DBG_PRINT(("%s=%lu\n", #v, (unsigned long)v)); \
    } while (0)

#define ICSLOG_DBG_INT(v) \
    do { \
        ICSLOG_DBG_PRINT(("%s=%ld\n", #v, (long)v)); \
    } while (0)

#define ICSLOG_DBG_HEX8(v) \
    do { \
        ICSLOG_DBG_PRINT(("%s=0x%02x\n", #v, (UINT8)v)); \
    } while (0)

#define ICSLOG_DBG_HEX(v) \
    do { \
        ICSLOG_DBG_PRINT(("%s=0x%08lx\n", #v, (unsigned long)v)); \
    } while (0)

#define ICSLOG_DBG_PTR(v) \
    do { \
        ICSLOG_DBG_PRINT(("%s=%p\n", #v, (const void*)v)); \
    } while (0)

#define ICSLOG_FUNC_BEGIN \
    do { \
        ICSLOG(ICSLOG_DEBUG, \
               ("D:%s:%011lu:%s: begin\n", ICSLOG_MODULE, \
                (unsigned long)utl_get_time_msec(), ICSLOG_FUNC)); \
    } while (0)

#define ICSLOG_FUNC_END \
    do { \
        ICSLOG(ICSLOG_DEBUG, \
               ("D:%s:%011lu:%s: end\n", ICSLOG_MODULE, \
                (unsigned long)utl_get_time_msec(), ICSLOG_FUNC)); \
    } while (0)

#if ICSLOG_LEVEL >= ICSLOG_DEBUG
#define ICSLOG_DUMP(data, len) \
    do { \
        UINT log_i; \
        for (log_i = 0; \
             (log_i < (UINT)(len)) && (log_i < (UINT)ICSLOG_MAX_DUMP_LEN); \
             log_i++) { \
            if ((log_i % 8) == 0) { \
                ICSLOG(ICSLOG_DEBUG, ("D:%s:%011lu:%s:DUMP:%s:", \
                                      ICSLOG_MODULE, \
                                      (unsigned long)utl_get_time_msec(), \
                                      ICSLOG_FUNC, # data)); \
            } \
            ICSLOG(ICSLOG_DEBUG, (" %02x", *((const UINT8*)(data) + log_i))); \
            if ((log_i % 8) == 7) { \
                ICSLOG(ICSLOG_DEBUG, ("\n")); \
            } \
        } \
        if ((log_i % 8) != 0) { \
            ICSLOG(ICSLOG_DEBUG, ("\n")); \
        } \
    } while (0)
#else
#define ICSLOG_DUMP(data, len)
#endif

#else /* ICSLOG_LEVEL */

#define ICSLOG(level, arg)
#define ICSLOG_ERR_STR(error, str)
#define ICSLOG_ERR_PRINT(error, arg)
#define ICSLOG_DBG_PRINT(arg)
#define ICSLOG_DBG_STR(v)
#define ICSLOG_DBG_UINT(v)
#define ICSLOG_DBG_INT(v)
#define ICSLOG_DBG_HEX8(v)
#define ICSLOG_DBG_HEX(v)
#define ICSLOG_DBG_PTR(v)
#define ICSLOG_FUNC_BEGIN
#define ICSLOG_FUNC_END
#define ICSLOG_DUMP(data, len)

#endif /* ICSLOG_LEVEL */

#ifdef __cplusplus
}
#endif

#endif /* !ICSLOG_H_ */
