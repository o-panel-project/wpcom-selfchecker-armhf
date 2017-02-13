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

#if defined(DRV_USB)
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

#ifdef __cplusplus
extern "C" {
#endif

/* global */
extern UINT16 g_funcnum;
extern UINT8 g_testnum;

extern char* g_port_name;
extern char* g_port2_name;
extern UINT32 g_speed;
extern UINT32 g_timeout;

extern ICS_HW_DEVICE g_rcs956_dev;
extern felica_cc_devf_t g_felica_dev;
extern icsdrv_raw_func_t* g_raw_func;

/* waiting time */
#define WAIT_POWER_DOWN 10

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

/* pause macro */
#define PAUSE(msg) { \
    TESTPR((msg)); \
    while (getchar() != '\n'){} \
}

#define PAUSE_SET_RCS860() { \
    PAUSE("Set felica chip: [Sample5N] ( >> Push 'Enter')\n"); \
}

#define PAUSE_SET_RCX407() { \
    PAUSE("Set felica chip: [RCX-407] ( >> Push 'Enter')\n"); \
}

#define PAUSE_SET_SDB200() { \
    PAUSE("Set felica chip: [SDB-200(S1)] ( >> Push 'Enter')\n"); \
}

#define PAUSE_RELEASE_CHIP() { \
    PAUSE("Release felica chip ( >> Push 'Enter')\n"); \
}

#define PAUSE_RW_REACTIVATE() { \
    PAUSE("Reset R/W ( >> Push 'Enter')\n"); \
}

#define PAUSE_SET_EXTERNAL_RF() { \
    PAUSE("Set external RF(Set another RC-S956 and execute sample_polling) ( >> Push 'Enter')\n"); \
}

#define PAUSE_RELEASE_RF() { \
    PAUSE("Release external RF ( >> Push 'Enter')\n"); \
}

#define PR_IDM(idm) \
    TESTPR("FeliCa IDm: [ %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x ]\n", \
           idm[0], idm[1], idm[2], idm[3], idm[4], idm[5], idm[6], idm[7])

#define PR_PMM(pmm) \
    TESTPR("FeliCa PMm: [ %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x ]\n", \
           pmm[0], pmm[1], pmm[2], pmm[3], pmm[4], pmm[5], pmm[6], pmm[7])

/* test numbers */
typedef enum _enum_test_function_number
{
    /* FeliCa card command library */
    JTNO_FCC_TOP              = 0x0000,
    JTNO_FELICA_CC_POLLING,
    JTNO_FELICA_CC_POLLING_MULTIPLE,
    JTNO_FELICA_CC_REQUEST_SERVICE,
    JTNO_FELICA_CC_REQUEST_RESPONSE,
    JTNO_FELICA_CC_READ_WITHOUT_ENCRYPTION,
    JTNO_FELICA_CC_WRITE_WITHOUT_ENCRYPTION,
    JTNO_FELICA_CC_SEARCH_SERVICE_CODE,
    JTNO_FELICA_CC_REQUEST_SYSTEM_CODE,
    JTNO_FELICA_CC_STUB_RCS956_INITIALIZE,

    JTNO_FELICA_CC_COMMUNICATION_TEST,

    /* RC-S956 driver */
    JTNO_RCS956_TOP           = 0x0100,
    JTNO_RCS956_INITIALIZE,
    JTNO_RCS956_OPEN,
    JTNO_RCS956_UART_OPEN,
    JTNO_RCS956_USB_OPEN,
    JTNO_RCS956_CLOSE,
    JTNO_RCS956_INITIALIZE_DEVICE,
    JTNO_RCS956_EXECUTE_COMMAND,
    JTNO_RCS956_SEND_ACK,
    JTNO_RCS956_PING,
    JTNO_RCS956_WAKE_UP,
    JTNO_RCS956_CANCEL_COMMAND,
    JTNO_RCS956_FELICA_COMMAND,
    JTNO_RCS956_RF_OFF,
    JTNO_RCS956_RF_ON,
    JTNO_RCS956_RESET,
    JTNO_RCS956_SET_DEV_SPEED,
    JTNO_RCS956_SET_SPEED,
    JTNO_RCS956_CLEAR_RX_QUEUE,
    JTNO_RCS956_GET_ACK_TIME,
    JTNO_RCS956_GET_FIRMWARE_VERSION,
    JTNO_RCS956_POWER_DOWN,
    JTNO_RCS956_SET_RF_SPEED,
    JTNO_RCS956_WAIT_FOR_WAKE_UP,
    JTNO_RCS956_UART_INITIALIZE_DEVICE,
    
    JTNO_END

} E_TEST_FUNC_NUM;

extern UINT32 do_testinitialize(void);
extern void do_testterminate(void);
extern UINT32 do_polling_to_rcs860(felica_card_t *card);
extern UINT32 do_polling_to_rcx407(felica_card_t *card);
extern UINT32 do_polling_to_sdb200(felica_card_t *card);
extern void measurement_timeout_start(void);
extern BOOL compare_timeout(UINT32 api_timeout, UINT32* exe_time);
extern UINT32 send_diagnose_command(void);
extern UINT32 send_get_firmware_version_command(void);

#ifdef __cplusplus
}
#endif

#endif /* !TESTMAIN_H_ */
