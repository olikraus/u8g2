

#ifndef _MENU_H
#define _MENU_H

#include "u8g2.h"

#define MENU_SMALL_FONT u8g2_font_baby_tr
#define MENU_NORMAL_FONT u8g2_font_ncenR08_tf
#define MENU_BIG_NUM u8g2_font_ncenR18_tf


typedef struct _menu_struct menu_t;
typedef struct _me_struct me_t;

typedef int (*me_cb)(menu_t *menu, const me_t *me, uint8_t msg);



struct _me_struct
{
  me_cb cb;
  void *val;
  void *arg;
  u8g2_uint_t x;
  u8g2_uint_t y;  
};


/* return 1, if this element can have focus */
#define ME_MSG_IS_FOCUS 1

/* draw focus graphics for the element */
#define ME_MSG_DRAW_FOCUS 2

/* user has pressed the select key */
#define ME_MSG_SELECT 3

/* advice for drawing */
#define ME_MSG_DRAW 4


struct _menu_struct
{
  u8g2_t *u8g2;
  
  volatile uint16_t current_index;	/* element which is processed right now */
  uint16_t focus_index;	/* element which has the focus at the moment */
  uint16_t radio_index;	/* if elements for a radio selection, then this is set by the cb */
  uint16_t me_count;	/* total number of elements in the list */
    
  /* pointer to the list of menu elements */
  const me_t *me_list;
  
};

void menu_SetEdgePixel(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) U8G2_NOINLINE;
void menu_ClearEdgePixel(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) U8G2_NOINLINE;
void menu_DrawBoxFocus(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) U8G2_NOINLINE;
void menu_DrawFrameFocus(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) U8G2_NOINLINE;

void menu_Init(menu_t *menu, u8g2_t *u8g2);
void menu_SetMEList(menu_t *menu, const me_t *me_list, uint16_t initial_focus);
void menu_Draw(menu_t *menu);
void menu_NextFocus(menu_t *menu);
void menu_Select(menu_t *menu);


int me_cb_null(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_big_toggle(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_wd_toggle(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_5(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_9(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_9_ro(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_23(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_23_ro(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_55(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_1_12(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_1_31(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_num_label(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_button_full_line(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_button_half_line(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_button_empty(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_label(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_16x16_bitmap_button(menu_t *menu, const me_t *me, uint8_t msg);

#endif
