/**
 * \brief    a header file for the RC-S956 module (ISO/IEC 14443-3 Type B)
 * \date     2009/07/07
 * \author   Copyright 2005,2006,2007,2008,2009 Sony Corporation
 */

#include "ics_types.h"
#include "ics_hwdev.h"
#include "icsdrv.h"
#include "rcs956.h"
#include "typeb_card.h"

#ifndef RCS956_TYPEB_H_
#define RCS956_TYPEB_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Constant
 */

#define RCS956_MAX_TYPEB_COMMAND_LEN                      262
#define RCS956_MAX_TYPEB_RESPONSE_LEN                     260

#define RCS956_MAX_TYPEB_RAW_COMMAND_LEN                  255
#define RCS956_MAX_TYPEB_RAW_RESPONSE_LEN                 255

#define RCS956_MODE_TYPEB_INITIATOR  0x03

/*
 * Macro
 */

/*
 * Prototype declaration
 */

UINT32 rcs956_typeb_polling(
    ICS_HW_DEVICE* rcs956,
    UINT8 afi,
    typeb_card_t* card,
    typeb_card_attrib_t* attrib,
    UINT32 timeout);

UINT32 rcs956_typeb_command(
    ICS_HW_DEVICE* rcs956,
    typeb_card_t* card,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 command_timeout,
    UINT32 timeout);
UINT32 rcs956_typeb_raw_command(
    ICS_HW_DEVICE* rcs956,
    typeb_card_t* card,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 command_timeout,
    UINT32 timeout);

UINT32 rcs956_typeb_deselect(
    ICS_HW_DEVICE* rcs956,
    typeb_card_t* card,
    UINT32 timeout);
UINT32 rcs956_typeb_select(
    ICS_HW_DEVICE* rcs956,
    UINT8 afi,
    typeb_card_t* card,
    UINT32 timeout);

#ifdef __cplusplus
}
#endif

#endif /* !RCS956_TYPEB_H_ */
