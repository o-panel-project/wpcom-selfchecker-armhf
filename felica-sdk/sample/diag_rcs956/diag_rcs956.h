/**
 * \brief    Self-Diagnosing Sample Library for RC-S956
 * \date     2008/10/14
 * \author   Copyright 2008 Sony Corporation
 */

#ifndef DIAG_RCS956_H_
#define DIAG_RCS956_H_

#include "ics_types.h"
#include "ics_hwdev.h"

#define DIAG_SUCCESS                  0
#define DIAG_ERR_DISCONN              1
#define DIAG_ERR_BAD_COMM             2
#define DIAG_ERR_BAD_RW               3
#define DIAG_ERR_UNSUPPORTED_RW       4
#define DIAG_ERR_POLLING              5

int diag_rcs956_init_device(
    ICS_HW_DEVICE* dev,
    const char* port_name,
    UINT32 timeout);
int diag_rcs956_check_firmware_version(
    ICS_HW_DEVICE* dev,
    const UINT16* versions,
    UINT32 num_vers,
    UINT32 timeout);
int diag_rcs956_communication_line_test(
    ICS_HW_DEVICE* dev,
    UINT32 timeout);
int diag_rcs956_rom_test(
    ICS_HW_DEVICE* dev,
    UINT32 timeout);
int diag_rcs956_ram_test(
    ICS_HW_DEVICE* dev,
    UINT32 timeout);
int diag_rcs956_polling_test_to_card(
    ICS_HW_DEVICE* dev,
    UINT32 timeout);
int diag_rcs956_eeprom_test(
    ICS_HW_DEVICE* dev,
    UINT32 timeout);

#endif /* !DIAG_RCS956_H_ */
