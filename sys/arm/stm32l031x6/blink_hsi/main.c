/* LED blink project for the STM32L031 */

#include "stm32l031xx.h"

volatile unsigned long SysTickCount = 0;

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  if ( SysTickCount & 1 )
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */
  else
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
}



void setHSIClock()
{
  
  
  /* test if the current clock source is something else than HSI */
  if ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) 
  {
    /* enable HSI */
    RCC->CR |= RCC_CR_HSION;    
    /* wait until HSI becomes ready */
    while ( (RCC->CR & RCC_CR_HSIRDY) == 0 )
      ;      
 
    /* enable the HSI "divide by 4" bit */
    RCC->CR |= (uint32_t)(RCC_CR_HSIDIVEN);
    /* wait until the "divide by 4" flag is enabled */
    while((RCC->CR & RCC_CR_HSIDIVF) == 0)
      ;
    
       
    /* then use the HSI clock */
    RCC->CFGR = (RCC->CFGR & (uint32_t) (~RCC_CFGR_SW)) | RCC_CFGR_SW_HSI; 
    
    /* wait until HSI clock is used */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
      ;
  }
  
  /* disable PLL */
  RCC->CR &= (uint32_t)(~RCC_CR_PLLON);
  /* wait until PLL is inactive */
  while((RCC->CR & RCC_CR_PLLRDY) != 0)
    ;

  /* set latency to 1 wait state */
  FLASH->ACR |= FLASH_ACR_LATENCY;
  
  /* At this point the HSI runs with 4 MHz */
  /* Multiply by 16 device by 2 --> 32 MHz */
  RCC->CFGR = (RCC->CFGR & (~(RCC_CFGR_PLLMUL| RCC_CFGR_PLLDIV ))) | (RCC_CFGR_PLLMUL16 | RCC_CFGR_PLLDIV2); 
  
  /* enable PLL */
  RCC->CR |= RCC_CR_PLLON; 
  
  /* wait until the PLL is ready */
  while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    ;

  /* use the PLL has clock source */
  RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); 
  /* wait until the PLL source is active */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) 
    ;
}

int main()
{
  setHSIClock();
  
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA13 */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA13 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* no Push/Pull for PA13 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed for PA13 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown for PA13 */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */

  SysTick->LOAD = 2000*500 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
    
  for(;;)
    ;
}
