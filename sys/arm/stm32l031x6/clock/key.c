/*
  key.c
*/

#include <stdint.h>
#include "stm32l031xx.h"	/* IRQ disable/enable */
#include "key.h"

#define KEY_QUEUE_MASK 0x0f
#define KEY_QUEUE_MAX (KEY_QUEUE_MASK+1)
uint8_t volatile key_queue[KEY_QUEUE_MAX];
uint8_t volatile key_queue_start = 0;
uint8_t volatile key_queue_end = 0;	


int key_add(uint8_t key)
{
  uint8_t new_end;
  new_end = key_queue_end;
  new_end++;
  new_end &= KEY_QUEUE_MASK;
  if ( new_end == key_queue_start )
    return 0;	/* buffer is full */
  key_queue[key_queue_end] = key;
  key_queue_end = new_end;
  return 1;
}

uint8_t key_get(void)
{
  uint8_t key;
  uint8_t new_start;
  
  if ( key_queue_start == key_queue_end )
  {
    return KEY_NONE;
  }
  //__disable_irq();
  key = key_queue[key_queue_start];
  new_start = key_queue_start;
  new_start++;
  new_start &= KEY_QUEUE_MASK;
  key_queue_start = new_start;
  //__enable_irq();
  return key;
}

