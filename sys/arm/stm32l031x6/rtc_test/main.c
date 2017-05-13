/* LED blink project for the STM32L031 */

#include "stm32l031xx.h"
#include "display.h"
#include "delay.h"

/*=======================================================================*/
/* global variables */

volatile unsigned long SysTickCount = 0;

/*=======================================================================*/

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
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
  SystemCoreClockUpdate();				/* Update SystemCoreClock() */
  //SystemCoreClock = 32000000UL;
  
  SysTick->LOAD = (SystemCoreClock/1000)*50 - 1;   /* 50ms task */
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
  
  
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();
  
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA13 */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA13 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* Push/Pull for PA13 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed for PA13 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown for PA13 */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
  GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */

  
  
  display_Init();
  display_Write("STM32L031\n");
  display_WriteUnsigned(SystemCoreClock);
  display_Write(" Hz\n");

  
  /* real time clock enable */
  
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;	/* enable power interface */
  PWR->CR |= PWR_CR_DBP;				/* activate write access to RCC->CSR */
  
  /* externel 32K clock source */
  RCC->CSR |= RCC_CSR_LSEBYP;			/* bypass oscillator */
  
  /* externel 32K oscillator */
  //RCC->CSR &= ~RCC_CSR_LSEBYP;		/* no bypass oscillator */
  //RCC->CSR &= ~RCC_CSR_LSEDRV_Msk	/* lowest drive */
  //RCC->CSR  |= RCC_CSR_LSEDRV_0;		/* medium low drive */
  
  RCC->CSR |= RCC_CSR_LSEON;			/* enable low speed external clock */
  delay_micro_seconds(100000*5);			/* LSE requires between 100ms to 200ms */
  if ( RCC->CSR & RCC_CSR_LSERDY )
    display_Write("32K Clock Ready\n");
  else
    display_Write("32K Clock Error\n");
    
  RCC->CSR &= ~RCC_CSR_RTCSEL_Msk;		/* no clock selection for RTC */
  RCC->CSR |= RCC_CSR_RTCSEL_LSE;		/* select LSE */
  RCC->CSR |= RCC_CSR_RTCEN;			/* enable RTC */
     
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;
  
  RTC->ISR = RTC_ISR_INIT;				/* request RTC stop */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* wait for stop */
      ;
  RTC->PRER = 0x07f00ff;
  RTC->TR = 0; 
  RTC->ISR =~ RTC_ISR_INIT; 				/* start RTC */
  
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
  
  
  
  //PWR->CR &= ~PWR_CR_DBP;				/* disable write access to RCC->CSR */

  
  for(;;)
  {
    delay_micro_seconds(500000);
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */
    delay_micro_seconds(500000);
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */

    display_WriteUnsigned(RTC->TR);
    display_Write("\n");
  }
}
