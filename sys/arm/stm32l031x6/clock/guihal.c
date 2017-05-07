/*

  guihal.c
  
*/

#include "stm32l031xx.h"
#include "gui.h"

/*============================================*/
/* reset */
void do_reset(void)
{
  /* clear the display first, to provide some visual feedback to the user */
  u8g2_ClearDisplay(gui_menu.u8g2);
  
  /* Deactivate tamper so that the uC can be programmed via UART */
  /* This is required, because RX pin is shared with Tamp2 */
  
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;
  
  EXTI->IMR &= ~EXTI_IMR_IM19;			/* disable tamper irq */

  RTC->TAMPCR = 0;						/* clear tamper detection */

  RTC->WPR = 0;						/* disable RTC write protection */
  RTC->WPR = 0;

  /* execute reset */
  
  NVIC_SystemReset();
}

/*============================================*/
/* load & store from/to permanent memory */


/* 5x 32bit */

void store_gui_data(uint32_t *data)
{
}

void load_gui_data(uint32_t *data)
{

  int i;
  
  for( i = 0; i < GUI_ALARM_CNT; i++ )
    data[i] = 0;
  
}


/*============================================*/
/* input */

int is_por_reset(void)
{
  return 1;
}

int is_button_reset(void)
{
  return 0;
}

/*============================================*/
/* output */

void enable_alarm(void)
{
}

void disable_alarm(void)
{
}

void set_time(uint8_t ht, uint8_t ho, uint8_t mt, uint8_t mo, uint8_t st, uint8_t so)
{
  uint32_t v;
  
  v = ht;
  v <<= 4,
  v |= ho;
  v <<= 4;
  v |= mt;
  v <<= 4;
  v |= mo;
  v <<= 4;
  v |= st;
  v <<= 4;
  v |= so;
  
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;

  RTC->ISR = RTC_ISR_INIT;				/* request RTC stop */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* wait for stop */
      ;
  RTC->PRER = 0x07f00ff;					/* 1 Hz clock */
  RTC->TR = v; 
  RTC->ISR =~ RTC_ISR_INIT; 				/* start RTC */
  
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
}


/*
  weekday: 0 = monday
*/
void set_date(uint8_t yt, uint8_t yo, uint8_t mt, uint8_t mo, uint8_t dayt, uint8_t dayo, uint8_t weekday)
{
  uint32_t v;
  
  v = yt;
  
  v <<= 4,
  v |= yo;
  
  v <<= 3;
  v |= weekday+1;	/* monday starts with 1 on the STM32L0 */
  v <<= 1;  
  v |= mt;
  
  v <<= 4;
  v |= mo;
  
  v <<= 4;
  v |= dayt;
  
  v <<= 4;
  v |= dayo;
  
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;

  RTC->ISR = RTC_ISR_INIT;				/* request RTC stop */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* wait for stop */
      ;
  RTC->PRER = 0x07f00ff;					/* 1 Hz clock */
  RTC->DR = v; 
  RTC->ISR =~ RTC_ISR_INIT; 				/* start RTC */
  
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
}

