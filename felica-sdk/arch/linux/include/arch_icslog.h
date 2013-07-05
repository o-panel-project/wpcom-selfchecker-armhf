/**
 * \brief    the header file for ICS log facilities (Linux)
 * \date     2008/10/14
 * \author   Copyright 2005,2007,2008 Sony Corporation
 */

#ifndef ARCH_ICSLOG_H_
#define ARCH_ICSLOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ICSLOG_PRINTF
#include <stdio.h>
#define ICSLOG_PRINTF printf
#endif

#ifdef __cplusplus
}
#endif

#endif /* !ARCH_ICSLOG_H_ */
