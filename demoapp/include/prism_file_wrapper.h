///////////////////////////////////////////////////////////////////////////////
//
//            Copyright (c) 2010 by Blue Water Embedded, Inc.
//
//  This software is copyrighted by and is the sole property of Blue Water
//  Embedded, Inc. All rights, title, ownership, or other intersts in the
//  software remain the property of Blue Water Embedded, Inc. This software
//  may only be used in accordance with the corresponding license agreement.
//  Any unauthorized use, duplications, transmission, distribution, or
//  disclosure of this software is expressly forbidden.
//
//  This Copyright notice may not be removed or modified without prior
//  written consent of Blue Water Embedded, Inc.
//
//  Blue Water Embedded, Inc. reserves the right to modify this software
//  without notice.
//
//  Blue Water Embedded, Inc.               info@bwembedded.com
//  3847 Pine Grove Ave                     http://www.bwembedded.com
//  Suite A
//  Fort Gratiot, MI 48059
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//  prism_file_wrapper - Prism file I/O wrapper class
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_FILE_
#define _PRISM_FILE_

BEGIN_NAMESPACE_PRISM

#if defined(PRISM_FILEIO_ABSTRACTION)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(MAX_PATH)
#define PRISM_MAX_PATH MAX_PATH
#else
#define PRISM_MAX_PATH 256
#endif  

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// put all the write modes first in the list:
const pm_uint_t PRISM_FILEMODE_RW   = 0x0001;  
const pm_uint_t PRISM_FILEMODE_WTC  = 0x0002;  
const pm_uint_t PRISM_FILEMODE_RWC  = 0x0004;  
const pm_uint_t PRISM_FILEMODE_RWTC = 0x0008;
const pm_uint_t PRISM_FILEMODE_WC   = 0x0010;   

// then binary and read modes
const pm_uint_t PRISM_FILEMODE_RO   = 0x0080;   
const pm_uint_t PRISM_FILEMODE_BIN  = 0x8000;   

const pm_uint_t PRISM_FILEMODE_MASK = 0x0fff;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PRISM_FILE_SEEK_SET       0x01
#define PRISM_FILE_SEEK_CUR       0x02
#define PRISM_FILE_SEEK_END       0x04

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(PRISM_FILESYS_ANSI_C)

#include <stdio.h>
#define prism_file_t          FILE*
#define PRISM_EOF             EOF

#elif defined(PRISM_FILESYS_FILEX)

#define PRISMX_FLUSH_CACHE  

#include "fx_api.h"
#define prism_file_t        FX_FILE*
#define prism_media_t       FX_MEDIA*

#define PRISM_EOF             -1
#define PRISM_FILE_SUCCESS    FX_SUCCESS
#define PRISM_FILE_NOT_FOUND  FX_NOT_FOUND

#if defined(PRISMX_FLUSH_CACHE)
#include "fx_user.h"
#endif 

#else

#error "No filesystem type was specified"

#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_File
{
    public:

        Pm_File();

       #if defined(PRISM_FILESYS_FILEX)
        Pm_File(prism_media_t pMedia);
       #endif

        virtual ~Pm_File();

        virtual void Close(void);
        virtual int8_t GetByte(void);

        const char *GetFileName(void) const
        {
            return mpFileName;
        }
        virtual pm_uint_t GetMode(void) const
        {
            return mMode;
        }
        virtual pm_bool_t IsEOF(void);
        virtual pm_bool_t IsOpen(void);

        virtual pm_bool_t Open(const char *pFileName,
                               const pm_uint_t Mode = PRISM_FILEMODE_RO);

        virtual pm_uint_t Read(void *pBuffer, pm_uint_t Size);

        pm_uint_t ReadRecords(void *pRecs, pm_uint_t RecSize,
                              pm_uint_t Count);

        virtual pm_uint_t ReadString(char *pBuffer, pm_uint_t Size);
        virtual pm_bool_t Seek(pm_int_t Offset, pm_uint_t Start);

       #if defined(PRISM_FILESYS_FILEX)
        virtual void SetMedia(prism_media_t pMedia) 
        {
            mpMedia = pMedia;
        }
       #endif

        virtual pm_uint_t Size();

        virtual pm_uint_t Write(const void *pBuffer,
                                const pm_uint_t Size = 1);

        pm_uint_t WriteRecords(const void *pRecs, pm_uint_t RecSize,
                               pm_uint_t Count);

        virtual pm_uint_t WriteString(const char *pBuffer);
        virtual pm_uint_t WriteString(const pm_char_t *pBuffer);

    protected:

       #if defined(PRISM_FILESYS_ANSI_C)
        char *BuildModeString_ANSI(const pm_uint_t Mode);
       #elif defined(PRISM_FILESYS_FILEX)
        pm_uint_t DetermineFileXMode(pm_uint_t Mode);
       #endif

        prism_file_t mpFile;
        char *mpFileName;

       #if defined(PRISM_FILESYS_FILEX)
        prism_media_t   mpMedia;
        FX_FILE         mFile;
       #endif

        pm_uint_t mMode;
};

#endif  

END_NAMESPACE_PRISM

#endif 


