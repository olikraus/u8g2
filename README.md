[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/olikraus/u8g2) 

![https://raw.githubusercontent.com/wiki/olikraus/u8g2/ref/u8g2_logo_transparent_orange.png](https://raw.githubusercontent.com/wiki/olikraus/u8g2/ref/u8g2_logo_transparent_orange.png) 


U8g2: Library for monochrome displays, version 2

U8g2 is a monochrome graphics library for embedded devices. 
U8g2 supports monochrome OLEDs and LCDs, which include the following controllers:
SSD1305, SSD1306, SSD1309, SSD1316, SSD1320, SSD1322, SSD1325, SSD1327, 
SSD1329, SSD1606, SSD1607, SH1106, SH1107, SH1108, SH1122, T6963, RA8835, LC7981, 
PCD8544, PCF8812, HX1230, UC1601, UC1604, UC1608, UC1610, UC1611, UC1617, 
UC1638, UC1701, ST7511, ST7528, ST7565, ST7567, ST7571, ST7586, ST7588, ST75256, 
ST75320, NT7534, ST7920, IST3020, IST7920, LD7032, KS0108, KS0713, HD44102, T7932, 
SED1520, SBN1661, IL3820, MAX7219
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

---

增加了对【GP1287AI】和【GP1247AI】两款VFD显示器的支持。

![GP1287AI Logo](https://github.com/izilzty/u8g2/raw/master/doc/gp1287ai_u8g2_logo.png) 

因为SPI字节顺序问题目前仅支持U8g2，不能支持U8x8（显示为镜像），详细信息可以参考u8x8_d_gp1287ai.c文件内的说明。

如需旧版本的U8g2下载地址如下，可以支持U8x8但不能支持软件SPI。https://docs.qq.com/doc/DTlp1akJUVlFVQW5r
