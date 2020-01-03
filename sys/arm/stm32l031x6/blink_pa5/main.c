/* LED blink project for the STM32L031 */

#include "stm32l031xx.h"

volatile unsigned long SysTickCount = 0;

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  if ( SysTickCount & 1 )
    GPIOA->BSRR = GPIO_BSRR_BS_5;		/* atomic set PA5 */
  else
    GPIOA->BSRR = GPIO_BSRR_BR_5;		/* atomic clr PA5 */
}

int main()
{
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();
  GPIOA->MODER &= ~GPIO_MODER_MODE5;	/* clear mode for PA13 */
  GPIOA->MODER |= GPIO_MODER_MODE5_0;	/* Output mode for PA13 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;	/* no Push/Pull for PA13 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED5;	/* low speed for PA13 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;	/* no pullup/pulldown for PA13 */
  GPIOA->BSRR = GPIO_BSRR_BR_5;		/* atomic clr PA13 */

  SysTick->LOAD = 2000*500 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
    
  for(;;)
    ;
}
