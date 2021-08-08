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


int main(void)
{
  int i = 0;
  int pins[4] = { 12, 13, 16, 17 };

  if (!bcm2835_init())
    exit(1);

  signal(SIGTERM, sigtermhandler);
  signal(SIGINT, siginthandler);

  atexit((void (*) (void))bcm2835_close);

  for( i = 0; i < sizeof(pins)/sizeof(*pins); i++ )
  {
    bcm2835_gpio_fsel(pins[i], BCM2835_GPIO_FSEL_INPT);
    
    //bcm2835_gpio_hen(pins[i]);  // high level detect
  }
  for(;;)
  {
    
    delaynanoseconds(500000000UL);
    
    printf("%07d  ", i);
    for( i = 0; i < sizeof(pins)/sizeof(*pins); i++ )
    {
      printf("%d: %d  ", pins[i], bcm2835_gpio_lev(pins[i]));
    }
    puts("");
    
    /*
    for( i = 0; i < sizeof(pins)/sizeof(*pins); i++ )
    {
      if ( bcm2835_gpio_eds(pins[i]) )  // event detected
      {
        bcm2835_gpio_set_eds(pins[i]);  // clear the event
        printf("Event: %d\n", pins[i]);
      }
    }
    */
    i++;
  }
}