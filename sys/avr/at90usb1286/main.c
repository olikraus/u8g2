#include <u8g2.h>
#include <util/delay.h>

u8g2_t u8g2;

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  uint8_t *data;

  switch(msg) {
    case U8X8_MSG_BYTE_INIT:
      // CLOCK (SCLK)
      DDRB  |=  (1 << 1);
      // DATA (MOSI)
      DDRB  |=  (1 << 2);
      // SPI2X: Double SPI Speed bit
      SPSR |= (1 << SPI2X);
      // Enable / Master
      SPCR  = ((1 << SPE) | (1 << MSTR));
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 ) {
        SPDR = (uint8_t)*data;
        while (!(SPSR & _BV(SPIF)));
        data++;
        arg_int--;
      }
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg){
    case U8X8_MSG_GPIO_AND_DELAY_INIT:  // called once during init phase of u8g2/u8x8
      DDRB |= (1<<0); // CS
      DDRB |= (1<<4); // DC
      DDRA |= (1<<7); // RESET
      break;              // can be used to setup pins
    case U8X8_MSG_DELAY_NANO:     // delay arg_int * 1 nano second
      // At 16Mhz, 1 instruction = 62.5ns, no action here.
      break;
    case U8X8_MSG_DELAY_100NANO:    // delay arg_int * 100 nano seconds
      // At 16Mhz, 1 instruction = 62.5ns, no action here.
      break;
    case U8X8_MSG_DELAY_10MICRO:    // delay arg_int * 10 micro seconds
      for(int i=0 ; i < arg_int ; i++)
        _delay_us(10);
      break;
    case U8X8_MSG_DELAY_MILLI:      // delay arg_int * 1 milli second
      for(int i=0 ; i < arg_int ; i++)
        _delay_ms(1);
      break;
    // case U8X8_MSG_DELAY_I2C:        // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
    //   break;              // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    // case U8X8_MSG_GPIO_D0:        // D0 or SPI clock pin: Output level in arg_int
    // case U8X8_MSG_GPIO_SPI_CLOCK:
    //   if(arg_int)
    //     PORTB |= (1<<1);
    //   else
    //     PORTB &= ~(1<<1);
    //   break;
    // case U8X8_MSG_GPIO_D1:        // D1 or SPI data pin: Output level in arg_int
    // //case U8X8_MSG_GPIO_SPI_DATA:
    //   if(arg_int)
    //     PORTB |= (1<<2);
    //   else
    //     PORTB &= ~(1<<2);
    //   break;
    // case U8X8_MSG_GPIO_D2:        // D2 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D3:        // D3 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D4:        // D4 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D5:        // D5 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D6:        // D6 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D7:        // D7 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_E:       // E/WR pin: Output level in arg_int
    //   break;
    case U8X8_MSG_GPIO_CS:        // CS (chip select) pin: Output level in arg_int
      if(arg_int)
        PORTB |= (1<<0);
      else
        PORTB &= ~(1<<0);
      break;
    case U8X8_MSG_GPIO_DC:        // DC (data/cmd, A0, register select) pin: Output level in arg_int
      if(arg_int)
        PORTB |= (1<<4);
      else
        PORTB &= ~(1<<4);
      break;
    case U8X8_MSG_GPIO_RESET:     // Reset pin: Output level in arg_int
      if(arg_int)
        PORTA |= (1<<7);
      else
        PORTA &= ~(1<<7);
      break;
    // case U8X8_MSG_GPIO_CS1:       // CS1 (chip select) pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_CS2:       // CS2 (chip select) pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_I2C_CLOCK:   // arg_int=0: Output low at I2C clock pin
    //   break;              // arg_int=1: Input dir with pullup high for I2C clock pin
    // case U8X8_MSG_GPIO_I2C_DATA:      // arg_int=0: Output low at I2C data pin
    //   break;              // arg_int=1: Input dir with pullup high for I2C data pin
    // case U8X8_MSG_GPIO_MENU_SELECT:
    //   u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
    //   break;
    // case U8X8_MSG_GPIO_MENU_NEXT:
    //   u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
    //   break;
    // case U8X8_MSG_GPIO_MENU_PREV:
    //   u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
    //   break;
    // case U8X8_MSG_GPIO_MENU_HOME:
    //   u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
    //   break;
    default:
      u8x8_SetGPIOResult(u8x8, 1);      // default return value
      break;
  }
  return 1;
}

int main(void)
{
  u8g2_Setup_sh1106_128x64_vcomh0_f(
    &u8g2, U8G2_R0,
    u8x8_byte_4wire_hw_spi, u8x8_gpio_and_delay
  );
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  while(1){
    for(uint8_t i=0;i<(128-20);i++){
      u8g2_ClearBuffer(&u8g2);
      u8g2_DrawBox(&u8g2, i,0,20,64);
      u8g2_SendBuffer(&u8g2);
    }
  }
}