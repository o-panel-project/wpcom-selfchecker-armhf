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
//  prism_resource_manager.h - Prism library popup panel class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef _PRISM_RES_MANAGER_
#define _PRISM_RES_MANAGER_

BEGIN_NAMESPACE_PRISM

#define RES_LOAD_TO_RAM   0x01
#define RES_LOAD_IN_PLACE 0x02
#define RES_DELETE_OLD    0x04

///////////////////////////////////////////////////////////////////////////////
#define PSR(a) Pm_Resource_Manager::LookupString(a)
#define PBR(a) Pm_Resource_Manager::GetBitmap(a)
#define PFR(a) Pm_Resource_Manager::GetFont(a)
#define PCR(a) Pm_Resource_Manager::GetColor(a)


///////////////////////////////////////////////////////////////////////////////
typedef struct {
    pm_bitmap_t *pBitmap;
    pm_uint_t BitmapId;
    uint8_t  Flags;
} pm_bitmap_entry_t;

///////////////////////////////////////////////////////////////////////////////
typedef struct {
    pm_color_t Color;
    pm_uint_t  ColorId;
} pm_color_entry_t;

///////////////////////////////////////////////////////////////////////////////
struct pm_string_page_t {
    pm_uint_t FirstSID;
    pm_uint_t LastSID;
    pm_uint_t NumLanguages;
    const pm_char_t   ***pTable;
    pm_string_page_t *pNext;
};

///////////////////////////////////////////////////////////////////////////////
typedef struct {
    pm_font_t   *pFont;
    pm_uint_t   FontId;
    uint8_t     Flags;
} pm_font_entry_t;

///////////////////////////////////////////////////////////////////////////////
typedef struct {

    const pm_string_page_t  *pStringTable;
    const pm_font_entry_t   *pFontTable;
    const pm_bitmap_entry_t *pBitmapTable;
    const pm_color_entry_t  *pColorTable;
    pm_uint_t FontTableSize;
    pm_uint_t BitmapTableSize;
    pm_uint_t ColorTableSize;
} pm_res_table_t;


///////////////////////////////////////////////////////////////////////////////
#define LOCK_RESOURCE_MANAGER
#define UNLOCK_RESOURCE_MANAGER

///////////////////////////////////////////////////////////////////////////////
#define NUM_SYSTEM_BITMAPS      MAX_BIDS
#define NUM_SYSTEM_COLORS       MAX_CIDS
#define NUM_SYSTEM_FONTS        MAX_FIDS


///////////////////////////////////////////////////////////////////////////////
#define FIRST_APP_STRING       100
#define FIRST_APP_COLOR        NUM_SYSTEM_COLORS
#define FIRST_APP_FONT          32  
#define FIRST_APP_BMP            64
#define INVALID_COLOR_ID        0xFFFFU

#define MIN_APP_TABLE_SIZE     8

enum PRISM_DEFAULT_FONT_IDS {
    FID_DEFAULT = 0,          
    FID_SMALL,
    FID_BUTTON,         
    FID_PROMPT,          
    MAX_FIDS
};

///////////////////////////////////////////////////////////////////////////////
class Pm_Resource_Manager
{
    public:

        static void AddResource(const pm_string_page_t *pPage,
            pm_uint_t Flags = 0);

        static void AddResource(pm_uint_t BitmapId, pm_bitmap_t *pMap,
            pm_uint_t Flags = 0);

        static void AddResource(pm_uint_t ColorId, pm_color_t Color,
            pm_uint_t Flags = 0);

        static void AddResource(pm_uint_t FontId, pm_font_t *pFont,
            pm_uint_t Flags = 0);

        static void DestroyBitmap(pm_uint_t BitmapId);
        static void DestroyFont(pm_font_t *pFont);
        static void DestroyFont(pm_uint_t FontId);
        static void DestroyAllResources(void);

        static pm_bitmap_t *GetBitmap(pm_uint_t BitmapId)
        {
            if (BitmapId < mBitmapTableSize)
            {
                return mpActiveBitmapTable[BitmapId];
            }
            return NULL;
        }

        static  uint16_t GetBitmapHeight(pm_uint_t BitmapId)
        {
            if (BitmapId < mBitmapTableSize)
            {
                if (mpActiveBitmapTable[BitmapId])
                {
                    return mpActiveBitmapTable[BitmapId]->Height;
                }
            }
            return 0;
        }

        static uint16_t GetBitmapWidth(pm_uint_t BitmapId)
        {
            if (BitmapId < mBitmapTableSize)
            {
                if (mpActiveBitmapTable[BitmapId])
                {
                    return mpActiveBitmapTable[BitmapId]->Width;
                }
            }
            return 0;
        }

        static  void GetBitmapWidthHeight(pm_uint_t BitmapId,
                                                uint16_t &Width,
                                                uint16_t &Height)
        {
            if (BitmapId < mBitmapTableSize)
            {
                if (mpActiveBitmapTable[BitmapId])
                {
                    Width = mpActiveBitmapTable[BitmapId]->Width;
                    Height = mpActiveBitmapTable[BitmapId]->Height;
                    return;
                }
            }
            Width = 0;
            Height = 0;
        }

        static pm_color_t GetColor(pm_uint_t ColorId)
        {
            if (ColorId < mColorTableSize)
            {
                return mpActiveColorTable[ColorId];
            }
            return PM_CLR_BLACK;
        }

        static pm_uint_t GetCurrentLanguage(void) { return mActiveLanguage; }
        static pm_char_t *GetLanguageName(pm_uint_t Language);
        static pm_uint_t GetNumLanguages(void);

        static pm_uint_t GetFirstAvailableBitmapId(void);
        static pm_uint_t GetFirstAvailableFontId(void);
		static pm_uint_t GetFirstAvailableColorId(void);

        static pm_font_t *GetFont(pm_uint_t FontId)
        {
            if (FontId < mFontTableSize)
            {
                if (mpActiveFontTable[FontId])
                {
                    return mpActiveFontTable[FontId];
                }
            }
            return mpActiveFontTable[FID_DEFAULT];
        }

        static void Initialize(void);

       #if defined(PRISM_FREETYPE_SUPPORT)
        static pm_bool_t InstallFreetypeFont(pm_uint_t Id, FT_Face Face);
       #endif

        static void LoadConstResources(pm_res_table_t *pTable,
            pm_bool_t DeleteOld = FALSE);

        static const pm_char_t *LookupString(pm_uint_t Id);

       #ifdef PRISM_GRADIENT_SUPPORT
        static pm_bitmap_t *MakeGradient(const pm_gradient_t &Grad);
       #endif

       #if defined(PRISM_BINARY_RESOURCES)

        static uint16_t GetResThemeCount(void);
        static uint16_t GetResStringCount(void);
        static pm_res_table_t *GetResourceTable(pm_int_t Index);
        static pm_bool_t LoadResourceFile(Pm_File *pFile,
                pm_bool_t DelOldStrs = FALSE);
       #endif

        static void SetCurrentLanguage(pm_uint_t Language);
        static pm_bool_t SetLanguageName(pm_uint_t Language, char *pName);
        static void    SetNumLanguages(pm_uint_t Num);

    private:

        Pm_Resource_Manager(void);
        ~Pm_Resource_Manager();

        static pm_font_t   **mpActiveFontTable;
        static pm_bitmap_t **mpActiveBitmapTable;
        static pm_color_t  *mpActiveColorTable;

       #ifdef PRISM_GRADIENT_SUPPORT
        static pm_gradient_t *mpGradientCache;
       #endif

        static pm_uint_t mFontTableSize;
        static pm_uint_t mBitmapTableSize;
        static pm_uint_t mColorTableSize;

       #ifdef PRISM_GRADIENT_SUPPORT
        static pm_uint_t mGradientCacheSize;
       #endif

       #if defined(PRISM_BINARY_RESOURCES)

        static pm_bool_t LoadStringTable(Pm_File *pFile);
        static pm_bool_t LoadResTheme(Pm_File *pFile, pm_res_table_t *pTable);
        static pm_bool_t ReadFont(Pm_File *pFile, pm_res_table_t *pTable);
        static pm_bool_t ReadImage(Pm_File *pFile, pm_res_table_t *pTable);
        static pm_bool_t ReadColors(Pm_File *pFile, pm_res_table_t *pTable);

        static void DestroyLoadedResTables(void);

        static pm_res_table_t **mpLoadedResTables;
        static pm_int_t mNumLoadedThemes;
        static pm_int_t mLoadedStringTableEntries;
        static pm_int_t mLoadedLanguages;
        static char **mLoadedLanguageNames;
        static pm_bool_t  mBigEndian;

       #endif

        static pm_string_page_t *mpStringTable;
        static pm_char_t **mpLanguageNames;
        static pm_uint_t  mActiveLanguage;
        static pm_uint_t  mNumLanguages;
};

enum PrismSysBitmapIds
{
    BID_UP_ARROW = 1,
    BID_DOWN_ARROW,
    BID_LEFT_ARROW,
    BID_RIGHT_ARROW,

    BID_CHECK,
    BID_DOT,
    BID_DRAG,

   //#if defined(PRISM_TITLE_BAR)
    BID_WIN_ICON,
    BID_SYS_BTN,
    BID_CLOSE_BTN,
    BID_MAX_BTN,
    BID_MIN_BTN,
    BID_RESTORE_BTN,
   //#endif

    BID_EXCLUSIVE_ON,
    BID_EXCLUSIVE_OFF,
    BID_EXCLUSIVE_ON_FOCUS,
    BID_EXCLUSIVE_OFF_FOCUS,
    BID_EXCLUSIVE_ON_DIS,
    BID_EXCLUSIVE_OFF_DIS,

    BID_TOGGLE_ON,
    BID_TOGGLE_OFF,
    BID_TOGGLE_ON_FOCUS,
    BID_TOGGLE_OFF_FOCUS,
    BID_TOGGLE_ON_DIS,
    BID_TOGGLE_OFF_DIS,

#if defined(PRISM_MOUSE_SUPPORT)
    BID_MOUSE,
    BID_IBEAM,
    BID_NESW_SIZE,
    BID_NWSE_SIZE,
    BID_HORZ_SIZE,
    BID_VERT_SIZE,
#endif

//#if defined(PRISM_TOUCH_SUPPORT)
    BID_TOUCH_CAL_TARGET,
//#endif

    MAX_BIDS
};

enum PrismSysColorIds
{
    // the standard VGA color palette

   #if (PRISM_COLOR_FORMAT >= PM_CF_4BPP_VGA)
    CID_BLACK = 0,
    CID_RED,
    CID_GREEN,
    CID_BROWN,
    CID_BLUE,
    CID_MAGENTA,
    CID_CYAN,
    CID_LIGHTGRAY,
    CID_DARKGRAY,
    CID_LIGHTRED,
    CID_LIGHTGREEN,
    CID_YELLOW,
    CID_LIGHTBLUE,
    CID_LIGHTMAGENTA,
    CID_LIGHTCYAN,
    CID_WHITE,
   #else

   #if (PRISM_COLOR_FORMAT >= PM_CF_4BPP_GRAY)
    CID_BLACK,
    CID_GRAY1,
    CID_GRAY2,
    CID_GRAY3,
    CID_GRAY4,
    CID_GRAY5,
    CID_GRAY6,
    CID_GRAY7,
    CID_GRAY8,
    CID_GRAY9,
    CID_GRAY10,
    CID_GRAY11,
    CID_GRAY12,
    CID_GRAY13,
    CID_GRAY14,
    CID_WHITE,
   #else
    CID_BLACK,
    CID_DARKGRAY,
    CID_LIGHTGRAY,
    CID_WHITE,
   #endif
   #endif
   
    CID_HIGHLIGHT,
    CID_DARKEN,
    CID_SHADOW,
    CID_ACTIVE_TITLE,   
    CID_INACTIVE_TITLE, 
    CID_NORMAL_TEXT,
    CID_HIGH_TEXT,
    CID_NORM_TEXT_BACK,
    CID_HIGH_TEXT_BACK,
    CID_PANEL_FILL,
    CID_POPUP_FILL,
    CID_BORDER,
    CID_WIDGET_FILL,
    CID_SBUTTON_FACE,   
    CID_SBUTTON_TEXT,
    CID_CURSOR,
    CID_DESKTOP,
    CID_SCROLL_FILL,
    CID_SCROLL_THUMB,

    CID_RES1,
    CID_RES2,
    CID_RES3,
    CID_RES4,
    CID_RES5,
    CID_RES6,

    CID_TREE_LINE,
    CID_TREE_TOGGLE,
    CID_HELP_PRMPT,
    CID_DROPBOX_BKGND,
    CID_MENUBTN,
    CID_MENUBAR_HOV,
    CID_MENUBTN_HOV,
    CID_MENUBTN_SEP1,
    CID_MENUBTN_SEP2,
    CID_MENUBTN_DIS,

   #if defined(PRISM_GRADIENT_BUTTONS)

    CID_BTN_BORDER1,
    CID_BTN_BORDER1_PSH,
    CID_BTN_BORDER1_FOC,
    CID_BTN_BORDER1_DIS,
    CID_BTN_BORDER2,
    CID_BTN_BORDER2_PSH,
    CID_BTN_BORDER2_FOC,
    CID_BTN_BORDER2_DIS,
    CID_BTN_GRAD1,
    CID_BTN_GRAD1_PSH,
    CID_BTN_GRAD1_FOC,
    CID_BTN_GRAD1_DIS,
    CID_BTN_GRAD2,
    CID_BTN_GRAD2_PSH,
    CID_BTN_GRAD2_FOC,
    CID_BTN_GRAD2_DIS,
    CID_TITLE_GRAD1,
    CID_TITLE_GRAD2,
    CID_TITLE_GRAD3,
    CID_TITLE_GRAD4,
    CID_TITLE_GRAD5,
    CID_TITLE_GRAD1_SEL,
    CID_TITLE_GRAD2_SEL,
    CID_TITLE_GRAD3_SEL,
    CID_TITLE_GRAD4_SEL,
    CID_TITLE_GRAD5_SEL,
    CID_MENUBAR_GRAD1,
    CID_MENUBAR_GRAD2,

    CID_SLIDEBTN_GRAD1,
    CID_SLIDEBTN_GRAD1_FOC,
    CID_SLIDEBTN_GRAD1_PSH,
    CID_SLIDEBTN_GRAD2,
    CID_SLIDEBTN_GRAD2_FOC,
    CID_SLIDEBTN_GRAD2_PSH,
   #endif

    MAX_CIDS
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if defined(PRISM_SMALL_FONT_ONLY)
#define DEFAULT_FONT_PTR  &SmallFont
#else
#define DEFAULT_FONT_PTR  &MediumFont
#endif

#define SMALL_FONT_PTR    &SmallFont

#define DEFAULT_BUTTON_FONT     SMALL_FONT_PTR
#define DEFAULT_PROMPT_FONT     DEFAULT_FONT_PTR

extern pm_font_t MediumFont;	// add worldpicom
extern pm_font_t SmallFont;		// add worldpicom

END_NAMESPACE_PRISM

#endif  

