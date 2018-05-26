#include "u8x8_avr.h"
#include <util/delay.h>

#define P_CPU_NS (1000000000UL / F_CPU)

uint8_t u8x8_byte_avr_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  uint8_t *data;

  switch(msg) {
    case U8X8_MSG_BYTE_INIT:
      DDRB  |=  (1 << 1); // CLOCK (SCLK)
      DDRB  |=  (1 << 2); // DATA (MOSI)
      SPSR |= (1 << SPI2X); // SPI2X: Double SPI Speed bit
      SPCR  = ((1 << SPE) | (1 << MSTR)); // Enable / Master
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

uint8_t u8x8_avr_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t cycles;

  switch(msg){
    case U8X8_MSG_DELAY_NANO:     // delay arg_int * 1 nano second
      // At 20Mhz, each cycle is 50ns, the call itself is slower.
      break;
    case U8X8_MSG_DELAY_100NANO:    // delay arg_int * 100 nano seconds
      // Approximate best case values...
      #define CALL_CYCLES 26UL
      #define CALC_CYCLES 4UL
      #define RETURN_CYCLES 4UL
      #define CYCLES_PER_LOOP 4UL

      cycles = (100UL * arg_int) / (P_CPU_NS * CYCLES_PER_LOOP);

      if(cycles > CALL_CYCLES + RETURN_CYCLES + CALC_CYCLES) break;

      __asm__ __volatile__ (
        "1: sbiw %0,1" "\n\t" // 2 cycles
        "brne 1b" : "=w" (cycles) : "0" (cycles) // 2 cycles
      );
      break;
    case U8X8_MSG_DELAY_10MICRO:    // delay arg_int * 10 micro seconds
      for(int i=0 ; i < arg_int ; i++)
        _delay_us(10);
      break;
    case U8X8_MSG_DELAY_MILLI:      // delay arg_int * 1 milli second
      for(int i=0 ; i < arg_int ; i++)
        _delay_ms(1);
      break;
    default:
      return 0;
  }
  return 1;
}
