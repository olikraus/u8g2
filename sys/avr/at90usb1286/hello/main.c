#include <u8g2.h>
#include <util/delay.h>
#include <u8x8_avr.h>

#define P_CPU_NS (1000000000UL / F_CPU)

u8g2_t u8g2;

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  // Re-use library for delays
  if(u8x8_avr_delay(u8x8, msg, arg_int, arg_ptr))
    return 1;

  switch(msg){
    case U8X8_MSG_GPIO_AND_DELAY_INIT:  // called once during init phase of u8g2/u8x8
      DDRB |= (1<<0); // CS
      DDRB |= (1<<4); // DC
      DDRA |= (1<<7); // RESET
      break;              // can be used to setup pins
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
    u8x8_byte_avr_hw_spi,
    u8x8_gpio_and_delay
  );
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  while(1){
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
    u8g2_DrawStr(&u8g2, 0, 15, "Hello!");
    u8g2_SendBuffer(&u8g2);
  }
}