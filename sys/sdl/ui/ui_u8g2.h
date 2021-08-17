
#ifndef ui_u8g2_h
#define ui_u8g2_h

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


void nu8g2_DrawButtonUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text);

uint8_t uif_label_u8g2(ui_t *ui, uint8_t msg);
uint8_t uif_frame_button_bold_select_u8g2(ui_t *ui, uint8_t msg);
uint8_t uif_frame_button_invers_select_u8g2(ui_t *ui, uint8_t msg);
uint8_t uif_line_button_invers_select_u8g2(ui_t *ui, uint8_t msg);

uint8_t uif_goto_frame_button_invers_select_u8g2(ui_t *ui, uint8_t msg);
uint8_t uif_goto_line_button_invers_select_u8g2(ui_t *ui, uint8_t msg);

uint8_t uif_input_uint8_invers_select_u8g2(ui_t *ui, uint8_t msg);
uint8_t uif_single_line_option_invers_select_u8g2(ui_t *ui, uint8_t msg);

#endif /* ui_u8g2_h */