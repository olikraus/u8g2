/* u8g2_d_setup.c */
/* generated code, codebuild, u8g2 project */

#include "u8g2.h"

/* ssd1305 */
/* ssd1305 1 */
void u8g2_Setup_ssd1305_128x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1305_128x32_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1305 2 */
void u8g2_Setup_ssd1305_128x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1305_128x32_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1305 f */
void u8g2_Setup_ssd1305_128x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1305_128x32_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1305 */
/* ssd1305 1 */
void u8g2_Setup_ssd1305_i2c_128x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1305_128x32_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1305 2 */
void u8g2_Setup_ssd1305_i2c_128x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1305_128x32_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1305 f */
void u8g2_Setup_ssd1305_i2c_128x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1305_128x32_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_vcomh0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_vcomh0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_vcomh0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_i2c_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_vcomh0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_i2c_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_vcomh0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_vcomh0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sh1106 */
/* sh1106 1 */
void u8g2_Setup_sh1106_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_sh1106_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_vcomh0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sh1106 2 */
void u8g2_Setup_sh1106_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_sh1106_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_vcomh0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sh1106 f */
void u8g2_Setup_sh1106_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_sh1106_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_vcomh0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sh1106 */
/* sh1106 1 */
void u8g2_Setup_sh1106_i2c_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_vcomh0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sh1106 2 */
void u8g2_Setup_sh1106_i2c_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_vcomh0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sh1106 f */
void u8g2_Setup_sh1106_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sh1106_128x64_vcomh0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_128x32_univision_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x32_univision, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_128x32_univision_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x32_univision, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_128x32_univision_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x32_univision, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_i2c_128x32_univision_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x32_univision, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_i2c_128x32_univision_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x32_univision, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_i2c_128x32_univision_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x32_univision, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_64x48_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x48_er, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_6_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_64x48_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x48_er, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_6_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_64x48_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x48_er, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_6_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_i2c_64x48_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x48_er, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_6_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_i2c_64x48_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x48_er, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_6_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_i2c_64x48_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x48_er, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_6_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_64x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_64x32_1f_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_1f, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_64x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_64x32_1f_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_1f, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_64x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_64x32_1f_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_1f, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_i2c_64x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_i2c_64x32_1f_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_1f, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_i2c_64x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_i2c_64x32_1f_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_1f, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_i2c_64x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_noname, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_ssd1306_i2c_64x32_1f_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_64x32_1f, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_96x16_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_96x16_er, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_2_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_96x16_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_96x16_er, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_2_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_96x16_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_96x16_er, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_2_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 */
/* ssd1306 1 */
void u8g2_Setup_ssd1306_i2c_96x16_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_96x16_er, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_2_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 2 */
void u8g2_Setup_ssd1306_i2c_96x16_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_96x16_er, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_2_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1306 f */
void u8g2_Setup_ssd1306_i2c_96x16_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_96x16_er, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_2_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 */
/* ssd1309 1 */
void u8g2_Setup_ssd1309_128x64_noname2_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname2, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 2 */
void u8g2_Setup_ssd1309_128x64_noname2_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname2, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 f */
void u8g2_Setup_ssd1309_128x64_noname2_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname2, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 */
/* ssd1309 1 */
void u8g2_Setup_ssd1309_i2c_128x64_noname2_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname2, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 2 */
void u8g2_Setup_ssd1309_i2c_128x64_noname2_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname2, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 f */
void u8g2_Setup_ssd1309_i2c_128x64_noname2_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname2, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 */
/* ssd1309 1 */
void u8g2_Setup_ssd1309_128x64_noname0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 2 */
void u8g2_Setup_ssd1309_128x64_noname0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 f */
void u8g2_Setup_ssd1309_128x64_noname0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname0, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 */
/* ssd1309 1 */
void u8g2_Setup_ssd1309_i2c_128x64_noname0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 2 */
void u8g2_Setup_ssd1309_i2c_128x64_noname0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1309 f */
void u8g2_Setup_ssd1309_i2c_128x64_noname0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1309_128x64_noname0, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1325 */
/* ssd1325 1 */
void u8g2_Setup_ssd1325_nhd_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1325_nhd_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1325 2 */
void u8g2_Setup_ssd1325_nhd_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1325_nhd_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1325 f */
void u8g2_Setup_ssd1325_nhd_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1325_nhd_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1325 */
/* ssd1325 1 */
void u8g2_Setup_ssd1325_i2c_nhd_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1325_nhd_128x64, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1325 2 */
void u8g2_Setup_ssd1325_i2c_nhd_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1325_nhd_128x64, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1325 f */
void u8g2_Setup_ssd1325_i2c_nhd_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1325_nhd_128x64, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1327 */
/* ssd1327 1 */
void u8g2_Setup_ssd1327_seeed_96x96_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1327_seeed_96x96, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_12_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1327 2 */
void u8g2_Setup_ssd1327_seeed_96x96_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1327_seeed_96x96, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_12_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1327 f */
void u8g2_Setup_ssd1327_seeed_96x96_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1327_seeed_96x96, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_12_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1327 */
/* ssd1327 1 */
void u8g2_Setup_ssd1327_i2c_seeed_96x96_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1327_seeed_96x96, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_12_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1327 2 */
void u8g2_Setup_ssd1327_i2c_seeed_96x96_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1327_seeed_96x96, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_12_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1327 f */
void u8g2_Setup_ssd1327_i2c_seeed_96x96_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1327_seeed_96x96, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_12_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1329 */
/* ssd1329 1 */
void u8g2_Setup_ssd1329_128x96_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1329_128x96_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_12_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1329 2 */
void u8g2_Setup_ssd1329_128x96_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1329_128x96_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_12_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1329 f */
void u8g2_Setup_ssd1329_128x96_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1329_128x96_noname, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_12_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ld7032 */
/* ld7032 1 */
void u8g2_Setup_ld7032_60x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ld7032_60x32, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ld7032 2 */
void u8g2_Setup_ld7032_60x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ld7032_60x32, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ld7032 f */
void u8g2_Setup_ld7032_60x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ld7032_60x32, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ld7032 */
/* ld7032 1 */
void u8g2_Setup_ld7032_i2c_60x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ld7032_60x32, u8x8_cad_ld7032_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ld7032 2 */
void u8g2_Setup_ld7032_i2c_60x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ld7032_60x32, u8x8_cad_ld7032_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ld7032 f */
void u8g2_Setup_ld7032_i2c_60x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ld7032_60x32, u8x8_cad_ld7032_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_8_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 */
/* st7920 1 */
void u8g2_Setup_st7920_p_192x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 2 */
void u8g2_Setup_st7920_p_192x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 f */
void u8g2_Setup_st7920_p_192x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 */
/* st7920 1 */
void u8g2_Setup_st7920_192x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 2 */
void u8g2_Setup_st7920_192x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 f */
void u8g2_Setup_st7920_192x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 */
/* st7920 1 */
void u8g2_Setup_st7920_s_192x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_st7920_spi, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 2 */
void u8g2_Setup_st7920_s_192x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_st7920_spi, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 f */
void u8g2_Setup_st7920_s_192x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_192x32, u8x8_cad_st7920_spi, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 */
/* st7920 1 */
void u8g2_Setup_st7920_p_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 2 */
void u8g2_Setup_st7920_p_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 f */
void u8g2_Setup_st7920_p_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 */
/* st7920 1 */
void u8g2_Setup_st7920_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 2 */
void u8g2_Setup_st7920_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 f */
void u8g2_Setup_st7920_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 */
/* st7920 1 */
void u8g2_Setup_st7920_s_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_st7920_spi, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 2 */
void u8g2_Setup_st7920_s_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_st7920_spi, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* st7920 f */
void u8g2_Setup_st7920_s_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7920_128x64, u8x8_cad_st7920_spi, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ls013b7dh03 */
/* ls013b7dh03 1 */
void u8g2_Setup_ls013b7dh03_128x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ls013b7dh03_128x128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ls013b7dh03 2 */
void u8g2_Setup_ls013b7dh03_128x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ls013b7dh03_128x128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ls013b7dh03 f */
void u8g2_Setup_ls013b7dh03_128x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ls013b7dh03_128x128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* uc1701 */
/* uc1701 1 */
void u8g2_Setup_uc1701_ea_dogs102_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_ea_dogs102, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_13_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1701 2 */
void u8g2_Setup_uc1701_ea_dogs102_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_ea_dogs102, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_13_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1701 f */
void u8g2_Setup_uc1701_ea_dogs102_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_ea_dogs102, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_13_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1701 */
/* uc1701 1 */
void u8g2_Setup_uc1701_mini12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_mini12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1701 2 */
void u8g2_Setup_uc1701_mini12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_mini12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1701 f */
void u8g2_Setup_uc1701_mini12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1701_mini12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* pcd8544 */
/* pcd8544 1 */
void u8g2_Setup_pcd8544_84x48_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_pcd8544_84x48, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_11_6_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* pcd8544 2 */
void u8g2_Setup_pcd8544_84x48_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_pcd8544_84x48, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_11_6_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* pcd8544 f */
void u8g2_Setup_pcd8544_84x48_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_pcd8544_84x48, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_11_6_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* pcf8812 */
/* pcf8812 1 */
void u8g2_Setup_pcf8812_96x65_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_pcf8812_96x65, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_9_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* pcf8812 2 */
void u8g2_Setup_pcf8812_96x65_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_pcf8812_96x65, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_9_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* pcf8812 f */
void u8g2_Setup_pcf8812_96x65_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_pcf8812_96x65, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_12_9_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1604 */
/* uc1604 1 */
void u8g2_Setup_uc1604_jlx19264_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1604_jlx19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1604 2 */
void u8g2_Setup_uc1604_jlx19264_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1604_jlx19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1604 f */
void u8g2_Setup_uc1604_jlx19264_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1604_jlx19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1604 */
/* uc1604 1 */
void u8g2_Setup_uc1604_i2c_jlx19264_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1604_jlx19264, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1604 2 */
void u8g2_Setup_uc1604_i2c_jlx19264_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1604_jlx19264, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1604 f */
void u8g2_Setup_uc1604_i2c_jlx19264_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1604_jlx19264, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 */
/* uc1608 1 */
void u8g2_Setup_uc1608_erc24064_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc24064, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 2 */
void u8g2_Setup_uc1608_erc24064_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc24064, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 f */
void u8g2_Setup_uc1608_erc24064_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc24064, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 */
/* uc1608 1 */
void u8g2_Setup_uc1608_i2c_erc24064_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc24064, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 2 */
void u8g2_Setup_uc1608_i2c_erc24064_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc24064, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 f */
void u8g2_Setup_uc1608_i2c_erc24064_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc24064, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 */
/* uc1608 1 */
void u8g2_Setup_uc1608_erc240120_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc240120, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_15_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 2 */
void u8g2_Setup_uc1608_erc240120_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc240120, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_15_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 f */
void u8g2_Setup_uc1608_erc240120_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc240120, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_15_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 */
/* uc1608 1 */
void u8g2_Setup_uc1608_i2c_erc240120_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc240120, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_15_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 2 */
void u8g2_Setup_uc1608_i2c_erc240120_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc240120, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_15_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 f */
void u8g2_Setup_uc1608_i2c_erc240120_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_erc240120, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_15_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 */
/* uc1608 1 */
void u8g2_Setup_uc1608_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_240x128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 2 */
void u8g2_Setup_uc1608_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_240x128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 f */
void u8g2_Setup_uc1608_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_240x128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 */
/* uc1608 1 */
void u8g2_Setup_uc1608_i2c_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_240x128, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 2 */
void u8g2_Setup_uc1608_i2c_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_240x128, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1608 f */
void u8g2_Setup_uc1608_i2c_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1608_240x128, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1638 */
/* uc1638 1 */
void u8g2_Setup_uc1638_160x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1638_160x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1638 2 */
void u8g2_Setup_uc1638_160x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1638_160x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1638 f */
void u8g2_Setup_uc1638_160x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1638_160x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1610 */
/* uc1610 1 */
void u8g2_Setup_uc1610_ea_dogxl160_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1610_ea_dogxl160, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_13_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1610 2 */
void u8g2_Setup_uc1610_ea_dogxl160_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1610_ea_dogxl160, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_13_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1610 f */
void u8g2_Setup_uc1610_ea_dogxl160_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1610_ea_dogxl160, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_13_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1610 */
/* uc1610 1 */
void u8g2_Setup_uc1610_i2c_ea_dogxl160_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1610_ea_dogxl160, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_13_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1610 2 */
void u8g2_Setup_uc1610_i2c_ea_dogxl160_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1610_ea_dogxl160, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_13_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1610 f */
void u8g2_Setup_uc1610_i2c_ea_dogxl160_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1610_ea_dogxl160, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_13_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 */
/* uc1611 1 */
void u8g2_Setup_uc1611_ea_dogm240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogm240, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 2 */
void u8g2_Setup_uc1611_ea_dogm240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogm240, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 f */
void u8g2_Setup_uc1611_ea_dogm240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogm240, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 */
/* uc1611 1 */
void u8g2_Setup_uc1611_i2c_ea_dogm240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogm240, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 2 */
void u8g2_Setup_uc1611_i2c_ea_dogm240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogm240, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 f */
void u8g2_Setup_uc1611_i2c_ea_dogm240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogm240, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 */
/* uc1611 1 */
void u8g2_Setup_uc1611_ea_dogxl240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogxl240, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 2 */
void u8g2_Setup_uc1611_ea_dogxl240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogxl240, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 f */
void u8g2_Setup_uc1611_ea_dogxl240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogxl240, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 */
/* uc1611 1 */
void u8g2_Setup_uc1611_i2c_ea_dogxl240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogxl240, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 2 */
void u8g2_Setup_uc1611_i2c_ea_dogxl240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogxl240, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 f */
void u8g2_Setup_uc1611_i2c_ea_dogxl240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ea_dogxl240, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 */
/* uc1611 1 */
void u8g2_Setup_uc1611_ew50850_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ew50850, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_20_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 2 */
void u8g2_Setup_uc1611_ew50850_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ew50850, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_20_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 f */
void u8g2_Setup_uc1611_ew50850_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ew50850, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_20_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 */
/* uc1611 1 */
void u8g2_Setup_uc1611_i2c_ew50850_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ew50850, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_20_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 2 */
void u8g2_Setup_uc1611_i2c_ew50850_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ew50850, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_20_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1611 f */
void u8g2_Setup_uc1611_i2c_ew50850_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1611_ew50850, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_20_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 */
/* st7565 1 */
void u8g2_Setup_st7565_ea_dogm128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_ea_dogm128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_64128n_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_64128n, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_zolen_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_zolen_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_lm6059_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_lm6059, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_erc12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_erc12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_nhd_c12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_nhd_c12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 2 */
void u8g2_Setup_st7565_ea_dogm128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_ea_dogm128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_64128n_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_64128n, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_zolen_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_zolen_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_lm6059_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_lm6059, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_erc12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_erc12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_nhd_c12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_nhd_c12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 f */
void u8g2_Setup_st7565_ea_dogm128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_ea_dogm128, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_64128n_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_64128n, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_zolen_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_zolen_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_lm6059_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_lm6059, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_erc12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_erc12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_st7565_nhd_c12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_nhd_c12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 */
/* st7565 1 */
void u8g2_Setup_st7565_nhd_c12832_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_nhd_c12832, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 2 */
void u8g2_Setup_st7565_nhd_c12832_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_nhd_c12832, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 f */
void u8g2_Setup_st7565_nhd_c12832_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_nhd_c12832, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1601 */
/* uc1601 1 */
void u8g2_Setup_uc1601_128x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1601_128x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1601 2 */
void u8g2_Setup_uc1601_128x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1601_128x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1601 f */
void u8g2_Setup_uc1601_128x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1601_128x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1601 */
/* uc1601 1 */
void u8g2_Setup_uc1601_i2c_128x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1601_128x32, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1601 2 */
void u8g2_Setup_uc1601_i2c_128x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1601_128x32, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* uc1601 f */
void u8g2_Setup_uc1601_i2c_128x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_uc1601_128x32, u8x8_cad_uc16xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 */
/* st7565 1 */
void u8g2_Setup_st7565_ea_dogm132_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_ea_dogm132, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_17_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 2 */
void u8g2_Setup_st7565_ea_dogm132_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_ea_dogm132, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_17_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7565 f */
void u8g2_Setup_st7565_ea_dogm132_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7565_ea_dogm132, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_17_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7567 */
/* st7567 1 */
void u8g2_Setup_st7567_pi_132x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7567_pi_132x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_17_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7567 2 */
void u8g2_Setup_st7567_pi_132x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7567_pi_132x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_17_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7567 f */
void u8g2_Setup_st7567_pi_132x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7567_pi_132x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_17_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7567 */
/* st7567 1 */
void u8g2_Setup_st7567_jlx12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7567_jlx12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7567 2 */
void u8g2_Setup_st7567_jlx12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7567_jlx12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7567 f */
void u8g2_Setup_st7567_jlx12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7567_jlx12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7588 */
/* st7588 1 */
void u8g2_Setup_st7588_jlx12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7588_jlx12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7588 2 */
void u8g2_Setup_st7588_jlx12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7588_jlx12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7588 f */
void u8g2_Setup_st7588_jlx12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7588_jlx12864, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7588 */
/* st7588 1 */
void u8g2_Setup_st7588_i2c_jlx12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7588_jlx12864, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7588 2 */
void u8g2_Setup_st7588_i2c_jlx12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7588_jlx12864, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st7588 f */
void u8g2_Setup_st7588_i2c_jlx12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st7588_jlx12864, u8x8_cad_ssd13xx_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st75256 */
/* st75256 1 */
void u8g2_Setup_st75256_jlx256128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st75256_jlx256128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st75256 2 */
void u8g2_Setup_st75256_jlx256128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st75256_jlx256128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st75256 f */
void u8g2_Setup_st75256_jlx256128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st75256_jlx256128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st75256 */
/* st75256 1 */
void u8g2_Setup_st75256_jlx172104_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st75256_jlx172104, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_22_13_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st75256 2 */
void u8g2_Setup_st75256_jlx172104_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st75256_jlx172104, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_22_13_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* st75256 f */
void u8g2_Setup_st75256_jlx172104_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_st75256_jlx172104, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_22_13_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* nt7534 */
/* nt7534 1 */
void u8g2_Setup_nt7534_tg12864r_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_nt7534_tg12864r, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* nt7534 2 */
void u8g2_Setup_nt7534_tg12864r_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_nt7534_tg12864r, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* nt7534 f */
void u8g2_Setup_nt7534_tg12864r_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_nt7534_tg12864r, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ist3020 */
/* ist3020 1 */
void u8g2_Setup_ist3020_erc19264_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ist3020_erc19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ist3020 2 */
void u8g2_Setup_ist3020_erc19264_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ist3020_erc19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ist3020 f */
void u8g2_Setup_ist3020_erc19264_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ist3020_erc19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sbn1661 */
/* sbn1661 1 */
void u8g2_Setup_sbn1661_122x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sbn1661_122x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sbn1661 2 */
void u8g2_Setup_sbn1661_122x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sbn1661_122x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sbn1661 f */
void u8g2_Setup_sbn1661_122x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sbn1661_122x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sed1520 */
/* sed1520 1 */
void u8g2_Setup_sed1520_122x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sed1520_122x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sed1520 2 */
void u8g2_Setup_sed1520_122x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sed1520_122x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sed1520 f */
void u8g2_Setup_sed1520_122x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sed1520_122x32, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_4_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ks0108 */
/* ks0108 1 */
void u8g2_Setup_ks0108_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ks0108_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ks0108 2 */
void u8g2_Setup_ks0108_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ks0108_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ks0108 f */
void u8g2_Setup_ks0108_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ks0108_128x64, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ks0108 */
/* ks0108 1 */
void u8g2_Setup_ks0108_erm19264_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ks0108_erm19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ks0108 2 */
void u8g2_Setup_ks0108_erm19264_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ks0108_erm19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ks0108 f */
void u8g2_Setup_ks0108_erm19264_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ks0108_erm19264, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_24_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* lc7981 */
/* lc7981 1 */
void u8g2_Setup_lc7981_160x80_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_160x80, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_10_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* lc7981 2 */
void u8g2_Setup_lc7981_160x80_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_160x80, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_10_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* lc7981 f */
void u8g2_Setup_lc7981_160x80_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_160x80, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_10_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* lc7981 */
/* lc7981 1 */
void u8g2_Setup_lc7981_160x160_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_160x160, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_20_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* lc7981 2 */
void u8g2_Setup_lc7981_160x160_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_160x160, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_20_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* lc7981 f */
void u8g2_Setup_lc7981_160x160_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_160x160, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_20_20_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* lc7981 */
/* lc7981 1 */
void u8g2_Setup_lc7981_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* lc7981 2 */
void u8g2_Setup_lc7981_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* lc7981 f */
void u8g2_Setup_lc7981_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_lc7981_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 */
/* t6963 1 */
void u8g2_Setup_t6963_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 2 */
void u8g2_Setup_t6963_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 f */
void u8g2_Setup_t6963_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 */
/* t6963 1 */
void u8g2_Setup_t6963_240x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_240x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 2 */
void u8g2_Setup_t6963_240x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_240x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 f */
void u8g2_Setup_t6963_240x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_240x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 */
/* t6963 1 */
void u8g2_Setup_t6963_256x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_256x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 2 */
void u8g2_Setup_t6963_256x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_256x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 f */
void u8g2_Setup_t6963_256x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_256x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 */
/* t6963 1 */
void u8g2_Setup_t6963_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_128x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 2 */
void u8g2_Setup_t6963_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_128x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* t6963 f */
void u8g2_Setup_t6963_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_t6963_128x64, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ssd1322 */
/* ssd1322 1 */
void u8g2_Setup_ssd1322_nhd_256x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1322_nhd_256x64, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_8_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1322 2 */
void u8g2_Setup_ssd1322_nhd_256x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1322_nhd_256x64, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_8_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1322 f */
void u8g2_Setup_ssd1322_nhd_256x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1322_nhd_256x64, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_32_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1606 */
/* ssd1606 1 */
void u8g2_Setup_ssd1606_172x72_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1606_172x72, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_22_9_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1606 2 */
void u8g2_Setup_ssd1606_172x72_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1606_172x72, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_22_9_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1606 f */
void u8g2_Setup_ssd1606_172x72_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1606_172x72, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_22_9_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1607 */
/* ssd1607 1 */
void u8g2_Setup_ssd1607_200x200_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1607_200x200, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_25_25_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1607 2 */
void u8g2_Setup_ssd1607_200x200_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1607_200x200, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_25_25_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* ssd1607 f */
void u8g2_Setup_ssd1607_200x200_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1607_200x200, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_25_25_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* il3820 */
/* il3820 1 */
void u8g2_Setup_il3820_296x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_il3820_296x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_37_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_il3820_v2_296x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_il3820_v2_296x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_37_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* il3820 2 */
void u8g2_Setup_il3820_296x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_il3820_296x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_37_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_il3820_v2_296x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_il3820_v2_296x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_37_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* il3820 f */
void u8g2_Setup_il3820_296x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_il3820_296x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_37_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
void u8g2_Setup_il3820_v2_296x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_il3820_v2_296x128, u8x8_cad_011, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_37_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
/* sed1330 */
/* sed1330 1 */
void u8g2_Setup_sed1330_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sed1330_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* sed1330 2 */
void u8g2_Setup_sed1330_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sed1330_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* sed1330 f */
void u8g2_Setup_sed1330_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_sed1330_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ra8835 */
/* ra8835 1 */
void u8g2_Setup_ra8835_nhd_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ra8835_nhd_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ra8835 2 */
void u8g2_Setup_ra8835_nhd_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ra8835_nhd_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ra8835 f */
void u8g2_Setup_ra8835_nhd_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ra8835_nhd_240x128, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_30_16_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ra8835 */
/* ra8835 1 */
void u8g2_Setup_ra8835_320x240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ra8835_320x240, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_40_30_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ra8835 2 */
void u8g2_Setup_ra8835_320x240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ra8835_320x240, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_40_30_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* ra8835 f */
void u8g2_Setup_ra8835_320x240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ra8835_320x240, u8x8_cad_100, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_40_30_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* max7219 */
/* max7219 1 */
void u8g2_Setup_max7219_32x8_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_max7219_32x8, u8x8_cad_empty, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_4_1_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* max7219 2 */
void u8g2_Setup_max7219_32x8_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_max7219_32x8, u8x8_cad_empty, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_4_1_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* max7219 f */
void u8g2_Setup_max7219_32x8_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_max7219_32x8, u8x8_cad_empty, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_4_1_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* a2printer */
/* a2printer 1 */
void u8g2_Setup_a2printer_384x240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_a2printer_384x240, u8x8_cad_empty, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_48_30_1(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* a2printer 2 */
void u8g2_Setup_a2printer_384x240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_a2printer_384x240, u8x8_cad_empty, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_48_30_2(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* a2printer f */
void u8g2_Setup_a2printer_384x240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_a2printer_384x240, u8x8_cad_empty, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_48_30_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_horizontal_right_lsb, rotation);
}
/* end of generated code */
