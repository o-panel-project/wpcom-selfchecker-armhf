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
//  prism_zip.h - Prism zip/unzip utility
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// NOTES:
//
// This software module originated as "zlib", a free software library that
// implements Limpel/Ziv compression and decompression. The original
// code has been heavilty modified to work within the Prism Runtime
// Framework.
//
// zlib was written by Jean-loup Gailly (compression) and
// Mark Adler (decompression). They can be reaches at zlip@gzip.org.
//
// The original code can also be downloaded here:
//
// http://zlib.net
//
// Pm_Zip and Pm_Unzip are just wrappers around the zlib code designed
// to make it easy to use and to utilize the Prism File I/O abstraction
// layer for portability.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _PRISM_ZIP_
#define _PRISM_ZIP_

#if defined(PRISM_ZIP) || defined(PRISM_UNZIP)

#if defined(PRISM_ZIP)

pm_int_t Pm_Zip( uint8_t **pDest, uint32_t *pDestLen,
    const uint8_t *pSource, uint32_t sourceLen);

#endif 

#if defined(PRISM_UNZIP)

pm_int_t Pm_Unzip( uint8_t *pDest, uint32_t *pDestLen,
                   const uint8_t *pSource,
                   uint32_t sourceLen );

#endif


#define PZIP_MAX_MEM_LEVEL 9
#define PZIP_MAX_WBITS 15

struct InflateInternalState;
struct DeflateInternalState;

typedef struct 
{
    const uint8_t *mpNextIn;  
    uint32_t mAvailIn;         
    uint32_t mTotalIn;

    uint8_t *mpOutBuf;           
    uint8_t* mpNextOut;        
    uint32_t mAvailOut;        
    uint32_t mTotalOut;         

    InflateInternalState *mpInflate;
    DeflateInternalState *mpDeflate; 

    pm_int_t mDataType;           
    uint32_t mAdler; 

} PzipStream;

#define PZIP_NO_FLUSH      0
#define PZIP_PARTIAL_FLUSH 1
#define PZIP_SYNC_FLUSH    2
#define PZIP_FULL_FLUSH    3
#define PZIP_FINISH        4

#define PZIP_OK            0
#define PZIP_STREAM_END    1
#define PZIP_NEED_DICT     2
#define PZIP_ERRNO        (-1)
#define PZIP_STREAM_ERROR (-2)
#define PZIP_DATA_ERROR   (-3)
#define PZIP_MEM_ERROR    (-4)
#define PZIP_BUF_ERROR    (-5)
#define PZIP_VERSION_ERROR (-6)

#define PZIP_NO_COMPRESSION         0
#define PZIP_BEST_SPEED             1
#define PZIP_BEST_COMPRESSION       9
#define PZIP_DEFAULT_COMPRESSION  (-1)

#define PZIP_FILTERED            1
#define PZIP_HUFFMAN_ONLY        2
#define PZIP_DEFAULT_STRATEGY    0

#define PZIP_BINARY   0
#define PZIP_ASCII    1
#define PZIP_UNKNOWN  2

#define PZIP_DEFLATED   8

#if defined(PRISM_ZIP)

pm_int_t PzipDeflateInit( PzipStream* pStrm, int32_t level, int32_t method,
                        int32_t windowBits, int32_t memLevel, pm_int_t strategy,
                        const pm_char_t* pVersion, uint32_t streamSize );



pm_int_t PzipDeflate( PzipStream* pStrm, pm_int_t flush );


pm_int_t PzipDeflateEnd( PzipStream* pStrm );

pm_int_t PzipDeflateReset( PzipStream* pStrm );

#endif  

#if defined(PRISM_UNZIP)



pm_int_t PzipInflateInit( PzipStream* pZ, int32_t windowBits,
                      const pm_char_t* pVersion, uint32_t streamSize );



pm_int_t PzipInflate( PzipStream* pStrm, int32_t flush );

pm_int_t PzipInflateEnd( PzipStream* pZ );

pm_int_t PzipInflateReset( PzipStream* pZ );

#endif 

uint32_t PzipAdler32( uint32_t adler, const uint8_t* pBuf, uint32_t len );

uint32_t PzipCrc32( uint32_t crc, const uint8_t* pBuf, pm_uint_t len );


#define PZIP_DEF_WBITS PZIP_MAX_WBITS

#if PZIP_MAX_MEM_LEVEL >= 8
#  define PZIP_DEF_MEM_LEVEL 8
#else
#  define PZIP_DEF_MEM_LEVEL PZIP_MAX_MEM_LEVEL
#endif

#define PZIP_STORED_BLOCK 0
#define PZIP_STATIC_TREES 1
#define PZIP_DYN_TREES 2

#define PZIP_MIN_MATCH 3
#define PZIP_MAX_MATCH 258

#define PZIP_PRESET_DICT 0x20 

typedef uint32_t (*PzipCheckFunc)( uint32_t, const uint8_t*, uint32_t );

#if defined(PRISM_UNZIP)

struct PzipInflateBlocksState;


PzipInflateBlocksState* PzipInflateBlocksNew(
   PzipStream* pZ, PzipCheckFunc c, uint32_t w );

pm_int_t PzipInflateBlocks( PzipInflateBlocksState*, PzipStream*, pm_int_t );

void PzipInflateBlocksReset( PzipInflateBlocksState*, PzipStream*, uint32_t* );
int32_t PzipInflateBlocksFree( PzipInflateBlocksState*, PzipStream* );

typedef enum
{
    METHOD,  
    FLAG,   
    DICT_4,   
    DICT_3,  
    DICT_2, 
    DICT_1,   
    DICT_0,   
    BLOCKS,  
    CHECK_4,   
    CHECK_3, 
    CHECK_2,   
    CHECK_1,   
    INFLATE_DONE,   
    INFLATE_BAD      
} InflateMode;

struct InflateInternalState
{
    InflateMode  mMode; 

    union
    {
        pm_uint_t mMethod; 

        struct
        {
            uint32_t mWas;              
            uint32_t mNeed;            
        } mCheck;        
    } mSub;  

    int32_t mNoWrap; 
    pm_uint_t mWbits;         
    PzipInflateBlocksState* mpBlocks;  
};


#endif 

#if defined(PRISM_UNZIP)


typedef struct
{
    union
    {
        struct
        {
            uint8_t mExop;      
            uint8_t mBits;      
        } mWhat;
    } mWord;

    uint32_t mBase;          
} PzipInflateHuft;


#define MANY 1440

pm_int_t PzipInflateTreesBits( uint32_t*, uint32_t*, PzipInflateHuft**,
                             PzipInflateHuft*, PzipStream* );

pm_int_t PzipInflateTreesDynamic( uint32_t, uint32_t, uint32_t*, 
                                uint32_t*, uint32_t*,
                                PzipInflateHuft**, PzipInflateHuft**,
                                PzipInflateHuft*, PzipStream* );

pm_int_t PzipInflateTreesFixed( uint32_t*, uint32_t*, PzipInflateHuft**,
                              PzipInflateHuft**, PzipStream* );

#endif  

#if defined(PRISM_UNZIP)

struct InflateCodesState;

InflateCodesState* PzipInflateCodesNew( uint32_t, uint32_t, 
                                        PzipInflateHuft*, PzipInflateHuft*, 
                                        PzipStream* );

pm_int_t PzipInflateCodes( PzipInflateBlocksState*, PzipStream*, pm_int_t );

void PzipInflateCodesFree( InflateCodesState*, PzipStream* );

#endif 

#if defined(PRISM_UNZIP)

typedef enum
{
    PZIP_TYPE,   
    PZIP_LENS,    
    PZIP_STORED,  
    PZIP_TABLE,   
    PZIP_BTREE,  
    PZIP_DTREE,  
    PZIP_CODES,    
    PZIP_DRY,     
    PZIP_DONE,   
    PZIP_BAD   
} PzipInflateBlockMode;

struct PzipInflateBlocksState
{
    PzipInflateBlockMode mMode;  

    union
    {
        uint32_t mLeft; 

        struct
        {
            pm_uint_t mTable;    
            pm_uint_t mIndex;        
            uint32_t* mpBlens;   
            uint32_t mBb;     
            PzipInflateHuft* mpTb; 
        } mTrees;   

        struct
        {
            InflateCodesState* mpCodes;
        } mDecode;   
    } mSub;        

    pm_uint_t mLast;     

    uint32_t mBitk; 
    uint32_t mBitb; 
    PzipInflateHuft* mpHufts; 
    uint8_t* mpPaneldow;
    uint8_t* mpEnd; 
    uint8_t* mpRead;
    uint8_t* mpWrite; 
    PzipCheckFunc mCheckFn;
    uint32_t mCheck;
};

#define PZIP_UPDBITS { pS->mBitb = b; pS->mBitk = k; }

#define PZIP_UPDIN \
        { pZ->mAvailIn = n; pZ->mTotalIn += int32_t(p_p - pZ->mpNextIn); \
          pZ->mpNextIn = p_p; }

#define PZIP_UPDOUT { pS->mpWrite = p_q; }

#define PZIP_UPDATE { PZIP_UPDBITS PZIP_UPDIN PZIP_UPDOUT }

#define PZIP_LEAVE { PZIP_UPDATE return PzipInflateFlush( pS, pZ, r ); }

#define PZIP_LOADIN \
        { p_p = pZ->mpNextIn; n = pZ->mAvailIn; b = pS->mBitb; \
          k = pS->mBitk; }

#define PZIP_NEEDBYTE { if (n) { r = PZIP_OK; } else { PZIP_LEAVE } }

#define PZIP_NEXTBYTE (n--, *p_p++)

#define PZIP_NEEDBITS(j) \
        { while (k < (j) ) \
          { PZIP_NEEDBYTE; b |= ((uint32_t) PZIP_NEXTBYTE) << k; k += 8; } }

#define PZIP_DUMPBITS(j) { b >>= (j); k -= (j); }

#define PZIP_WAVAIL \
        (pm_uint_t) (p_q < pS->mpRead ? pS->mpRead - p_q - 1 : pS->mpEnd - p_q)

#define PZIP_LOADOUT { p_q = pS->mpWrite; m = (pm_uint_t) PZIP_WAVAIL; }
#define PZIP_LOADOUT_2 { p_q = pS->mpWrite; }

#define PZIP_WRAP \
        { if (p_q == pS->mpEnd && pS->mpRead != pS->mpPaneldow) \
          { p_q = pS->mpPaneldow; m = (pm_uint_t) PZIP_WAVAIL; } }

#define PZIP_FLUSH \
        { PZIP_UPDOUT r = PzipInflateFlush( pS, pZ, r); PZIP_LOADOUT }
#define PZIP_FLUSH_2 \
        { PZIP_UPDOUT r = PzipInflateFlush( pS, pZ, r); PZIP_LOADOUT_2 }

#define PZIP_NEEDOUT \
        { \
            if (m == 0) \
            { \
                PZIP_WRAP \
                if (m == 0) \
                { \
                    PZIP_FLUSH \
                    PZIP_WRAP \
                    if (m == 0) PZIP_LEAVE \
                } \
            } \
            r = PZIP_OK; \
        }

#define PZIP_OUTBYTE(a) { *p_q++ = (uint8_t) (a); m--; }

#define PZIP_LOAD { PZIP_LOADIN PZIP_LOADOUT }

extern pm_uint_t PzipInflateMask[17];


pm_int_t PzipInflateFlush( PzipInflateBlocksState*, PzipStream*, pm_int_t);

#endif 

#if defined(PRISM_UNZIP)

pm_int_t PzipInflateFast( pm_uint_t, pm_uint_t, PzipInflateHuft*, PzipInflateHuft*,
                      PzipInflateBlocksState*, PzipStream* );

#endif  

#if defined(PRISM_ZIP)

#define PZIP_LENGTH_CODES 29

#define PZIP_LITERALS 256

#define PZIP_L_CODES (PZIP_LITERALS + 1 + PZIP_LENGTH_CODES)

#define PZIP_D_CODES 30

#define PZIP_BL_CODES 19

#define PZIP_HEAP_SIZE (2 * PZIP_L_CODES + 1)

#define PZIP_MAX_BITS 15

#define PZIP_INIT_STATE 42
#define PZIP_BUSY_STATE 113
#define PZIP_FINISH_STATE 666

typedef struct
{
    union
    {
        pm_uint_t mFreq;      
        pm_uint_t mCode;      
    } mFc;

    union
    {
        pm_uint_t mDad;     
        pm_uint_t mLen;      
    } mDl;
} PzipCtData;

#define PzipFreq mFc.mFreq
#define PzipCode mFc.mCode
#define PzipDad mDl.mDad
#define PzipLen mDl.mLen

typedef struct PzipStaticTreeDescStruct PzipStaticTreeDesc;

typedef struct
{
    PzipCtData* mpDynTree;    
    pm_int_t mMaxCode;  
    PzipStaticTreeDesc* mpStatDesc;
} PzipTreeDesc;

struct DeflateInternalState
{
    PzipStream* mpStrm;     
    pm_int_t mStatus;     
    uint8_t* mpPendingBuf; 
    uint32_t mPendingBufSize; 
    uint8_t* mpPendingOut; 
    uint32_t mPending;   
    pm_int_t mNoHeader;  
    uint8_t mDataType;  
    uint8_t mMethod;    
    pm_int_t mLastFlush;

    uint32_t mWsize; 
    int32_t mWbits; 
    uint32_t mWmask; 

    uint8_t* mpPaneldow;


    uint32_t mWindowSize;


    pm_uint_t* mpPrev;

    pm_uint_t* mpHead; 

    pm_uint_t mInsh;         
    pm_uint_t mHashSize;     
    pm_uint_t mHashBits;     
    pm_uint_t mHashMask;      
    pm_uint_t mHashShift;

    int32_t mBlockStart;

    uint32_t mMatchLength;   
    pm_uint_t mPrevMatch;             
    pm_int_t mMatchAvailable;   
    uint32_t mStrStart;               
    pm_uint_t mMatchStart;          
    uint32_t mLookahead;            

    uint32_t mPrevLength;

    pm_uint_t mMaxChainLength;
    uint32_t mMaxLazyMatch;

#define PzipMaxInsertLength mMaxLazyMatch

    pm_int_t mLevel;  
    pm_int_t mStrategy; 

    uint32_t mGoodMatch;

    pm_int_t mNiceMatch;


    PzipCtData mDynLtree[PZIP_HEAP_SIZE];
    PzipCtData mDynDtree[2 * PZIP_D_CODES + 1]; 
    PzipCtData mBlTree[2 * PZIP_BL_CODES + 1];

    PzipTreeDesc mLdesc;             
    PzipTreeDesc mDdesc;          
    PzipTreeDesc mBlDesc;         

    pm_uint_t mBlCount[PZIP_MAX_BITS + 1];


    pm_int_t mHeap[2 * PZIP_L_CODES + 1];
    pm_int_t mHeapLen;           
    pm_int_t mHeapMax;    

    uint8_t mDepth[2 * PZIP_L_CODES + 1];

    uint8_t* mpLbuf;


    pm_uint_t mLitBufsize;

    pm_uint_t mLastLit; 


    pm_uint_t* mpDbuf;

    uint32_t mOptLen;       
    uint32_t mStaticLen; 
    pm_uint_t mMatches; 
    pm_int_t mLastEobLen; 

    pm_uint_t mBiBuf;
    int32_t mBiValid;
};

#define PzipPutByte(s, c) { s->mpPendingBuf[s->mPending++] = (c); }

#define PZIP_MIN_LOOKAHEAD (PZIP_MAX_MATCH + PZIP_MIN_MATCH + 1)

#define PZIP_MAX_DIST(s) ((s)->mWsize - PZIP_MIN_LOOKAHEAD)

void PzipTrInit( DeflateInternalState* pS );

int32_t PzipTrTally( DeflateInternalState* pS, uint32_t dist, uint32_t lc );

void PzipTrFlushBlock( DeflateInternalState* pS, uint8_t *pBuf,
                       uint32_t storedLen, int32_t eof );

void PzipTrAlign( DeflateInternalState* pS );

void PzipTrStoredBlock( DeflateInternalState* pS, uint8_t *pBuf,
                        uint32_t storedLen, int32_t eof );

#define PzipDcode(dist) \
    ((dist) < 256 ? gPzipDistCode[dist] : gPzipDistCode[256 + ((dist) >> 7)])

#define PzipTrTallyLit(s, c, flush) flush = PzipTrTally( s, 0, c )
#define PzipTrTallyLit_2(s, c, flush) PzipTrTally( s, 0, c )

#define PzipTrTallyDist(s, distance, length, flush) \
        flush = PzipTrTally( s, distance, length )

#endif  

#endif 

#endif

