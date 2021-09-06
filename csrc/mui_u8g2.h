/*

  mui_u8g2.h

  Monochrome minimal user interface: Glue code between mui and u8g2.

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

*/

#ifndef MUI_U8G2_H
#define MUI_U8G2_H

/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
extern "C" {
#endif


/* helper functions */

u8g2_uint_t mui_get_x(mui_t *ui);
u8g2_uint_t mui_get_y(mui_t *ui);
u8g2_t *mui_get_U8g2(mui_t *ui);

u8g2_uint_t mui_u8g2_get_draw_button_pi_flags(mui_t *ui) MUI_NOINLINE;
void mui_u8g2_draw_button_utf(mui_t *ui, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text) MUI_NOINLINE;


/* incomplete field functions */

uint8_t mui_frame_button_bold_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t mui_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t mui_half_width_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg);
uint8_t mui_line_button_invers_select_u8g2(mui_t *ui, uint8_t msg);

/* ready to use field functions */

uint8_t mui_u8g2_draw_text(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_btn_jmp_wm_fi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_btn_jmp_w2_fi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_btn_jmp_w1_fi(mui_t *ui, uint8_t msg);

uint8_t mui_u8g2_btn_exit_wm_fi(mui_t *ui, uint8_t msg);

uint8_t mui_u8g2_u8_value_0_9_wm_mse_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_value_0_100_wm_mud_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_opt_line_wa_mse_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_opt_line_wa_mud_pi(mui_t *ui, uint8_t msg);

uint8_t mui_u8g2_u8_opt_parent_wa_mse_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_opt_child_wm_mse_pi(mui_t *ui, uint8_t msg);

uint8_t mui_u8g2_u8_chkbox_wm_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_radio_wm_pi(mui_t *ui, uint8_t msg);


uint8_t mui_u8g2_u8_char_wm_mud_pi(mui_t *ui, uint8_t msg);

#ifdef __cplusplus
}
#endif


#endif /* MUI_U8G2_H */

