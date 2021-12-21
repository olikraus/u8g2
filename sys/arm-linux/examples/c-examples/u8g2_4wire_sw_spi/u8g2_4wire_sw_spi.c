#include "u8g2port.h"

// GPIO chip number for character device
#define GPIO_CHIP_NUM 0

#define OLED_SPI_PIN_MOSI           15
#define OLED_SPI_PIN_SCK            14

// Use a different pin here rather than default SPI pins, which may cause issue
#define OLED_SPI_PIN_RES            199
#define OLED_SPI_PIN_DC             198
#define OLED_SPI_PIN_CS             13

int main(void) {
	u8g2_t u8g2;

	// Initialization
	u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi,
			u8x8_arm_linux_gpio_and_delay);
	init_spi_sw(&u8g2, GPIO_CHIP_NUM, OLED_SPI_PIN_DC, OLED_SPI_PIN_RES,
	OLED_SPI_PIN_MOSI, OLED_SPI_PIN_SCK, OLED_SPI_PIN_CS, 0);

	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);

	// Draw
	/* full buffer example, setup procedure ends in _f */
	u8g2_ClearBuffer(&u8g2);

	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
	u8g2_DrawStr(&u8g2, 1, 18, "U8g2 SW SPI");

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
