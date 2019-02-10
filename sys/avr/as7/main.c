/*

  main.c
	
  AVR Test Project
  This project will use 4-Wire SW SPI

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2018, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
	
*/


#include <avr/io.h>
#include <u8g2.h>
#include <util/delay.h>

#define DISPLAY_CLK_DIR DDRB
#define DISPLAY_CLK_PORT PORTB
#define DISPLAY_CLK_PIN 5

#define DISPLAY_DATA_DIR DDRB
#define DISPLAY_DATA_PORT PORTB
#define DISPLAY_DATA_PIN 3

#define DISPLAY_CS_DIR DDRB
#define DISPLAY_CS_PORT PORTB
#define DISPLAY_CS_PIN 2

#define DISPLAY_DC_DIR DDRB
#define DISPLAY_DC_PORT PORTB
#define DISPLAY_DC_PIN 1

#define DISPLAY_RESET_DIR DDRB
#define DISPLAY_RESET_PORT PORTB
#define DISPLAY_RESET_PIN 0




#define P_CPU_NS (1000000000UL / F_CPU)

u8g2_t u8g2;

uint8_t u8x8_avr_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint8_t cycles;

	switch(msg)
	{
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

			if(cycles > CALL_CYCLES + RETURN_CYCLES + CALC_CYCLES) 
				break;

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


uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	// Re-use library for delays

	switch(msg)
	{
		case U8X8_MSG_GPIO_AND_DELAY_INIT:  // called once during init phase of u8g2/u8x8
			DISPLAY_CLK_DIR |= 1<<DISPLAY_CLK_PIN;
			DISPLAY_DATA_DIR |= 1<<DISPLAY_DATA_PIN;
			DISPLAY_CS_DIR |= 1<<DISPLAY_CS_PIN;
			DISPLAY_DC_DIR |= 1<<DISPLAY_DC_PIN;
			DISPLAY_RESET_DIR |= 1<<DISPLAY_RESET_PIN;
			break;              // can be used to setup pins
		case U8X8_MSG_GPIO_SPI_CLOCK:        // Clock pin: Output level in arg_int
			if(arg_int)
				DISPLAY_CLK_PORT |= (1<<DISPLAY_CLK_PIN);
			else
				DISPLAY_CLK_PORT &= ~(1<<DISPLAY_CLK_PIN);
			break;
		case U8X8_MSG_GPIO_SPI_DATA:        // MOSI pin: Output level in arg_int
			if(arg_int)
				DISPLAY_DATA_PORT |= (1<<DISPLAY_DATA_PIN);
			else
				DISPLAY_DATA_PORT &= ~(1<<DISPLAY_DATA_PIN);
			break;
		case U8X8_MSG_GPIO_CS:        // CS (chip select) pin: Output level in arg_int
			if(arg_int)
				DISPLAY_CS_PORT |= (1<<DISPLAY_CS_PIN);
			else
				DISPLAY_CS_PORT &= ~(1<<DISPLAY_CS_PIN);
			break;
		case U8X8_MSG_GPIO_DC:        // DC (data/cmd, A0, register select) pin: Output level in arg_int
			if(arg_int)
				DISPLAY_DC_PORT |= (1<<DISPLAY_DC_PIN);
			else
				DISPLAY_DC_PORT &= ~(1<<DISPLAY_DC_PIN);
			break;
		
		case U8X8_MSG_GPIO_RESET:     // Reset pin: Output level in arg_int
			if(arg_int)
				DISPLAY_RESET_PORT |= (1<<DISPLAY_RESET_PIN);
			else
				DISPLAY_RESET_PORT &= ~(1<<DISPLAY_RESET_PIN);
			break;
		default:
			if (u8x8_avr_delay(u8x8, msg, arg_int, arg_ptr))	// check for any delay msgs
				return 1;
			u8x8_SetGPIOResult(u8x8, 1);      // default return value
			break;
	}
	return 1;
}

int main(void)
{
	/*
		Select a setup procedure for your display from here: https://github.com/olikraus/u8g2/wiki/u8g2setupc
		1. Arg: Address of an empty u8g2 structure
		2. Arg: Usually U8G2_R0, others are listed here: https://github.com/olikraus/u8g2/wiki/u8g2reference#carduino-example
		3. Arg: Protocol procedure (u8g2-byte), list is here: https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform#communication-callback-eg-u8x8_byte_hw_i2c
		4. Arg: Defined in this code itself (see above)
	*/
	u8g2_Setup_st7565_ea_dogm132_f( &u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_avr_gpio_and_delay );
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	
	/* full buffer example, setup procedure ends in _f */
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
	u8g2_DrawStr(&u8g2, 1, 18, "U8g2 on AVR");
	u8g2_SendBuffer(&u8g2);
		
	while(1){
	}
}
