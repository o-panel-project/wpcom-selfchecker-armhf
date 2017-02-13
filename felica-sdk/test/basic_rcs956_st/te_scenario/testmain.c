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
#define DEFAULT_WINXP_UART_PORTNAME (char*)"COM1"
#define DEFAULT_LINUX_UART_PORTNAME (char*)"/dev/ttyS0"
#define DEFAULT_WINCE_UART_PORTNAME (char*)"COM1:"
#define DEFAULT_LINUX_USB_PORTNAME (char*)"/dev/bus/usb/001/001"
#define DEFAULT_WINCE_USB_PORTNAME (char*)"FGU1:"

/* Default Baudrate */
#define DEFAULT_SPEED 115200

/* Default Timeout */
#define DEFAULT_TIMEOUT 1000

/* Common Global Variable */
UINT16 g_funcnum = 0;
UINT8 g_testnum = 0;
UINT16 g_testcnt = 0;

#if defined(ARCH_WINXP)
char* g_port_name = DEFAULT_WINXP_UART_PORTNAME;
#elif defined(ARCH_LINUX)
#if defined(DRV_UART)
char* g_port_name = DEFAULT_LINUX_UART_PORTNAME;
#else
char* g_port_name = DEFAULT_LINUX_USB_PORTNAME;
#endif
#else
#if defined(DRV_UART)
char* g_port_name = DEFAULT_WINCE_UART_PORTNAME;
#else
char* g_port_name = DEFAULT_WINCE_USB_PORTNAME;
#endif
#endif

UINT32 g_speed = DEFAULT_SPEED;
UINT32 g_timeout = DEFAULT_TIMEOUT;
icsdrv_raw_func_t* g_raw_func;
ICS_HW_DEVICE g_rcs956_dev;
felica_cc_devf_t g_felica_dev;

/* Test Functions */
extern UINT32 te_multithread(void);

/* Scenario fcc */
extern UINT32 te_polling(void);
extern UINT32 te_read_write(void);
extern UINT32 te_pmm(void);
extern UINT32 te_retry_fcc(void);

/* Scenario rcs956 */
extern UINT32 te_powerdown_and_wakeup(void);
extern UINT32 te_chg_transmission_speed(void);
extern UINT32 te_chg_rwmode(void);
extern UINT32 te_retry_rcs956(void);

static UINT32 te_start(void);
static UINT32 te_finish(void);

/* regist test function */
static testcase_t te_rcs956_tc[] = {
    TESTCASE(te_start),
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/* Scenario fcc */
    TESTCASE(te_polling),
    TESTCASE(te_read_write),
    TESTCASE(te_pmm),
    TESTCASE(te_retry_fcc),

/* Scenario rcs956 */
    TESTCASE(te_powerdown_and_wakeup),
    TESTCASE(te_chg_transmission_speed),
    TESTCASE(te_chg_rwmode),
    TESTCASE(te_retry_rcs956),

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
    fprintf(stderr, "  -p port\n");
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
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            i++;
            g_port_name = argv[i];

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

