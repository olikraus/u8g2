/*
  u8x8cb.c
  
  STM32L031
  
  PA13: Clock
  PA14: Data
  Both lines have a pullup resistor
  
*/

#include "stm32l031xx.h"
#include "delay.h"
#include "u8x8.h"


uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      /* only support for software I2C*/
    
      RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
      __NOP();
      __NOP();
      
      GPIOA->MODER &= ~GPIO_MODER_MODE14;	/* clear mode for PA10 */
      //GPIOA->MODER |= GPIO_MODER_MODE14_0;	/* Output mode for PA10 */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_14;	/* no open drain for PA10 */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED14;	/* low speed for PA10 */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD14;	/* no pullup/pulldown for PA10 */
      //GPIOA->BSRR = GPIO_BSRR_BS_14;		/* atomic set PA10 */
    
      GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA9 */
      //GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA9 */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* no open drain for PA9 */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed for PA9 */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown for PA9 */
      //GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA9 */
        
      break;
    case U8X8_MSG_DELAY_NANO:
      /* not required for SW I2C */
      break;
    
    case U8X8_MSG_DELAY_10MICRO:
      /* not used at the moment */
      break;
    
    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
      break;
   
    case U8X8_MSG_DELAY_MILLI:
      delay_micro_seconds(arg_int*1000UL);
      break;
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
      delay_micro_seconds(arg_int<=2?5:1);
      break;
    
    case U8X8_MSG_GPIO_I2C_CLOCK:
      
      if ( arg_int == 0 )
      {
	GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA10 */
	GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA10 */
	GPIOA->BSRR = GPIO_BSRR_BR_3;		/* atomic clr PA9 */
      }
      else
      {
	//GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA9 */
	GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA9: input mode */
      }
      break;
    case U8X8_MSG_GPIO_I2C_DATA:
      
      if ( arg_int == 0 )
      {
	GPIOA->MODER &= ~GPIO_MODER_MODE14;	/* clear mode for PA10 */
	GPIOA->MODER |= GPIO_MODER_MODE14_0;	/* Output mode for PA10 */
	GPIOA->BSRR = GPIO_BSRR_BR_14;		/* atomic clr PA10 */
      }
      else
      {
	//GPIOA->BSRR = GPIO_BSRR_BS_14;		/* atomic set PA10 */
	// input mode
	GPIOA->MODER &= ~GPIO_MODER_MODE14;	/* clear mode for PA10: input mode */
      }
      break;
/*
    case U8X8_MSG_GPIO_MENU_SELECT:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_SELECT_PORT, KEY_SELECT_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_NEXT_PORT, KEY_NEXT_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_PREV_PORT, KEY_PREV_PIN));
      break;
    
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_HOME_PORT, KEY_HOME_PIN));
      break;
*/
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}
