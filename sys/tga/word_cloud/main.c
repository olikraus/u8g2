
#include "u8g2.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


struct _box_t 
{
  uint32_t w, h;
};
typedef struct _box_t box_t;

struct _pos_t 
{
  uint32_t x, y;
};
typedef struct _pos_t pos_t;

struct _pbox_t		// placed box
{
  pos_t pos;
  int box_idx;
};
typedef struct _pbox_t pbox_t;

#define BOX_LIST_MAX 50
box_t box_list[BOX_LIST_MAX];
char box_word[BOX_LIST_MAX][64];
const uint8_t *box_font[BOX_LIST_MAX];
int box_cnt = 0;

#define PLACE_OPTION_MAX 100
int place_option_cnt = 0;
pos_t place_option_list[PLACE_OPTION_MAX];

#define PLACED_BOX_MAX 100
int placed_box_cnt = 0;
pbox_t placed_box_list[PLACED_BOX_MAX];
uint32_t placed_box_area_max_x;
uint32_t placed_box_area_max_y;
uint32_t placed_box_area_value;

/*============================================*/

int place_option_find(pos_t *p)
{
  int i;
  for( i = 0; i < place_option_cnt; i++ )
  {
    if ( place_option_list[i].x == p->x && place_option_list[i].y == p->y)
      return i;		// place option already exists
  }
  return -1;
}

void place_option_add(pos_t *p)
{
  int i;
  assert(place_option_cnt < PLACE_OPTION_MAX);
  
  if ( place_option_find(p) >= 0 )
      return;		// place option already exists
  
  // the placement option also must not be part of any existing box
  for( i = 2; i < placed_box_cnt; i++ )
  {
    if ( p->x >= placed_box_list[i].pos.x && p->x < placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w )
      if ( p->y >= placed_box_list[i].pos.y && p->y < placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h )
	return;		// place option would be inside an existing box
  }
  
  // place option does not exist, add new place option
  
  place_option_list[place_option_cnt] = *p;
  place_option_cnt++;
}

void place_option_by_xy(uint32_t x, uint32_t y)
{
  pos_t p;
  p.x = x;
  p.y = y;
  place_option_add(&p);
}

void place_option_delete(int idx)
{
  if ( idx < 0 || idx >= place_option_cnt )
    return;
  for( ; idx+1 < place_option_cnt; idx++ )
  {
    place_option_list[idx] = place_option_list[idx+1];
  }
  place_option_cnt--;
}

/*============================================*/

// p is copyied, b only the address is stored
pbox_t *placed_box_push(pos_t *p, int box_idx)
{
  assert(placed_box_cnt < PLACED_BOX_MAX);

  
  // place the new box
  placed_box_list[placed_box_cnt].pos = *p;
  placed_box_list[placed_box_cnt].box_idx = box_idx;
  placed_box_cnt++;
  return placed_box_list+placed_box_cnt-1;
}

void placed_box_pop(void)
{
  assert(placed_box_cnt > 0);
  placed_box_cnt--;
}

uint32_t placed_box_calculate_max_area(void)
{
  int i;
  placed_box_area_max_y = 0;
  placed_box_area_max_x = 0;
  for( i = 2; i < placed_box_cnt; i++ )
  {
    
    if ( placed_box_area_max_y < placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h )
      placed_box_area_max_y = placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h;
    if ( placed_box_area_max_x < placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w )
      placed_box_area_max_x = placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w;
  }
  
  // weight of y is higher, so this should give a more wider picture
  placed_box_area_value = placed_box_area_max_x + placed_box_area_max_y * 3;
  return placed_box_area_value;
}

/*============================================*/

int is_intersection(pos_t *p1, box_t *b1, pos_t *p2, box_t *b2) 
{ 
    pos_t q1 = *p1;
    pos_t q2 = *p2;
    q1.x += b1->w;
    q1.y += b1->h;
    q2.x += b2->w;
    q2.y += b2->h;
  
    if ( p1->x >= q2.x || q1.x <= p2->x )
      return 0;
    if ( p1->y >= q2.y || q1.y <= p2->y )
      return 0;
  
    return 1; 
} 

int is_intersection_with_placed_box(pos_t *p1, box_t *b1, int idx) 
{
  return is_intersection(p1, b1, &(placed_box_list[idx].pos),  box_list+placed_box_list[idx].box_idx );
}

int is_any_intersection(pos_t *p1, box_t *b1)
{
  int i;
  // start with the third box, because the first two are boundary boxes
  for( i = 2; i < placed_box_cnt; i++ )
  {
    if ( is_intersection_with_placed_box(p1, b1, i) != 0 )
      return 1;
  }
  return 0;
}


// add new place options by a give pbox
void place_option_by_pbox(pbox_t *pbox)
{
  int i, found_i;
  uint32_t y, x;
  uint32_t max_y, max_x;
  
  // from the lower left corner, search towards the left until another box is hit.
  y = pbox->pos.y + box_list[pbox->box_idx].h;
  max_x = 0;
  found_i = -1;
  for( i = 0; i < placed_box_cnt; i++ )
  {
    if ( placed_box_list+i != pbox ) // do not check ourself
    {
      if ( placed_box_list[i].pos.x <= pbox->pos.x )
      {
	if ( y >=  placed_box_list[i].pos.y && y < placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h )
	{
	  // left box found, but is it max?
	  // must use >= here, because the initial boxes have zero area
	  if ( max_x <= placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w )
	  {
	    max_x = placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w;
	    found_i = i;
	  }
	}
      }
    }
  }
  if (found_i >= 0)
    place_option_by_xy(max_x, y);
  

  // from the upper right corner, search towards the top until another box is hit.
  x = pbox->pos.x + box_list[pbox->box_idx].w;
  max_y = 0;
  found_i = -1;
  for( i = 0; i < placed_box_cnt; i++ )
  {
    if ( placed_box_list+i != pbox ) // do not check ourself
    {
      if ( placed_box_list[i].pos.y <= pbox->pos.y )
      {
	if ( x >=  placed_box_list[i].pos.x && x < placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w )
	{
	  // upper box found, but is it the next ?
	  // must use >= here, because the initial boxes have zero area
	  if ( max_y <= placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h )
	  {
	    max_y = placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h;
	    found_i = i;
	  }
	}
      }
    }
  }
  if (found_i >= 0)
    place_option_by_xy(x, max_y);

}

void add_all_place_options(void)
{
  int i;
  //place_option_cnt = 0;	// clear the place option list
  for( i = 2; i < placed_box_cnt; i++ )
  {
    place_option_by_pbox(placed_box_list+i);
  }
}

void show(void)
{
  pos_t p;
  box_t b;
  int i;
  b.w = 1;
  b.h = 1;
  for( p.y = 0; p.y < 30; p.y++ )
  {
    for( p.x = 0; p.x < 60; p.x++ )
    {

      for( i = 0; i < place_option_cnt; i++ )
      {
	if ( place_option_list[i].x == p.x &&  place_option_list[i].y == p.y )
	{
	  break;
	}
      }
      if ( i < place_option_cnt )
      {
	printf("*");
      }
      else
      {
	
	for( i = 0; i < placed_box_cnt; i++ )
	{
	  if ( is_intersection_with_placed_box(&p, &b, i) != 0 )
	    break;
	}
	if ( i < placed_box_cnt )
	{
	  printf("%c", i+'a');
	}
	else
	{
	  printf(" ");
	}
      }
    }
    printf("\n");
  }
}

void init(void)
{
  pos_t p;
  

  box_list[0].w = 0x0ffffffff;
  box_list[0].h = 0;

  box_list[1].w = 0;
  box_list[1].h = 0x0ffffffff;
  
  box_cnt = 2;
  
  p.x = 0;
  p.y = 0;  
  placed_box_push(&p, 0);

  p.x = 0;
  p.y = 0;  
  placed_box_push(&p, 1);

  // create one place option at the upper left
  place_option_by_xy(0, 0);
  
}

void do_best_place(int box_idx)
{
  int i;
  int found_i;
  pbox_t *pbox;
  uint32_t value;
  uint32_t lowest_value;
  
  found_i = -1;
  lowest_value = 0x0ffffffff;
  for( i = 0; i < place_option_cnt; i++ )
  {
    // check whether this placement would generate an intersection
    if ( is_any_intersection(place_option_list+i, box_list+box_idx) == 0 )
    {
      // place the box at the position
      pbox = placed_box_push(place_option_list+i, box_idx);
      value = placed_box_calculate_max_area();
      value *= 8;
      value += pbox->pos.x;
      value += pbox->pos.y;
      
      // greedy algorithm: We search for the lowest area increase
      if ( lowest_value > value )
      {
	lowest_value = value ;
	found_i = i;
      }
      
      placed_box_pop();
    }
  }

  if ( found_i >= 0 )
  {
    
    // now permanently place the box
    pbox = placed_box_push(place_option_list+found_i, box_idx);

    // delete the position from the place option list
    place_option_delete(found_i);
    
    // calculate new place options with the new box
    //place_option_by_pbox(pbox);
 
    // recalculate all place options
    add_all_place_options();
  }
  else
  {
    assert(found_i >= 0 );
  }
}





  
u8g2_t u8g2;


void cloud_add(const uint8_t *font, char *word)
{
  u8g2_uint_t extra = 8;
  u8g2_SetFont(&u8g2, font);
  box_list[box_cnt].w = u8g2_GetStrWidth(&u8g2, word) + extra; 
  box_list[box_cnt].h = u8g2_GetAscent(&u8g2) - u8g2_GetDescent(&u8g2) + extra;
  strcpy(box_word[box_cnt], word); 
  box_font[box_cnt] = font;
  box_cnt++;
}

void cloud_init(void)
{
  init();  
}

void cloud_calculate(void)
{
  int i;

  for( i = 2; i < box_cnt; i++ )
    do_best_place(i);
  
}



int main(void)
{
  int i;

  u8g2_SetupBuffer_TGA_DESC(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  u8g2_SetFont(&u8g2, u8g2_font_helvB18_tr);
  u8g2_SetFontPosTop(&u8g2);
  
  cloud_init();
  cloud_add(u8g2_font_helvR24_tr, "ABC");
  cloud_add(u8g2_font_helvB24_tr, "XYZ");
  cloud_add(u8g2_font_helvR12_tr, "Embedded");
  cloud_add(u8g2_font_helvB12_tr, "Electronics");
  cloud_add(u8g2_font_helvR14_tr, "Arduino");
  cloud_add(u8g2_font_helvB14_tr, "Font");
  cloud_add(u8g2_font_helvR18_tr, "Controller");
  cloud_add(u8g2_font_helvB18_tr, "U8g2");
  cloud_calculate();
  
    
    u8g2_FirstPage(&u8g2);
    do
    {
      for( i = 2; i < placed_box_cnt; i++ )
      {
	u8g2_SetFont(&u8g2, box_font[placed_box_list[i].box_idx]);
	u8g2_DrawStr(&u8g2, 
	  placed_box_list[i].pos.x, 
	  placed_box_list[i].pos.y, 
	  box_word[placed_box_list[i].box_idx]);	
	/*
	u8g2_DrawFrame(&u8g2,
	  placed_box_list[i].pos.x, 
	  placed_box_list[i].pos.y, 
	  box_list[placed_box_list[i].box_idx].w, 
	  box_list[placed_box_list[i].box_idx].h);
	*/
      }
      
    u8g2_SetFont(&u8g2, u8g2_font_helvB18_tr);
     // u8g2_DrawStr(&u8g2, 20, 20, "abc");
      
    } while( u8g2_NextPage(&u8g2) );
  tga_save("u8g2.tga");
  
  return 0;
}

