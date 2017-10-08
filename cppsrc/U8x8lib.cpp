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

size_t U8X8::write(uint8_t v) 
{
  if ( v == '\n' )
  {
    ty++;
    tx=0;
  }
  else
  {
    u8x8_DrawGlyph(&u8x8, tx, ty, v);
    tx++;
  }
  return 1;
}



/*=============================================*/
/* callbacks */

#ifdef U8X8_USE_PINS
extern "C" uint8_t u8x8_gpio_and_delay_arduino(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
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

#ifndef __AVR__	
    /* this case is not compiled for any AVR, because AVR uC are so slow */
    /* that this delay does not matter */
    case U8X8_MSG_DELAY_NANO:
      delayMicroseconds(arg_int==0?0:1);
      break;
#endif
    
    case U8X8_MSG_DELAY_10MICRO:
      /* not used at the moment */
      break;
    
    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
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
#endif // U8X8_USE_PINS


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
  //uint8_t not_takeover_edge = 1 - takeover_edge;

  /* the following static vars are recalculated in U8X8_MSG_BYTE_START_TRANSFER */
  /* so, it should be possible to used multiple displays with different pins */
  
  static volatile uint8_t *arduino_clock_port;
  
  static uint8_t arduino_clock_mask;
  static uint8_t arduino_clock_n_mask;
  
  static volatile uint8_t *arduino_data_port;
  static uint8_t arduino_data_mask;
  static uint8_t arduino_data_n_mask;



  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
    
      data = (uint8_t *)arg_ptr;      
      if ( takeover_edge == 0 )
      {
	while( arg_int > 0 )
	{
	  b = *data;
	  data++;
	  arg_int--;
	  /* issue 156, check for speed */
#if F_CPU <= 17000000
	  if ( b == 0 )
	  {
	    *arduino_data_port &= arduino_data_n_mask;
	    for( i = 0; i < 8; i++ )
	    {
	      *arduino_clock_port |= arduino_clock_mask;	    
	      *arduino_clock_port &= arduino_clock_n_mask;
	    }
	  }
	  else
#endif
	  {
	    for( i = 0; i < 8; i++ )
	    {
	      if ( b & 128 )
		*arduino_data_port |= arduino_data_mask;
	      else
		*arduino_data_port &= arduino_data_n_mask;

	      *arduino_clock_port |= arduino_clock_mask;	    
	      b <<= 1;
	      *arduino_clock_port &= arduino_clock_n_mask;
	    }
	  }
	}
      }
      else
      {
	while( arg_int > 0 )
	{
	  b = *data;
	  data++;
	  arg_int--;
	  /* issue 156, check for speed */
#if F_CPU <= 17000000
	  if ( b == 0 )
	  {
	    *arduino_data_port &= arduino_data_n_mask;
	    for( i = 0; i < 8; i++ )
	    {
	      *arduino_clock_port &= arduino_clock_n_mask;
	      *arduino_clock_port |= arduino_clock_mask;	    
	    }
	  }
	  else
#endif
	  {
	    for( i = 0; i < 8; i++ )
	    {
	      if ( b & 128 )
		*arduino_data_port |= arduino_data_mask;
	      else
		*arduino_data_port &= arduino_data_n_mask;

	      *arduino_clock_port &= arduino_clock_n_mask;
	      b <<= 1;
	      *arduino_clock_port |= arduino_clock_mask;	    
	    }
	  }
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

#ifdef U8X8_USE_PINS

extern "C" uint8_t u8x8_byte_arduino_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
#ifdef U8X8_HAVE_HW_SPI
  uint8_t *data;
  uint8_t internal_spi_mode;
 
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      
      // 1.6.5 offers a block transfer, but the problem is, that the
      // buffer is overwritten with the incoming data
      // so it can not be used...
      // SPI.transfer((uint8_t *)arg_ptr, arg_int);
      
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
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      /* no wait required here */
      
      /* for SPI: setup correct level of the clock signal */
      // removed, use SPI.begin() instead: pinMode(11, OUTPUT);
      // removed, use SPI.begin() instead: pinMode(13, OUTPUT);
      // removed, use SPI.begin() instead: digitalWrite(13, u8x8_GetSPIClockPhase(u8x8));
      
      /* setup hardware with SPI.begin() instead of previous digitalWrite() and pinMode() calls */


      /* issue #377 */
      /* issue #378: removed ESP8266 support, which is implemented differently */
#if defined(ESP_PLATFORM) || defined(ARDUINO_ARCH_ESP32)
      /* ESP32 has the following begin: SPI.begin(int8_t sck=SCK, int8_t miso=MISO, int8_t mosi=MOSI, int8_t ss=-1); */
      /* not sure about ESP8266 */
      if ( u8x8->pins[U8X8_PIN_I2C_CLOCK] != U8X8_PIN_NONE && u8x8->pins[U8X8_PIN_I2C_DATA] != U8X8_PIN_NONE )
      {
	/* SPI.begin(int8_t sck=SCK, int8_t miso=MISO, int8_t mosi=MOSI, int8_t ss=-1); */
	/* actually MISO is not used, but what else could be used here??? */
	SPI.begin(u8x8->pins[U8X8_PIN_I2C_CLOCK], MISO, u8x8->pins[U8X8_PIN_I2C_DATA]);
      }
      else
      {
	SPI.begin();
      }
#else
      SPI.begin();	
#endif 

      

      break;
      
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
      
    case U8X8_MSG_BYTE_START_TRANSFER:
      /* SPI mode has to be mapped to the mode of the current controller, at least Uno, Due, 101 have different SPI_MODEx values */
      internal_spi_mode =  0;
      switch(u8x8->display_info->spi_mode)
      {
	case 0: internal_spi_mode = SPI_MODE0; break;
	case 1: internal_spi_mode = SPI_MODE1; break;
	case 2: internal_spi_mode = SPI_MODE2; break;
	case 3: internal_spi_mode = SPI_MODE3; break;
      }
      
#if ARDUINO >= 10600
      SPI.beginTransaction(SPISettings(u8x8->display_info->sck_clock_hz, MSBFIRST, internal_spi_mode));
#else
      SPI.begin();
      
      if ( u8x8->display_info->sck_pulse_width_ns < 70 )
	SPI.setClockDivider( SPI_CLOCK_DIV2 );
      else if ( u8x8->display_info->sck_pulse_width_ns < 140 )
	SPI.setClockDivider( SPI_CLOCK_DIV4 );
      else
	SPI.setClockDivider( SPI_CLOCK_DIV8 );
      SPI.setDataMode(internal_spi_mode);
      SPI.setBitOrder(MSBFIRST);
#endif
      
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
      
    case U8X8_MSG_BYTE_END_TRANSFER:      
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);

#if ARDUINO >= 10600
      SPI.endTransaction();
#else
      SPI.end();
#endif

      break;
    default:
      return 0;
  }
  
#else
#endif
  return 1;
}


/* issue #244 */
extern "C" uint8_t u8x8_byte_arduino_2nd_hw_spi(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
#ifdef U8X8_HAVE_2ND_HW_SPI
  uint8_t *data;
  uint8_t internal_spi_mode;
 
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      
      // 1.6.5 offers a block transfer, but the problem is, that the
      // buffer is overwritten with the incoming data
      // so it can not be used...
      // SPI.transfer((uint8_t *)arg_ptr, arg_int);
      
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
	SPI1.transfer((uint8_t)*data);
	data++;
	arg_int--;
      }
  
      break;
    case U8X8_MSG_BYTE_INIT:
      /* disable chipselect */
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      /* no wait required here */
      
      /* for SPI1: setup correct level of the clock signal */
      // removed, use SPI.begin() instead: pinMode(11, OUTPUT);
      // removed, use SPI.begin() instead: pinMode(13, OUTPUT);
      // removed, use SPI.begin() instead: digitalWrite(13, u8x8_GetSPIClockPhase(u8x8));
      
      /* setup hardware with SPI.begin() instead of previous digitalWrite() and pinMode() calls */
      SPI1.begin();	

      break;
      
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
      
    case U8X8_MSG_BYTE_START_TRANSFER:
      /* SPI1 mode has to be mapped to the mode of the current controller, at least Uno, Due, 101 have different SPI_MODEx values */
      internal_spi_mode =  0;
      switch(u8x8->display_info->spi_mode)
      {
	case 0: internal_spi_mode = SPI_MODE0; break;
	case 1: internal_spi_mode = SPI_MODE1; break;
	case 2: internal_spi_mode = SPI_MODE2; break;
	case 3: internal_spi_mode = SPI_MODE3; break;
      }
      
#if ARDUINO >= 10600
      SPI1.beginTransaction(SPISettings(u8x8->display_info->sck_clock_hz, MSBFIRST, internal_spi_mode));
#else
      SPI1.begin();
      
      if ( u8x8->display_info->sck_pulse_width_ns < 70 )
	SPI1.setClockDivider( SPI_CLOCK_DIV2 );
      else if ( u8x8->display_info->sck_pulse_width_ns < 140 )
	SPI1.setClockDivider( SPI_CLOCK_DIV4 );
      else
	SPI1.setClockDivider( SPI_CLOCK_DIV8 );
      SPI1.setDataMode(internal_spi_mode);
      SPI1.setBitOrder(MSBFIRST);
#endif
      
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
      
    case U8X8_MSG_BYTE_END_TRANSFER:      
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);

#if ARDUINO >= 10600
      SPI1.endTransaction();
#else
      SPI1.end();
#endif

      break;
    default:
      return 0;
  }
  
#else
#endif
  return 1;
}


/*=============================================*/

extern "C" uint8_t u8x8_byte_arduino_hw_i2c(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
#ifdef U8X8_HAVE_HW_I2C
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      Wire.write((uint8_t *)arg_ptr, (int)arg_int);
      break;
    case U8X8_MSG_BYTE_INIT:
#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266) || defined(ESP_PLATFORM) || defined(ARDUINO_ARCH_ESP32)
      /* for ESP8266/ESP32, Wire.begin has two more arguments: clock and data */          
      if ( u8x8->pins[U8X8_PIN_I2C_CLOCK] != U8X8_PIN_NONE && u8x8->pins[U8X8_PIN_I2C_DATA] != U8X8_PIN_NONE )
      {
	// second argument for the wire lib is the clock pin. In u8g2, the first argument of the  clock pin in the clock/data pair
	Wire.begin(u8x8->pins[U8X8_PIN_I2C_DATA] , u8x8->pins[U8X8_PIN_I2C_CLOCK]);
      }
      else
      {
	Wire.begin();
      }
#else
      Wire.begin();
#endif
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
#if ARDUINO >= 10600
      /* not sure when the setClock function was introduced, but it is there since 1.6.0 */
      /* if there is any error with Wire.setClock() just remove this function call */
      if ( u8x8->display_info->i2c_bus_clock_100kHz >= 4 )
      {
	Wire.setClock(400000L); 
      }
#endif
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

extern "C" uint8_t u8x8_byte_arduino_2nd_hw_i2c(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
#ifdef U8X8_HAVE_2ND_HW_I2C
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      Wire1.write((uint8_t *)arg_ptr, (int)arg_int);
      break;
    case U8X8_MSG_BYTE_INIT:
      Wire1.begin();
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
#if ARDUINO >= 10600
      /* not sure when the setClock function was introduced, but it is there since 1.6.0 */
      /* if there is any error with Wire.setClock() just remove this function call */
      if ( u8x8->display_info->i2c_bus_clock_100kHz >= 4 )
      {
	Wire1.setClock(400000L); 
      }
#endif
      Wire1.beginTransmission(u8x8_GetI2CAddress(u8x8)>>1);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      Wire1.endTransmission();
      break;
    default:
      return 0;
  }
#endif
  return 1;
}

#endif // U8X8_USE_PINS

/*=============================================*/

/*
  replacement for a more faster u8x8_byte_8bit_8080mode
  in general u8x8_byte_8bit_8080mode could be a fallback:

  uint8_t u8x8_byte_arduino_8bit_8080mode(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
  {
    return u8x8_byte_8bit_8080mode(u8x8, msg,arg_int, arg_ptr);
  }



*/

#ifndef __AVR_ARCH__
#define __AVR_ARCH__ 0
#endif 

#if !defined(U8X8_USE_PINS)
  /* no pin information (very strange), so fallback */
extern "C" uint8_t u8x8_byte_arduino_8bit_8080mode(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  return u8x8_byte_8bit_8080mode(u8x8, msg,arg_int, arg_ptr);
}

#elif __AVR_ARCH__ == 4 || __AVR_ARCH__ == 5 || __AVR_ARCH__ == 51 || __AVR_ARCH__ == 6

/* this function completly replaces u8x8_byte_8bit_8080mode*/
extern "C" uint8_t u8x8_byte_arduino_8bit_8080mode(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t i, b;
  uint8_t *data;

  /* the following static vars are recalculated in U8X8_MSG_BYTE_START_TRANSFER */
  /* so, it should be possible to use multiple displays with different pins */
  
  static volatile uint8_t *arduino_e_port;
  static volatile uint8_t arduino_e_mask;
  static volatile uint8_t arduino_e_n_mask;
  
  static volatile uint8_t *arduino_data_port[8];
  static volatile uint8_t arduino_data_mask[8];
  static volatile uint8_t arduino_data_n_mask[8];

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
	  if ( b & 1 )
	    *arduino_data_port[i] |= arduino_data_mask[i];
	  else
	    *arduino_data_port[i] &= arduino_data_n_mask[i];
	  b >>= 1;

	}
	
	*arduino_e_port &= arduino_e_n_mask;

	      
	/* AVR Architecture is very slow, extra call is not required */
	//u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->sda_setup_time_ns);
	u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->data_setup_time_ns);
	
	*arduino_e_port |= arduino_e_mask;
	
	/* AVR Architecture is very slow, extra call is not required */
	//u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->sck_pulse_width_ns);
	u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->write_pulse_width_ns);
	
      }
      break;
      
    case U8X8_MSG_BYTE_INIT:
      /* disable chipselect */
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      /* no wait required here */
      
      /* ensure that the enable signal is high */
      u8x8_gpio_call(u8x8, U8X8_MSG_GPIO_E, 1);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);

      /* there is no consistency checking for u8x8->pins[U8X8_PIN_E] */
    
      arduino_e_port = portOutputRegister(digitalPinToPort(u8x8->pins[U8X8_PIN_E]));
      arduino_e_mask = digitalPinToBitMask(u8x8->pins[U8X8_PIN_E]);
      arduino_e_n_mask = ~arduino_e_mask;

      /* there is no consistency checking for u8x8->pins[U8X8_PIN_D0] */

      for( i = 0; i < 8; i++ )
      {
	arduino_data_port[i] = portOutputRegister(digitalPinToPort(u8x8->pins[U8X8_PIN_D0+i]));
	arduino_data_mask[i] = digitalPinToBitMask(u8x8->pins[U8X8_PIN_D0+i]);
	arduino_data_n_mask[i] = ~arduino_data_mask[i];
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

#else
  /* fallback */
extern "C" uint8_t u8x8_byte_arduino_8bit_8080mode(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  return u8x8_byte_8bit_8080mode(u8x8, msg,arg_int, arg_ptr);
}
  
#endif


/*=============================================*/

/*
  replacement for a more faster u8x8_byte_ks0108
  in general u8x8_byte_ks0108 could be a fallback:

  uint8_t u8x8_byte_arduino_ks0108(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
  {
    return u8x8_byte_ks0108(u8x8, msg,arg_int, arg_ptr);
  }



*/

#ifndef __AVR_ARCH__
#define __AVR_ARCH__ 0
#endif 

#if !defined(U8X8_USE_PINS)
  /* no pin information (very strange), so fallback */
extern "C" uint8_t u8x8_byte_arduino_ks0108(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  return u8x8_byte_ks0108(u8x8, msg,arg_int, arg_ptr);
}

#elif __AVR_ARCH__ == 4 || __AVR_ARCH__ == 5 || __AVR_ARCH__ == 51 || __AVR_ARCH__ == 6

/* this function completly replaces u8x8_byte_ks0108*/
extern "C" uint8_t u8x8_byte_arduino_ks0108(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t i, b;
  uint8_t *data;

  /* the following static vars are recalculated in U8X8_MSG_BYTE_START_TRANSFER */
  /* so, it should be possible to use multiple displays with different pins */
  
  static volatile uint8_t *arduino_e_port;
  static volatile uint8_t arduino_e_mask;
  static volatile uint8_t arduino_e_n_mask;
  
  static volatile uint8_t *arduino_data_port[8];
  static volatile uint8_t arduino_data_mask[8];
  static volatile uint8_t arduino_data_n_mask[8];

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
	  if ( b & 1 )
	    *arduino_data_port[i] |= arduino_data_mask[i];
	  else
	    *arduino_data_port[i] &= arduino_data_n_mask[i];
	  b >>= 1;

	}
	
	*arduino_e_port |= arduino_e_mask;

	      
	/* AVR Architecture is very slow, extra call is not required */
	u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->data_setup_time_ns);
	
	*arduino_e_port &= arduino_e_n_mask;
	
	/* AVR Architecture is very slow, extra call is not required */
	u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->write_pulse_width_ns);
	
      }
      break;
      
    case U8X8_MSG_BYTE_INIT:
      /* disable chipselect */
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      /* no wait required here */
      
      /* ensure that the enable signal is low */
      u8x8_gpio_call(u8x8, U8X8_MSG_GPIO_E, 0);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_byte_set_ks0108_cs(u8x8, arg_int);
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);

      /* there is no consistency checking for u8x8->pins[U8X8_PIN_E] */
    
      arduino_e_port = portOutputRegister(digitalPinToPort(u8x8->pins[U8X8_PIN_E]));
      arduino_e_mask = digitalPinToBitMask(u8x8->pins[U8X8_PIN_E]);
      arduino_e_n_mask = ~arduino_e_mask;

      /* there is no consistency checking for u8x8->pins[U8X8_PIN_D0] */

      for( i = 0; i < 8; i++ )
      {
	arduino_data_port[i] = portOutputRegister(digitalPinToPort(u8x8->pins[U8X8_PIN_D0+i]));
	arduino_data_mask[i] = digitalPinToBitMask(u8x8->pins[U8X8_PIN_D0+i]);
	arduino_data_n_mask[i] = ~arduino_data_mask[i];
      }

      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_byte_set_ks0108_cs(u8x8, arg_int);
      break;
    default:
      return 0;
  }
  return 1;
}

#else
  /* fallback */
extern "C" uint8_t u8x8_byte_arduino_ks0108(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  return u8x8_byte_ks0108(u8x8, msg,arg_int, arg_ptr);
}
  
#endif








#ifdef U8X8_USE_PINS

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

void u8x8_SetPin_HW_I2C(u8x8_t *u8x8, uint8_t reset, uint8_t clock, uint8_t data)
{
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
  u8x8_SetPin(u8x8, U8X8_PIN_I2C_CLOCK, clock);
  u8x8_SetPin(u8x8, U8X8_PIN_I2C_DATA, data);
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

void u8x8_SetPin_KS0108(u8x8_t *u8x8, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t dc, uint8_t cs0, uint8_t cs1, uint8_t cs2, uint8_t reset)
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
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, cs0);
  u8x8_SetPin(u8x8, U8X8_PIN_CS1, cs1);
  u8x8_SetPin(u8x8, U8X8_PIN_CS2, cs2);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

void u8x8_SetPin_SED1520(u8x8_t *u8x8, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t dc, uint8_t e1, uint8_t e2, uint8_t reset)
{
  u8x8_SetPin(u8x8, U8X8_PIN_D0, d0);
  u8x8_SetPin(u8x8, U8X8_PIN_D1, d1);
  u8x8_SetPin(u8x8, U8X8_PIN_D2, d2);
  u8x8_SetPin(u8x8, U8X8_PIN_D3, d3);
  u8x8_SetPin(u8x8, U8X8_PIN_D4, d4);
  u8x8_SetPin(u8x8, U8X8_PIN_D5, d5);
  u8x8_SetPin(u8x8, U8X8_PIN_D6, d6);
  u8x8_SetPin(u8x8, U8X8_PIN_D7, d7);
  u8x8_SetPin(u8x8, U8X8_PIN_E, e1);
  u8x8_SetPin(u8x8, U8X8_PIN_CS, e2);
  u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
  u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}
#endif // U8X8_USE_PINS
