/**
 * \brief    Type A Card
 * \date     2009/05/25
 * \author   Copyright 2005,2006,2008,2009 Sony Corporation
 */

#include "ics_types.h"
#include "iso14443_4_card.h"

#ifndef TYPEA_CARD_H_
#define TYPEA_CARD_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Type and structure
 */

typedef struct typea_card_t {
    UINT8 atqa[2];
    UINT8 sak;
    UINT32 uid_len;
    UINT8 uid[10];
    UINT32 priv_value;
    iso14443_4_ats_t* ats;
    void* priv_data;
} typea_card_t;

#ifdef __cplusplus
}
#endif

#endif /* !TYPEA_CARD_H_ */
