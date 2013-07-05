/*
 * Copyright 2006,2007,2008 Sony Corporation
 */

#include <rcs956_usb.h>
#include <felica_cc.h>
#include <stub/felica_cc_stub_rcs956.h>

const icsdrv_basic_func_t* g_drv_func = &rcs956_usb_basic_func;

UINT32 (*g_felica_cc_stub_initialize_func)(
    felica_cc_devf_t* devf,
    ICS_HW_DEVICE* dev) = felica_cc_stub_rcs956_initialize;
