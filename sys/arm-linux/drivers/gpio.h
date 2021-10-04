#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

// Direction
#define GPIO_IN                 (0) 
#define GPIO_OUT                (1) 

// Value
#define GPIO_LOW                (0)
#define GPIO_HIGH               (1)

//nanopi
#define GPIO_PIN(x)             (x)

int exportGPIOPin(int pin);
int unexportGPIOPin(int pin);

// GPIO_IN or GPIO_OUT
int setGPIODirection(int pin, int direction);
int getGPIODirection(int pin);

// GPIO_LOW or GPIO_HIGH
int setGPIOValue(int pin, int value);
int getGPIOValue(int pin);

#endif
