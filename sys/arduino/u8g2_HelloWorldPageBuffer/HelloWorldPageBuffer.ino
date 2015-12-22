#include <Arduino.h>
#include <SPI.h>

#include <U8g2lib.h>

U8G2_SSD1306_128x64_NONAME_1_SW_SPI u8g2(&u8g2_cb_r0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);



void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"Hello World!");
  } while ( u8g2.nextPage() );
  delay(1000);
}

