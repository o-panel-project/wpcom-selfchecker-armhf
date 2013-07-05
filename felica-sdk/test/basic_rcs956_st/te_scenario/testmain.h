/**
 * \brief    Test Main Header
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#ifndef TESTMAIN_H_
#define TESTMAIN_H_

#include <stdlib.h>
#include <string.h>

#ifndef TESTPR
#include <stdio.h>
#define TESTPR printf
#endif

#include "felica_card.h"
#include "felica_cc.h"
#include "felica_cc_stub.h"
#include "felica_cc_stub_rcs956.h"

#include "rcs956.h"
#include "rcs956_uart.h"

#if !defined(ARCH_WINXP)
#include "rcs956_usb.h"
#endif

#include "ics_types.h"
#include "ics_error.h"
#include "ics_hwdev.h"
#include "icsdrv.h"
#include "icsdrv_uart.h"
#include "icslib_chk.h"
#include "icslog.h"

#include "arch_ics_types.h"
#include "arch_icslog.h"
#include "arch_utl.h"

#include "utl.h"

#ifndef TESTPR_FLUSH
#include <stdio.h>
#define TESTPR_FLUSH fflush(stdout)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* global */
extern UINT16 g_funcnum;
extern UINT8 g_testnum;

extern char* g_port_name;
extern UINT32 g_speed;
extern UINT32 g_timeout;

extern ICS_HW_DEVICE g_rcs956_dev;
extern felica_cc_devf_t g_felica_dev;
extern icsdrv_raw_func_t* g_raw_func;

/* waiting time */
#define WAIT_POWER_DOWN 10
#define WAIT_BEFOR_CANCEL 100

/* check macro */
#define T_CHECK(cond) { \
    if (!(cond)) { \
        TESTPR("#error:%d: %s\n", __LINE__, #cond); \
        return (0xffff); \
    } \
}

#define T_CHECK_EQ(first, second) { \
    if ((first) != (second)) { \
        TESTPR("#error:%d: %s != %s(%d != %d)\n", \
                __LINE__, #first, #second, (UINT)first, (UINT)second); \
        return (0xffff); \
    } \
}

#define T_CHECK_NE(first, second) { \
    if ((first) == (second)) { \
        TESTPR("#error:%d: %s == %s(%d == %d)\n",\
                __LINE__, #first, #second, (UINT)first, (UINT)second); \
        return (0xffff); \
    } \
}

#define PAUSE(msg) { \
    TESTPR((msg)); \
    while (getchar() != '\n'){} \
}

#define PAUSE_SET_RCS860() { \
    PAUSE("Set felica chip: [RC-S860] ( >> Push 'Enter')\n"); \
}

#define PAUSE_SET_RCX407() { \
    PAUSE("Set felica chip: [RCX-407] ( >> Push 'Enter')\n"); \
}

#define PAUSE_SET_SDB200() { \
    PAUSE("Set felica chip: [SDB-200] ( >> Push 'Enter')\n"); \
}

#define PAUSE_RELEASE_CHIP() { \
    PAUSE("Release felica chip ( >> Push 'Enter')\n"); \
}

#define PAUSE_SET_AND_RELEASE_CHIP_RCS860() { \
    PAUSE("Set and Release felica chip: [RC-S860] ( >> Push Enter)\n"); \
}

#define PAUSE_SET_AND_RELEASE_CHIP_RCX407() { \
    PAUSE("Set and Release felica chip: [RCX-407] ( >> Push Enter)\n"); \
}

#define PAUSE_SET_AND_RELEASE_CHIP_SDB200() { \
    PAUSE("Set and Release felica chip: [SDB-200] ( >> Push Enter)\n"); \
}

#define PAUSE_DISCONNECT() { \
    PAUSE("Disconnect Serial Cable ( >> Push Enter)\n"); \
}

#define PAUSE_CONNECT() { \
    PAUSE("Connect Serial Cable\n"); \
}

#define PR_IDM(idm) \
    TESTPR("felica IDm: [ %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x ]\n", \
           idm[0], idm[1], idm[2], idm[3], idm[4], idm[5], idm[6], idm[7])

#define PR_PMM(pmm) \
    TESTPR("felica PMm: [ %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x ]\n", \
           pmm[0], pmm[1], pmm[2], pmm[3], pmm[4], pmm[5], pmm[6], pmm[7])

#define PR_TESTNO(testno) { \
    TESTPR(testno); \
    g_testcnt++; \
}

#define TE_FELICA_CC_CALC_TIMEOUT(x, nblocks) \
    FELICA_CC_CALC_TIMEOUT(x, nblocks) + 16

/* Polling test */
#if defined(DRV_UART)
#define POLLING_TIMEOUT_AT_DEVICE        40
#else
#define POLLING_TIMEOUT_AT_DEVICE        100

#endif
#define DEFAULT_LOOP_TIMES               1
#define DEFAULT_MAX_POLLING_TIMES        1000
#define N_POLLINGS_PER_CYCLE             50
#define N_CYCLES_PER_LOOP                20

/* test numbers */
typedef enum _enum_test_function_number
{
    STNO_SCENARIO_TOP           = 0x0100,

    /* Scenario fcc */
    STNO_POLLING,
    STNO_READ_WRITE,
    STNO_PMM,
    STNO_RETRY_FCC,

    /* Scenario rcs956 */
    STNO_POWERDOWN_AND_WAKEUP,
    STNO_CHG_TRANSMISSION_SPEED,
    STNO_CHG_RWMODE,
    STNO_RETRY_RCS956,

    STNO_END

} E_TEST_FUNC_NUM;

extern UINT32 do_testinitialize(void);
extern void do_testterminate(void);
extern UINT32 do_polling_to_rcs860(felica_card_t *card);
extern UINT32 do_polling_to_rcx407(felica_card_t *card);
extern UINT32 do_polling_to_sdb200(felica_card_t *card);
extern void retry_result_init(void);
extern void retry_recovery(void);
extern void retry_result_cntup(UINT rc);
extern void retry_result_print(void);

#ifdef __cplusplus
}
#endif

#endif /* !TESTMAIN_H_ */
