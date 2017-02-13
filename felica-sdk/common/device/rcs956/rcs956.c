/**
 * \brief    RC-S956 Driver
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#undef ICSLOG_MODULE
#define ICSLOG_MODULE "DGC"

#include "ics_types.h"
#include "ics_error.h"
#include "icslib_chk.h"
#include "icslog.h"
#include "utl.h"

#include "rcs956.h"

/* --------------------------------
 * Constant
 * -------------------------------- */

#define RCS956_DEFAULT_SPEED                    115200
#define RCS956_AUTO_LED_PERIOD                  50      /* 0.01s unit */

#define RCS956_RESET_WAIT_MS                    10      /* ms */
#define RCS956_SET_DEV_SPEED_WAIT_MS            50      /* ms */

#define RCS956_MAX_CANCEL_COMMAND_TIMEOUT       1000    /* ms */

#define RCS956_COMMAND_BUF_LEN                  (8 + 265 + 2)
#define RCS956_COMMAND_POS                      8

#define RCS956_MAX_COMMAND_LEN                  265
#define RCS956_MAX_RESPONSE_LEN                 265

#define RCS956_COMMAND_CODE                     0xd4
#define RCS956_RESPONSE_CODE                    0xd5

#define RCS956_CMD_GET_FIRMWARE_VERSION         0x02
#define RCS956_RES_GET_FIRMWARE_VERSION         0x03
#define RCS956_CMD_READ_REGISTER                0x06
#define RCS956_RES_READ_REGISTER                0x07
#define RCS956_CMD_WRITE_REGISTER               0x08
#define RCS956_RES_WRITE_REGISTER               0x09
#define RCS956_CMD_SET_SERIAL_BAUDRATE          0x10
#define RCS956_RES_SET_SERIAL_BAUDRATE          0x11
#define RCS956_CMD_POWER_DOWN                   0x16
#define RCS956_RES_POWER_DOWN                   0x17
#define RCS956_CMD_RESET                        0x18
#define RCS956_RES_RESET                        0x19
#define RCS956_CMD_RF_CONFIGURATION             0x32
#define RCS956_RES_RF_CONFIGURATION             0x33
#define RCS956_CMD_COMMUNICATE_THRU_EX          0xa0
#define RCS956_RES_COMMUNICATE_THRU_EX          0xa1

#define RCS956_CMD_IN_COMMUNICATE_THRU          0x42
#define RCS956_RES_IN_COMMUNICATE_THRU          0x43

#define RCS956_WAKE_UP_BYTE                     0x55

#define RCS956_RFCONFIG_RF_FIELD                0x01
#define RCS956_RFCONFIG_TIMEOUT                 0x02
#define RCS956_RFCONFIG_MAXRETRY                0x05
#define RCS956_RFCONFIG_FELICA_RF_PARAM         0x0b
#define RCS956_RFCONFIG_RF_SPEED                0x80
#define RCS956_RFCONFIG_WAIT_TIME               0x81

#define RCS956_RFCONFIG_RF_OFF                  0x00
#define RCS956_RFCONFIG_RF_ON                   0x01

#define RCS956_RESET_FUNCTION                   0x01

#define RCS956_STATUS_SUCCESS                   0x00
#define RCS956_STATUS_TIMEOUT                   0x01
#define RCS956_STATUS_CRC_ERR                   0x02
#define RCS956_STATUS_RF_OFF                    0x31

/* --------------------------------
 * Prototype Declaration
 * -------------------------------- */

static UINT32 rcs956_execute_command_internal(
    ICS_HW_DEVICE* rcs956,
    UINT8 command_buf[RCS956_COMMAND_BUF_LEN],
    UINT32 command_len,
    UINT32* response_pos,
    UINT32* response_len,
    UINT32 timeout);

static UINT32 rcs956_receive_ack(
    ICS_HW_DEVICE* rcs956,
    UINT32 time0,
    UINT32 timeout);

static UINT8 rcs956_calc_dcs(
    const UINT8* data,
    UINT32 data_len);

static UINT32 rcs956_check_felica_rf_param_diff(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout);

/* --------------------------------
 * Function
 * -------------------------------- */

/* ------------------------
 * Macro
 * ------------------------ */

#define RCS956_ACK_TIME(rcs956) ((rcs956)->priv_value)
#define RCS956_RAW_FUNC(rcs956) ((icsdrv_raw_func_t*)((rcs956)->priv_data))

/* ------------------------
 * Exported
 * ------------------------ */

/**
 * This function initializes the driver.
 *
 * \param  rcs956                [OUT] Handle to access the port.
 * \param  raw_func               [IN] Raw driver functions.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 */
UINT32 rcs956_initialize(
    ICS_HW_DEVICE* rcs956,
    const icsdrv_raw_func_t* raw_func)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_initialize"
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(raw_func, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_PTR(raw_func);

    rcs956->priv_data = (void*)raw_func;

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function opens a port to the device.
 *
 * \param  rcs956                [OUT] Handle to access the port.
 * \param  port_name              [IN] The port name to open.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_BUSY              Device busy.
 * \retval ICS_ERROR_PERMISSION        Permission denied.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_open(
    ICS_HW_DEVICE* rcs956,
    const char* port_name)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_open"
    UINT32 rc;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(port_name, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_STR(port_name);

    /* open the device */
    if (RCS956_RAW_FUNC(rcs956)->open != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->open(&(rcs956->handle), port_name);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->open()");
            return rc;
        }
    }

    if (RCS956_RAW_FUNC(rcs956)->set_speed != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->set_speed(rcs956->handle,
                                                RCS956_DEFAULT_SPEED);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->set_speed()");
            if (RCS956_RAW_FUNC(rcs956)->close != NULL) {
                RCS956_RAW_FUNC(rcs956)->close(rcs956->handle);
            }
            return rc;
        }
    }
    RCS956_SET_SPEED(rcs956, RCS956_DEFAULT_SPEED);

    if (RCS956_RAW_FUNC(rcs956)->clear_rx_queue != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->clear_rx_queue(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->clear_rx_queue()");
            /* ignore error */
        }
    }

    RCS956_ACK_TIME(rcs956) = 0;

    ICSLOG_DBG_HEX(rcs956->handle);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function closes the port.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_close(
    ICS_HW_DEVICE* rcs956)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_close"
    UINT32 rc;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);

    if (RCS956_RAW_FUNC(rcs956)->close != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->close(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->close()");
            return rc;
        }
    }
    rcs956->handle = ICS_INVALID_HANDLE;

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function initializes the device.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 */
UINT32 rcs956_initialize_device(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_initialize_device"
    UINT32 rc;
    UINT8 command[6];
    UINT8 response[3];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    /* reset the mode of the device */
    rc = rcs956_reset(rcs956, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_reset()");
        return rc;
    }

    /* RFConfiguration */
    /* set RF timeout */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_RF_CONFIGURATION;
    command[2] = RCS956_RFCONFIG_TIMEOUT;
    command[3] = 0x00;
    command[4] = 0x00;
    command[5] = 0x00;
    rc = rcs956_execute_command(rcs956,
                                command, 6,
                                2, response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != 2) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_RF_CONFIGURATION)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "RFConfiguration");
        return rc;
    }

    /* set RF retry */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_RF_CONFIGURATION;
    command[2] = RCS956_RFCONFIG_MAXRETRY;
    command[3] = 0x00;
    command[4] = 0x00;
    command[5] = 0x00;
    rc = rcs956_execute_command(rcs956,
                                command, 6,
                                2, response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != 2) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_RF_CONFIGURATION)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "RFConfiguration");
        return rc;
    }

    /* set RF wait */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_RF_CONFIGURATION;
    command[2] = RCS956_RFCONFIG_WAIT_TIME;
    command[3] = 143; /* 14.3ms (= 20ms - 5ms - 0.7ms) */
    rc = rcs956_execute_command(rcs956,
                                command, 4,
                                2, response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != 2) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_RF_CONFIGURATION)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "RFConfiguration");
        return rc;
    }

    /* clear the LargeBWPLL bit */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_WRITE_REGISTER;
    command[2] = 0x63; /* 630dh = CIU_ManualRCV register */
    command[3] = 0x0d;
    command[4] = 0x00; /* clear the LargetBWPLL bit */
    rc = rcs956_execute_command(rcs956,
                                command, 5,
                                3, response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != 3) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_WRITE_REGISTER) ||
        (response[2] != 0x00)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "WriteRegister");
        return rc;
    }

    /* check RF parameters for FeliCa initiator/target modes */
    rc = rcs956_check_felica_rf_param_diff(rcs956, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_check_felica_rf_param_diff()");
        return rc;
    }

    RCS956_SET_LAST_MODE(rcs956, RCS956_MODE_FELICA_INITIATOR);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function gets version of the FW.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  ic_type               [OUT] Number of IC type.
 * \param  version               [OUT] Version of firmware.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response.
 */
UINT32 rcs956_get_firmware_version(
    ICS_HW_DEVICE* rcs956,
    UINT8* ic_type,
    UINT16* version,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_get_firmware_version"
    UINT32 rc;
    UINT8 command[2];
    UINT8 response[6];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(ic_type, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(version, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    /* send a Get firmware version command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_GET_FIRMWARE_VERSION;
    rc = rcs956_execute_command(rcs956,
                                command, sizeof(command),
                                sizeof(response), response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != sizeof(response)) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_GET_FIRMWARE_VERSION)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }

    /* firmware version */
    *ic_type = response[2];
    *version = ((response[3] << 8) |
                (response[4] << 0));

    ICSLOG_DBG_HEX8(*ic_type);
    ICSLOG_DBG_HEX(*version);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function checks the device is alive.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 */
UINT32 rcs956_ping(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_ping"
    UINT32 rc;
    UINT8 ic_type;
    UINT16 version;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    /* send a GetFirmwareVersion command */
    rc = rcs956_get_firmware_version(rcs956, &ic_type, &version, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_get_firmware_version()");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function resets the mode of the device.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response.
 */
UINT32 rcs956_reset(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_reset"
    UINT32 rc;
    UINT8 command[3];
    UINT8 response[2];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    /* send a Reset command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_RESET;
    command[2] = RCS956_RESET_FUNCTION;
    rc = rcs956_execute_command(rcs956,
                                command, sizeof(command),
                                sizeof(response), response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != sizeof(response)) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_RESET)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }

    rc = rcs956_send_ack(rcs956, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_send_ack()");
        return rc;
    }

    /* drain the transmitting queue */
    if (RCS956_RAW_FUNC(rcs956)->drain_tx_queue != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->drain_tx_queue(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->drain_tx_queue()");
            return rc;
        }
    }

    /* wait */
    rc = utl_msleep(RCS956_RESET_WAIT_MS);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "utl_msleep()");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sends a command to the device and receives response.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  command                [IN] A command to write.
 * \param  command_len            [IN] The length of the command.
 * \param  max_response_len       [IN] The size of response buffer.
 * \param  response              [OUT] Recieved response.
 * \param  response_len          [OUT] The length of the response.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 * \retval ICS_ERROR_BUF_OVERFLOW      Response buffer overflow.
 */
UINT32 rcs956_execute_command(
    ICS_HW_DEVICE* rcs956,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_execute_command"
    UINT32 rc;
    UINT8 buf[RCS956_COMMAND_BUF_LEN];
    UINT32 response_pos;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(command, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(response, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(response_len, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_IN_RANGE(command_len, 1, RCS956_MAX_COMMAND_LEN,
                           ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(command_len);
    ICSLOG_DUMP(command, command_len);
    ICSLOG_DBG_UINT(max_response_len);
    ICSLOG_DBG_UINT(timeout);

    /* copy the command to the buffer */
    utl_memcpy(buf + RCS956_COMMAND_POS, command, command_len);

    /* execute the command */
    rc = rcs956_execute_command_internal(rcs956, buf, command_len,
                                         &response_pos, response_len,
                                         timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command_internal()");
        return rc;
    }

    ICSLOG_DBG_UINT(*response_len);

    if (*response_len <= max_response_len) {
        utl_memcpy(response, buf + response_pos, *response_len);
        ICSLOG_DUMP(response, *response_len);
    } else {
        utl_memcpy(response, buf + response_pos, max_response_len);
        ICSLOG_DUMP(response, max_response_len);
        rc = ICS_ERROR_BUF_OVERFLOW;
        ICSLOG_ERR_STR(rc, "Buffer overflow.");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sends an ACK to the device.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_send_ack(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_send_ack"
    UINT32 rc;
    static const UINT8 ack[6] = {0x00, 0x00, 0xff, 0x00, 0xff, 0x00};
    UINT32 time0;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    time0 = utl_get_time_msec();

    /* send command */
    if (RCS956_RAW_FUNC(rcs956)->write != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->write(rcs956->handle, ack, sizeof(ack),
                                            time0, timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->write()");
            return rc;
        }
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function cancels the previous command.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_cancel_command(
    ICS_HW_DEVICE* rcs956)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_cancel_command"
    UINT32 rc;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);

    /* drain the transmitting queue */
    if (RCS956_RAW_FUNC(rcs956)->drain_tx_queue != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->drain_tx_queue(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->drain_tx_queue()");
            return rc;
        }
    }
    rc = utl_msleep(1);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "utl_msleep()");
        return rc;
    }

    /* send an ACK packet */
    rc = rcs956_send_ack(rcs956, RCS956_MAX_CANCEL_COMMAND_TIMEOUT);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_send_ack()");
        return rc;
    }

    /* drain the transmitting queue */
    if (RCS956_RAW_FUNC(rcs956)->drain_tx_queue != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->drain_tx_queue(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->drain_tx_queue()");
            return rc;
        }
    }
    rc = utl_msleep(1);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "utl_msleep()");
        return rc;
    }

    /* clear the queue for receiving */
    if (RCS956_RAW_FUNC(rcs956)->clear_rx_queue != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->clear_rx_queue(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->clear_rx_queue()");
            return rc;
        }
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sends the FeliCa command and receives a FeliCa response.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  command                [IN] The card command to send.
 * \param  command_len            [IN] The length of the card command.
 * \param  max_response_len       [IN] The maximum length of response.
 * \param  response              [OUT] Received response.
 * \param  response_len          [OUT] The length of the response.
 * \param  command_timeout        [IN] Time-out at the device. (ms)
 * \param  timeout                [IN] Time-out. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_DEVICE            Error at device.
 * \retval ICS_ERROR_FRAME_CRC         CRC error.
 * \retval ICS_ERROR_RF_OFF            RF was turned off.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 * \retval ICS_ERROR_BUF_OVERFLOW      The length of the received response
 *                                     exceeded max_response_len.
 */
UINT32 rcs956_felica_command(
    ICS_HW_DEVICE* rcs956,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 command_timeout,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_felica_command"
    UINT32 rc;
    UINT32 rc2;
    UINT32 timeout_0_5;
    BOOL use_in_communicate_thru;
    UINT32 rcs956_response_pos;
    UINT32 rcs956_command_len;
    UINT32 rcs956_response_len;
    UINT8 buf[RCS956_COMMAND_BUF_LEN];
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    if (command_len > 0) {
        ICSLIB_CHKARG_NE(command, NULL, ICS_ERROR_INVALID_PARAM);
    }
    ICSLIB_CHKARG_LE(command_len, RCS956_MAX_FELICA_COMMAND_LEN,
                     ICS_ERROR_INVALID_PARAM);
    if (max_response_len > 0) {
        ICSLIB_CHKARG_NE(response, NULL, ICS_ERROR_INVALID_PARAM);
    }
    if (response != NULL) {
        ICSLIB_CHKARG_NE(response_len, NULL, ICS_ERROR_INVALID_PARAM);
    }

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(command_len);
    ICSLOG_DUMP(command, command_len);
    ICSLOG_DBG_UINT(max_response_len);
    ICSLOG_DBG_UINT(command_timeout);
    ICSLOG_DBG_UINT(timeout);

    /* make a command packet for RC-S956 */
    if (command_timeout >= ((UINT32)0x10000 / 2)) {
        timeout_0_5 = 0xffff;
        if ((response != NULL) && (command_len > 0)) {
            use_in_communicate_thru = TRUE;
        } else {
            use_in_communicate_thru = FALSE;
        }
    } else {
        timeout_0_5 = (command_timeout * 2);
        use_in_communicate_thru = FALSE;
    }
    buf[RCS956_COMMAND_POS + 0] = RCS956_COMMAND_CODE;
    if (use_in_communicate_thru) {
        buf[RCS956_COMMAND_POS + 1] = RCS956_CMD_IN_COMMUNICATE_THRU;
        buf[RCS956_COMMAND_POS + 2] = (command_len + 1);
        utl_memcpy(buf + RCS956_COMMAND_POS + 3, command, command_len);
        rcs956_command_len = (3 + command_len);
    } else {
        buf[RCS956_COMMAND_POS + 1] = RCS956_CMD_COMMUNICATE_THRU_EX;
        if (response == NULL) {
            buf[RCS956_COMMAND_POS + 2] = 0; /* no data to receive */
            buf[RCS956_COMMAND_POS + 3] = 0;
        } else {
            buf[RCS956_COMMAND_POS + 2] = (UINT8)((timeout_0_5 >> 0) & 0xff);
            buf[RCS956_COMMAND_POS + 3] = (UINT8)((timeout_0_5 >> 8) & 0xff);
        }
        if (command_len > 0) {
            buf[RCS956_COMMAND_POS + 4] = (command_len + 1);
            utl_memcpy(buf + RCS956_COMMAND_POS + 5, command, command_len);
            rcs956_command_len = (5 + command_len);
        } else {
            rcs956_command_len = 4;
        }
    }

    /* send the packet to RC-S956 */
    rc = rcs956_execute_command_internal(rcs956, buf, rcs956_command_len,
                                         &rcs956_response_pos,
                                         &rcs956_response_len,
                                         timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command_internal()");
        if ((rc == ICS_ERROR_TIMEOUT) ||
            (rc == ICS_ERROR_INVALID_RESPONSE)) {
            /* cancel the command */
            rc2 = rcs956_cancel_command(rcs956);
            if (rc2 != ICS_ERROR_SUCCESS) {
                ICSLOG_ERR_STR(rc2, "rcs956_cancel_command()");
                /* Note: ignore error */
            }
        }
        return rc;
    }
    if ((rcs956_response_len < 3) ||
        (buf[rcs956_response_pos + 0] != RCS956_RESPONSE_CODE) ||
        (use_in_communicate_thru &&
         (buf[rcs956_response_pos + 1] != RCS956_RES_IN_COMMUNICATE_THRU)) ||
        (!use_in_communicate_thru &&
         (buf[rcs956_response_pos + 1] != RCS956_RES_COMMUNICATE_THRU_EX))) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");

        /* cancel the command */
        rc2 = rcs956_cancel_command(rcs956);
        if (rc2 != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc2, "rcs956_cancel_command()");
            /* Note: ignore error */
        }

        return rc;
    }
    if (!use_in_communicate_thru &&
        (buf[rcs956_response_pos + 2] == RCS956_STATUS_TIMEOUT)) {
        if (response != NULL) {
            rc = ICS_ERROR_TIMEOUT;
            ICSLOG_ERR_STR(rc, "Time-out at the device.");
            return rc;
        }
    } else if (buf[rcs956_response_pos + 2] == RCS956_STATUS_RF_OFF) {
        rc = ICS_ERROR_RF_OFF;
        ICSLOG_ERR_STR(rc, "RF was turned off.");
        return rc;
    } else if (buf[rcs956_response_pos + 2] == RCS956_STATUS_CRC_ERR) {
        rc = ICS_ERROR_FRAME_CRC;
        ICSLOG_ERR_STR(rc, "CRC error.");
        return rc;
    } else if (buf[rcs956_response_pos + 2] != RCS956_STATUS_SUCCESS) {
        rc = ICS_ERROR_DEVICE;
        ICSLOG_ERR_STR(rc, "Error at the device.");
        return rc;
    }

    if ((rcs956_response_len > 3) &&
        (response != NULL)) {
        if (buf[rcs956_response_pos + 3] != (rcs956_response_len - 3)) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "Invalid response.");
            return rc;
        }

        *response_len = (rcs956_response_len - 4);
        ICSLOG_DBG_UINT(*response_len);
        if (*response_len > max_response_len) {
            utl_memcpy(response, buf + rcs956_response_pos + 4,
                       max_response_len);
            ICSLOG_DUMP(response, max_response_len);
            rc = ICS_ERROR_BUF_OVERFLOW;
            ICSLOG_ERR_STR(rc, "Buffer overflow.");
            return rc;
        }

        utl_memcpy(response, buf + rcs956_response_pos + 4, *response_len);
        ICSLOG_DUMP(response, *response_len);
    } else if (response_len != NULL) {
        *response_len = 0;
        ICSLOG_DBG_UINT(*response_len);
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function turns RF off.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response.
 */
UINT32 rcs956_rf_off(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_rf_off"
    UINT32 rc;
    UINT8 command[4];
    UINT8 response[2];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    /* RF off command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_RF_CONFIGURATION;
    command[2] = RCS956_RFCONFIG_RF_FIELD;
    command[3] = RCS956_RFCONFIG_RF_OFF;
    rc = rcs956_execute_command(rcs956,
                                command, sizeof(command),
                                2, response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != 2) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_RF_CONFIGURATION)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function turns RF on.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_DEV_IO            Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response.
 */
UINT32 rcs956_rf_on(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_rf_on"
    UINT32 rc;
    UINT8 command[4];
    UINT8 response[2];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    /* RF on command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_RF_CONFIGURATION;
    command[2] = RCS956_RFCONFIG_RF_FIELD;
    command[3] = RCS956_RFCONFIG_RF_ON;
    rc = rcs956_execute_command(rcs956,
                                command, sizeof(command),
                                2, response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }

    if ((response_len != sizeof(response)) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_RF_CONFIGURATION)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sets the communication speed of the device.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  speed                  [IN] The communication speed to set.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 */
UINT32 rcs956_set_dev_speed(
    ICS_HW_DEVICE* rcs956,
    UINT32 speed,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_set_dev_speed"
    UINT32 rc;
    UINT8 br;
    UINT8 command[3];
    UINT8 response[2];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(speed);
    ICSLOG_DBG_UINT(timeout);

    /* calculate BR */
    switch (speed) {
    case 9600:
        br = 0x00;
        break;
    case 19200:
        br = 0x01;
        break;
    case 38400:
        br = 0x02;
        break;
    case 57600:
        br = 0x03;
        break;
    case 115200:
        br = 0x04;
        break;
    case 230400:
        br = 0x05;
        break;
    case 460800:
        br = 0x06;
        break;
    case 921600:
        br = 0x07;
        break;
    default:
        rc = ICS_ERROR_INVALID_PARAM;
        ICSLOG_ERR_STR(rc, "Invalid speed.");
        return rc;
    }

    /* send a SetSerialBaudRate command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_SET_SERIAL_BAUDRATE;
    command[2] = br;
    rc = rcs956_execute_command(rcs956, command, sizeof(command),
                                sizeof(response),
                                response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }

    if ((response_len != sizeof(response)) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_SET_SERIAL_BAUDRATE)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }
    rc = rcs956_send_ack(rcs956, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_send_ack()");
        return rc;
    }

    rc = utl_msleep(RCS956_SET_DEV_SPEED_WAIT_MS);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "utl_msleep()");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sets the communication speed of the port.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  speed                  [IN] The communication speed to set.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_set_speed(
    ICS_HW_DEVICE* rcs956,
    UINT32 speed)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_set_speed"
    UINT32 rc;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_IS_VALID_SPEED(speed), FALSE,
                     ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(speed);

    if (RCS956_RAW_FUNC(rcs956)->set_speed != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->set_speed(rcs956->handle, speed);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->set_speed()");
            return rc;
        }
    }
    RCS956_SET_SPEED(rcs956, speed);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function puts the device into power down mode.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  wake_up                [IN] Source of wake up.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response.
 */
UINT32 rcs956_power_down(
    ICS_HW_DEVICE* rcs956,
    UINT8 wake_up,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_power_down"
    UINT32 rc;
    UINT8 buf[10];
    UINT32 time0;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_HEX8(wake_up);
    ICSLOG_DBG_UINT(timeout);

    /* clear the queue for receiving */
    if (RCS956_RAW_FUNC(rcs956)->clear_rx_queue != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->clear_rx_queue(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->clear_rx_queue()");
            return rc;
        }
    }

    time0 = utl_get_time_msec();

    /* send a Power down command */
    if (RCS956_RAW_FUNC(rcs956)->write != NULL) {
        buf[0] = 0x00;
        buf[1] = 0x00;
        buf[2] = 0xff;
        buf[3] = 3;
        buf[4] = (UINT8)-3;
        buf[5] = RCS956_COMMAND_CODE;
        buf[6] = RCS956_CMD_POWER_DOWN;
        buf[7] = wake_up;
        buf[8] = rcs956_calc_dcs(buf + 5, 3);
        buf[9] = 0x00;
        rc = RCS956_RAW_FUNC(rcs956)->write(rcs956->handle, buf, 5 + 3 + 2,
                                            time0, timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_func->write()");
            return rc;
        }
    }

    rc = rcs956_receive_ack(rcs956, time0, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_receive_ack()");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function wakes the device up.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_wake_up(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_wake_up"
    UINT32 rc;
    UINT8 wakeup_command = RCS956_WAKE_UP_BYTE;
    UINT32 time0;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    time0 = utl_get_time_msec();

    /* send 0x55 to the device */
    if (RCS956_RAW_FUNC(rcs956)->write != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->write(rcs956->handle,
                                            &wakeup_command,
                                            sizeof(wakeup_command),
                                            time0, timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_write()");
            return rc;
        }

        if (RCS956_RAW_FUNC(rcs956)->drain_tx_queue != NULL) {
            rc = RCS956_RAW_FUNC(rcs956)->drain_tx_queue(rcs956->handle);
            if (rc != ICS_ERROR_SUCCESS) {
                ICSLOG_ERR_STR(rc, "icsdrv_raw_drain_tx_queue()");
                /* Note: continue */
            }
        }

        /* sleep >200us */
        rc = utl_msleep(1);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "utl_msleep()");
            return rc;
        }
    }

    /* cancel the previous command (if PowerDown) */
    rc = rcs956_cancel_command(rcs956);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_cancel_command()");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function waits the device to be waked up.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_wait_for_wake_up(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_wait_for_wake_up"
    UINT32 rc;
    UINT8 buf[5 + 3 + 2];
    UINT32 time0;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(timeout);

    time0 = utl_get_time_msec();

    /* receive something */
    if (RCS956_RAW_FUNC(rcs956)->read != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->read(rcs956->handle,
                                           sizeof(buf),
                                           sizeof(buf),
                                           buf,
                                           NULL,
                                           time0, timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_read()");
            return rc;
        }
    }

    ICSLOG_DUMP(buf, sizeof(buf));

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sets the RF speed of the device.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  rf_speed               [IN] The RF speed to set. (212Kbps/424Kbps)
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 */
UINT32 rcs956_set_rf_speed(
    ICS_HW_DEVICE* rcs956,
    UINT8 rf_speed,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_set_rf_speed"
    UINT32 rc;
    UINT8 command[4];
    UINT8 response[2];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_IN_RANGE(rf_speed,
                           RCS956_RF_SPEED_212K, RCS956_RF_SPEED_424K,
                           ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_UINT(rf_speed);
    ICSLOG_DBG_UINT(timeout);

    /* send a SetSerialBaudRate command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_RF_CONFIGURATION;
    command[2] = RCS956_RFCONFIG_RF_SPEED;
    command[3] = rf_speed;
    rc = rcs956_execute_command(rcs956, command, sizeof(command),
                                sizeof(response),
                                response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }

    if ((response_len != sizeof(response)) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_RF_CONFIGURATION)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function clears the receiving queue.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_clear_rx_queue(
    ICS_HW_DEVICE* rcs956)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_clear_rx_queue"
    UINT32 rc;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);

    if (RCS956_RAW_FUNC(rcs956)->clear_rx_queue != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->clear_rx_queue(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_clear_rx_queue()");
            return rc;
        }
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function returns the time when this driver received the last ACK.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  ack_time              [OUT] The time when received an ACK.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 */
UINT32 rcs956_get_ack_time(
    ICS_HW_DEVICE* rcs956,
    UINT32* ack_time)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_get_ack_time"
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(ack_time, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);

    *ack_time = RCS956_ACK_TIME(rcs956);

    ICSLOG_DBG_UINT(*ack_time);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/* ------------------------
 * Internal
 * ------------------------ */

/**
 * This function sends a command to the device and receives response.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  command_buf        [IN/OUT] The buffer for command and response.
 *                                     The command should be placed at
 *                                     RCS956_COMMAND_POS.
 *                                     The response will be placed at
 *                                     response_pos.
 * \param  command_len            [IN] The length of the command.
 * \param  response_pos          [OUT] Start position of the recieved response.
 * \param  response_len          [OUT] The length of the response.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 */
static UINT32 rcs956_execute_command_internal(
    ICS_HW_DEVICE* rcs956,
    UINT8 command_buf[RCS956_COMMAND_BUF_LEN],
    UINT32 command_len,
    UINT32* response_pos,
    UINT32* response_len,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_execute_command_internal"
    UINT32 rc;
    UINT32 preamble_len;
    UINT8 dcs;
    UINT32 time0;
    UINT32 read_len;
    UINT32 n;
    BOOL ack_read;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956)->write, NULL,
                     ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956)->read, NULL,
                     ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_IN_RANGE(command_len, 1, RCS956_MAX_COMMAND_LEN,
                           ICS_ERROR_INVALID_PARAM);

    ack_read = FALSE;

    /* clear the queue for receiving */
    if (RCS956_RAW_FUNC(rcs956)->clear_rx_queue != NULL) {
        rc = RCS956_RAW_FUNC(rcs956)->clear_rx_queue(rcs956->handle);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_clear_rx_queue()");
            return rc;
        }
    }

    time0 = utl_get_time_msec();

    /* send command */
    dcs = rcs956_calc_dcs(command_buf + RCS956_COMMAND_POS, command_len);
    if (command_len <= 255) {
        /* normal frame */
        command_buf[RCS956_COMMAND_POS - 5] = 0x00;
        command_buf[RCS956_COMMAND_POS - 4] = 0x00;
        command_buf[RCS956_COMMAND_POS - 3] = 0xff;
        command_buf[RCS956_COMMAND_POS - 2] = (UINT8)command_len;
        command_buf[RCS956_COMMAND_POS - 1] =
            (UINT8)-command_buf[RCS956_COMMAND_POS - 2];
        preamble_len = 5;
    } else {
        /* extended frame */
        command_buf[RCS956_COMMAND_POS - 8] = 0x00;
        command_buf[RCS956_COMMAND_POS - 7] = 0x00;
        command_buf[RCS956_COMMAND_POS - 6] = 0xff;
        command_buf[RCS956_COMMAND_POS - 5] = 0xff;
        command_buf[RCS956_COMMAND_POS - 4] = 0xff;
        command_buf[RCS956_COMMAND_POS - 3] =
            (UINT8)((command_len >> 8) & 0xff);
        command_buf[RCS956_COMMAND_POS - 2] =
            (UINT8)((command_len >> 0) & 0xff);
        command_buf[RCS956_COMMAND_POS - 1] =
            (UINT8)-(command_buf[RCS956_COMMAND_POS - 3] +
                     command_buf[RCS956_COMMAND_POS - 2]);
        preamble_len = 8;
    }
    command_buf[RCS956_COMMAND_POS + command_len] = dcs;
    command_buf[RCS956_COMMAND_POS + command_len + 1] = 0x00;

    rc = RCS956_RAW_FUNC(rcs956)->write(rcs956->handle,
                                        command_buf +
                                        RCS956_COMMAND_POS - preamble_len,
                                        preamble_len + command_len + 2,
                                        time0, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "icsdrv_raw_write()");
        return rc;
    }

    /* receive ACK, response header */
    rc = RCS956_RAW_FUNC(rcs956)->read(rcs956->handle,
                                       6,
                                       RCS956_COMMAND_BUF_LEN,
                                       command_buf,
                                       &read_len,
                                       time0, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "icsdrv_raw_read()");
        return rc;
    }
    if (utl_memcmp(command_buf, "\x00\x00\xff\x00\xff\x00", 6) == 0) {
        RCS956_ACK_TIME(rcs956) = utl_get_time_msec();
        ack_read = TRUE;
        read_len -= 6;
        utl_memcpy(command_buf, command_buf + 6, read_len);

        if (read_len < 6) {
            n = read_len;
            rc = RCS956_RAW_FUNC(rcs956)->read(rcs956->handle,
                                               6 - read_len,
                                               RCS956_COMMAND_BUF_LEN - n,
                                               command_buf + n,
                                               &read_len,
                                               time0, timeout);
            if (rc != ICS_ERROR_SUCCESS) {
                ICSLOG_ERR_STR(rc, "icsdrv_raw_read()");
                return rc;
            }
            read_len += n;
        }
    }

    /* check header */
    if (utl_memcmp(command_buf, "\x00\x00\xff", 3) != 0) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response header.");
        return rc;
    }
    if ((command_buf[3] == 0xff) && (command_buf[4] == 0xff)) {
        /* extended frame */
        if (read_len < 9) {
            n = read_len;
            rc = RCS956_RAW_FUNC(rcs956)->read(rcs956->handle,
                                               9 - read_len,
                                               RCS956_COMMAND_BUF_LEN - n,
                                               command_buf + n,
                                               &read_len,
                                               time0, timeout);
            if (rc != ICS_ERROR_SUCCESS) {
                ICSLOG_ERR_STR(rc, "icsdrv_raw_read()");
                return rc;
            }
            read_len += n;
        }
        if (((command_buf[5] + command_buf[6] + command_buf[7]) & 0xff) != 0) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "Invalid response header.");
            return rc;
        }
        *response_pos = 8;
        *response_len = (((UINT32)command_buf[5] << 8) |
                         ((UINT32)command_buf[6] << 0));
    } else {
        /* normal frame */
        if (((command_buf[3] + command_buf[4]) & 0xff) != 0) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "Invalid response header.");
            return rc;
        }
        *response_pos = 5;
        *response_len = command_buf[3];
    }
    if (*response_len > RCS956_MAX_RESPONSE_LEN) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Too long response length.");
        return rc;
    }

    /* read the rest of packet */
    if (read_len < (*response_pos + *response_len + 2)) {
        n = (*response_pos + *response_len + 2 - read_len);
        rc = RCS956_RAW_FUNC(rcs956)->read(rcs956->handle,
                                           n, n,
                                           command_buf + read_len,
                                           NULL,
                                           time0, timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "icsdrv_raw_read()");
            return rc;
        }
    }

    /* check response */
    dcs = rcs956_calc_dcs(command_buf + *response_pos, *response_len);
    if ((command_buf[*response_pos + *response_len] != dcs) ||
        (command_buf[*response_pos + *response_len + 1] != 0x00)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response body.");
        return rc;
    }

    if (!ack_read) {
        RCS956_ACK_TIME(rcs956) = utl_get_time_msec();
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function receives an ACK from the device.
 *
 * @param  rwdev                  [IN] The handle to access the port.
 * @param  time0                  [IN] The base time for time-out. (ms)
 * @param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 */
static UINT32 rcs956_receive_ack(
    ICS_HW_DEVICE* rcs956,
    UINT32 time0,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_receive_ack"
    UINT32 rc;
    UINT8 ack_buf[6];
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956)->read, NULL,
                     ICS_ERROR_INVALID_PARAM);

    /* receive ACK */
    rc = RCS956_RAW_FUNC(rcs956)->read(rcs956->handle,
                                       6, 6,
                                       ack_buf,
                                       NULL,
                                       time0, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "icsdrv_raw_read()");
        return rc;
    }
    if (utl_memcmp(ack_buf, "\x00\x00\xff\x00\xff\x00", 6) != 0) {
        /* not ACK */
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Not ACK packet.");
        return rc;
    }

    RCS956_ACK_TIME(rcs956) = utl_get_time_msec();

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function calculates the DCS of a data.
 *
 * \param  data                   [IN] A data.
 * \param  data_len               [IN] The length of the data.
 *
 * \return The DCS of the data.
 */
static UINT8 rcs956_calc_dcs(
    const UINT8* data,
    UINT32 data_len)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_calc_dcs"
    UINT8 sum;
    UINT32 i;
    ICSLOG_FUNC_BEGIN;

    sum = 0;
    for (i = 0; i < data_len; i++) {
        sum += data[i];
    }

    ICSLOG_FUNC_END;
    return (UINT8)-(sum & 0xff);
}

/**
 * This function checks the RF parameters for FeliCa target mode are
 * different from the parameters for FeliCa initiator. If different,
 * set NEED_CHANGE_FELICA_RF_PARAM_FLAG of rcs956 and initialize
 * the FeliCa RF parameters for FeliCa initiator mode.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 */
static UINT32 rcs956_check_felica_rf_param_diff(
    ICS_HW_DEVICE* rcs956,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_check_felica_rf_param_diff"
    UINT32 rc;
    UINT8 command[18];
    UINT8 response[10];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

#ifdef CONFIG_DISABLE_RCS956_CHECK_RF_PARAM
    RCS956_SET_NEED_CHANGE_FELICA_RF_PARAM_FLAG(rcs956, 0);
#else /* CONFIG_DISABLE_RCS956_CHECK_RF_PARAM */
    /* read and check FeliCa RF parameters in EEPROM */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_READ_REGISTER;
    command[2] = 0xa0; /* a01ch = EEPROM:GsNOn (initiator) */
    command[3] = 0x1c;
    command[4] = 0xa0; /* a01dh = EEPROM:CWGsP (initiator) */
    command[5] = 0x1d;
    command[6] = 0xa0; /* a01eh = EEPROM:ModGsP (initiator) */
    command[7] = 0x1e;
    command[8] = 0xa0; /* a022h = EEPROM:GsNOff (initiator) */
    command[9] = 0x22;
    command[10] = 0xa0; /* a0cbh = EEPROM:GsNOn (target) */
    command[11] = 0xcb;
    command[12] = 0xa0; /* a0cch = EEPROM:CWGsP (target) */
    command[13] = 0xcc;
    command[14] = 0xa0; /* a0cdh = EEPROM:ModGsP (target) */
    command[15] = 0xcd;
    command[16] = 0xa0; /* a0ceh = EEPROM:GsNOff (target) */
    command[17] = 0xce;
    rc = rcs956_execute_command(rcs956,
                                command, 18,
                                10, response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len == 1) &&
        (response[0] == 0x7f)) {
        /* Syntax error: EEPROM does not exist */
        RCS956_SET_NEED_CHANGE_FELICA_RF_PARAM_FLAG(rcs956, 0);
    } else if ((response_len != 10) ||
               (response[0] != RCS956_RESPONSE_CODE) ||
               (response[1] != RCS956_RES_READ_REGISTER)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "ReadRegister");
        return rc;
    } else if ((response[2] != response[6]) ||
               (response[3] != response[7]) ||
               (response[4] != response[8]) ||
               (response[5] != response[9])) {
        RCS956_SET_NEED_CHANGE_FELICA_RF_PARAM_FLAG(rcs956, 1);
    } else {
        RCS956_SET_NEED_CHANGE_FELICA_RF_PARAM_FLAG(rcs956, 0);
    }
#endif /* CONFIG_DISABLE_RCS956_CHECK_RF_PARAM */

    if (RCS956_NEED_CHANGE_FELICA_RF_PARAM(rcs956)) {
        /* read RF parameters for FeliCa initiator mode from EEPROM */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_READ_REGISTER;
        command[2] = 0xa0; /* a01bh = EEPROM:RFCfg (initiator) */
        command[3] = 0x1b;
        command[4] = 0xa0; /* a01ch = EEPROM:GsNOn (initiator) */
        command[5] = 0x1c;
        command[6] = 0xa0; /* a01dh = EEPROM:CWGsP (initiator) */
        command[7] = 0x1d;
        command[8] = 0xa0; /* a01eh = EEPROM:ModGsP (initiator) */
        command[9] = 0x1e;
        command[10] = 0xa0; /* a01fh = EEPROM:Demod RfOn */
        command[11] = 0x1f;
        command[12] = 0xa0; /* a020h = EEPROM:RxThreshold (initiator) */
        command[13] = 0x20;
        command[14] = 0xa0; /* a021h = EEPROM:Demod RfOff */
        command[15] = 0x21;
        command[16] = 0xa0; /* a022h = EEPROM:GsNOff (initiator) */
        command[17] = 0x22;
        rc = rcs956_execute_command(rcs956,
                                    command, 18,
                                    10, response, &response_len,
                                    timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
            if (rc == ICS_ERROR_BUF_OVERFLOW) {
                rc = ICS_ERROR_INVALID_RESPONSE;
            }
            return rc;
        }
        if ((response_len != 10) ||
            (response[0] != RCS956_RESPONSE_CODE) ||
            (response[1] != RCS956_RES_READ_REGISTER)) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "ReadRegister");
            return rc;
        }

        /* write RF parameters for FeliCa initiator mode to RAM */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_RF_CONFIGURATION;
        command[2] = RCS956_RFCONFIG_FELICA_RF_PARAM;
        utl_memcpy(command + 3, response + 2, 8);
        rc = rcs956_execute_command(rcs956,
                                    command, 11,
                                    2, response, &response_len,
                                    timeout);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
            if (rc == ICS_ERROR_BUF_OVERFLOW) {
                rc = ICS_ERROR_INVALID_RESPONSE;
            }
            return rc;
        }
        if ((response_len != 2) ||
            (response[0] != RCS956_RESPONSE_CODE) ||
            (response[1] != RCS956_RES_RF_CONFIGURATION)) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "ReadRegister");
            return rc;
        }
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}
