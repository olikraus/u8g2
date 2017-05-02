/*

  guihal.c
  
*/


#include "gui.h"
#include <stdio.h>

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

/*============================================*/
/* output */

void enable_alarm(void)
{
}

void disable_alarm(void)
{
}
