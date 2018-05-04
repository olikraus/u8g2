/*

  TrackControl.ino
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2018, olikraus@gmail.com
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

#include <Arduino.h>
#include <U8g2lib.h>
#include <string.h>
#include <stdarg.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define MENU_FONT u8g2_font_helvB18_tr
#define SUBMENU_FONT u8g2_font_helvB12_tr


/*
  158*128
  bold: 65..70
  normal: 71...76
*/
/*
const uint8_t track_font[1646] U8G2_FONT_SECTION("track_font") = 
  "\14\0\7\4\7\7\10\6\11\232|\2\2~\0~\0\0\0\0\0\6UA\273z\276\240\350\271\365\0"
  "?\200\260=\300\17\200d\17\360\3\260\327\3\374\265\36\340\17 V\17\360\7 \251\7\70\36\304{\200"
  "\220nk\326^n\355\255t\250\225z\207\225\200\204\225\204\203\205\342G~\315\315\374\310\257\271\231\237\271"
  "\231\237\271E\67sC\267\346\206n\350\205n\321K\275\320\355\377\377\377\377W\257\325\15\335V\257\352\206"
  "n\325K\335\320M\335\320\255\272\241\233\272\241\37\272\241\337\346\210<\30H\134\30'|\30\307\233\32h"
  "\233\36(\233\42\310\232(I\252,\213\311\62\13\311\70\357\367>\17\320B\17\360@\17\360O\364\0\177"
  "\200Vz\200?\200B=\300\37\340\264\36\340\7X\17\0Bc<%\243\352Y\60p\4Wlv\66"
  "DS\62M\226fy\63D\303\277\332}\65T\273\331N\355\330N\355\330N\355\330N\355\330N\15\253"
  "\35\333\251\241\332\261\235\32VK\264\34M\315\226\214\305Z\314\275\36@\260\36\300\201\36\340\201\36\240\211"
  "\36`\221\36 \231\36\340\244\36\0\235\36\300\254\36@\270\234L\1Cu\256\275\373\350\31\320B\17\20"
  "<\17@\66\17p\60\17\200.\17\240BO\324H\253\244L\351\204\357tP\27\312\244\14\312\244\14\352"
  "\240L\312\240L\252\244\14\312\244\14\312J\31\224\275T]\312\240\354\377\377\377\377\177'\203\262N\6e"
  "\235\316\311\240\314\311\240\314\311\240\314\311\234\316\311\234\216\351\62\241\22\36)\33)\321\2\61P\353<\316"
  "\3\210\216\3\61Q*\31\21\0D\236~\362\243\354\271\364\0\37\240\375\36\340\7\70\267\7\370\1\332\354"
  "\1\376\0\342\365\0\177\200\302z\270\206j?Fj\303%ZH\365a\313\324Y\316\264QR\224IU"
  "tIWTIY\64A\335\23A\336\23\71\340\363\70\342\323\70\344\263@e\263\70\346\223\70h\203\70"
  "js\70jc\70lS\60mC\70\355\63\60o\63\60o#\60\360\23\60q\23(r\63\20\363\203"
  "\267\377\377\377\377\341K\276\340\355\341\13\336\302\27\274\205/x\353n\360\346n\360\346~\354\347n\356\307"
  "~w?w\223'\16\0E|\63s\262\352\331\42b\304eZg\225o\205X\370X\247uX\210u"
  "Z\247eZW\313\264N\313\264N\313\270L\313\270L\313\264N\313j\235\226\345*.\323\262\134\225\313"
  "\264\354\377Z\305U\327\62\255\342\252Z\246U\134\245U\134\245eZ\245eZ\245eZ\245eX\246e"
  "X\26\353\254\356VH\205RH\205R\351\264L\313\304H\16\364\0B\363\0\247\363\0\345\363\0B\364"
  "H\13\5\0F\254\332.%\354y\344\216\17\220~\17\360\0\1\367\0\17`h\17\360\0\353\365\0!"
  "\361\0e\325(\17\20N\214\303JJ\264D\311\244>I\245\70\251\225\64\10\226\60H\226,\250\206("
  "\350\206$(\207\42g\207\36\247\207\32\347\207\26'x\26fx\22\207x\20\246x\16\346x\14%i"
  "\12&y\10ei\10ey\6\244i\6\244i\10\342i\250V\325P\255jU\65T{\325\372\243Z"
  "\324R\255\250\26\265P-j\241\32j\241ZS\63=Skz\244gj\246Gz\213\342\240\351\251\234"
  "\253\271\230M\351\220\17\20\224\216\311\234)jF\0G\240\367<\341\350\271\364\0?\0z=\300\17\300"
  "Z\17\360\3\70\325\3\31\340B\25\37j\37\67r\237\267\5\67\276\322*\301\262*C\301*\361JN"
  "\67\37s\42'\363\61'r\62'r\62\37\364\61'\363A\37s\62\37sj>\350\363\377\377\377\377"
  "\177\365A\237G'\364\211N\346\204>\350\204>\350dN\350dN\350\324\234\320if\301 d\302 "
  "\341\342 \337\2!\134#!\332R)Vs\61S\243\61\316\323A\310\4R\301\65z\0\363z\200A"
  "z\200?\0:=\300\37\240\264\36\340\7 \23\0H[:d\343\352\71 p\343\66l\364\2C\206"
  "dgv\67\63\64\213Vj\365l\64[\251\325l\245V\331\210\255\330([\261\21[\261\21[e#"
  "\67b+\66r#\266z\65\253vjV\355\320\320L\311\26\314\265\36\340\230\36@\215\36\0\225\36\300"
  "\240\36 \264\36 \270\236\213\311\20I_+\374;\351\31\340:\17 \64\17PF\212\204J\250tN"
  "\346T;\21\24\35j\240(\324@\21\324@\21\324@\21\324@\21\324\204\42\250\11EN\4\65\377\377"
  "\377\377\377\377w\242N\343D\235\306\211\234\306\211\62\221\23\61\221\23\61Q\246b\242\257T\221\254\221\21"
  "\241\20\372\250\316\2%Q\11\1J\227{\61\344\354\71\364\0?\200\320=\300\17\200h\17\360\3\260\230"
  "\322\241\26\12\236\24*\36\323A\236\321a\236\317\201\232\216\241\226\215\261\226L\311\226K\331\22K\351\22"
  "J\371\22I\11\23H\31\23G)\223\6\61\23\6A\23\5Q\217DQ\23\4a\23\3i\23\3q"
  "\23\2y\23\302\210\217\1\211\23\301\220\223\0\231\217\300\240\37:\11\4\373\300\317\377\377\377\377\357N\335"
  "\7~\272\223\373\300\217;\271\17\374\270\17\374\270\223\373\270\23;\271\217;\261\223\373\270\23;\355>\32"
  "\0Ki\61r\362\352\331\42\134\244UXV\253~-\342\42\255\252E\134\244UZ\304EZ\304E\134"
  "\303E\134\303E\134\303E\134\303E\134\303EZ\304\65\134\244E\134\223\213\270\346\377\377\271\204k\326\32"
  "\256\251\65\134Sk\270Fk\270Fk\264Hkj\221\326\324\42,z\253JUT&eR\347\204L"
  "\211\264<\17\20@n\244N\0L\240Y.%\354\271\264\220\17p\200\17\340r\212\321f\310\242^\247"
  "\203XFtP\346dJeUD\305e>%V:\204V\66\304V\62\4W.dG*\244G&"
  "\344G\42$H cH\34\204H\30\244X\24\344H\24#I\20$I\16dI\14dY\10\244I"
  "\10\304\71\10\343I\6\42:\254\244J\254\223\225T\211uf%Ub\235\377\253\16\353\254J\252\303:"
  "U\207uTIuXGuTIuT\11\225TG\225Pi\324\62\255\223\32i\221\232\250\201\242'"
  "j*\246r\242h\251\6\23\0\0\0\0";
*/

/*
  Fontname: track
  Copyright: unknown
  Glyphs: 12/12
  BBX Build Mode: 0
*/
const uint8_t track_font[1650] U8G2_FONT_SECTION("track_font") = 
  "\14\0\7\4\7\7\10\6\11\232|\2\2~\0~\0\0\0\0\0\6UA\273z\276\240\350\271\365\0"
  "\77\200\260=\300\17\200d\17\360\3\260\327\3\374\265\36\340\17 V\17\360\7 \251\7\70\36\304{\200"
  "\220nk\326^n\355\255t\250\225z\207\225\200\204\225\204\203\205\342G~\315\315\374\310\257\271\231\237\271"
  "\231\237\271E\67sC\267\346\206n\350\205n\321K\275\320\355\377\377\377\377W\257\325\15\335V\257\352\206"
  "n\325K\335\320M\335\320\255\272\241\233\272\241\37\272\241\337\346\210<\30H\134\30'|\30\307\233\32h"
  "\233\36(\233\42\310\232(I\252,\213\311\62\13\311\70\357\367>\17\320B\17\360@\17\360O\364\0\177"
  "\200Vz\200\77\200B=\300\37\340\264\36\340\7X\17\0Bc<%\243\352Y\60p\4Wlv\66"
  "DS\62M\226fy\63D\303\277\332}\65T\273\331N\355\330N\355\330N\355\330N\355\330N\15\253"
  "\35\333\251\241\332\261\235\32VK\264\34M\315\226\214\305Z\314\275\36@\260\36\300\201\36\340\201\36\240\211"
  "\36`\221\36 \231\36\340\244\36\0\235\36\300\254\36@\270\234L\1Cu\256\275\373\350\31\320B\17\20"
  "<\17@\66\17p\60\17\200.\17\240BO\324H\253\244L\351\204\357tP\27\312\244\14\312\244\14\352"
  "\240L\312\240L\252\244\14\312\244\14\312J\31\224\275T]\312\240\354\377\377\377\377\177'\203\262N\6e"
  "\235\316\311\240\314\311\240\314\311\240\314\311\234\316\311\234\216\351\62\241\22\36)\33)\321\2\61P\353<\316"
  "\3\210\216\3\61Q*\31\21\0D\236~\362\243\354\271\364\0\37\240\375\36\340\7\70\267\7\370\1\332\354"
  "\1\376\0\342\365\0\177\200\302z\270\206j\77Fj\303%ZH\365a\313\324Y\316\264QR\224IU"
  "tIWTIY\64A\335\23A\336\23\71\340\363\70\342\323\70\344\263@e\263\70\346\223\70h\203\70"
  "js\70jc\70lS\60mC\70\355\63\60o\63\60o#\60\360\23\60q\23(r\63\20\363\203"
  "\267\377\377\377\377\341K\276\340\355\341\13\336\302\27\274\205/x\353n\360\346n\360\346~\354\347n\356\307"
  "~w\77w\223'\16\0E|\63s\262\352\331\42b\304eZg\225o\205X\370X\247uX\210u"
  "Z\247eZW\313\264N\313\264N\313\270L\313\270L\313\264N\313j\235\226\345*.\323\262\134\225\313"
  "\264\354\377Z\305U\327\62\255\342\252Z\246U\134\245U\134\245eZ\245eZ\245eZ\245eX\246e"
  "X\26\353\254\356VH\205RH\205R\351\264L\313\304H\16\364\0B\363\0\247\363\0\345\363\0B\364"
  "H\13\5\0F\254\332.%\354y\344\216\17\220~\17\360\0\1\367\0\17`h\17\360\0\353\365\0!"
  "\361\0e\325(\17\20N\214\303JJ\264D\311\244>I\245\70\251\225\64\10\226\60H\226,\250\206("
  "\350\206$(\207\42g\207\36\247\207\32\347\207\26'x\26fx\22\207x\20\246x\16\346x\14%i"
  "\12&y\10ei\10ey\6\244i\6\244i\10\342i\250V\325P\255jU\65T{\325\372\243Z"
  "\324R\255\250\26\265P-j\241\32j\241ZS\63=Skz\244gj\246Gz\213\342\240\351\251\234"
  "\253\271\230M\351\220\17\20\224\216\311\234)jF\0G\240\367<\341\350\271\364\0\77\0z=\300\17\300"
  "Z\17\360\3\70\325\3\31\340B\25\37j\37\67r\237\267\5\67\276\322*\301\262*C\301*\361JN"
  "\67\37s\42'\363\61'r\62'r\62\37\364\61'\363A\37s\62\37sj>\350\363\377\377\377\377"
  "\177\365A\237G'\364\211N\346\204>\350\204>\350dN\350dN\350\324\234\320if\301 d\302 "
  "\341\342 \337\2!\134#!\332R)Vs\61S\243\61\316\323A\310\4R\301\65z\0\363z\200A"
  "z\200\77\0:=\300\37\240\264\36\340\7 \23\0H[:d\343\352\71 p\343\66l\364\2C\206"
  "dgv\67\63\64\213Vj\365l\64[\251\325l\245V\331\210\255\330([\261\21[\261\21[e#"
  "\67b+\66r#\266z\65\253vjV\355\320\320L\311\26\314\265\36\340\230\36@\215\36\0\225\36\300"
  "\240\36 \264\36 \270\236\213\311\20I_+\374;\351\31\340:\17 \64\17PF\212\204J\250tN"
  "\346T;\21\24\35j\240(\324@\21\324@\21\324@\21\324@\21\324\204\42\250\11EN\4\65\377\377"
  "\377\377\377\377w\242N\343D\235\306\211\234\306\211\62\221\23\61\221\23\61Q\246b\242\257T\221\254\221\21"
  "\241\20\372\250\316\2%Q\11\1J\227{\61\344\354\71\364\0\77\200\320=\300\17\200h\17\360\3\260\230"
  "\322\241\26\12\236\24*\36\323A\236\321a\236\317\201\232\216\241\226\215\261\226L\311\226K\331\22K\351\22"
  "J\371\22I\11\23H\31\23G)\223\6\61\23\6A\23\5Q\217DQ\23\4a\23\3i\23\3q"
  "\23\2y\23\302\210\217\1\211\23\301\220\223\0\231\217\300\240\37:\11\4\373\300\317\377\377\377\377\357N\335"
  "\7~\272\223\373\300\217;\271\17\374\270\17\374\270\223\373\270\23;\271\217;\261\223\373\270\23;\355>\32"
  "\0Ki\61r\362\352\331\42\134\244UXV\253~-\342\42\255\252E\134\244UZ\304EZ\304E\134"
  "\303E\134\303E\134\303E\134\303E\134\303EZ\304\65\134\244E\134\223\213\270\346\377\377\271\204k\326\32"
  "\256\251\65\134Sk\270Fk\270Fk\264Hkj\221\326\324\42,z\253JUT&eR\347\204L"
  "\211\264<\17\20@n\244N\0L\240Y.%\354\271\264\220\17p\200\17\340r\212\321f\310\242^\247"
  "\203XFtP\346dJeUD\305e>%V:\204V\66\304V\62\4W.dG*\244G&"
  "\344G\42$H cH\34\204H\30\244X\24\344H\24#I\20$I\16dI\14dY\10\244I"
  "\10\304\71\10\343I\6\42:\254\244J\254\223\225T\211uf%Ub\235\377\253\16\353\254J\252\303:"
  "U\207uTIuXGuTIuT\11\225TG\225Pi\324\62\255\223\32i\221\232\250\201\242'"
  "j*\246r\242h\251\6\23\0\0\0\0\4\377\377\0";

/*================================================================================*/

U8G2_ST75256_JLX256128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 5);  // MKR Zero, Enable U8g2 16 bit mode for this display



/*================================================================================*/

uint8_t test_i2c_address(uint8_t adr)
{
  Wire.beginTransmission(adr);
  return Wire.endTransmission(false);
}

void system_test(void)
{
  u8x8_SetFont(u8g2.getU8x8(), u8x8_font_amstrad_cpc_extended_r);
  u8x8_Draw1x2String(u8g2.getU8x8(), 0, 0, "Oli's Spur N Controller (2018)");
  u8x8_DrawString(u8g2.getU8x8(), 0, 3, "Teste I2C Weichen...");
  if ( test_i2c_address(17) == 0 )
    u8x8_DrawString(u8g2.getU8x8(), 26, 3, "Ok");
  else
    u8x8_DrawString(u8g2.getU8x8(), 26, 3, "Fehler");
    
  u8x8_DrawString(u8g2.getU8x8(), 0, 4, "Teste I2C Motortreiber...");
  if ( test_i2c_address(40) == 0 )
    u8x8_DrawString(u8g2.getU8x8(), 26, 4, "Ok");
  else
    u8x8_DrawString(u8g2.getU8x8(), 26, 4, "Fehler");
   
  /* wait for keypress */
  while( u8g2.getMenuEvent() == 0 )
    ;

  
}




void setup(void) {
  
  // MKR Zero Test Board
  u8g2.begin(/*Select=*/ 0, /*Right/Next=*/ 1, /*Left/Prev=*/ 2, /*Up=*/ 4, /*Down=*/ 3, /*Home/Cancel=*/ A6); 

  u8g2.setFontMode(1);
  
  Wire.begin();
  
  system_test();
}

/*================================================================================*/

#define LOG_BUF_LINES 7
#define LOG_BUF_LINE_LEN 32
#define LOG_FONT u8g2_font_lucasfont_alternate_tr
#define LOG_FONT_LH 9

char log_buf[LOG_BUF_LINES][LOG_BUF_LINE_LEN];
uint8_t log_end = 0;

  //u8g2_font_u8glib_4_tr

void draw_log(void)
{
  uint8_t i;
  u8g2.setFont(LOG_FONT);
  
  for( i = 0; i < LOG_BUF_LINES; i++ )
  {
    u8g2.drawStr(160, 64+i*LOG_FONT_LH, log_buf[i]);
  }
}

void scroll_log(void)
{
  uint8_t i;
  for( i = 1; i < LOG_BUF_LINES; i++ )
  {
    strcpy(log_buf[i-1], log_buf[i]);
  }
  log_buf[LOG_BUF_LINES-1][0] = '\0';
}

void log_line(const char *fmt, ...)
{
  va_list va;
  
  va_start(va, fmt);
  vsnprintf(log_buf[log_end], LOG_BUF_LINE_LEN, fmt, va);
  log_buf[log_end][LOG_BUF_LINE_LEN-1] = '\0';
  va_end(va);
  
  if ( log_end < LOG_BUF_LINES-1 )
  {
    log_end++;
  }
  else
  {
    scroll_log();
    
  }
}




/*================================================================================*/

struct menu_entry_type
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
};

struct menu_state
{
  int16_t menu_start;		/* in pixel */
  int16_t frame_position;		/* in pixel */
  uint8_t position;			/* position, array index */
};

/*
  Icon configuration
  Width and height must match the icon font size
  GAP: Space between the icons
  BGAP: Gap between the display border and the cursor.
*/
#define ICON_WIDTH 64
#define ICON_HEIGHT 64
#define ICON_GAP 8
#define ICON_BGAP 16
#define ICON_Y 64+ ICON_GAP

struct menu_entry_type menu_entry_list[] =
{
  { u8g2_font_open_iconic_other_8x_t, 66, "Weichen-Test"},
  { u8g2_font_open_iconic_embedded_8x_t, 67, "Motor Test"},
  { u8g2_font_open_iconic_embedded_8x_t, 66, "Gear Game"},
  { u8g2_font_open_iconic_embedded_8x_t, 68, "Home"},
  { u8g2_font_open_iconic_embedded_8x_t, 72, "Configuration"},
  { NULL, 0, NULL } 
};

int8_t button_event = 0;		// set this to 0, once the event has been processed

void check_button_event(void)
{
  if ( button_event == 0 )
    button_event = u8g2.getMenuEvent();
}


void main_menu_draw(struct menu_state *state)
{
  int16_t x;
  uint8_t i;
  x = state->menu_start;
  i = 0;
  while( menu_entry_list[i].icon > 0 )  
  {
    if ( x >= -ICON_WIDTH && x < u8g2.getDisplayWidth() )
    {
      u8g2.setFont(menu_entry_list[i].font);
      u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon );
    }
    i++;
    x += ICON_WIDTH + ICON_GAP;
    check_button_event();
  }
  //u8g2.drawFrame(state->frame_position-1, ICON_Y-ICON_HEIGHT-1, ICON_WIDTH+2, ICON_WIDTH+2);
  u8g2.drawFrame(state->frame_position-2, ICON_Y-ICON_HEIGHT-2, ICON_WIDTH+4, ICON_WIDTH+4);
  u8g2.drawFrame(state->frame_position-3, ICON_Y-ICON_HEIGHT-3, ICON_WIDTH+6, ICON_WIDTH+6);
  u8g2.drawFrame(state->frame_position-4, ICON_Y-ICON_HEIGHT-4, ICON_WIDTH+8, ICON_WIDTH+8);
  u8g2.drawFrame(state->frame_position-5, ICON_Y-ICON_HEIGHT-5, ICON_WIDTH+10, ICON_WIDTH+10);
  u8g2.drawFrame(state->frame_position-6, ICON_Y-ICON_HEIGHT-6, ICON_WIDTH+12, ICON_WIDTH+12);
  check_button_event();
}


void main_menu_to_right(struct menu_state *state)
{
  if ( menu_entry_list[state->position+1].font != NULL )
  {
    if ( (int16_t)state->frame_position+ 2*(int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth() )
    {
      state->position++;
      state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
    }
    else
    {
      state->position++;      
      state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
    }
  }
}

void main_menu_to_left(struct menu_state *state)
{
  if ( state->position > 0 )
  {
    if ( (int16_t)state->frame_position >= (int16_t)ICON_BGAP+(int16_t)ICON_WIDTH+ (int16_t)ICON_GAP )
    {
      state->position--;
      state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
    }    
    else
    {
      state->position--; 
      state->frame_position = ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);      
    }
  }
}


uint8_t towards_int16(int16_t *current, int16_t dest)
{
  if ( *current < dest )
  {
    (*current)++;
    return 1;
  }
  else if ( *current > dest )
  {
    (*current)--;
    return 1;
  }
  return 0;
}

uint8_t towards(struct menu_state *current, struct menu_state *destination)
{
  uint8_t r = 0;
  uint8_t i;
  for( i = 0; i < 10; i++ )
  {
    r |= towards_int16( &(current->frame_position), destination->frame_position);
    r |= towards_int16( &(current->menu_start), destination->menu_start);
  }
  return r;
}


struct menu_state current_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state destination_state = { ICON_BGAP, ICON_BGAP, 0 };

uint16_t main_menu(void) 
{
  button_event = 0;
  for(;;)
  {
    do
    {
      u8g2.firstPage();
      do
      {
	main_menu_draw(&current_state);  
	u8g2.setFont(MENU_FONT);  
	u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[destination_state.position].name))/2,u8g2.getDisplayHeight()-7);
	u8g2.print(menu_entry_list[destination_state.position].name);
	check_button_event();
	delay(20);
      } while( u8g2.nextPage() );
      if ( button_event == U8X8_MSG_GPIO_MENU_NEXT )
	main_menu_to_right(&destination_state);
      if ( button_event == U8X8_MSG_GPIO_MENU_PREV )
	main_menu_to_left(&destination_state);
      if ( button_event == U8X8_MSG_GPIO_MENU_SELECT )
      {
	button_event = 0;
	return destination_state.position;
      }
      if ( button_event > 0 )	// all known events are processed, clear event
	button_event = 0;
    } while ( towards(&current_state, &destination_state) );
  }
}


/*================================================================================*/

int8_t track_bold1_idx = -1;
int8_t track_bold2_idx = -1;
int8_t track_switch[4] = { 0, 0, 0, 0 };
int16_t track_switch_x[4][2] = { {  85, 85 },   { 9, 19 }, {   58,   58 }, { 105, 105 } };
int16_t track_switch_y[4][2] = { {    0,  16 }, { 55, 55 }, { 104, 114 }, { 114, 123 } };

/* sw=0..3, pos=0..1 */
void set_swtich(uint8_t sw, uint8_t pos)
{
  uint8_t error;
  sw &= 3;
  pos &= 1;
  Wire.beginTransmission(17);		// switch controller
  error = Wire.endTransmission(false);
  if ( error == 0 )
  {
    Wire.beginTransmission(17);		// switch controller
    Wire.write(0);
    Wire.write(sw*2+pos);
    Wire.endTransmission();
    log_line("I2C ok %d, sw=%d/%d", error, sw, pos);
  }
  else
  {
    log_line("I2C err %d, sw=%d/%d", error, sw, pos);
  }
}


void show_tracks(void)
{
  int i;
  int16_t x, y;
  u8g2.firstPage();
  do
  {
    u8g2.setFont(track_font);  
    for( i = 0; i < 6; i++ )
    {
      if ( i ==  track_bold1_idx || i ==  track_bold2_idx )
      {
	u8g2.drawGlyph(0,128, 'A'+i);
      }
      else
      {
	u8g2.drawGlyph(0,128, 'A'+6+i);
      }
    }
    
    check_button_event();
    for( i = 0; i < 4; i++ )
    {      
      x = track_switch_x[i][track_switch[i]];
      y = track_switch_y[i][track_switch[i]];
      u8g2.setDrawColor(0);
      u8g2.drawBox(x, y, 6, 6);
      
      u8g2.setDrawColor(1);
      u8g2.drawBox(x+1, y+1, 4, 4);      
    }

    /*
    for( i = 0; i < 4; i++ )
    {      
      x = track_switch_x[i][1];
      y = track_switch_y[i][1];
      u8g2.setDrawColor(0);
      u8g2.drawBox(x, y, 6, 6);
      
      u8g2.setDrawColor(1);
      u8g2.drawBox(x+1, y+1, 4, 4);      
    }
    */

    
    check_button_event();
    
    draw_log();
  } while( u8g2.nextPage() );
  
}


void track_switch_test(void)
{
  track_bold1_idx = -1;
  track_bold2_idx = -1;
  button_event = 0;
  for(;;)
  {
    show_tracks();
    if ( button_event == U8X8_MSG_GPIO_MENU_NEXT )
    {
      track_switch[3] ^=1;
      set_swtich(3, track_switch[3]);
    }
    if ( button_event == U8X8_MSG_GPIO_MENU_PREV )
    {
      track_switch[1] ^=1;
      set_swtich(1, track_switch[1]);
    }
    if ( button_event == U8X8_MSG_GPIO_MENU_UP )
    {
      track_switch[0] ^=1;
      set_swtich(0, track_switch[0]);
    }
    if ( button_event == U8X8_MSG_GPIO_MENU_DOWN )
    {
      track_switch[2] ^=1;
      set_swtich(2, track_switch[2]);
    }
    if ( button_event == U8X8_MSG_GPIO_MENU_HOME )
      return;
    button_event = 0;
  }
}

/*================================================================================*/


/* sw=0..3, pos=0..1 */
void set_motor_speed(uint8_t motor, uint8_t speed)
{
  uint8_t error;
  Wire.beginTransmission(40+motor);		// motor controller
  error = Wire.endTransmission(false);
  if ( error == 0 )
  {
    Wire.beginTransmission(40+motor);		// switch controller
    Wire.write(0);
    Wire.write(speed);
    Wire.endTransmission();
    log_line("I2C ok %d, m%d/%d", error, motor, speed);
  }
  else
  {
    log_line("I2C err %d, m%d/%d", error, motor, speed);
  }
}

uint16_t motor_noise_raw;
uint16_t motor_noise_filt;
uint16_t motor_speed_raw;	// lower is faster
uint16_t motor_speed_filt;	// lower is faster

void read_motor_data(uint8_t motor)
{
  uint8_t error;
  Wire.beginTransmission(40+motor);		// motor controller
  error = Wire.endTransmission(false);
  if ( error == 0 )
  {
    Wire.beginTransmission(40+motor);		// motor controller
    Wire.write(1);						// reset to index 0
    Wire.endTransmission();
    Wire.requestFrom(40+motor, 9);
    Wire.read();	// dummy read
    motor_noise_raw = Wire.read();
    motor_noise_raw |= (((uint16_t)Wire.read())<<8);
    motor_noise_filt = Wire.read();
    motor_noise_filt |= (((uint16_t)Wire.read())<<8);
    motor_speed_raw = Wire.read();
    motor_speed_raw |= (((uint16_t)Wire.read())<<8);
    motor_speed_filt = Wire.read();
    motor_speed_filt |= (((uint16_t)Wire.read())<<8);
  }  
}

void show_motor_set_speed(uint8_t motor, uint8_t speed)
{
  u8g2_uint_t y;
  u8g2_uint_t o = 18;
  button_event = 0;

  read_motor_data(motor);

  u8g2.firstPage();  
  do
  {
    check_button_event();
    u8g2.setFont(MENU_FONT);
    u8g2.drawBox(5, 0, 2, 128);
    u8g2.drawHLine(0, 127-speed/2, 12);    
    
    check_button_event();
    y = (uint8_t)(((uint16_t)(128-o)*(uint16_t)(127-speed/2))>>7);
    u8g2.setCursor(14, y+o);
    u8g2.print(speed);
    check_button_event();

    u8g2.setCursor(60, 20);
    u8g2.print("Motor: ");
    u8g2.print(motor);

    u8g2.setFont(SUBMENU_FONT);
    u8g2.setCursor(60, 50);
    u8g2.print("Noise: ");
    u8g2.setCursor(120, 50);
    u8g2.print(motor_noise_filt);

    u8g2.setCursor(60, 70);
    u8g2.print("Speed: ");
    u8g2.setCursor(120, 70);
    u8g2.print(motor_speed_filt);

    draw_log();
  } while( u8g2.nextPage() );
  
}

void motor_speed_test(void)
{
  uint8_t speed[6] = { 128, 128, 128, 128, 128, 128 };
  uint8_t step = 10;
  uint8_t motor = 0;

  for( motor = 0; motor < 6; motor++)
    set_motor_speed(motor, speed[motor]);      

  motor = 0;
  for(;;)
  {
    show_motor_set_speed(motor, speed[motor]);
    
    if ( button_event == U8X8_MSG_GPIO_MENU_UP )
    {
      if ( speed[motor] < 128 && speed[motor]+step > 128 )
	speed[motor] = 128;
      else if ( speed[motor] < 255-step )
	speed[motor] += step;
      else
	speed[motor] = 255;
      set_motor_speed(motor, speed[motor]);      
    }
    if ( button_event == U8X8_MSG_GPIO_MENU_DOWN )
    {
      if ( speed[motor] > 128 && speed[motor]-step < 128 )
	speed[motor] = 128;
      else if ( speed[motor] >= step )
	speed[motor] -=step;
      else
	speed[motor] = 0;
      set_motor_speed(motor, speed[motor]);      
    }
    if ( button_event == U8X8_MSG_GPIO_MENU_SELECT )
    {
      motor++;
      if ( motor >= 6 )
	motor = 0;
      set_motor_speed(motor, speed[motor]);      
    }
    if ( button_event == U8X8_MSG_GPIO_MENU_HOME )
    {
      button_event = 0;	
      return;
    }
    button_event = 0;	
  }
}

/*================================================================================*/

void loop(void)
{
  uint16_t pos;
  pos = main_menu();

  if ( pos == 0 )
    track_switch_test();
  else if ( pos == 1 )
    motor_speed_test();
    
}

