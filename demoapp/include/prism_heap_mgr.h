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
//  prism_heap_mgr.cpp - A built-in thread-safe heap management facility
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_HEAP_MGR_
#define _PRISM_HEAP_MGR_


struct PRISM_MEM_BLOCK {
    uint32_t Magic;
    PRISM_MEM_BLOCK *pNext;
    PRISM_MEM_BLOCK *pPrev;
    PRISM_MEM_BLOCK *pNextFree;
    uint32_t Size;
};

typedef struct {
    uint8_t         *pData;
    PRISM_MEM_BLOCK *pFreeList;
    uint32_t Size;
} pm_heap_t;

void PrismHeapInitialize(pm_heap_t *pControl);

void PrismMalloc(pm_heap_t *pControl,
    void **pMem, uint32_t Size);

void PrismFree(pm_heap_t *pControl, void *pMem);

uint32_t GetHeapUsedMem(pm_heap_t *pControl);

void PrismHeapFailure(void);

#define PRISM_HEAP_USED_MAGIC 0xdeadbeefL
#define PRISM_HEAP_FREE_MAGIC 0x5a5a5a5aL

#define PRISM_HEAP_NEWBLOCK_LIMIT 256


#endif  






