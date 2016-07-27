/*

  U8x8lib.cpp
  
  Arduino specific low level functions


  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
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


#include "U8x8lib.h"
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif 
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*=============================================*/
/* callbacks */

extern "C" uint8_t u8x8_gpio_and_delay_arduino(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t i;
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
    
      for( i = 0; i < U8X8_PIN_CNT; i++ )
	if ( u8x8->pins[i] != U8X8_PIN_NONE )
	{
	  if ( i < U8X8_PIN_OUTPUT_CNT )
	  {
	    pinMode(u8x8->pins[i], OUTPUT);
	  }
	  else
	  {
#ifdef INPUT_PULLUP
	    pinMode(u8x8->pins[i], INPUT_PULLUP);
#else
	    pinMode(u8x8->pins[i], OUTPUT);
	    digitalWrite(u8x8->pins[i], 1);
#endif 
	  }
	}
	  
      break;
  
    case U8X8_MSG_DELAY_MILLI:
      delay(arg_int);
      break;
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
      delayMicroseconds(arg_int<=2?5:2);
      break;
    case U8X8_MSG_GPIO_I2C_CLOCK:
    case U8X8_MSG_GPIO_I2C_DATA:
      if ( arg_int == 0 )
      {
	pinMode(u8x8_GetPinValue(u8x8, msg), OUTPUT);
	digitalWrite(u8x8_GetPinValue(u8x8, msg), 0);
      }
      else
      {
#ifdef INPUT_PULLUP
	pinMode(u8x8_GetPinValue(u8x8, msg), INPUT_PULLUP);
#else
	pinMode(u8x8_GetPinValue(u8x8, msg), OUTPUT);
	digitalWrite(u8x8_GetPinValue(u8x8, msg), 1);
#endif 
      }
      break;
    default:
      if ( msg >= U8X8_MSG_GPIO(0) )
      {
	i = u8x8_GetPinValue(u8x8, msg);
	if ( i != U8X8_PIN_NONE )
	{
	  if ( u8x8_GetPinIndex(u8x8, msg) < U8X8_PIN_OUTPUT_CNT )
	  {
	    digitalWrite(i, arg_int);
	  }
	  else
	  {
	    u8x8_SetGPIOResult(u8x8, digitalRead(i) == 0 ? 0 : 1);
	  }
	}
	break;
      }
      
      return 0;
  }
  return 1;
}


/*=============================================*/

/*
  replacement for a more faster u8x8_byte_4wire_sw_spi
  in general u8x8_byte_4wire_sw_spi could be a fallback:

  uint8_t u8x8_byte_arduino_4wire_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
  {
    return u8x8_byte_4wire_sw_spi(u8x8, msg,arg_int, arg_ptr);
  }



*/

#ifndef __AVR_ARCH__
#define __AVR_ARCH__ 0
#endif 

#if !defined(U8X8_USE_PINS)
  /* no pin information (very strange), so fallback */
  uint8_t u8x8_byte_arduino_4wire_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
  {
    return u8x8_byte_4wire_sw_spi(u8x8, msg,arg_int, arg_ptr);
  }

#elif __AVR_ARCH__ == 4 || __AVR_ARCH__ == 5 || __AVR_ARCH__ == 51 || __AVR_ARCH__ == 6

/* this function completly replaces u8x8_byte_4wire_sw_spi*/
extern "C" uint8_t u8x8_byte_arduino_4wire_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t i, b;
  uint8_t *data;
  uint8_t takeover_edge = u8x8_GetSPIClockPhase(u8x8);
  uint8_t not_takeover_edge = 1 - takeover_edge;

  /* the following static vars are recalculated in U8X8_MSG_BYTE_START_TRANSFER */
  /* so, it should be possible to used multiple displays with different pins */
  
  static volatile uint8_t *arduino_clock_port;
  static volatile uint8_t arduino_clock_mask;
  static volatile uint8_t arduino_clock_n_mask;
  
  static volatile uint8_t *arduino_data_port;
  static volatile uint8_t arduino_data_mask;
  static volatile uint8_t arduino_data_n_mask;


  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
	b = *data;
	data++;
	arg_int--;
	for( i = 0; i < 8; i++ )
	{
	  if ( b & 128 )
	    *arduino_data_port |= arduino_data_mask;
	  else
	    *arduino_data_port &= arduino_data_n_mask;
	  b <<= 1;

	  if ( not_takeover_edge != 0 )
	    *arduino_clock_port |= arduino_clock_mask;
	  else
	    *arduino_clock_port &= arduino_clock_n_mask;
		
	  /* AVR Architecture is very slow, extra call is not required */
	  //u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->sda_setup_time_ns);
	  
	  if ( takeover_edge != 0 )
	    *arduino_clock_port |= arduino_clock_mask;
	  else
	    *arduino_clock_port &= arduino_clock_n_mask;
	  
	  /* AVR Architecture is very slow, extra call is not required */
	  //u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->sck_pulse_width_ns);
	}    
      }
      break;
      
    case U8X8_MSG_BYTE_INIT:
      /* disable chipselect */
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      /* no wait required here */
      
      /* for SPI: setup correct level of the clock signal */
      u8x8_gpio_SetSPIClock(u8x8, u8x8_GetSPIClockPhase(u8x8));
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);

      /* there is no consistency checking for u8x8->pins[U8X8_PIN_SPI_CLOCK] */
    
      arduino_clock_port = portOutputRegister(digitalPinToPort(u8x8->pins[U8X8_PIN_SPI_CLOCK]));
      arduino_clock_mask = digitalPinToBitMask(u8x8->pins[U8X8_PIN_SPI_CLOCK]);
      arduino_clock_n_mask = ~arduino_clock_mask;

      /* there is no consistency checking for u8x8->pins[U8X8_PIN_SPI_DATA] */

      arduino_data_port = portOutputRegister(digitalPinToPort(u8x8->pins[U8X8_PIN_SPI_DATA]));
      arduino_data_mask = digitalPinToBitMask(u8x8->pins[U8X8_PIN_SPI_DATA]);
      arduino_data_n_mask = ~arduino_data_mask;
    
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

#else
  /* fallback */
  uint8_t u8x8_byte_arduino_4wire_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
  {
    return u8x8_byte_4wire_sw_spi(u8x8, msg,arg_int, arg_ptr);
  }
  
#endif


/*=============================================*/


extern "C" uint8_t u8x8_byte_arduino_hw_spi(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
#ifdef U8X8_HAVE_HW_SPI
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
      pinMode(11, OUTPUT);
      pinMode(13, OUTPUT);
      digitalWrite(13, u8x8_GetSPIClockPhase(u8g2_GetU8x8(u8g2)));
      break;
      
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8g2, arg_int);
      break;
      
    case U8X8_MSG_BYTE_START_TRANSFER:
      SPI.begin();
      
      if ( u8g2->display_info->sck_pulse_width_ns < 70 )
	SPI.setClockDivider( SPI_CLOCK_DIV2 );
      else if ( u8g2->display_info->sck_pulse_width_ns < 140 )
	SPI.setClockDivider( SPI_CLOCK_DIV4 );
      else
	SPI.setClockDivider( SPI_CLOCK_DIV8 );
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
    default:
      return 0;
  }
#endif
  return 1;
}


/*=============================================*/

extern "C" uint8_t u8x8_byte_arduino_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
#ifdef U8X8_HAVE_HW_I2C
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      Wire.write((uint8_t *)arg_ptr, (int)arg_int);
      break;
    case U8X8_MSG_BYTE_INIT:
      Wire.begin();
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      Wire.beginTransmission(u8x8_GetI2CAddress(u8x8)>>1);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      Wire.endTransmission();
      break;
    default:
      return 0;
  }
#endif
  return 1;
}


/*
  use U8X8_PIN_NONE as value for "reset", if there is no reset line
*/

void u8x8_SetPin_4Wire_SW_SPI(u8x8_t *u8x8, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_SPI_CLOCK, clock);
  u8x8_SetPin(u8x8, U8X8_PIN_SPI_DATA, data);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

#ifdef _obsolete_com_specific_setup
void u8x8_Setup_4Wire_SW_SPI(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset)
{
  u8x8_Setup(u8x8, display_cb, u8x8_cad_001, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
  
  /* assign individual pin values (only for ARDUINO, if pin_list is available) */
  u8x8_SetPin(u8x8, U8X8_PIN_SPI_CLOCK, clock);
  u8x8_SetPin(u8x8, U8X8_PIN_SPI_DATA, data);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}
#endif /* obsolete com specific setup */

void u8x8_SetPin_3Wire_SW_SPI(u8x8_t *u8x8, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_SPI_CLOCK, clock);
  u8x8_SetPin(u8x8, U8X8_PIN_SPI_DATA, data);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

#ifdef _obsolete_com_specific_setup
void u8x8_Setup_3Wire_SW_SPI(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset)
{
  u8x8_Setup(u8x8, display_cb, u8x8_cad_001, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
  
  /* assign individual pin values (only for ARDUINO, if pin_list is available) */
  u8x8_SetPin(u8x8, U8X8_PIN_SPI_CLOCK, clock);
  u8x8_SetPin(u8x8, U8X8_PIN_SPI_DATA, data);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}
#endif /* obsolete com specific setup */

/*
  use U8X8_PIN_NONE as value for "reset", if there is no reset line
*/
void u8x8_SetPin_4Wire_HW_SPI(u8x8_t *u8x8, uint8_t cs, uint8_t dc, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

void u8x8_SetPin_ST7920_HW_SPI(u8x8_t *u8x8, uint8_t cs, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}


#ifdef _obsolete_com_specific_setup
void u8x8_Setup_4Wire_HW_SPI(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t cs, uint8_t dc, uint8_t reset)
{
  u8x8_Setup(u8x8, display_cb, u8x8_cad_001, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
  
  /* assign individual pin values (only for ARDUINO, if pin_list is available) */
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}
#endif /* obsolete com specific setup */


void u8x8_SetPin_SW_I2C(u8x8_t *u8x8, uint8_t clock, uint8_t data, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_I2C_CLOCK, clock);
  u8x8_SetPin(u8x8, U8X8_PIN_I2C_DATA, data);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

#ifdef _obsolete_com_specific_setup
void u8x8_Setup_SSD13xx_SW_I2C(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t clock, uint8_t data, uint8_t reset)
{
  u8x8_Setup(u8x8, display_cb, u8x8_cad_001, u8x8_byte_ssd13xx_sw_i2c, u8x8_gpio_and_delay_arduino);
  
  /* assign individual pin values (only for ARDUINO, if pin_list is available) */
  u8x8_SetPin(u8x8, U8X8_PIN_I2C_CLOCK, clock);
  u8x8_SetPin(u8x8, U8X8_PIN_I2C_DATA, data);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}
#endif /* obsolete com specific setup */

void u8x8_SetPin_HW_I2C(u8x8_t *u8x8, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

void u8x8_SetPin_8Bit_6800(u8x8_t *u8x8, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_D0, d0);
  u8x8_SetPin(u8x8, U8X8_PIN_D1, d1);
  u8x8_SetPin(u8x8, U8X8_PIN_D2, d2);
  u8x8_SetPin(u8x8, U8X8_PIN_D3, d3);
  u8x8_SetPin(u8x8, U8X8_PIN_D4, d4);
  u8x8_SetPin(u8x8, U8X8_PIN_D5, d5);
  u8x8_SetPin(u8x8, U8X8_PIN_D6, d6);
  u8x8_SetPin(u8x8, U8X8_PIN_D7, d7);
  u8x8_SetPin(u8x8, U8X8_PIN_E, enable);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

#ifdef _obsolete_com_specific_setup
void u8x8_Setup_8Bit_6800(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset)
{
  u8x8_Setup(u8x8, display_cb, u8x8_cad_001, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
  
  /* assign individual pin values (only for ARDUINO, if pin_list is available) */
  u8x8_SetPin(u8x8, U8X8_PIN_D0, d0);
  u8x8_SetPin(u8x8, U8X8_PIN_D1, d1);
  u8x8_SetPin(u8x8, U8X8_PIN_D2, d2);
  u8x8_SetPin(u8x8, U8X8_PIN_D3, d3);
  u8x8_SetPin(u8x8, U8X8_PIN_D4, d4);
  u8x8_SetPin(u8x8, U8X8_PIN_D5, d5);
  u8x8_SetPin(u8x8, U8X8_PIN_D6, d6);
  u8x8_SetPin(u8x8, U8X8_PIN_D7, d7);
  u8x8_SetPin(u8x8, U8X8_PIN_E, enable);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}
#endif /* obsolete com specific setup */


void u8x8_SetPin_8Bit_8080(u8x8_t *u8x8, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t wr, uint8_t cs, uint8_t dc, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_D0, d0);
  u8x8_SetPin(u8x8, U8X8_PIN_D1, d1);
  u8x8_SetPin(u8x8, U8X8_PIN_D2, d2);
  u8x8_SetPin(u8x8, U8X8_PIN_D3, d3);
  u8x8_SetPin(u8x8, U8X8_PIN_D4, d4);
  u8x8_SetPin(u8x8, U8X8_PIN_D5, d5);
  u8x8_SetPin(u8x8, U8X8_PIN_D6, d6);
  u8x8_SetPin(u8x8, U8X8_PIN_D7, d7);
  u8x8_SetPin(u8x8, U8X8_PIN_E, wr);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}


#ifdef _obsolete_com_specific_setup
void u8x8_Setup_8Bit_8080(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t wr, uint8_t cs, uint8_t dc, uint8_t reset)
{
  u8x8_Setup(u8x8, display_cb, u8x8_cad_001, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
  
  /* assign individual pin values (only for ARDUINO, if pin_list is available) */
  u8x8_SetPin(u8x8, U8X8_PIN_D0, d0);
  u8x8_SetPin(u8x8, U8X8_PIN_D1, d1);
  u8x8_SetPin(u8x8, U8X8_PIN_D2, d2);
  u8x8_SetPin(u8x8, U8X8_PIN_D3, d3);
  u8x8_SetPin(u8x8, U8X8_PIN_D4, d4);
  u8x8_SetPin(u8x8, U8X8_PIN_D5, d5);
  u8x8_SetPin(u8x8, U8X8_PIN_D6, d6);
  u8x8_SetPin(u8x8, U8X8_PIN_D7, d7);
  u8x8_SetPin(u8x8, U8X8_PIN_E, wr);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}
#endif /* obsolete com specific setup */

