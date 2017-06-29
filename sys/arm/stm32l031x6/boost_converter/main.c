/* LED blink project for the STM32L031 */

#include "stm32l031xx.h"

/*===============================================*/
/* 
  systick IRQ 
*/
volatile unsigned long SysTickCount = 0;

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  if ( SysTickCount & 1 )
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */
  else
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
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
  
  /* setup internal reference */
  
  SYSCFG->CFGR3 |= SYSCFG_CFGR3_ENBUFLP_VREFINT_COMP;	/* enable VREFINT during low power mode */
  while( (SYSCFG->CFGR3 & SYSCFG_CFGR3_VREFINT_RDYF) == 0 )	/* wait for VREFINT until it becomes ready */
    ;
  
  /* setup COMP2 */
  
  /* select PA7 as positive input --> 101 */
  COMP2->CSR &= ~(uint32_t)COMP_CSR_COMP2INPSEL;	
  COMP2->CSR |= COMP_CSR_COMP2INPSEL_0 | COMP_CSR_COMP2INPSEL_2;
  
  /* select 1/4 internal reference voltage --> 0.306 Volt -->100 */
  COMP2->CSR &= ~(uint32_t)COMP_CSR_COMP2INNSEL;	
  COMP2->CSR |= COMP_CSR_COMP2INNSEL_2;
  
  /* invert polarity */
  COMP2->CSR |= COMP_CSR_COMP2POLARITY;

  /* comparator enable */
  COMP2->CSR |= COMP_CSR_COMP2EN;
  
  /* setup PWM mode for TIM22 */
 
  /* configure output compare for channel 1 of TIM22 */
  TIM22->CCMR1 &= ~(uint32_t)TIM_CCMR1_CC1S;

  /* configure pwm mode 1 */
  TIM22->CCMR1 &= ~(uint32_t)TIM_CCMR1_OC1M;	/* clear mode to 000 */
  TIM22->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;	/* pwm mode 1 (110) */
  
  TIM22->CCMR1 |= TIM_CCMR1_OC1PE;	/* load modified CCR1 during update event only */

  TIM22->PSC = 0;		/* run with max speed (2 MHz after reset) */
  TIM22->ARR = 20;		/* period of 20 clocks (100KHz if sys clock is not modified */
  TIM22->CCR1 = 10; 	/* a value between 0 and ARR, which defines the duty cycle */

  /* output the result of channel 1 to PA6 */
  TIM22->CCER |= TIM_CCER_CC1E;
  /* do not invert the output polarity (this is also the default value) */
  TIM22->CCER &= ~(uint32_t)TIM_CCER_CC1P;  

  /* use up counter (this is also the default value) */
  TIM22->CR1 &= ~(uint32_t)TIM_CR1_DIR; 

  /* do not use "one pulse mode" (continues mode, this is also the default value) */
  TIM22->CR1 &= ~(uint32_t)TIM_CR1_OPM; 
  
  /* always generate an update event (this is also default) */
  TIM22->CR1 &= ~(uint32_t)TIM_CR1_UDIS;
  
  /* update event can be caused by UG bit and overflow (this is default) */ 
  TIM22->CR1 &= ~(uint32_t)TIM_CR1_URS;
  
  /* enable the counter */
  TIM22->CR1 |= TIM_CR1_CEN;
  
  
  

TIMx->CR1 |= TIM_CR1_CEN; /* (6) */
TIMx->EGR |= TIM_EGR_UG; /* (7) */

  /* setup gated mode for TIM22  */

  
/* (1) Configure channel 1 to detect low level on the TI1 input
by writing CC1S = ‘01’,
and configure the input filter duration by writing the IC1F[3:0]
bits
in the TIMx_CCMR1 register (if no filter is needed, keep
IC1F=0000).*/
/* (2) Select polarity by writing CC1P=1 in the TIMx_CCER register */
/* (3) Configure the timer in gated mode by writing SMS=101
Select TI1 as the trigger input source by writing TS=101
in the TIMx_SMCR register.*/
/* (4) Set prescaler to 4000-1 in order to get an increment each 250us */
/* (5) Enable the counter by writing CEN=1 in the TIMx_CR1 register. */
TIMx->CCMR1 |= TIM_CCMR1_CC1S_0; /* (1 */
TIMx->CCER |= TIM_CCER_CC1P; /* (2) */
TIMx->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0 \
| TIM_SMCR_TS_2 | TIM_SMCR_TS_0; /* (3) */
TIMx->PSC = 3999; /* (4) */
TIMx->CR1 |= TIM_CR1_CEN; /* (5) */  
  
}



int main()
{
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA13 */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA13 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* no Push/Pull for PA13 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed for PA13 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown for PA13 */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */

  SysTick->LOAD = 2000*500 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
    
  for(;;)
    ;
}
