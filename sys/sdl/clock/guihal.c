/*

  guihal.c
  
*/


#include "gui.h"
#include <stdio.h>

/* 5x 32bit */

void store_gui_data(uint32_t *data)
{
  FILE *fp;
  fp = fopen("clock.dat", "w");
  fwrite(data, sizeof(uint32_t), 5, fp);
  fclose(fp);
}

void load_gui_data(uint32_t *data)
{
  FILE *fp;
  fp = fopen("clock.dat", "r");
  fread(data, sizeof(uint32_t), 5, fp);
  fclose(fp);
}


