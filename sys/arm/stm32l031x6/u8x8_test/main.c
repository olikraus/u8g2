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

void printBits(uint8_t y, uint16_t val)
{
  int i;
  for( i = 0; i < 16; i++ )
  {
    u8x8_DrawGlyph(&u8x8, i, y, val & (1<<(15-i)) ? '1' : '0' );
  }
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
  
  u8x8_DrawString(&u8x8, 0,6, u8x8_u16toa((y2 - y1)/(x2 - x1), 5));
  u8x8_DrawString(&u8x8, 7,6, u8x8_u16toa(t, 5));
  u8x8_DrawString(&u8x8, 13,6, u8x8_u16toa(y, 3));
  
  return y;
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
  u8x8_DrawString(&u8x8, 0,2, "Vref:");
  u8x8_DrawString(&u8x8, 7,2, u8x8_u16toa(readADC(17), 4));  
  u8x8_DrawString(&u8x8, 0,3, "Temp:");
  u8x8_DrawString(&u8x8, 7,3, u8x8_u16toa(readADC(18), 4));
  u8x8_DrawString(&u8x8, 13,3, u8x8_u16toa(getTemperature(), 3));
  u8x8_DrawString(&u8x8, 0,4, "c30:");
  u8x8_DrawString(&u8x8, 7,4, u8x8_u16toa(*(uint16_t *)(0x1FF8007A), 4));
  u8x8_DrawString(&u8x8, 0,5, "c130:");
  u8x8_DrawString(&u8x8, 7,5, u8x8_u16toa(*(uint16_t *)(0x1FF8007E), 4));


  for(;;)
  {
  u8x8_DrawString(&u8x8, 0,2, "Vref:");
  u8x8_DrawString(&u8x8, 7,2, u8x8_u16toa(readADC(17), 4));  
  u8x8_DrawString(&u8x8, 0,3, "Temp:");
  u8x8_DrawString(&u8x8, 7,3, u8x8_u16toa(readADC(18), 4));
  u8x8_DrawString(&u8x8, 13,3, u8x8_u16toa(getTemperature(), 3));
  u8x8_DrawString(&u8x8, 0,4, "c30:");
  u8x8_DrawString(&u8x8, 7,4, u8x8_u16toa(*(uint16_t *)(0x1FF8007A), 4));
  u8x8_DrawString(&u8x8, 0,5, "c130:");
  u8x8_DrawString(&u8x8, 7,5, u8x8_u16toa(*(uint16_t *)(0x1FF8007E), 4));
    delay_micro_seconds(500000);
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */
    delay_micro_seconds(500000);
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
  }
}
