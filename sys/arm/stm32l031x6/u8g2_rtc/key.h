/*
  key.h
*/

#ifndef _KEY_H
#define KEY_H

#define KEY_NONE 0
#define KEY_SELECT 1
#define KEY_NEXT 2

int key_add(uint8_t key);
uint8_t key_get(void);

extern volatile uint8_t key_queue_start;
extern volatile uint8_t key_queue_end;	


#endif