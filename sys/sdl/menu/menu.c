

#include "menu.h"


/* this function must be the last function in the list. it also marks the end of a list */
int me_cb_null(menu_t *menu, const me_t *me, uint8_t msg)
{
  return 0;
}



int me_cb_0_9(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_GET_BBX:
      menu->x = me->x;
      menu->y = me->y - u8g2_GetAscent(menu->u8g2);
      menu->w = u8g2_GetGlyphWidth(menu->u8g2, '0');
      menu->h = u8g2_GetAscent(menu->u8g2) - u8g2_GetDescent(menu->u8g2);
      return 1;
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->arg);
	val++;
	if ( val > 9 )
	  val = 0;
	*(uint8_t *)(me->arg) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      u8g2_DrawGlyph(menu->u8g2, me->x, me->y, *(uint8_t *)(me->arg) + '0');
      return 1;
  }
  return 0;
}

/* call menu element from menu->current_index */
int menu_CallME(menu_t *menu, uint8_t msg)
{
  const me_t *me;
  me = menu->me_list+menu->current_index;
  return me->cb(menu, me, msg);
}

/* stay on current focus if valid, move to next valid focus */
void menu_CalcNextValidFocus(menu_t *menu)
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


void menu_Init(menu_t *menu, const me_t *me_list, uint16_t initial_focus)
{
  menu->me_list = me_list;
  
  menu->me_count = 0;
  while( me_list[menu->me_count].cb !=  me_cb_null )
    menu->me_count++;
  
  menu->focus_index = 0;
  menu_CalcNextValidFocus(menu);
  
  menu->radio_index = menu->me_count;
  
  
  
}