


#ifndef _MAP_H
#define _MAP_H

#include <stdint.h>


struct _map_struct
{
  unsigned char *data;
  uint8_t width;
  uint8_t height;
};
typedef struct _map_struct map_t;

struct _item_template_struct
{
  uint16_t init_proc;
  uint16_t hit_proc;
  uint16_t step_proc;  
}
typedef struct _item_template_struct item_template_t;

extern map_t map_list[];

#endif