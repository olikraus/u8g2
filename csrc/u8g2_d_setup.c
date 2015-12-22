/* u8g2_d_setup.c */
/* generated code, codebuild, u8g2 project */

#include "u8g2.h"

/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_128x64_noname_1(u8g2_t *u8g2, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb, const u8g2_cb_t *rotation)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_ssd1306_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_128x64_noname_2(u8g2_t *u8g2, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb, const u8g2_cb_t *rotation)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_ssd1306_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_128x64_noname_f(u8g2_t *u8g2, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb, const u8g2_cb_t *rotation)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_ssd1306_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, rotation);
}
/* uc1701 */
/* uc1701 1 */
void u8g2_Setup_uc1701_dogs102_1(u8g2_t *u8g2, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb, const u8g2_cb_t *rotation)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_dogs102, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_uc1701_13_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, rotation);
}
/* uc1701 2 */
void u8g2_Setup_uc1701_dogs102_2(u8g2_t *u8g2, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb, const u8g2_cb_t *rotation)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_dogs102, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_uc1701_13_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, rotation);
}
/* uc1701 f */
void u8g2_Setup_uc1701_dogs102_f(u8g2_t *u8g2, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb, const u8g2_cb_t *rotation)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_dogs102, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_uc1701_13_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, rotation);
}
/* end of generated code */
