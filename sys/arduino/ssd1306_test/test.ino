


#include <Arduino.h>
#include <SPI.h>

#include "U8x8lib.h"


/*=============================================*/
/* display setup procedures */


u8x8_t u8x8;
uint8_t tile[8] = { 0x0f, 0x0f, 0x0f, 0x0f, 0xf0, 0xf0, 0xf0, 0xf0 };

void setup(void)
{
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(7, 0);	// default output in I2C mode for the SSD1306 test shield
  digitalWrite(9, 0);	// default output in I2C mode for the SSD1306 test shield: set the i2c adr to 0
  digitalWrite(10, 0);	// default output in I2C mode for the SSD1306 test shield
  
  //u8x8_Setup_3Wire_SW_SPI(&u8x8, u8x8_d_ssd1306_128x64_noname, 13, 11, 10, 8);
  //u8x8_Setup_4Wire_SW_SPI(&u8x8, u8x8_d_ssd1306_128x64_noname, 13, 11, 10, 9, 8);
  //u8x8_Setup_4Wire_HW_SPI(&u8x8, u8x8_d_ssd1306_128x64_noname, 10, 9, 8);

  u8x8_Setup_SSD13xx_SW_I2C(&u8x8, u8x8_d_ssd1306_128x64_noname, 13, 11, 8);
  //u8x8_Setup_8Bit_6800(&u8x8, u8x8_d_ssd1306_128x64_noname, 13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8);
  //u8x8_Setup_8Bit_8080(&u8x8, u8x8_d_ssd1306_128x64_noname, 13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8);
  
  //u8x8_Setup_4Wire_SW_SPI(&u8x8, u8x8_d_uc1701_dogs102, 13, 11, 10, 9, 8);
  //u8x8_Setup_4Wire_HW_SPI(&u8x8, u8x8_d_uc1701_dogs102, 10, 9, 8);		
}

void loop(void)
{
  u8x8_InitDisplay(&u8x8);  
  //u8x8_SetFlipMode(&u8x8, 1);

  //digitalWrite(9, 0);	// default output in I2C mode for the SSD1306 test shield: set the i2c adr to 0

  for(;;)
  {
    u8x8_ClearDisplay(&u8x8);  
    u8x8_SetPowerSave(&u8x8, 0);
    //u8x8_SetContrast(&u8x8, 10);
    
    //delay(500);

    u8x8_SetFont(&u8x8, u8x8_font_chroma48medium8_r);
    u8x8_DrawString(&u8x8, 0, 0, "Hello World");
    u8x8_DrawString(&u8x8, 3, 1, "ABCdefg");
    
    u8x8_DrawTile(&u8x8, 1, 1, 1, tile);
    u8x8_DrawTile(&u8x8, 2, 2, 1, tile);
    u8x8_DrawTile(&u8x8, 3, 3, 1, tile);
    u8x8_DrawTile(&u8x8, 4, 4, 1, tile);
    u8x8_DrawTile(&u8x8, 5, 5, 1, tile);
    u8x8_DrawTile(&u8x8, 6, 6, 1, tile);

    //delay(2000);
  }
}

