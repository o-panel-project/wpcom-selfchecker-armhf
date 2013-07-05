/**
 * \brief    RC-S956 Driver (ISO/IEC 14443-3 Type B)
 * \date     2009/08/27
 * \author   Copyright 2005,2006,2007,2008,2009 Sony Corporation
 */

#undef ICSLOG_MODULE
#define ICSLOG_MODULE "DGB"

#include "ics_types.h"
#include "ics_error.h"
#include "icslib_chk.h"
#include "icslog.h"
#include "utl.h"

#include "rcs956.h"
#include "rcs956_typeb.h"

/* --------------------------------
 * Constant
 * -------------------------------- */

#define RCS956_MAX_COMMAND_LEN                  265
#define RCS956_MAX_RESPONSE_LEN                 265

#define RCS956_COMMAND_CODE                     0xd4
#define RCS956_RESPONSE_CODE                    0xd5

#define RCS956_CMD_SET_PARAMETERS               0x12
#define RCS956_RES_SET_PARAMETERS               0x13

#define RCS956_CMD_READ_REGISTER                0x06
#define RCS956_RES_READ_REGISTER                0x07
#define RCS956_CMD_WRITE_REGISTER               0x08
#define RCS956_RES_WRITE_REGISTER               0x09
#define RCS956_CMD_RF_CONFIGURATION             0x32
#define RCS956_RES_RF_CONFIGURATION             0x33
#define RCS956_CMD_IN_DATA_EXCHANGE             0x40
#define RCS956_RES_IN_DATA_EXCHANGE             0x41
#define RCS956_CMD_IN_DESELECT                  0x44
#define RCS956_RES_IN_DESELECT                  0x45
#define RCS956_CMD_IN_LIST_PASSIVE_TARGET       0x4a
#define RCS956_RES_IN_LIST_PASSIVE_TARGET       0x4b
#define RCS956_CMD_IN_RELEASE                   0x52
#define RCS956_RES_IN_RELEASE                   0x53
#define RCS956_CMD_IN_SELECT                    0x54
#define RCS956_RES_IN_SELECT                    0x55
#define RCS956_CMD_COMMUNICATE_THRU_EX          0xa0
#define RCS956_RES_COMMUNICATE_THRU_EX          0xa1

#define RCS956_RFCONFIG_MAXRETRY                0x05
#define RCS956_RFCONFIG_TYPEA_RF_PARAM          0x0a
#define RCS956_RFCONFIG_TYPEB_RF_PARAM          0x0c

#define RCS956_SET_PARAMETERS_DEFAULT           0x1c

#define RCS956_STATUS_SUCCESS                   0x00
#define RCS956_STATUS_TIMEOUT                   0x01
#define RCS956_STATUS_CRC_ERR                   0x02
#define RCS956_STATUS_PARITY_ERR                0x03
#define RCS956_STATUS_OVERFLOW_ERR              0x07
#define RCS956_STATUS_PROTOCOL_ERR              0x0b
#define RCS956_STATUS_RF_FORMAT_ERR             0x13
#define RCS956_STATUS_RF_OFF                    0x31

/* Reset Timeout */
#define RCS956_TYPEB_RESET_TIMEOUT              200 /* ms */

/* WriteRegister Timeout */
#define RCS956_TYPEB_WRITE_REGISTER_TIMEOUT     200 /* ms */

/* ReadRegister Timeout */
#define RCS956_TYPEB_READ_REGISTER_TIMEOUT      200 /* ms */

/* RFConfiguration Timeout */
#define RCS956_TYPEB_RF_CONFIGURATION_TIMEOUT   200 /* ms */

/* WUPB Timeout (at RC-S956) */
#define RCS956_WUPB_TIMEOUT                     5   /* ms */

/* --------------------------------
 * Prototype Declaration
 * -------------------------------- */

static UINT32 rcs956_typeb_setup_initiator(
    ICS_HW_DEVICE* rcs956);

#ifndef CONFIG_RCS956_TYPEB_NOT_SET_RXNOERR
static UINT32 rcs956_typeb_set_rxnoerr(
    ICS_HW_DEVICE* rcs956);
#endif /* !CONFIG_RCS956_TYPEB_NOT_SET_RXNOERR */

/* --------------------------------
 * Function
 * -------------------------------- */

/* ------------------------
 * Macro
 * ------------------------ */

#define RCS956_RAW_FUNC(rcs956) ((icsdrv_raw_func_t*)((rcs956)->priv_data))

/* ------------------------
 * Exported
 * ------------------------ */

/**
 * This function detects a Type B card.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  afi                    [IN] AFI.
 * \param  card                  [OUT] Card information.
 * \param  attrib                [OUT] Received ATTRIB. (or NULL)
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 * \retval ICS_ERROR_BUF_OVERFLOW      The length of ATTRIB exceeded the limit.
 */
UINT32 rcs956_typeb_polling(
    ICS_HW_DEVICE* rcs956,
    UINT8 afi,
    typeb_card_t* card,
    typeb_card_attrib_t* attrib,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typeb_polling"
    UINT32 rc;
    UINT8 command[16];
    UINT8 response[17 + 64];
    UINT32 command_len;
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(card, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_HEX8(afi);
    ICSLOG_DBG_PTR(card);
    ICSLOG_DBG_PTR(attrib);
    ICSLOG_DBG_UINT(timeout);

    /* reset the mode of the device */
    rc = rcs956_reset(rcs956, RCS956_TYPEB_RESET_TIMEOUT);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_reset()");
        return rc;
    }

    /* set up the device for Type B initiator mode */
    rc = rcs956_typeb_setup_initiator(rcs956);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_typeb_setup_initiator()");
        return rc;
    }

    /* InListPassiveTarget */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_IN_LIST_PASSIVE_TARGET;
    command[2] = 0x01; /* MaxTg = 1 */
    command[3] = 0x03; /* Type B */
    command[4] = afi;
    command_len = 5;
    rc = rcs956_execute_command(rcs956,
                                command, command_len,
                                sizeof(response), response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len < 3) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_IN_LIST_PASSIVE_TARGET) ||
        (response[2] > 1)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "InListPassiveTarget");
        return rc;
    }
    if (response[2] == 0) {
        rc = ICS_ERROR_TIMEOUT;
        ICSLOG_ERR_STR(rc, "Time-out.");
        return rc;
    }

    /* parse the response */
    if ((response_len < (3 + 14)) ||
        (response[3] != 1) ||
        (response_len != (3 + 14 + response[16]))) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }

    utl_memcpy(card->pupi, response + 5, 4);
    utl_memcpy(card->application_data, response + 9, 4);
    utl_memcpy(card->protocol_info, response + 13, 3);
    ICSLOG_DUMP(card->pupi, 4);
    ICSLOG_DUMP(card->application_data, 4);
    ICSLOG_DUMP(card->protocol_info, 3);

    card->attrib = attrib;
    if (attrib != NULL) {
        attrib->len = response[16];
        ICSLOG_DBG_UINT(attrib->len);
        if (attrib->len > TYPEB_CARD_MAX_ATTRIB_LEN) {
            /* Note: never reach here */
            utl_memcpy(attrib->data, response + 17, TYPEB_CARD_MAX_ATTRIB_LEN);
            ICSLOG_DUMP(attrib->data, TYPEB_CARD_MAX_ATTRIB_LEN);
            rc = ICS_ERROR_BUF_OVERFLOW;
            ICSLOG_ERR_STR(rc, "Buffer overflow.");
            return rc;
        }

        utl_memcpy(attrib->data, response + 17, attrib->len);
        ICSLOG_DUMP(attrib->data, attrib->len);
    }

#ifndef CONFIG_RCS956_TYPEB_NOT_SET_RXNOERR
    rc = rcs956_typeb_set_rxnoerr(rcs956);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_typeb_set_rxnoerr()");
        return rc;
    }
#endif /* !CONFIG_RCS956_TYPEB_NOT_SET_RXNOERR */

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sends the 14443-4B command and receives a 14443-4B response.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  card                   [IN] The card to send the command. (ignored)
 * \param  command                [IN] The card command to send.
 * \param  command_len            [IN] The length of the card command.
 * \param  max_response_len       [IN] The maximum length of response.
 * \param  response              [OUT] Received response.
 * \param  response_len          [OUT] The length of the response.
 * \param  command_timeout        [IN] Time-out at the device. (ms) (ignored)
 * \param  timeout                [IN] Time-out. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_DEVICE            Error at device.
 * \retval ICS_ERROR_FRAME_CRC         CRC/parity error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 * \retval ICS_ERROR_BUF_OVERFLOW      The length of the received response
 *                                     exceeded max_response_len.
 */
UINT32 rcs956_typeb_command(
    ICS_HW_DEVICE* rcs956,
    typeb_card_t* card,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 command_timeout,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typeb_command"
    UINT32 rc;
    UINT32 rc2;
    UINT8 rcs956_command[RCS956_MAX_COMMAND_LEN];
    UINT32 rcs956_command_len;
    UINT8 rcs956_response[RCS956_MAX_RESPONSE_LEN];
    UINT32 rcs956_response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    if (command_len > 0) {
        ICSLIB_CHKARG_NE(command, NULL, ICS_ERROR_INVALID_PARAM);
    }
    ICSLIB_CHKARG_LE(command_len, RCS956_MAX_TYPEB_COMMAND_LEN,
                     ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(response, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(response_len, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_PTR(card);
    ICSLOG_DBG_UINT(command_len);
    ICSLOG_DUMP(command, command_len);
    ICSLOG_DBG_UINT(max_response_len);
    ICSLOG_DBG_UINT(command_timeout);
    ICSLOG_DBG_UINT(timeout);

    /* InDataExchange command */
    rcs956_command[0] = RCS956_COMMAND_CODE;
    rcs956_command[1] = RCS956_CMD_IN_DATA_EXCHANGE;
    rcs956_command[2] = 0x01; /* target num */
    utl_memcpy(rcs956_command + 3, command, command_len);
    rcs956_command_len = (3 + command_len);
    rc = rcs956_execute_command(rcs956,
                                rcs956_command, rcs956_command_len,
                                sizeof(rcs956_response),
                                rcs956_response, &rcs956_response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
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
        (rcs956_response[0] != RCS956_RESPONSE_CODE) ||
        (rcs956_response[1] != RCS956_RES_IN_DATA_EXCHANGE)) {
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
    if (rcs956_response[2] == RCS956_STATUS_TIMEOUT) {
        rc = ICS_ERROR_TIMEOUT;
        ICSLOG_ERR_STR(rc, "Time-out at the device.");
        return rc;
    } else if ((rcs956_response[2] == RCS956_STATUS_CRC_ERR) ||
               (rcs956_response[2] == RCS956_STATUS_PARITY_ERR)) {
        rc = ICS_ERROR_FRAME_CRC;
        ICSLOG_ERR_STR(rc, "CRC/parity error.");
        return rc;
    } else if ((rcs956_response[2] == RCS956_STATUS_OVERFLOW_ERR) ||
               (rcs956_response[2] == RCS956_STATUS_PROTOCOL_ERR) ||
               (rcs956_response[2] == RCS956_STATUS_RF_FORMAT_ERR)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid RF response.");
        return rc;
    } else if (rcs956_response[2] != RCS956_STATUS_SUCCESS) {
        rc = ICS_ERROR_DEVICE;
        ICSLOG_ERR_STR(rc, "Error at the device.");
        return rc;
    }

    *response_len = (rcs956_response_len - 3);
    ICSLOG_DBG_UINT(*response_len);
    if (*response_len > max_response_len) {
        utl_memcpy(response, rcs956_response + 3, max_response_len);
        ICSLOG_DUMP(response, max_response_len);
        rc = ICS_ERROR_BUF_OVERFLOW;
        ICSLOG_ERR_STR(rc, "Buffer overflow.");
        return rc;
    }

    utl_memcpy(response, rcs956_response + 3, *response_len);
    ICSLOG_DUMP(response, *response_len);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sends the 14443-3B command and receives a 14443-3B response.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  card                   [IN] The card to send the command. (ignored)
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
 * \retval ICS_ERROR_FRAME_CRC         CRC/parity error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 * \retval ICS_ERROR_BUF_OVERFLOW      The length of the received response
 *                                     exceeded max_response_len.
 */
UINT32 rcs956_typeb_raw_command(
    ICS_HW_DEVICE* rcs956,
    typeb_card_t* card,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 command_timeout,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typeb_raw_command"
    UINT32 rc;
    UINT32 rc2;
    UINT32 timeout_0_5;
    UINT8 rcs956_command[RCS956_MAX_COMMAND_LEN];
    UINT32 rcs956_command_len;
    UINT8 rcs956_response[RCS956_MAX_RESPONSE_LEN];
    UINT32 rcs956_response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    if (command_len > 0) {
        ICSLIB_CHKARG_NE(command, NULL, ICS_ERROR_INVALID_PARAM);
    }
    ICSLIB_CHKARG_LE(command_len, RCS956_MAX_TYPEB_RAW_COMMAND_LEN,
                     ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(response, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(response_len, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_PTR(card);
    ICSLOG_DBG_UINT(command_len);
    ICSLOG_DUMP(command, command_len);
    ICSLOG_DBG_UINT(max_response_len);
    ICSLOG_DBG_UINT(command_timeout);
    ICSLOG_DBG_UINT(timeout);

    /* CommunicateThruEX command */
    if (command_timeout >= ((UINT32)0x10000 / 2)) {
        timeout_0_5 = 0xffff;
    } else {
        timeout_0_5 = (command_timeout * 2);
    }
    rcs956_command[0] = RCS956_COMMAND_CODE;
    rcs956_command[1] = RCS956_CMD_COMMUNICATE_THRU_EX;
    rcs956_command[2] = (UINT8)((timeout_0_5 >> 0) & 0xff);
    rcs956_command[3] = (UINT8)((timeout_0_5 >> 8) & 0xff);
    utl_memcpy(rcs956_command + 4, command, command_len);
    rcs956_command_len = (4 + command_len);
    rc = rcs956_execute_command(rcs956,
                                rcs956_command, rcs956_command_len,
                                sizeof(rcs956_response),
                                rcs956_response, &rcs956_response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
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
        (rcs956_response[0] != RCS956_RESPONSE_CODE) ||
        (rcs956_response[1] != RCS956_RES_COMMUNICATE_THRU_EX)) {
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
    if (rcs956_response[2] == RCS956_STATUS_TIMEOUT) {
        rc = ICS_ERROR_TIMEOUT;
        ICSLOG_ERR_STR(rc, "Time-out at the device.");
        return rc;
    } else if ((rcs956_response[2] == RCS956_STATUS_CRC_ERR) ||
               (rcs956_response[2] == RCS956_STATUS_PARITY_ERR)) {
        rc = ICS_ERROR_FRAME_CRC;
        ICSLOG_ERR_STR(rc, "CRC/parity error.");
        return rc;
    } else if ((rcs956_response[2] == RCS956_STATUS_OVERFLOW_ERR) ||
               (rcs956_response[2] == RCS956_STATUS_PROTOCOL_ERR) ||
               (rcs956_response[2] == RCS956_STATUS_RF_FORMAT_ERR)) {
        if (rcs956_response[2] == RCS956_STATUS_OVERFLOW_ERR) {
            utl_msleep(7);
        }
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid RF response.");
        return rc;
    } else if (rcs956_response[2] != RCS956_STATUS_SUCCESS) {
        rc = ICS_ERROR_DEVICE;
        ICSLOG_ERR_STR(rc, "Error at the device.");
        return rc;
    }

    *response_len = (rcs956_response_len - 3);
    ICSLOG_DBG_UINT(*response_len);
    if (*response_len > max_response_len) {
        utl_memcpy(response, rcs956_response + 3, max_response_len);
        ICSLOG_DUMP(response, max_response_len);
        rc = ICS_ERROR_BUF_OVERFLOW;
        ICSLOG_ERR_STR(rc, "Buffer overflow.");
        return rc;
    }

    utl_memcpy(response, rcs956_response + 3, *response_len);
    ICSLOG_DUMP(response, *response_len);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function deselects the card.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  card                   [IN] The card to deselect. (ignored)
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_DEVICE            Error at device.
 * \retval ICS_ERROR_FRAME_CRC         CRC/parity error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response.
 */
UINT32 rcs956_typeb_deselect(
    ICS_HW_DEVICE* rcs956,
    typeb_card_t* card,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typeb_deselect"
    UINT32 rc;
    UINT8 command[4];
    UINT8 response[4];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_PTR(card);
    ICSLOG_DBG_UINT(timeout);

    /* InDeselect command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_IN_DESELECT;
    command[2] = 0x01; /* num of targets */
    command[3] = 0x01; /* target num */
    rc = rcs956_execute_command(rcs956,
                                command, sizeof(command),
                                4, response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != 4) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_IN_DESELECT) ||
        (response[2] != 0x01)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }
    if (response[3] == RCS956_STATUS_TIMEOUT) {
        rc = ICS_ERROR_TIMEOUT;
        ICSLOG_ERR_STR(rc, "Time-out at the device.");
        return rc;
    } else if ((response[3] == RCS956_STATUS_CRC_ERR) ||
               (response[3] == RCS956_STATUS_PARITY_ERR)) {
        rc = ICS_ERROR_FRAME_CRC;
        ICSLOG_ERR_STR(rc, "CRC/parity error.");
        return rc;
    } else if ((response[3] == RCS956_STATUS_OVERFLOW_ERR) ||
               (response[3] == RCS956_STATUS_PROTOCOL_ERR) ||
               (response[3] == RCS956_STATUS_RF_FORMAT_ERR)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid RF response.");
        return rc;
    } else if (response[3] != RCS956_STATUS_SUCCESS) {
        rc = ICS_ERROR_DEVICE;
        ICSLOG_ERR_STR(rc, "Error at the device.");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function selects the card.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  afi                    [IN] AFI.
 * \param  card                  [OUT] The card to select.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_DEVICE            Error at device.
 * \retval ICS_ERROR_FRAME_CRC         CRC/parity error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response.
 * \retval ICS_ERROR_BUF_OVERFLOW      The length of ATTRIB exceeded the limit.
 */
UINT32 rcs956_typeb_select(
    ICS_HW_DEVICE* rcs956,
    UINT8 afi,
    typeb_card_t* card,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typeb_select"
    UINT32 rc;
    UINT8 command[7];
    UINT8 response[RCS956_MAX_RESPONSE_LEN];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(card, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_HEX8(afi);
    ICSLOG_DBG_PTR(card);
    ICSLOG_DBG_UINT(timeout);

    /* CommunicateThruEX(WUPB) command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_COMMUNICATE_THRU_EX;
    command[2] = (UINT8)(((RCS956_WUPB_TIMEOUT * 2) << 0) & 0xff);
    command[3] = (UINT8)(((RCS956_WUPB_TIMEOUT * 2) << 8) & 0xff);
    command[4] = 0x05; /* APf */
    command[5] = 0x00; /* all */
    command[6] = 0x08; /* PARAM (WUPB) */
    rc = rcs956_execute_command(rcs956,
                                command, 7,
                                sizeof(response), response, &response_len,
                                timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len < 3) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_COMMUNICATE_THRU_EX)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }
    if (response[2] == RCS956_STATUS_TIMEOUT) {
        rc = ICS_ERROR_TIMEOUT;
        ICSLOG_ERR_STR(rc, "Time-out at the device.");
        return rc;
    } else if ((response[2] == RCS956_STATUS_CRC_ERR) ||
               (response[2] == RCS956_STATUS_PARITY_ERR)) {
        rc = ICS_ERROR_FRAME_CRC;
        ICSLOG_ERR_STR(rc, "CRC/parity error.");
        return rc;
    } else if ((response[2] == RCS956_STATUS_OVERFLOW_ERR) ||
               (response[2] == RCS956_STATUS_PROTOCOL_ERR) ||
               (response[2] == RCS956_STATUS_RF_FORMAT_ERR)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid RF response.");
        return rc;
    } else if (response[2] != RCS956_STATUS_SUCCESS) {
        rc = ICS_ERROR_DEVICE;
        ICSLOG_ERR_STR(rc, "Error at the device.");
        return rc;
    }

    /* polling */
    rc = rcs956_typeb_polling(rcs956, afi, card, card->attrib, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_typeb_polling()");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sets up RC-S956 for Type B initiator mode.
 *
 * \param  rcs956                 [IN] RC-S956 device.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 */
static UINT32 rcs956_typeb_setup_initiator(
    ICS_HW_DEVICE* rcs956)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typeb_setup_initiator"
    UINT32 rc;
    UINT8 command[24];
    UINT8 response[13];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    if (RCS956_LAST_MODE(rcs956) != RCS956_MODE_TYPEB_INITIATOR) {
        /* clear the LargeBWPLL bit */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_WRITE_REGISTER;
        command[2] = 0x63; /* 630dh = CIU_ManualRCV register */
        command[3] = 0x0d;
        command[4] = 0x00; /* clear the LargetBWPLL bit */
        rc = rcs956_execute_command(rcs956, command, 5,
                                    3, response, &response_len,
                                    RCS956_TYPEB_WRITE_REGISTER_TIMEOUT);
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

        /* read RF parameters for Type A/B initiator mode from EEPROM */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_READ_REGISTER;
        command[2] = 0xa0; /* a0c0h = EEPROM:RFCfg (Type B initiator) */
        command[3] = 0xc0;
        command[4] = 0xa0; /* a011h = EEPROM:GsNOn (Type A initiator) */
        command[5] = 0x11;
        command[6] = 0xa0; /* a0c1h = EEPROM:CWGsP (Type B initiator) */
        command[7] = 0xc1;
        command[8] = 0xa0; /* a013h = EEPROM:ModGsP (Type A initiator) */
        command[9] = 0x13;
        command[10] = 0xa0; /* a0c2h = EEPROM:Demod RfOn (Type B) */
        command[11] = 0xc2;
        command[12] = 0xa0; /* a015h = EEPROM:RxThres (Type A initiator) */
        command[13] = 0x15;
        command[14] = 0xa0; /* a016h = EEPROM:Demod RfOff */
        command[15] = 0x16;
        command[16] = 0xa0; /* a017h = EEPROM:GsNOff (Type A initiator) */
        command[17] = 0x17;
        command[18] = 0xa0; /* a018h = EEPROM:ModWidth (Type A initiator) */
        command[19] = 0x18;
        command[20] = 0xa0; /* a019h = EEPROM:MifNFC (Type A initiator) */
        command[21] = 0x19;
        command[22] = 0xa0; /* a01ah = EEPROM:TxBitPhase (Type A initiator) */
        command[23] = 0x1a;
        rc = rcs956_execute_command(rcs956, command, 24,
                                    13, response, &response_len,
                                    RCS956_TYPEB_READ_REGISTER_TIMEOUT);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
            if (rc == ICS_ERROR_BUF_OVERFLOW) {
                rc = ICS_ERROR_INVALID_RESPONSE;
            }
            return rc;
        }
        if ((response_len != 13) ||
            (response[0] != RCS956_RESPONSE_CODE) ||
            (response[1] != RCS956_RES_READ_REGISTER)) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "ReadRegister");
            return rc;
        }

        /* write RF parameters for Type A/B initiator mode to RAM */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_RF_CONFIGURATION;
        command[2] = RCS956_RFCONFIG_TYPEA_RF_PARAM;
        utl_memcpy(command + 3, response + 2, 11);
        rc = rcs956_execute_command(rcs956, command, 14,
                                    2, response, &response_len,
                                    RCS956_TYPEB_RF_CONFIGURATION_TIMEOUT);
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

        /* read RF parameters for Type B initiator mode from EEPROM */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_READ_REGISTER;
#ifdef CONFIG_RCS956_TYPEB_STRONG_RF_PARAM
        command[2] = 0xa0; /* a0cfh = EEPROM:GsNOn (Type B initiator) */
        command[3] = 0xcf;
        command[4] = 0xa0; /* a0d0h = EEPROM:ModGsP (Type B initiator) */
        command[5] = 0xd0;
        command[6] = 0xa0; /* a0d1h = EEPROM:RxThres (Type B initiator) */
        command[7] = 0xd1;
#else /* CONFIG_RCS956_TYPEB_STRONG_RF_PARAM */
        command[2] = 0xa0; /* a023h = EEPROM:GsNOn (Type B initiator) */
        command[3] = 0x23;
        command[4] = 0xa0; /* a024h = EEPROM:ModGsP (Type B initiator) */
        command[5] = 0x24;
        command[6] = 0xa0; /* a025h = EEPROM:RxThres (Type B initiator) */
        command[7] = 0x25;
#endif /* CONFIG_RCS956_TYPEB_STRONG_RF_PARAM */
        rc = rcs956_execute_command(rcs956, command, 8,
                                    5, response, &response_len,
                                    RCS956_TYPEB_READ_REGISTER_TIMEOUT);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
            if (rc == ICS_ERROR_BUF_OVERFLOW) {
                rc = ICS_ERROR_INVALID_RESPONSE;
            }
            return rc;
        }
        if ((response_len != 5) ||
            (response[0] != RCS956_RESPONSE_CODE) ||
            (response[1] != RCS956_RES_READ_REGISTER)) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "ReadRegister");
            return rc;
        }

        /* write RF parameters for Type B initiator mode to RAM */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_RF_CONFIGURATION;
        command[2] = RCS956_RFCONFIG_TYPEB_RF_PARAM;
        utl_memcpy(command + 3, response + 2, 3);
        rc = rcs956_execute_command(rcs956, command, 6,
                                    2, response, &response_len,
                                    RCS956_TYPEB_RF_CONFIGURATION_TIMEOUT);
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

        RCS956_SET_LAST_MODE(rcs956, RCS956_MODE_TYPEB_INITIATOR);
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

#ifndef CONFIG_RCS956_TYPEB_NOT_SET_RXNOERR
/**
 * This function sets the RxNoErr of RC-S956.
 *
 * \param  rcs956                 [IN] RC-S956 device.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 */
static UINT32 rcs956_typeb_set_rxnoerr(
    ICS_HW_DEVICE* rcs956)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typeb_set_rxnoerr"
    UINT32 rc;
    UINT8 command[5];
    UINT8 response[3];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* setr the RxNoErr bit */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_WRITE_REGISTER;
    command[2] = 0x63; /* 6303h = CIU_RxMode register */
    command[3] = 0x03;
    command[4] = 0x8b; /* set the RxNoErr bit */
    rc = rcs956_execute_command(rcs956, command, 5,
                                3, response, &response_len,
                                RCS956_TYPEB_WRITE_REGISTER_TIMEOUT);
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

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}
#endif /* !CONFIG_RCS956_TYPEB_NOT_SET_RXNOERR */
