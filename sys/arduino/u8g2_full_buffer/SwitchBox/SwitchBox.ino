/*

  HelloWorld.ino

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <Arduino.h>
#include <U8g2lib.h>


uint8_t light_to_pin[4] = { 11, 10, 9, 8 };
uint8_t switch_to_pin[4] = { 7, 6, 5, 4 };

#define SWITCH_ON 0
#define SWITCH_OFF 0
#define LIGHT_ON 1
#define LIGHT_OFF 0
#define TIMEOUT 3000

uint8_t switch_status[4];
#define DEBOUNCE_CNT 3
uint8_t switch_debounce[4];
uint8_t is_switch_changed;
uint8_t last_switch_on_cnt;	// number of "on" leds before the last change
uint8_t switch_on_cnt;		// current number of "on" leds
uint8_t max_on_cnt;			// last number max number of "on" leds

// max_on_cnt==1: max_on_cnt_pos1 --> on led
// max_on_cnt==2: max_on_cnt_pos1/2 --> on led
// max_on_cnt==3: max_on_cnt_pos1 --> off led
int8_t max_on_cnt_pos1 = -1;
int8_t max_on_cnt_pos2 = -1;

uint32_t switch_changed_millis;

#define STATE_WAIT 0

uint8_t state = STATE_WAIT;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R3, /* reset=*/ U8X8_PIN_NONE);  

U8G2LOG u8g2log;
uint8_t u8log_buffer[6*14];


void init_switch(void)
{
  uint8_t i;
  for( i = 0; i < 4; i++ )
  {
    pinMode(switch_to_pin[i], INPUT_PULLUP);    
    switch_debounce[i] = 0;
    switch_status[i] = 2;
  }
}

void read_switch_status(void)
{
  uint8_t i, v, on_cnt;
  int8_t pos1, pos2, npos1;
  for( i = 0; i < 4; i++ )
  {
     v = digitalRead(switch_to_pin[i]);
     if ( switch_debounce[i] > 0 )
     {
      if ( switch_status[i] == v )
      {
	switch_debounce[i] = 0;	// do nothing, wait for other values
      }
      else
      {
	switch_debounce[i]--;
	if ( switch_debounce[i] == 0 )
	{
	  switch_status[i] = v;	// new value detected
	  is_switch_changed = 1;
	  switch_changed_millis = millis();
	}
      }
     }
     else if ( switch_status[i] != v )
     {
	switch_debounce[i] = DEBOUNCE_CNT;	// start debounce
     }
  }
  
  on_cnt = 0;
  pos1 = -1;
  for( i = 0; i < 4; i++ )
  {
    if ( switch_status[i] == SWITCH_ON )
    {
      on_cnt++;
      if ( pos1 < 0 )
	pos1 = i;
      else
	pos2 = i;
    }
    else
    {
      npos1 = i;
    }
  }
  
  if ( switch_on_cnt != on_cnt )
  {
    last_switch_on_cnt = switch_on_cnt;
    switch_on_cnt = on_cnt; 
    if ( last_switch_on_cnt < switch_on_cnt )
    {
      max_on_cnt = on_cnt;
      if ( max_on_cnt == 1 )
      {
	max_on_cnt_pos1 = pos1; 
	max_on_cnt_pos2 = -1; 
      }
      else if ( max_on_cnt == 2 )
      {
	max_on_cnt_pos1 = pos1; 
	max_on_cnt_pos2 = pos2; 
      }
      else if ( max_on_cnt == 3 )
      {
	max_on_cnt_pos1 = npos1; 
	max_on_cnt_pos2 = -1; 
      }
      else
      {
	max_on_cnt_pos1 = -1; 
	max_on_cnt_pos2 = -1; 
      }
    }
  }
  
}

void print_switch_status(void)
{
  uint8_t i;
  
  u8g2log.print(last_switch_on_cnt);
  u8g2log.print(">");
  u8g2log.print(switch_on_cnt);
  u8g2log.print("/");
  u8g2log.print(max_on_cnt);
  u8g2log.print("\n");
  
  u8g2log.print(max_on_cnt_pos1);
  u8g2log.print("/");
  u8g2log.print(max_on_cnt_pos2);
  u8g2log.print("\n");
  
  for( i = 0; i < 4; i++ )
    u8g2log.print(switch_status[i] == 0 ? "*" : ".");
  u8g2log.print("\n");
}


void init_light(void)
{
  uint8_t i;
  for( i = 0; i < 4; i++ )
  {
    digitalWrite(light_to_pin[i], 1);
    pinMode(light_to_pin[i], OUTPUT);    
    digitalWrite(light_to_pin[i], 1);
  }
}

void write_switch_to_light(void)
{
  uint8_t i;
  for( i = 0; i < 4; i++ )
  {
    if ( switch_status[i] == SWITCH_ON )
      digitalWrite(light_to_pin[i], LIGHT_ON);
    else
      digitalWrite(light_to_pin[i], LIGHT_OFF);
  }
}

/*==============================================*/

void next_state(void)
{
  read_switch_status();
  switch(state)
  {
    case STATE_WAIT:
      if ( switch_changed_millis+TIMEOUT < millis() )
      {
	u8g2log.print("t-out\n");
	switch_changed_millis = millis();
      }
      if ( is_switch_changed != 0 )
      {
	print_switch_status();
	write_switch_to_light();
	is_switch_changed = 0;
      }
  }
}


/*==============================================*/
void setup(void) 
{
  u8g2.begin();
  //u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);	// set the font for the terminal window
    
  //u8g2.setFont(u8g2_font_lucasfont_alternate_tr);
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2log.begin(u8g2, 6, 14, u8log_buffer);	  
  
  init_switch();
  init_light();
}

void loop(void) 
{
  next_state();
}

