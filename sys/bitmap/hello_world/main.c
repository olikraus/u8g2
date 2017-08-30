#include "u8g2.h"
#include <stdio.h>

/*
 * This example uses the Bitmap device to draw to an in-memory buffer
 * and writes the result to a TGA file. This can for example be used to
 * make screenshots of an application normally running on an Arduino.
 */

u8g2_t u8g2;

int main(void)
{
  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, 128, 64);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);
  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

  u8g2_ClearBuffer(&u8g2);
  // Here, you can call other code to actually draw things, such as
  // existing application code to generate a screenshot
  u8g2_DrawStr(&u8g2, 10, 30, "Hello, world!");
  u8g2_SendBuffer(&u8g2);
  u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2), "small.tga");

  // This changes size, but also resets other stuff like font
  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, 128, 128);
  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

  u8g2_ClearBuffer(&u8g2);
  // Here, you can call other code to actually draw things, such as
  // existing application code to generate a screenshot
  u8g2_DrawStr(&u8g2, 10, 30, "Hello, world!");
  u8g2_DrawStr(&u8g2, 10, 100, "Hello, larger world!");
  u8g2_SendBuffer(&u8g2);
  u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2), "large.tga");

  return 0;
}

