/* LED blink project for the STM32L031 */

#include "stm32l031xx.h"
#include "delay.h"
#include "u8g2.h"
#include "rtc.h"

/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*=======================================================================*/
/* global variables */

volatile unsigned long SysTickCount = 0;
rtc_t rtc;
u8g2_t u8g2;

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

  /* LED output line */
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA13 */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA13 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* Push/Pull for PA13 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed for PA13 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown for PA13 */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
  GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */

  /* PA0, button input */
  GPIOA->MODER &= ~GPIO_MODER_MODE0;	/* clear mode for PA0 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;	/* no pullup/pulldown for PA0 */
  GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;	/* pullup for PA0 */

  /* PA2, button input */
  GPIOA->MODER &= ~GPIO_MODER_MODE2;	/* clear mode for PA2 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;	/* no pullup/pulldown for PA2 */
  GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;	/* pullup for PA2 */


  /* setup display */
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_tf);
  u8g2_ClearBuffer(&u8g2);
  u8g2_DrawStr(&u8g2, 0,12, "STM32L031");
  u8g2_DrawStr(&u8g2, 0,24, u8x8_u8toa(SystemCoreClock/1000000, 2));
  u8g2_DrawStr(&u8g2, 20,24, "MHz");
  u8g2_SendBuffer(&u8g2);
  

  
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
  /*
  if ( RCC->CSR & RCC_CSR_LSERDY )
    display_Write("32K Clock Ready\n");
  else
    display_Write("32K Clock Error\n");
    */
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
    u8g2_ClearBuffer(&u8g2);
    rtc_register_to_bcd(&rtc);
    rtc_bcd_to_ymd_hms(&rtc);
    rtc_draw_time(&rtc, &u8g2);
    if ( GPIOA->IDR & GPIO_IDR_ID0 )
      u8g2_DrawStr(&u8g2, 20, 50, "+");
    else
      u8g2_DrawStr(&u8g2, 20, 50, "-");
    if ( GPIOA->IDR & GPIO_IDR_ID2 )
      u8g2_DrawStr(&u8g2, 0, 50, "+");
    else
      u8g2_DrawStr(&u8g2, 0, 50, "-");
    u8g2_SendBuffer(&u8g2);
    
    delay_micro_seconds(50000);
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic set PA13 */
    delay_micro_seconds(50000);
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic clr PA13 */


    //display_WriteUnsigned(RTC->TR);
    //display_Write("\n");
  }
}
