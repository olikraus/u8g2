#include <U8g2lib.h>

// GPIO chip number for character device
#define GPIO_CHIP_NUM 0
// SPI bus uses upper 4 bits and lower 4 bits, so 0x10 will be /dev/spidev1.0
#define SPI_BUS 0x10
#define OLED_SPI_PIN_RES            199
#define OLED_SPI_PIN_DC             198

// CS pin is controlled by linux spi driver
#define OLED_SPI_PIN_CS             U8X8_PIN_NONE

// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for all supported devices
static U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0,
/* cs=*/OLED_SPI_PIN_CS,
/* dc=*/OLED_SPI_PIN_DC,
/* reset=*/OLED_SPI_PIN_RES);
// same as the NONAME variant, but may solve the "every 2nd line skipped" problem

int main() {
	u8g2.initSpiHw(GPIO_CHIP_NUM, SPI_BUS, OLED_SPI_PIN_DC, OLED_SPI_PIN_RES,
			OLED_SPI_PIN_CS);
	u8g2.begin();
	u8g2.setFont(u8g2_font_6x13_tr);            // choose a suitable font
	u8g2.drawStr(1, 18, "U8g2 on HW SPI"); // write something to the internal memory
	u8g2.sendBuffer();                // transfer internal memory to the display
	u8g2.setFont(u8g2_font_unifont_t_symbols);
	u8g2.drawGlyph(112, 56, 0x2603);
	u8g2.sendBuffer();
	u8g2.sleepMs(5000);
	u8g2.setPowerSave(1);
	u8g2.doneSpi();
	u8g2.doneUserData();
}
