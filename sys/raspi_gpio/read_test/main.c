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
}


int main(void)
{

  if (!bcm2835_init())
    exit(1);

  signal(SIGTERM, sigtermhandler);

  atexit((void (*) (void))bcm2835_close);

  bcm2835_gpio_fsel(12, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(13, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(16, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(17, BCM2835_GPIO_FSEL_INPT);
  
  for(;;)
  {
    
    delaynanoseconds(1000000UL);
    
    printf("%d: %d  ", 12, bcm2835_gpio_lev(12));
    printf("%d: %d  ", 13, bcm2835_gpio_lev(13));
    printf("%d: %d  ", 16, bcm2835_gpio_lev(16));
    printf("%d: %d  ", 17, bcm2835_gpio_lev(17));
    puts("");
    
  }
}