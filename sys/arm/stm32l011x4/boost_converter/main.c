/* 
  boost converter project for the STM32L011x4

  STM32L011D3P	D=14Pin 3=8KB P=TTSOP
  STM32L011D4P	D=14Pin 4=16KB P=TTSOP
  STM32L011F3P	F=20Pin 3=8KB P=TTSOP
  STM32L011F4P	F=20Pin 4=16KB P=TTSOP
  
  
  TSSOP20:
  
  PB9/BOOT	1				20	PA14
  PC14		2				19	PA13
  PC15		3				18	PA10 TIM21_CH1, TIM2_CH3
  Reset		4				17	PA9	TIM21_CH2
  VDDA		5				16	VDD
  PA0 C1-		6				15	GND
  PA1 C1+	7				14	PB1 ADC9
  PA2 C2-	TX	8				13	PA7	C2+
  PA3 C2+ RX	9				12	PA6	ADC6
  PA4 C1- C2-	10				11	PA5 C1- C2-
  
  
  Comp 2:  
    Input: REFINT /4
    Input: PA7 (C2+), 
    Output: PA9 (TIM21_CH2)
    
  Comp 1:
    Input: REFINT
    Input: PA1 (C1+)
    Output: PA10 (TIM2_CH3)
    
  
*/

#include "stm32l011xx.h"

/*===============================================*/
/* 
  systick IRQ 
*/
volatile unsigned long SysTickCount = 0;

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  
  
  if ( SysTickCount & 1 )
    GPIOA->BSRR = GPIO_BSRR_BS_0;		/* atomic set PA0 */
  else
    GPIOA->BSRR = GPIO_BSRR_BR_0;		/* atomic clr PA0 */
  
  /*
  if (  COMP2->CSR & COMP_CSR_COMP2VALUE )
    GPIOA->BSRR = GPIO_BSRR_BS_0;	
  else
    GPIOA->BSRR = GPIO_BSRR_BR_0;	
  */
}

/*===============================================*/
/* 
  boost converter
    PA6=Output  to N-MOS Gate
    PA7=Reference Input
*/
void boost_converter(void)
{
  /* assumes, that SYSCFG_CRGR3 is not locked */
  /* assumes, that COMP2 is not locked */
  
  /* setup internal reference (ca. 1.2V) */
  
  SYSCFG->CFGR3 |= SYSCFG_CFGR3_ENBUFLP_VREFINT_COMP;	/* enable VREFINT during low power mode */
  while( (SYSCFG->CFGR3 & SYSCFG_CFGR3_VREFINT_RDYF) == 0 )	/* wait for VREFINT until it becomes ready */
    ;
  
  /* configure PA7 as COMP2 Plus input */
  /* GPIO has to be in input state without any pull up/down resistor: This is default, so nothing needs to be done here */
  
  
  /* configure PA9 as TIM21 output */
  
  GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH1_Msk;
  GPIOA->AFR[1] |= 5<<GPIO_AFRH_AFRH1_Pos;		/* AF5 selects TIM21 */
  
  GPIOA->MODER &= ~GPIO_MODER_MODE9;	/* clear mode */  
  GPIOA->MODER |= GPIO_MODER_MODE9_1;	/* Alternate Function Mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;	/* no Push/Pull  */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED9;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;	/* no pullup/pulldown  */
  GPIOA->BSRR = GPIO_BSRR_BR_9;		/* atomic clr */
  
  /* setup COMP2 */
  /*
    COMP2 has only one register: CSR
    Bit 31:	Lock bit, will lock CSR until next reset if set to 1
    Bit 30: Output value, considering polarity
    Bit 15: Polarity, 0: Not inverted
    Bits 10:8: Plus input
      000: PA3
      001: PB4
      010: PB5
      011: PB6
      100: PB7
      101: PA7 (for category 1 devices only)
    Bits 6:4: Minus input
      000: VREFINT
      001: PA2
      010: PA4
      011: PA5
      100: 1/4 VREFINT
      101: 1/2 VREFINT
      110: 3/4 VREFINT
      111: PB  
    Bit 3: Speed selection, 0: Low speed
    Bit 0: Enable 
  */
  
  
  /* select PA7 as positive input --> 101, only available for the STM32L011 */
  COMP2->CSR &= ~(uint32_t)COMP_CSR_COMP2INPSEL;	
  COMP2->CSR |= COMP_CSR_COMP2INPSEL_0 | COMP_CSR_COMP2INPSEL_2;
  
  /*
      0.306V 
      20mA		15 Ohm	20.4mA
      20mA		18 Ohm	17mA
      350mA	1 Ohm	306mA    --> 300mWatt
  */
  /* select 1/4 internal reference voltage --> 0.306 Volt -->100 */
  COMP2->CSR &= ~(uint32_t)COMP_CSR_COMP2INNSEL;	
  COMP2->CSR |= COMP_CSR_COMP2INNSEL_2;
  
  /* invert polarity */
  //COMP2->CSR |= COMP_CSR_COMP2POLARITY;

  /* comparator enable */
  COMP2->CSR |= COMP_CSR_COMP2EN;
  
  /* setup PWM mode for TIM21 */
 
  /* configure output compare for channel 2 of TIM21 */
  TIM21->CCMR1 &= ~(uint32_t)TIM_CCMR1_CC2S;

  /* configure pwm mode 1 */
  TIM21->CCMR1 &= ~(uint32_t)TIM_CCMR1_OC2M;	/* clear mode to 000 */
  TIM21->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;	/* pwm mode 1 (110) */
  
  TIM21->CCMR1 |= TIM_CCMR1_OC2PE;	/* load modified CCR1 during update event only */

  TIM21->PSC = 0;		/* run with max speed (2 MHz after reset) */
  TIM21->ARR = 20;		/* period of 20 clocks (100KHz if sys clock is not modified) */
  TIM21->CCR2 = 5; 	/* a value between 0 and ARR, which defines the duty cycle */

  /* output the result of channel 2 to PA9 */
  TIM21->CCER |= TIM_CCER_CC2E;
  /* do not invert the output polarity (this is also the default value) */
  TIM21->CCER &= ~(uint32_t)TIM_CCER_CC2P;  

  /* use up counter (this is also the default value) */
  TIM21->CR1 &= ~(uint32_t)TIM_CR1_DIR; 

  /* do not use "one pulse mode" (continues mode, this is also the default value) */
  TIM21->CR1 &= ~(uint32_t)TIM_CR1_OPM; 
  
  /* always generate an update event (this is also default) */
  TIM21->CR1 &= ~(uint32_t)TIM_CR1_UDIS;
  
  /* update event can be caused by UG bit and overflow (this is default) */ 
  TIM21->CR1 &= ~(uint32_t)TIM_CR1_URS;
  
  
  /* connect COMP2 with TIM21 */
  /* the following two bits are not documented in RM0377 */
  /* However, it is mentioned in "A.9.10 ETR configuration to clear OCxREF code example" */
  TIM21->CCMR1 |= TIM_CCMR1_OC2CE;	/* enable clearing on OC1 for ETR clearing */
  TIM21->SMCR |= TIM_SMCR_OCCS; /* Select ETR as OCREF clear source (reserved bit = 1) */
  //TIM21->EGR |= TIM_EGR_UG;
  
  TIM21->OR &= ~TIM21_OR_ETR_RMP_Msk;
  TIM21->OR |= TIM21_OR_ETR_RMP_0;		/* bit pattern 01: connect with COMP2 */
  
  /* enable the counter */
  TIM21->CR1 |= TIM_CR1_CEN;
  
  
}



int main()
{
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN; 
  RCC->APB2ENR |= RCC_APB2ENR_TIM21EN; 
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   
  __NOP();
  __NOP();
  
  GPIOA->MODER &= ~GPIO_MODER_MODE0;	/* clear mode for PA0 */
  GPIOA->MODER |= GPIO_MODER_MODE0_0;	/* Output mode for PA0 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_0;	/* no Push/Pull for PA0 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED0;	/* low speed for PA0 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;	/* no pullup/pulldown for PA0 */
  GPIOA->BSRR = GPIO_BSRR_BR_0;		/* atomic clr PA0 */

  boost_converter();
  
  SysTick->LOAD = 2000*500 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
    
  for(;;)
    ;
}
