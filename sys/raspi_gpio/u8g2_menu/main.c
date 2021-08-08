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
#include "u8g2.h"


u8g2_t u8g2; // a structure which will contain all the data for one display


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


/*
  title: 		NULL for no title, valid str for title line. Can contain mutliple lines, separated by '\n'
  start_pos: 	default position for the cursor, first line is 1.
  sl:			string list (list of strings separated by \n)
  returns 0 if user has pressed the home key
  returns the selected line if user has pressed the select key
  side effects:
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFontPosBaseline(u8g2);
	
*/

#define BUTTON_SELECT 0
#define BUTTON_NEXT 1

extern void u8g2_DrawSelectionList(u8g2_t *u8g2, u8sl_t *u8sl, u8g2_uint_t y, const char *s);

#define MY_BORDER_SIZE 1
#define MY_RULE_SIZE 2

uint8_t u8g2_RaspiUserInterfaceSelectionList(u8g2_t *u8g2, const char *title, uint8_t start_pos, const char *sl)
{
  u8sl_t u8sl;
  u8g2_uint_t yy;

  uint8_t event;

  u8g2_uint_t line_height = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2)+MY_BORDER_SIZE;

  uint8_t title_lines = u8x8_GetStringLineCnt(title);
  uint8_t display_lines;

  
  if ( start_pos > 0 )	/* issue 112 */
    start_pos--;		/* issue 112 */


  if ( title_lines > 0 )
  {
	display_lines = (u8g2_GetDisplayHeight(u8g2)-2-MY_RULE_SIZE) / line_height;
	u8sl.visible = display_lines;
	u8sl.visible -= title_lines;
  }
  else
  {
	display_lines = u8g2_GetDisplayHeight(u8g2) / line_height;
	u8sl.visible = display_lines;
  }

  u8sl.total = u8x8_GetStringLineCnt(sl);
  u8sl.first_pos = 0;
  u8sl.current_pos = start_pos;

  if ( u8sl.current_pos >= u8sl.total )
    u8sl.current_pos = u8sl.total-1;
  if ( u8sl.first_pos+u8sl.visible <= u8sl.current_pos )
    u8sl.first_pos = u8sl.current_pos-u8sl.visible+1;

  u8g2_SetFontPosBaseline(u8g2);
  
  for(;;)
  {
      u8g2_FirstPage(u8g2);
      do
      {
        yy = u8g2_GetAscent(u8g2);
        if ( title_lines > 0 )
        {
          yy += u8g2_DrawUTF8Lines(u8g2, 0, yy, u8g2_GetDisplayWidth(u8g2), line_height, title);
		
	  //u8g2_DrawHLine(u8g2, 0, yy-line_height- u8g2_GetDescent(u8g2) + 1, u8g2_GetDisplayWidth(u8g2));
	  u8g2_DrawBox(u8g2, 0, yy-line_height- u8g2_GetDescent(u8g2) + 1, u8g2_GetDisplayWidth(u8g2), MY_RULE_SIZE);
		
          
	  yy += 2+MY_RULE_SIZE;
        }
        u8g2_DrawSelectionList(u8g2, &u8sl, yy, sl);
      } while( u8g2_NextPage(u8g2) );
      
#ifdef U8G2_REF_MAN_PIC
      return 0;
#endif


      for(;;)
      {
        event = button_get_event();
        if ( event == BUTTON_SELECT )
          return u8sl.current_pos+1;		/* +1, issue 112 */
        else if ( event == BUTTON_NEXT  )
        {
          u8sl_Next(&u8sl);
          break;
        }
        /*
        else if ( event == U8X8_MSG_GPIO_MENU_PREV || event == U8X8_MSG_GPIO_MENU_UP )
        {
          u8sl_Prev(&u8sl);
          break;
        }
        */
      }
  }
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
  
  
  u8g2_Setup_sh1107_i2c_seeed_128x128_f(&u8g2, U8G2_R3, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_raspi_gpio_hal);  // init u8g2 structure
  u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, 5);
  u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, 6);
  u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
  u8g2_SetPowerSave(&u8g2, 0); // wake up display  
  u8g2_ClearBuffer(&u8g2);
  u8g2_SendBuffer(&u8g2);
  
  u8g2_SetFont(&u8g2, u8g2_font_lastapprenticebold_tr);
  //u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);
  
  //u8g2_DrawStr(&u8g2, 0, 50, "Hello");
  //u8g2_DrawStr(&u8g2, 0, 100, "World!");
  //u8g2_SendBuffer(&u8g2);  
  
  u8g2_RaspiUserInterfaceSelectionList(&u8g2, "Menu", 1, "apple\nbanana\nraspberry\nMango");
  
  for( ;; )
  {
    b = button_get_event();
    if ( b >= 0 )
      printf("Button %d\n", b);
  }
  
  
    //delaynanoseconds(500000000UL);
    
}

