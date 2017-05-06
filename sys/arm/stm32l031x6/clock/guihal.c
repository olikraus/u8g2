/*

  guihal.c
  
*/

#include "stm32l031xx.h"
#include "gui.h"

/*============================================*/
/* reset */
void do_reset(void)
{
  // deactivate temper and reactivate button pullups
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
