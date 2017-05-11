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
#include "gui.h"

/*=======================================================================*/
/* Configuration */
/* Note: 50ms systick */

/* delay until other another button press is accepted */
/* time is in systicks (50ms) */
#define TAMPER_SYSTICK_DELAY 6

/* delay until the menu goes back time display and standby mode */
/* delay in systicks (50ms) */
/* 50ms*20 = 1 second */
/* 50ms*20*10 = 10 second */
/* 50ms*20*18 = 18 second */
#define MENU_IDLE_SYSTICK_TIMEOUT (20*18)

/* wakeup period: The uC will wake up after the specified number of seconds */
/* the value is one less the intended number of seconds: 
/* 0: wakeup every 1 second */
/* 14: wakeup every 15 seconds */
/* 29: wakeup every 30 seconds */
/* After wakeup the uC will refresh the display and check for the alarms. This means the wakeup time should not be */
/* more than 1 minute. There might be also a delay of up to WAKEUP_PERIOD+1 seconds until the alarm is detected. */
/* Large values reduce power consumtion, but displayed time and alarm might be later than the actual RTC time. */
#define WAKEUP_PERIOD 14

/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*=======================================================================*/
/* global variables */

#define RESET_REASON_POR 0
#define RESET_REASON_NVIC_RESET 1
#define RESET_REASON_TAMP2 2
#define RESET_REASON_TAMP3 3
#define RESET_REASON_WUF 4

volatile unsigned long SysTickCount = 0;
volatile unsigned long RTCWUCount = 0;
volatile unsigned long RTCIRQCount = 0;
volatile unsigned long Tamper2Count = 0;
volatile unsigned long Tamper3Count = 0;
volatile unsigned long MenuIdleTimer = 0;
volatile unsigned long PWR_CSR_Backup;
volatile unsigned long ResetReason = RESET_REASON_POR;

rtc_t rtc;
u8g2_t u8g2;


/*=======================================================================*/

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  
  if ( Tamper3Count > 0 )
  {
    MenuIdleTimer = 0;
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
    MenuIdleTimer = 0;
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

  MenuIdleTimer++;
  
}

void __attribute__ ((interrupt, used)) RTC_IRQHandler(void)
{
  //enableRCCRTCWrite();
    
  if ( (EXTI->PR & EXTI_PR_PIF20) != 0 )	/* interrupt caused by wake up */
  {
    EXTI->PR = EXTI_PR_PIF20;		/* wake up is connected to line 20, clear this IRQ */
    RTCWUCount++;
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
      MenuIdleTimer = 0;
      Tamper3Count = TAMPER_SYSTICK_DELAY;
    }
    if ( RTC->ISR & RTC_ISR_TAMP2F )
    {
      key_add(KEY_SELECT);
      MenuIdleTimer = 0;
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
  
  PWR_CSR_Backup = PWR->CSR;			/* create a backup of the original PWR_CSR register for later analysis */
  PWR->CR |= PWR_CR_CSBF;				/* clear the standby flag in the PWR_CSR register, but luckily we have a copy */
  PWR->CR |= PWR_CR_CWUF;				/* also clear the WUF flag in PWR_CSR */
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
void initDisplay(uint8_t is_por)
{
  /* setup display */
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);
  
  gui_Init(&u8g2, is_por);
  
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
     
  RTC->ISR = RTC_ISR_INIT;				/* request RTC stop */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* wait for stop */
      ;
  RTC->PRER = 0x07f00ff;					/* 1 Hz clock */
  RTC->TR = 0; 
  RTC->ISR =~ RTC_ISR_INIT; 				/* start RTC */
  
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
}

/*=======================================================================*/
/*
  enable RTC wakeup and interrupt

  This must be executed after any reset.
*/
void startRTCWakeUp(void)
{
  /* wake up time setup & start */
  
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;
  
  RTC->CR &=~ RTC_CR_WUTE; 			/* disable wakeup timer for reprogramming */
  while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF)
    ;
  
  RTC->WUTR = WAKEUP_PERIOD;			/* wakeup time */
  //RTC->WUTR = 0;						/* reload is 1: 1Hz with the 1Hz clock */
  RTC->CR &= ~RTC_CR_WUCKSEL;			/* clear selection register */
  RTC->CR |= RTC_CR_WUCKSEL_2;			/* select the 1Hz clock */
  RTC->CR |= RTC_CR_WUTE | RTC_CR_WUTIE ; 
  
  /* clear all the detection flags, not 100% sure whether this is required */
  RTC->ISR &= ~RTC_ISR_WUTF;	
  RTC->ISR &= ~RTC_ISR_TAMP2F;
  RTC->ISR &= ~RTC_ISR_TAMP3F;
  
  
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
  EXTI->RTSR |= EXTI_RTSR_RT19;			/* rising edge for tamper*/
  EXTI->IMR |= EXTI_IMR_IM19;			/* interrupt enable */
  
  RTC->WPR = 0;						/* disable RTC write protection */
  RTC->WPR = 0;
}

/* read values from RTC and store the values into the gui_data struct */
void readRTC(void)
{
  uint32_t r;
  int i;
  uint8_t bcd[12];
  
  r = RTC->TR;

  i = 0;
  do
  {
    bcd[i] = r & 15;
    r >>= 4;
    i++;
  } while( i < 6 );
  
  bcd[1] &= 7;	/* seconds */
  bcd[3] &= 7;	/* minutes */
  bcd[5] &= 3; /* hours */

  gui_data.h = bcd[4]  + bcd[5]*10;;
  gui_data.mt = bcd[3];
  gui_data.mo = bcd[2];
  gui_data.st = bcd[1];
  gui_data.so = bcd[0];


  r = RTC->DR;
  i = 0;
  do
  {
    bcd[i] = r & 15;
    r >>= 4;
    i++;
  } while( i < 6 );

  bcd[1] &= 3;	/* days */
  bcd[3] &= 1;	/* months */  

  gui_data.day = bcd[0] + bcd[1]*10;
  gui_data.month = bcd[2] + bcd[3]*10;
  gui_data.year_o = bcd[4];
  gui_data.year_t = bcd[5];
  
  
  gui_date_adjust();	/* calculate weekday */
  //gui_Recalculate();  /* this will also store the values back in the backup registers */
}

void enterStandByMode(void)
{
  MenuIdleTimer = 0;
  PWR->CR |= PWR_CR_PDDS;				/* Power Down Deepsleep */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;	/* set the cortex M0+ deep sleep flag */  
  __DSB();  							/* finish memory access */
  __WFI();								/* enter deep sleep */
  __NOP();
}

/*=======================================================================*/
int main()
{
  int i;
  startHSIClock();						/* Increase system clock, must be executed after each reset */
  SystemCoreClockUpdate();				/* Update variable SystemCoreClock, must be executed after each reset */
  startUp();							/* basic system setup + make a backup of PWR_CSR (PWR_CSR_Backup), must be executed after each reset */
  startSysTick();						/* start the sys tick interrupt, must be executed after each reset */
  
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


  /* the lowest two bits of the PWR_CSR reg indicate wake up from standby (bit 1) and WUF als source (bit 0) */
  /* both bits are 0 for POR and button reset, both bits are 1 for a wakeup reset */
  /* bits	|	root cause */
  /* 00	|	POR or NVIC		--> perform full setup */
  /* 11	|	Standby + WUF 	--> continue with main screen */
  /* 01	|	NVIC-Reset 		--> perform full setup */
  /* we check bit 1 only */
  
  switch(PWR_CSR_Backup & 3)
  {
    case 0: 	/* Power on reset */
      ResetReason = RESET_REASON_POR; 
      break;
    case 1: 	/* reset by NVIC_SystemReset() */
      ResetReason = RESET_REASON_NVIC_RESET; 
      break;
    default:	/* probably a reset caused by RTC */
      /* analyse RTC_ISR register */
      if ( RTC->ISR & RTC_ISR_TAMP2F )
	ResetReason = RESET_REASON_TAMP2; 
      else if ( RTC->ISR & RTC_ISR_TAMP3F )
	ResetReason = RESET_REASON_TAMP3; 
      else 
	ResetReason = RESET_REASON_WUF;	
      break;
  }

  
  
  if ( ResetReason == RESET_REASON_POR || ResetReason == RESET_REASON_NVIC_RESET )
  {
    /* Power on reset */
    initRTC();
    readRTC();
    initDisplay(1);	/* init display assumes proper values in gui_data */
  }
  else
  {
    /* Reset caused by wakeup */
    
    /* we probably have to clear the RTC detection flags for WUF and TAMPER */
    /* this is done later in startRTCWakeUp() */
 
    
    readRTC();		
    
    /* do a warm start of the display, this means that the display reset is skipped and the init sequence is not sent */
    initDisplay(0);	/* init display assumes proper values in gui_data, additionally the alarm flag might be set here */
  }
  
  startRTCWakeUp();						/* setup wakeup and temper, probably required after each reset */

  NVIC_SetPriority(RTC_IRQn, 0);
  NVIC_EnableIRQ(RTC_IRQn);
  

  if ( ResetReason == RESET_REASON_WUF && gui_data.is_alarm == 0 )
  {
    /* update current time */
    u8g2_ClearBuffer(&u8g2);
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic set PA13 */
    gui_Draw();
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic clr PA13 */
    u8g2_SendBuffer(&u8g2);
    /* go back to sleep mode */
    enterStandByMode();
  }
  
  for(;;)
  {
    if ( gui_menu.me_list == melist_display_time )
    {
      readRTC();
      gui_SignalTimeChange();
    }
    
    u8g2_ClearBuffer(&u8g2);
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic set PA13 */
    gui_Draw();
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic clr PA13 */
    u8g2_SendBuffer(&u8g2);
    
    for(;;)
    {
      i = key_get();
      if ( i == KEY_NONE )
	break;
      if ( i == KEY_SELECT )
	gui_Select();
      if ( i == KEY_NEXT )
	gui_Next();      
    }
    
    if ( MenuIdleTimer > MENU_IDLE_SYSTICK_TIMEOUT && gui_data.is_equal == 0 )
    {
      
      if ( gui_menu.me_list != melist_display_time )
      {
	/* jump back to the display menu and redraw the time. not sure if this is required */
	menu_SetMEList(&gui_menu, melist_display_time, 0);
	readRTC();
	gui_SignalTimeChange();
	u8g2_ClearBuffer(&u8g2);
	gui_Draw();
	u8g2_SendBuffer(&u8g2);
      }
      
      /* stop everything except RTC */
      enterStandByMode();
    }

  }
}
