
/* SDRAM Disp timer command */
#define TIMER_COMMAND_INIT          0                                   /* timer init                           */
#define TIMER_COMMAND_COUNT_UP      1                                   /* timer count up request               */
#define TIMER_COMMAND_MESSAGE       -1                                  /* thread/timer start message           */
#define TIMER_COMMAND_OK            -2                                  /* thread/timer OK finish               */
#define TIMER_COMMAND_NG            -3                                  /* thread/timer NG finish               */
#define TIMER_COMMAND_NEXT          -99                                 /* timer next operation                 */

/* umount */
#define UMOUNT_INIT                 -1                                  /* SD-RAM umount init                   */
#define UMOUNT_CLR                  0                                   /* SD-RAM umount clear                  */
#define UMOUNT_PLAY                 99                                  /* SD-RAM umount playing                */

#define GET_TIME_INT                0                                   /* get time timer init                  */
#define GET_TIME_START              GET_TIME_INT + 1                    /* get time timer start                 */
#define GET_TIME_MAX_INT            0

#define FILE_NOT_FOUND                  -1
#define FILE_FOUND                      1

#define AGING_INIT                  -1                                  /* AGING init                           */
#define AGING_START                 0                                   /* AGING start                          */

/* Movie test mode(for Aging test)  */
#define MOVIE_MODE_NORMAL           0                                   /* Normal movie mode                    */
#define MOVIE_MODE_AGING            1                                   /* Aging movie mode                     */

/* Aging Timer Stop                 */
#define AGING_TIMER_INIT            0                                   /* Aging timer init                     */
#define AGING_TIMER_STOP            1                                   /* Aging timer stop                     */

/* Thread */
#define SDRAM_THD_INIT              0                                   /* Thread init                          */
#define SDRAM_THD_START             1                                   /* Thread start                         */
#define SDRAM_THD_FINISH_REQUEST    2                                   /* Thread finish request                */
#define SDRAM_THD_FINISH            99                                  /* Thread finish                        */

/* SDRAM Disp timer command */
#define TIMER_COMMAND_INIT          0                                   /* timer init                           */
#define TIMER_COMMAND_COUNT_UP      1                                   /* timer count up request               */
#define TIMER_COMMAND_MESSAGE       -1                                  /* thread/timer start message           */
#define TIMER_COMMAND_OK            -2                                  /* thread/timer OK finish               */
#define TIMER_COMMAND_NG            -3                                  /* thread/timer NG finish               */
#define TIMER_COMMAND_NEXT          -99                                 /* timer next operation                 */

#define THD_100MSEC                 100000                              /* 100msec timer                        */

/* Infinite status */
#define INFINITE_INIT               -1                                  /* Infinite status init                 */
#define INFINITE_NORMAL             0                                   /* Infinite status normal               */
#define INFINITE_INFINITE           1                                   /* Infinite status infinite             */
#define INFINITE_AGING              2                                   /* Infinite status aging                */

/* RTC timer stop/start */
#define SDRAM_RTC_START             0                                   /* RTC timer start                      */
#define SDRAM_RTC_STOP              1                                   /* RTC timer stop                       */

/* Sdram_testthd status */
#define SCRIPT_INIT_ST              0                                   /* test script init                     */
#define SCRIPT_START_ST             1                                   /* test script start                    */
#define SCRIPT_FINISH_ST            2                                   /* finish file check                    */
#define SCRIPT_RESULT_ST            3                                   /* result file check                    */
#define SCRIPT_RESULT_DISP_ST       10                                  /* sdram result disp                    */
#define SCRIPT_SHELL_ERROR          90                                  /* shell error                          */
#define SCRIPT_FINISH_ERROR         91                                  /* finish file read error               */
#define SCRIPT_RESULT_ERROR         92                                  /* result file read error               */
#define SCRIPT_STATUS_ERROR         98                                  /* status error                         */
#define SCRIPT_RESULT_DISP          100                                 /* result disp                          */
#define SCRIPT_PUSH_WAIT            999                                 /* Exit BTN push wait                   */

