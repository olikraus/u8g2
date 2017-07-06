


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

extern map_t map_list[];

#endif