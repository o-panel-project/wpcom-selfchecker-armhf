/**
 * \brief    The Main Test Program
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "testmain.h"

/* Information of Test Function */
typedef struct testcase_t {
    char    *name;
    UINT32  (*func)(void);
} testcase_t;

#define NOTESTCASE (0x5555)

#define TESTCASE(x) {(char*)#x, x}
#define TESTTOOL_NCASES(a) (sizeof(a) / sizeof(a[0]))

/* Default Port Name */
#define DEFAULT_WINXP_UART_PORT1NAME (char*)"COM1"
#define DEFAULT_WINXP_UART_PORT2NAME (char*)"COM2"
#define DEFAULT_LINUX_UART_PORT1NAME (char*)"/dev/ttyS0"
#define DEFAULT_LINUX_UART_PORT2NAME (char*)"/dev/ttyS1"
#define DEFAULT_LINUX_USB_PORT1NAME (char*)"/dev/bus/usb/001/001"
#define DEFAULT_LINUX_USB_PORT2NAME (char*)"/dev/bus/usb/001/002"
#define DEFAULT_WINCE_UART_PORT1NAME (char*)"COM1:"
#define DEFAULT_WINCE_UART_PORT2NAME (char*)"COM2:"
#define DEFAULT_WINCE_USB_PORT1NAME (char*)"FGU1:"
#define DEFAULT_WINCE_USB_PORT2NAME (char*)"FGU2:"

/* Default Baudrate */
#define DEFAULT_SPEED 115200

/* Default Timeout */
#define DEFAULT_TIMEOUT 1000

/* Common Global Variable */
UINT16 g_funcnum = 0;
UINT8 g_testnum = 0;

#if defined(DRV_UART)
#if defined(ARCH_WINXP)
char* g_port_name = DEFAULT_WINXP_UART_PORT1NAME;
char* g_port2_name = DEFAULT_WINXP_UART_PORT2NAME;
#elif defined(ARCH_LINUX)
char* g_port_name = DEFAULT_LINUX_UART_PORT1NAME;
char* g_port2_name = DEFAULT_LINUX_UART_PORT2NAME;
#elif defined(ARCH_WINCE)
char* g_port_name = DEFAULT_WINCE_UART_PORT1NAME;
char* g_port2_name = DEFAULT_LINUX_UART_PORT2NAME;
#else
char* g_port_name = "";
char* g_port2_name = "";
#endif

#else
#if defined(ARCH_LINUX)
char* g_port_name = DEFAULT_LINUX_USB_PORT1NAME;
char* g_port2_name = DEFAULT_LINUX_USB_PORT2NAME;
#elif defined(ARCH_WINCE)
char* g_port_name = DEFAULT_WINCE_USB_PORT1NAME;
char* g_port2_name = DEFAULT_WINCE_USB_PORT2NAME;
#else
char* g_port_name = "";
char* g_port2_name = "";
#endif
#endif

UINT32 g_speed = DEFAULT_SPEED;
UINT32 g_timeout = DEFAULT_TIMEOUT;
ICS_HW_DEVICE g_rcs956_dev;
felica_cc_devf_t g_felica_dev;
icsdrv_raw_func_t* g_raw_func;

/* Test Functions */
/* FeliCa card command library */
extern UINT32 te_felica_cc_polling(void);
extern UINT32 te_felica_cc_polling_multiple(void);
extern UINT32 te_felica_cc_request_service(void);
extern UINT32 te_felica_cc_request_response(void);
extern UINT32 te_felica_cc_write_without_encryption(void);
extern UINT32 te_felica_cc_read_without_encryption(void);
extern UINT32 te_felica_cc_request_system_code(void);
extern UINT32 te_felica_cc_stub_rcs956_initialize(void);

extern UINT32 te_felica_cc_communication_test(void);

/* RC-S956 driver */
extern UINT32 te_rcs956_initialize(void);
extern UINT32 te_rcs956_open(void);
extern UINT32 te_rcs956_close(void);
extern UINT32 te_rcs956_initialize_device(void);
extern UINT32 te_rcs956_get_firmware_version(void);
extern UINT32 te_rcs956_reset(void);
extern UINT32 te_rcs956_power_down(void);
extern UINT32 te_rcs956_execute_command(void);
extern UINT32 te_rcs956_send_ack(void);
extern UINT32 te_rcs956_ping(void);
extern UINT32 te_rcs956_wake_up(void);
extern UINT32 te_rcs956_wait_for_wake_up(void);
extern UINT32 te_rcs956_cancel_command(void);
extern UINT32 te_rcs956_felica_command(void);
extern UINT32 te_rcs956_rf_off(void);
extern UINT32 te_rcs956_rf_on(void);
extern UINT32 te_rcs956_set_dev_speed(void);
extern UINT32 te_rcs956_set_speed(void);
extern UINT32 te_rcs956_set_rf_speed(void);
extern UINT32 te_rcs956_clear_rx_queue(void);
extern UINT32 te_rcs956_get_ack_time(void);
#if defined(DRV_UART)
extern UINT32 te_rcs956_uart_open(void);
extern UINT32 te_rcs956_uart_initialize_device(void);
#else
extern UINT32 te_rcs956_usb_open(void);
#endif

static UINT32 te_start(void);
static UINT32 te_finish(void);

/* regist test function */
static testcase_t te_rcs956_tc[] = {
    TESTCASE(te_start),

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/* FeliCa card command library */
    TESTCASE(te_felica_cc_polling),
    TESTCASE(te_felica_cc_polling_multiple),
    TESTCASE(te_felica_cc_request_service),
    TESTCASE(te_felica_cc_request_response),
    TESTCASE(te_felica_cc_read_without_encryption),
    TESTCASE(te_felica_cc_write_without_encryption),
    TESTCASE(te_felica_cc_request_system_code),
    TESTCASE(te_felica_cc_stub_rcs956_initialize),
    TESTCASE(te_felica_cc_communication_test),

/* RC-S956 driver */
    TESTCASE(te_rcs956_initialize),
    TESTCASE(te_rcs956_open),
    TESTCASE(te_rcs956_close),
    TESTCASE(te_rcs956_initialize_device),
    TESTCASE(te_rcs956_get_firmware_version),
    TESTCASE(te_rcs956_reset),
    TESTCASE(te_rcs956_power_down),
    TESTCASE(te_rcs956_execute_command),
    TESTCASE(te_rcs956_send_ack),
    TESTCASE(te_rcs956_ping),
    TESTCASE(te_rcs956_wake_up),
    TESTCASE(te_rcs956_wait_for_wake_up),
    TESTCASE(te_rcs956_cancel_command),
    TESTCASE(te_rcs956_felica_command),
    TESTCASE(te_rcs956_rf_off),
    TESTCASE(te_rcs956_rf_on),
    TESTCASE(te_rcs956_set_dev_speed),
    TESTCASE(te_rcs956_set_speed),
    TESTCASE(te_rcs956_set_rf_speed),
    TESTCASE(te_rcs956_clear_rx_queue),
    TESTCASE(te_rcs956_get_ack_time),
#if defined(DRV_UART)
    TESTCASE(te_rcs956_uart_open),
    TESTCASE(te_rcs956_uart_initialize_device),
#else
    TESTCASE(te_rcs956_usb_open),
#endif
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    TESTCASE(te_finish)
};

/**
 * This function displays the Help menu.
 *
 * \param argv0               [IN] Input character string from command line.
 */
static void print_usage(
    const char* argv0)
{
    fprintf(stderr, "Usage: %s [options] testcases...\n", argv0);
    fprintf(stderr, "  -p1 port1\n");
    fprintf(stderr, "  -p2 port2\n");
    fprintf(stderr, "  -t timeout\n");

    return;
}

/**
 * When the test begins, this function is executed. 
 *
 * \retval NOTESTCASE
 */
static UINT32 te_start(void)
{
    g_speed = DEFAULT_SPEED;
    g_timeout = DEFAULT_TIMEOUT;

#if defined(DRV_UART)
    g_raw_func = (icsdrv_raw_func_t*)&icsdrv_uart_raw_func;
    rcs956_initialize(&g_rcs956_dev, &icsdrv_uart_raw_func);

#else
    g_raw_func = (icsdrv_raw_func_t*)&rcs956_usb_raw_func;
    rcs956_initialize(&g_rcs956_dev, &rcs956_usb_raw_func);

#endif

    TESTPR("\ntest start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n" );
    return NOTESTCASE;
}

/**
 * When the test ended, this function is executed. 
 *
 * \retval NOTESTCASE
 */
static UINT te_finish(void)
{
    TESTPR("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< test end\n");
    return NOTESTCASE;
}

/**
 * This function executes all the test functions registered in the table.
 *
 * \param testcases            [IN] Table of test functions.
 * \param ncases               [IN] Number of table registration.
 *
 * \retval Number of Test Failures.
 */
static int do_testcases(
    const testcase_t testcases[], int ncases)
{
    int res;
    int ntests;
    int nfails;
    int i;

    ntests = 0;
    nfails = 0;
    for (i = 0; i < ncases; i++) {
        res = testcases[i].func();
        if (res == NOTESTCASE) {
            continue;
        }
        
        if (res == 0) {
            TESTPR("TEST:%-48s: PASS\n", testcases[i].name);
        } else {
            TESTPR("TEST:%-48s: FAIL\n", testcases[i].name);
            nfails++;
            do_testterminate();
        }
        ntests++;
    }
    
    if (ntests > 1 && nfails > 0) {
        TESTPR("%d/%d failed\n", nfails, ntests);
    }

    return nfails;
}

/**
 * This function executes all the test functions 
 * for which it is specified in the table. 
 *
 * \param testcases            [IN] Table of test functions.
 * \param ncases               [IN] Number of table registration.
 * \param names                [IN] Test functions Names.
 * \param nnames               [IN] Test functions Name's Length.
 *
 * \retval Number of Test Failures.
 */
static int do_specified_testcases(
    const testcase_t testcases[], int ncases,
               char *const names[], int nnames)
{
    int res;
    int ntests;
    int nfails;
    int i, namei;

    ntests = 0;
    nfails = 0;
    te_start();
    for (namei = 0; namei < nnames; namei++) {
        for (i = 0; i < ncases; i++) {
            if (strcmp(names[namei], testcases[i].name) != 0) {
                continue;
            }
            res = testcases[i].func();
            if (res == NOTESTCASE) {
                continue;
            }
            if (res == 0) {
                TESTPR("TEST:%-48s: PASS\n",
                        testcases[i].name);
            } else {
                TESTPR("TEST:%-48s: FAIL\n",
                       testcases[i].name);
                nfails++;
                do_testterminate();
            }
            ntests++;
            break;
        }
    }
    
    if (ntests > 1 && nfails > 0) {
        TESTPR("%d/%d failed\n", nfails, ntests);
    }
    te_finish();

    return nfails;
}

/**
 * This function displays the test function in the table.
 *
 * \param testcases            [IN] Table of test functions.
 * \param ncases               [IN] Number of table registration.
 */
static void print_testcases(
    const testcase_t testcases[], int ncases)
{
    int i;

    for (i = 0; i < ncases; i++) {
        TESTPR("%s\n", testcases[i].name);
    }

    return;
}

/**
 * It initiates the processing from this function. 
 *
 * \param argc          [IN] Input character string length from command line.
 * \param argv          [IN] Input character string from command line.
 *
 * \retval Number of Test Failures.
 */
int main(
    int argc,
    char* argv[])
{
    int res;
    int i;

    /* parse options */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p1") == 0 && i + 1 < argc) {
            i++;
            g_port_name = argv[i];
        } else if (strcmp(argv[i], "-p2") == 0 && i + 1 < argc) {
            i++;
            g_port2_name = argv[i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            i++;
            g_timeout = strtoul(argv[i], NULL, 0);
        } else if (argv[i][0] == '-') {
            print_usage(argv[0]);
            fprintf(stderr, "testcases:\n");
            print_testcases(te_rcs956_tc, TESTTOOL_NCASES(te_rcs956_tc));
            return -1;
        } else {
            break;
        }
    }

    if (i < argc) {
        res = do_specified_testcases(te_rcs956_tc,
                                     TESTTOOL_NCASES(te_rcs956_tc),
                                     argv + i, argc - i);
    } else {
        res = do_testcases(te_rcs956_tc,
                           TESTTOOL_NCASES(te_rcs956_tc));
    }

    return res;
}

