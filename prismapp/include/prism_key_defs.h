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
//  prism_key_defs.h - Prism key codes and event types
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

BEGIN_NAMESPACE_PRISM

#ifndef _PRISMKEYDEFS_
#define _PRISMKEYDEFS_

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PM_KEY_BACKSPACE    0x08
#define PM_KEY_TAB          0x09
#define PM_KEY_CR           0x0d
#define PM_KEY_ESC          0x1b
#define PM_KEY_SPACE        0x20




///////////////////////////////////////////////////////////////////////////////
// Map these non-printable control keys into the 
// Unicode range 0x800 to 0x8ff, which is unused
// by the Unicode standard. This ensures that control
// keys are never confused with Unicode characters.
//
///////////////////////////////////////////////////////////////////////////////

#define PM_KEY_DELETE       0x800    
#define PM_KEY_INSERT       0x801 
#define PM_KEY_END          0x802 
#define PM_KEY_LNUP         0x803  
#define PM_KEY_LNDN         0x804  
#define PM_KEY_PGUP         0x805  
#define PM_KEY_PGDN         0x806  

#define PM_KEY_LEFT         0x810 
#define PM_KEY_RIGHT        0x811 
#define PM_KEY_ALT          0x812  
#define PM_KEY_CTRL         0x813  
#define PM_KEY_SHIFT        0x814 
#define PM_KEY_CAPS         0x815   
#define PM_KEY_NUMLOCK      0x816  
#define PM_KEY_SCROLLLOCK   0x817 

#define PM_KEY_NEXT         0x820
#define PM_KEY_PREVIOUS     0x821    

#define PM_KEY_HOME         0x830  
#define PM_MENU_KEY         0x831

// The function keys are not used by Prism, but might
// be used by applications.
//
// On the desktop platforms, the F1 key is mapped to
// PM_MENU_KEY so PM_KEY_F1 is never used.

#define PM_KEY_F1           0x841  
#define PM_KEY_F2           0x842
#define PM_KEY_F3           0x843
#define PM_KEY_F4           0x844
#define PM_KEY_F5           0x845
#define PM_KEY_F6           0x846
#define PM_KEY_F7           0x847
#define PM_KEY_F8           0x848
#define PM_KEY_F9           0x849
#define PM_KEY_F10          0x84a

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

   
            
#define PM_KEYMOD_LSHIFT 0x01     
#define PM_KEYMOD_RSHIFT 0x02     
#define PM_KEYMOD_SHIFT  0x03  

#define PM_KEYMOD_CTRL   0x04      
#define PM_KEYMOD_ALT    0x08    
#define PM_KEYMOD_CAPS   0x10    

#define PM_KEYMOD_LBUTTON 0x8000
#define PM_KEYMOD_RBUTTON 0x4000

#endif 

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifdef INCLUDE_PRISM_AT_KEYTABLE 

#define AT_KEYTABLE_LEN 0x50

pm_int_t  ATKeyTable[AT_KEYTABLE_LEN][2] = {
                     {0x00,            0x00}, 
                  {PM_KEY_ESC,          PM_KEY_ESC}, 
                     {'1',             '!'}, 
                     {'2',             '@'}, 
                     {'3',             '#'}, 
                     {'4',             '$'}, 
                     {'5',             '%'}, 
                     {'6',             '^'}, 
                     {'7',             '&'}, 
                     {'8',             '*'}, 
                     {'9',             '('}, 
                     {'0',             ')'}, 
                     {'-',             '_'}, 
                     {'=',             '+'}, 
            {PM_KEY_BACKSPACE,    PM_KEY_BACKSPACE},
                  {PM_KEY_TAB,          PM_KEY_TAB},
                     {'q',             'Q'}, 
                     {'w',             'W'}, 
                     {'e',             'E'}, 
                     {'r',             'R'}, 
                     {'t',             'T'}, 
                     {'y',             'Y'}, 
                     {'u',             'U'}, 
                     {'i',             'I'}, 
                     {'o',             'O'}, 
                     {'p',             'P'}, 
                     {'[',             '{'}, 
                     {']',             '}'}, 
                   {PM_KEY_CR,           PM_KEY_CR},
                 {PM_KEY_CTRL,         PM_KEY_CTRL}, 
                     {'a',             'A'}, 
                     {'s',             'S'}, 
                     {'d',             'D'}, 
                     {'f',             'F'}, 
                     {'g',             'G'}, 
                     {'h',             'H'}, 
                     {'j',             'J'}, 
                     {'k',             'K'}, 
                     {'l',             'L'}, 
                     {';',             ':'}, 
                    {'\'',            '\"'}, 
                     {'`',             '`'},
                     {0,               0}, 
                     {'\\',            '|'}, 
                     {'z',             'Z'}, 
                     {'x',             'X'}, 
                     {'c',             'C'}, 
                     {'v',             'V'}, 
                     {'b',             'B'}, 
                     {'n',             'N'}, 
                     {'m',             'M'}, 
                     {',',             '<'}, 
                     {'.',             '>'}, 
                     {'/',             '?'}, 
                     {0,               0}, 
                     {'*',             '*'}, 
                  {PM_KEY_ALT,          PM_KEY_ALT}, 
                     {' ',             ' '}, 
                     {0,               0}, 
                   {PM_KEY_HOME,         PM_KEY_HOME},    
                   {PM_KEY_LNUP,         PM_KEY_LNUP},    
                   {PM_KEY_PGUP,         PM_KEY_PGUP},    
                   {'-',             '_'},    
                   {PM_KEY_LEFT,         PM_KEY_LEFT},    
                   {0,               0},   
                   {PM_KEY_RIGHT,        PM_KEY_RIGHT},
                      {'+',             '+'}, 
                   {PM_KEY_END,          PM_KEY_END},    
                   {PM_KEY_LNDN,         PM_KEY_LNDN},    
                   {PM_KEY_PGDN,         PM_KEY_PGDN},    
                   {PM_KEY_INSERT,       PM_KEY_INSERT},     
                   {PM_KEY_DELETE,       PM_KEY_DELETE},     
};          

END_NAMESPACE_PRISM
#endif

