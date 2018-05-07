
#include <stdio.h>
#include <stdlib.h>
#include "u8g2.h"

/* both arrays are NULL terminated */
extern const uint8_t *u8g2_font_list[];
extern char *u8g2_font_names[];

u8g2_t u8g2;

int main(int argc, char **argv)
{
  int i, is_found;
  uint16_t encoding;
  
  if ( argc <= 1 )
  {
    printf("codesearch encoding\n");
  }
  else
  {
    encoding = atoi(argv[1]);
    u8g2_Setup_null(&u8g2, U8G2_R0, u8x8_byte_empty, u8x8_dummy_cb);
    i = 0;
    is_found = 0;
    while( u8g2_font_list[i] != NULL )
    {
      u8g2_SetFont(&u8g2, u8g2_font_list[i]);
      if ( u8g2_IsGlyph(&u8g2, encoding) )
      {
	if ( is_found == 0 )
	{
	  printf("encoding '%d' available in the following fonts:\n", encoding);
	  is_found = 1;
	}
	printf("%s\n", u8g2_font_names[i]);
      }
      i++;
    }
    if ( is_found == 0 )
    {
	  printf("encoding '%d' is not available in any font.\n", encoding);
    }
  }
  return 0;
}
