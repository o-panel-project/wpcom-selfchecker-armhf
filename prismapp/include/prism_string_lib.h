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
//  prism_string_lib.h - Prism string library replacements and constants
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_STRING_LIBRARY_
#define _PRISM_STRING_LIBRARY_

BEGIN_NAMESPACE_PRISM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class pm_string_t
{
    friend class Pm_Text_Widget;

    public:

        pm_string_t(void);
        pm_string_t(pm_uint_t StringId);
        pm_string_t(const pm_char_t *pStr);
        pm_string_t(const char *pStr);

        ~pm_string_t();

		void AssignText(const pm_uint_t StringId);
		void AssignText(const pm_char_t *pStr);
		void AssignText(const char *pStr);

        pm_uint_t GetStringId(void) {return mStringId;}
        pm_int_t GetStringLen(void) {return mStrLen;}
		
    protected:
    
        void Init(pm_uint_t StringId, const pm_char_t *pText);

        pm_int_t mStrLen;
        pm_bool_t mCopy;
        pm_uint_t mStringId;
        pm_char_t *mpText;
};


#if defined(PRISM_LTOA_REPLACEMENT)
char      *Pm_ltoa(long val, char *s, int rad);
pm_char_t *Pm_ltoa(long val, pm_char_t *s, int rad);

#else
#define Pm_ltoa(a, b, c) _ltoa(a, b, c)
#endif

#if defined(PRISM_STRING_LIBRARY_REPLACEMENTS)

pm_char_t *Pm_strcat(pm_char_t *s1, const pm_char_t *s2);
pm_char_t *Pm_strncat(pm_char_t *s1, const pm_char_t *s2, int iMax);
pm_char_t *Pm_strcpy(pm_char_t *s1, const pm_char_t *s2);
pm_char_t *Pm_strncpy(pm_char_t *s1, const pm_char_t *s2, int iMax);
int      Pm_strcmp(const pm_char_t *s1, const pm_char_t *s2);
int      Pm_strncmp(const pm_char_t *s1, const pm_char_t *s2, int iMax);
int      Pm_strlen(const pm_char_t *s1);
int32_t  Pm_atol(const pm_char_t *s1);
int      Pm_atoi(const pm_char_t *s1);

int      Pm_strcmp(const char *s1, const char *s2);
int      Pm_strncmp(const char *s1, const char *s2, int iMax);
int      Pm_strlen(const char *s1);
int      Pm_atoi(const char *s1);
long     Pm_atol(const char *s1);
char     *Pm_strcpy(char *s1, const char *s2);
char     *Pm_strncpy(char *s1, const char *s2, int iMax);
char     *Pm_strcat(char *s1, const char *s2);
char     *Pm_strncat(char *s1, const char *s2, int iMax);

char *Pm_strcpy(char *s1, const pm_char_t *s2);
pm_char_t *Pm_strcpy(pm_char_t *s1, const char *s2);
char *Pm_strncpy(char *s1, const pm_char_t *s2, int iMax);
pm_char_t *Pm_strncpy(pm_char_t *s1, const char *s2, int iMax);
char *Pm_strcat(char *s1, const pm_char_t *s2);
pm_char_t *Pm_strcat(pm_char_t *s1, const char *s2);
char *Pm_strncat(char *s1, const pm_char_t *s2, int iMax);
pm_char_t *Pm_strncat(pm_char_t *s1, const char *s2, int iMax);


#else 

#define Pm_strcat(a, b) strcat(a, b)
#define Pm_strncat(a, b, c) strncat(a, b, c)
#define Pm_strcpy(a, b) strcpy(a, b)
#define Pm_strncpy(a, b, c) strncpy(a, b, c)
#define Pm_strcmp(a, b) strcmp(a, b)
#define Pm_strncmp(a, b, c) strncmp(a, b, c)
#define Pm_strlen(a) strlen(a)
#define Pm_atol(a) atol(a)
#define Pm_atoi(a) atoi(a)


#endif 


void     Pm_Uni_to_Ascii(char *s1, const pm_char_t *s2);
void     Pm_Ascii_to_Uni(pm_char_t *s1, const char *s2);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef PRISM_BIG5_CONVERSION

void Big5ToUnicode(pm_char_t *s1);
void Big5ToUnicode(pm_char_t *s1, const pm_char_t *s2);
pm_char_t Big5ToUnicode(pm_char_t c1);
void UnicodeToBig5(pm_char_t *s1);
void UnicodeToBig5(pm_char_t *s1, const pm_char_t *s2);
pm_char_t UnicodeToBig5(pm_char_t c1);

#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef PRISM_SJIS_CONVERSION

void SJISToUnicode(pm_char_t *s1);
void SJISToUnicode(pm_char_t *s1, const pm_char_t *s2);
pm_char_t SJISToUnicode(pm_char_t c1);
void UnicodeToSJIS(pm_char_t *s1);
void UnicodeToSJIS(pm_char_t *s1, const pm_char_t *s2);
pm_char_t UnicodeToSJIS(pm_char_t c1);

#endif

#ifdef PRISM_UTF8_ENCODING
pm_char_t *PrismUtf8StrnCpy(pm_char_t *s1, const uint8_t *s2, int iMax);
pm_int_t PrismFindUniStrLen(const uint8_t *Text);
pm_int_t PrismUtf8LenFromUni(const pm_char_t *Text);
void PrismUtf8ToUnicode(const uint8_t *Text, pm_char_t *UniText);
void PrismUnicodeToUtf8(const pm_char_t *Text, uint8_t *Utf8Text);
#endif


const pm_char_t PM_SC_MINIMIZE[] = {'M','i','n','i','m','i','z','e','\0'};
const pm_char_t PM_SC_MAXIMIZE[] = {'M','a','x','i','m','i','z','e','\0'};

const pm_char_t PM_SC_OK[] = {'O','K','\0'};
const pm_char_t PM_SC_NO[] = {'N','o','\0'};
const pm_char_t PM_SC_YES[] = {'Y','e','s','\0'};
const pm_char_t PM_SC_ABORT[] = {'A','b','o','r','t','\0'};
const pm_char_t PM_SC_CANCEL[] = {'C','a','n','c','e','l','\0'};
const pm_char_t PM_SC_RETRY[] =  {'R','e','t','r','y','\0'};
const pm_char_t PM_SC_CLOSE[] = {'C','l','o','s','e','\0'};
const pm_char_t PM_SC_APPLY[] = {'A','p','p','l','y', 0};

const pm_char_t PM_SC_NULL[]     = {'\0'};
const pm_char_t PM_SC_RESTORE[]  = {'R','e','s','t','o','r','e','\0'};
const pm_char_t PM_SC_SPACE[]    = {' ','\0'};
const pm_char_t PM_SC_HELP[]     = {'H','e','l','p', 0};
const pm_char_t PM_SC_DOTDOT[]   = {'.','.','.',0};

const pm_int_t PM_SID_MINIMIZE =   1;
const pm_int_t PM_SID_MAXIMIZE =   2;

const pm_int_t PM_SID_OK =         3;
const pm_int_t PM_SID_NO =         4;
const pm_int_t PM_SID_YES =        5;
const pm_int_t PM_SID_ABORT =      6;
const pm_int_t PM_SID_CANCEL =     7;
const pm_int_t PM_SID_RETRY =      8;
const pm_int_t PM_SID_CLOSE =      9;
const pm_int_t PM_SID_APPLY =      10;
const pm_int_t PM_SID_DOTDOT =     12;

END_NAMESPACE_PRISM

#endif  



