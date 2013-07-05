/**
 * \brief    The stub functions binding felica_cc to rcs956_uart/usb.
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#include "ics_types.h"
#include "felica_cc.h"
#include "ics_hwdev.h"

#ifndef FELICA_CC_STUB_RCS956_H_
#define FELICA_CC_STUB_RCS956_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Constant
 */

#define FELICA_CC_STUB_RCS956_MAX_COMMAND_LEN             254
#define FELICA_CC_STUB_RCS956_MAX_RESPONSE_LEN            254

/*
 * Prototype declaration
 */

UINT32 felica_cc_stub_rcs956_initialize(
    felica_cc_devf_t* devf,
    ICS_HW_DEVICE* rcs956_dev);

#ifdef __cplusplus
}
#endif

#endif /* !FELICA_CC_STUB_RCS956_H_ */
