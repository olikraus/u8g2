/* 

  Clock for the STM32L031 
  
  EXTI line
  17			RTC alarm
  19			RTC tamper & timestamp & CSS_LSE
  20			RTC wakeup timer
  
  PA0 TAMP2	Button
  PA2 TAMP3	Button


  __enable_irq();
  __disable_irq();
  
*/

#include <stdint.h>
#include <stddef.h>
#include "stm32l031xx.h"
#include "delay.h"
#include "u8g2.h"
//#include "rtc.h"
#include "key.h"
#include "gui.h"

/*=======================================================================*/
/* Configuration */
/* Note: 50ms systick */

/* delay until other another button press is accepted */
/* time is in systicks (50ms) */
#define TAMPER_SYSTICK_DELAY 22

/* delay until the menu goes back time display and standby mode */
/* delay in systicks (50ms) */
/* 50ms*20 = 1 second */
/* 50ms*20*10 = 10 second */
/* 50ms*20*18 = 18 second */
#define MENU_IDLE_SYSTICK_TIMEOUT (20*18)

/* max alarm duration */
/* time in systicks (50ms) */
/* 50ms*20 = 1 second */
/* 50ms*20*10 = 10 second */
/* 50ms*20*18 = 18 second */
/* 50ms*20*120 = 120 seconds */
#define ALARM_MAX_SYSTICK_TIME (20*30)


/* wakeup period: The uC will wake up after the specified number of seconds */
/* the value is one less the intended number of seconds: */
/* 0: wakeup every 1 second */
/* 14: wakeup every 15 seconds */
/* 29: wakeup every 30 seconds */
/* After wakeup the uC will refresh the display and check for the alarms. This means the wakeup time should not be */
/* more than 1 minute. There might be also a delay of up to WAKEUP_PERIOD+1 seconds until the alarm is detected. */
/* Large values reduce power consumtion, but displayed time and alarm might be later than the actual RTC time. */
#define WAKEUP_PERIOD 14

/* DST (daylight savings time) rules */
/* 0: DST not applied */
/* 1: EU */
/* 2: US */
#define DST_RULE 1


/* Contrast value for the display in normal mode (u8g2_SetContrast). */
/* 208: default value for the SSD1306 */
#define DISPLAY_CONTRAST_NORMAL 210 

/* Contrast value for the display in standby mode, if the display mode DISPLAY_STANDBY_MODE_REDUCED is aktive. */
/* 208: default value for the SSD1306, value 0 still shows something on the display */
#define DISPLAY_CONTRAST_REDUCED 5 


/* the following variable defines the behavior of the display during standby of the uC */
#define DISPLAY_STANDYB_MODE_ALWAYS_ON 0
#define DISPLAY_STANDBY_MODE_REDUCED 1
#define DISPLAY_STANDBY_MODE_OFF 2
volatile unsigned long DisplayStandbyMode = DISPLAY_STANDBY_MODE_OFF;


/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
int is_dst_by_date(uint8_t region);
void adjustDST(uint8_t region);


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
volatile unsigned long isIgnoreNextKey = 0;
volatile unsigned long Tamper2Count = 0;
volatile unsigned long Tamper3Count = 0;
volatile unsigned long MenuIdleTimer = 0;
volatile unsigned long PWR_CSR_Backup;
volatile unsigned long ResetReason = RESET_REASON_POR;
volatile unsigned long AlarmSeqPos = 0;
volatile unsigned long AlarmSeqDly = 0;
const uint8_t *AlarmSeqPtr = NULL;
const uint8_t *AlarmSeqStart = NULL;
volatile unsigned long AlarmSeqTime = 0;
volatile unsigned long RTCUpdateCount = 0;		// decremented in SysTick IRQ if not 0
volatile unsigned long NextDSTAdjustment = 0;



//rtc_t rtc;
u8g2_t u8g2;


/*=======================================================================*/
#define AOff(dly) 		(0<<5)|((dly)&0x01f)
#define ABeep(dly)		(1<<5)|((dly)&0x01f)
#define ARepeat()		(0xfe)
#define AEnd()		(0xff)


/*=======================================================================*/

const uint8_t ASeqTrippleBeep[] = 
{ ABeep(1),AOff(2), ABeep(1),AOff(2),ABeep(1),AOff(22), ARepeat() };


/*=======================================================================*/

void set_alarm_sequence(const uint8_t *alarm_sequence)
{
  GPIOA->BSRR = GPIO_BSRR_BR_6;		/* atomic clr PA6 */
  AlarmSeqDly = 0;
  AlarmSeqPtr = alarm_sequence;
  AlarmSeqStart = alarm_sequence;
  AlarmSeqTime = 0;
}

void ExecuteAlarmSequenceStep(void)
{
  if ( AlarmSeqPtr == NULL )
    return;
  //AlarmSeqTime++;
  //if ( AlarmSeqTime > ALARM_MAX_SYSTICK_TIME )
  //{
  //  set_alarm_sequence(NULL);
  //  gui_data.is_alarm = 0;			// disable alarm
  //  return;
  //}
    
  if ( AlarmSeqDly > 0 )
  {
    AlarmSeqDly--;
    return ;
  }
  switch( (*AlarmSeqPtr)>>5 )
  {
    case 0:
      GPIOA->BSRR = GPIO_BSRR_BR_6;		/* atomic clr PA6 */
      AlarmSeqDly = ((*AlarmSeqPtr) & 0x01f);
      break;
    case 1:
      GPIOA->BSRR = GPIO_BSRR_BS_6;		/* atomic set PA13 */
      AlarmSeqDly = ((*AlarmSeqPtr) & 0x01f);
      break;
    default:
      if ( *AlarmSeqPtr == 0x0fe )
	AlarmSeqPtr = AlarmSeqStart;
      return;
  }
  AlarmSeqPtr++;  
}


void SetAlarmSequence(const uint8_t *alarm_sequence)
{
  __disable_irq();
  set_alarm_sequence(alarm_sequence);
  __enable_irq();
}



/*=======================================================================*/

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  int is_t2 = 0;
  int is_t3 = 0;

  SysTickCount++;  
  
  /* read the tamper/button state */
  /* this is more complicated, because there are no external pull ups for the buttons */
  /* pull ups can be activated via GPIO, but are disabled if the pin is configured as tamper input */
  /* As a consequence, we have to disable tamper (so that the internal pullups are active), then */
  /* after some delay, get the GPIO value of the tamper input and restore tamper status */

  if ( Tamper2Count > 0 || Tamper3Count > 0 )
  {
    RTC->WPR = 0x0ca;					/* disable RTC write protection */
    RTC->WPR = 0x053;

    RTC->TAMPCR &= ~RTC_TAMPCR_TAMP2E;	/* disable tamper  so that we can do normal GPIO access */
    RTC->TAMPCR &= ~RTC_TAMPCR_TAMP3E;	/* disable tamper  so that we can do normal GPIO access */
    __NOP();							/* add delay after disable tamper so that GPIO can read the value */
    __NOP();

    is_t2 = (GPIOA->IDR & GPIO_IDR_ID0) != 0 ? 1 : 0;
    is_t3 = (GPIOA->IDR & GPIO_IDR_ID2) != 0 ? 1 : 0;

    RTC->TAMPCR |= RTC_TAMPCR_TAMP2E;	/* enable tamper */
    RTC->TAMPCR |= RTC_TAMPCR_TAMP3E;	/* enable tamper */

    RTC->WPR = 0;						/* enable RTC write protection */
    RTC->WPR = 0;
  }
  
  if ( Tamper3Count > 0 )
  {
    Tamper3Count--;
    /* check for timeout or whether the user has released the button */
    if ( Tamper3Count == 0 || is_t3 )
    {
      Tamper3Count = 0;
      RTC->ISR &= ~RTC_ISR_TAMP3F;		/* clear tamper flag, allow new tamper event */
    }
  }
  else
  {
     RTC->ISR &= ~RTC_ISR_TAMP3F;		/* clear tamper flag, allow new tamper event */
  }

  if ( Tamper2Count > 0 )
  {
    Tamper2Count--;
    /* check for timeout or whether the user has released the button */
    if ( Tamper2Count == 0 || is_t2)
    {
      Tamper2Count = 0;
      RTC->ISR &= ~RTC_ISR_TAMP2F;		/* clear tamper flag, allow new tamper event */
    }
  }
  else
  {
      RTC->ISR &= ~RTC_ISR_TAMP2F;		/* clear tamper flag, allow new tamper event */
  }

  ExecuteAlarmSequenceStep();
  
  MenuIdleTimer++;
  if ( RTCUpdateCount > 0 )
    RTCUpdateCount--;
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
      if ( isIgnoreNextKey == 0 )
      {
	key_add(KEY_NEXT);
      }
      isIgnoreNextKey = 0;
      MenuIdleTimer = 0;
      Tamper3Count = TAMPER_SYSTICK_DELAY;
    }
    if ( RTC->ISR & RTC_ISR_TAMP2F )
    {
      if ( isIgnoreNextKey == 0 )
      {
	key_add(KEY_SELECT);
      }
      isIgnoreNextKey = 0;
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
  
  /* PA0, TAMP2, button input */
  GPIOA->MODER &= ~GPIO_MODER_MODE0;	/* clear mode for PA0 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;	/* no pullup/pulldown for PA0 */
  GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;	/* pullup for PA0 */

  /* PA2, TAMP3, button input */
  GPIOA->MODER &= ~GPIO_MODER_MODE2;	/* clear mode for PA2 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;	/* no pullup/pulldown for PA2 */
  GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;	/* pullup for PA2 */


  /* buzzer output */
  GPIOA->MODER &= ~GPIO_MODER_MODE6;	/* clear mode for PA6 */
  GPIOA->MODER |= GPIO_MODER_MODE6_0;	/* Output mode for PA6 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;	/* Push/Pull for PA6 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED6;	/* low speed for PA6 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6;	/* no pullup/pulldown for PA6 */
  GPIOA->BSRR = GPIO_BSRR_BR_6;		/* atomic clr PA6 */
  
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

  This must be executed after every reset
*/
void initDisplay(uint8_t is_por)
{
  /* setup display */
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);
  
  gui_Init(&u8g2, is_por);
  
  u8g2_SetFlipMode(&u8g2, 1);
}


/*=======================================================================*/
/*
  configure and start RTC

  This must be executed only after POR reset.

  write access must be activated before calling this function: PWR->CR |= PWR_CR_DBP;

  return values:
    0: 	no clock avilable
    1:		external clock
    2:		external oszillator

*/
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

/*=======================================================================*/
/*
  enable RTC wakeup and interrupt

  This must be executed after any reset.
*/
void startRTCWakeUp(void)
{
  /* wake up time setup & start */
  
  __disable_irq();
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


    // RTC_TAMPCR_TAMPPUDIS 

  /* wake up IRQ is connected to line 20 */
  EXTI->RTSR |= EXTI_RTSR_RT20;			/* rising edge for wake up line */
  EXTI->IMR |= EXTI_IMR_IM20;			/* interrupt enable */

  /* tamper IRQ is connected to line 19 */
  EXTI->RTSR |= EXTI_RTSR_RT19;			/* rising edge for tamper*/
  EXTI->IMR |= EXTI_IMR_IM19;			/* interrupt enable */
  
  RTC->WPR = 0;						/* disable RTC write protection */
  RTC->WPR = 0;
  __enable_irq();
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
  
  if ( DisplayStandbyMode == DISPLAY_STANDBY_MODE_REDUCED )
    u8g2_SetContrast(&u8g2, DISPLAY_CONTRAST_REDUCED);

  if ( DisplayStandbyMode == DISPLAY_STANDBY_MODE_OFF )
    u8g2_SetPowerSave(&u8g2, 1);

  
  SetAlarmSequence(NULL);
  GPIOA->MODER &= ~GPIO_MODER_MODE6;	/* clear mode for PA6 --> input */
  
  PWR->CR |= PWR_CR_PDDS;				/* Power Down Deepsleep */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;	/* set the cortex M0+ deep sleep flag */  
  __DSB();  							/* finish memory access */
  __WFI();								/* enter deep sleep */
  __NOP();
}

/*
  ch 0..15: 	GPIO
  ch 16:		???
  ch 17:		vref (bandgap)
  ch18:		temperature sensor

  returns 12 bit result, right aligned 
*/
uint16_t readADC(uint8_t ch)
{
  uint32_t data;
  uint32_t i;
  
  __disable_irq();
  
  /* ADC RESET */
  
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;	/* enable ADC clock */
  __NOP();								/* let us wait for some time */
  __NOP();								/* let us wait for some time */  
  RCC->APB2RSTR |= RCC_APB2RSTR_ADCRST;
  __NOP();								/* let us wait for some time */
  __NOP();								/* let us wait for some time */
  RCC->APB2RSTR &= ~RCC_APB2RSTR_ADCRST;
  __NOP();								/* let us wait for some time */
  __NOP();								/* let us wait for some time */
  

  /* Enable some basic parts */
  
  ADC1->IER = 0;						/* do not allow any interrupts */
  ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE;	/* select HSI16 clock */
  
  ADC1->CR |= ADC_CR_ADVREGEN;				/* enable ADC voltage regulator, probably not required, because this is automatically activated */
  ADC->CCR |= ADC_CCR_VREFEN; 			/* Wake-up the VREFINT */  
  ADC->CCR |= ADC_CCR_TSEN; 			/* Wake-up the temperature sensor */  

  __NOP();								/* let us wait for some time */
  __NOP();								/* let us wait for some time */

  /* CALIBRATION */
  
  if ((ADC1->CR & ADC_CR_ADEN) != 0) /* clear ADEN flag if required */
  {
    ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);
  }
  ADC1->CR |= ADC_CR_ADCAL; 				/* start calibration */
  while ((ADC1->ISR & ADC_ISR_EOCAL) == 0) 	/* wait for clibration finished */
  {
  }
  ADC1->ISR |= ADC_ISR_EOCAL; 			/* clear the status flag, by writing 1 to it */
  __NOP();								/* not sure why, but some nop's are required here, at least 4 of them */
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();

  /* ENABLE ADC */
  
  ADC1->ISR |= ADC_ISR_ADRDY; 			/* clear ready flag */
  ADC1->CR |= ADC_CR_ADEN; 			/* enable ADC */
  while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* wait for ADC */
  {
  }
  
  //printBits(5, ADC1->ISR );
  //printBits(6, ADC1->CR );

  /* CONFIGURE ADC */

  ADC1->CFGR1 &= ~ADC_CFGR1_EXTEN;	/* software enabled conversion start */
  ADC1->CFGR1 &= ~ADC_CFGR1_ALIGN;		/* right alignment */
  ADC1->CFGR1 &= ~ADC_CFGR1_RES;		/* 12 bit resolution */
  ADC1->CHSELR = 1<<ch; 				/* Select channel */
  ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; /* Select a sampling mode of 111 (very slow)*/

  /* DO CONVERSION */
  
  data = 0;
  for( i = 0; i < 8; i++ )
  {
    
    ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
    while ((ADC1->ISR & ADC_ISR_EOC) == 0) /* wait end of conversion */
    {
    }
    data += ADC1->DR;						/* get ADC result and clear the ISR_EOC flag */
  }
  data >>= 3;
  
  /* DISABLE ADC */
  
  /* at this point the end of sampling and end of sequence bits are also set in ISR registr */  
  if ( (ADC1->CR & ADC_CR_ADEN) != 0 )
  {
    ADC1->CR |= ADC_CR_ADDIS; 			/* disable ADC... maybe better execute a reset */
    while ((ADC1->CR & ADC_CR_ADEN) != 0) 	/* wait for ADC disable, ADEN is also cleared */
    {
    }
  }

  /* DISABLE OTHER PARTS, INCLUDING CLOCK */
  
  ADC->CCR &= ~ADC_CCR_VREFEN; 		/* disable VREFINT */  
  ADC->CCR &= ~ADC_CCR_TSEN; 			/* disable temperature sensor */  
  ADC1->CR &= ~ADC_CR_ADVREGEN;		/* disable ADC voltage regulator */
  RCC->APB2ENR &= ~RCC_APB2ENR_ADCEN;	/* disable ADC clock */
  
  __enable_irq();
  return data;
}

uint16_t getTemperature(void)
{
  int16_t y1, y2,x1, x2, t;
  int16_t y;
  
  
  y1 = 30;
  x1 = *(uint16_t *)(0x1FF8007A);	// 30 degree with 3.0V
  x1 *=30;
  x1 /=33;
  y2 = 110;		// AN3964: 110 degree, Datasheet: 130 degree
  x2 = *(uint16_t *)(0x1FF8007E);	// 130 degree with 3.0V
  x2 *=30;
  x2 /=33;
  t = readADC(18);
  
  y = ( (y2 - y1) * ( t - x1) ) / (x2 - x1) + y1;
  
  return y;
}

uint8_t getBatteryLevels(uint16_t adc, uint16_t cnt)
{
  uint16_t levels;
  if ( adc < 1233 )
    return 0;
  adc -= 1233;
  
  levels = (adc*cnt)/(4096-1223);
  return levels;  
}

void drawBatSymbol(uint16_t adc)
{
  u8g2_uint_t w, levels;
  w = u8g2_GetDisplayWidth(&u8g2);
  u8g2_DrawHLine(&u8g2, w-5, 0, 2);
  u8g2_DrawFrame(&u8g2, w-7, 1, 6, 9);
  levels = getBatteryLevels(adc, 8);
  while( levels > 0 )
  {
    
    u8g2_DrawHLine(&u8g2, w-6, 9-levels, 4);
    levels--;
  }
}


/*=======================================================================*/
int main()
{
  int i;
  uint16_t adc;
  startHSIClock();						/* Increase system clock, must be executed after each reset */
  SystemCoreClockUpdate();				/* Update variable SystemCoreClock, must be executed after each reset */
  startUp();							/* basic system setup + make a backup of PWR_CSR (PWR_CSR_Backup), must be executed after each reset */
  startSysTick();						/* start the sys tick interrupt, must be executed after each reset */
  adjustDST(DST_RULE);					/* adjust DST... ok,this is only done after reset, hopefully this is often enough. This must be called before readRTC() */
  

  /* LED output line */
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA13 */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA13 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* Push/Pull for PA13 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed for PA13 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown for PA13 */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
  GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */


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
    unsigned int r;
    /* Power on reset */
    r = initRTC();
    readRTC();
    initDisplay(1);	/* init display assumes proper values in gui_data */
    if ( r == 0 )
    {
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, MENU_NORMAL_FONT);
	u8g2_DrawStr(&u8g2, 0, 15, "No RTC Clock");
	u8g2_SendBuffer(&u8g2);      
	delay_micro_seconds(3000000);      
	do_reset();
    }
    
    /*set a alarm time for testing */
    //gui_alarm_list[0].enable = 1;
    //gui_alarm_list[0].m = 1;
    //gui_alarm_list[0].wd[5] = 1;
    
    gui_Recalculate();
    
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
  

  if ( DisplayStandbyMode != DISPLAY_STANDYB_MODE_ALWAYS_ON )
  {
    /* before the RTC is enabled via startRTCWakeUp(), avoid key detection if we are in any other mode than ALWAYS_ON  */
    if ( ResetReason == RESET_REASON_TAMP2 || ResetReason == RESET_REASON_TAMP3 )
      isIgnoreNextKey = 1;
  }
  
  startRTCWakeUp();						/* setup wakeup and temper, enable RTC IRQ, probably required after each reset */

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
  
  /* turn on display now */
  u8g2_SetPowerSave(&u8g2, 0);	/* not required for the ALWAYS_ON mode, but does not matter in the other modes */
  //u8g2_SetContrast(&u8g2, DISPLAY_CONTRAST_NORMAL);
  set_contrast();
  
  /* get current voltage level of the battery */
  adc = readADC(5);	



  /* start user loop */
  for(;;)
  {
    if ( RTCUpdateCount == 0 )
    {
      if ( gui_menu.me_list == melist_display_time )
      {
	readRTC();
	gui_SignalTimeChange();
      }
      else
      {
	//readRTC();
	//gui_Recalculate();
      }

      
      RTCUpdateCount = 10;  // update every 10 systicks (half second)
    }
    
    
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

    u8g2_ClearBuffer(&u8g2);
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic set PA13 */
    gui_Draw();
    if ( gui_menu.me_list == melist_display_time )
    {
      u8g2_SetFont(&u8g2, MENU_NORMAL_FONT);
      if ( gui_data.display_voltage )
	u8g2_DrawStr(&u8g2, 0, 8, u8x8_u16toa((adc*330UL)>>12, 3));
      drawBatSymbol(adc);
    }
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic clr PA13 */
    u8g2_SendBuffer(&u8g2);
    
    
    
    if ( MenuIdleTimer > MENU_IDLE_SYSTICK_TIMEOUT )
    {
      if ( gui_data.is_equal  == 0 )	// idea is, that the alarm does not go off during the alarm to avoid another alarm in the same minute
      {
	if ( gui_menu.me_list != melist_display_time )
	{
	  /* jump back to the display menu and redraw the time. not sure if this is required */
	  menu_SetMEList(&gui_menu, melist_display_time, 0);
	  readRTC();
	  gui_SignalTimeChange();
	  u8g2_ClearBuffer(&u8g2);
	  gui_Draw();
	  u8g2_SetFont(&u8g2, MENU_NORMAL_FONT);
	  u8g2_DrawStr(&u8g2, 0, 8, u8x8_u16toa((adc*330UL)>>12, 3));
	  drawBatSymbol(adc);
	  u8g2_SendBuffer(&u8g2);
	}
	
	/* stop everything except RTC */
	enterStandByMode();
      }
      else
      {
	/* read and recalculate so that the gui_data.is_equal is updated */ 
	readRTC();
	gui_Recalculate();
      }
    }
    
    __DSB();  							/* finish memory access */
    __WFI();								/* enter sleep */
    __NOP();

    
  }
  
  
  
}
