/**
 * \brief    Type B Card
 * \date     2009/07/07
 * \author   Copyright 2005,2006,2008,2009 Sony Corporation
 */

#include "ics_types.h"

#ifndef TYPEB_CARD_H_
#define TYPEB_CARD_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Constant
 */

#define TYPEB_CARD_MAX_ATTRIB_LEN                       256

/*
 * Type and structure
 */

typedef struct typeb_card_attrib_t {
    UINT32 len;
    UINT8 data[TYPEB_CARD_MAX_ATTRIB_LEN];
} typeb_card_attrib_t;

typedef struct typeb_card_t {
    UINT8 pupi[4];
    UINT8 application_data[4];
    UINT8 protocol_info[3];
    typeb_card_attrib_t* attrib;
    UINT32 priv_value;
    void* priv_data;
} typeb_card_t;

#ifdef __cplusplus
}
#endif

#endif /* !TYPEB_CARD_H_ */
