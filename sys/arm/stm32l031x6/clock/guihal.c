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
