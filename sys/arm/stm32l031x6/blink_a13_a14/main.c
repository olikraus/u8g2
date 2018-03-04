/* LED blink project for the STM32L031 */

#include "stm32l031xx.h"

volatile unsigned long SysTickCount = 0;

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  switch ( SysTickCount & 3 )
  {
    case 0:
      GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */
      break;
    case 1:
      GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
      break;
    case 2:
      GPIOA->BSRR = GPIO_BSRR_BS_14;		/* atomic set PA13 */
      break;
    case 3:
      GPIOA->BSRR = GPIO_BSRR_BR_14;		/* atomic clr PA13 */
      break;
  }
}

int main()
{
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA13 */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA13 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* no Push/Pull for PA13 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed for PA13 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown for PA13 */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */

  GPIOA->MODER &= ~GPIO_MODER_MODE14;	/* clear mode for PA14 */
  GPIOA->MODER |= GPIO_MODER_MODE14_0;	/* Output mode for PA14 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_14;	/* no Push/Pull for PA14 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED14;	/* low speed for PA14 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD14;	/* no pullup/pulldown for PA14 */
  GPIOA->BSRR = GPIO_BSRR_BR_14;		/* atomic clr PA14 */

  SysTick->LOAD = 2000*500 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
    
  for(;;)
    ;
}
