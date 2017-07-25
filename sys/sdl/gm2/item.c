/*

  item.c
  
  during level setup, the item pool is filled with items from the
  onmap list.
  
*/

#include <stddef.h>
#include "ugl_bc.h"
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

item_t *item_under_pos;	/* set by getMapTile() */

bc_t bc;


/*===============================================*/


void execute(uint16_t pos)
{
  bc_exec(&bc, map_code, pos);
}

/*===============================================*/

void posStep(pos_t *pos, uint8_t dir)
{
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
    default:
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
  item_pool[item_cnt].dir = 4; /* no move */
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
  item = item_pool;
  do
  {
    posStep(&(item->pos), item->dir);
    item->dir = 4;	/* no move */
    item++;
    i--;
  } while( i != 0);
}

void callStepAllItems(void)
{
  uint8_t i;
  item_t *item;
  i = item_cnt;
  item = item_pool;
  do
  {
    execute(item_template_list[item->template_index].step_proc);
    
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
  
  execute(map_list[level].init_proc);

}

/*
  return a tile on the map.
  as a side effect, 
    item_under_pos
  is set if the tile is from an item. Otherwise item_under_pos is set to NULL
*/
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
    {
      return item->tile;
      item_under_pos = item;
    }
  }
  
  item_under_pos = NULL;
  
  offset = y;
  offset *= map_list[current_level].width;
  offset += x;
  
  return map_list[current_level].data[offset];
}

/*
  sideeffect: will set item_under_pos
*/
uint8_t getMapTileByPos(pos_t *pos)
{
  return getMapTile(pos->x, pos->y);
}

/*
  Check whether a tile is solid by definition.
  The case, when hitting a tile and it became wakable (because it disapears
  or moves away) is not considered.
*/
uint8_t isSolidTile(uint8_t tile)
{
  if ( tile >= 0x07d && tile <= 0x088 )
    return 1;
  return 0;
}

/*
  sideeffect: will set item_under_pos
*/
uint8_t canWalkTo(pos_t *pos)
{
  uint8_t tile;
  tile = getMapTileByPos(pos);
  /* 
  if ( item_under_pos != NULL )
    ...
  */
  if ( isSolidTile(tile) != 0 )
    return 0;
  return 1;
}


/*
  0: not moved
  1: moved
  sideeffect: will set item_under_pos
*/
uint8_t moveItem(uint8_t item_index, uint8_t dir)
{
  item_t *item;
  pos_t pos;
  item = pool_GetItem(item_index);
  pos = item->pos;
  posStep(&pos, dir);
  if ( canWalkTo(&pos) != 0 )
  {
    item->dir = dir;
    return 1;
  }
  return 0;
}

