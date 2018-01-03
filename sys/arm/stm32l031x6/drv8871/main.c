/* 

  drv8871 DC motor

  Example for the STM32L031 Eval Board with 128x64 OLED at PA13/PA14
  
  IN_A: PA1 / AF2: TIM2_CH2
  IN_B: PB1 / AF?: TIM2_CH4
  VarRes: PA5 / ADC CH5
  Voltage sense: PA6 / ADC CH6
  0.2ms IRQ: PA7 (TIM22_CH2) (optional)
  
  
  IN_A	IN_B	OUT_A	OUT_B
  1		0		1		0
  0		1		0		1	
  0		0		0		0	
  1		1		HiZ		HiZ
  
  
  state machine
  
      
      
  
  
*/

#include <stdio.h>
#include "stm32l031xx.h"
#include "delay.h"
#include "u8g2.h"

/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/*=======================================================================*/
/* global variables */

u8g2_t u8g2;                    // u8g2 object
uint8_t u8g2_x, u8g2_y;         // current position on the screen

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
  
  /* setup display */
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R2, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_tf);
  u8g2_ClearBuffer(&u8g2);
  u8g2_DrawStr(&u8g2, 0,12, "STM32L031");
  u8g2_DrawStr(&u8g2, 0,24, u8x8_u8toa(SystemCoreClock/1000000, 2));
  u8g2_DrawStr(&u8g2, 20,24, "MHz");
  u8g2_SendBuffer(&u8g2);
  
  u8g2_x = 0;
  u8g2_y = 0;  
}


void outChar(uint8_t c)
{
  u8g2_x+=u8g2_DrawGlyph(&u8g2, u8g2_x, u8g2_y, c);
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
  u8g2_x = 0;
  u8g2_y = r;
}

/*=======================================================================*/
/* ADC Init */

void initADC(void)
{
  //__disable_irq();
  
  /* ADC and DMA Clock Enable */
  
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;	/* enable ADC clock */
  RCC->AHBENR |= RCC_AHBENR_DMAEN; 	/* enable DMA clock */
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
  ADC1->CFGR1 = ADC_CFGR1_RES_1;		/* 8 bit resolution */

  
  ADC1->CR |= ADC_CR_ADVREGEN;				/* enable ADC voltage regulator, probably not required, because this is automatically activated */
  ADC->CCR |= ADC_CCR_VREFEN; 			/* Wake-up the VREFINT */  
  ADC->CCR |= ADC_CCR_TSEN; 			/* Wake-up the temperature sensor */  

  __NOP();								/* let us wait for some time */
  __NOP();								/* let us wait for some time */

  /* CALIBRATION */
  
  if ((ADC1->CR & ADC_CR_ADEN) != 0) /* clear ADEN flag if required */
  {
  /* is this correct? i think we must use the disable flag here */
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

}


/*=======================================================================*/
/* ADC Subtasks */

#define ADC_SUB_TASK_NONE 0
#define ADC_SUB_TASK_STOP_ADC 1
#define ADC_SUB_TASK_ENABLE_ADC 2
#define ADC_SUB_TASK_DISABLE_ADC 3
#define ADC_SUB_TASK_CONVERSION 4

#define ADC_SUB_STATE_INIT 0
#define ADC_SUB_STATE_ACTIVE 1
#define ADC_SUB_STATE_DONE 2

volatile uint8_t adc_sub_task = ADC_SUB_TASK_NONE;
volatile uint8_t adc_sub_state = ADC_SUB_STATE_INIT;
uint16_t adc_result = 0;

int adcIsSubDone(void)
{
  if ( adc_sub_state == ADC_SUB_STATE_DONE )
    return 1;
  if ( adc_sub_task == ADC_SUB_TASK_NONE )
    return 1;
  return 0;
}

/*
  int adcStartSubTask(uint8_t msg)

  Args:
    msg: One of ADC_SUB_TASK_STOP_ADC, ADC_SUB_TASK_ENABLE_ADC, ADC_SUB_TASK_DISABLE_ADC

  Returns:
    0 if there is any other subtask active
  
*/
int adcStartSubTask(uint8_t msg)
{
  if ( adcIsSubDone() == 0 )
    return 0;
  adc_sub_task = msg;
  adc_sub_state = ADC_SUB_STATE_INIT;
  return 1;
}

/*
  void adcExecSub(void)

  Desc:
    Executes any active ADC subtask. This can be executed inside an interrupt.
  
*/
void adcExecSub(void)
{
  switch(adc_sub_task)
  {
    case ADC_SUB_TASK_STOP_ADC:
      switch(adc_sub_state)
      {
	case ADC_SUB_STATE_INIT:
	  /* STOP ANY ADC CONVERSION */
	  ADC1->CR |= ADC_CR_ADSTP;
	  adc_sub_state = ADC_SUB_STATE_ACTIVE;
	  /* fall through */
	case ADC_SUB_STATE_ACTIVE:
	  if ( (ADC1->CR & ADC_CR_ADSTP) == 0 )
	    adc_sub_state = ADC_SUB_STATE_DONE;
	  break;
      }      
      break;
    case ADC_SUB_TASK_ENABLE_ADC:
      switch(adc_sub_state)
      {
	case ADC_SUB_STATE_INIT:
	  /* ENABLE ADC (but do not start) */
	  /* after the ADC is enabled, it must not be reconfigured */
	  ADC1->ISR |= ADC_ISR_ADRDY; 			/* clear ready flag */
	  ADC1->CR |= ADC_CR_ADEN; 			/* enable ADC */
	  adc_sub_state = ADC_SUB_STATE_ACTIVE;
	  /* fall through */
	case ADC_SUB_STATE_ACTIVE:
	  if ( (ADC1->ISR & ADC_ISR_ADRDY) != 0 )
	    adc_sub_state = ADC_SUB_STATE_DONE;
	  break;
      }      
      break;
    case ADC_SUB_TASK_DISABLE_ADC:
      switch(adc_sub_state)
      {
	case ADC_SUB_STATE_INIT:
	  /* required to change the configuration of the ADC */
	  /* Check for the ADEN flag. */
	  /* Setting ADDIS will fail if the ADC is alread disabled. */
	  if ((ADC1->CR & ADC_CR_ADEN) == 0) 
	  {
	    adc_sub_state = ADC_SUB_STATE_DONE;
	  }
	  else
	  {
	    ADC1->CR |= ADC_CR_ADDIS;
	    if ( (ADC1->CR & ADC_CR_ADDIS) == 0 )
	      adc_sub_state = ADC_SUB_STATE_DONE;
	    else
	      adc_sub_state = ADC_SUB_STATE_ACTIVE;
	  }
	  break;
	case ADC_SUB_STATE_ACTIVE:
	  if ((ADC1->CR & ADC_CR_ADEN) == 0) 
	  {
	    adc_sub_state = ADC_SUB_STATE_DONE;
	  }
	  if ( (ADC1->CR & ADC_CR_ADDIS) == 0 )
	  {
	    adc_sub_state = ADC_SUB_STATE_DONE;
	  }
	  break;
      }      
      break;
    case ADC_SUB_TASK_CONVERSION:
      switch(adc_sub_state)
      {
	case ADC_SUB_STATE_INIT:
	  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */	
	  adc_sub_state = ADC_SUB_STATE_ACTIVE;
	  /* fall through */
	case ADC_SUB_STATE_ACTIVE:
	  if ( (ADC1->ISR & ADC_ISR_EOC) != 0 )
	  {
	    adc_sub_state = ADC_SUB_STATE_DONE;
	    adc_result = ADC1->DR;
	  }
	  break;
      }      
      break;
  }
}


/* STOP ANY ADC CONVERSION */
void stopADC(void)
{
  //ADC1->CR |= ADC_CR_ADSTP;
  //while(ADC1->CR & ADC_CR_ADSTP)
  //  ;
  
  while( adcStartSubTask(ADC_SUB_TASK_STOP_ADC) == 0 )
    adcExecSub();
  
  while( adcIsSubDone() == 0 )
    adcExecSub();
}

/* CONFIGURATION with ADEN=0 */
/* required to change the configuration of the ADC */
void disableADC(void)
{
  /* Check for the ADEN flag. */
  /* Setting ADDIS will fail if the ADC is alread disabled: The while loop will not terminate */
#ifdef xxxx
  if ((ADC1->CR & ADC_CR_ADEN) != 0) 
  {
    /* is this correct? i think we must use the disable flag here */
    ADC1->CR |= ADC_CR_ADDIS;
    while(ADC1->CR & ADC_CR_ADDIS)
      ;
  }
#endif

  while( adcStartSubTask(ADC_SUB_TASK_DISABLE_ADC) == 0 )
    adcExecSub();
  
  while( adcIsSubDone() == 0 )
    adcExecSub();

}

/* ENABLE ADC (but do not start) */
/* after the ADC is enabled, it must not be reconfigured */
void enableADC(void)
{
  //ADC1->ISR |= ADC_ISR_ADRDY; 			/* clear ready flag */
  //ADC1->CR |= ADC_CR_ADEN; 			/* enable ADC */
  //while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* wait for ADC */
  //{
  //}

  while( adcStartSubTask(ADC_SUB_TASK_ENABLE_ADC) == 0 )
    adcExecSub();
  
  while( adcIsSubDone() == 0 )
    adcExecSub();
  
}


/*=======================================================================*/
/* ADC Single Conversion */


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

uint8_t adc_single_conversion_channel = 5;
volatile uint8_t adc_single_conversion_state = 0;
uint16_t adc_single_conversion_result;

int adcStartSingleConversion(uint8_t channel)
{
  if ( adc_single_conversion_state != 0 )
    return 0;
  adc_single_conversion_state = 1;
  adc_single_conversion_channel = channel;
  return 1;
}

void adcExecSingleConversion(void)
{
  switch(adc_single_conversion_state)
  {
    case 1:
      if ( adcStartSubTask(ADC_SUB_TASK_STOP_ADC) == 0 )
      {
	adcExecSub();
	break;
      }
      adc_single_conversion_state++;
      /* fall through */
    case 2:
      if ( adcIsSubDone() == 0 )
      {
	adcExecSub();
	break;
      }
      adc_single_conversion_state++;
      /* fall through */
    case 3:
      if ( adcStartSubTask(ADC_SUB_TASK_DISABLE_ADC) == 0 )
      {
	adcExecSub();
	break;
      }
      adc_single_conversion_state++;
      /* fall through */
    case 4:
      if ( adcIsSubDone() == 0 )
      {
	adcExecSub();
	break;
      }
      
      /* CONFIGURE ADC */
      
      //ADC1->CFGR1 &= ~ADC_CFGR1_EXTEN;	/* software enabled conversion start */
      //ADC1->CFGR1 &= ~ADC_CFGR1_ALIGN;		/* right alignment */
      ADC1->CFGR1 = ADC_CFGR1_RES_1;		/* 8 bit resolution */
      //ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; /* Select a sampling mode of 111 (very slow)*/
      ADC1->SMPR  = 0;
      
      adc_single_conversion_state++;
      /* fall through */
    case 5:
      if ( adcStartSubTask(ADC_SUB_TASK_ENABLE_ADC) == 0 )
      {
	adcExecSub();
	break;
      }
      adc_single_conversion_state++;
      /* fall through */
    case 6:
      if ( adcIsSubDone() == 0 )
      {
	adcExecSub();
	break;
      }
      
      ADC1->CHSELR = 1<<adc_single_conversion_channel; 				/* Select channel (can be done also if ADC is enabled) */
      adc_single_conversion_state++;
      /* fall through */      
    case 7:
      if ( adcStartSubTask(ADC_SUB_TASK_CONVERSION) == 0 )
      {
	adcExecSub();
	break;
      }
      adc_single_conversion_state++;
      /* fall through */
    case 8:
      if ( adcIsSubDone() == 0 )
      {
	adcExecSub();
	break;
      }
      adc_single_conversion_result = adc_result;
      adc_single_conversion_state = 0;
      break;
  }
}

uint16_t getADC(uint8_t ch)
{
  while( adcStartSingleConversion(ch) == 0)
    adcExecSingleConversion();
  while( adc_single_conversion_state != 0 )
    adcExecSingleConversion();
  return adc_single_conversion_result;
}



/*=======================================================================*/
/* ADC Multi (DMA) Conversion */

uint8_t adc_multi_conversion_channel = 6;
volatile uint8_t adc_multi_conversion_state = 0;
uint16_t adc_multi_conversion_count = 256;
uint16_t *adc_multi_conversion_buffer = NULL;

int adcStartMultiConversion(uint8_t channel, uint16_t cnt, uint16_t *buf)
{
  if ( adc_multi_conversion_state != 0 )
    return 0;
  adc_multi_conversion_state = 1;
  adc_multi_conversion_channel = channel;
  adc_multi_conversion_count = cnt;
  adc_multi_conversion_buffer = buf;
  return 1;
}


void adcExecMultiConversion(void)
{
  switch(adc_multi_conversion_state)
  {
    case 1:
      if ( adcStartSubTask(ADC_SUB_TASK_STOP_ADC) == 0 )
      {
	adcExecSub();
	break;
      }
      adc_multi_conversion_state++;
      /* fall through */
    case 2:
      if ( adcIsSubDone() == 0 )
      {
	adcExecSub();
	break;
      }
      adc_multi_conversion_state++;
      /* fall through */
    case 3:
      if ( adcStartSubTask(ADC_SUB_TASK_DISABLE_ADC) == 0 )
      {
	adcExecSub();
	break;
      }
      adc_multi_conversion_state++;
      /* fall through */
    case 4:
      if ( adcIsSubDone() == 0 )
      {
	adcExecSub();
	break;
      }
      
      /* CONFIGURE ADC */

      /* disable and reset to defaults */
      DMA1_Channel1->CCR = 0;
      
      /* defaults: 
	  - 8 Bit access	--> will be changed below
	  - read from peripheral	--> ok
	  - none-circular mode  --> ok
	  - no increment mode   --> will be changed below
      */
      
      
      DMA1_Channel1->CNDTR = adc_multi_conversion_count;                                        /* buffer size */
      DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);                     /* source value */
      //  DMA1_Channel1->CPAR = (uint32_t)&(GPIOA->ODR);                    /* source value */
      DMA1_Channel1->CMAR = (uint32_t)adc_multi_conversion_buffer;                   /* destination memory */

      DMA1_CSELR->CSELR &= ~DMA_CSELR_C1S;         /* 0000: select ADC for DMA CH 1 (this is reset default) */
      DMA1_CSELR->CSELR &= ~DMA_CSELR_C2S;         /* 0000: select ADC for DMA CH 2 (this is reset default) */
      
      DMA1_Channel1->CCR |= DMA_CCR_MINC;		/* increment memory */   
      DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;		/* 01: 16 Bit access */   
      DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;		/* 01: 16 Bit access */   
      
      DMA1_Channel1->CCR |= DMA_CCR_EN;                /* enable */

      
      /* 
	detect rising edge on external trigger (ADC_CFGR1_EXTEN_0)
	recive trigger from TIM2 (ADC_CFGR1_EXTSEL_1)  
	8 Bit resolution (ADC_CFGR1_RES_1)
      
	Use DMA one shot mode and enable DMA (ADC_CFGR1_DMAEN)
	Once DMA is finished, it will disable continues mode (ADC_CFGR1_CONT)
      */
      
      
      ADC1->CFGR1 = 
	    ADC_CFGR1_CONT				/* continues mode */
	    | ADC_CFGR1_EXTEN_0 	/* rising edge */
	//    | ADC_CFGR1_EXTEN_1 			/*  */	    
	    | ADC_CFGR1_EXTSEL_1 			/* TIM2 */
    //	| ADC_CFGR1_RES_1				/* 8 Bit resolution, no value means 12 bit */
	    | ADC_CFGR1_DMAEN;			/* enable generation of DMA requests */

      //ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; 
      //ADC1->SMPR = ADC_SMPR_SMP_1 ; 
      //ADC1->SMPR = ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 ; 
      ADC1->SMPR = ADC_SMPR_SMP_2 ; 
      
      /*
	12.5 + 8.5 = 21 ADC Cycles pre ADC sampling
	4 MHz / 21 cycle / 256 = 744 Hz
      */
      adc_multi_conversion_state++;
      /* fall through */
    case 5:
      if ( adcStartSubTask(ADC_SUB_TASK_ENABLE_ADC) == 0 )
      {
	adcExecSub();
	break;
      }
      adc_multi_conversion_state++;
      /* fall through */
    case 6:
      if ( adcIsSubDone() == 0 )
      {
	adcExecSub();
	break;
      }
      
      ADC1->CHSELR = 1<<adc_multi_conversion_channel; 				/* Select channel (can be done also if ADC is enabled) */
      /* conversion will be started automatically with rising edge of TIM2, yet ADSTART is still required */
      ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
      adc_multi_conversion_state++;
      
      /* fall through */      
    case 7:
      if ( DMA1_Channel1->CNDTR > 0 )
	break;
      adc_multi_conversion_state = 0;
      break;
  }
}


void scanADC(uint8_t ch, uint16_t cnt, uint16_t *buf)
{
  while( adcStartMultiConversion(ch, cnt, buf) == 0)
    adcExecMultiConversion();
  while( adc_multi_conversion_state != 0 )
    adcExecMultiConversion();
}



/*=======================================================================*/
/*
  5000Hz Data Acquisition

  Acqusition:
  1./2. Read DC Motor Voltage into buffer 1
  3./4. Read DC Motor Voltage into buffer 2   
  5./6. Read signle ADC from the variable resistor 
  parallel: Calculate noise via difference signal


*/

volatile uint16_t adc_variable_resistor_value = 0;
volatile uint8_t adc_acquisition_state = 0;
volatile uint8_t adc_calculation_state = 0;

#define BUF_MUL 2

uint16_t adc_buf[128*BUF_MUL];
uint16_t adc_buf2[128*BUF_MUL];
uint16_t adc_diff[128*BUF_MUL];
uint32_t adc_diff_sum_tmp = 0;
uint16_t adc_diff_sum_cnt = 0;
volatile uint32_t adc_diff_sum = 0;
volatile uint16_t adc_diff_noise_per_sample_raw = 0;		// scaled by 8 bits
volatile uint16_t adc_diff_noise_per_sample_filt = 0;		// scaled by 8 bits
volatile uint16_t adc_max_tmp = 0;
volatile uint16_t adc_max_raw = 0;
volatile uint16_t adc_max_filt = 0;
volatile uint16_t adc_calculation_pos;

/* 128*BUF_MUL / ADC_CALC_PER_STEP must have no reminder */
#define ADC_CALC_PER_STEP 32
void adcExecAcquisition(void)
{
  uint16_t i;
  uint16_t a, b, d, z;
  switch(adc_acquisition_state)
  {
    case 1:
      if ( adcStartMultiConversion(6, 128*BUF_MUL, adc_buf) == 0)
      {
	adcExecMultiConversion();
	break;
      }
      adc_acquisition_state++;
      /* fall through */
    case 2:
      if ( adc_multi_conversion_state != 0 )
      {
	adcExecMultiConversion();
	break;
      }
      adc_acquisition_state++;
      /* fall through */
    case 3:
      if ( adcStartMultiConversion(6, 128*BUF_MUL, adc_buf2) == 0)
      {
	adcExecMultiConversion();
	break;
      }
      adc_acquisition_state++;
      /* fall through */
    case 4:
      if ( adc_multi_conversion_state != 0 )
      {
	adcExecMultiConversion();
	break;
      }
      adc_acquisition_state++;
      adc_calculation_state = 1;
      adc_calculation_pos = 0;
      adc_diff_sum_tmp = 0;
      adc_diff_sum_cnt = 0;
      adc_max_tmp = 0;
      /* fall through */
    case 5:
      if ( adcStartSingleConversion(5) == 0)
      {
	adcExecSingleConversion();
	break;
      }
      adc_acquisition_state++;
      /* fall through */
    case 6:
      if ( adc_single_conversion_state != 0 )
      {
	adcExecSingleConversion();
	break;
      }
      adc_variable_resistor_value = adc_single_conversion_result;
      adc_acquisition_state++;
      /* fall through */
    case 7:
      if ( adc_calculation_state >= 2 )		// wait for calculation
      {
	adc_acquisition_state = 1;
	adc_calculation_state = 0;
      }
      break;
  }
  
  switch(adc_calculation_state)
  {
    case 1:      
      i = adc_calculation_pos;
      adc_calculation_pos += ADC_CALC_PER_STEP;
      if ( adc_calculation_pos >= 128U*BUF_MUL )
	adc_calculation_pos = 128U*BUF_MUL;
      while( i < adc_calculation_pos )
      {
	a = adc_buf[i];
	b = adc_buf2[i];
	if ( a > b )
	  d = a - b;
	else
	  d = b - a;
	/* ignore values around 0 and very large differences (spikes) 
	  At least values 0 and 1 for a should be ignored.
	  Height of the spices is not really clear. 
	*/
	if ( a > 4 && b > 4 && d < 24)
	{
	  adc_diff_sum_tmp += d;
	  adc_diff_sum_cnt++;
	  z = a + b;
	  z >>= 1;
	  if ( adc_max_tmp < z )
	    adc_max_tmp = z;
	}
	adc_diff[i] = d;
	i++;
      }
      
      if ( adc_calculation_pos >= 128U*BUF_MUL )
      {
	adc_calculation_pos = 0;
	adc_diff_sum = adc_diff_sum_tmp;
	adc_diff_noise_per_sample_raw = (adc_diff_sum_tmp * 256UL)/adc_diff_sum_cnt;
	/*
	  this is a strong low-pass filter
	  currently the filter value is calculated with 100Hz (every 5th duty cycle)
	  3V DC Motor: adc_diff_noise_per_sample_filt < 0x0200 stop, adc_diff_noise_per_sample_filt > 0x0250 running
	*/
        adc_diff_noise_per_sample_filt = (((((1UL<<5) - 1)*(uint32_t)adc_diff_noise_per_sample_filt)) + (uint32_t)((1*adc_diff_noise_per_sample_raw)))>>5; 
	
	/*
	  low-pass filter for the max value of the ADC.
	  If the DC motor rotates, then the max value indicates speed: lower values are faster, higher values are slower
	  3V DC Motor: values are from 0x01d (fastest) to 0x90 (almost stopped)
	*/
	adc_max_raw = adc_max_tmp;
        adc_max_filt = (((((1UL<<5) - 1)*(uint32_t)adc_max_filt)) + (uint32_t)((1*adc_max_raw))) >> 5; 
	
        adc_calculation_state++;
      }
      break;
  }
  
}



/*=======================================================================*/
/* TIM2: PWM signal for the DC Motor */
//#define TIM_CYCLE_TIME 5355
/* 7950 --> 500Hz */
#define TIM_CYCLE_TIME 7950
#define TIM_CYCLE_UPPER_SKIP 100
#define TIM_CYCLE_LOWER_SKIP 200


void initTIM2(uint8_t is_gpio_a)
{
  /* enable clock for TIM2 */
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  
  /* prescalar for AHB and APB1 */
  
  /* reselt defaults for HPRE and PPRE1: no clock division */
  // RCC->CFGR &= ~RCC_CFGR_HPRE;
  // RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  // RCC->CFGR &= ~RCC_CFGR_PPRE1;
  // RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
  
  /* configure GPIOA PA1 for TIM2 */
  GPIOA->MODER &= ~GPIO_MODER_MODE1;	/* clear mode for PA1 */  
  GPIOA->MODER |= GPIO_MODER_MODE1_1;  /* alt fn */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1;    /* push-pull */
  GPIOA->AFR[0] &= ~(15<<4);            /* Clear Alternate Function PA1 */
  GPIOA->AFR[0] |= 2<<4;                   /* AF2 Alternate Function PA1 */

  /* configure GPIOA PB1 for TIM2 */
  GPIOB->MODER &= ~GPIO_MODER_MODE1;	/* clear mode for PB1 */  
  GPIOB->MODER |= GPIO_MODER_MODE1_1;  /* alt fn */
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT_1;    /* push-pull */
  GPIOB->AFR[0] &= ~(15<<4);            /* Clear Alternate Function PB1 */
  GPIOB->AFR[0] |= 5<<4;                   /* AF5 Alternate Function PB1 */

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
  
  TIM2->CR2 |= TIM_CR2_MMS_1;		/* Update event for TRGO */
  
  TIM2->ARR = TIM_CYCLE_TIME;                         /* total cycle count */
  TIM2->CCR2 = 1024;                            /* duty cycle for channel 2 (PA1) */
  TIM2->CCR4 = 1024;                            /* duty cycle for channel 4 (PB1) */
  
  //TIM2->CCMR1 &= ~TIM_CCMR1_OC2CE;      /* disable clear output compare 2 **/
  TIM2->CCMR1 |= TIM_CCMR1_OC2M;            /* all 3 bits set: PWM Mode 2 */
  TIM2->CCMR1 |= TIM_CCMR1_OC2PE;            /* preload enable CCR2 is preloaded*/
  TIM2->CCER |= TIM_CCER_CC2P;                     /* polarity 0: normal (reset default) / 1: inverted*/

  TIM2->CCMR2 |= TIM_CCMR2_OC4M;            /* all 3 bits set: PWM Mode 2 */
  TIM2->CCMR2 |= TIM_CCMR2_OC4PE;            /* preload enable CCR2 is preloaded*/
  TIM2->CCER |= TIM_CCER_CC4P;                     /* polarity 0: normal (reset default) / 1: inverted*/
  
  
  
  if ( is_gpio_a )
    TIM2->CCER |= TIM_CCER_CC2E;                     /* set output enable for channel 2 */
  else
    TIM2->CCER |= TIM_CCER_CC4E;                     /* set output enable for channel 4 */
  
  TIM2->PSC = 7;						/* divide by 8 */

  
  TIM2->CR1 |= TIM_CR1_CEN;            /* counter enable */
  
  /*
    TIM2 cycle:
    32000000Hz / 5355 / 8 = 747 Hz  
  */
}

void setTIM2RawDuty(uint32_t duty_cycle, uint8_t is_gpio_a)
{
  TIM2->CCR2 = duty_cycle;
  TIM2->CCR4 = duty_cycle;
  
  if ( is_gpio_a )
  {
    TIM2->CCMR1 |= TIM_CCMR1_OC2M;            /* all 3 bits set: PWM Mode 2 */
    TIM2->CCER |= TIM_CCER_CC2E;                     /* set output enable for channel 2 */
    
    //TIM2->CCER &= ~TIM_CCER_CC4E;                     /* set output disable for channel 4 */    
    TIM2->CCMR2 &= ~TIM_CCMR2_OC4M_1;            /* Mode 101 force high */

  }
  else
  {
    TIM2->CCMR2 |= TIM_CCMR2_OC4M;            /* all 3 bits set: PWM Mode 2 */
    TIM2->CCER |= TIM_CCER_CC4E;                     /* set output enable for channel 4 */
    
    //TIM2->CCER &= ~TIM_CCER_CC2E;                     /* set output disable for channel 2 */
    TIM2->CCMR1 &= ~TIM_CCMR1_OC2M_1;            /* Mode 101 force high */
  }
}




/*=======================================================================*/
/* TIM22 */

/* 
  TIM22: 0.2ms IRQ 

  Assumptions:
    APB2: 32MHz
    GPIO A anabled
*/
void initTIM22(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_TIM22EN;
  
  /* configure GPIOA PA7 for TIM2 CH2*/
  GPIOA->MODER &= ~GPIO_MODER_MODE7;	/* clear mode for PA1 */  
  GPIOA->MODER |= GPIO_MODER_MODE7_1;  /* alt fn */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7;    /* push-pull */
  GPIOA->AFR[0] &= ~(15<<28);            /* Clear Alternate Function PA7 */
  //GPIOA->AFR[0] |= 5<<28;                   /* AF5 Alternate Function PA7 NOTE: OUTPUT at PA7 influences ADC! */
  
  TIM22->CR2 |= TIM_CR2_MMS_1;		/* Update event for TRGO */
  TIM22->ARR = 6400;				/* 0.2ms (5000Hz) with 32MHz */
  TIM22->CCR2 = 2000;                            /* duty cycle for channel 2 (PA7) */
  
  TIM22->CCMR1 |= TIM_CCMR1_OC2M;            /* all 3 bits set: PWM Mode 2 */
  TIM22->CCMR1 |= TIM_CCMR1_OC2PE;            /* preload enable --> more accurate duty cycle visible */
  TIM22->CCER |= TIM_CCER_CC2E;                     /* set output enable for channel 2 */
  TIM22->CCER |= TIM_CCER_CC2P;                     /* polarity 0: normal (reset default) / 1: inverted*/
  TIM22->PSC = 0;						/* divide by 1 */
    
  TIM22->DIER |= TIM_DIER_UIE;			/* enable TIM22 update interrupt: call TIM22_IRQHandler on reload */

  /* enable IRQ in NVIC */
  NVIC_SetPriority(TIM22_IRQn, 0);
  NVIC_EnableIRQ(TIM22_IRQn);
  
  
  TIM22->CR1 |= TIM_CR1_CEN;            /* counter enable */
  
  adc_acquisition_state = 1;			/* enable data acquisition */

}

volatile uint16_t adc_max;

void __attribute__ ((interrupt, used)) TIM22_IRQHandler(void)
{
  
/*
  the following loop requires about 5000 clock cycles 1/3 of the IRQ time:
  
  uint16_t i;
  adc_max = 0;
  for( i = 0; i < 256; i++ )
  {
    adc_max += TIM22->CNT;
  }
*/

  adcExecAcquisition();

  TIM22->CCR2 = TIM22->CNT;		/* store the current count value in compare register: duty cycle signals load */
  TIM22->SR &= ~TIM_SR_UIF;			/* clear interrupt */
}


/*=======================================================================*/


void main()
{
  uint16_t adc_value;
  uint16_t tim_duty;
  uint16_t zero_pos;
  uint16_t i;
  u8g2_uint_t y, yy;
  
  setHSIClock();        /* enable 32 MHz Clock */
  startUp();               /* enable systick irq and several power regions  */
  initDisplay();          /* aktivate display */
  initADC();

  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  RCC->IOPENR |= RCC_IOPENR_IOPBEN;		/* Enable clock for GPIO Port B */
  __NOP();
  __NOP();
  
  GPIOA->MODER &= ~GPIO_MODER_MODE1;	/* clear mode for PA1 */
  GPIOA->MODER |= GPIO_MODER_MODE1_0;	/* Output mode for PA1 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1;	/* no Push/Pull for PA1 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED1;	/* low speed for PA1 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;	/* no pullup/pulldown for PA1 */
  GPIOA->BSRR = GPIO_BSRR_BS_1;		/* atomic set PA1 */

  GPIOB->MODER &= ~GPIO_MODER_MODE1;	/* clear mode for PB1 */
  GPIOB->MODER |= GPIO_MODER_MODE1_0;	/* Output mode for PB1 */
  //GPIOB->OTYPER &= ~GPIO_OTYPER_OT_1;	/* no Push/Pull for PB1 */
  GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED1;	/* low speed for PB1 */
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD1;	/* no pullup/pulldown for PB1 */
  GPIOB->BSRR = GPIO_BSRR_BR_1;		/* atomic reset PB1 */


  initTIM2(1);
  initTIM22();
  
  

  for(;;)
  {
    
    u8g2_ClearBuffer(&u8g2);
   

    //adc_value = getADC(5);
    adc_value = adc_variable_resistor_value;
    if ( adc_value >= 0x080 )
    {
      adc_value -= 0x080;
      adc_value *= 2;
      tim_duty = ((uint32_t)adc_value*((uint32_t)TIM_CYCLE_TIME-TIM_CYCLE_UPPER_SKIP-TIM_CYCLE_LOWER_SKIP))>>8;
      tim_duty += TIM_CYCLE_LOWER_SKIP;      
      setTIM2RawDuty(tim_duty, 1);
    }
    else
    {
      adc_value = 0x080 - adc_value;
      adc_value *= 2;
      tim_duty = ((uint32_t)adc_value*((uint32_t)TIM_CYCLE_TIME-TIM_CYCLE_UPPER_SKIP-TIM_CYCLE_LOWER_SKIP))>>8;
      tim_duty += TIM_CYCLE_LOWER_SKIP;      
      setTIM2RawDuty(tim_duty, 0);
    }
    
    
    /*
    
    TIM2->SR &= ~TIM_SR_UIF;
    while( (TIM2->SR & TIM_SR_UIF) == 0 )
      ;
    
    yy = 30;
    for( i = 0; i < 128; i++ )
    {
      y = 30-(gpio_buf[i*BUF_MUL]&2)*2;
      u8g2_DrawPixel(&u8g2, i, y);
      if ( y < yy )
	u8g2_DrawVLine(&u8g2, i, y, yy-y+1);
      else
	u8g2_DrawVLine(&u8g2, i, yy, y-yy+1);
      yy = y;
    }
    */
    
    

    /*
    for( i = 0; i < 128*BUF_MUL; i++ )
    {
      adc_buf[i] = i;
    }
    
    scanADC(6, 128*BUF_MUL, adc_buf);
    */

    yy = 60;
    
    zero_pos = ((uint32_t)tim_duty * (uint32_t)256) / (uint32_t)TIM_CYCLE_TIME;
    zero_pos +=4;
    zero_pos += (256-zero_pos)>>6;

    setRow(10); outHex16(adc_value); 
    outStr(" "); outHex16(adc_diff_noise_per_sample_filt); 
    //outStr(" ");  outHex16(adc_diff_sum_cnt); 
    outStr(" ");  outHex16(adc_max_raw); 
    outStr(" ");  outHex16(adc_max_filt); 
    //outStr("|"); outHex8(adc_buf[zero_pos/2]); outStr("|"); outHex8(adc_buf[zero_pos]); 
    
    u8g2_DrawVLine(&u8g2, zero_pos/2, yy-7, 15);
    u8g2_DrawVLine(&u8g2, zero_pos/4, yy-7, 15);
    for( i = 0; i < 128; i++ )
    {
      y = 60-(adc_buf[i*BUF_MUL]>>5);
      //y = 60-(adc_diff[i*BUF_MUL]>>2);
      u8g2_DrawPixel(&u8g2, i, y);
      if ( y < yy )
	u8g2_DrawVLine(&u8g2, i, y, yy-y+1);
      else
	u8g2_DrawVLine(&u8g2, i, yy, y-yy+1);
      yy = y;
    }

    u8g2_SendBuffer(&u8g2);
    
  }
  
}
