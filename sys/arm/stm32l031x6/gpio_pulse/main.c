/* GPIO pulse generator project for the STM32L031 */

#include "stm32l031xx.h"

/*================================================*/
/* forward declaration */
void setGPIO( uint8_t n );



volatile unsigned long SysTickCount = 0;



void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  if ( SysTickCount & 1 )
    clearGPIO();
  else
    setGPIO( (SysTickCount>>1) & 7 );
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

void initGPIO(void)
{
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();
  

  GPIOA->MODER &= ~GPIO_MODER_MODE14;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE14_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_14;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED14;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD14;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_14;		/* atomic clr */
  
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr */
  
  GPIOA->MODER &= ~GPIO_MODER_MODE7;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE7_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED7;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_7;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE6;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE6_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED6;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_6;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE5;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE5_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED5;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_5;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE4;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE4_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED4;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_4;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE1;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE1_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED1;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_1;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE0;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE0_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_0;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED0;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_0;		/* atomic clr */

}

void clearGPIO(void)
{
  GPIOA->BSRR = GPIO_BSRR_BR_14
    | GPIO_BSRR_BR_13
    | GPIO_BSRR_BR_7
    | GPIO_BSRR_BR_6
    | GPIO_BSRR_BR_5
    | GPIO_BSRR_BR_4
    | GPIO_BSRR_BR_1
    | GPIO_BSRR_BR_0;
}

/*
  0:	PA14
  1:	PA13
  2: 	PA7
  3: 	PA6
  4: 	PA5
  5: 	PA4
  6: 	PA1
  7: 	PA0
*/
void setGPIO( uint8_t n )
{
  clearGPIO();
  switch(n)
  {
     case 0: GPIOA->BSRR = GPIO_BSRR_BS_14; break;
     case 1: GPIOA->BSRR = GPIO_BSRR_BS_13; break;
     case 2: GPIOA->BSRR = GPIO_BSRR_BS_7; break;
     case 3: GPIOA->BSRR = GPIO_BSRR_BS_6; break;
     case 4: GPIOA->BSRR = GPIO_BSRR_BS_5; break;
     case 5: GPIOA->BSRR = GPIO_BSRR_BS_4; break;
     case 6: GPIOA->BSRR = GPIO_BSRR_BS_1; break;
     case 7: GPIOA->BSRR = GPIO_BSRR_BS_0; break;

  }
}

int main()
{
  initGPIO();
  
  setHSIClock();

  SysTick->LOAD = 32000*100 - 1;		// 100 ms
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
    
  for(;;)
    ;
}
