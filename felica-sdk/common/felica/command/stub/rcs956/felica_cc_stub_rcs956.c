/**
 * \brief    The stub functions binding felica_cc to rcs956_uart/usb.
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#undef ICSLOG_MODULE
#define ICSLOG_MODULE "fcg"

#include "ics_types.h"
#include "ics_error.h"
#include "icslib_chk.h"
#include "icslog.h"
#include "utl.h"
#include "rcs956.h"
#include "felica_cc.h"
#include "stub/felica_cc_stub_rcs956.h"

#define RCS956_MAX_POLLING_CARDS        1

/* --------------------------------
 * Constant
 * -------------------------------- */

/* additional time to the timeout parameter for felica_command (ms) */
#define FELICA_CC_STUB_RCS956_ADD_TIMEOUT                 10

#define FELICA_CC_STUB_RCS956_MIN_SPEED                   9600

/* InListPassiveTarget Timeout */
#define FELICA_CC_STUB_RCS956_MIN_POLLING_TIMEOUT         200

/* WriteRegister Timeout */
#define FELICA_CC_STUB_RCS956_WRITE_REGISTER_TIMEOUT      200

/* ReadRegister Timeout */
#define FELICA_CC_STUB_RCS956_READ_REGISTER_TIMEOUT       200

/* RFConfiguration Timeout */
#define FELICA_CC_STUB_RCS956_RF_CONFIGURATION_TIMEOUT    200

/* --------------------------------
 * Prototype Declaration
 * -------------------------------- */

static UINT32 felica_cc_stub_rcs956_polling(
    void* dev,
    const UINT8 polling_param[4],
    UINT32 max_num_of_cards,
    UINT32* num_of_cards,
    felica_card_t* cards,
    felica_card_option_t* card_options,
    UINT32 timeout);
static UINT32 felica_cc_stub_rcs956_thru(
    void* dev,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 timeout);

static UINT32 felica_cc_stub_rcs956_setup_initiator(
    ICS_HW_DEVICE* rcs956);

/* --------------------------------
 * Macro
 * -------------------------------- */

/* --------------------------------
 * Function
 * -------------------------------- */

/* ------------------------
 * Exported
 * ------------------------ */

/**
 * This function initializes the device structure for felica_cc.
 *
 * \param  devf                  [OUT] The device structure for felica_cc.
 * \param  rcs956_dev             [IN] The device structure for rcs956.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid argument.
 */
UINT32 felica_cc_stub_rcs956_initialize(
    felica_cc_devf_t* devf,
    ICS_HW_DEVICE* rcs956_dev)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "felica_cc_stub_rcs956_initialize"
    ICSLOG_FUNC_BEGIN;

    /* check the parameters */
    ICSLIB_CHKARG_NE(devf, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(rcs956_dev, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_PTR(devf);
    ICSLOG_DBG_PTR(rcs956_dev);

    /* initialize the members */
    devf->dev = rcs956_dev;
    devf->polling_func = felica_cc_stub_rcs956_polling;
    devf->thru_func = felica_cc_stub_rcs956_thru;

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/* ------------------------
 * Internal
 * ------------------------ */

/**
 * This function detects remote cards using Polling command.
 *
 * \param  dev                    [IN] My ICS device.
 * \param  polling_param          [IN] Polling command except for command code.
 * \param  max_num_of_cards       [IN] The maximum number of cards to detect.
 * \param  num_of_cards          [OUT] The number of detected cards.
 * \param  cards                 [OUT] Detected cards.
 * \param  card_options          [OUT] Options of the detected cards.
 * \param  timeout                [IN] Time-out period.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 * \retval ICS_ERROR_BUF_OVERFLOW      The number of cards exceeded the limit.
 */
static UINT32 felica_cc_stub_rcs956_polling(
    void* dev,
    const UINT8 polling_param[4],
    UINT32 max_num_of_cards,
    UINT32* num_of_cards,
    felica_card_t* cards,
    felica_card_option_t* card_options,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "felica_cc_stub_rcs956_polling"
    UINT32 rc;
    UINT8 rcs956_command[9];
    UINT8 rcs956_response[3 + 21 + 21];
    UINT32 rcs956_response_len;
    UINT i;
    UINT p;
    UINT32 response_timeout;
    ICSLOG_FUNC_BEGIN;

    /* check the parameters */
    ICSLIB_CHKARG_BE(max_num_of_cards, 1, ICS_ERROR_INVALID_PARAM);

    if (max_num_of_cards > RCS956_MAX_POLLING_CARDS) {
        max_num_of_cards = RCS956_MAX_POLLING_CARDS;
    }

    /* set up RC-S956 for initiator mode */
    rc = felica_cc_stub_rcs956_setup_initiator(dev);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "felica_cc_stub_rcs956_setup_initiator()");
        return rc;
    }

    /* make an InListPassiveTarget command for RC-S956 */
    rcs956_command[0] = 0xd4;
    rcs956_command[1] = 0x4a; /* InListPassiveTarget */
    rcs956_command[2] = max_num_of_cards;
    rcs956_command[3] = 0x01;
    rcs956_command[4] = 0x00;
    utl_memcpy(rcs956_command + 5, polling_param, 4);

    if (timeout < FELICA_CC_STUB_RCS956_MIN_POLLING_TIMEOUT) {
        response_timeout = FELICA_CC_STUB_RCS956_MIN_POLLING_TIMEOUT;
    } else {
        response_timeout = timeout;
    }

    /* send the command to RC-S956 */
    rc = rcs956_execute_command(dev, rcs956_command,
                                sizeof(rcs956_command),
                                sizeof(rcs956_response),
                                rcs956_response,
                                &rcs956_response_len,
                                response_timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
        if (rc == ICS_ERROR_BUF_OVERFLOW) {
            rc = ICS_ERROR_INVALID_RESPONSE;
        } else if ((rc == ICS_ERROR_TIMEOUT) ||
                   (rc == ICS_ERROR_INVALID_RESPONSE)) {
            UINT32 rc2;

            rc2 = rcs956_cancel_command(dev);
            if (rc2 != ICS_ERROR_SUCCESS) {
                ICSLOG_ERR_STR(rc2, "rcs956_cancel_command()");
                /* Note: ignore error */
            }
        }
        return rc;
    }
    if ((rcs956_response_len < 3) ||
        (rcs956_response[0] != 0xd5) ||
        (rcs956_response[1] != 0x4b) ||
        (rcs956_response[2] > max_num_of_cards)) {
        rc = ICS_ERROR_INVALID_RESPONSE;
        ICSLOG_ERR_STR(rc, "Invalid response.");
        return rc;
    }
    if (rcs956_response[2] == 0) {
        rc = ICS_ERROR_TIMEOUT;
        ICSLOG_ERR_STR(rc, "Time-out.");
        return rc;
    }

    /* parse the response */
    *num_of_cards = rcs956_response[2];
    p = 3;
    for (i = 0; i < *num_of_cards; i++) {
        if (((p + 3) > rcs956_response_len) ||
            (rcs956_response[p] != (i + 1)) ||
            ((rcs956_response[p + 1] != 18) &&
             (rcs956_response[p + 1] != 20)) ||
            ((p + 1 + rcs956_response[p + 1]) > rcs956_response_len) ||
            (rcs956_response[p + 2] != 0x01)) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "Invalid response.");
            return rc;
        }
        utl_memcpy(cards[i].idm, rcs956_response + p + 3, 8);
        utl_memcpy(cards[i].pmm, rcs956_response + p + 11, 8);
        if ((rcs956_response[p + 1] == 20) && card_options != NULL) {
            card_options[i].option_len = 2;
            utl_memcpy(card_options[i].option, rcs956_response + p + 19, 2);
        } else if (card_options != NULL) {
            card_options[i].option_len = 0;
        }
        p += (1 + rcs956_response[p + 1]);
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sends the FeliCa card command and receives a response.
 *
 * \param  dev                    [IN] My ICS device.
 * \param  command                [IN] The card command to send.
 * \param  command_len            [IN] The length of the card command.
 * \param  max_response_len       [IN] The maximum length of response.
 * \param  response              [OUT] Received response.
 * \param  response_len          [OUT] The length of the response.
 * \param  timeout                [IN] Time-out. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_DEVICE            Error at device.
 * \retval ICS_ERROR_FRAME_CRC         CRC error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 * \retval ICS_ERROR_BUF_OVERFLOW      The length of the received response
 *                                     exceeded max_response_len.
 */
static UINT32 felica_cc_stub_rcs956_thru(
    void* dev,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "felica_cc_stub_rcs956_thru"
    UINT32 rc;
    ICS_HW_DEVICE* rcs956;
    UINT32 speed;
    UINT32 nbits;
    UINT32 add_time;
    UINT32 driver_timeout;
    ICSLOG_FUNC_BEGIN;

    /* check the parameter */
    ICSLIB_CHKARG_LE(command_len,
                     FELICA_CC_STUB_RCS956_MAX_COMMAND_LEN,
                     ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(dev, NULL, ICS_ERROR_INVALID_PARAM);

    /* calculate additional time */
    rcs956 = dev;
    speed = RCS956_SPEED(rcs956);
    if (speed < FELICA_CC_STUB_RCS956_MIN_SPEED) {
        speed = FELICA_CC_STUB_RCS956_MIN_SPEED;
    }

    /*
     * extra timeout period between the controller and RC-S956
     *
     * 15 + command_len:command packet size
     * 6:ack packet size
     * 14 + MAX_RESPONSE_LEN:response packet size(max)
     * 10:1 byte = 10 bits
     */
    nbits = (((15 + command_len) + 6 +
              (14 + FELICA_CC_STUB_RCS956_MAX_RESPONSE_LEN)) * 10);
    add_time = (((nbits * 1000) + (speed - 1)) / speed);

    /*
     * extra timeout period for RF communication between RC-S956 and the card,
     * calculated by assuming the worst case (Largest command and response)
     *
     * Command len  = Preamble + Sync Code + LEN + MAX_COMMAND_LEN + CRC
     *              = 6 + 2 + 1 + MAX_COMMAND_LEN + 2
     *              = MAX_COMMAND_LEN + 11
     * Response len (without preamble and sync code because they are already
     *               included in the timeout)
     *              = LEN + MAX_RESPONSE_LEN + CRC
     *              = 1 + MAX_RESPONSE_LEN + 2
     *              = MAX_RESPONSE_LEN + 3
     * RF Speed     = 212kbps by assumption
     * Bits per byte = 8
     */
    add_time +=
        (((FELICA_CC_STUB_RCS956_MAX_COMMAND_LEN + 11 +
           FELICA_CC_STUB_RCS956_MAX_RESPONSE_LEN + 3) * 8 * 1000) / 211875);

    /* extra timeout for period in the controller and RC-S956 */
    add_time += FELICA_CC_STUB_RCS956_ADD_TIMEOUT;

    if ((0xffffffff - add_time) >= timeout) {
        driver_timeout = (timeout + add_time);
    } else {
        driver_timeout = 0xffffffff;
    }

    /* transceive the command */
    rc = rcs956_felica_command(dev,
                               command, command_len, max_response_len,
                               response, response_len,
                               timeout, driver_timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        ICSLOG_ERR_STR(rc, "rcs956_felica_command()");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sets up RC-S956 for initiator mode.
 *
 * \param  rcs956                 [IN] RC-S956 device.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 * \retval ICS_ERROR_INVALID_RESPONSE  Received an invalid response packet.
 */
static UINT32 felica_cc_stub_rcs956_setup_initiator(
    ICS_HW_DEVICE* rcs956)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "felica_cc_stub_rcs956_setup_initiator"
    UINT32 rc;
    UINT8 rcs956_command[18];
    UINT8 rcs956_response[10];
    UINT32 rcs956_response_len;
    ICSLOG_FUNC_BEGIN;

    if (RCS956_LAST_MODE(rcs956) != RCS956_MODE_FELICA_INITIATOR) {
        /* clear the LargeBWPLL bit */
        rcs956_command[0] = 0xd4;
        rcs956_command[1] = 0x08; /* WriteRegister */
        rcs956_command[2] = 0x63; /* 630dh = CIU_ManualRCV register */
        rcs956_command[3] = 0x0d;
        rcs956_command[4] = 0x00; /* clear the LargetBWPLL bit */
        rc = rcs956_execute_command(
            rcs956, rcs956_command, 5,
            3, rcs956_response, &rcs956_response_len,
            FELICA_CC_STUB_RCS956_WRITE_REGISTER_TIMEOUT);
        if (rc != ICS_ERROR_SUCCESS) {
            ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
            if (rc == ICS_ERROR_BUF_OVERFLOW) {
                rc = ICS_ERROR_INVALID_RESPONSE;
            }
            return rc;
        }
        if ((rcs956_response_len != 3) ||
            (rcs956_response[0] != 0xd5) ||
            (rcs956_response[1] != 0x09) ||
            (rcs956_response[2] != 0x00)) {
            rc = ICS_ERROR_INVALID_RESPONSE;
            ICSLOG_ERR_STR(rc, "WriteRegister");
            return rc;
        }

        if (RCS956_NEED_CHANGE_FELICA_RF_PARAM(rcs956)) {
            /* read RF parameters for FeliCa initiator mode from EEPROM */
            rcs956_command[0] = 0xd4;
            rcs956_command[1] = 0x06; /* ReadRegister */
            rcs956_command[2] = 0xa0; /* a01bh = EEPROM:RFCfg (initiator) */
            rcs956_command[3] = 0x1b;
            rcs956_command[4] = 0xa0; /* a01ch = EEPROM:GsNOn (initiator) */
            rcs956_command[5] = 0x1c;
            rcs956_command[6] = 0xa0; /* a01dh = EEPROM:CWGsP (initiator) */
            rcs956_command[7] = 0x1d;
            rcs956_command[8] = 0xa0; /* a01eh = EEPROM:ModGsP (initiator) */
            rcs956_command[9] = 0x1e;
            rcs956_command[10] = 0xa0; /* a01fh = EEPROM:Demod RfOn */
            rcs956_command[11] = 0x1f;
            rcs956_command[12] = 0xa0; /* a020h = EEPROM:RxThres (initiator) */
            rcs956_command[13] = 0x20;
            rcs956_command[14] = 0xa0; /* a021h = EEPROM:Demod RfOff */
            rcs956_command[15] = 0x21;
            rcs956_command[16] = 0xa0; /* a022h = EEPROM:GsNOff (initiator) */
            rcs956_command[17] = 0x22;
            rc = rcs956_execute_command(
                rcs956, rcs956_command, 18,
                10, rcs956_response, &rcs956_response_len,
                FELICA_CC_STUB_RCS956_READ_REGISTER_TIMEOUT);
            if (rc != ICS_ERROR_SUCCESS) {
                ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
                if (rc == ICS_ERROR_BUF_OVERFLOW) {
                    rc = ICS_ERROR_INVALID_RESPONSE;
                }
                return rc;
            }
            if ((rcs956_response_len != 10) ||
                (rcs956_response[0] != 0xd5) ||
                (rcs956_response[1] != 0x07)) {
                rc = ICS_ERROR_INVALID_RESPONSE;
                ICSLOG_ERR_STR(rc, "ReadRegister");
                return rc;
            }

            /* write RF parameters for FeliCa initiator mode to RAM */
            rcs956_command[0] = 0xd4;
            rcs956_command[1] = 0x32; /* RFConfiguration */
            rcs956_command[2] = 0x0b; /* Set FeliCa RF param */
            utl_memcpy(rcs956_command + 3, rcs956_response + 2, 8);
            rc = rcs956_execute_command(
                rcs956, rcs956_command, 11,
                2, rcs956_response, &rcs956_response_len,
                FELICA_CC_STUB_RCS956_RF_CONFIGURATION_TIMEOUT);
            if (rc != ICS_ERROR_SUCCESS) {
                ICSLOG_ERR_STR(rc, "rcs956_execute_command()");
                if (rc == ICS_ERROR_BUF_OVERFLOW) {
                    rc = ICS_ERROR_INVALID_RESPONSE;
                }
                return rc;
            }
            if ((rcs956_response_len != 2) ||
                (rcs956_response[0] != 0xd5) ||
                (rcs956_response[1] != 0x33)) {
                rc = ICS_ERROR_INVALID_RESPONSE;
                ICSLOG_ERR_STR(rc, "ReadRegister");
                return rc;
            }
        }

        RCS956_SET_LAST_MODE(rcs956, RCS956_MODE_FELICA_INITIATOR);
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}
