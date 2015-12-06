
#ifndef _U8X8LIB_HH
#define _U8X8LIB_HH

#include <Arduino.h>
#include <Print.h>

#include "u8x8.h"

class U8x8 : public Print
{
  protected:
    u8x8_t u8x8;
  public:
    U8x8(void) {}  
    ucg_t *getU8x8(void) { return &u8x8; };
}

class U8x8_4W_SW_SPI : public  U8x8
{
  
}
  


  


#endif /* _U8X8LIB_HH */