


#include <Arduino.h>
#include <SPI.h>
#include "u8x8.h"

/*
  Fontname: FreeType-Amstrad CPC extended-Medium-R-Normal--8-80-72-72-P-64-ISO10646-1
  Copyright: Copyright ruboku 2008
  Glyphs: 95/196
  BBX Build Mode: 3
*/
//#include "u8x2.h"

const uint8_t bdf_font[762] U8X8_FONT_SECTION("bdf_font") = {
  32,126,0,0,0,0,0,0,0,0,0,0,0,95,95,0,
  0,0,0,7,7,0,7,7,0,0,20,127,127,28,127,127,
  20,0,0,36,42,127,127,42,18,0,70,102,48,24,12,102,
  98,0,48,122,79,93,55,122,72,0,0,0,0,7,7,0,
  0,0,0,0,28,62,99,65,0,0,0,0,65,99,62,28,
  0,0,0,0,0,0,0,0,0,0,0,8,8,62,62,8,
  8,0,0,0,128,224,96,0,0,0,0,8,8,8,8,8,
  8,0,0,0,0,96,96,0,0,0,96,48,24,12,6,3,
  1,0,62,127,81,73,69,127,62,0,0,64,66,127,127,64,
  64,0,0,114,123,73,73,111,102,0,0,34,97,73,73,127,
  54,0,24,20,82,127,127,80,16,0,0,39,111,73,73,121,
  51,0,0,62,127,73,73,123,50,0,0,3,1,113,125,15,
  7,0,0,54,127,73,73,127,54,0,0,38,111,73,73,127,
  62,0,0,0,0,108,108,0,0,0,0,0,128,236,108,0,
  0,0,0,8,28,54,99,65,0,0,0,36,36,36,36,36,
  36,0,0,65,99,54,28,8,0,0,0,2,3,81,89,15,
  6,0,62,127,65,93,93,95,30,0,0,124,126,19,19,126,
  124,0,65,127,127,73,73,127,54,0,28,62,99,65,65,99,
  34,0,65,127,127,65,99,62,28,0,65,127,127,73,93,65,
  99,0,65,127,127,73,29,1,3,0,60,126,67,65,81,115,
  114,0,0,127,127,8,8,127,127,0,0,65,65,127,127,65,
  65,0,48,112,64,65,127,63,1,0,65,127,127,8,28,119,
  99,0,65,127,127,65,64,96,112,0,127,127,14,28,14,127,
  127,0,127,127,6,12,24,127,127,0,28,62,99,65,99,62,
  28,0,65,127,127,73,9,7,6,0,60,126,67,81,51,110,
  92,0,65,127,127,9,25,63,102,0,0,38,111,73,73,123,
  50,0,0,3,65,127,127,65,3,0,0,63,127,64,64,127,
  63,0,0,31,63,96,96,63,31,0,127,127,48,24,48,127,
  127,0,97,115,30,12,30,115,97,0,0,7,79,120,120,79,
  7,0,71,99,113,89,77,103,115,0,0,0,127,127,65,65,
  0,0,1,3,6,12,24,48,96,0,0,0,65,65,127,127,
  0,0,8,12,6,3,6,12,8,0,0,0,0,0,0,0,
  0,0,0,0,2,6,12,8,0,0,32,116,84,84,60,120,
  64,0,67,63,127,68,68,124,56,0,0,56,124,68,68,108,
  40,0,56,124,68,69,63,127,64,0,0,56,124,84,84,92,
  24,0,0,72,126,127,73,3,2,0,0,152,188,164,164,252,
  124,0,65,127,127,8,4,124,120,0,0,0,68,125,125,64,
  0,0,0,96,224,128,132,252,125,0,65,127,127,16,56,108,
  68,0,0,0,65,127,127,64,0,0,120,124,12,56,12,124,
  120,0,4,124,120,4,4,120,120,0,0,56,124,68,68,124,
  56,0,132,252,248,164,36,60,24,0,24,60,36,164,248,252,
  132,0,68,124,120,68,12,8,0,0,0,72,92,84,84,116,
  32,0,0,4,63,127,68,100,32,0,0,60,124,64,64,124,
  124,0,0,28,60,96,96,60,28,0,60,124,96,56,96,124,
  60,0,68,108,56,16,56,108,68,0,0,156,188,160,160,252,
  124,0,0,76,100,116,92,76,68,0,0,65,65,119,62,8,
  8,0,0,0,0,127,127,0,0,0,0,8,8,62,119,65,
  65,0,2,3,1,3,2,1,1,0};



uint8_t u8x8_gpio_and_delay_arduino(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      pinMode(8, OUTPUT);
      pinMode(9, OUTPUT);
      pinMode(10, OUTPUT);
      pinMode(11, OUTPUT);
      pinMode(13, OUTPUT);
      break;
  
    case U8X8_MSG_DELAY_MILLI:
      delay(arg_int);
      break;
      
    case U8X8_MSG_GPIO_DC:
	digitalWrite(9, arg_int);
	break;
	
    case U8X8_MSG_GPIO_CS:
	digitalWrite(10, arg_int);
	break;
	
    case U8X8_MSG_GPIO_RESET:
	digitalWrite(8, arg_int);
	break;
	
    case U8X8_MSG_GPIO_CLOCK:
	digitalWrite(13, arg_int);
	break;
      
    case U8X8_MSG_GPIO_DATA:
	digitalWrite(11, arg_int);
	break;
      
    default:
      return 0;
  }
  return 1;
}



uint8_t u8x8_byte_arduino_hw_spi(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t *data;
 
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:

      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
	SPI.transfer((uint8_t)*data);
	data++;
	arg_int--;
      }
      break;
    case U8X8_MSG_BYTE_INIT:
      /* disable chipselect */
      u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_disable_level);
      /* no wait required here */
      
      /* for SPI: setup correct level of the clock signal */
      digitalWrite(13, u8g2->display_info->sck_takeover_edge);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8g2, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      SPI.begin();
      SPI.setClockDivider( SPI_CLOCK_DIV2 );
      SPI.setDataMode(SPI_MODE0);
      SPI.setBitOrder(MSBFIRST);
      
      u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_enable_level);  
      u8g2->gpio_and_delay_cb(u8g2, U8X8_MSG_DELAY_NANO, u8g2->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      u8g2->gpio_and_delay_cb(u8g2, U8X8_MSG_DELAY_NANO, u8g2->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_disable_level);
      SPI.end();
      break;
    case U8X8_MSG_BYTE_SET_I2C_ADR:
      break;
    case U8X8_MSG_BYTE_SET_DEVICE:
      break;
    default:
      return 0;
  }
  return 1;
}

void u8x8_Setup_UC1701_DOGS102(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_uc1701_dogs102;
  u8x8->cad_cb = u8x8_cad_001;
  u8x8->byte_cb = u8x8_byte_8bit_sw_spi;
  u8x8->gpio_and_delay_cb = u8x8_gpio_and_delay_arduino;

  /* setup display info */
  u8x8_display_Setup(u8x8);
}

void u8x8_Setup_SSD1306_128x64_NONAME(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_ssd1306_128x64_noname;
  u8x8->cad_cb = u8x8_cad_001;
  u8x8->byte_cb = u8x8_byte_arduino_hw_spi;
  //u8x8->byte_cb = u8x8_byte_8bit_sw_spi;
  u8x8->gpio_and_delay_cb = u8x8_gpio_and_delay_arduino;
  
  /* setup display info */
  u8x8_display_Setup(u8x8);
}


u8x8_t u8g2;
uint8_t tile[8] = { 0x0f, 0x0f, 0x0f, 0x0f, 0xf0, 0xf0, 0xf0, 0xf0 };

void setup(void)
{
  //u8x8_Setup_UC1701_DOGS102(&u8g2);  
  u8x8_Setup_SSD1306_128x64_NONAME(&u8g2);
}

void loop(void)
{
  u8x8_display_Init(&u8g2);  
  //u8x8_display_SetFlipMode(&u8g2, 1);


  for(;;)
  {
    u8x8_display_ClearScreen(&u8g2);  
    u8x8_display_SetPowerSave(&u8g2, 0);
    //u8x8_display_SetContrast(&u8g2, 10);
    
    delay(500);

    u8x8_Set8x8Font(&u8g2, bdf_font);
    u8x8_Draw8x8String(&u8g2, 0, 0, "Hello gAjByCD");
    u8x8_Draw8x8String(&u8g2, 3, 1, "World B");
    
    u8x8_display_DrawTile(&u8g2, 1, 1, 1, tile);
    u8x8_display_DrawTile(&u8g2, 2, 2, 1, tile);
    u8x8_display_DrawTile(&u8g2, 3, 3, 1, tile);
    u8x8_display_DrawTile(&u8g2, 4, 4, 1, tile);
    u8x8_display_DrawTile(&u8g2, 5, 5, 1, tile);
    u8x8_display_DrawTile(&u8g2, 6, 6, 1, tile);

    delay(2000);
  }
}

