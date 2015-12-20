/*
  
  U8g2lib.h
  
  C++ Arduino wrapper for the u8g2 struct and c functions for the u8g2 library


  U8x8lib.h is included for the declaration of the helper functions in U8x8lib.cpp.
  U8g2 class is based on the u8g2 struct from u8g2.h, the U8x8 class from U8x8lib.h is not used.
  
*/


#ifndef _U8G2LIB_HH
#define _U8G2LIB_HH

#include <Arduino.h>
#include <Print.h>
#include <U8x8lib.h>

#include "u8g2.h"

class U8G2 : public Print
{
  protected:
    u8g2_t u8g2;
  public:
    uint8_t tx, ty;
  
    U8G2(void) { home(); }
    u8x8_t *getU8x8(void) { return u8g2_GetU8x8(&u8g2); }
    u8g2_t *getU8g2(void) { return &u8g2; }

    /* u8x8 interface */
    uint8_t getCols(void) { return u8x8_GetCols(u8g2_GetU8x8(&u8g2)); }
    uint8_t getRows(void) { return u8x8_GetRows(u8g2_GetU8x8(&u8g2)); }
    
    
};




#endif /* _U8G2LIB_HH */