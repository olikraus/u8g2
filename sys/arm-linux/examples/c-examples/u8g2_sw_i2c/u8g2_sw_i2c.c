#include "u8g2port.h"

// GPIO chip number for character device
#define GPIO_CHIP_NUM 0
#define OLED_I2C_PIN_SCL                    11
#define OLED_I2C_PIN_SDA                    12
#define OLED_I2C_PIN_RESET                  U8X8_PIN_NONE

int main(void) {
	u8g2_t u8g2;

	// Initialization
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c,
			u8x8_arm_linux_gpio_and_delay);
	init_i2c_sw(&u8g2, GPIO_CHIP_NUM, OLED_I2C_PIN_SCL, OLED_I2C_PIN_SDA,
	OLED_I2C_PIN_RESET, 0);

	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);

	// Draw
	/* full buffer example, setup procedure ends in _f */
	u8g2_ClearBuffer(&u8g2);

	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
	u8g2_DrawStr(&u8g2, 1, 18, "U8g2 SW I2C");

	u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
	u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603);

	u8g2_SendBuffer(&u8g2);

	printf("Initialized ...\n");
	sleep_ms(5000);
	u8g2_SetPowerSave(&u8g2, 1);
	// Close and deallocate GPIO resources
	done_user_data(&u8g2);
	printf("Done\n");

	return 0;
}
