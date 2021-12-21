#include <U8g2lib.h>

// GPIO chip number for character device
#define GPIO_CHIP_NUM 0
#define OLED_I2C_PIN_SCL                    11
#define OLED_I2C_PIN_SDA                    12
#define OLED_I2C_PIN_RESET                  U8X8_PIN_NONE

// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for all supported devices
static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,
/* clock=*/OLED_I2C_PIN_SCL,
/* data=*/OLED_I2C_PIN_SDA,
/* reset=*/U8X8_PIN_NONE);
// All Boards without Reset of the Display

int main(void) {
	u8g2.initI2cSw(GPIO_CHIP_NUM, OLED_I2C_PIN_SCL, OLED_I2C_PIN_SDA,
			OLED_I2C_PIN_RESET, 0);
	u8g2.begin();
	u8g2.setFont(u8g2_font_6x13_tr);            // choose a suitable font
	u8g2.drawStr(1, 18, "U8g2 on SW I2C"); // write something to the internal memory
	u8g2.sendBuffer();                // transfer internal memory to the display
	u8g2.setFont(u8g2_font_unifont_t_symbols);
	u8g2.drawGlyph(112, 56, 0x2603);
	u8g2.sendBuffer();
	u8g2.sleepMs(5000);
	u8g2.setPowerSave(1);
	u8g2.doneUserData();
}
