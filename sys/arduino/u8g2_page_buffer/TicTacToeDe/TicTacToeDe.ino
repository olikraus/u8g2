/*

  TicTacToeDe.ino
  
*/

#include <Arduino.h>
#include <U8g2lib.h>


/*==============================================*/
/* Konstante Werte */

#define GITTER_X_OFFSET 35
#define GITTER_Y_OFFSET 2
#define ZELLE_BREITE 17
#define ZELLE_HOEHE 17
#define ZELLE_RAHMENBREITE 3

#define FELD_NICHT_BELEGT 0
#define FELD_MARKIERT_DURCH_SPIELER 1
#define FELD_MARKIERT_DURCH_COMPUTER 2

/*==============================================*/
/* Variablen */

/* U8g2 Graphics API, setup for ARDUBOY Production, Kickstarter Edition */
U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6);	

uint8_t cursor_position; 	/* Aktuelle Cursorposition die vom User ausgewählt wurde */
u8g2_uint_t zelle_x, zelle_y;	/* Berechnete Position einer Zelle (berechne_zell_position) */

int spielfeld[3][3];		/* Enthält die aktuelle Belegung mit x und o */
int gewinner;			/* Zeigt den gewinner an (wenn es einen gibt) */

/*==============================================*/
/* vorbelegung und spiel initalisierenalisierung */
void spiel_initalisieren(void)
{
  int x, y;
    
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
/* Hilfsroutine */

/* berechne die Zell-Position, Ergebnis in zelle_x und zelle_y */
void berechne_zell_position(uint8_t x, uint8_t y )
{
  zelle_x = x;
  zelle_x*=ZELLE_BREITE+ZELLE_RAHMENBREITE;
  zelle_x+= GITTER_X_OFFSET;
  zelle_y = y;
  zelle_y *=ZELLE_HOEHE+ZELLE_RAHMENBREITE;
  zelle_y +=GITTER_Y_OFFSET;
}

/*==============================================*/
/* "game engine" */

void zeichne_gitter(void)
{
  u8g2_uint_t x, y;
  x = GITTER_X_OFFSET+ZELLE_BREITE;  
  u8g2.drawVLine(x+1, GITTER_Y_OFFSET, ZELLE_HOEHE*3+ZELLE_RAHMENBREITE*2);
  x += ZELLE_BREITE+ZELLE_RAHMENBREITE;
  u8g2.drawVLine(x+1, GITTER_Y_OFFSET, ZELLE_HOEHE*3+ZELLE_RAHMENBREITE*2);

  y = GITTER_Y_OFFSET+ZELLE_HOEHE;  
  u8g2.drawHLine(GITTER_X_OFFSET, y+1, ZELLE_BREITE*3+ZELLE_RAHMENBREITE*2);
  y += ZELLE_HOEHE+ZELLE_RAHMENBREITE;
  u8g2.drawHLine(GITTER_X_OFFSET, y+1, ZELLE_BREITE*3+ZELLE_RAHMENBREITE*2);
}

void zeichne_spielfeld(void)
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
	berechne_zell_position(x, y);
	u8g2.drawLine(zelle_x, zelle_y, zelle_x+ZELLE_BREITE-1, zelle_y+ZELLE_HOEHE-1);
	u8g2.drawLine(zelle_x+ZELLE_BREITE-1, zelle_y, zelle_x, zelle_y+ZELLE_HOEHE-1);
      }
      if ( spielfeld[y][x] == FELD_MARKIERT_DURCH_COMPUTER )
      {
	berechne_zell_position(x, y);
	u8g2.drawCircle(zelle_x+ZELLE_BREITE/2, zelle_y+ZELLE_HOEHE/2, ZELLE_HOEHE/2);
      }
    }	/* for x */
  } /* for y */
}

void zeichne_gitter_und_spielfeld(void)
{
  zeichne_gitter();
  zeichne_spielfeld();
}

void zeige_spielfeld_an(void)
{
    u8g2.firstPage();
    do 
    {
      yield();	// Arduino
      zeichne_gitter_und_spielfeld();
    } while ( u8g2.nextPage() );
}

/*==============================================*/
/* Benitzerschnittstelle (User Interface) */

/* Zeichne den Cursor für die Benutzereingabe */
void zeichne_cursor(void)
{
  u8g2_uint_t x, y;
  x = cursor_position % 3;
  y = cursor_position / 3;
  berechne_zell_position(x, y );
  u8g2.drawFrame(zelle_x, zelle_y, ZELLE_BREITE, ZELLE_HOEHE);
  u8g2.drawFrame(zelle_x+1, zelle_y+1, ZELLE_BREITE-2, ZELLE_HOEHE-2);
}

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
	zeichne_gitter_und_spielfeld();
	zeichne_cursor();
	if ( keycode == 0 )
	  keycode = u8g2.getMenuEvent();
      } while ( u8g2.nextPage() );

      while ( keycode == 0 )
      {
	yield();	// Arduino
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
/* Künstliche Computer Intelligenz */

void zug_computer(void)
{
  int x, y;
  
  /* Sehr dumme AI: Nimm den nächsten freien Platz */
  
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

int ist_das_spiel_zuende(void)
{
  int i, x, y;
  int ist_alles_voll;
  
  zeige_spielfeld_an();

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
    u8g2.userInterfaceMessage("Spieler", "gewinnt", "", " ok ");
  else if ( gewinner == FELD_MARKIERT_DURCH_COMPUTER )
    u8g2.userInterfaceMessage("Computer", "gewinnt", "", " ok ");
  else
    u8g2.userInterfaceMessage("", "Unentschieden", "", " ok ");
}

/*==============================================*/

void setup(void) 	// Arduino setup
{
  // Intialisierung für Arduboy (Production)
  u8g2.begin(/*Select=*/ 7, /*Right/Next=*/ A1, /*Left/Prev=*/ A2, /*Up=*/ A0, /*Down=*/ A3, /*Home/Cancel=*/ 8); 
}

void loop(void) 		// Arduino loop 
{
  spiel_initalisieren();
  zeige_spielfeld_an();
  for(;;)
  {
    zug_spieler();
    zeige_spielfeld_an();
    if ( ist_das_spiel_zuende() )
    {
      break;
    } /* if */
    delay(300);
    zug_computer();
    zeige_spielfeld_an();
    if ( ist_das_spiel_zuende() )
    {
      break;
    } /* if */
    
  } /* for */
  zeige_ergebnis_an();
}

