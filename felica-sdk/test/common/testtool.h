/*
 * testtool.h - a test framework
 *
 */

#ifndef TESTTOOL_H_
#define TESTTOOL_H_

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TESTPR
#include <stdio.h>
#define TESTPR  printf
#endif

#define T_CHECK(cond) { \
    if (!(cond)) { \
        TESTPR("#error:%d: %s\n", __LINE__, #cond); \
        return -1; \
    } \
}

typedef struct testcase_t {
    const char* name;
    int (*func)(void);
} testcase_t;

#define TESTCASE(x) {#x, x}
#define TESTTOOL_NCASES(a)  (sizeof(a) / sizeof(a[0]))

static int do_testcases(
    const testcase_t testcases[],
    int ncases)
{
    int res;
    int ntests;
    int nfails;
    int i;

    ntests = 0;
    nfails = 0;
    for (i = 0; i < ncases; i++) {
        res = testcases[i].func();
        if (res == 0) {
            TESTPR("TEST:%-48s: PASS\n", testcases[i].name);
        } else {
            TESTPR("TEST:%-48s: FAIL\n", testcases[i].name);
            nfails++;
        }
        ntests++;
    }
    if (ntests > 1 && nfails > 0) {
        TESTPR("%d/%d failed\n", nfails, ntests);
    }

    return nfails;
}

static int do_specified_testcases(
    const testcase_t testcases[],
    int ncases,
    char* const names[],
    int nnames)
{
    int res;
    int ntests;
    int nfails;
    int i;
    int namei;

    ntests = 0;
    nfails = 0;
    for (namei = 0; namei < nnames; namei++) {
        for (i = 0; i < ncases; i++) {
            if (strcmp(names[namei], testcases[i].name) == 0) {
                res = testcases[i].func();
                if (res == 0) {
                    TESTPR("TEST:%-48s: PASS\n",
                           testcases[i].name);
                } else {
                    TESTPR("TEST:%-48s: FAIL\n",
                           testcases[i].name);
                    nfails++;
                }
                ntests++;
                break;
            }
        }
    }
    if (ntests > 1 && nfails > 0) {
        TESTPR("%d/%d failed\n", nfails, ntests);
    }

    return nfails;
}

static void print_testcases(
    const testcase_t testcases[],
    int ncases)
{
    int i;

    for (i = 0; i < ncases; i++) {
        TESTPR("%s\n", testcases[i].name);
    }

    return;
}

#ifdef __cplusplus
}
#endif

#endif /* !TESTTOOL_H_ */
