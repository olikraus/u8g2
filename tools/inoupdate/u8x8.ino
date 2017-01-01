// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8x8setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
//U8X8_SSD1306_128X64_NONAME_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_SSD1306_128X64_NONAME_4W_HW_SPI u8x8(/* cs=*/ 6, /* dc=*/ 4, /* reset=*/ 12);	// Arduboy (DevKit)
//U8X8_SSD1306_128X64_NONAME_4W_HW_SPI u8x8(/* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6);	// Arduboy 10 (Production, Kickstarter Edition)
//U8X8_SSD1306_128X64_NONAME_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_SSD1306_128X64_NONAME_3W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* reset=*/ 8);
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 2, /* data=*/ 0, /* reset=*/ U8X8_PIN_NONE); 	      // Digispark ATTiny85
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
//U8X8_SSD1306_128X64_VCOMH0_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// same as the NONAME variant, but maximizes setContrast() range
//U8X8_SH1106_128X64_NONAME_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_SH1106_128X64_VCOMH0_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// same as the NONAME variant, but maximizes setContrast() range
//U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
//U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(/* clock=*/ 21, /* data=*/ 20, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather M0 Basic Proto + FeatherWing OLED
//U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8X8_SSD1306_64X48_ER_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered
//U8X8_SSD1306_128X64_NONAME_6800 u8x8(13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8);
//U8X8_SSD1306_128X64_NONAME_8080 u8x8(13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8);
//U8X8_SSD1322_NHD_256X64_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_SSD1322_NHD_256X64_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_SSD1325_NHD_128X64_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_SSD1325_NHD_128X64_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	
//U8X8_KS0108_128X64 u8x8(8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE); 	// Set R/W to low!
//U8X8_KS0108_ERM19264 u8x8(8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ 16, /* reset=*/  U8X8_PIN_NONE); 	// Set R/W to low!
//U8X8_UC1701_EA_DOGS102_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_UC1701_EA_DOGS102_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8X8_PCD8544_84X48_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Nokia 5110 Display
//U8X8_PCD8544_84X48_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);							// Nokia 5110 Display
//U8X8_PCF8812_96X65_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Could be also PCF8814
//U8X8_PCF8812_96X65_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);						// Could be also PCF8814
//U8X8_ST7565_EA_DOGM128_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_EA_DOGM128_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_ZOLEN_128X64_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_ZOLEN_128X64_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_LM6059_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_LM6059_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_NHD_C12832_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_NHD_C12832_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_UC1604_JLX19264_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8X8_UC1604_JLX19264_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8X8_UC1608_ERC24064_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // Due, SW SPI, ERC24064-1 Test Board 
//U8X8_UC1608_240X128_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8X8_UC1610_EA_DOGXL160_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/  U8X8_PIN_NONE);
//U8X8_UC1610_EA_DOGXL160_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/  U8X8_PIN_NONE);
//U8X8_UC1611_EA_DOGM240_2ND_HW_I2C u8x8(/* reset=*/ 8);	// Due, 2nd I2C, DOGM240 Test Board
//U8X8_UC1611_EA_DOGM240_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // SW SPI, Due DOGXL240 Test Board
//U8X8_UC1611_EA_DOGXL240_2ND_HW_I2C u8x8(/* reset=*/ 8);	// Due, 2nd I2C, DOGXL240 Test Board
//U8X8_UC1611_EA_DOGXL240_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // SW SPI, Due DOGXL240 Test Board
//U8X8_SSD1606_172X72_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// eInk/ePaper Display

