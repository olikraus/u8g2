/*

  guihal.c
  
*/


#include "gui.h"
#include <stdio.h>

/*============================================*/
/* reset */
void do_reset(void)
{
}

/*============================================*/
/* load & store from/to permanent memory */


/* 5x 32bit */

void store_gui_data(uint32_t *data)
{
  FILE *fp;
  fp = fopen("clock.dat", "w");
  if ( fp != NULL )
  {
    fwrite(data, sizeof(uint32_t), 5, fp);
    fclose(fp);
  }
}

void load_gui_data(uint32_t *data)
{
  FILE *fp;

  int i;
  
  for( i = 0; i < GUI_ALARM_CNT; i++ )
    data[i] = 0;
  
  fp = fopen("clock.dat", "r");
  if ( fp != NULL )
  {
    fread(data, sizeof(uint32_t), 5, fp);
    fclose(fp);
  }
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

uint32_t get_boot_status_register(void)
{
  return 0;
}

uint32_t get_pwr_status_register(void)
{
  return 0;
}

uint32_t get_reset_reason(void)
{
  return 0;
}

uint32_t get_wakeup_count(void)
{
  return 0;
}

uint32_t get_dst_by_date(void)
{
  return -1;
}

uint32_t get_dst_by_RTC(void)
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
}

void set_date(uint8_t yt, uint8_t yo, uint8_t mt, uint8_t mo, uint8_t dayt, uint8_t dayo, uint8_t weekday)
{
}

/* set contrast to gui_data.contrast, value 1..7, 0 is default (do not set) */
void set_contrast(void)
{
}



