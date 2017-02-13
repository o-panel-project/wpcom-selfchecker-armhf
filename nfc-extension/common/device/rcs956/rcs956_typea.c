/**
 * \brief    RC-S956 Driver (ISO/IEC 14443-3 Type A)
 * \date     2009/05/25
 * \author   Copyright 2005,2006,2007,2008,2009 Sony Corporation
 */

#undef ICSLOG_MODULE
#define ICSLOG_MODULE "DGA"

#include "ics_types.h"
#include "ics_error.h"
#include "icslib_chk.h"
#include "icslog.h"
#include "utl.h"

#include "rcs956.h"
#include "rcs956_typea.h"

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

#define RCS956_RFCONFIG_MAXRETRY                0x05
#define RCS956_RFCONFIG_TYPEA_RF_PARAM          0x0a

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
#define RCS956_TYPEA_RESET_TIMEOUT              200 /* ms */

/* WriteRegister Timeout */
#define RCS956_TYPEA_WRITE_REGISTER_TIMEOUT     200 /* ms */

/* ReadRegister Timeout */
#define RCS956_TYPEA_READ_REGISTER_TIMEOUT      200 /* ms */

/* RFConfiguration Timeout */
#define RCS956_TYPEA_RF_CONFIGURATION_TIMEOUT   200 /* ms */

/* SetParameter Timeout */
#define RCS956_TYPEA_SET_PARAMETERS_TIMEOUT     200 /* ms */

/* --------------------------------
 * Prototype Declaration
 * -------------------------------- */

static UINT32 rcs956_typea_setup_initiator(
    ICS_HW_DEVICE* rcs956);
static UINT32 rcs956_typea_reset_rf_retry(
    ICS_HW_DEVICE* rcs956);

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
 * This function detects a Type A card.
 *
 * \param  rcs956                 [IN] The handle to access the port.
 * \param  req_uid                [IN] UID of the card to detect. (or NULL)
 * \param  req_uid_len            [IN] The length of the req_uid.
 * \param  card                  [OUT] Card information.
 * \param  ats                   [OUT] Received ATS. (or NULL)
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Invalid response.
 * \retval ICS_ERROR_BUF_OVERFLOW      The length of ATS exceeded the limit.
 */
UINT32 rcs956_typea_polling(
    ICS_HW_DEVICE* rcs956,
    const UINT8* req_uid,
    UINT32 req_uid_len,
    typea_card_t* card,
    iso14443_4_ats_t* ats,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typea_polling"
    UINT32 rc;
    UINT8 command[16];
    UINT8 response[18 + 64];
    UINT32 command_len;
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);
    if (req_uid_len > 0) {
        ICSLIB_CHKARG_IN_RANGE(req_uid_len, 4, 10, ICS_ERROR_INVALID_PARAM);
        ICSLIB_CHKARG_EQ(req_uid_len - ((req_uid_len / 3) * 3), 1,
                         ICS_ERROR_INVALID_PARAM);
        ICSLIB_CHKARG_NE(req_uid, NULL, ICS_ERROR_INVALID_PARAM);
    }
    ICSLIB_CHKARG_NE(card, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    if (req_uid_len > 0) {
        ICSLOG_DBG_UINT(req_uid_len);
        ICSLOG_DUMP(req_uid, req_uid_len);
    }
    ICSLOG_DBG_PTR(card);
    ICSLOG_DBG_PTR(ats);
    ICSLOG_DBG_UINT(timeout);

    /* reset the mode of the device */
    rc = rcs956_reset(rcs956, RCS956_TYPEA_RESET_TIMEOUT);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_reset()");
        return rc;
    }

    /* set up the device for Type A initiator mode */
    rc = rcs956_typea_setup_initiator(rcs956);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_typea_setup_initiator()");
        return rc;
    }

    /* SetParameters */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_SET_PARAMETERS;
    command[2] = RCS956_SET_PARAMETERS_DEFAULT;
    if (ats != NULL) {
        command[2] |= 0x10; /* fAutomaticRATS = 1 */
    } else {
        command[2] &= ~0x10; /* fAutomaticRATS = 0 */
    }
    rc = rcs956_execute_command(rcs956,
                                command, 3,
                                2, response, &response_len,
                                RCS956_TYPEA_SET_PARAMETERS_TIMEOUT);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        }
        return rc;
    }
    if ((response_len != 2) ||
        (response[0] != RCS956_RESPONSE_CODE) ||
        (response[1] != RCS956_RES_SET_PARAMETERS)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "SetParameters");
        return rc;
    }

    /* InListPassiveTarget */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_IN_LIST_PASSIVE_TARGET;
    command[2] = 0x01; /* MaxTg = 1 */
    command[3] = 0x00; /* Type A */
    command_len = 4;
    if (req_uid_len == 4) {
        utl_memcpy(command + 4, req_uid, 4);
        command_len += 4;
    } else if (req_uid_len == 7) {
        command[4] = 0x88; /* cascade tag */
        utl_memcpy(command + 5, req_uid, 7);
        command_len += 8;
    } else if (req_uid_len == 10) {
        command[4] = 0x88; /* cascade tag */
        utl_memcpy(command + 5, req_uid, 3);
        command[8] = 0x88; /* cascade tag */
        utl_memcpy(command + 9, req_uid + 3, 7);
        command_len += 12;
    }
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
    if ((response_len < (3 + 5)) ||
        (response[3] != 1) ||
        (response_len < (UINT32)(3 + 5 + response[7]))) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }
    utl_memcpy(card->atqa, response + 4, 2);
    card->sak = response[6];
    card->uid_len = response[7];
    utl_memcpy(card->uid, response + 8, card->uid_len);
    ICSLOG_DUMP(card->atqa, 2);
    ICSLOG_DBG_HEX8(card->sak);
    ICSLOG_DUMP(card->uid, card->uid_len);

    card->ats = ats;
    if (ats != NULL) {
        ats->len = (response_len - 8 - card->uid_len);
        ICSLOG_DBG_UINT(ats->len);
        if (ats->len == 0) {
            card->ats = NULL;
        }
        if (ats->len > ISO14443_4_CARD_MAX_ATS_LEN) {
            /* Note: never reach here */
            utl_memcpy(ats->data, response + 8 + card->uid_len,
                       ISO14443_4_CARD_MAX_ATS_LEN);
            ICSLOG_DUMP(ats->data, ISO14443_4_CARD_MAX_ATS_LEN);
            rc = ICS_ERROR_BUF_OVERFLOW;
            ICSLOG_ERR_STR(rc, "Buffer overflow.");
            return rc;
        }

        utl_memcpy(ats->data, response + 8 + card->uid_len, ats->len);
        ICSLOG_DUMP(ats->data, ats->len);
    } else if (response_len != (8 + card->uid_len)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sends the Type A command and receives a Type A response.
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
UINT32 rcs956_typea_command(
    ICS_HW_DEVICE* rcs956,
    typea_card_t* card,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 command_timeout,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typea_command"
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
    ICSLIB_CHKARG_LE(command_len, RCS956_MAX_TYPEA_COMMAND_LEN,
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
UINT32 rcs956_typea_deselect(
    ICS_HW_DEVICE* rcs956,
    typea_card_t* card,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typea_deselect"
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
 * \param  card                   [IN] The card to select. (ignored)
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
UINT32 rcs956_typea_select(
    ICS_HW_DEVICE* rcs956,
    typea_card_t* card,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typea_select"
    UINT32 rc;
    UINT8 command[3];
    UINT8 response[3];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_PTR(card);
    ICSLOG_DBG_UINT(timeout);

    /* InSelect command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_IN_SELECT;
    command[2] = 0x01; /* target num */
    rc = rcs956_execute_command(rcs956,
                                command, sizeof(command),
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
        (response[1] != RCS956_RES_IN_SELECT)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }
    if (response[2] != RCS956_STATUS_SUCCESS) {
        rc = rcs956_typea_reset_rf_retry(rcs956);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "rcs956_typea_reset_rf_retry()");
            return rc;
        }
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

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function releases the card.
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
UINT32 rcs956_typea_release(
    ICS_HW_DEVICE* rcs956,
    typea_card_t* card,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typea_release"
    UINT32 rc;
    UINT8 command[5];
    UINT8 response[4];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_NE(rcs956, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(RCS956_RAW_FUNC(rcs956), NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(rcs956);
    ICSLOG_DBG_PTR(card);
    ICSLOG_DBG_UINT(timeout);

    /* InRelease command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_IN_RELEASE;
    command[2] = 0x01; /* num of targets */
    command[3] = 0x01; /* target num */
    rc = rcs956_execute_command(rcs956,
                                command, 4,
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
        (response[1] != RCS956_RES_IN_RELEASE) ||
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

    /* WriteRegister command */
    command[0] = RCS956_COMMAND_CODE;
    command[1] = RCS956_CMD_WRITE_REGISTER;
    command[2] = 0xff;
    command[3] = 0xe9;
    command[4] = 0x00;
    rc = rcs956_execute_command(rcs956,
                                command, 5,
                                3, response, &response_len,
                                RCS956_TYPEA_WRITE_REGISTER_TIMEOUT);
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

/**
 * This function sets up RC-S956 for Type A initiator mode.
 *
 * \param  rcs956                 [IN] RC-S956 device.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 */
static UINT32 rcs956_typea_setup_initiator(
    ICS_HW_DEVICE* rcs956)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typea_setup_initiator"
    UINT32 rc;
    UINT8 command[24];
    UINT8 response[13];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

    if (RCS956_LAST_MODE(rcs956) != RCS956_MODE_TYPEA_INITIATOR) {
        /* clear the LargeBWPLL bit */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_WRITE_REGISTER;
        command[2] = 0x63; /* 630dh = CIU_ManualRCV register */
        command[3] = 0x0d;
        command[4] = 0x00; /* clear the LargetBWPLL bit */
        rc = rcs956_execute_command(rcs956, command, 5,
                                    3, response, &response_len,
                                    RCS956_TYPEA_WRITE_REGISTER_TIMEOUT);
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

        /* read RF parameters for Type A initiator mode from EEPROM */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_READ_REGISTER;
        command[2] = 0xa0; /* a010h = EEPROM:RFCfg (initiator) */
        command[3] = 0x10;
        command[4] = 0xa0; /* a011h = EEPROM:GsNOn (initiator) */
        command[5] = 0x11;
        command[6] = 0xa0; /* a012h = EEPROM:CWGsP (initiator) */
        command[7] = 0x12;
        command[8] = 0xa0; /* a013h = EEPROM:ModGsP (initiator) */
        command[9] = 0x13;
        command[10] = 0xa0; /* a014h = EEPROM:Demod RfOn */
        command[11] = 0x14;
        command[12] = 0xa0; /* a015h = EEPROM:RxThres (initiator) */
        command[13] = 0x15;
        command[14] = 0xa0; /* a016h = EEPROM:Demod RfOff */
        command[15] = 0x16;
        command[16] = 0xa0; /* a017h = EEPROM:GsNOff (initiator) */
        command[17] = 0x17;
        command[18] = 0xa0; /* a018h = EEPROM:ModWidth (initiator) */
        command[19] = 0x18;
        command[20] = 0xa0; /* a019h = EEPROM:MifNFC (initiator) */
        command[21] = 0x19;
        command[22] = 0xa0; /* a01ah = EEPROM:TxBitPhase (initiator) */
        command[23] = 0x1a;
        rc = rcs956_execute_command(rcs956, command, 24,
                                    13, response, &response_len,
                                    RCS956_TYPEA_READ_REGISTER_TIMEOUT);
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

        /* write RF parameters for Type A initiator mode to RAM */
        command[0] = RCS956_COMMAND_CODE;
        command[1] = RCS956_CMD_RF_CONFIGURATION;
        command[2] = RCS956_RFCONFIG_TYPEA_RF_PARAM;
        utl_memcpy(command + 3, response + 2, 11);
        rc = rcs956_execute_command(rcs956, command, 14,
                                    2, response, &response_len,
                                    RCS956_TYPEA_RF_CONFIGURATION_TIMEOUT);
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

        RCS956_SET_LAST_MODE(rcs956, RCS956_MODE_TYPEA_INITIATOR);
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function resets the maximum retries of RF packets.
 *
 * \param  rcs956                 [IN] RC-S956 device.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 */
static UINT32 rcs956_typea_reset_rf_retry(
    ICS_HW_DEVICE* rcs956)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_typea_reset_rf_retry"
    UINT32 rc;
    UINT8 command[6];
    UINT8 response[2];
    UINT32 response_len;
    ICSLOG_FUNC_BEGIN;

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
                                RCS956_TYPEA_RF_CONFIGURATION_TIMEOUT);
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

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}
