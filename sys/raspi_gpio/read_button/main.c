/*

  main.c

  raspberry pi zero GPIO read test

  For speedup run this example as root, either wis
    sudo ./main
  or by setting the superuser bit:
    sudo chown root:root ./main
    sudo chmod u+s ./main

*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "bcm2835.h"
#include "raspi_gpio_hal.h"

void sigtermhandler(int x)
{
  puts("SIGTERM");
  bcm2835_close();
  exit(0);
}

void siginthandler(int x)
{
  puts("SIGINT");
  bcm2835_close();
  exit(0);
}


#define BUTTON_STATE_NOT_PRESSED 0
#define BUTTON_STATE_HIGH_DETECT 1
#define BUTTON_STATE_HIGH_WAIT 2
#define BUTTON_STATE_LOW_DELAY 3


struct button_struct
{
  int state;
  int delay;
  uint8_t pin;
};
typedef struct button_struct button_t;
#define BUTTON_DELAY_HIGH 2
#define BUTTON_DELAY_LOW 2

#define BUTTON_CNT 2
button_t button_list[BUTTON_CNT];

int button_init(void)
{
  int i;
  for( i = 0; i < BUTTON_CNT; i++ )
  {
    bcm2835_gpio_fsel(button_list[i].pin, BCM2835_GPIO_FSEL_INPT);
  }
}

int button_get_event(void)
{
  int button_value = -1;
  int i;
  for( i = 0; i < BUTTON_CNT; i++ )
  {
    switch(button_list[i].state)
    {
      case BUTTON_STATE_NOT_PRESSED:
        button_list[i].delay = 0;
        if ( bcm2835_gpio_lev(button_list[i].pin) != 0 )
          button_list[i].state = BUTTON_STATE_HIGH_DETECT;
        break;
      case BUTTON_STATE_HIGH_DETECT:
        if ( bcm2835_gpio_lev(button_list[i].pin) == 0 )
        {
          button_list[i].delay = 0;
          button_list[i].state = BUTTON_STATE_LOW_DELAY;
        }
        button_list[i].delay++;
        if ( button_list[i].delay >= BUTTON_DELAY_HIGH )
        {
          button_value = i;
          button_list[i].state = BUTTON_STATE_HIGH_WAIT;
        }
        break;
      case BUTTON_STATE_HIGH_WAIT:
        if ( bcm2835_gpio_lev(button_list[i].pin) == 0 )
        {
          button_list[i].delay = 0;
          button_list[i].state = BUTTON_STATE_LOW_DELAY;
        }
        break;
      case BUTTON_STATE_LOW_DELAY:
        button_list[i].delay++;
        if ( button_list[i].delay >= BUTTON_DELAY_LOW )
          button_list[i].state = BUTTON_STATE_NOT_PRESSED;
        break;
    }
  }
  return button_value;
}

int main(void)
{
  int i = 0;
  int b = -1;
  int pins[4] = { 12, 13, 16, 17 };

  if (!bcm2835_init())
    exit(1);

  signal(SIGTERM, sigtermhandler);
  signal(SIGINT, siginthandler);

  atexit((void (*) (void))bcm2835_close);

  button_list[0].pin = 12;
  button_list[1].pin = 16;
  
  button_init();
  for( ;; )
  {
    b = button_get_event();
    if ( b >= 0 )
      printf("Button %d\n", b);
  }
  
  
    //delaynanoseconds(500000000UL);
    
}