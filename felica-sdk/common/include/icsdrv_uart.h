/**
 * \brief    a header file for the UART Driver
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#include "ics_types.h"
#include "icsdrv.h"

#ifndef ICSDRV_UART_H_
#define ICSDRV_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

UINT32 icsdrv_uart_open(
    ICS_HANDLE* handle,
    const char* port_name);
UINT32 icsdrv_uart_close(
    ICS_HANDLE handle);

UINT32 icsdrv_uart_write(
    ICS_HANDLE handle,
    const UINT8* data,
    UINT32 data_len,
    UINT32 time0,
    UINT32 timeout);
UINT32 icsdrv_uart_read(
    ICS_HANDLE handle,
    UINT32 min_read_len,
    UINT32 max_read_len,
    UINT8* data,
    UINT32* read_len,
    UINT32 time0,
    UINT32 timeout);

UINT32 icsdrv_uart_set_speed(
    ICS_HANDLE handle,
    UINT32 speed);
UINT32 icsdrv_uart_clear_rx_queue(
    ICS_HANDLE handle);
UINT32 icsdrv_uart_drain_tx_queue(
    ICS_HANDLE handle);

static const icsdrv_raw_func_t icsdrv_uart_raw_func = {
    "icsdrv_uart",
    icsdrv_uart_open,
    icsdrv_uart_close,
    icsdrv_uart_write,
    icsdrv_uart_read,
    icsdrv_uart_set_speed,
    icsdrv_uart_clear_rx_queue,
    icsdrv_uart_drain_tx_queue,
    0,
    NULL,
};

#ifdef __cplusplus
}
#endif

#endif /* !ICSDRV_UART_H_ */
