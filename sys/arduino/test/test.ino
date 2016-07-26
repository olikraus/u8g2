


#include <Arduino.h>
#include <SPI.h>
#include "u8x8.h"




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
      digitalWrite(13, u8x8_GetSPIClockPhase(u8g2_GetU8x8(u8g2)));
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8g2, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      SPI.begin();
      SPI.setClockDivider( SPI_CLOCK_DIV2 );
      SPI.setDataMode(u8g2->display_info->spi_mode);
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
  u8x8_SetupMemory(u8x8);
}

void u8x8_Setup_SSD1306_128x64_NONAME(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_ssd1306_128x64_noname;
  u8x8->cad_cb = u8x8_cad_001;
  //u8x8->byte_cb = u8x8_byte_arduino_hw_spi;
  u8x8->byte_cb = u8x8_byte_8bit_sw_spi;
  u8x8->gpio_and_delay_cb = u8x8_gpio_and_delay_arduino;
  
  /* setup display info */
  u8x8_SetupMemory(u8x8);
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
  u8x8_InitDisplay(&u8g2);  
  //u8x8_SetFlipMode(&u8g2, 1);


  for(;;)
  {
    u8x8_ClearDisplay(&u8g2);  
    u8x8_SetPowerSave(&u8g2, 0);
    //u8x8_SetContrast(&u8g2, 10);
    
    delay(500);

    u8x8_SetFont(&u8g2, u8x8_font_chroma48medium8_r);
    u8x8_DrawString(&u8g2, 0, 0, "Hello World");
    u8x8_DrawString(&u8g2, 3, 1, "ABCdefg");
    
    u8x8_DrawTile(&u8g2, 1, 1, 1, tile);
    u8x8_DrawTile(&u8g2, 2, 2, 1, tile);
    u8x8_DrawTile(&u8g2, 3, 3, 1, tile);
    u8x8_DrawTile(&u8g2, 4, 4, 1, tile);
    u8x8_DrawTile(&u8g2, 5, 5, 1, tile);
    u8x8_DrawTile(&u8g2, 6, 6, 1, tile);

    delay(2000);
  }
}

