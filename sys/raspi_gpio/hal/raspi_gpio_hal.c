/*

  raspi_gpio_hal.c

  This could needs to be compiled with -DU8X8_USE_PINS 
  
  Code partly taken over from 
    http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_GPIO_C.html
    https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform#template-for-the-gpio-and-delay-callback


*/

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
/*
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
*/

#include "bcm2835.h"
#include "u8x8.h"


void delaynanoseconds(unsigned long ns)
{
  struct timespec ts;

  ts.tv_sec = 0;
  ts.tv_nsec = ns;
  nanosleep(&ts, (struct timespec *)NULL);
}


uint8_t u8x8_gpio_and_delay_raspi_gpio_hal(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  int i;
  //printf("msg %d\n", msg);
  switch(msg)
  {
    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
    case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
    case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
    case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
    case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
    case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
    case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
      if ( u8x8_GetPinValue(u8x8, msg) != U8X8_PIN_NONE )
      {
	bcm2835_gpio_write(u8x8_GetPinValue(u8x8, msg), arg_int);
      }
      break;
    
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8      
      if (!bcm2835_init())
        exit(1);
      
      atexit((void (*) (void))bcm2835_close);
      for( i = 0; i < U8X8_PIN_CNT; i++ )
	if ( u8x8->pins[i] != U8X8_PIN_NONE )
	{
	  if ( i < U8X8_PIN_OUTPUT_CNT )
	  {
            bcm2835_gpio_fsel(u8x8->pins[i], BCM2835_GPIO_FSEL_OUTP);
	  }
	  else
	  {
            // not supported at the moment
            //gpio_direction(u8x8->pins[i], 0);
	  }
	}
      delaynanoseconds(100000);
      break;							// can be used to setup pins
    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
      delaynanoseconds(arg_int);
      break;    
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
      delaynanoseconds((unsigned long)arg_int*100UL);
      break;
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
      delaynanoseconds((unsigned long)arg_int*10000UL);
      break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
      delaynanoseconds((unsigned long)arg_int*1000000UL);
      break;
    case U8X8_MSG_DELAY_I2C:				// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
      // correct value would be 10 for 100KHz, but we use 1
      // delay seems to be huge by the rest of the system
      bcm2835_delayMicroseconds(1);
      break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us        
    
    case U8X8_MSG_GPIO_MENU_SELECT:
      /* read pin level: uint8_t bcm2835_gpio_lev(uint8_t pin); */
      u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
      break;
    default:
      //u8x8_SetGPIOResult(u8x8, 1);			// default return value
      break;
  }
  return 1;
}

