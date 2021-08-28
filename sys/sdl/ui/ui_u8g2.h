
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

uint8_t muif_tframe_button_bold_select_u8g2(mmui_t *ui, uint8_t msg);
uint8_t muif_thalf_width_frame_button_invers_select_u8g2(mmui_t *ui, uint8_t msg);
uint8_t muif_tline_button_invers_select_u8g2(mmui_t *ui, uint8_t msg);

/* ready to use field functions */

uint8_t muif_tlabel_u8g2(mmui_t *ui, uint8_t msg);
uint8_t muif_tgoto_half_width_frame_button_invers_select_u8g2(mmui_t *ui, uint8_t msg);
uint8_t muif_tgoto_line_button_invers_select_u8g2(mmui_t *ui, uint8_t msg);
uint8_t muif_tinput_uint8_invers_select_u8g2(mmui_t *ui, uint8_t msg);
uint8_t muif_tsingle_line_option_invers_select_u8g2(mmui_t *ui, uint8_t msg);
uint8_t muif_tshow_option_goto_form_invers_select_u8g2(mmui_t *ui, uint8_t msg);

uint8_t muif_tcheckbox_invers_select_u8g2(mmui_t *ui, uint8_t msg);
uint8_t muif_tassign_arg_go_back_invers_select_u8g2(mmui_t *ui, uint8_t msg);


#endif /* mui_u8g2_h */

