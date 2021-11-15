#include <U8g2lib.h>

// Set I2C bus and address
#define I2C_BUS 0
#define I2C_ADDRESS 0x3c * 2
// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for all supported devices
static U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/
		U8X8_PIN_NONE);

int main(void) {
	// GPIO chip doesn't matter for hardware I2C
	u8g2.initI2cHw(I2C_BUS);
	u8g2.setI2CAddress(I2C_ADDRESS);
	u8g2.begin();
	u8g2.setFont(u8g2_font_6x13_tr);            // choose a suitable font
	u8g2.drawStr(1, 18, "U8g2 on HW I2C"); // write something to the internal memory
	u8g2.sendBuffer();                // transfer internal memory to the display
	u8g2.setFont(u8g2_font_unifont_t_symbols);
	u8g2.drawGlyph(112, 56, 0x2603);
	u8g2.sendBuffer();
	u8g2.sleepMs(5000);
	u8g2.setPowerSave(1);
	u8g2.doneI2c();
	u8g2.doneUserData();
}
