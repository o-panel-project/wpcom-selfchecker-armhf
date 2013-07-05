
#include "testmain.h"

UINT te_polling(void);
static UINT32 polling_rcs956(
    UINT32 timeout,
    UINT32 loop_times,
    UINT32 max_polling_times,
    UINT8 testnum);

/**
 * This function is test program
 * for te_polling.
 *
 * \retval 0x0000        Test passed.
 * \retval 0xffff        Test failed.
 */

UINT te_polling(void)
{
    UINT32 success_rate;
    UINT rc;

    /* function number */
    g_funcnum = STNO_POLLING;

    rc = do_testinitialize();
    T_CHECK_EQ(ICS_ERROR_SUCCESS, rc);

    /*********************************/
    /* TSS00-0000N */
    g_testnum = 0;

    TESTPR("TSS00-0000N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_RELEASE_CHIP();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(0, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0001N */
    g_testnum = 1;

    TESTPR("TSS00-0001N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_RCS860();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0002N */
    g_testnum = 2;

    TESTPR("TSS00-0002N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_AND_RELEASE_CHIP_RCS860();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_NE(0, success_rate);
    T_CHECK_NE(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0003N */
    g_testnum = 3;

    TESTPR("TSS00-0003N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_RCX407();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0004N */
    g_testnum = 4;

    TESTPR("TSS00-0004N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_AND_RELEASE_CHIP_RCX407();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_NE(0, success_rate);
    T_CHECK_NE(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0005N */
    g_testnum = 5;

    TESTPR("TSS00-0005N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_SDB200();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0006N */
    g_testnum = 6;

    TESTPR("TSS00-0006N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_AND_RELEASE_CHIP_SDB200();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_NE(0, success_rate);
    T_CHECK_NE(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0007N */
    g_testnum = 7;

    TESTPR("TSS00-0007N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_RCS860();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0008N */
    g_testnum = 8;

    TESTPR("TSS00-0008N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_RCX407();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0009N */
    g_testnum = 9;

    TESTPR("TSS00-0009N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_SDB200();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(100, success_rate);

    /* cleanup */

    /*********************************/
    /* TSS00-0010N */
    g_testnum = 10;
#if defined(DRV_UART)
    TESTPR("TSS00-0010N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_SET_RCS860();

    /* test execution */
    success_rate = polling_rcs956(POLLING_TIMEOUT_AT_DEVICE,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(100, success_rate);

    /* cleanup */
#endif

    /*********************************/
    /* TSS00-0011N */
    g_testnum = 11;

    TESTPR("TSS00-0011N ...\n");

    /* set parameters */

    /* build condition */
    PAUSE_RELEASE_CHIP();

    /* test execution */
    success_rate = polling_rcs956(1,
                                  DEFAULT_LOOP_TIMES,
                                  DEFAULT_MAX_POLLING_TIMES,
                                  g_testnum);

    /* results judgement */
    T_CHECK_EQ(0, success_rate);

    /* cleanup */

    /*********************************/
    /* terminate */
    do_testterminate();

    return (0);
}

static UINT32 polling_rcs956(
    UINT32 timeout,
    UINT32 loop_times,
    UINT32 max_polling_times,
    UINT8 testnum)
{
    UINT rc;
    UINT32 n_pollings_in_cycle;
    UINT32 n_pollings_in_loop;
    UINT32 n_pollings_total;
    UINT32 n_successes_in_cycle;
    UINT32 n_successes_in_loop;
    UINT32 n_cycles;
    UINT32 n_loops;
    const UINT8 parameter[] = {0xff, 0xff, 0x00, 0x00};
    UINT32 success_rate = 0;
    felica_card_t card;
    felica_card_option_t card_option;

    n_pollings_total = 0;

    for (n_loops = 0;
        (loop_times == 0) || (n_loops < loop_times);
         n_loops++) {
        if ((max_polling_times > 0) &&
            (n_pollings_total >= max_polling_times)) {
            break;
        }

        n_pollings_in_loop = 0;
        n_successes_in_loop = 0;

        for (n_cycles = 0; n_cycles < N_CYCLES_PER_LOOP; n_cycles++) {
            if ((max_polling_times > 0) &&
                (n_pollings_total >= max_polling_times)) {
                break;
            }

            n_successes_in_cycle = 0;

            for (n_pollings_in_cycle = 0;
                 n_pollings_in_cycle < N_POLLINGS_PER_CYCLE;
                 n_pollings_in_cycle++) {
                if ((max_polling_times > 0) &&
                    (n_pollings_total >= max_polling_times)) {
                    break;
                }

                /* polling */
                if (testnum == 7) {
                    rc = rcs956_rf_off(&g_rcs956_dev, g_timeout);
                    if(ICS_ERROR_SUCCESS != rc) {
                        return (UINT32)0xffffffff;
                    }
                } else if (testnum == 8) {
                    rc = rcs956_initialize_device(&g_rcs956_dev, g_timeout);
                    if(ICS_ERROR_SUCCESS != rc) {
                        return (UINT32)0xffffffff;
                    }
                } else if (testnum == 9) {
                    rc = rcs956_reset(&g_rcs956_dev, g_timeout);
                    if(ICS_ERROR_SUCCESS != rc) {
                        return (UINT32)0xffffffff;
                    }
                } else if (testnum == 10) {
                    rc = rcs956_power_down(&g_rcs956_dev, 0x10, g_timeout);
                    if(ICS_ERROR_SUCCESS != rc) {
                        return (UINT32)0xffffffff;
                    }

                    rc = utl_msleep(WAIT_POWER_DOWN);
                    if(ICS_ERROR_SUCCESS != rc) {
                        return (UINT32)0xffffffff;
                    }

                    rc = rcs956_wake_up(&g_rcs956_dev, g_timeout);
                    if(ICS_ERROR_SUCCESS != rc) {
                        return (UINT32)0xffffffff;
                    }
                } else if (testnum == 11) {
                    timeout++;
                }

                rc = felica_cc_polling(&g_felica_dev, parameter,
                                       &card, &card_option, timeout);

                if (rc == ICS_ERROR_SUCCESS) {
                    TESTPR("o");
                    TESTPR_FLUSH;
                    n_successes_in_cycle++;
                } else {
                    if (rc == ICS_ERROR_TIMEOUT) {
                        TESTPR(".");
                        TESTPR_FLUSH;
                    } else {
                        TESTPR("X");
                        TESTPR_FLUSH;
                    }
                }
                n_pollings_total++;
                
                if (testnum == 11) {
                    rc = rcs956_ping(&g_rcs956_dev, g_timeout);
                    if (rc != ICS_ERROR_SUCCESS) {
                        return (UINT32)0xffffffff;
                    }
                }
            }

            TESTPR("    [%3u/%3u] %3u%%\n", n_successes_in_cycle,
                                            n_pollings_in_cycle,
                                            (n_successes_in_cycle * 100)
                                            / n_pollings_in_cycle);
            TESTPR_FLUSH;

            n_pollings_in_loop += n_pollings_in_cycle;
            n_successes_in_loop += n_successes_in_cycle;
        }

        success_rate = ((n_successes_in_loop * 10000) / n_pollings_in_loop);
        TESTPR("(TOTAL) [%4u/%4u] ... %3u.%02u%%\n\n",
                n_successes_in_loop,
                n_pollings_in_loop,
                success_rate / 100,
                success_rate % 100);
        TESTPR_FLUSH;
    }

    return (UINT32)(success_rate / 100);
}
