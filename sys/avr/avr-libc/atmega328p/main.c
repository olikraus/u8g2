#include <u8g2.h>
#include <util/delay.h>
#include <u8x8_avr.h>
#include <avr/power.h>

#define CS_DDR DDRB
#define CS_PORT PORTB
#define CS_BIT 2

#define DC_DDR DDRB
#define DC_PORT PORTB
#define DC_BIT 1

#define RESET_DDR DDRB
#define RESET_PORT PORTB
#define RESET_BIT 0


u8g2_t u8g2;

uint8_t
u8x8_gpio_and_delay (u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  // Re-use library for delays
  if (u8x8_avr_delay(u8x8, msg, arg_int, arg_ptr))
    return 1;

  switch (msg) {
    // called once during init phase of u8g2/u8x8
    // can be used to setup pins
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      CS_DDR |= _BV(CS_BIT);
      DC_DDR |= _BV(DC_BIT);
      RESET_DDR |= _BV(RESET_BIT);
      break;
    // CS (chip select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_CS:
      if (arg_int)
        CS_PORT |= _BV(CS_BIT);
      else
        CS_PORT &= ~_BV(CS_BIT);
      break;
    // DC (data/cmd, A0, register select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_DC:
      if (arg_int)
        DC_PORT |= _BV(DC_BIT);
      else
        DC_PORT &= ~_BV(DC_BIT);
      break;
    // Reset pin: Output level in arg_int
    case U8X8_MSG_GPIO_RESET:
      if (arg_int)
        RESET_PORT |= _BV(RESET_BIT);
      else
        RESET_PORT &= ~_BV(RESET_BIT);
      break;
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}

int main (void) {
  u8g2_Setup_sh1106_128x64_noname_f(
    &u8g2, U8G2_R0,
    u8x8_byte_avr_hw_spi,
    u8x8_gpio_and_delay
  );
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);

  while (1) {
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
    u8g2_DrawStr(&u8g2, 0, 15, "Hello!");
    u8g2_SendBuffer(&u8g2);
  }
}
