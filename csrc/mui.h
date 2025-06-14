/*

  mui.h

  Monochrome minimal user interface: Core library.

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2021, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  MUIF  (Monochrome User Interface Functions)
    n:  A number 0 to 9 without any quotes, e.g.: 5
    id: Exactly two characters or numbers in double quotes, e.g. "G5".
    cb: A callback function with the following prototype: "uint8_t muif_cb(mui_t *ui, uint8_t msg)"
      There are MANY predefined callback functions, see separate list
    var: Address of a variable. 
    text: Normal text, but special characters might be required for some callback functions, for
      example the text might contain a list of selectable elements separated with the '|' symbol.

  MUIF_STYLE(n,cb)
    Corresponding FDS command: MUI_STYLE(n)
    Change the style of any other elements after MUI_STYLE(n), does not draw anything
    
  MUIF_RO(id,cb)
    Corresponding FDS command: MUI_DATA(id, text) MUI_XY(id, x, y), MUI_XYT(id, x,y,text), MUI_XYA(id, x,y,a), MUI_XYAT(id, x,y,a,text)
    Places a read only element on the form. 
    The correct FDS command depends on the callback function.
    
  MUIF_LABEL(cb)
    Corresponding FDS command: MUI_LABEL(x,y,text)
    Places a text at the specified position, similar to MUIF_RO
    
  MUIF_GOTO(cb)
    Corresponding FDS command: MUI_GOTO(x,y,n,text)
    Places a button at the specified position, similar to MUIF_BUTTON, but does not require an ID.
    
  MUIF_BUTTON(id,cb)
    Corresponding FDS command: MUI_XY(id, x, y), MUI_XYT(id, x,y,text), MUI_XYA(id, x,y,a), MUI_XYAT(id, x,y,a,text)
    Places a selectable element on the form. 
  
  MUIF_VARIABLE(id,var,cb)
    Corresponding FDS command: MUI_XY(id, x, y), MUI_XYA(id, x,y,a)
    Places a user input element at the specified location.
    The correct FDS command depends on the callback function.


*/

#ifndef MUI_H
#define MUI_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if defined(__GNUC__) && defined(__AVR__)
#include <avr/pgmspace.h>
#endif 


/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================*/
/* defines */

#define MUI_CHECK_EOFDS


/*==========================================*/
/* GNUC AVR PROGMEM interface */

#ifdef __GNUC__
#  define MUI_NOINLINE __attribute__((noinline))
#else
#  define MUI_NOINLINE
#endif


#if defined(__GNUC__) && defined(__AVR__)
#  define mui_pgm_read(adr) pgm_read_byte_near(adr)
#  define mui_pgm_wread(adr) pgm_read_word_near(adr) 
#  define MUI_PROGMEM PROGMEM
#endif


#ifndef mui_pgm_read
#  ifndef CHAR_BIT
#  	define mui_pgm_read(adr) (*(const uint8_t *)(adr)) 
#  else
#	if CHAR_BIT > 8 
#  	  define mui_pgm_read(adr) ((*(const uint8_t *)(adr)) & 0x0ff)
#     else
#  	  define mui_pgm_read(adr) (*(const uint8_t *)(adr)) 
#     endif 
#  endif
#endif

#ifndef mui_pgm_wread
#  	define mui_pgm_wread(adr) (*(const uint16_t *)(adr)) 
#endif

#ifndef MUI_PROGMEM
#  define MUI_PROGMEM
#endif


/*=== forward declarations ===*/
typedef struct mui_struct mui_t;
typedef const struct muif_struct muif_t;
typedef uint8_t (*muif_cb)(mui_t *ui, uint8_t msg);
typedef const char fds_t MUI_PROGMEM;



/*=== struct declarations === */

struct muif_struct
{
  uint8_t id0;
  uint8_t id1;
  uint8_t cflags;             // config flags e.g. MUIF_CFLAG_IS_CURSOR_SELECTABLE, if so, then it will not receive any cursor/touch msgs
  uint8_t extra;
  void *data;                           // might be a pointer to a variable
  muif_cb cb;                        // callback
} MUI_PROGMEM;

/* assumes that pointers are 16 bit so encapsulate the wread i another ifdef __AVR__ */
#if defined(__GNUC__) && defined(__AVR__)
#  define muif_get_id0(muif) mui_pgm_read(&((muif)->id0))
#  define muif_get_id1(muif) mui_pgm_read(&((muif)->id1))
#  define muif_get_cflags(muif) mui_pgm_read(&((muif)->cflags))
#  define muif_get_extra(muif) mui_pgm_read(&((muif)->extra))
#  define muif_get_data(muif) ((void *)mui_pgm_wread(&((muif)->data)))
#  define muif_get_cb(muif) ((muif_cb)mui_pgm_wread(&((muif)->cb)))
#else
#  define muif_get_id0(muif) ((muif)->id0)
#  define muif_get_id1(muif) ((muif)->id1)
#  define muif_get_cflags(muif) ((muif)->cflags)
#  define muif_get_extra(muif) ((muif)->extra)
#  define muif_get_data(muif) ((muif)->data)
#  define muif_get_cb(muif) ((muif)->cb)
#endif



#define MUIF_MSG_NONE 0
#define MUIF_MSG_DRAW 1
#define MUIF_MSG_FORM_START 2
#define MUIF_MSG_FORM_END 3
/* MUIF_MSG_CURSOR_ENTER return values: 255=skip this field, <255, continue*/
#define MUIF_MSG_CURSOR_ENTER 4
#define MUIF_MSG_CURSOR_SELECT 5

/* optional VALUE messages, ignored by the mui core, but can be used inside the field functions  */
/* usually MUIF_MSG_VALUE_INCREMENT behaves like  MUIF_MSG_CURSOR_SELECT */
#define MUIF_MSG_VALUE_INCREMENT 6
#define MUIF_MSG_VALUE_DECREMENT 7


#define MUIF_MSG_CURSOR_LEAVE 8
#define MUIF_MSG_TOUCH_DOWN 9
#define MUIF_MSG_TOUCH_UP 10
/* MUIF_MSG_EVENT_NEXT return values: 0=not handled, 1=handled, do nothing */
/* If MUIF_MSG_EVENT_NEXT/PREV are NOT handled by the field function, then this msg will change the field */
#define MUIF_MSG_EVENT_NEXT 11
/* MUIF_MSG_EVENT_PREV return values: 0=not handled, 1=handled, do nothing */
#define MUIF_MSG_EVENT_PREV 12

/* dynamic flags */
#define MUIF_DFLAG_IS_CURSOR_FOCUS 0x01
#define MUIF_DFLAG_IS_TOUCH_FOCUS 0x02
  
/* config flags */
#define MUIF_CFLAG_IS_CURSOR_SELECTABLE 0x01
#define MUIF_CFLAG_IS_TOUCH_SELECTABLE 0x02
#define MUIF_CFLAG_IS_EXECUTE_ON_SELECT 0x04


/* end user MUIF entries */
#define MUIF(id,cflags,data,cb) { id[0], id[1], cflags, 0, data, cb} 
#define MUIF_STYLE(n,cb)  MUIF("S" #n, 0, 0, cb) 
#define MUIF_RO(id,cb) MUIF(id,0, 0,cb)
#define MUIF_LABEL(cb) MUIF(".L",0, 0,cb)
#define MUIF_GOTO(cb) MUIF(".G",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,cb)
#define MUIF_BUTTON(id,cb) MUIF(id,MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,cb)
#define MUIF_EXECUTE_ON_SELECT_BUTTON(id,cb) MUIF(id,MUIF_CFLAG_IS_CURSOR_SELECTABLE|MUIF_CFLAG_IS_EXECUTE_ON_SELECT,0,cb)
#define MUIF_VARIABLE(id,var,cb) MUIF(id,MUIF_CFLAG_IS_CURSOR_SELECTABLE,(var),cb)



/* must be smaller than or equal to 255 */
#ifndef MUI_MAX_TEXT_LEN
#define MUI_MAX_TEXT_LEN 41
#endif

#define MUI_MENU_CACHE_CNT 4

#define MUI_MENU_LAST_FORM_STACK_SIZE 4

struct mui_struct
{
  void *graphics_data;
  fds_t *root_fds;  
  
  muif_t *muif_tlist;
  size_t muif_tcnt;
  
  fds_t *current_form_fds;         // the current form, NULL if the ui is not active at the moment
  fds_t *cursor_focus_fds;           // the field which has the current cursor focus, NULL if there is no current focus
  fds_t *touch_focus_fds;            // the field which has touch focus

  fds_t *token;             // current token position

  uint16_t form_scroll_total;            // reserved for MUIF, not used by mui
  uint16_t form_scroll_top;              // reserved for MUIF, not used by mui
  uint8_t form_scroll_visible;          // reserved for MUIF, not used by mui
  
  
  //uint8_t selected_value;   // This variable is not used by the user interface but can be used by any field function
  uint8_t tmp8;
  
  /* 0: mse, 1: mud */
  uint8_t is_mud;         // a temp variable for the MUIF function to store remember up down mode. This variable will be cleared before sending MUIF_MSG_CURSOR_ENTER
  /* current field/style variables */
  //uint8_t cursor_focus_position;        // the index of the field which has focus, can be used as last argument for mui_EnterForm
  
  uint8_t delimiter;    // outer delimiter of the text part of a field
  uint8_t cmd;          // current cmd or field (e.g. U or F)
  uint8_t id0;            // identifier of the field, manually provided or derived (G cmd has fixed id "FG")
  uint8_t id1;
  uint8_t x;               // position of the field
  uint8_t y;
  uint8_t dflags;
  uint8_t arg;          // extra argument of the field. For example the G: form is put here, assigned by MUI_XYA or MUI_XYAT 
  int len;          // length of the current command
  fds_t *fds;             // current position, *fds = cmd
  muif_t *uif;                   // user interface field or style for the given id0 / id1, assigned by mui_prepare_current_field()
  char text[MUI_MAX_TEXT_LEN+1];

  /* target  */
  fds_t *tmp_fds;
  fds_t *target_fds;     // used by several task functions as a return / result value
  
  /* last form and field, used by mui_SaveForm and mui_RestoreForm */
  uint8_t last_form_id[MUI_MENU_LAST_FORM_STACK_SIZE];
  uint8_t last_form_cursor_focus_position[MUI_MENU_LAST_FORM_STACK_SIZE];
  fds_t *last_form_fds;           // not used by mui_RestoreForm, but can be used by field functions, warning: this is the FDS of the field, from where the jump started to the child (cursor_focus_fds)
  int8_t last_form_stack_pos;
  
  /* menu cursor position backup */
  /* idea is, to restore the cursor position if we jump to that form */
  uint8_t menu_form_last_added;
  uint8_t menu_form_id[MUI_MENU_CACHE_CNT];
  uint8_t menu_form_cursor_focus_position[MUI_MENU_CACHE_CNT];
} ;

#define mui_IsCursorFocus(mui) ((mui)->dflags & MUIF_DFLAG_IS_CURSOR_FOCUS)
#define mui_IsTouchFocus(mui) ((mui)->dflags & MUIF_CFLAG_IS_TOUCH_SELECTABLE)




/*=== form string definitions ===*/

#define MUI_0 "\x00"
#define MUI_1 "\x01"
#define MUI_2 "\x02"
#define MUI_3 "\x03"
#define MUI_4 "\x04"
#define MUI_5 "\x05"
#define MUI_6 "\x06"
#define MUI_7 "\x07"
#define MUI_8 "\x08"
#define MUI_9 "\x09"
#define MUI_10 "\x0a"
#define MUI_11 "\x0b"
#define MUI_12 "\x0c"
#define MUI_13 "\x0d"
#define MUI_14 "\x0e"
#define MUI_15 "\x0f"
#define MUI_16 "\x10"
#define MUI_17 "\x11"
#define MUI_18 "\x12"
#define MUI_19 "\x13"
#define MUI_20 "\x14"
#define MUI_21 "\x15"
#define MUI_22 "\x16"
#define MUI_23 "\x17"
#define MUI_24 "\x18"
#define MUI_25 "\x19"
#define MUI_26 "\x1a"
#define MUI_27 "\x1b"
#define MUI_28 "\x1c"
#define MUI_29 "\x1d"
#define MUI_30 "\x1e"
#define MUI_31 "\x1f"
#define MUI_32 "\x20"
#define MUI_33 "\x21"
#define MUI_34 "\x22"
#define MUI_35 "\x23"
#define MUI_36 "\x24"
#define MUI_37 "\x25"
#define MUI_38 "\x26"
#define MUI_39 "\x27"
#define MUI_40 "\x28"
#define MUI_41 "\x29"
#define MUI_42 "\x2a"
#define MUI_43 "\x2b"
#define MUI_44 "\x2c"
#define MUI_45 "\x2d"
#define MUI_46 "\x2e"
#define MUI_47 "\x2f"
#define MUI_48 "\x30"
#define MUI_49 "\x31"
#define MUI_50 "\x32"
#define MUI_51 "\x33"
#define MUI_52 "\x34"
#define MUI_53 "\x35"
#define MUI_54 "\x36"
#define MUI_55 "\x37"
#define MUI_56 "\x38"
#define MUI_57 "\x39"
#define MUI_58 "\x3a"
#define MUI_59 "\x3b"
#define MUI_60 "\x3c"
#define MUI_61 "\x3d"
#define MUI_62 "\x3e"
#define MUI_63 "\x3f"
#define MUI_64 "\x40"
#define MUI_65 "\x41"
#define MUI_66 "\x42"
#define MUI_67 "\x43"
#define MUI_68 "\x44"
#define MUI_69 "\x45"
#define MUI_70 "\x46"
#define MUI_71 "\x47"
#define MUI_72 "\x48"
#define MUI_73 "\x49"
#define MUI_74 "\x4a"
#define MUI_75 "\x4b"
#define MUI_76 "\x4c"
#define MUI_77 "\x4d"
#define MUI_78 "\x4e"
#define MUI_79 "\x4f"
#define MUI_80 "\x50"
#define MUI_81 "\x51"
#define MUI_82 "\x52"
#define MUI_83 "\x53"
#define MUI_84 "\x54"
#define MUI_85 "\x55"
#define MUI_86 "\x56"
#define MUI_87 "\x57"
#define MUI_88 "\x58"
#define MUI_89 "\x59"
#define MUI_90 "\x5a"
#define MUI_91 "\x5b"
#define MUI_92 "\x5c"
#define MUI_93 "\x5d"
#define MUI_94 "\x5e"
#define MUI_95 "\x5f"
#define MUI_96 "\x60"
#define MUI_97 "\x61"
#define MUI_98 "\x62"
#define MUI_99 "\x63"
#define MUI_100 "\x64"
#define MUI_101 "\x65"
#define MUI_102 "\x66"
#define MUI_103 "\x67"
#define MUI_104 "\x68"
#define MUI_105 "\x69"
#define MUI_106 "\x6a"
#define MUI_107 "\x6b"
#define MUI_108 "\x6c"
#define MUI_109 "\x6d"
#define MUI_110 "\x6e"
#define MUI_111 "\x6f"
#define MUI_112 "\x70"
#define MUI_113 "\x71"
#define MUI_114 "\x72"
#define MUI_115 "\x73"
#define MUI_116 "\x74"
#define MUI_117 "\x75"
#define MUI_118 "\x76"
#define MUI_119 "\x77"
#define MUI_120 "\x78"
#define MUI_121 "\x79"
#define MUI_122 "\x7a"
#define MUI_123 "\x7b"
#define MUI_124 "\x7c"
#define MUI_125 "\x7d"
#define MUI_126 "\x7e"
#define MUI_127 "\x7f"
#define MUI_128 "\x80"
#define MUI_129 "\x81"
#define MUI_130 "\x82"
#define MUI_131 "\x83"
#define MUI_132 "\x84"
#define MUI_133 "\x85"
#define MUI_134 "\x86"
#define MUI_135 "\x87"
#define MUI_136 "\x88"
#define MUI_137 "\x89"
#define MUI_138 "\x8a"
#define MUI_139 "\x8b"
#define MUI_140 "\x8c"
#define MUI_141 "\x8d"
#define MUI_142 "\x8e"
#define MUI_143 "\x8f"
#define MUI_144 "\x90"
#define MUI_145 "\x91"
#define MUI_146 "\x92"
#define MUI_147 "\x93"
#define MUI_148 "\x94"
#define MUI_149 "\x95"
#define MUI_150 "\x96"
#define MUI_151 "\x97"
#define MUI_152 "\x98"
#define MUI_153 "\x99"
#define MUI_154 "\x9a"
#define MUI_155 "\x9b"
#define MUI_156 "\x9c"
#define MUI_157 "\x9d"
#define MUI_158 "\x9e"
#define MUI_159 "\x9f"
#define MUI_160 "\xa0"
#define MUI_161 "\xa1"
#define MUI_162 "\xa2"
#define MUI_163 "\xa3"
#define MUI_164 "\xa4"
#define MUI_165 "\xa5"
#define MUI_166 "\xa6"
#define MUI_167 "\xa7"
#define MUI_168 "\xa8"
#define MUI_169 "\xa9"
#define MUI_170 "\xaa"
#define MUI_171 "\xab"
#define MUI_172 "\xac"
#define MUI_173 "\xad"
#define MUI_174 "\xae"
#define MUI_175 "\xaf"
#define MUI_176 "\xb0"
#define MUI_177 "\xb1"
#define MUI_178 "\xb2"
#define MUI_179 "\xb3"
#define MUI_180 "\xb4"
#define MUI_181 "\xb5"
#define MUI_182 "\xb6"
#define MUI_183 "\xb7"
#define MUI_184 "\xb8"
#define MUI_185 "\xb9"
#define MUI_186 "\xba"
#define MUI_187 "\xbb"
#define MUI_188 "\xbc"
#define MUI_189 "\xbd"
#define MUI_190 "\xbe"
#define MUI_191 "\xbf"
#define MUI_192 "\xc0"
#define MUI_193 "\xc1"
#define MUI_194 "\xc2"
#define MUI_195 "\xc3"
#define MUI_196 "\xc4"
#define MUI_197 "\xc5"
#define MUI_198 "\xc6"
#define MUI_199 "\xc7"
#define MUI_200 "\xc8"
#define MUI_201 "\xc9"
#define MUI_202 "\xca"
#define MUI_203 "\xcb"
#define MUI_204 "\xcc"
#define MUI_205 "\xcd"
#define MUI_206 "\xce"
#define MUI_207 "\xcf"
#define MUI_208 "\xd0"
#define MUI_209 "\xd1"
#define MUI_210 "\xd2"
#define MUI_211 "\xd3"
#define MUI_212 "\xd4"
#define MUI_213 "\xd5"
#define MUI_214 "\xd6"
#define MUI_215 "\xd7"
#define MUI_216 "\xd8"
#define MUI_217 "\xd9"
#define MUI_218 "\xda"
#define MUI_219 "\xdb"
#define MUI_220 "\xdc"
#define MUI_221 "\xdd"
#define MUI_222 "\xde"
#define MUI_223 "\xdf"
#define MUI_224 "\xe0"
#define MUI_225 "\xe1"
#define MUI_226 "\xe2"
#define MUI_227 "\xe3"
#define MUI_228 "\xe4"
#define MUI_229 "\xe5"
#define MUI_230 "\xe6"
#define MUI_231 "\xe7"
#define MUI_232 "\xe8"
#define MUI_233 "\xe9"
#define MUI_234 "\xea"
#define MUI_235 "\xeb"
#define MUI_236 "\xec"
#define MUI_237 "\xed"
#define MUI_238 "\xee"
#define MUI_239 "\xef"
#define MUI_240 "\xf0"
#define MUI_241 "\xf1"
#define MUI_242 "\xf2"
#define MUI_243 "\xf3"
#define MUI_244 "\xf4"
#define MUI_245 "\xf5"
#define MUI_246 "\xf6"
#define MUI_247 "\xf7"
#define MUI_248 "\xf8"
#define MUI_249 "\xf9"
#define MUI_250 "\xfa"
#define MUI_251 "\xfb"
#define MUI_252 "\xfc"
#define MUI_253 "\xfd"
#define MUI_254 "\xfe"
#define MUI_255 "\xff"

/* form: one id only */
#define MUI_FORM(n) "U" MUI_##n

/* style: one id only */
#define MUI_STYLE(n) "S" #n

#define MUI_AUX(id) "Z" id

#define MUI_DATA(id, text) "D" id "\xff" text "\xff"

#define MUI_XY(id, x, y) "F" id MUI_##x MUI_##y
/* button id must be two chars, but must be unique everywhere */
#define MUI_XYT(id, x,y,text) "B" id MUI_##x MUI_##y  "\xff" text "\xff"
#define MUI_XYA(id, x,y,a) "A" id MUI_##x MUI_##y  MUI_##a
#define MUI_XYAT(id, x,y,a,text) "T" id MUI_##x MUI_##y  MUI_##a "\xff" text "\xff"

#define MUI_LABEL(x,y,text) "L" MUI_##x MUI_##y "\xff" text "\xff"
#define MUI_GOTO(x,y,n,text) "G" MUI_##x MUI_##y MUI_##n "\xff" text "\xff"
#define MUI_goto(x,y,n,text) "g" MUI_##x MUI_##y MUI_##n "\xff" text "\xff"


uint8_t mui_get_fds_char(fds_t *s) MUI_NOINLINE;

uint8_t mui_fds_first_token(mui_t *ui) MUI_NOINLINE;
uint8_t mui_fds_next_token(mui_t *ui) MUI_NOINLINE;
uint8_t mui_fds_get_nth_token(mui_t *ui, uint8_t n) MUI_NOINLINE;
uint8_t mui_fds_get_token_cnt(mui_t *ui) MUI_NOINLINE;

void mui_Init(mui_t *ui, void *graphics_data, fds_t *fds, muif_t *muif_tlist, size_t muif_tcnt);
uint8_t mui_GetCurrentCursorFocusPosition(mui_t *ui) ;
void mui_Draw(mui_t *ui);
/* warning: The next function will overwrite the ui field variables like ui->arg, etc. 26 sep 2021: only ui->text is modified */
uint8_t mui_GetSelectableFieldTextOption(mui_t *ui, fds_t *fds, uint8_t nth_token);
/* warning: The next function will overwrite the ui field variables like ui->arg, etc 26 sep 2021: only ui->text is modified*/
uint8_t mui_GetSelectableFieldOptionCnt(mui_t *ui, fds_t *fds);
void mui_EnterForm(mui_t *ui, fds_t *fds, uint8_t initial_cursor_position);
void mui_LeaveForm(mui_t *ui);
uint8_t mui_GotoForm(mui_t *ui, uint8_t form_id, uint8_t initial_cursor_position);
void mui_SaveFormWithCursorPosition(mui_t *ui, uint8_t cursor_pos); /* Save current form with manully provied cursor position, Used together with mui_RestoreForm */
void mui_SaveForm(mui_t *ui);     /* Save current form+cursor position. Used together with mui_RestoreForm */
uint8_t mui_RestoreForm(mui_t *ui);        /* Restore form and cursor position, previously saved with mui_SaveForm */
void mui_SaveCursorPosition(mui_t *ui, uint8_t cursor_position);         /* stores a cursor position for use with mui_GotoFormAutoCursorPosition */
uint8_t mui_GotoFormAutoCursorPosition(mui_t *ui, uint8_t form_id);

int mui_GetCurrentFormId(mui_t *ui);    /* form id or -1 if the menu system is inactive */
void mui_NextField(mui_t *ui);
void mui_PrevField(mui_t *ui);
void mui_SendSelect(mui_t *ui);
void mui_SendSelectWithExecuteOnSelectFieldSearch(mui_t *ui);  /* use this if MUIF_EXECUTE_ON_SELECT_BUTTON is used */

void mui_SendValueIncrement(mui_t *ui);
void mui_SendValueDecrement(mui_t *ui);



#define mui_IsFormActive(ui) ((ui)->current_form_fds != NULL) 

#ifdef __cplusplus
}
#endif

#endif /* MUI_H */

