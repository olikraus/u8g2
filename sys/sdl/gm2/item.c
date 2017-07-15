/*

  item.c
  
  during level setup, the item pool is filled with items from the
  onmap list.
  
*/

#include "item.h"
#include "map.h"

/* max number of items in the pool */
#define ITEM_MAX 32

/* current number of items in the pool */
uint8_t item_cnt;

/* current level */
uint8_t current_level;

/* the pool with all dynamic created items */
item_t item_pool[ITEM_MAX];


/*===============================================*/

void posStep(pos_t *pos, uint8_t dir)
{
  dir &= 3;
  switch( dir )
  {
    case 0:
	pos->x+=1;
	break;
    case 1:
	pos->y+=1;
	break;
    case 2:
	pos->x-=1;
	break;
    case 3:
	pos->y-=1;
	break;
  }
}

/*===============================================*/


void pool_Clear(void)
{
  item_cnt = 0;
}

uint8_t pool_NewItem(void)
{
  if ( item_cnt >= ITEM_MAX )
    return ITEM_MAX;
  item_pool[item_cnt].dir = 0;
  item_cnt++;
  return item_cnt-1;
}

item_t *pool_GetItem(uint8_t idx)
{
  return item_pool+idx;
}

/*
  Based on the dir attribute, all items, including hero are moved
*/
void moveAllItems(void)
{
  uint8_t i;
  item_t *item;
  i = item_cnt;
  *item = item_pool;
  do
  {
    posStep(&(item->pos), item->dir);
    item->dir = 0;
    item++;
    i--;
  } while( i != 0);
}


/*===============================================*/

/*
void item_SetDefaultTile(uint8_t idx)
{
  item_t *item = pool_GetItem((idx);
  
  item->tile = item_template_list[item->template_index].fg_tile;
}
*/


/*===============================================*/

void setupLevel(uint8_t level)
{
  uint8_t i, cnt;
  item_t *item;
  item_onmap_t *onmap_ptr;
  current_level = level;
  
  cnt = map_list[level].onmap_cnt;
  
  /* build the pool */
  pool_Clear();
  
  /* first item always is our hero (index 0) */
  item = pool_GetItem(pool_NewItem());
  item->pos.x = 0;
  item->pos.y = 0;
  item->tile = 0x04e;
  item->template_index = 0;	/* not used, but still template index should be reserverd then */
  
  onmap_ptr = map_list[level].onmap_list;
  for( i = 0; i < cnt; i++ )
  {
    /* no check of pool_NewItem() here, this should always succeed */
    item = pool_GetItem(pool_NewItem());
    item->pos.x = onmap_ptr->x;
    item->pos.y = onmap_ptr->y;
    item->template_index = onmap_ptr->template_index;
    item->tile = item_template_list[item->template_index].fg_tile;
    onmap_ptr++;
  }
}

uint8_t getMapTile(uint8_t x, uint8_t y)
{
  item_t *item;
  uint16_t offset;
  uint8_t i, cnt;
 
  cnt = item_cnt;
  for( i = 0; i < cnt; i++ )
  {
    item = pool_GetItem(i);
    if ( item->pos.x == x && item->pos.y == y )
      return item->tile;
  }
  
  offset = y;
  offset *= map_list[current_level].width;
  offset += x;
  
  return map_list[current_level].data[offset];
}

uint8_t getMapTileByPos(pos_t *pos)
{
  return getMapTile(pos->x, pos->y);
}
