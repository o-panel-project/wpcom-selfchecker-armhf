
#ifndef _RES_FILE_HEADER_
#define _RES_FILE_HEADER_

#if defined(PRISM_BINARY_RESOURCES)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RES_MODULE_TAG              0xbeef       
#define ENDIAN_SWAPPED_MODULE_TAG   0xefbe

#define RES_FILE_FORMAT_VERSION 0x0301  

#define RES_MODTYPE_STABLE  1
#define RES_MODTYPE_SLANG   2
#define RES_MODTYPE_IMAGE   3
#define RES_MODTYPE_FONT    4
#define RES_MODTYPE_COLOR   5
#define RES_MODTYPE_THEME   6

#define RES_NAME_LENGTH   64  

struct BinaryResHeader
{
    uint16_t  Magic;
    uint16_t  FormatVersion;
    uint16_t  NumThemes;
    uint16_t  HasStringTable;
};

struct BinResTheme {
    uint16_t Magic;
    uint16_t Type;
    uint16_t ThemeIndex;
};

struct ResStringTableModHeader {
    uint16_t Magic;
    uint16_t Type;
    uint16_t NumLanguages;
    uint16_t NumStringIds;
    char     UserString[120];
};

struct ResStringLangModHeader {
    uint16_t Magic;
    uint16_t Type;
    uint16_t LanguageIndex;
    char     LanguageName[RES_NAME_LENGTH];
    int32_t  StringDataSize;
};

struct ResFontModHeader {
    uint16_t   Magic;
    uint16_t   Type;
    uint16_t   NumPages;
    uint16_t   Zipped;
    uint32_t   FontId;
    char       FontName[RES_NAME_LENGTH];
};

struct ResImageModHeader {
    uint16_t  Magic;
    uint16_t  Type;
    uint16_t  NumFrames;
    uint16_t  Zipped;
    uint32_t  ImageId;
};

struct ResImageFrameHeader {
    uint16_t Magic;
    uint32_t ZipSize;
    uint32_t UnzipSize;
};

struct ResColorModHeader {
    uint16_t Magic;
    uint16_t Type;
    uint16_t ColorCount;
};


#endif      
#endif    



