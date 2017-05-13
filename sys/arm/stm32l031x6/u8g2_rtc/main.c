/* 

  RTC for the STM32L031 
  
  EXTI line
  17			RTC alarm
  19			RTC tamper & timestamp & CSS_LSE
  20			RTC wakeup timer
  

  __enable_irq()
  __disable_irq()
  
*/

#include "stm32l031xx.h"
#include "delay.h"
#include "u8g2.h"
#include "rtc.h"
#include "key.h"

/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*=======================================================================*/
/* global variables */

#define TAMPER_SYSTICK_DELAY 5

volatile unsigned long SysTickCount = 0;
volatile unsigned long RTCIRQCount = 0;
volatile unsigned long Tamper2Count = 0;
volatile unsigned long Tamper3Count = 0;
rtc_t rtc;
u8g2_t u8g2;

/*=======================================================================*/
/* utility function */
void enableRCCRTCWrite(void) U8G2_NOINLINE;
void enableRCCRTCWrite(void)
{
  //RCC->APB1ENR |= RCC_APB1ENR_PWREN;	/* enable power interface */
  //PWR->CR |= PWR_CR_DBP;				/* activate write access to RCC->CSR and RTC */
}

void disableRCCRTCWrite(void) U8G2_NOINLINE;
void disableRCCRTCWrite(void)
{
    //PWR->CR &= ~PWR_CR_DBP;				/* disable write access to RCC->CSR and RTC */
    //RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;	/* disable power interface */
}


/*=======================================================================*/

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  
  if ( Tamper3Count > 0 )
  {
    Tamper3Count--;
    if ( Tamper3Count == 0 )
    {
      //enableRCCRTCWrite();
      RTC->ISR &= ~RTC_ISR_TAMP3F;		/* clear tamper flag, allow new tamper event */
      //disableRCCRTCWrite();
    }
  }
  else
  {
      RTC->ISR &= ~RTC_ISR_TAMP3F;		/* clear tamper flag, allow new tamper event */
  }

  if ( Tamper2Count > 0 )
  {
    Tamper2Count--;
    if ( Tamper2Count == 0 )
    {
      //enableRCCRTCWrite();
      RTC->ISR &= ~RTC_ISR_TAMP2F;		/* clear tamper flag, allow new tamper event */
      //disableRCCRTCWrite();
    }
  }
  else
  {
      RTC->ISR &= ~RTC_ISR_TAMP2F;		/* clear tamper flag, allow new tamper event */
  }
  
}

void __attribute__ ((interrupt, used)) RTC_IRQHandler(void)
{
  //enableRCCRTCWrite();
    
  if ( (EXTI->PR & EXTI_PR_PIF20) != 0 )	/* interrupt caused by wake up */
  {
    EXTI->PR = EXTI_PR_PIF20;		/* wake up is connected to line 20, clear this IRQ */
  }
  
  /* the wake up time flag must be cleared, otherwise no further IRQ will happen */
  /* in principle, this should happen only when a IRQ line 20 IRQ happens, but */
  /* it will be more safe to clear this flag for any interrupt */
  
  RTC->ISR &= ~RTC_ISR_WUTF;	/* clear the wake up flag */

  
  if ( (EXTI->PR & EXTI_PR_PIF19) != 0 )	/* interrupt caused by tamper event */
  {
    EXTI->PR = EXTI_PR_PIF19;		/* clear tamper IRQ */
    
    /* The TAMPxF flag has to be cleared, but this is done in the systick handler after some delay */
    //RTC->ISR &= ~RTC_ISR_TAMP3F;
    //RTC->ISR &= ~RTC_ISR_TAMP2F;
    
    if ( RTC->ISR & RTC_ISR_TAMP3F )
    {
      key_add(KEY_NEXT);
      Tamper3Count = TAMPER_SYSTICK_DELAY;
    }
    if ( RTC->ISR & RTC_ISR_TAMP2F )
    {
      key_add(KEY_SELECT);
      Tamper2Count = TAMPER_SYSTICK_DELAY;
    }
    
  }
  //disableRCCRTCWrite();   
  RTCIRQCount++;
}

/*=======================================================================*/
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
}

/*=======================================================================*/
/*
  Set internal high speed clock as clock for the system
  Also call SystemCoreClockUpdate()

  This must be executed after each reset.
*/
void startHSIClock()
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

/*=======================================================================*/
/*
  Setup systick interrupt.
  A call to SystemCoreClockUpdate() is required before calling this function.

  This must be executed after each reset.
*/
void startSysTick(void)
{
  SysTick->LOAD = (SystemCoreClock/1000)*50 - 1;   /* 50ms task */
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
}

/*=======================================================================*/
/*
  Setup u8g2

  This must be executed only after POR reset.
*/
void initDisplay(void)
{
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
}


/*=======================================================================*/
/*
  configure and start RTC

  This must be executed only after POR reset.
*/
void initRTC(void)
{
  /* real time clock enable */  
  //enableRCCRTCWrite();
  
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;
    
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
     
  /* RTC Start */
  RTC->ISR = RTC_ISR_INIT;				/* request RTC stop */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* wait for stop */
      ;
  RTC->PRER = 0x07f00ff;
  RTC->TR = 0; 
  RTC->ISR =~ RTC_ISR_INIT; 				/* start RTC */
  
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
  //disableRCCRTCWrite();
}

/*=======================================================================*/
/*
  enable RTC wakeup and interrupt

  This must be executed after any reset.
*/
void startRTCWakeUp(void)
{
  /* wake up time setup & start */
  //enableRCCRTCWrite();
  
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;
  
  RTC->CR &=~ RTC_CR_WUTE; 			/* disable wakeup timer for reprogramming */
  while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF)
    ;
  
  RTC->WUTR = 0;						/* reload is 1: 1Hz with the 1Hz clock */
  RTC->CR &= ~RTC_CR_WUCKSEL;			/* clear selection register */
  RTC->CR |= RTC_CR_WUCKSEL_2;			/* select the 1Hz clock */
  RTC->CR |= RTC_CR_WUTE | RTC_CR_WUTIE ; 
  
  RTC->ISR &= ~RTC_ISR_WUTF;	/* clear the wake up flag... is this required? */
  
  
  /* tamper (button) detection */
  /* low level, filtered, pullup enabled, IRQ enabled, Sample Freq is 128Hz */
  RTC->TAMPCR = 
    RTC_TAMPCR_TAMP3NOERASE | RTC_TAMPCR_TAMP3IE | RTC_TAMPCR_TAMP3E | 
    RTC_TAMPCR_TAMP2NOERASE | RTC_TAMPCR_TAMP2IE | RTC_TAMPCR_TAMP2E | 
    RTC_TAMPCR_TAMPPRCH_0 | RTC_TAMPCR_TAMPFLT_1 | RTC_TAMPCR_TAMPFREQ;
  
  /* wake up IRQ is connected to line 20 */
  EXTI->RTSR |= EXTI_RTSR_RT20;			/* rising edge for wake up line */
  EXTI->IMR |= EXTI_IMR_IM20;			/* interrupt enable */

  /* tamper IRQ is connected to line 19 */
  EXTI->RTSR |= EXTI_RTSR_RT19;			/* rising edge for wake up line */
  EXTI->IMR |= EXTI_IMR_IM19;			/* interrupt enable */
  
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
  //disableRCCRTCWrite();
}


int cursor_pos = 0;
int select_pos = 0;

/*=======================================================================*/
int main()
{
  int i;
  startHSIClock();
  SystemCoreClockUpdate();				/* Update SystemCoreClock() */
  startUp();
  //SystemCoreClock = 32000000UL;
  startSysTick();
  
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


  initDisplay();
  initRTC();
  startRTCWakeUp();
  

  NVIC_SetPriority(RTC_IRQn, 0);
  NVIC_EnableIRQ(RTC_IRQn);
  
  
  for(;;)
  {
    u8g2_ClearBuffer(&u8g2);
    rtc_register_to_bcd(&rtc);
    rtc_bcd_to_ymd_hms(&rtc);
    rtc_draw_time(&rtc, &u8g2);
    
    
    RTC->WPR = 0x0ca;					/* disable RTC write protection */
    RTC->WPR = 0x053;
    RTC->TAMPCR &= ~RTC_TAMPCR_TAMP3E;
    RTC->TAMPCR &= ~RTC_TAMPCR_TAMP2E;
    __NOP();							/* add delay after disable tamper so that GPO can read the value */
    __NOP();
    
    if ( GPIOA->IDR & GPIO_IDR_ID0 )
      u8g2_DrawStr(&u8g2, 15, 45, "+");
    else
      u8g2_DrawStr(&u8g2, 15, 45, "-");
    if ( GPIOA->IDR & GPIO_IDR_ID2 )
      u8g2_DrawStr(&u8g2, 0, 45, "+");
    else
      u8g2_DrawStr(&u8g2, 0, 45, "-");

    RTC->TAMPCR |= RTC_TAMPCR_TAMP3E;
    RTC->TAMPCR |= RTC_TAMPCR_TAMP2E;
    
    RTC->WPR = 0;						/* enable RTC write protection */
    RTC->WPR = 0;
    
    u8g2_DrawStr(&u8g2, 30,45, u8x8_u8toa(RTCIRQCount, 3));
    u8g2_DrawStr(&u8g2, 90,45, u8x8_u8toa(key_queue_start, 2));

    for( i = 0; i < 4; i++ )
    {
      if ( i == select_pos )
	u8g2_DrawBox(&u8g2, i*20+10, 50, 10, 10);
      else
	u8g2_DrawFrame(&u8g2, i*20+10, 50, 10, 10);
      if ( i == cursor_pos )
	u8g2_DrawFrame(&u8g2, i*20+10-1, 50-1, 10+2, 10+2);	
    }
    
    u8g2_SendBuffer(&u8g2);
    
    delay_micro_seconds(50000);
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic set PA13 */
    delay_micro_seconds(50000);
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic clr PA13 */

    for(;;)
    {
      i = key_get();
      if ( i == KEY_NONE )
	break;
      if ( i == KEY_SELECT )
	select_pos = cursor_pos;
      if ( i == KEY_NEXT )
	cursor_pos = ( cursor_pos + 1 ) & 3;
      
    }

    //display_WriteUnsigned(RTC->TR);
    //display_Write("\n");
  }
}
