[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/olikraus/u8g2) 

![https://raw.githubusercontent.com/wiki/olikraus/u8g2/ref/u8g2_logo_transparent_orange.png](https://raw.githubusercontent.com/wiki/olikraus/u8g2/ref/u8g2_logo_transparent_orange.png) 


U8g2: Library for monochrome displays, version 2

U8g2 is a monochrome graphics library for embedded devices. 
U8g2 supports monochrome OLEDs and LCDs, which include the following controllers:
SSD1305, SSD1306, SSD1309, SSD1312, SSD1316, SSD1320, SSD1322, SSD1325, SSD1327, 
SSD1329, SSD1606, SSD1607, SH1106, SH1107, SH1108, SH1122, T6963, RA8835, LC7981, 
PCD8544, PCF8812, HX1230, UC1601, UC1604, UC1608, UC1610, UC1611, UC1617, UC1638,
UC1701, ST7511, ST7528, ST7565, ST7567, ST7571, ST7586, ST7588, ST75256, ST75320, 
NT7534, ST7920, IST3020, IST7920, LD7032, KS0108, KS0713, HD44102, T7932, SED1520, 
SBN1661, IL3820, MAX7219
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

另有GP1287AI所适用的驱动板，如有需要请前往 [https://github.com/izilzty/GP1287AI_VFD_BOARD](https://github.com/izilzty/GP1287AI_VFD_BOARD) 下载。

![GP1287AI Logo](https://github.com/izilzty/u8g2/raw/master/doc/gp1287ai_u8g2_logo.png) 

在Arduino上的安装方法：
* 首先在Arduino IDE内安装原始的U8g2库。
* 打开库文件位置，默认为`C:\Users\%USERNAME%\Documents\Arduino\libraries`，如果你有更改过默认库位置请自行查找。
* 进入`U8g2`文件夹。
* 编辑`keywords.txt`，搜索`writeBufferXBM2	KEYWORD2`，在其下方分别添加`writeBufferPBM3	KEYWORD2`和`writeBufferXBM3	KEYWORD2`。此步骤可跳过，只会影响这两个关键字的高亮显示。
* 进入`U8g2\src`文件夹。
* 将文件夹内的5个文件替换为此仓库`cppsrc`文件夹内的文件。
* 编辑`MUIU8g2.h`，将2个include路径改为`clib\mui.h`和`clib\mui_u8g2.h`。
* 编辑`U8g2lib.h`，将1个include路径改为`clib\u8g2.h`，`U8x8lib.h`所在行不用更改。
* 编辑`U8x8lib.h`，将1个include路径改为`clib\u8x8.h`。
* 进入`clib`文件夹。
* 将文件夹内的全部文件替换为此仓库`csrc`文件夹内的文件。
* 编辑`u8g2.h`，去除`#define U8G2_16BIT`的注译，启用大尺寸屏幕支持。ESP等平台可以不用编辑，会自动开启。
* 退出Arduino IDE并重新打开，修改完成。修改好的库可自行打包，在其他电脑上直接安装使用。

注意，修改完成后如果U8g2库有后续更新，在更新后会恢复为原始版本，所以请不要点击更新，或将`library.properties`文件内的版本号更改为更高版本或不存在的版本消除更新提示。

C++/Arduino支持的构造函数为：

```
U8G2_GP1287AI_256X50_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);
U8G2_GP1287AI_256X50_2_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);
U8G2_GP1287AI_256X50_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);

U8G2_GP1287AI_256X50_1_2ND_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);
U8G2_GP1287AI_256X50_2_2ND_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);
U8G2_GP1287AI_256X50_F_2ND_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);

U8G2_GP1287AI_256X50_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/  13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);
U8G2_GP1287AI_256X50_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/  13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);
U8G2_GP1287AI_256X50_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/  13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);

U8G2_GP1247AI_253X63_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);
U8G2_GP1247AI_253X63_2_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);
U8G2_GP1247AI_253X63_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);

U8G2_GP1247AI_253X63_1_2ND_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);
U8G2_GP1247AI_253X63_2_2ND_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);
U8G2_GP1247AI_253X63_F_2ND_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 9);

U8G2_GP1247AI_253X63_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/  13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);
U8G2_GP1247AI_253X63_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/  13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);
U8G2_GP1247AI_253X63_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/  13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);
``` 

在Arduino上使用和其他屏幕一样，直接把构造函数粘贴到示例文件里即可点亮。

C支持的初始化函数为：
```
u8g2_Setup_gp1287ai_256x50_1(&u8g2, U8G2_R0, u8x8_byte_hw_spi, u8x8_gpio_and_delay);
u8g2_Setup_gp1287ai_256x50_2(&u8g2, U8G2_R0, u8x8_byte_hw_spi, u8x8_gpio_and_delay);
u8g2_Setup_gp1287ai_256x50_f(&u8g2, U8G2_R0, u8x8_byte_hw_spi, u8x8_gpio_and_delay);
 
u8g2_Setup_gp1247ai_253x63_1(&u8g2, U8G2_R0, u8x8_byte_hw_spi, u8x8_gpio_and_delay);
u8g2_Setup_gp1247ai_253x63_2(&u8g2, U8G2_R0, u8x8_byte_hw_spi, u8x8_gpio_and_delay);
u8g2_Setup_gp1247ai_253x63_f(&u8g2, U8G2_R0, u8x8_byte_hw_spi, u8x8_gpio_and_delay);
```

屏幕初始化后的默认亮度很低，以避免测试时意外点亮造成的烧屏，使用时请重新设置亮度：

```
setContrast();
```

如需截图请使用：

```
writeBufferXBM3();
writeBufferPBM3();
```

其他问题

* 如果你的PCB有HVEN（高压使能、高压开关）引脚，需要自己用GPIO控制开关，U8g2不进行控制。
* DC引脚未使用，不用分配引脚。
* 因为SPI字节顺序问题目前仅支持U8g2，不能支持U8x8（显示为镜像），详细信息可以参考u8x8_d_gp1287ai.c文件内的说明。
* 如需旧版本的U8g2下载地址如下，可以支持U8x8但不能支持软件SPI。https://docs.qq.com/doc/DTlp1akJUVlFVQW5r
* 移植说明：https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform
* C的初始化序列：https://github.com/olikraus/u8g2/wiki/u8g2setupc
