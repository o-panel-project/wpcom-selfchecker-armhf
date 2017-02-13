/**
 * \brief    a header file for the RC-S956 module
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#include "ics_types.h"
#include "ics_hwdev.h"
#include "icsdrv.h"

#ifndef RCS956_H_
#define RCS956_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Constant
 */

#define RCS956_MAX_FELICA_COMMAND_LEN                     254
#define RCS956_MAX_FELICA_RESPONSE_LEN                    254

#define RCS956_WAKEUP_UART                                0x10
#define RCS956_WAKEUP_RF_DETECT                           0x08

#define RCS956_RF_SPEED_212K                              0x01
#define RCS956_RF_SPEED_424K                              0x02

/*
 * Macro
 */

#define RCS956_SPEED(rcs956) ((UINT32)(((rcs956)->status) & 0xff) * 9600)
#define RCS956_SET_SPEED(rcs956, speed) \
    do { \
        (rcs956)->status = (((rcs956)->status & 0xffffff00) | \
                            (((speed) / 9600) & 0xff)); \
    } while (0)
#define RCS956_IS_VALID_SPEED(speed) \
    (((speed) % 9600) == 0)

#define RCS956_LAST_MODE(rcs956) (((rcs956)->status >> 8) & 0x1f)
#define RCS956_SET_LAST_MODE(rcs956, mode) \
    do { \
        (rcs956)->status = (((rcs956)->status & 0xffffe0ff) | \
                            (((UINT32)(mode) & 0x1f) << 8)); \
    } while (0)
#define RCS956_MODE_FELICA_INITIATOR 0x01
#define RCS956_MODE_FELICA_TARGET    0x11

#define RCS956_NEED_CHANGE_FELICA_RF_PARAM(rcs956) \
    (((rcs956)->status & 0x2000) != 0)
#define RCS956_SET_NEED_CHANGE_FELICA_RF_PARAM_FLAG(rcs956, flag) \
    do { \
        (rcs956)->status = (((rcs956)->status & 0xffffdfff) | \
                            ((UINT32)(flag != 0) << 13)); \
    } while (0)

/*
 * Prototype declaration
 */

UINT32 rcs956_initialize(
    ICS_HW_DEVICE* rcs956,
    const icsdrv_raw_func_t* raw_func);

/* open: (uart, usb) */
UINT32 rcs956_open(
    ICS_HW_DEVICE* rcs956,
    const char* port_name);

/* close: (uart, usb) */
UINT32 rcs956_close(
    ICS_HW_DEVICE* rcs956);

/* initialize the device: (uart, usb) */
UINT32 rcs956_initialize_device(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* get firmware version: (uart, usb) */
UINT32 rcs956_get_firmware_version(
    ICS_HW_DEVICE* rcs956,
    UINT8* ic_type,
    UINT16* version,
    UINT32 timeout);

/* check the device is alive: (uart, usb) */
UINT32 rcs956_ping(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* reset: (uart, usb) */
UINT32 rcs956_reset(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* send a command to the device and receives response: (uart, usb) */
UINT32 rcs956_execute_command(
    ICS_HW_DEVICE* rcs956,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 timeout);

/* send an ACK to the device: (uart, usb) */
UINT32 rcs956_send_ack(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* cancel the previous command: (uart, usb) */
UINT32 rcs956_cancel_command(
    ICS_HW_DEVICE* rcs956);

/* send the FeliCa command and receive a FeliCa response: (uart, usb) */
UINT32 rcs956_felica_command(
    ICS_HW_DEVICE* rcs956,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 command_timeout,
    UINT32 timeout);

/* turn RF off: (uart, usb) */
UINT32 rcs956_rf_off(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* turn RF on: (uart, usb) */
UINT32 rcs956_rf_on(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* set the communication speed of the device: (uart) */
UINT32 rcs956_set_dev_speed(
    ICS_HW_DEVICE* rcs956,
    UINT32 speed,
    UINT32 timeout);

/* set the communication speed of the port: (uart) */
UINT32 rcs956_set_speed(
    ICS_HW_DEVICE* rcs956,
    UINT32 speed);

/* execute the power down command: (uart) */
UINT32 rcs956_power_down(
    ICS_HW_DEVICE* rcs956,
    UINT8 wake_up,
    UINT32 timeout);

/* wake the device up: (uart) */
UINT32 rcs956_wake_up(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* wait for the device to be waked up: (uart) */
UINT32 rcs956_wait_for_wake_up(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* set the rf communication speed of the device: (uart, usb) */
UINT32 rcs956_set_rf_speed(
    ICS_HW_DEVICE* rcs956,
    UINT8 rf_speed,
    UINT32 timeout);

/* clear the receiving queue: (uart, usb) */
UINT32 rcs956_clear_rx_queue(
    ICS_HW_DEVICE* rcs956);

/* return the time when this driver received the last ACK: (uart, usb) */
UINT32 rcs956_get_ack_time(
    ICS_HW_DEVICE* rcs956,
    UINT32* ack_time);

#ifdef __cplusplus
}
#endif

#endif /* !RCS956_H_ */
