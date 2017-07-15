/*

  item.h
  
*/

#ifndef _ITEM_H
#define _ITEM_H

#include <stdint.h>

struct _pos_struct
{
  uint8_t x;
  uint8_t y;  
};
typedef struct _pos_struct pos_t;

struct _item_struct
{
  pos_t pos;
  uint8_t dir;			/* movement has two parts: 1. dir is set, then 2. dir is executed */
  uint8_t tile;			/* current foreground tile, defaults to the value from the template list */
  uint8_t template_index;  /* index into item_template_list[] */
};
typedef struct _item_struct item_t;


void posStep(pos_t *pos, uint8_t dir);
void moveAllItems(void);

uint8_t getMapTileByPos(pos_t *pos);


void setupLevel(uint8_t level);
uint8_t getMapTile(uint8_t x, uint8_t y);

#endif /* _ITEM_H */