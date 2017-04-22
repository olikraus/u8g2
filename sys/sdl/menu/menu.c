

#include "menu.h"

/*================================================*/

#define MENU_NORMAL_FONT u8g2_font_ncenR08_tf
#define MENU_BIG_NUM u8g2_font_ncenR18_tf


/* this function must be the last function in the list. it also marks the end of a list */
int me_cb_null(menu_t *menu, const me_t *me, uint8_t msg)
{
  return 0;
}


/*
  Name: 	me_cb_0_9  
  Val:	uint8_t *
  Arg:	char *
*/
int me_cb_str_toggle(menu_t *menu, const me_t *me, uint8_t msg)
{  
  uint8_t val = *(uint8_t *)(me->val);
  u8g2_uint_t x, y, w, h;
  w = 10;
  h = 10;
  x = me->x;
  y = me->y - u8g2_GetAscent(menu->u8g2)-1;
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_GET_BBX:
      menu->x = x-1;
      menu->y = y-1;
      menu->w = w+2;
      menu->h = h+2;
      return 1;
    case ME_MSG_SELECT:
      {
	val++;
	if ( val > 1 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
      u8g2_DrawUTF8(menu->u8g2, me->x, me->y, (const char *)(me->arg));
      if ( val > 0 )
	u8g2_DrawRFrame(menu->u8g2, x, y, w, h, 1);
      return 1;
  }
  return 0;
}

/*
  Name: 	me_cb_0_9
  Val:	uint8_t *
*/
int me_cb_0_9(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_GET_BBX:
      menu->x = me->x;
      menu->y = me->y - u8g2_GetAscent(menu->u8g2)-1;
      menu->w = u8g2_GetGlyphWidth(menu->u8g2, '0');
      menu->h = u8g2_GetAscent(menu->u8g2) + 2;
      return 1;
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val++;
	if ( val > 9 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      u8g2_DrawGlyph(menu->u8g2, me->x, me->y, *(uint8_t *)(me->val) + '0');
      return 1;
  }
  return 0;
}


/*
  Name: 	me_cb_0_5
  Val:	uint8_t *
*/
int me_cb_0_5(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val++;
	if ( val > 5 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
  }
  return me_cb_0_9(menu, me, msg);
}

/*
  Name: 	me_cb_0_23
  Val:	uint8_t *
*/
int me_cb_0_23(menu_t *menu, const me_t *me, uint8_t msg)
{  
  char s[4];
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_GET_BBX:
      menu->x = me->x;
      menu->y = me->y - u8g2_GetAscent(menu->u8g2)-1;
      menu->w = u8g2_GetGlyphWidth(menu->u8g2, '0')*2;
      menu->h = u8g2_GetAscent(menu->u8g2) + 2;
      return 1;
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val++;
	if ( val > 23 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      s[0] = *(uint8_t *)(me->val);
      s[1] = s[0];
      s[1] %= 10;
      s[1] += '0';
      s[0] /= 10;
      s[0] += '0';
      s[2] = '\0';
	u8g2_DrawUTF8(menu->u8g2, me->x, me->y, s);
      return 1;
  }
  return 0;
}

/*
  Name: 	me_cb_0_55
  Val:	uint8_t *
*/
int me_cb_0_55(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val+=5;
	if ( val > 55 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
  }
  return me_cb_0_23(menu, me, msg);
}

/*
  Name: 	me_cb_label
  can not get focus
  Arg:	char *
*/
int me_cb_num_label(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
    case ME_MSG_GET_BBX:
    case ME_MSG_SELECT:
      break;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      u8g2_DrawUTF8(menu->u8g2, me->x, me->y, (char *)(me->arg) );
      return 1;
  }
  return 0;
}

/*
  Name: 	me_cb_text_line
  Val:	me_t *
  Arg:	char *
*/
int me_cb_text_line(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_GET_BBX:
      menu->x = 0;
      menu->y = me->y - u8g2_GetAscent(menu->u8g2) -1;
      menu->w = u8g2_GetDisplayWidth(menu->u8g2) ;
      menu->h = u8g2_GetAscent(menu->u8g2) - u8g2_GetDescent(menu->u8g2) +1;
      return 1;
    case ME_MSG_SELECT:
      if ( me->val != NULL )
	menu_SetMEList(menu, (const me_t *)(me->val), 0);
      return 1;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
      u8g2_DrawUTF8(menu->u8g2, me->x, me->y, (char *)(me->arg) );
      return 1;
  }
  return 0;
}

/*
  Name: 	me_cb_label
  can not get focus
  Arg:	char *
*/
int me_cb_label(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
    case ME_MSG_GET_BBX:
    case ME_MSG_SELECT:
      break;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
      u8g2_DrawUTF8(menu->u8g2, me->x, me->y, (char *)(me->arg) );
      return 1;
  }
  return 0;
}

/*================================================*/

/* call menu element from menu->current_index */
int menu_CallME(menu_t *menu, uint8_t msg)
{
  const me_t *me;
  me = menu->me_list+menu->current_index;
  return me->cb(menu, me, msg);
}

/* stay on current focus if valid, move to next valid focus */
static void menu_CalcNextValidFocus(menu_t *menu) U8G2_NOINLINE;
static void menu_CalcNextValidFocus(menu_t *menu)
{
  for(;;)
  {
    menu->current_index = menu->focus_index;
    if ( menu->current_index >= menu->me_count )
      break;    
    if ( menu_CallME(menu, ME_MSG_IS_FOCUS) != 0 )
      break;
    menu->focus_index++;
  }
}

/* advance current focus to the next element */
void menu_NextFocus(menu_t *menu)
{
  menu->focus_index++;
  if ( menu->focus_index >= menu->me_count )
    menu->focus_index = 0;
  menu_CalcNextValidFocus(menu);
}

/* send select message to the element which has the current focus */
void menu_Select(menu_t *menu)
{
  menu->current_index = menu->focus_index;
  menu_CallME(menu, ME_MSG_SELECT);
}



void menu_SetMEList(menu_t *menu, const me_t *me_list, uint16_t initial_focus)
{
  menu->me_list = me_list;
  
  menu->me_count = 0;
  while( me_list[menu->me_count].cb !=  me_cb_null )
    menu->me_count++;
  
  menu->focus_index = 0;
  menu_CalcNextValidFocus(menu);
  
  menu->radio_index = menu->me_count;
}

me_t melist_emty[] = 
{
  { me_cb_null, NULL, 0, 0 }
};


void menu_Init(menu_t *menu, u8g2_t *u8g2)
{
  menu->u8g2 = u8g2;
  menu_SetMEList(menu, melist_emty, 0);
}

/* draw focus for menu->current_index */
void menu_DrawFocus(menu_t *menu)
{
  uint8_t color;
  menu_CallME(menu, ME_MSG_GET_BBX);
  /*
  menu->x--;
  menu->y--;
  menu->h+=2;
  menu->w+=2;
  */
  color = u8g2_GetDrawColor(menu->u8g2);
  u8g2_SetDrawColor(menu->u8g2, 2);
  u8g2_DrawBox(menu->u8g2, menu->x, menu->y, menu->w, menu->h);
  u8g2_SetDrawColor(menu->u8g2, 0);
  menu->w--;
  menu->h--;
  u8g2_DrawPixel(menu->u8g2, menu->x,menu->y);
  u8g2_DrawPixel(menu->u8g2, menu->x+menu->w,menu->y);
  u8g2_DrawPixel(menu->u8g2, menu->x,menu->y+menu->h);
  u8g2_DrawPixel(menu->u8g2, menu->x+menu->w,menu->y+menu->h);
  u8g2_SetDrawColor(menu->u8g2, color);
}


void menu_Draw(menu_t *menu)
{
  for( menu->current_index = 0; menu->current_index < menu->me_count; menu->current_index++ )
  {
    menu_CallME(menu, ME_MSG_DRAW);
    if ( menu->current_index == menu->focus_index )
    {
      menu_DrawFocus(menu);
    }
  }
}

