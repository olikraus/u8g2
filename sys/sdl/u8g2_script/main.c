
#include "u8g2.h"
#include <stdio.h>
#include <string.h>


void ep_skip_space(const char **s)
{
  for(;;)
  {
    if ( **s == '\0' )
      break;
    if ( **s == '\n' )
      break;
    if ( **s == '\r' )
      break;
    if ( **s > ' ' )
      return;
    (*s)++;
  }
}

void ep_skip_new_line(const char **s)
{
  for(;;)
  {
    if ( **s == '\0' )
      return;
    if ( **s == '\n' )
      break;
    if ( **s == '\r' )
      break;
    (*s)++;    
  }

  for(;;)
  {
    if ( **s == '\0' )
      return;
    if ( **s > ' ' )
      return;
    (*s)++;
  }
  
}

unsigned ep_get_integer(const char **s)
{
  unsigned v = 0;
  for(;;)
  {
    if ( **s >= '0' && **s <= '9' )
    {
      v *= 10;
      v+= (u8g2_uint_t)(**s-'0');
    }
    else
    {
      break;
    }
    (*s)++;
  }
  ep_skip_space(s);
  return v;
}

const char *ep_get_identifier(const char **s)
{
  static char b[34] = "";
  uint8_t i = 0;
  b[0] = '\0';
  for(;;)
  {
    if ( **s == '\0' )
      break;
    if ( (**s >= '0' && **s <= '9') 
      || (**s >= 'a' && **s <= 'z') 
      || (**s >= 'A' && **s <= 'Z') 
      || ( **s == '_' ) )
    {
      if ( i < 32 )
      {
        b[i] = **s;
        i++;
        b[i] = '\0';
      }
    }
    else
    {
      break;
    }
    
    (*s)++;
  }
  ep_skip_space(s);
  return b;
}

const char *ep_get_string(const char **s)
{
  static char b[66] = "";
  uint8_t i = 0;
  b[0] = '\0';
  if ( **s != '\"' )
    return b;
  (*s)++;
  for(;;)
  {
    if ( **s == '\"' )
    {
      (*s)++;
      break;
    }
    if ( **s == '\0' )
      break;
    if ( i < 64 )
    {
      b[i] = **s;
      i++;
      b[i] = '\0';
    }
    (*s)++;
  }
  ep_skip_space(s);
  return b;
}

void ep_cmd_text(u8g2_t *u8g2, const char **s)
{
  u8g2_uint_t x;
  u8g2_uint_t y;
  const char *text;
  x = ep_get_integer(s);
  y = ep_get_integer(s);
  if ( **s == '\"' )
    text = ep_get_string(s);
  else
    text = ep_get_identifier(s);
  u8g2_DrawStr(u8g2, x, y, text);  
}

void ep_cmd_char(u8g2_t *u8g2, const char **s)
{
  u8g2_uint_t x;
  u8g2_uint_t y;
  uint16_t g;
  
  x = ep_get_integer(s);
  y = ep_get_integer(s);
  g = ep_get_integer(s);
  u8g2_DrawGlyph(u8g2, x, y, g);  
}

void ep_cmd_font(u8g2_t *u8g2, const char **s)
{
    const char *font = ep_get_identifier(s);
    if ( strcmp(font, "cu12") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_cu12_tf);
    else if ( strcmp(font, "helvB08") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvB08_tf);
    else if ( strcmp(font, "helvR08") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvR08_tf);
    else if ( strcmp(font, "helvB10") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvB10_tf);
    else if ( strcmp(font, "helvR10") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvR10_tf);
    else if ( strcmp(font, "helvB12") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvB12_tf);
    else if ( strcmp(font, "helvR12") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvR12_tf);
    else if ( strcmp(font, "helvB14") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvB14_tf);
    else if ( strcmp(font, "helvR14") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvR14_tf);
    else if ( strcmp(font, "helvB18") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvB18_tf);
    else if ( strcmp(font, "helvR18") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvR18_tf);
    else if ( strcmp(font, "helvB24") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvB24_tf);
    else if ( strcmp(font, "helvR24") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_helvR24_tf);
    else if ( strcmp(font, "4x6") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_4x6_tf);
    else if ( strcmp(font, "5x7") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_5x7_tf);
    else if ( strcmp(font, "5x8") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_5x8_tf);
    else if ( strcmp(font, "6x10") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_6x10_tf);
    else if ( strcmp(font, "6x13") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_6x13_tf);
    else if ( strcmp(font, "6x13B") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_6x13B_tf);
    else if ( strcmp(font, "7x13") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_7x13_tf);
    else if ( strcmp(font, "7x13B") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_7x13B_tf);
    else if ( strcmp(font, "7x14") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_7x14_tf);
    else if ( strcmp(font, "7x14B") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_7x14B_tf);
    else if ( strcmp(font, "8x13") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_7x13_tf);
    else if ( strcmp(font, "8x13B") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_8x13B_tf);
    
    else if ( strcmp(font, "open_inconic_1x") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_1x_t);
    else if ( strcmp(font, "open_inconic_2x") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_2x_t);
    else if ( strcmp(font, "open_inconic_4x") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_4x_t);
    else if ( strcmp(font, "open_inconic_6x") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_6x_t);
    else if ( strcmp(font, "open_inconic_8x") == 0 )
      u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_8x_t);
    
    else
      u8g2_SetFont(u8g2, u8g2_font_6x10_tf);      
}

void ep_cmd_color(u8g2_t *u8g2, const char **s)
{  
  uint8_t c = ep_get_integer(s);
  u8g2_SetDrawColor(u8g2, c);
}

void ep_cmd_transparent(u8g2_t *u8g2, const char **s)
{  
  uint8_t v = ep_get_integer(s);
  u8g2_SetFontMode(u8g2, v);
}


void ep_cmd_box(u8g2_t *u8g2, const char **s)
{
  u8g2_uint_t x;
  u8g2_uint_t y;
  u8g2_uint_t w;
  u8g2_uint_t h;
  x = ep_get_integer(s);
  y = ep_get_integer(s);
  w = ep_get_integer(s);
  h = ep_get_integer(s);
  u8g2_DrawBox(u8g2, x, y, w, h);  
}

void ep_cmd_rbox(u8g2_t *u8g2, const char **s)
{
  u8g2_uint_t x;
  u8g2_uint_t y;
  u8g2_uint_t w;
  u8g2_uint_t h;
  u8g2_uint_t r;
  x = ep_get_integer(s);
  y = ep_get_integer(s);
  w = ep_get_integer(s);
  h = ep_get_integer(s);
  r = ep_get_integer(s);
  u8g2_DrawRBox(u8g2, x, y, w, h, r);  
}

void ep_cmd_frame(u8g2_t *u8g2, const char **s)
{
  u8g2_uint_t x;
  u8g2_uint_t y;
  u8g2_uint_t w;
  u8g2_uint_t h;
  x = ep_get_integer(s);
  y = ep_get_integer(s);
  w = ep_get_integer(s);
  h = ep_get_integer(s);
  u8g2_DrawFrame(u8g2, x, y, w, h);  
}

void ep_cmd_rframe(u8g2_t *u8g2, const char **s)
{
  u8g2_uint_t x;
  u8g2_uint_t y;
  u8g2_uint_t w;
  u8g2_uint_t h;
  u8g2_uint_t r;
  x = ep_get_integer(s);
  y = ep_get_integer(s);
  w = ep_get_integer(s);
  h = ep_get_integer(s);
  r = ep_get_integer(s);
  u8g2_DrawRFrame(u8g2, x, y, w, h, r);
}

void ep_cmd_clear(u8g2_t *u8g2, const char **s)
{
    u8g2_ClearBuffer(u8g2);
}



void ep_cmd(u8g2_t *u8g2, const char **s)
{
  const char *cmd;
  ep_skip_space(s);
  for(;;)
  {
    if ( **s == '\0' )
      break;
    cmd = ep_get_identifier(s);
    //printf("cmd: %s %s\n", cmd, *s);
    if ( strcmp(cmd, "text") == 0 ) 
      ep_cmd_text(u8g2, s);
    else if ( strcmp(cmd, "char") == 0 ) 
      ep_cmd_char(u8g2, s);    
    else if ( strcmp(cmd, "font") == 0 ) 
      ep_cmd_font(u8g2, s);    
    else if ( strcmp(cmd, "color") == 0 ) 
      ep_cmd_color(u8g2, s);    
    else if ( strcmp(cmd, "transparent") == 0 ) 
      ep_cmd_transparent(u8g2, s);    
    else if ( strcmp(cmd, "box") == 0 ) 
      ep_cmd_box(u8g2, s);    
    else if ( strcmp(cmd, "rbox") == 0 ) 
      ep_cmd_rbox(u8g2, s);    
    else if ( strcmp(cmd, "frame") == 0 ) 
      ep_cmd_frame(u8g2, s);    
    else if ( strcmp(cmd, "rframe") == 0 ) 
      ep_cmd_rframe(u8g2, s);    
    else if ( strcmp(cmd, "clear") == 0 ) 
      ep_cmd_clear(u8g2, s);    
    ep_skip_new_line(s);
  }
}


u8g2_t u8g2;

int main(void)
{
  int x, y;
  int k;
  int i;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  //u8g2_SetFont(&u8g2, u8g2_font_cu12_t_arabic);
  //u8g2_SetFont(&u8g2, u8g2_font_10x20_t_arabic);
  u8g2_SetFont(&u8g2, u8g2_font_cu12_tf);
  
  x = 50;
  y = 30;

  
  for(;;)
  {
#ifdef U8G2_WITH_HVLINE_COUNT
    u8g2.hv_cnt = 0UL;
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    
    u8g2_FirstPage(&u8g2);
    i = 0;
    do
    {
      const char *s1 = 
      "transparent 1\n"
      "font 4x5\n"
      "text 0 14 Hello\n"
      "font helvB12\n"
      "text 10 28 World\n"
      "box 0 0 10 2\n"
      "rbox 40 0 10 6 2\n"
      "frame 20 0 10 4\n"
      "rframe 60 0 10 8 3\n"
      "font open_inconic_8x\n"
      "char 64 64 282\n"
      ;
      ep_cmd(&u8g2, &s1);

      
      if ( i == 1 )
      {
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y0, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y0, 1, 0);

      }
      
      i++;
      

    } while( u8g2_NextPage(&u8g2) );
#ifdef U8G2_WITH_HVLINE_COUNT
    printf("hv cnt: %ld\n", u8g2.hv_cnt);
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 273 ) y -= 7;
    if ( k == 274 ) y += 7;
    if ( k == 276 ) x -= 7;
    if ( k == 275 ) x += 7;
    
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    if ( k == 'q' ) break;
    
  }
  
  //u8x8_Set8x8Font(u8g2_GetU8x8(&u8g2), bdf_font);
  //u8x8_Draw8x8String(u8g2_GetU8x8(&u8g2), 0, 0, "Hello World!");
  
  
  
  
  
  
  return 0;
}

