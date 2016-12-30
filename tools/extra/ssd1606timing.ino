/* Arduino Smart_Eink Library 
 * Copyright (C) 2016 by NOA Labs
 * Author  Bruce Guo (NOA Labs)
 *
 * This file is E-ink demo showing string.
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this Library. If not, see
 * <http://www.gnu.org/licenses/>.
 */
/*
D/C ~ D5
CS ~ D6
BUSY ~ D7
BS ~ D8
MOSI ~ D11
MISO ~ D12
CLK ~ D13

*/


 
#include <SmartEink.h>
#include <SPI.h>

E_ink Eink;

unsigned long wait_max;

void wait(void)
{
  unsigned v;
  unsigned long w;
  w = millis();
  do
  {
    v = digitalRead(A0);
  } while( v != 0 );
  w = millis() - w;
  Serial.print("wait: ");
  Serial.print(w);
  Serial.println(" ");
}

void setup()
{
    Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  
  digitalWrite(8, HIGH);

  delay(1000);
  Eink.InitEink();
  delay(1000);

  Eink.WriteComm(0x3b);//set gate line width
  Eink.WriteData(0x08);//60 + value*4 us  --> this influences delay 4: 694/ms, 9:1062/297, 8(POR):957/266

}

unsigned x = 0;
void loop()
{

Eink.WriteComm(0x44);//set RAM x address start/end, in page 36
Eink.WriteData(0x00);//RAM x address start at 00h;
Eink.WriteData(0x11);//RAM x address end at 11h(17)->72: [because 1F(31)->128 and 12(18)->76] 
Eink.WriteComm(0x45);//set RAM y address start/end, in page 37
Eink.WriteData(0x00);//RAM y address start at 00h;
Eink.WriteData(0xab);//RAM y address start at ABh(171)->172: [because B3(179)->180]
 

  Eink.WriteComm(0x11);//data enter mode
  Eink.WriteData(0x07);
  Eink.WriteComm(0x4E);//set RAM x address count to 0;
  Eink.WriteData(0x00);
  Eink.WriteComm(0x4F);//set RAM y address count to 0;
  Eink.WriteData(0);

  Eink.ClearScreen();// clear the screen

  Eink.WriteComm(0x11);//data enter mode
  Eink.WriteData(0x07);
  Eink.WriteComm(0x4E);//set RAM x address count to 0;
  Eink.WriteData(0x08);
  Eink.WriteComm(0x4F);//set RAM y address count to 0;
  Eink.WriteData(x*2);
  x ++;
  x = x & 0x01f;
  //delay(1000); 
  

  Eink.WriteComm(0x24);
  
  Eink.WriteData(0x00);
  Eink.WriteData(0x03);
  Eink.WriteData(0x0f);
  Eink.WriteData(0x3f);
  Eink.WriteData(0xff);
  
  Eink.WriteData(0xff);
  Eink.WriteData(0xff);
  Eink.WriteData(0xff);
  Eink.WriteData(0xff);

  Eink.WriteData(0x03);
  Eink.WriteData(0x0f);
  Eink.WriteData(0x3f);
  Eink.WriteData(0xff);
  
  Eink.WriteData(0xff);
  Eink.WriteData(0xff);
  Eink.WriteData(0xff);
  Eink.WriteData(0xff);
  Eink.WriteData(0x00);

  Eink.WriteComm(0x22);//display updata sequence option ,in page 33
  Eink.WriteData(0xC0);//enable sequence: clk -> CP -> on
  Eink.WriteComm(0x20);
  Serial.print("Clk+CP ");
  wait();

  //delay(1000); 
  Eink.WriteComm(0x22);//display updata sequence option ,in page 33
  Eink.WriteData(0x04);//enable sequence: clk -> CP -> LUT -> initial display -> pattern display
  Eink.WriteComm(0x20);
  //delay(1000); 
  Serial.print("To Display ");
  wait();
  
  Eink.WriteComm(0x22);
  Eink.WriteData(0x03);
  Eink.WriteComm(0x20);
  //delay(1000); 
  
  Serial.print("CP/Clock off ");
  wait();

  //delay(1000); 

//Eink.ClearScreen();// clear the screen

//Eink.EinkP8x16Str(14,8,"NOA-Labs.com");
//Eink.EinkP8x16Str(10,8,"smart-prototyping.com");
//Eink.EinkP8x16Str(6,8,"0123456789");
//Eink.EinkP8x16Str(2,8,"ABCDEFG abcdefg");

//Eink.RefreshScreen(); 
}
void xloop()
{ 

}

