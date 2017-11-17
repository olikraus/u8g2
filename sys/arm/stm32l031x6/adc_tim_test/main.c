/* 

  adc_tim_test

  Example for the STM32L031 Eval Board with 128x64 OLED at PA13/PA14
  
  LED: PA1 / AF2: TIM2_CH2
  VarRes: PA5 / ADC CH5
  
  
*/

#include <stdio.h>
#include "stm32l031xx.h"
#include "delay.h"
#include "u8x8.h"

/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/*=======================================================================*/
/* global variables */

u8x8_t u8x8;                    // u8x8 object
uint8_t u8x8_x, u8x8_y;         // current position on the screen

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

  SystemCoreClockUpdate();				/* Update SystemCoreClock global variable */  
}

/*
  Enable several power regions: PWR, GPIOA

  This must be executed after each reset.
*/
void startUp(void)
{  
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;	/* enable power interface (PWR) */
  PWR->CR |= PWR_CR_DBP;				/* activate write access to RCC->CSR and RTC */  
  
  SysTick->LOAD = (SystemCoreClock/1000)*50 - 1;   /* 50ms task */
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */      
}

/*=======================================================================*/
/* u8x8 display procedures */

void initDisplay(void)
{
  u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);
  u8x8_InitDisplay(&u8x8);
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetPowerSave(&u8x8, 0);
  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_r);  
  u8x8_x = 0;
  u8x8_y = 0;  
}


void outChar(uint8_t c)
{
  if ( u8x8_x >= u8x8_GetCols(&u8x8) )
  {
    u8x8_x = 0;
    u8x8_y++;
  }
  u8x8_DrawGlyph(&u8x8, u8x8_x, u8x8_y, c);
  u8x8_x++;
}

void outStr(const char *s)
{
  while( *s )
    outChar(*s++);
}

void outHexHalfByte(uint8_t b)
{
  b &= 0x0f;
  if ( b < 10 )
    outChar(b+'0');
  else
    outChar(b+'a'-10);
}

void outHex8(uint8_t b)
{
  outHexHalfByte(b >> 4);
  outHexHalfByte(b);
}

void outHex16(uint16_t v)
{
  outHex8(v>>8);
  outHex8(v);
}

void outHex32(uint32_t v)
{
  outHex16(v>>16);
  outHex16(v);
}

void setRow(uint8_t r)
{
  u8x8_x = 0;
  u8x8_y = r;
}

/*=======================================================================*/

void initADC(void)
{
  //__disable_irq();
  
  /* ADC Clock Enable */
  
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;	/* enable ADC clock */
  __NOP();								/* let us wait for some time */
  __NOP();								/* let us wait for some time */  
  
  /* ADC Reset */
  
  RCC->APB2RSTR |= RCC_APB2RSTR_ADCRST;
  __NOP();								/* let us wait for some time */
  __NOP();								/* let us wait for some time */
  RCC->APB2RSTR &= ~RCC_APB2RSTR_ADCRST;
  __NOP();								/* let us wait for some time */
  __NOP();								/* let us wait for some time */
 
  /* ADC Basic Setup */
  
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
  /* is this correct, i think we must use the disable flag here */
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
}

/*
  ch0   PA0     pin 6
  ch1   PA1     pin 7
  ch2   PA2     pin 8
  ch3   PA3     pin 9
  ch4   PA4     pin 10
  ch5   PA5     pin 11
  ch6   PA6     pin 12
  ch7   PA7     pin 13
  ch8   PB0     -
  ch9   PB1     pin 14


  ch 0..15: 	GPIO
  ch 16:		???
  ch 17:		vref (bandgap)
  ch18:		temperature sensor

  returns 12 bit result, right aligned 
*/
uint16_t getADC(uint8_t ch)
{
  uint32_t data;
  uint32_t i;
  
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
  
  return data;
}

/*=======================================================================*/

void initTIM(void)
{
  /* enable clock for TIM2 */
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  
  /*enable clock for GPIOA */
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  
    __NOP();                                                          /* extra delay for clock stabilization required? */
    __NOP();
  
  /* prescalar for AHB and APB1 */
  
  /* reselt defaults for HPRE and PPRE1: no clock division */
  // RCC->CFGR &= ~RCC_CFGR_HPRE;
  // RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  // RCC->CFGR &= ~RCC_CFGR_PPRE1;
  // RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
  
  /* configure GPIOA PA1 for TIM2 */
  GPIOA->MODER &= ~GPIO_MODER_MODE1;	/* clear mode for PA9 */  
  GPIOA->MODER |= GPIO_MODER_MODE1_1;  /* alt fn */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1;    /* push-pull */
  GPIOA->AFR[0] &= ~(15<<4);            /* Clear Alternate Function PA1 */
  GPIOA->AFR[0] |= 2<<4;                   /* AF2 Alternate Function PA1 */
  
  /* TIM2 configure */
  /* disable all interrupts */
  //TIM2->DIER = 0;             /* 0 is reset default value */
  
  /* clear everything, including the "Update disable" flag, so that updates */
  /* are generated */
  // TIM2->CR1 = 0;             /* 0 is reset default value */
  //TIM2->CR1 |= TIM_CR1_ARPE;  // ARR is not modified so constant update is ok
  /* Update request by manual UG bit setting or slave controller */
  /* both is not required here */
  /* so, update request by couter over/underflow remains */
  //TIM2->CR1 |= TIM_CR1_URS;             /* only udf/ovf generae events */
  
  
  TIM2->ARR = 4096;                              /* total cycle count */
  TIM2->CCR2 = 1024;                            /* duty cycle */
  //TIM2->CCMR1 &= ~TIM_CCMR1_OC2CE;      /* disable clear output compare 2 **/
  TIM2->CCMR1 |= TIM_CCMR1_OC2M;            /* all 3 bits set: PWM Mode 2 */
  //TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_0;     /* 110: PWM Mode 1 */
  TIM2->CCMR1 |= TIM_CCMR1_OC2PE;            /* preload enable CCR2 is preloaded*/
  // TIM2->CCMR1 &= ~TIM_CCMR1_OC2FE;            /* fast disable (reset default) */
  // TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;               /* configure cc2 as output (this is reset default) */
  
  
  //TIM2->EGR  |=  TIM_EGR_CC2G;              /* capture event cc2 */
  TIM2->CCER |= TIM_CCER_CC2E;                     /* set output enable */
  //TIM2->CCER |= TIM_CCER_CC2P;                     /* polarity 0: normal (reset default) / 1: inverted*/
  
  TIM2->PSC = 7;
  
  TIM2->CR1 |= TIM_CR1_CEN;            /* counter enable */
}

/*=======================================================================*/

void main()
{
  uint16_t adc_value;
  uint16_t i;
  
  setHSIClock();        /* enable 32 MHz Clock */
  startUp();               /* enable systick irq and several power regions  */
  initDisplay();          /* aktivate display */
  initADC();

  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();
  GPIOA->MODER &= ~GPIO_MODER_MODE1;	/* clear mode for PA1 */
  GPIOA->MODER |= GPIO_MODER_MODE1_0;	/* Output mode for PA1 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1;	/* no Push/Pull for PA1 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED1;	/* low speed for PA1 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;	/* no pullup/pulldown for PA1 */
  GPIOA->BSRR = GPIO_BSRR_BS_1;		/* atomic set PA1 */
  
  initTIM();
  
  
  setRow(0); outStr("ADC Test"); 

  setRow(2); outStr("ch5 pin11: "); 
  setRow(3); outHex16(getADC(5)); 
  setRow(4); outStr("bandgap:   "); 
  setRow(5); outHex16(getADC(17)); 
  setRow(6); outStr("temp:      "); 
  setRow(7); outHex16(getADC(18)); 

  for(;;)
  {
    
    adc_value = getADC(5);
    TIM2->CCR2 = adc_value;
    setRow(3); outHex16(adc_value); 
    
  }
  
}
