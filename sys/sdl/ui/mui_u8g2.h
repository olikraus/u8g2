/*

  mui_u8g2.h

  Monochrome minimal user interface: Glue code between mui and u8g2.

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

#ifndef mui_u8g2_h
#define mui_u8g2_h

/* border width */
#define U8G2_BTN_BW_POS 0
#define U8G2_BTN_BW_MASK 7
#define U8G2_BTN_BW0 0x00
#define U8G2_BTN_BW1 0x01
#define U8G2_BTN_BW2 0x02
#define U8G2_BTN_BW3 0x03

#define U8G2_BTN_SHADOW_POS 3
#define U8G2_BTN_SHADOW_MASK 0x18
#define U8G2_BTN_SHADOW0 0x08
#define U8G2_BTN_SHADOW1 0x10
#define U8G2_BTN_SHADOW2 0x18

/* text is displayed inverted */
#define U8G2_BTN_INV 0x20

/* horizontal center */
#define U8G2_BTN_HCENTER 0x40

/* use padding_h value as total string width */
#define U8G2_BTN_PADWIDTH 0x80


void u8g2_DrawButtonUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text);

/* incomplete field functions */

uint8_t muif_tframe_button_bold_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t muif_thalf_width_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t muif_tline_button_invers_select_u8g2(mui_t *ui, uint8_t msg);

/* ready to use field functions */

uint8_t muif_tlabel_u8g2(mui_t *ui, uint8_t msg);
uint8_t muif_tgoto_half_width_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t muif_tgoto_line_button_invers_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t muif_tinput_uint8_invers_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t muif_tsingle_line_option_invers_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t muif_tshow_option_goto_form_invers_select_u8g2(mui_t *ui, uint8_t msg);

uint8_t muif_tcheckbox_invers_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t muif_tassign_arg_go_back_invers_select_u8g2(mui_t *ui, uint8_t msg);


#endif /* mui_u8g2_h */

