/* 
  LED blink project for the STM32L011 
  
  Assumes LED at Pin 6, GPIO PA0
*/

#include "stm32l011xx.h"

volatile unsigned long SysTickCount = 0;

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  
  if ( SysTickCount & 1 )
    GPIOA->BSRR = GPIO_BSRR_BS_0;		/* atomic set PA0 */
  else
    GPIOA->BSRR = GPIO_BSRR_BR_0;		/* atomic clr PA0 */
}

int main()
{
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();

  GPIOA->MODER &= ~GPIO_MODER_MODE0;	/* clear mode for PA0 */
  GPIOA->MODER |= GPIO_MODER_MODE0_0;	/* Output mode for PA0 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_0;	/* no Push/Pull for PA0 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED0;	/* low speed for PA0 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;	/* no pullup/pulldown for PA0 */
  GPIOA->BSRR = GPIO_BSRR_BR_0;		/* atomic clr PA0 */

  
  SysTick->LOAD = 2000*500 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
    
  for(;;)
    ;
}
