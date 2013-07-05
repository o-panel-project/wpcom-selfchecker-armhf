/**
 * \brief    Self-Diagnosing Sample Library for RC-S956
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#include "rcs956.h"
#include "ics_types.h"
#include "ics_error.h"
#include "diag_rcs956.h"
#include <string.h>

#define COMMUNICATE_LINE_TEST_PARAM_SIZE  262

/* The instance is defined in diag_rcs956_uart.c or diag_rcs956_usb.c. */
extern const icsdrv_basic_func_t* g_drv_func;

/**
 * This function opens and initializes the device which is specified by
 * \a port name in order to verifiy that the controller is able to recognize
 * the device.
 *
 * \param  dev                   [OUT] The handle to access the port.
 * \param  port_name              [IN] Port name.
 * \param  timeout               [OUT] Time-out period for initialization. (ms)
 *
 * \retval DIAG_SUCCESS                Success.
 * \retval DIAG_ERR_DISCONN            The device is disconnected.
 */
int diag_rcs956_init_device(
    ICS_HW_DEVICE* dev,
    const char* port_name,
    UINT32 timeout)
{
    UINT32 rc;

    /* Open the device */
    rc = g_drv_func->open(dev, port_name);
    if (rc != ICS_ERROR_SUCCESS) {
        return DIAG_ERR_DISCONN;
    }

    /* Initialize the device */
    rc = g_drv_func->initialize_device(dev, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        return DIAG_ERR_DISCONN;
    }

    return DIAG_SUCCESS;
}

/**
 * This function gets the firmware version of the device in order to verify
 * the version is supported.
 * The supported versions are specified by \a versions array.  The number of
 * elements in the array is specifeid by \a num_ver.  The firmware version
 * which is obtained by the device is compared with all values in \a versions
 * array.  If either of them is matched, this function returns DIAG_SUCCESS.
 *
 * \param  dev                    [IN] The handle to access the port.
 * \param  versions               [IN] The supported versions.
 * \param  num_ver                [IN] The number of supported versions.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval DIAG_SUCCESS                Success.
 * \retval DIAG_ERR_BAD_COMM           The communication between the controller
 *                                     and the device is malfunctioning.
 * \retval DIAG_ERR_UNSUPPORTED_RW     Unsupported version.
 */
int diag_rcs956_check_firmware_version(
    ICS_HW_DEVICE* dev,
    const UINT16* versions,
    UINT32 num_ver,
    UINT32 timeout)
{
    UINT32 rc;
    UINT8 ic_type = 0;
    UINT16 version = 0;
    BOOL is_supported = FALSE;
    UINT32 i;

    /* Send GetFirmwareVersion Command */
    rc = rcs956_get_firmware_version(dev, &ic_type, &version, timeout);
    if (rc != ICS_ERROR_SUCCESS) {
        return DIAG_ERR_BAD_COMM;
    }

    /* Check if the obtained version is supported */
    for (i = 0; i < num_ver; i++) {
        if (version == versions[i]) {
            is_supported = TRUE;
            break;
        }
    }

    if (!is_supported) {
        return DIAG_ERR_UNSUPPORTED_RW;
    }

    return DIAG_SUCCESS;
}

/**
 * This function performs Communicate Line Test in order to verify that the
 * communication between the controller and the device is functioning
 * appropriately.
 * In Communicate Line Test, the controller sends 262 bytes to the device and
 * expects the identical data to be sent by the device.  Normal communication
 * can be verified by comparing the sent and received data.
 *
 * \param  dev                    [IN] The handle to access the port.
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval DIAG_SUCCESS                Success.
 * \retval DIAG_ERR_BAD_COMM           The communication between the controller
 *                                     and the device is malfunctioning.
 */
int diag_rcs956_communication_line_test(
    ICS_HW_DEVICE* dev,
    UINT32 timeout)
{
    /* Diagnose Command, Communication Line Test */
    UINT8 command[3 + COMMUNICATE_LINE_TEST_PARAM_SIZE] = {0xd4, 0x00, 0x00};
    UINT8 response[2 + COMMUNICATE_LINE_TEST_PARAM_SIZE];
    UINT32 response_len = 0;
    UINT32 rc;
    UINT i;

    /* Initialize InParam */
    for (i = 0; i < COMMUNICATE_LINE_TEST_PARAM_SIZE; i++) {
        command[3 + i] = (UINT8)(i & 0xffU);
    }

    /* Clean the response buffer */
    memset(response, 0, sizeof(response));

    /* Send Diagnose Command */
    rc = g_drv_func->execute_command(dev, command, sizeof(command),
                                     sizeof(response), response, &response_len,
                                     timeout);
    if ((rc != ICS_ERROR_SUCCESS) ||
        (response_len != (2 + COMMUNICATE_LINE_TEST_PARAM_SIZE)) ||
        (response[0] != 0xd5) ||
        (response[1] != 0x01)) {
        return DIAG_ERR_BAD_COMM;
    }

    /* Compare InParam and OutParam */
    if (memcmp(&command[3], &response[2],
               COMMUNICATE_LINE_TEST_PARAM_SIZE) != 0) {
        return DIAG_ERR_BAD_COMM;
    }

    return DIAG_SUCCESS;
}

/**
 * This function performs ROM Test in order to verify that the ROM is free
 * from errors.
 * In ROM Test, the device checks its ROM by calculating the check-sum.
 * If the test passes, the device sends the result back to the controller.
 *
 * \param  dev                   [IN] The handle to access the port.
 * \param  timeout               [IN] Time-out period. (ms)
 *
 * \retval DIAG_SUCCESS               Success.
 * \retval DIAG_ERR_BAD_COMM          The communication between the controller
 *                                    and the device is malfunctioning
 * \retval DIAG_ERR_BAD_RW            The R/W is malfunctioning.
 */
int diag_rcs956_rom_test(
    ICS_HW_DEVICE* dev,
    UINT32 timeout)
{
    /* Diagnose Command, ROM Test */
    const UINT8 command[3] = {0xd4, 0x00, 0x01};
    UINT8 response[2 + 1];
    UINT32 response_len = 0;
    UINT8 result;
    UINT32 rc;

    /* Clean the response buffer */
    memset(response, 0, sizeof(response));

    /* Send Diagnose Command */
    rc = g_drv_func->execute_command(dev, command, sizeof(command),
                                     sizeof(response), response, &response_len,
                                     timeout);
    if ((rc != ICS_ERROR_SUCCESS) ||
        (response_len != (2 + 1)) ||
        (response[0] != 0xd5) ||
        (response[1] != 0x01)) {
        return DIAG_ERR_BAD_COMM;
    }

    result = response[2];
    if (result == 0x00) {
        return DIAG_SUCCESS;
    } else if (result == 0xffU) {
        return DIAG_ERR_BAD_RW;
    } else {
        return DIAG_ERR_BAD_COMM;
    }
}

/**
 * This function performs RAM Test in order to verify that the ROM is free
 * from errors.
 * In RAM Test, the device checks its RAM by calculating the check-sum.
 * If the test passes, the device sends the result back to the controller.
 *
 * \param  dev                   [IN] The handle to access the port.
 * \param  timeout               [IN] Time-out period. (ms)
 *
 * \retval DIAG_SUCCESS               Success.
 * \retval DIAG_ERR_BAD_COMM          The communication between the controller
 *                                    and the device is malfunctioning.
 * \retval DIAG_ERR_BAD_RW            The R/W is malfunctioning.
 */
int diag_rcs956_ram_test(
    ICS_HW_DEVICE* dev,
    UINT32 timeout)
{
    /* Diagnose Command, RAM Test */
    const UINT8 command[3] = {0xd4, 0x00, 0x02};
    UINT8 response[2 + 1];
    UINT32 response_len = 0;
    UINT8 result;
    UINT32 rc;

    /* Clean the response buffer */
    memset(response, 0, sizeof(response));

    /* Send Diagnose Command */
    rc = g_drv_func->execute_command(dev, command, sizeof(command),
                                     sizeof(response), response, &response_len,
                                     timeout);
    if ((rc != ICS_ERROR_SUCCESS) ||
        (response_len != (2 + 1)) ||
        (response[0] != 0xd5) ||
        (response[1] != 0x01)) {
        return DIAG_ERR_BAD_COMM;
    }

    result = response[2];
    if (result == 0x00) {
        return DIAG_SUCCESS;
    } else if (result == 0xffU) {
        return DIAG_ERR_BAD_RW;
    } else {
        return DIAG_ERR_BAD_COMM;
    }
}

/**
 * This function performs Polling Test to Target in order to verify that the
 * the device can caputure external target (card) appropriately.
 * In Polling Test to Target, the device executes 128 Polling commands, and
 * sends the number of failures back to the controller.
 * One failure out of 128 is tolerated and this function returns DIAG_SUCCESS.
 * If two or more failures occured, this function returns the DIAG_ERR_POLLING
 * error.
 *
 * \param  dev                   [IN] The handle to access the port.
 * \param  timeout               [IN] Time-out period. (ms)
 *
 * \retval DIAG_SUCCESS               Success.
 * \retval DIAG_ERR_BAD_COMM          The communication between the controller
 *                                    and the device is malfunctioning.
 * \retval DIAG_ERR_POLLING           The Polling command is malfunctioning or
 *                                    there may be some problem on the card.
 */
int diag_rcs956_polling_test_to_card(
    ICS_HW_DEVICE* dev,
    UINT32 timeout)
{
    /* Diagnose Command, Polling Test to Target (212kbps) */
    const UINT8 command[3 + 1] = {0xd4, 0x00, 0x04, 0x01};
    UINT8 response[2 + 1];
    UINT32 response_len = 0;
    UINT8 fail_cnt;
    UINT32 rc;

    /* Clean the response buffer */
    memset(response, 0, sizeof(response));

    /* Send Diagnose Command */
    rc = g_drv_func->execute_command(dev, command, sizeof(command),
                                     sizeof(response), response, &response_len,
                                     timeout);
    if ((rc != ICS_ERROR_SUCCESS) ||
        (response_len != (2 + 1)) ||
        (response[0] != 0xd5) ||
        (response[1] != 0x01)) {
        return DIAG_ERR_BAD_COMM;
    }

    fail_cnt = response[2];
    if (fail_cnt > 1) {
        return DIAG_ERR_POLLING;
    }

    return DIAG_SUCCESS;
}

/**
 * This function tests the EEPROM on the device in the following sequence.
 *
 * 1. The data in  address 0x00 == 0x55 and in 0x01 = 0xAA
 * 2. The sum of data in addresses 0x00-0x0f == 0
 * 3. The data in addresses 0x02 == 1
 * 4: The sum of data in addresses 0x10-0x3f == 0
 *
 * \param  dev                   [IN] The handle to access the port.
 * \param  timeout               [IN] Time-out period. (ms)
 *
 * \retval DIAG_SUCCESS               Success.
 * \retval DIAG_ERR_BAD_COMM          The communication between the controller
 *                                    and the device is malfunctioning.
 * \retval DIAG_ERR_BAD_RW            The R/W is malfunctioning.
 */
int diag_rcs956_eeprom_test(
    ICS_HW_DEVICE* dev,
    UINT32 timeout)
{
    /* Read Register Command, maximum 64 addresses */
    UINT8 command[2 + (2 * 64)] = {0xd4, 0x06};
    UINT8 response[2 + 64];
    UINT32 response_len = 0;
    UINT32 sum;
    UINT16 eeprom_addr;
    int idx;
    UINT32 rc;

    /*
     * Check 1: The sum of data in addresses 0x00-0x0f == 0
     */
    command[0] = 0xd4;
    command[1] = 0x06;
    eeprom_addr = 0x00;
    idx = 2;
    while (eeprom_addr <= 0x0f) {
        command[idx++] = 0xa0;
        command[idx++] = (UINT8)eeprom_addr;
        eeprom_addr++;
    }

    /* Clean the response buffer */
    memset(response, 0, sizeof(response));

    /* Send ReadRegister Command */
    rc = g_drv_func->execute_command(dev, command, idx,
                                     sizeof(response), response, &response_len,
                                     timeout);

    /* execute_command fails in case of bad communication */
    if (rc != ICS_ERROR_SUCCESS) {
        return DIAG_ERR_BAD_COMM;
    }

    /* Syntax error occurs if EEPROM is not available */
    if ((response_len == 1) &&
        (response[0] == 0x7f)) {
        return DIAG_ERR_BAD_RW;
    }

    /* invalid response */
    if ((response_len != (2 + (0x0f - 0x00 + 1))) ||
        (response[0] != 0xd5) ||
        (response[1] != 0x07)) {
        return DIAG_ERR_BAD_COMM;
    }

    /* Calculate the check-sum */
    sum = 0;
    for (idx = 2; idx < (2 + (0x0f - 0x00 + 1)); idx++) {
        sum += response[idx];
    }
    sum &= 0xffU;
    if (sum != 0) {
        return DIAG_ERR_BAD_RW;
    }

    /*
     * Check 2: The sum of data in addresses 0x10-0x3f == 0
     */
    command[0] = 0xd4;
    command[1] = 0x06;
    eeprom_addr = 0x10;
    idx = 2;
    while (eeprom_addr <= 0x3f) {
        command[idx++] = 0xa0;
        command[idx++] = (UINT8)eeprom_addr;
        eeprom_addr++;
    }

    /* Clean the response buffer */
    memset(response, 0, sizeof(response));

    /* Send ReadRegister Command */
    rc = g_drv_func->execute_command(dev, command, idx,
                                     sizeof(response), response, &response_len,
                                     timeout);

    /* execute_command fails in case of bad communication */
    if (rc != ICS_ERROR_SUCCESS) {
        return DIAG_ERR_BAD_COMM;
    }

    /* Syntax error occurs if EEPROM is not available */
    if ((response_len == 1) &&
        (response[0] == 0x7f)) {
        return DIAG_ERR_BAD_RW;
    }

    /* invalid response */
    if ((response_len != (2 + (0x3f - 0x10 + 1))) ||
        (response[0] != 0xd5) ||
        (response[1] != 0x07)) {
        return DIAG_ERR_BAD_COMM;
    }

    /* Calculate the check-sum */
    sum = 0;
    for (idx = 2; idx < (2 + (0x3f - 0x10 + 1)); idx++) {
        sum += response[idx];
    }
    sum &= 0xffU;
    if (sum != 0) {
        return DIAG_ERR_BAD_RW;
    }

    /*
     * Check 3: The sum of data in addresses 0xc0-0xff == 0
     */
    command[0] = 0xd4;
    command[1] = 0x06;
    eeprom_addr = 0xc0;
    idx = 2;
    while (eeprom_addr <= 0xff) {
        command[idx++] = 0xa0;
        command[idx++] = (UINT8)eeprom_addr;
        eeprom_addr++;
    }

    /* Clean the response buffer */
    memset(response, 0, sizeof(response));

    /* Send ReadRegister Command */
    rc = g_drv_func->execute_command(dev, command, idx,
                                     sizeof(response), response, &response_len,
                                     timeout);

    /* execute_command fails in case of bad communication */
    if (rc != ICS_ERROR_SUCCESS) {
        return DIAG_ERR_BAD_COMM;
    }

    /* Syntax error occurs if EEPROM is not available */
    if ((response_len == 1) &&
        (response[0] == 0x7f)) {
        return DIAG_ERR_BAD_RW;
    }

    /* invalid response */
    if ((response_len != (2 + (0xff - 0xc0 + 1))) ||
        (response[0] != 0xd5) ||
        (response[1] != 0x07)) {
        return DIAG_ERR_BAD_COMM;
    }

    /* Calculate the check-sum */
    sum = 0;
    for (idx = 2; idx < (2 + (0xff - 0xc0 + 1)); idx++) {
        sum += response[idx];
    }
    sum &= 0xffU;
    if (sum != 0) {
        return DIAG_ERR_BAD_RW;
    }

    return DIAG_SUCCESS;
}
