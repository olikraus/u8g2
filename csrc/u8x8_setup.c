/*

  u8x8_setup.c
  
*/


#include "u8x8.h"

uint8_t u8x8_dummy_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* the dummy callback will not handle any message and will fail for all messages */
  return 0;
}


/*
  Description:
    Setup u8x8
  Args:
    u8x8	An empty u8x8 structure
*/
void u8x8_SetupDefaults(u8x8_t *u8x8)
{
    u8x8->display_info = NULL;
    u8x8->display_cb = u8x8_dummy_cb;
    u8x8->cad_cb = u8x8_dummy_cb;
    u8x8->byte_cb = u8x8_dummy_cb;
    u8x8->gpio_and_delay_cb = u8x8_dummy_cb;
  
#ifdef U8X8_USE_PINS 
  {
    uint8_t i;
    for( i = 0; i < U8X8_PIN_CNT; i++ )
      u8x8->pins[i] = U8X8_PIN_NONE;
  }
#endif
  
    
}


