/*

  mui.h

  monochrome minimal user interface

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
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

*/
#ifndef MUI_H
#define MUI_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <string.h>
#include <sys/types.h>

#define UI_CHECK_EOFDS

#ifdef __GNUC__
#  define UI_NOINLINE __attribute__((noinline))
#else
#  define UI_NOINLINE
#endif



/*=== forward declarations ===*/
typedef struct ui_struct ui_t;
typedef struct muif_struct uif_t;


/*=== struct declarations === */

typedef uint8_t (*uif_cb)(ui_t *ui, uint8_t msg);

struct muif_struct
{
  uint8_t id0;
  uint8_t id1;
  uint8_t cflags;             // config flags e.g. UIF_CFLAG_IS_CURSOR_SELECTABLE, if so, then it will not receive any cursor/touch msgs
  uint8_t extra;
  void *data;                           // might be a pointer to a variable
  uif_cb cb;                        // callback
};

#define UIF(id,cflags,data,cb) { id[0], id[1], cflags, 0, data, cb}

#define uif_get_id0(uif) ((uif)->id0)
#define uif_get_id1(uif) ((uif)->id1)
#define uif_get_cflags(uif) ((uif)->cflags)
#define uif_get_extra(uif) ((uif)->extra)
#define uif_get_data(uif) ((uif)->data)
#define uif_get_cb(uif) ((uif)->cb)

#define UIF_MSG_NONE 0
#define UIF_MSG_DRAW 1
#define UIF_MSG_FORM_START 2
#define UIF_MSG_FORM_END 3
#define UIF_MSG_CURSOR_ENTER 4
#define UIF_MSG_CURSOR_SELECT 5
#define UIF_MSG_CURSOR_LEAVE 6
#define UIF_MSG_TOUCH_DOWN 7
#define UIF_MSG_TOUCH_UP 8


/* dynamic flags */
#define UIF_DFLAG_IS_CURSOR_FOCUS 0x01
#define UIF_DFLAG_IS_TOUCH_FOCUS 0x02
  
/* config flags */
#define UIF_CFLAG_IS_CURSOR_SELECTABLE 0x01
#define UIF_CFLAG_IS_TOUCH_SELECTABLE 0x02

typedef const char *fds_t;      // form/field definition string

uint8_t ui_get_fds_char(fds_t s);


/* must be smaller than or equal to 255 */
#define UI_MAX_TEXT_LEN 31

struct ui_struct
{
  void *graphics_data;
  fds_t root_fds;  
  
  uif_t *uif_list;
  size_t uif_cnt;
  
  fds_t current_form_fds;         // the current form, NULL if the ui is not active at the moment
  fds_t cursor_focus_fds;           // the field which has the current cursor focus, NULL if there is no current focus
  fds_t touch_focus_fds;            // the field which has touch focus

  fds_t token;             // current token position
  
  uint8_t selected_value;   // This variable is not used by the user interface but can be used by any field function
  uint8_t tmp8;
  /* current field/style variables */
  //uint8_t cursor_focus_position;        // the index of the field which has focus, can be used as last argument for ui_EnterForm
  
  uint8_t delimiter;    // outer delimiter of the text part of a field
  uint8_t cmd;          // current cmd or field (e.g. U or F)
  uint8_t id0;            // identifier of the field, manually provided or derived (G cmd has fixed id "FG")
  uint8_t id1;
  uint8_t x;               // position of the field
  uint8_t y;
  uint8_t dflags;
  uint8_t arg;          // extra argument of the field. For example the G: form is put here
  ssize_t len;          // length of the current command
  fds_t fds;             // current position, *fds = cmd
  uif_t *uif;                   // user interface field or style for the given id0 / id1
  char text[UI_MAX_TEXT_LEN+1];

  /* target  */
  fds_t tmp_fds;
  fds_t target_fds;     // used by several task functions as a return / result value
  
  /* last form and field */
  uint8_t last_form_id;
  uint8_t last_form_cursor_focus_position;
};

#define ui_IsCursorFocus(ui) ((ui)->dflags & UIF_DFLAG_IS_CURSOR_FOCUS)
#define ui_IsTouchFocus(ui) ((ui)->dflags & UIF_CFLAG_IS_TOUCH_SELECTABLE)




/*=== form string definitions ===*/

#define UI_0 "\x00"
#define UI_1 "\x01"
#define UI_2 "\x02"
#define UI_3 "\x03"
#define UI_4 "\x04"
#define UI_5 "\x05"
#define UI_6 "\x06"
#define UI_7 "\x07"
#define UI_8 "\x08"
#define UI_9 "\x09"
#define UI_10 "\x0a"
#define UI_11 "\x0b"
#define UI_12 "\x0c"
#define UI_13 "\x0d"
#define UI_14 "\x0e"
#define UI_15 "\x0f"
#define UI_16 "\x10"
#define UI_17 "\x11"
#define UI_18 "\x12"
#define UI_19 "\x13"
#define UI_20 "\x14"
#define UI_21 "\x15"
#define UI_22 "\x16"
#define UI_23 "\x17"
#define UI_24 "\x18"
#define UI_25 "\x19"
#define UI_26 "\x1a"
#define UI_27 "\x1b"
#define UI_28 "\x1c"
#define UI_29 "\x1d"
#define UI_30 "\x1e"
#define UI_31 "\x1f"
#define UI_32 "\x20"
#define UI_33 "\x21"
#define UI_34 "\x22"
#define UI_35 "\x23"
#define UI_36 "\x24"
#define UI_37 "\x25"
#define UI_38 "\x26"
#define UI_39 "\x27"
#define UI_40 "\x28"
#define UI_41 "\x29"
#define UI_42 "\x2a"
#define UI_43 "\x2b"
#define UI_44 "\x2c"
#define UI_45 "\x2d"
#define UI_46 "\x2e"
#define UI_47 "\x2f"
#define UI_48 "\x30"
#define UI_49 "\x31"
#define UI_50 "\x32"
#define UI_51 "\x33"
#define UI_52 "\x34"
#define UI_53 "\x35"
#define UI_54 "\x36"
#define UI_55 "\x37"
#define UI_56 "\x38"
#define UI_57 "\x39"
#define UI_58 "\x3a"
#define UI_59 "\x3b"
#define UI_60 "\x3c"
#define UI_61 "\x3d"
#define UI_62 "\x3e"
#define UI_63 "\x3f"
#define UI_64 "\x40"
#define UI_65 "\x41"
#define UI_66 "\x42"
#define UI_67 "\x43"
#define UI_68 "\x44"
#define UI_69 "\x45"
#define UI_70 "\x46"
#define UI_71 "\x47"
#define UI_72 "\x48"
#define UI_73 "\x49"
#define UI_74 "\x4a"
#define UI_75 "\x4b"
#define UI_76 "\x4c"
#define UI_77 "\x4d"
#define UI_78 "\x4e"
#define UI_79 "\x4f"
#define UI_80 "\x50"
#define UI_81 "\x51"
#define UI_82 "\x52"
#define UI_83 "\x53"
#define UI_84 "\x54"
#define UI_85 "\x55"
#define UI_86 "\x56"
#define UI_87 "\x57"
#define UI_88 "\x58"
#define UI_89 "\x59"
#define UI_90 "\x5a"
#define UI_91 "\x5b"
#define UI_92 "\x5c"
#define UI_93 "\x5d"
#define UI_94 "\x5e"
#define UI_95 "\x5f"
#define UI_96 "\x60"
#define UI_97 "\x61"
#define UI_98 "\x62"
#define UI_99 "\x63"
#define UI_100 "\x64"
#define UI_101 "\x65"
#define UI_102 "\x66"
#define UI_103 "\x67"
#define UI_104 "\x68"
#define UI_105 "\x69"
#define UI_106 "\x6a"
#define UI_107 "\x6b"
#define UI_108 "\x6c"
#define UI_109 "\x6d"
#define UI_110 "\x6e"
#define UI_111 "\x6f"
#define UI_112 "\x70"
#define UI_113 "\x71"
#define UI_114 "\x72"
#define UI_115 "\x73"
#define UI_116 "\x74"
#define UI_117 "\x75"
#define UI_118 "\x76"
#define UI_119 "\x77"
#define UI_120 "\x78"
#define UI_121 "\x79"
#define UI_122 "\x7a"
#define UI_123 "\x7b"
#define UI_124 "\x7c"
#define UI_125 "\x7d"
#define UI_126 "\x7e"
#define UI_127 "\x7f"
#define UI_128 "\x80"
#define UI_129 "\x81"
#define UI_130 "\x82"
#define UI_131 "\x83"
#define UI_132 "\x84"
#define UI_133 "\x85"
#define UI_134 "\x86"
#define UI_135 "\x87"
#define UI_136 "\x88"
#define UI_137 "\x89"
#define UI_138 "\x8a"
#define UI_139 "\x8b"
#define UI_140 "\x8c"
#define UI_141 "\x8d"
#define UI_142 "\x8e"
#define UI_143 "\x8f"
#define UI_144 "\x90"
#define UI_145 "\x91"
#define UI_146 "\x92"
#define UI_147 "\x93"
#define UI_148 "\x94"
#define UI_149 "\x95"
#define UI_150 "\x96"
#define UI_151 "\x97"
#define UI_152 "\x98"
#define UI_153 "\x99"
#define UI_154 "\x9a"
#define UI_155 "\x9b"
#define UI_156 "\x9c"
#define UI_157 "\x9d"
#define UI_158 "\x9e"
#define UI_159 "\x9f"
#define UI_160 "\xa0"
#define UI_161 "\xa1"
#define UI_162 "\xa2"
#define UI_163 "\xa3"
#define UI_164 "\xa4"
#define UI_165 "\xa5"
#define UI_166 "\xa6"
#define UI_167 "\xa7"
#define UI_168 "\xa8"
#define UI_169 "\xa9"
#define UI_170 "\xaa"
#define UI_171 "\xab"
#define UI_172 "\xac"
#define UI_173 "\xad"
#define UI_174 "\xae"
#define UI_175 "\xaf"
#define UI_176 "\xb0"
#define UI_177 "\xb1"
#define UI_178 "\xb2"
#define UI_179 "\xb3"
#define UI_180 "\xb4"
#define UI_181 "\xb5"
#define UI_182 "\xb6"
#define UI_183 "\xb7"
#define UI_184 "\xb8"
#define UI_185 "\xb9"
#define UI_186 "\xba"
#define UI_187 "\xbb"
#define UI_188 "\xbc"
#define UI_189 "\xbd"
#define UI_190 "\xbe"
#define UI_191 "\xbf"
#define UI_192 "\xc0"
#define UI_193 "\xc1"
#define UI_194 "\xc2"
#define UI_195 "\xc3"
#define UI_196 "\xc4"
#define UI_197 "\xc5"
#define UI_198 "\xc6"
#define UI_199 "\xc7"
#define UI_200 "\xc8"
#define UI_201 "\xc9"
#define UI_202 "\xca"
#define UI_203 "\xcb"
#define UI_204 "\xcc"
#define UI_205 "\xcd"
#define UI_206 "\xce"
#define UI_207 "\xcf"
#define UI_208 "\xd0"
#define UI_209 "\xd1"
#define UI_210 "\xd2"
#define UI_211 "\xd3"
#define UI_212 "\xd4"
#define UI_213 "\xd5"
#define UI_214 "\xd6"
#define UI_215 "\xd7"
#define UI_216 "\xd8"
#define UI_217 "\xd9"
#define UI_218 "\xda"
#define UI_219 "\xdb"
#define UI_220 "\xdc"
#define UI_221 "\xdd"
#define UI_222 "\xde"
#define UI_223 "\xdf"
#define UI_224 "\xe0"
#define UI_225 "\xe1"
#define UI_226 "\xe2"
#define UI_227 "\xe3"
#define UI_228 "\xe4"
#define UI_229 "\xe5"
#define UI_230 "\xe6"
#define UI_231 "\xe7"
#define UI_232 "\xe8"
#define UI_233 "\xe9"
#define UI_234 "\xea"
#define UI_235 "\xeb"
#define UI_236 "\xec"
#define UI_237 "\xed"
#define UI_238 "\xee"
#define UI_239 "\xef"
#define UI_240 "\xf0"
#define UI_241 "\xf1"
#define UI_242 "\xf2"
#define UI_243 "\xf3"
#define UI_244 "\xf4"
#define UI_245 "\xf5"
#define UI_246 "\xf6"
#define UI_247 "\xf7"
#define UI_248 "\xf8"
#define UI_249 "\xf9"
#define UI_250 "\xfa"
#define UI_251 "\xfb"
#define UI_252 "\xfc"
#define UI_253 "\xfd"
#define UI_254 "\xfe"
#define UI_255 "\xff"

/* form: one id only */
#define UI_FORM(n) "U" UI_##n
#define UI_LABEL(x,y,text) "L" UI_##x UI_##y "\xff" text "\xff"
#define UI_XY(id, x,y) "F" id UI_##x UI_##y
/* button id must be two chars, but must be unique everywhere */
#define UI_XYT(id, x,y,text) "B" id UI_##x UI_##y  "\xff" text "\xff"
#define UI_XYA(id, x,y,a) "A" id UI_##x UI_##y  UI_##a
#define UI_XYAT(id, x,y,a,text) "T" id UI_##x UI_##y  UI_##a "\xff" text "\xff"

#define UI_GOTO(x,y,n,text) "G" UI_##x UI_##y UI_##n "\xff" text "\xff"
#define UI_goto(x,y,n,text) "g" UI_##x UI_##y UI_##n "\xff" text "\xff"


uint8_t ui_get_fds_char(fds_t s);

uint8_t ui_fds_first_token(ui_t *ui);
uint8_t ui_fds_next_token(ui_t *ui);
uint8_t ui_fds_get_nth_token(ui_t *ui, uint8_t n);
uint8_t ui_fds_get_token_cnt(ui_t *ui);


void ui_Init(ui_t *ui, void *graphics_data, fds_t fds, uif_t *uif_list, size_t uif_cnt);
uint8_t ui_GetCurrentCursorFocusPosition(ui_t *ui);
void ui_Draw(ui_t *ui);
void ui_GetSelectableFieldTextOption(ui_t *ui, uint8_t form_id, uint8_t cursor_position, uint8_t nth_token);
void ui_EnterForm(ui_t *ui, uint8_t initial_cursor_position);
void ui_LeaveForm(ui_t *ui);
uint8_t ui_GotoForm(ui_t *ui, uint8_t form_id, uint8_t initial_cursor_position);
void ui_SaveForm(ui_t *ui);
void ui_RestoreForm(ui_t *ui);
void ui_NextField(ui_t *ui);
void ui_PrevField(ui_t *ui);
void ui_SendSelect(ui_t *ui);

#endif /* MUI_H */

