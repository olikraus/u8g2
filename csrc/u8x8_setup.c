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


/*
  Description:
    Setup u8x8 and assign the callback function. The dummy 
    callback "u8x8_dummy_cb" can be used, if no callback is required.
    This setup will not communicate with the display itself.
    Use u8x8_InitDisplay() to send the startup code to the Display.
  Args:
    u8x8				An empty u8x8 structure
    display_cb			Display/controller specific callback function
    cad_cb				Display controller specific communication callback function
    byte_cb			Display controller/communication specific callback funtion
    gpio_and_delay_cb	Environment specific callback function

*/
void u8x8_Setup(u8x8_t *u8x8, u8x8_msg_cb display_cb, u8x8_msg_cb cad_cb, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  /* setup defaults and reset pins to U8X8_PIN_NONE */
  u8x8_SetupDefaults(u8x8);

  /* setup specific callbacks */
  u8x8->display_cb = display_cb;
  u8x8->cad_cb = cad_cb;
  u8x8->byte_cb = byte_cb;
  u8x8->gpio_and_delay_cb = gpio_and_delay_cb;

  /* setup display info */
  u8x8_SetupMemory(u8x8);
}

