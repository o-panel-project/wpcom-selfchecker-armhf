/**
 * \brief    a header file for the RC-S956 module (ISO/IEC 14443-3 Type A)
 * \date     2009/05/25
 * \author   Copyright 2005,2006,2007,2008,2009 Sony Corporation
 */

#include "ics_types.h"
#include "ics_hwdev.h"
#include "icsdrv.h"
#include "rcs956.h"
#include "typea_card.h"
#include "iso14443_4_card.h"

#ifndef RCS956_TYPEA_H_
#define RCS956_TYPEA_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Constant
 */

#define RCS956_MAX_TYPEA_COMMAND_LEN                      262
#define RCS956_MAX_TYPEA_RESPONSE_LEN                     260

#define RCS956_MODE_TYPEA_INITIATOR  0x02

/*
 * Macro
 */

/*
 * Prototype declaration
 */

UINT32 rcs956_typea_polling(
    ICS_HW_DEVICE* rcs956,
    const UINT8* req_uid,
    UINT32 req_uid_len,
    typea_card_t* card,
    iso14443_4_ats_t* ats,
    UINT32 timeout);

UINT32 rcs956_typea_command(
    ICS_HW_DEVICE* rcs956,
    typea_card_t* card,
    const UINT8* command,
    UINT32 command_len,
    UINT32 max_response_len,
    UINT8* response,
    UINT32* response_len,
    UINT32 command_timeout,
    UINT32 timeout);

UINT32 rcs956_typea_deselect(
    ICS_HW_DEVICE* rcs956,
    typea_card_t* card,
    UINT32 timeout);
UINT32 rcs956_typea_select(
    ICS_HW_DEVICE* rcs956,
    typea_card_t* card,
    UINT32 timeout);
UINT32 rcs956_typea_release(
    ICS_HW_DEVICE* rcs956,
    typea_card_t* card,
    UINT32 timeout);

#ifdef __cplusplus
}
#endif

#endif /* !RCS956_TYPEA_H_ */
