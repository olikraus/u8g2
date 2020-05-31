#include <linux-i2c.h>
#include <u8g2.h>
#include <stdio.h>

#define SSD1306_ADDR  0x3c

u8g2_t u8g2;

int main (void)
{
//	u8g2_Setup_ssd1305_i2c_128x32_noname_1(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);
//	u8g2_Setup_ssd1305_i2c_128x32_adafruit_1(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);
        u8g2_Setup_ssd1306_i2c_128x32_univision_1(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);
	
//	u8g2_Setup_ssd1305_i2c_128x32_noname_2(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);
// 	u8g2_Setup_ssd1305_i2c_128x32_adafruit_2(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);
//     u8g2_Setup_ssd1306_i2c_128x32_univision_2(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);
	
// 	u8g2_Setup_ssd1305_i2c_128x32_noname_f(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);
// 	u8g2_Setup_ssd1305_i2c_128x32_adafruit_f(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);
//     u8g2_Setup_ssd1306_i2c_128x32_univision_f(&u8g2, U8G2_R0, u8x8_byte_linux_i2c, u8x8_linux_i2c_delay);

	u8g2_SetI2CAddress(&u8g2, SSD1306_ADDR);

	u8g2_InitDisplay(&u8g2);

	u8g2_SetPowerSave(&u8g2, 0);

	u8g2_ClearBuffer(&u8g2);

	u8g2_SetFont(&u8g2, u8g2_font_smart_patrol_nbp_tr);

	u8g2_SetFontRefHeightText(&u8g2);

	u8g2_SetFontPosTop(&u8g2);

	u8g2_DrawStr(&u8g2, 0, 0, "u8g2 Linux I2C");

	u8g2_SendBuffer(&u8g2);

}


