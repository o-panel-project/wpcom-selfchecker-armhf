/**
 * \brief    a header file for the RC-S956 UART module
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#include "ics_types.h"
#include "ics_hwdev.h"
#include "icsdrv.h"
#include "rcs956.h"

#ifndef RCS956_UART_H_
#define RCS956_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Prototype declaration
 */

UINT32 rcs956_uart_open(
    ICS_HW_DEVICE* rcs956,
    const char* port_name);
UINT32 rcs956_uart_initialize_device(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

#define rcs956_uart_close                rcs956_close
#define rcs956_uart_get_firmware_version rcs956_get_firmware_version
#define rcs956_uart_ping                 rcs956_ping
#define rcs956_uart_reset                rcs956_reset
#define rcs956_uart_execute_command      rcs956_execute_command
#define rcs956_uart_send_ack             rcs956_send_ack
#define rcs956_uart_cancel_command       rcs956_cancel_command
#define rcs956_uart_felica_command       rcs956_felica_command
#define rcs956_uart_rf_off               rcs956_rf_off
#define rcs956_uart_rf_on                rcs956_rf_on
#define rcs956_uart_set_dev_speed        rcs956_set_dev_speed
#define rcs956_uart_set_speed            rcs956_set_speed
#define rcs956_uart_power_down           rcs956_power_down
#define rcs956_uart_wake_up              rcs956_wake_up
#define rcs956_uart_wait_for_wake_up     rcs956_wait_for_wake_up
#define rcs956_uart_set_rf_speed         rcs956_set_rf_speed
#define rcs956_uart_claer_rx_queue       rcs956_clear_rx_queue
#define rcs956_uart_get_ack_time         rcs956_get_ack_time

static const icsdrv_basic_func_t rcs956_uart_basic_func = {
    "rcs956_uart",
    rcs956_uart_open,
    rcs956_close,
    rcs956_uart_initialize_device,
    rcs956_ping,
    rcs956_reset,
    rcs956_execute_command,
    rcs956_cancel_command,
    rcs956_felica_command,
    RCS956_MAX_FELICA_COMMAND_LEN,
    RCS956_MAX_FELICA_RESPONSE_LEN,
    rcs956_rf_off,
    rcs956_rf_on,
    rcs956_set_dev_speed,
    rcs956_set_speed,
    0,
    NULL,
};

#ifdef __cplusplus
}
#endif

#endif /* !RCS956_UART_H_ */
