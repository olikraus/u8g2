/*

  item.h
  
*/

#ifndef _ITEM_H
#define _ITEM_H

#include <stdint.h>

struct _item_struct
{
  uint8_t x;
  uint8_t y;
  uint8_t tile;			/* current foreground tile, defaults to the value from the template list */
  uint8_t template_index;  /* index into item_template_list[] */
};
typedef struct _item_struct item_t;



void setupLevel(uint8_t level);
uint8_t getMapTile(uint8_t x, uint8_t y);

#endif /* _ITEM_H */