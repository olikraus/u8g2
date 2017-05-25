/* LED blink project for the STM32L031 */

#include "stm32l031xx.h"
#include "delay.h"
#include "u8x8.h"

/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/*=======================================================================*/
/* global variables */

u8x8_t u8x8;
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

/*
  Enable several power regions: PWR, GPIOA
  Enable write access to RTC

  This must be executed after each reset.
*/
void startUp(void)
{
  
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;	/* enable power interface */
  PWR->CR |= PWR_CR_DBP;				/* activate write access to RCC->CSR and RTC */
  
  //PWR_CSR_Backup = PWR->CSR;			/* create a backup of the original PWR_CSR register for later analysis */
  PWR->CR |= PWR_CR_CSBF;				/* clear the standby flag in the PWR_CSR register, but luckily we have a copy */
  PWR->CR |= PWR_CR_CWUF;				/* also clear the WUF flag in PWR_CSR */
  
  /* PA0, TAMP2, button input */
  GPIOA->MODER &= ~GPIO_MODER_MODE0;	/* clear mode for PA0 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;	/* no pullup/pulldown for PA0 */
  GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;	/* pullup for PA0 */

  /* PA2, TAMP3, button input */
  GPIOA->MODER &= ~GPIO_MODER_MODE2;	/* clear mode for PA2 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;	/* no pullup/pulldown for PA2 */
  GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;	/* pullup for PA2 */

  
}


/* write access must be activated before calling this function: PWR->CR |= PWR_CR_DBP; */
unsigned int initRTC(void)
{
  unsigned int r = 0;
  /* real time clock enable */  
  
  //enableRCCRTCWrite();

  __disable_irq();
  
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;
    
  /* try externel 32K clock source */
  RCC->CSR |= RCC_CSR_LSEBYP;			/* bypass oscillator */
  
  
  RCC->CSR |= RCC_CSR_LSEON;			/* enable low speed external clock */
  delay_micro_seconds(100000*5);			/* LSE requires between 100ms to 200ms */
  
  if ( RCC->CSR & RCC_CSR_LSERDY )
  {
    r = 1;
  }
  else
  {
    RCC->CSR &= ~RCC_CSR_LSEON;			/* disable external clock */
    
    /* try externel 32K oscillator */
    RCC->CSR &= ~RCC_CSR_LSEBYP;		/* no bypass oscillator */
    RCC->CSR &= ~RCC_CSR_LSEDRV_Msk;	/* lowest drive */
    RCC->CSR  |= RCC_CSR_LSEDRV_0;		/* medium low drive */

    RCC->CSR |= RCC_CSR_LSEON;			/* enable low speed external clock */
    delay_micro_seconds(100000*6);			/* LSE requires between 200ms and 400ms */
    
    if ( RCC->CSR & RCC_CSR_LSERDY )
    {
      r = 2;
    }
  }
  
  if ( r > 0 )
  {
    
    RCC->CSR &= ~RCC_CSR_RTCSEL_Msk;		/* no clock selection for RTC */
    RCC->CSR |= RCC_CSR_RTCSEL_LSE;		/* select LSE */
    RCC->CSR |= RCC_CSR_RTCEN;			/* enable RTC */
       
    RTC->ISR = RTC_ISR_INIT;				/* request RTC stop */
    while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* wait for stop */
	;
    RTC->PRER = 0x07f00ff;					/* 1 Hz clock */
    RTC->TR = 0; 
    RTC->ISR =~ RTC_ISR_INIT; 				/* start RTC */
    
  }
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
  
  
  __enable_irq();
  
  return r;
}


int main()
{
  unsigned int rtcState;
  setHSIClock();
  SystemCoreClockUpdate();				/* Update SystemCoreClock() */
  //SystemCoreClock = 32000000UL;
  
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

  SysTick->LOAD = (SystemCoreClock/1000)*50 - 1;   /* 50ms task */
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
  
  
  /* setup display */
  //u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);
  //u8g2_InitDisplay(&u8g2);
  //u8g2_SetPowerSave(&u8g2, 0);

  startUp();
  rtcState = initRTC();

  u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);
  u8x8_InitDisplay(&u8x8);
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetPowerSave(&u8x8, 0);
  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_r);  
  u8x8_DrawString(&u8x8, 0,0, "Hello World!");
  u8x8_DrawGlyph(&u8x8, 0,1, rtcState+'0');
  
  for(;;)
  {
    delay_micro_seconds(500000);
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */
    delay_micro_seconds(500000);
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
  }
}
