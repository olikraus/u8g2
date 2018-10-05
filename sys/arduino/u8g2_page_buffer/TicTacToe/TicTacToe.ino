/*

  TicTacToe.ino
  
  Tic Tac Toe (three in a row) game

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2018, olikraus@gmail.com
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




/*=== ARDUBOY Production, Kickstarter Edition ===*/
U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6);	// Arduboy (Production, Kickstarter Edition)

void setup(void) 
{
  u8g2.begin(/*Select=*/ 7, /*Right/Next=*/ A1, /*Left/Prev=*/ A2, /*Up=*/ A0, /*Down=*/ A3, /*Home/Cancel=*/ 8); // Arduboy 10 (Production)
}

#define FLOW_CHART_DELAY 1500

u8g2_uint_t flow_chart_x_offset = 68;
u8g2_uint_t flow_chart_y_offset = 0;

void draw_text_box(u8g2_uint_t x, u8g2_uint_t y, const char *s, uint8_t bold)
{
  x += flow_chart_x_offset;
  y += flow_chart_y_offset;

  u8g2.setFont(u8g2_font_5x7_mr);
  u8g2.drawFrame(x, y, 58, 10);
  if ( bold )
    u8g2.drawFrame(x-1, y-1, 58+2, 10+2);
  
  u8g2.drawStr(x+2, y+7, s);
}

void draw_box_vline(u8g2_uint_t x, u8g2_uint_t y0, u8g2_uint_t y1)
{
  x += flow_chart_x_offset;
  x += 58/2;
  y0 += flow_chart_y_offset;
  y1 += flow_chart_y_offset;
  u8g2.drawVLine(x, y0, y1-y0+1);
}



void draw_flow_chart(uint8_t pos)
{
  flow_chart_y_offset = 0;
  if ( pos > 3 )
  {
    flow_chart_y_offset -= (pos-3)*20;
  }
  draw_text_box(0, 0, "init game", pos == 1);
  draw_box_vline(0, 10, 20);
  draw_text_box(0, 20, "show board", pos == 2);
  draw_box_vline(0, 30, 40);
  draw_text_box(0, 40, "player move", pos == 3);
  draw_box_vline(0, 50, 60);
  draw_text_box(0, 60, "show board", pos == 4);
  draw_box_vline(0, 70, 80);
  draw_text_box(0, 80, "check game", pos == 5);
  draw_box_vline(0, 90, 100);
  draw_text_box(0, 100, "ai move", pos == 6);
  draw_box_vline(0, 110, 120);
  draw_text_box(0, 120, "show board", pos == 7);
  draw_box_vline(0, 130, 140);
  draw_text_box(0, 140, "check game", pos == 8);
  draw_box_vline(0, 150, 160);
  draw_text_box(0, 160, "result", pos == 9);
}

void show_flow_chart(uint8_t pos)
{
    u8g2.firstPage();
    do 
    {
      yield();
      draw_flow_chart(pos);
    } while ( u8g2.nextPage() );
    delay(FLOW_CHART_DELAY);
}


/*==============================================*/

#define GRID_X_OFFSET 2
#define GRID_Y_OFFSET 2
#define CELL_WIDTH 17
#define CELL_HEIGHT 17
#define CELL_FRAME_WIDTH 3

uint8_t cursor_position; /* 0...8 */
u8g2_uint_t cell_x, cell_y;

/* calculate the cell position, return pixel position in global variables cell_x and cell_y */
void calc_cell_position(uint8_t x, uint8_t y )
{
  cell_x = x;
  cell_x*=CELL_WIDTH+CELL_FRAME_WIDTH;
  cell_x+= GRID_X_OFFSET;
  cell_y = y;
  cell_y *=CELL_HEIGHT+CELL_FRAME_WIDTH;
  cell_y +=GRID_Y_OFFSET;
}

void draw_grid(void)
{
  u8g2_uint_t x, y;
  x = GRID_X_OFFSET+CELL_WIDTH;  
  u8g2.drawVLine(x+1, GRID_Y_OFFSET, CELL_HEIGHT*3+CELL_FRAME_WIDTH*2);
  x += CELL_WIDTH+CELL_FRAME_WIDTH;
  u8g2.drawVLine(x+1, GRID_Y_OFFSET, CELL_HEIGHT*3+CELL_FRAME_WIDTH*2);

  y = GRID_Y_OFFSET+CELL_HEIGHT;  
  u8g2.drawHLine(GRID_X_OFFSET, y+1, CELL_WIDTH*3+CELL_FRAME_WIDTH*2);
  y += CELL_HEIGHT+CELL_FRAME_WIDTH;
  u8g2.drawHLine(GRID_X_OFFSET, y+1, CELL_WIDTH*3+CELL_FRAME_WIDTH*2);
}


void draw_cursor(void)
{
  u8g2_uint_t x, y;
  x = cursor_position % 3;
  y = cursor_position / 3;
  calc_cell_position(x, y );
  u8g2.drawFrame(cell_x, cell_y, CELL_WIDTH, CELL_HEIGHT);
  u8g2.drawFrame(cell_x+1, cell_y+1, CELL_WIDTH-2, CELL_HEIGHT-2);
}

/*==============================================*/

#define FELD_NICHT_BELEGT 0
#define FELD_MARKIERT_DURCH_SPIELER 1
#define FELD_MARKIERT_DURCH_COMPUTER 2

int spielfeld[3][3];


int gewinner = FELD_NICHT_BELEGT;

/*==============================================*/
/* vorbelegung game_initalisierung */
void game_init(void)
{
  int x, y;
  
  show_flow_chart(1);
  
  gewinner = FELD_NICHT_BELEGT;
  for( y = 0; y < 3; y = y + 1 )
  {
    for( x = 0; x < 3; x = x + 1 )
    {
      spielfeld[y][x] = FELD_NICHT_BELEGT;
    }
  }
}

/*==============================================*/
/* "game engine" */

void draw_board(void)
{
  int x, y;
  for( y = 0; y < 3; y = y + 1 )
  {
    for( x = 0; x < 3; x = x + 1 )
    {
      if ( spielfeld[y][x] == FELD_NICHT_BELEGT )
      {
	  /* nichts anzeigen */
      }
      if ( spielfeld[y][x] == FELD_MARKIERT_DURCH_SPIELER )
      {
	calc_cell_position(x, y);
	u8g2.drawLine(cell_x, cell_y, cell_x+CELL_WIDTH-1, cell_y+CELL_HEIGHT-1);
	u8g2.drawLine(cell_x+CELL_WIDTH-1, cell_y, cell_x, cell_y+CELL_HEIGHT-1);
      }
      if ( spielfeld[y][x] == FELD_MARKIERT_DURCH_COMPUTER )
      {
	calc_cell_position(x, y);
	u8g2.drawCircle(cell_x+CELL_WIDTH/2, cell_y+CELL_HEIGHT/2, CELL_HEIGHT/2);
      }
    }	/* for x */
  } /* for y */
}

void draw_grid_board(void)
{
  draw_grid();
  draw_board();
}

void show_board(uint8_t flow_chart_pos)
{
    u8g2.firstPage();
    do 
    {
      yield();
      draw_flow_chart(flow_chart_pos);
      draw_grid_board();
      draw_cursor();
    } while ( u8g2.nextPage() );
    delay(FLOW_CHART_DELAY);
}

/*==============================================*/
/* Benitzerschnittstelle (User Interface) */

void zug_spieler(void)
{
  static uint8_t keycode = 0;
  int x;
  int y;
  int zaehler = 0;
  
  for(;;)   /* check valid entry */
  {
    for(;;)	/* cursor movement loop */
    {
      keycode = 0;
      u8g2.firstPage();
      do 
      {
	draw_flow_chart(3);
	draw_grid_board();
	draw_cursor();
	if ( keycode == 0 )
	  keycode = u8g2.getMenuEvent();
      } while ( u8g2.nextPage() );

      while ( keycode == 0 )
      {
	yield();
	keycode = u8g2.getMenuEvent();
      }

      if ( keycode == U8X8_MSG_GPIO_MENU_DOWN )
      {
	cursor_position += 3;
	if ( cursor_position > 9 )
	  cursor_position -= 9;
      }

      if ( keycode == U8X8_MSG_GPIO_MENU_NEXT )
      {
	if ( cursor_position % 3 == 2 )
	  cursor_position-=2;
	else
	  cursor_position += 1;
      }

      if ( keycode == U8X8_MSG_GPIO_MENU_UP )
      {
	cursor_position -= 3;
	if ( cursor_position > 9 )
	  cursor_position += 9;
      }

      if ( keycode == U8X8_MSG_GPIO_MENU_PREV )
      {
	if ( cursor_position % 3 == 0 )
	  cursor_position+=2;
	else
	  cursor_position -= 1;
      }
      cursor_position = cursor_position % 9;

      if ( keycode == U8X8_MSG_GPIO_MENU_SELECT )
	break;
      if ( keycode == U8X8_MSG_GPIO_MENU_HOME )
	break;
    }

    y = cursor_position / 3;
    x = cursor_position % 3;
    
    if ( spielfeld[y][x] == FELD_NICHT_BELEGT )
    {
      spielfeld[y][x] = FELD_MARKIERT_DURCH_SPIELER ;
      return;
    }

    /*
    if ( zaehler > 3 )
    {
      printf("Du doedl, jetzt langts aber...\n");
      exit(1);
    }
    printf("Ahh, das feld ist schon belegt, nochmal...\n");
    zaehler = zaehler + 1;
    */
  }
}

/*==============================================*/
/* Künstliche Computer Intelligenz (AI NPC) */

void zug_computer(void)
{
  int x, y;
  
  /* AI: saudumme AI */
  
  for( y = 0; y < 3; y = y + 1 )
  {
    for( x = 0; x < 3; x = x + 1 )
    {
      if ( spielfeld[y][x] == FELD_NICHT_BELEGT )
      {
	spielfeld[y][x] = FELD_MARKIERT_DURCH_COMPUTER;
	return;
      }
    }
  }
}

/*==============================================*/
/* Spiel Zuende Prüfung */

/* gibt 1 zurück, wenn in der zeile überall markierung gesetzt ist */
int zeile_gewonnen(int y, int markierung)
{
  int x;	
  for( x = 0; x < 3; x++ )
  {
    if ( spielfeld[y][x] !=  markierung )
      return 0;
  }
  return 1;
}

/* gibt 1 zurück, wenn in der zeile überall markierung gesetzt ist */
int spalte_gewonnen(int x, int markierung)
{
  int y;	
  for( y = 0; y < 3; y++ )
  {
    if ( spielfeld[y][x] !=  markierung )
      return 0;
  }
  return 1;
}

int diagonale_gewonnen(int markierung)
{
  int i;
  
  for( i = 0; i < 3; i ++ )
  {
    if ( spielfeld[i][i] != markierung )
      return 0;
  }
  return 1;
}

int inverse_diagonale_gewonnen(int markierung)
{
  int i;
  
  for( i = 0; i < 3; i ++ )
  {
    if ( spielfeld[2-i][i] != markierung )
      return 0;
  }
  return 1;
}



int ist_das_spiel_zuende(uint8_t flow_chart_pos)
{
  int i, x, y;
  int ist_alles_voll;
  
  show_board(flow_chart_pos);

  for( i = 0; i < 3; i ++ )
  {
    if ( zeile_gewonnen(i, FELD_MARKIERT_DURCH_SPIELER) )
      gewinner = FELD_MARKIERT_DURCH_SPIELER;
    if ( zeile_gewonnen(i, FELD_MARKIERT_DURCH_COMPUTER) )
      gewinner = FELD_MARKIERT_DURCH_COMPUTER;
    if ( spalte_gewonnen(i, FELD_MARKIERT_DURCH_SPIELER) )
      gewinner = FELD_MARKIERT_DURCH_SPIELER;
   if ( spalte_gewonnen(i, FELD_MARKIERT_DURCH_COMPUTER) )
      gewinner = FELD_MARKIERT_DURCH_COMPUTER;
  }
  if ( diagonale_gewonnen(FELD_MARKIERT_DURCH_SPIELER) )
      gewinner = FELD_MARKIERT_DURCH_SPIELER;
  if ( diagonale_gewonnen(FELD_MARKIERT_DURCH_COMPUTER) )
      gewinner = FELD_MARKIERT_DURCH_COMPUTER;

  if ( inverse_diagonale_gewonnen(FELD_MARKIERT_DURCH_SPIELER) )
      gewinner = FELD_MARKIERT_DURCH_SPIELER;
  if ( inverse_diagonale_gewonnen(FELD_MARKIERT_DURCH_COMPUTER) )
      gewinner = FELD_MARKIERT_DURCH_COMPUTER;
  
  if ( gewinner != FELD_NICHT_BELEGT )
    return 1;

  ist_alles_voll = 1;
  for( y = 0; y < 3; y = y + 1 )
  {
    for( x = 0; x < 3; x = x + 1 )
    {
      if ( spielfeld[y][x] == FELD_NICHT_BELEGT )
      {
	ist_alles_voll = 0;
      }
    }
  }


  if ( ist_alles_voll )
    return 1;
    
  return 0;
}

/*==============================================*/

void zeige_ergebnis_an(void)
{
  u8g2.setFont(u8g2_font_helvB12_tr);
  if ( gewinner == FELD_MARKIERT_DURCH_SPIELER )
    u8g2.userInterfaceMessage("", "payer wins...", "", " ok ");
  else if ( gewinner == FELD_MARKIERT_DURCH_COMPUTER )
    u8g2.userInterfaceMessage("", "computer wins..", "", " ok ");
  else
    u8g2.userInterfaceMessage("", "game is a draw...", "", " ok ");
}

/*==============================================*/

/* HIER IST DIE HAUPTSCHLEIFE */

void game_play_loop(void)
{
  game_init();
  show_board(2);
  for(;;)
  {
    zug_spieler();
    show_board(4);
    if ( ist_das_spiel_zuende(5) )
    {
      break;
      
    } /* if */
    show_board(6);
    zug_computer();
    show_board(7);
    if ( ist_das_spiel_zuende(8) )
    {
      break;
    } /* if */
    
  } /* for */
  show_board(9);
  zeige_ergebnis_an();
  
} /* main */



void loop(void) 
{
  game_play_loop();
}

