/**
 * \brief    a header file for the RC-S956 USB module
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#include "ics_types.h"
#include "ics_hwdev.h"
#include "icsdrv.h"
#include "rcs956.h"

#ifndef RCS956_USB_H_
#define RCS956_USB_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Prototype declaration
 */

/* driver functions */
UINT32 rcs956_usb_open(
    ICS_HW_DEVICE* rcs956,
    const char* port_name);

#define rcs956_usb_close                rcs956_close
#define rcs956_usb_initialize_device    rcs956_initialize_device
#define rcs956_usb_get_firmware_version rcs956_get_firmware_version
#define rcs956_usb_ping                 rcs956_ping
#define rcs956_usb_reset                rcs956_reset
#define rcs956_usb_execute_command      rcs956_execute_command
#define rcs956_usb_send_ack             rcs956_send_ack
#define rcs956_usb_cancel_command       rcs956_cancel_command
#define rcs956_usb_felica_command       rcs956_felica_command
#define rcs956_usb_rf_off               rcs956_rf_off
#define rcs956_usb_rf_on                rcs956_rf_on
#define rcs956_usb_set_rf_speed         rcs956_set_rf_speed
#define rcs956_usb_claer_rx_queue       rcs956_clear_rx_queue
#define rcs956_usb_get_ack_time         rcs956_get_ack_time

static const icsdrv_basic_func_t rcs956_usb_basic_func = {
    "rcs956_usb",
    rcs956_usb_open,
    rcs956_close,
    rcs956_initialize_device,
    rcs956_ping,
    rcs956_reset,
    rcs956_execute_command,
    rcs956_cancel_command,
    rcs956_felica_command,
    RCS956_MAX_FELICA_COMMAND_LEN,
    RCS956_MAX_FELICA_RESPONSE_LEN,
    rcs956_rf_off,
    rcs956_rf_on,
    NULL, /* set dev speed */
    NULL, /* set speed */
    0,
    NULL,
};

/* raw functions */
UINT32 rcs956_usb_raw_open(
    ICS_HANDLE* rcs956,
    const char* port_name);
UINT32 rcs956_usb_raw_close(
    ICS_HANDLE handle);
UINT32 rcs956_usb_raw_write(
    ICS_HANDLE handle,
    const UINT8* data,
    UINT32 data_len,
    UINT32 time0,
    UINT32 timeout);
UINT32 rcs956_usb_raw_read(
    ICS_HANDLE handle,
    UINT32 min_read_len,
    UINT32 max_read_len,
    UINT8* data,
    UINT32* read_len,
    UINT32 time0,
    UINT32 timeout);
UINT32 rcs956_usb_raw_clear_rx_queue(
    ICS_HANDLE handle);
UINT32 rcs956_usb_raw_drain_tx_queue(
    ICS_HANDLE handle);
UINT32 rcs956_usb_raw_reset(
    ICS_HANDLE handle);

static const icsdrv_raw_func_t rcs956_usb_raw_func = {
    "rcs956_usb",
    rcs956_usb_raw_open,
    rcs956_usb_raw_close,
    rcs956_usb_raw_write,
    rcs956_usb_raw_read,
    NULL,
    rcs956_usb_raw_clear_rx_queue,
    rcs956_usb_raw_drain_tx_queue,
    0,
    NULL,
};

#ifdef __cplusplus
}
#endif

#endif /* !RCS956_USB_H_ */
