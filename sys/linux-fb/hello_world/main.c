#include "u8g2.h"
#include <stdio.h>

u8g2_t u8g2;

const char *fb_dev = "/dev/fb0";

int main(int argc, char **argv)
{
	if(argc > 1){
		fb_dev = argv[1];
	}

	u8g2_SetupLinuxFb(&u8g2,U8G2_R0, fb_dev);
	u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
	u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);
	u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

	u8g2_ClearBuffer(&u8g2);
	u8g2_DrawUTF8(&u8g2, 2, 10, "U8G2 on");
	u8g2_DrawUTF8(&u8g2, 2, 20, "Linux");
	u8g2_DrawUTF8(&u8g2, 2, 31, "Framebuffer");
	u8g2_SetFont(&u8g2, u8g2_font_open_iconic_all_4x_t);
	u8g2_DrawGlyph(&u8g2,90, 32, 120);
	//u8g2_DrawFrame(&u8g2,0,0,128,32);
	u8g2_SendBuffer(&u8g2);
	return 0;
}

