/*

  AirQuality.ino
  
  ATMEGA328P (Arduino UNO/Pro Trinket) only

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
  
  http://shelvin.de/arduino-in-den-sleep_mode_pwr_down-schlaf-modus-setzen/  
  
  
  battery symbol
    trinket 3.3V requires at least 3.5V (https://learn.adafruit.com/introducing-pro-trinket/pinouts)
    Assuming 3x AAA and a fresh cell with 1.5V: --> 4.5V
    There are 6 battery symbols
    symbol 0: < 3.5V
    symbol 1: 3.5 - 3.7
    symbol 2: 3.7 - 3.9
    symbol 3: 3.9 - 4.1
    symbol 4: 4.1 - 4-5
    symbol 5: >= 4.5V    
    1V difference --> 5 steps --> 1V / 5 --> 0.2V step

    R1 = 470K
    R2 = 100K
    Rsum = 570K
    
    analogRead will use the internal 1.1V reference signal
    
    Um = 100K * 4.5V / 570K = 0.7894 V --> *1024/1.1 --> 735 (analogRead)
    Um = 100K * 3.5V / 570K = 0.6140 V --> 571 (analogRead)
    
    735 - 571 = 164 --> 33    
    
    analogRead*1.1/1024 = Um, Um = 100 * Ubat/570
    analogRead*1.1/1024 = 10 * Ubat/57
    analogRead*1.1*57/10240 = Ubat
    Ubat = analogRead*1.1*57/10240 = analogRead*0.006123 = analogRead/163
    
    
*/


#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_SGP30.h>
#include <SHTSensor.h>				// adafruit sht31 lib v1.0.0 has a wrong 500 milliseconds delay, so use the SHTSensor.h lib
#include <avr/sleep.h> 


//===================================================
// Constants: Timing values for the sensor & display state machine
// all values are "seconds"

// define startup calibration time: 2h
//#define STARTUP_TIME (60*60*2)
// five minutes
#define STARTUP_TIME (60*5)

// define duration after which the display is disabled: 30 seconds
#define DISPLAY_TIME ((30))

// Datasheet, page 8:
// For the first 15s after the “Init_air_quality” command  the  sensor  
// is  in  an  initialization  phase  during  which  a  “Measure_air_quality”  
// command  returns  fixed  values
// --> sensor warmup time
#define SENSOR_WARMUP_TIME ((16))

// SENSOR_MEASURE_TIME defines the duration how long the measurement
// shell be aktive (after SENSOR_WARMUP_TIME)
#define SENSOR_MEASURE_TIME (14)

// This is the time, after which the gas sensor should do another measurement.
// The sum of SENSOR_WARMUP_TIME and SENSOR_MEASURE_TIME must 
// be lesser than SENSOR_SAMPLE_TIME
// every 7 minutes, so there are at least two measures per history entry
#define SENSOR_SAMPLE_TIME (7*60)

// This is the time after which a new history entry is generated
// There are 96 entries in the history table (HIST_CNT) 
// 15*60 = 15 minutes --> 96 * 15 minutes --> 24h
#define NEW_HISTORY_DELAY (15*60)
//#define NEW_HISTORY_DELAY 2

// changing the content of the display is done via "shakes"
// shakes are detected by a tilt switsch.
// This value defines the number of shake events to change the display page.
#define NEW_DISPLAY_SHAKE_THRESHOLD 7

// number of seconds, for which a new display page is fixed
// this means, for this duration, the user can not change the display page
#define NEW_DISPLAY_COOL_DOWN 3


// Battery empty value
// U = 100K * 3.5V / 570K = 0.6140 V --> 571 (analogRead)
#define BATTERY_LOW 571
// step width for each battery symbol
#define BATTERY_STEP 33


U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

SHTSensor sht;							// temperature & humidity sensor

Adafruit_SGP30 sgp;						// air quality sensor

U8G2LOG u8g2log;						

// setup the terminal (U8G2LOG) and connect to u8g2 for automatic refresh of the display
// The size (width * height) depends on the selected font and the display

#define U8LOG_WIDTH 24
#define U8LOG_HEIGHT 5
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];

//===================================================
// Constants: Font definitions for U8g2

#define FONT_NARROW u8g2_font_mercutio_basic_nbp_tr
#define FONT_SMALL u8g2_font_helvB08_tf
#define FONT_MED_NUM u8g2_font_helvB14_tn
//#define FONT_BIG u8g2_font_inr49_mn
#define FONT_BIG u8g2_font_logisoso38_tn

//===================================================
// Constants: State values for the sensor & display coordination

#define STATE_RESET 0
#define STATE_STARTUP_DISP_ON 1
#define STATE_STARTUP_DISP_OFF 2
#define STATE_WARMUP_DISP_ON 11
#define STATE_WARMUP_DISP_OFF 12
#define STATE_MEASURE_DISP_ON 21
#define STATE_MEASURE_DISP_OFF 22
#define STATE_SENSOR_SLEEP_DISP_OFF 32


//===================================================
// Constants: Temperature boundaries

#define TEMP_LOW -20
#define TEMP_HIGH (TEMP_LOW+120)

//===================================================
// Constants: Number of different display pages
#define DISPLAY_PAGE_CNT 3

//===================================================
// Constants: History 

#define HIST_CNT 96

// history sample time: number of seconds between each history entry
// --> see NEW_HISTORY_DELAY
// 15 min = 15*60 seconds: 96 entries for 24h

//===================================================
// State variable for the sensor & display coordination

uint8_t state = STATE_RESET;		// assign STATE_xxx constants

uint8_t is_display_enabled = 0;		// modified by enable_display() and disable_display()


//===================================================
// Variables: Air quality sensor related varables

uint8_t is_ethanol_read = 0;

float temperature_raw;
float humidity_raw;
uint8_t temperature;	/* with offset and multiplied by 2 */
uint8_t humidity;		/* *2 */
uint16_t tvoc_raw;
uint16_t tvoc_lp;
uint16_t eco2_raw;
uint16_t eco2_lp;
uint16_t ethanol_raw = 0;
int is_air_quality_available = 0;


// Calibration values, values seem to be 0x8a27 and 0x08a98 for my sensor, so 0 will be used as not set
uint16_t eco2_base = 0;		// calibration value eCO2
uint16_t tvoc_base = 0;		// calibration value TVOC

//===================================================
uint16_t battery_raw;
uint16_t battery_glyph; 

//===================================================
// Variables: Timer for the state machine

volatile uint32_t wdt_count = 0;
volatile uint8_t wdt_sec = 0;
volatile uint8_t wdt_min = 0;
volatile uint8_t wdt_hour = 0;
volatile uint16_t wdt_day = 0;


volatile uint16_t startup_timer = 0;
volatile uint16_t sensor_sample_timer = SENSOR_SAMPLE_TIME;
volatile uint8_t is_sensor_sample_timer_alarm = 0;		// if zero: wake up gas sensor
volatile uint8_t display_timer = 0;

volatile uint8_t sensor_warmup_timer = 0;
volatile uint8_t sensor_measure_timer = 0;

volatile uint8_t new_display_cool_down_timer = 0;

volatile uint8_t is_wdt_irq = 0;

volatile uint8_t is_new_history_entry = 0;
volatile uint16_t new_history_timer = NEW_HISTORY_DELAY;

//volatile uint16_t battery_level = 0;	// in millivolt


uint32_t millis_sensor;	// Debugging: Duration of the sensour measurement
uint32_t millis_display;	// Debugging: Duration of the display refresh


//===================================================
// Variables: Shake detection

volatile uint8_t is_shake = 0;
volatile uint8_t shake_cnt = 0;
volatile uint8_t shake_last_cnt = 0;

//===================================================
// Variables: Current visible display page

uint8_t current_display_page = 0; // 0 .. DISPLAY_PAGE_CNT - 1

//===================================================
// Variables: History management

uint8_t hist_start = 0;
uint8_t hist_end = 1;
uint8_t hist_last = 0;

uint16_t hist_eco2_max[HIST_CNT];
uint16_t hist_eco2_min[HIST_CNT];

//uint8_t hist_temp_max[HIST_CNT];
//uint8_t hist_temp_min[HIST_CNT];

//uint8_t hist_rh_max[HIST_CNT];
//uint8_t hist_rh_min[HIST_CNT];

//===================================================

void hist_append(void)
{
  hist_last = hist_end;
  
  if ( hist_end == hist_start )	
  {	// history is full
    hist_start++;
    if ( hist_start >= HIST_CNT )
      hist_start = 0;
  }
  
    
  hist_end++;
  if ( hist_end >= HIST_CNT )
  {
    hist_end = 0;
  }
}


uint16_t maximum(uint16_t a, uint16_t b)
{
  if ( a < b )
    return b;
  return a;
}

uint16_t minimum(uint16_t a, uint16_t b)
{
  if ( a > b )
    return b;
  return a;
}


/* append current values to last history entry */
void add_hist_minmax(void)
{
  
  hist_eco2_max[hist_last] = maximum(hist_eco2_max[hist_last], eco2_raw);
  hist_eco2_min[hist_last] = minimum(hist_eco2_min[hist_last], eco2_raw);

  //hist_temp_max[hist_last] = maximum(hist_temp_max[hist_last], temperature);
  //hist_temp_min[hist_last] = minimum(hist_temp_min[hist_last], temperature);

  //hist_rh_max[hist_last] = maximum(hist_rh_max[hist_last], humidity);
  //hist_rh_min[hist_last] = minimum(hist_rh_min[hist_last], humidity);
}

void add_hist_new(void)
{
  hist_append();

  hist_eco2_max[hist_last] = eco2_raw;
  hist_eco2_min[hist_last] = eco2_raw;

  //hist_temp_max[hist_last] = temperature;
  //hist_temp_min[hist_last] = temperature;

  //hist_rh_max[hist_last] = humidity;
  //hist_rh_min[hist_last] = humidity;

}





//===================================================


ISR(WDT_vect) 
{
  is_wdt_irq = 1;
  wdt_count++; 
  
  
  wdt_sec++;
  if ( wdt_sec >= 60 )
  {
    wdt_sec = 0;
    wdt_min++;
    if ( wdt_min >= 60 )
    {
      wdt_min = 0;
      wdt_hour++;
      if ( wdt_hour >= 24 )
      {
	wdt_hour = 0;
	wdt_day++;
      }
    }
  }
  
  if ( startup_timer > 0 )
    startup_timer--;
  if ( display_timer > 0 )
    display_timer--;
  if ( sensor_warmup_timer > 0 )
    sensor_warmup_timer--;
  if ( sensor_measure_timer > 0 )
    sensor_measure_timer--;
  if ( sensor_sample_timer > 0 )
  {
    sensor_sample_timer--;
  }
  else
  {
    sensor_sample_timer = SENSOR_SAMPLE_TIME;
    if ( is_sensor_sample_timer_alarm == 0 )
      is_sensor_sample_timer_alarm = 1;
  }

  if ( new_history_timer > 0 )
  {
    new_history_timer--;
  }
  else
  {
    new_history_timer = NEW_HISTORY_DELAY;
    if ( is_new_history_entry == 0 )
      is_new_history_entry = 1;
  }


  shake_last_cnt = shake_cnt;
  shake_cnt = 0;
}

void enableWDT() 
{
  MCUSR = 0;			// clear all reset flags including the WDT flag
  WDTCSR = B00011000; // enable bit 4 (WDCE) and bit 3 (WDE) to change the prescalar
  WDTCSR = B01000110; // Enable watchdog IRQ  and set prescaler to 128k --> 1 sec
}

void reducePower(void)
{
  //ADCSRA = ADCSRA & B01111111; // ADC abschalten, ADEN bit7 zu 0
  ACSR = B10000000; // Analogen Comparator abschalten, ACD bit7 zu 1
  DIDR0 = DIDR0 | B00111111; // Digitale Eingangspuffer ausschalten, analoge Eingangs Pins 0-5 auf 1
}


// argument for attachInterrupt
// This is called if something happens on the tilt switch

void detectShake(void)
{
  is_shake = 1;			// used and cleared in is_display_on_event()
  if ( shake_cnt < 255 )
    shake_cnt++;		// used and cleared in ISR(WDT_vect) 
}

//===================================================


void setup(void) {
  uint8_t i;
  reducePower();
  analogReference(INTERNAL);		// use the internal 1.1V reference of the ATmega
  
  Wire.begin();

  u8g2.begin();  
  u8g2.enableUTF8Print();
  
  u8g2.setFont(FONT_NARROW);	// set the font for the terminal window
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(0);	// set extra space between lines in pixel, this can be negative
  u8g2log.setRedrawMode(0);		// 0: Update screen with newline, 1: Update screen for every char  

  
  u8g2log.print(F("Air Quality\n"));
  
  for( i = 0; i < HIST_CNT; i++ )
  {
    hist_eco2_max[i] = 0;
    hist_eco2_min[i] = 0;
  }
  
  
 if (sht.init()) {
    u8g2log.print(F("SHT31 found at 0x44\n"));
  } else {
    u8g2log.print(F("SHT31 not found\n"));
    while (1);
  }
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH); 


  if (sgp.begin()) {   				
    u8g2log.print(F("SGP30 found\n"));
  } else {
    u8g2log.print(F("SGP30 not found\n"));
    while (1);
  }
  
  u8g2log.print(F("SGP #"));
  u8g2log.print(sgp.serialnumber[0], HEX);
  u8g2log.print(sgp.serialnumber[1], HEX);
  u8g2log.println(sgp.serialnumber[2], HEX);
  

  delay(1000);
  
  // tilt detection at pin 2
  //pinMode(2, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(2), detectShake, CHANGE);
  
  // tilt detection at pin 3
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), detectShake, CHANGE);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 

  enableWDT();
}


//===================================================
// read ethanol value from SGP30 
// bypass Adafruit lib, because this command is not included


// not used at the moment
uint16_t readEthanol(void)
{
  uint8_t buf[6];
  uint8_t i;

  Wire.beginTransmission(0x58);
  Wire.write(0x20);     // 0x2050: measure signals command
  Wire.write(0x50);
  Wire.endTransmission();
  
  delay(200);           // max 200ms according to the datasheet
  
  if (Wire.requestFrom(0x58, 6) != 6) 
    return 0;
  for (i=0; i<6; i++)
    buf[i] = Wire.read();

  // each data value has: msb, lsb and crc
  // ethanol value is the second data returned
  // crc is ignored
  return (((uint16_t)buf[3])<<8) + (uint16_t)buf[4];
}


//===================================================

void readBatteryVoltageLevel(void)
{
  battery_raw = analogRead(0);
  battery_glyph = 0;
  if ( battery_raw >= BATTERY_LOW )
  {
    battery_glyph = (battery_raw - BATTERY_LOW)/BATTERY_STEP;
    if ( battery_glyph > 5 )
      battery_glyph = 5;
  }
  battery_glyph += 48;
}


//===================================================

/* 
  Calculate absolute humidity [mg/m^3]
  Args: Temperature [°C], humidity [%RH]
*/

uint32_t getAbsoluteHumidity(float temperature, float humidity) 
{
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}



void startAirQuality(void)
{
}

void readAirQuality(void)
{

  
  if ( is_ethanol_read == 0 )
  {
    // normal air quality read
    sht.readSample();
    temperature_raw = sht.getTemperature();
    humidity_raw = sht.getHumidity();
    
    if ( temperature_raw <= (float)TEMP_LOW )
    {
      temperature = 0;
    }
    else if ( temperature_raw >= (float)TEMP_HIGH )
    {
      temperature = TEMP_HIGH - TEMP_LOW;
      temperature *= 2;
    }
    else
    {
      temperature = (uint8_t)((temperature_raw-(float)TEMP_LOW)*2.0);
    }
    
    humidity = (uint8_t)((humidity_raw)*2.0);
  
    sgp.setHumidity(getAbsoluteHumidity(temperature_raw, humidity_raw));
    
    is_air_quality_available = sgp.IAQmeasure();
  
    if ( is_air_quality_available )
    {
      tvoc_raw = sgp.TVOC;
      eco2_raw = sgp.eCO2;
    }
    else
    {
      tvoc_raw = 0;
      eco2_raw = 400;
    }
    
    if ( is_new_history_entry != 0 )
    {
      add_hist_new();
      is_new_history_entry = 0;
    }
    else
    {
      add_hist_minmax();
    }

    // ethanol should be read only, if the display is active:
    /* not used
    if ( is_display_enabled )
    {
      is_ethanol_read = 1;
    }
    */
    
  }
  else
  {
    
    // special ethanol read
    //ethanol_raw = readEthanol();

    // according to the datasheet, the baseline values are corrupted... so restore them if available
    //if ( eco2_base != 0 )
    //  sgp.setIAQBaseline(eco2_base, tvoc_base);   // Restore the baseline values
    
    is_ethanol_read = 0;    // next: read normal air quality values
  }
}



uint16_t get_uint8(void *ptr, uint8_t pos)
{
  return ((uint8_t *)ptr)[pos];
}

uint16_t get_uint16(void *ptr, uint8_t pos)
{
  return ((uint16_t *)ptr)[pos];
}

void draw_temperature(uint16_t x)
{
  uint8_t frac;
  frac = x & 1;
  x >>= 1;
  x += TEMP_LOW;
  x = x & 0x0ff;
  u8g2.print(x);
  u8g2.print('.');
  if ( frac ) 
    u8g2.print('5');
  else
    u8g2.print('0');
}

void draw_humidity(uint16_t x)
{
  uint8_t frac;
  frac = x & 1;
  x >>= 1;
  u8g2.print(x);
  u8g2.print('.');
  if ( frac ) 
    u8g2.print('5');
  else
    u8g2.print('0');
}

void draw_16bit(uint16_t x)
{
  u8g2.print(x);
}

void draw_graph( uint16_t (*get_val)(void *ptr, uint8_t pos), void *min_array, void *max_array, void (*draw_value)(uint16_t x))
{
  uint8_t i, ii, x;
  uint16_t max = 0;
  uint16_t min = 0x0ffff;
  uint16_t delta;
  uint8_t ymin, ymax;
  i = hist_start;
  for(;;)
  {
    ii = i;
    i++;
    if ( i >= HIST_CNT )
      i = 0;
    if ( i == hist_end )
      break;
    if ( min > get_val(min_array, ii) )
      min = get_val(min_array, ii);
    if ( max < get_val(max_array, ii) )
      max = get_val(max_array, ii);
  }
  if ( min > max )
  {
    min = 0;
    max = 600;
    return;
  }
  if ( min + 30 >= max )
    max = min + 30;
    
  if ( max - min < 100 )
  {
    max += 19;
    max /= 20;
    max *= 20;
    
    min /= 20;
    min *= 20;
  }
  else
  {
    max += 199;
    max /= 200;
    max *= 200;
    
    min /= 200;
    min *= 200;
  }
    
  delta = max-min;

  i = hist_start;
  x = 127-HIST_CNT;

  u8g2.setFont(FONT_NARROW);
  u8g2.setDrawColor(1);

  for(;;)
  {
    ii = i;
    i++;
    if ( i >= HIST_CNT )
      i = 0;
    if ( i == hist_end )
      break;
    ymin = ((unsigned long)(get_val(min_array, ii) - min)*30UL)/delta;
    ymax = ((unsigned long)(get_val(max_array, ii) - min)*30UL)/delta;
    u8g2.drawVLine(x, 63-ymax, ymax-ymin+1);
    x++;
  }
  u8g2.setCursor(0, 46);
  draw_value(max);
  u8g2.setCursor(0, 63);
  draw_value(min);
}

//===================================================

void draw_1_2_eco2_history(u8g2_uint_t x, u8g2_uint_t y)
{
  
    draw_graph( get_uint16, hist_eco2_min, hist_eco2_max, draw_16bit);
}

void draw_1_4_temperature(u8g2_uint_t x, u8g2_uint_t y)
{
    u8g2.setFont(FONT_MED_NUM);
    
    u8g2.setCursor(x, 28+y);
    draw_temperature(temperature);

    u8g2.setFont(FONT_SMALL);
    
    u8g2.setCursor(x, 10+y);
    u8g2.print(F("°C"));
}

void draw_1_4_humidity(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_MED_NUM);
  u8g2.setCursor(x, 28+y);
  draw_humidity(humidity);
  
  u8g2.setFont(FONT_SMALL);
  u8g2.setCursor(x, 10+y);
  u8g2.print(F("%RH"));
}

void draw_gray_out(u8g2_uint_t x, u8g2_uint_t y)
{
  if ( state == STATE_WARMUP_DISP_ON )
  {
    uint8_t xx, yy;
    u8g2.setDrawColor(0);
    for( yy = y+14; yy < y+28; yy+=1 )
    {
      for( xx = 0; xx < 52; xx += 2 )
      {
	u8g2.drawPixel(xx+x+(yy&1), yy);
      }
    }
    u8g2.setDrawColor(1);
  }
}

void draw_1_4_eco2(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_SMALL);

  u8g2.setCursor(x, y+10);
  u8g2.print(F("ppm CO"));
  u8g2.setCursor(x+40, y+15);
  u8g2.print(F("²"));

  u8g2.setFont(FONT_MED_NUM);

  u8g2.setCursor(x, y+28);
  u8g2.print(eco2_raw);
  
  draw_gray_out(x, y);  
}

void draw_1_4_tvoc(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_SMALL);
  u8g2.setCursor(x, y+10);
  u8g2.print(F("ppb TVOC"));

  u8g2.setFont(FONT_MED_NUM);

  u8g2.setCursor(x, y+28);
  u8g2.print(tvoc_raw);

  draw_gray_out(x, y);  
}

void draw_1_4_base(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_SMALL);
  u8g2.setCursor(x, y+11);
  u8g2.print(F("Sec "));
  u8g2.print(wdt_count);

  u8g2.setCursor(x, y+21);
  u8g2.print(F("B-C  "));
  u8g2.print(eco2_base, HEX);

  u8g2.setCursor(x, y+31);
  u8g2.print(F("B-T "));
  u8g2.print(tvoc_base, HEX);

}

void draw_1_4_delay(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_SMALL);
  u8g2.setCursor(x, y+11);
  u8g2.print(F("Sens "));
  u8g2.print(millis_sensor);

  u8g2.setCursor(x, y+21);
  u8g2.print(F("Disp "));
  u8g2.print(millis_display);

}

void draw_1_4_uptime(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_SMALL);
  u8g2.setCursor(x, y+11);
  u8g2.print(F("Day "));
  u8g2.print(wdt_day);

  u8g2.setCursor(x, y+21);
  u8g2.print(F("Hour "));
  u8g2.print(wdt_hour);

  u8g2.setCursor(x, y+31);
  u8g2.print(F("Min "));
  u8g2.print(wdt_min);
}





void draw_1_4_system(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_SMALL);
  u8g2.setCursor(x, y+11);
  u8g2.print(F("ETH "));
  u8g2.print(ethanol_raw, HEX);

  u8g2.setCursor(x, y+21);
  u8g2.print(F("St "));
  u8g2.print(state);
  u8g2.print(F(" Sh "));
  u8g2.print(shake_last_cnt);

  u8g2.setCursor(x, y+31);
  u8g2.print(F("Bat "));
  u8g2.print(battery_raw);

}

void draw_1_4_battery(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(u8g2_font_battery19_tn);
  u8g2.drawGlyph(x, y+20, battery_glyph);  
}

void draw_1_4_emoticon(u8g2_uint_t x, u8g2_uint_t y)
{
  uint8_t tvoc_idx, eco2_idx, emo_idx;
  if ( tvoc_raw <= 75 )
    tvoc_idx = 1;
  else if ( tvoc_raw <= 150 )
    tvoc_idx = 2;
  else if ( tvoc_raw <= 300 )
    tvoc_idx = 3;
  else if ( tvoc_raw <= 500 )
    tvoc_idx = 4;
  else if ( tvoc_raw <= 1000 )
    tvoc_idx = 5;
  else if ( tvoc_raw <= 1500 )
    tvoc_idx = 6;
  else if ( tvoc_raw <= 3000 )
    tvoc_idx = 7;
  else if ( tvoc_raw <= 5000 )
    tvoc_idx = 8;
  else
    tvoc_idx = 9;

  if ( eco2_raw <= 500 )
    eco2_idx = 1;
  else if ( eco2_raw <= 600 )
    eco2_idx = 2;
  else if ( eco2_raw <= 800 )
    eco2_idx = 3;
  else if ( eco2_raw <= 1000 )
    eco2_idx = 4;
  else if ( eco2_raw <= 1200 )
    eco2_idx = 5;
  else if ( eco2_raw <= 1400 )
    eco2_idx = 6;
  else if ( eco2_raw <= 5000 )
    eco2_idx = 7;
  else if ( eco2_raw <= 10000 )
    eco2_idx = 8;
  else
    eco2_idx = 9;

  emo_idx = tvoc_idx;
  if ( emo_idx < eco2_idx )
    emo_idx = eco2_idx;
  u8g2.setFont(u8g2_font_emoticons21_tr);
  u8g2.drawGlyph(x+20, y+21, 32+emo_idx);

  
}

//===================================================

void draw_all_numbers(void)
{
  u8g2.setFontMode(1);
  u8g2.firstPage();
  do {
    u8g2.drawHLine(0, 32, 128);
    u8g2.drawVLine(62, 0, 64);
  
    
    draw_1_4_temperature(1, 0);
    draw_1_4_humidity(66, 0);

    if ( is_air_quality_available )
    {
      draw_1_4_eco2(1, 32+3);
      draw_1_4_tvoc(66, 32+3);
    }
      
  } while ( u8g2.nextPage() );
}


void draw_with_emo(void)
{
  u8g2.setFontMode(1);
  u8g2.firstPage();
  do {
    //u8g2.drawHLine(0, 32, 128);
    //u8g2.drawVLine(62, 0, 64);

    u8g2.drawHLine(0, 32, 128);
    u8g2.drawVLine(62-14, 0, 32);
    u8g2.drawVLine(98, 0, 32);
    u8g2.drawVLine(62-8, 32, 32);
    u8g2.drawVLine(128-12, 32, 32);

    
    draw_1_4_temperature(1, 0);
    draw_1_4_humidity(66-14, 0);
    draw_1_4_emoticon(127-20-21, 4);
    //draw_1_4_wdt_count(66, 0);

    if ( is_air_quality_available )
    {
      draw_1_4_eco2(1, 32+3);
      //draw_1_4_emoticon(66, 32+6);
      draw_1_4_tvoc(66-8, 32+3);      
    }
      
    draw_1_4_battery(128-8, 32+7);
  } while ( u8g2.nextPage() );
}

void draw_system(void)
{
  u8g2.setFontMode(1);
  u8g2.firstPage();
  do {
    u8g2.drawHLine(0, 32, 128);
    u8g2.drawVLine(62, 0, 64);
  
    
    draw_1_4_uptime(1, 0);
    //draw_1_4_delay(1,0);
    //draw_1_4_temperature(1, 0);
    draw_1_4_eco2(66, 0);
    //draw_1_4_humidity(66, 0);
    //draw_1_4_wdt_count(66, 0);

    draw_1_4_base(0, 32);
    draw_1_4_system(66, 32);
      
  } while ( u8g2.nextPage() );
}


void draw_with_history(void)
{
  u8g2.setFontMode(1);
  u8g2.firstPage();
  do {
    u8g2.drawHLine(0, 32, 128);
    u8g2.drawVLine(62-20, 0, 32);
    u8g2.drawVLine(98, 0, 32);
  
    
    draw_1_4_temperature(1, 0);
    if ( is_air_quality_available )
    {
      draw_1_4_eco2(66-20, 0);
      draw_1_4_emoticon(127-20-21, 4);
    }
    
    draw_1_2_eco2_history(0, 32);
    
    
      
  } while ( u8g2.nextPage() );
}

//===================================================


uint8_t is_display_on_event(void)
{
  if ( is_shake > 0 )
  {
    is_shake = 0;
    return 1;
  }
  if ( shake_last_cnt >= 1 )
    return 1;
  return 0;
}

void handle_new_display_page(void)
{
  if ( new_display_cool_down_timer > 0 )
  {
    new_display_cool_down_timer--;    
  }
  else
  {
    if ( shake_last_cnt > NEW_DISPLAY_SHAKE_THRESHOLD )
    {
      new_display_cool_down_timer = NEW_DISPLAY_COOL_DOWN;
      current_display_page++;
      if ( current_display_page >= DISPLAY_PAGE_CNT )
	current_display_page = 0;
    }
  }
}

void disable_display(void)
{
  u8g2.clear();
  u8g2.setPowerSave(1);
  is_display_enabled = 0;
}

void enable_display(void)
{
  is_display_enabled = 1;
  u8g2.setPowerSave(0);
}

void disable_sensors(void)
{
  /* both sensors will go to idle/sleep mode when the I2C soft reset is sent */
  Wire.beginTransmission(0);
  Wire.write(6);
  Wire.endTransmission(true);	// true: send full stop on I2C

  delay(5);
  
}

void enable_sensors(void)
{
  sht.init();
  sgp.IAQinit();
  delay(5);
}

//===================================================


void next_state(void)
{
  uint32_t start;
  switch(state)
  {
    case STATE_RESET:
      enable_display();
      startup_timer = STARTUP_TIME;
      state = STATE_STARTUP_DISP_ON;
      display_timer = DISPLAY_TIME;
      break;
	
    // - - -  Start Up - - -
      
    case STATE_STARTUP_DISP_ON:
      start = millis();
      sgp.getIAQBaseline(&eco2_base, &tvoc_base);	// always store the calibration values during measure
      readAirQuality();
      millis_sensor = millis() - start;
      
      if ( is_display_on_event() )
      {
	// display is already enabled, but the timer is reseted
	display_timer = DISPLAY_TIME;
      }
      
      if ( display_timer == 0 )
      {
	disable_display();
	state = STATE_STARTUP_DISP_OFF;
      }
      else if ( startup_timer == 0 )
      {
	sensor_measure_timer = SENSOR_MEASURE_TIME;	
	state = STATE_MEASURE_DISP_ON;
      }
      break;
      
    case STATE_STARTUP_DISP_OFF:
      start = millis();
      sgp.getIAQBaseline(&eco2_base, &tvoc_base);	// always store the calibration values during measure
      readAirQuality();
      millis_sensor = millis() - start;
      
      if ( is_display_on_event() )
      {
	enable_display();
	display_timer = DISPLAY_TIME;
	new_display_cool_down_timer = NEW_DISPLAY_COOL_DOWN;		// ensure, that the display page is visible for a while
	state = STATE_STARTUP_DISP_ON;
      }
      else if ( startup_timer == 0 )
      {
	sensor_measure_timer = SENSOR_MEASURE_TIME;	
	state = STATE_MEASURE_DISP_OFF;
      }
      break;
      
    // - - -  Sensor Warm Up - - -
      
    case STATE_WARMUP_DISP_ON:				// DONE
      if ( is_display_on_event() )
      {
	// display is already enabled, but the timer is reseted
	display_timer = DISPLAY_TIME;
      }

      if ( display_timer == 0 )
      {
	disable_display();
	state = STATE_WARMUP_DISP_OFF;
      }
      else if ( sensor_warmup_timer == 0 )
      {
	// set the baseline at the end of the warmup
	// this is described in the "SGP30 Driver Integration"
	if ( eco2_base != 0 )
	  sgp.setIAQBaseline(eco2_base, tvoc_base);		// Restore the baseline values
	  
	sensor_measure_timer = SENSOR_MEASURE_TIME;
	state = STATE_MEASURE_DISP_ON;
      }
      break;
      
    case STATE_WARMUP_DISP_OFF:		// DONE
      if ( is_display_on_event() )
      {
	enable_display();
	display_timer = DISPLAY_TIME;
	new_display_cool_down_timer = NEW_DISPLAY_COOL_DOWN;		// ensure, that the display page is visible for a while
	state = STATE_WARMUP_DISP_ON;
      }
      else if ( sensor_warmup_timer == 0 )
      {
	// set the baseline at the end of the warmup
	// this is described in the "SGP30 Driver Integration"
	if ( eco2_base != 0 )
	  sgp.setIAQBaseline(eco2_base, tvoc_base);		// Restore the baseline values
	        
	sensor_measure_timer = SENSOR_MEASURE_TIME;
	state = STATE_MEASURE_DISP_OFF;
      }
      break;

    // - - -  Sensor Measure - - -

    case STATE_MEASURE_DISP_ON:				// DONE
      start = millis();
      sgp.getIAQBaseline(&eco2_base, &tvoc_base);	// always store the calibration values during measure
      readAirQuality();
      millis_sensor = millis() - start;
      
      if ( is_display_on_event() )
      {
	// display is already enabled, but the timer is reseted
	display_timer = DISPLAY_TIME;
      }
	
      if ( display_timer == 0 )
      {
	disable_display();
	state = STATE_MEASURE_DISP_OFF;
      }
      else if ( sensor_measure_timer == 0 )
      {
	// ignored: Measurement is continued until the display goes off.
      }
      break;
      
    case STATE_MEASURE_DISP_OFF:		// DONE
      start = millis();
      sgp.getIAQBaseline(&eco2_base, &tvoc_base);	// always store the calibration values during measure
      readAirQuality();
      millis_sensor = millis() - start;
      
      if ( is_display_on_event() )
      {
	enable_display();
	display_timer = DISPLAY_TIME;
	new_display_cool_down_timer = NEW_DISPLAY_COOL_DOWN;		// ensure, that the display page is visible for a while
	state = STATE_MEASURE_DISP_ON;
      }
      else if ( sensor_measure_timer == 0 )
      {
	disable_sensors();
	state = STATE_SENSOR_SLEEP_DISP_OFF;
      }
      break;

    // - - -  Sensor Sleep - - -

    case STATE_SENSOR_SLEEP_DISP_OFF:			// DONE
      if ( is_display_on_event() )
      {
	enable_display();
	display_timer = DISPLAY_TIME;
	new_display_cool_down_timer = NEW_DISPLAY_COOL_DOWN;		// ensure, that the display page is visible for a while
	
	enable_sensors();
	
	sensor_warmup_timer = SENSOR_WARMUP_TIME;
	state = STATE_WARMUP_DISP_ON;
      }
      else if ( is_sensor_sample_timer_alarm != 0 )
      {
	is_sensor_sample_timer_alarm = 0;
	
	enable_sensors();
	  
	sensor_warmup_timer = SENSOR_WARMUP_TIME;
	state = STATE_WARMUP_DISP_OFF;
      }
      break;
      
    default:
      state = STATE_RESET;
      break;
  }
}



//===================================================



void loop(void) {
  uint8_t i;
  u8g2_uint_t g;
  uint32_t start;

  if ( is_wdt_irq )
  {
    is_wdt_irq = 0;

    next_state();
    readBatteryVoltageLevel();
    handle_new_display_page();

    if ( is_display_enabled )		// "is_display_enabled" will be calculated in next_state()
    {
      start = millis();
      if ( current_display_page == 0 )
	draw_with_emo();
      else if ( current_display_page == 1 )
	draw_with_history();
      else if ( current_display_page == 2 )
	draw_system();      
      millis_display = millis() - start;
    }
    else
    {
      millis_display = 0;
    }
  }

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  //noInterrupts();
  sleep_enable(); 
  //interrupts();  
  sleep_mode(); 
  sleep_disable(); 
}
