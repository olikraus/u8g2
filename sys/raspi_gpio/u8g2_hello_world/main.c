/*

  main.c

  hello world example for raspberry pi

  For speedup run this example as root, either wis
    sudo ./main
  or by setting the superuser bit:
    sudo chown root:root ./main
    sudo chmod u+s ./main

*/

#include "raspi_gpio_hal.h"

#include "u8g2.h"

int main(void)
{
  u8g2_t u8g2; // a structure which will contain all the data for one display
  u8g2_Setup_sh1107_i2c_seeed_128x128_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_raspi_gpio_hal);  // init u8g2 structure
  u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, 5);
  u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, 6);
  u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
  u8g2_SetPowerSave(&u8g2, 0); // wake up display  
  u8g2_ClearBuffer(&u8g2);
  u8g2_SendBuffer(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_helvB24_tr);
  u8g2_DrawStr(&u8g2, 0, 50, "Hello");
  u8g2_DrawStr(&u8g2, 0, 100, "World!");
  u8g2_SendBuffer(&u8g2);
}