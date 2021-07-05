
#include "raspi_gpio_hal.h"

#include "u8g2.h"

int main(void)
{
  u8g2_t u8g2; // a structure which will contain all the data for one display
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_raspi_gpio_hal);  // init u8g2 structure
  u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
  u8g2_SetPowerSave(&u8g2, 0); // wake up display  
  u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
  u8g2_DrawStr(&u8g2, 0, 20, "Hello World!");
  u8g2_SendBuffer(&u8g2);
}