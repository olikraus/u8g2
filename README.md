![https://raw.githubusercontent.com/wiki/olikraus/u8g2/ref/u8g2_logo_transparent_orange.png](https://raw.githubusercontent.com/wiki/olikraus/u8g2/ref/u8g2_logo_transparent_orange.png) 


U8g2: Library for monochrome displays, version 2

U8g2 is a monochrome graphics library for embedded devices. 
U8g2 supports monochrome OLEDs and LCDs, which include the following controllers:
SSD1305, SSD1306, SSD1309, SSD1322, SSD1325, SSD1327, SSD1329, SSD1606, SSD1607, 
SH1106, SH1107, SH1108, SH1122, T6963, RA8835, LC7981, PCD8544, PCF8812, HX1230, 
UC1601, UC1604, UC1608, UC1610, UC1611, UC1701, ST7565, ST7567, ST7588, ST75256, NT7534, IST3020, ST7920, LD7032, KS0108, SED1520, SBN1661, IL3820, MAX7219 
(see [here](https://github.com/olikraus/u8g2/wiki/u8g2setupcpp) for a full list).

The Arduino library U8g2 can be installed from the library manager of the Arduino IDE. U8g2 also includes U8x8 library:
 * U8g2
   * Includes all graphics procedures (line/box/circle draw).
   * Supports many fonts. (Almost) no restriction on the font height.
   * Requires some memory in the microcontroller to render the display.
 * U8x8
   * Text output only (character) device.
   * Only fonts allowed with fit into a 8x8 pixel grid.
   * Writes directly to the display. No buffer in the microcontroller required.

[Setup Guide and Reference Manual](https://github.com/olikraus/u8g2/wiki)
