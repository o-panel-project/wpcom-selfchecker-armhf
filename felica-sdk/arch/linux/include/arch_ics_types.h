/**
 * \brief    Definition of types used in ICS Library (Linux)
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#ifndef ARCH_ICS_TYPES_H_
#define ARCH_ICS_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * [Porting Note]
 *   Modify these definitions according to your system.
 */

/* file handle */

typedef int  ICS_HANDLE;
#define ICS_INVALID_HANDLE  (-1)

/* integer types */

typedef int            INT;
typedef unsigned int   UINT;

/* boolean */

typedef int            BOOL;
#ifdef TRUE
#undef TRUE
#endif
#define TRUE   (!0)
#ifdef FALSE
#undef FALSE
#endif
#define FALSE  0

/* bit-width-specific integer types */

typedef signed char    INT8;
typedef unsigned char  UINT8;
typedef signed short   INT16;
typedef unsigned short UINT16;
typedef signed int     INT32;
typedef unsigned int   UINT32;

#ifdef __cplusplus
}
#endif

#endif /* !ARCH_ICS_TYPES_H_ */
