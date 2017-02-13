/**
 * \brief    ISO/IEC 14443-4 Card
 * \date     2009/05/25
 * \author   Copyright 2005,2006,2008,2009 Sony Corporation
 */

#include "ics_types.h"

#ifndef ISO14443_4_CARD_H_
#define ISO14443_4_CARD_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Constant
 */

#define ISO14443_4_CARD_MAX_ATS_LEN                     (256 - 2)

/*
 * Type and structure
 */

typedef struct iso14443_4_ats_t {
    UINT32 len;
    UINT8 data[ISO14443_4_CARD_MAX_ATS_LEN];
} iso14443_4_ats_t;

#ifdef __cplusplus
}
#endif

#endif /* !ISO14443_4_CARD_H_ */
