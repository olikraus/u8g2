/*

  AirQuality.ino
  
  ATMEGA328 (Arduino UNO) only

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
  
*/

// The Adafruit_SHT31 lib v1.0.0 should be disabled, because measurement will take 500ms, which is too much
//#define USE_ADAFRUIT_SHT31_LIB

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_SGP30.h>
#ifdef USE_ADAFRUIT_SHT31_LIB
#include <Adafruit_SHT31.h>			// adafruit sht31 lib v1.0.0 has a 500 milliseconds delay, 15 are sufficient
#else // USE_ADAFRUIT_SHT31_LIB
#include <SHTSensor.h>				
#endif // USE_ADAFRUIT_SHT31_LIB
#include <avr/sleep.h> 



U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#ifdef USE_ADAFRUIT_SHT31_LIB
Adafruit_SHT31 sht31 = Adafruit_SHT31();	// temperature & humidity sensor
#else // USE_ADAFRUIT_SHT31_LIB
SHTSensor sht;
#endif // USE_ADAFRUIT_SHT31_LIB

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
// Constants: Timing values for the sensor & display state machine

// define startup calibration time: 2h
//#define STARTUP_TIME (60*60*2)
#define STARTUP_TIME (60)

// define startup calibration time: 30 seconds
#define DISPLAY_TIME ((30))

// Datasheet, page 8:
// For the first 15s after the “Init_air_quality” command  the  sensor  
// is  in  an  initialization  phase  during  which  a  “Measure_air_quality”  
// command  returns  fixed  values
// --> sensor warmup time
#define SENSOR_WARMUP_TIME ((16))

#define SENSOR_MEASURE_TIME (14)

// the sum of SENSOR_WARMUP_TIME and SENSOR_MEASURE_TIME must 
// be lesser than SENSOR_SAMPLE_TIME
#define SENSOR_SAMPLE_TIME (2*60)


// number of seconds, for which a new display is fixed
#define NEW_DISPLAY_COOL_DOWN 4

// number of shakes required to change the display
#define NEW_DISPLAY_SHAKE_THRESHOLD 5

//===================================================
// Constants: Temperature boundaries

#define TEMP_LOW -20
#define TEMP_HIGH (TEMP_LOW+120)

//===================================================
// Constants: Number of different display pages
#define DISPLAY_CNT 3

//===================================================
// Constants: History 

#define HIST_CNT 96

// history sample time: number of seconds between each history entry
// 15 min = 15*60 seconds: 96 entries for 24h
#define HISTORY_SAMPLE_TIME (15*60)

//===================================================
// State variable for the sensor & display coordination

uint8_t state = STATE_RESET;		// assign STATE_xxx constants

//===================================================
// Variables: Air quality sensor related varables

float temperature_raw;
float humidity_raw;
uint8_t temperature;	/* with offset and multiplied by 2 */
uint8_t humidity;		/* *2 */
uint16_t tvoc_raw;
uint16_t tvoc_lp;
uint16_t eco2_raw;
uint16_t eco2_lp;
int is_air_quality_available = 0;

uint16_t eco2_base;		// calibration value eCO2
uint16_t tvoc_base;		// calibration value TVOC

//===================================================
// Variables: Timer for the state machine

volatile uint32_t wdt_count = 0;

volatile uint16_t startup_timer = 0;
volatile uint16_t sensor_sample_timer = SENSOR_SAMPLE_TIME;
volatile uint8_t is_sensor_sample_timer_alarm = 0;
volatile uint8_t display_timer = 0;

volatile uint8_t sensor_warmup_timer = 0;
volatile uint8_t sensor_measure_timer = 0;

volatile uint8_t new_display_cool_down_timer = 0;

volatile uint8_t is_wdt_irq = 0;

uint32_t millis_sensor;
uint32_t millis_display;

//===================================================
// Variables: Shake detection

volatile uint8_t is_shake = 0;
volatile uint8_t shake_cnt = 0;
volatile uint8_t shake_last_cnt = 0;

//===================================================
// Variables: Current visible display

uint8_t current_display = 0; // 0 .. DISPLAY_CNT - 1

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
  ADCSRA = ADCSRA & B01111111; // ADC abschalten, ADEN bit7 zu 0
  ACSR = B10000000; // Analogen Comparator abschalten, ACD bit7 zu 1
  //DIDR0 = DIDR0 | B00111111; // Digitale Eingangspuffer ausschalten, analoge Eingangs Pins 0-5 auf 1
}


void detectShake(void)
{
  is_shake = 1;
  if ( shake_cnt < 255 )
    shake_cnt++;
}

//===================================================


void setup(void) {
  reducePower();
  Wire.begin();

  u8g2.begin();  
  u8g2.enableUTF8Print();
  
  u8g2.setFont(FONT_NARROW);	// set the font for the terminal window
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(0);	// set extra space between lines in pixel, this can be negative
  u8g2log.setRedrawMode(0);		// 0: Update screen with newline, 1: Update screen for every char  

  
  u8g2log.print(F("Air Quality\n"));
  
#ifdef USE_ADAFRUIT_SHT31_LIB
  if (sht31.begin(0x44)) {   				// 0x45 for alternate i2c addr
    u8g2log.print(F("SHT31 found at 0x44\n"));
  } else {
    u8g2log.print(F("SHT31 not found\n"));
    while (1);
  }
#else // USE_ADAFRUIT_SHT31_LIB
  if (sht.init()) {
    u8g2log.print(F("SHT31 found at 0x44\n"));
  } else {
    u8g2log.print(F("SHT31 not found\n"));
    while (1);
  }
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH); 
#endif // USE_ADAFRUIT_SHT31_LIB

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
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), detectShake, CHANGE);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 

  enableWDT();
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

uint8_t new_counter = 0; 

void readAirQuality(void)
{
  uint8_t i;
  int32_t tvoc_coeff = 7;	// 1..32
  int32_t eco2_coeff = 7;	// 1..32


#ifdef USE_ADAFRUIT_SHT31_LIB
  temperature_raw = sht31.readTemperature();
  humidity_raw = sht31.readHumidity();
#else // USE_ADAFRUIT_SHT31_LIB
  sht.readSample();
  temperature_raw = sht.getTemperature();
  humidity_raw = sht.getHumidity();
#endif // USE_ADAFRUIT_SHT31_LIB

  
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
  
  if ( (new_counter & 3) == 0 )
    add_hist_new();
  else
    add_hist_minmax();
  new_counter++;
  
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
    return;
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

void draw_page1(void)
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.setCursor(0, 10);
    //u8g2.print(temperature_raw);
    draw_temperature(temperature);
    //u8g2.setCursor(33, 10);
    u8g2.print(F(" °C"));
    u8g2.setCursor(64, 10);
    draw_humidity(humidity);
    //u8g2.setCursor(98, 10);
    u8g2.print(F(" %RH"));

    //u8g2.setCursor(0, 20);
    //u8g2.print(eco2_base, HEX);
    //u8g2.print(" ");
    //u8g2.print(tvoc_base, HEX);


    if ( is_air_quality_available )
    {
      u8g2.setCursor(0, 30);
      u8g2.print(tvoc_raw);
      u8g2.print(" / ");
      u8g2.print(eco2_raw);
    }
      
    draw_graph( get_uint16, hist_eco2_min, hist_eco2_max, draw_16bit);
      
  } while ( u8g2.nextPage() );
}

//===================================================

void draw_eco2(void)
{
  u8g2.setFontMode(1);
  u8g2.firstPage();
  do {
    u8g2.setFont(FONT_SMALL);
    u8g2.setCursor(0, 10);
    //u8g2.print(temperature_raw);
    draw_temperature(temperature);
    //u8g2.setCursor(33, 10);
    u8g2.print(F(" °C"));
    u8g2.setCursor(64, 10);
    draw_humidity(humidity);
    //u8g2.setCursor(98, 10);
    u8g2.print(F(" %RH"));

    //u8g2.setCursor(0, 20);
    //u8g2.print(eco2_base, HEX);
    //u8g2.print(" ");
    //u8g2.print(tvoc_base, HEX);

    if ( is_air_quality_available )
    {
      u8g2.setFont(FONT_BIG);
      u8g2.setCursor(20, 63);
      u8g2.print(eco2_raw);
      
      u8g2.setFont(FONT_SMALL);
      u8g2.setCursor(0, 25);
      u8g2.print(F("CO"));
      u8g2.setCursor(17, 30);
      u8g2.print(F("²"));
      
      //u8g2.print(tvoc_raw);
    }
      
  } while ( u8g2.nextPage() );
}

//===================================================

void draw_1_2_eco2_history(u8g2_uint_t x, u8g2_uint_t y)
{
    u8g2.setFont(FONT_NARROW);
  
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
}

void draw_1_4_tvoc(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_SMALL);
  u8g2.setCursor(x, y+10);
  u8g2.print(F("ppb TVOC"));

  u8g2.setFont(FONT_MED_NUM);

  u8g2.setCursor(x, y+28);
  u8g2.print(tvoc_raw);
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


void draw_1_4_system(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(FONT_SMALL);
  u8g2.setCursor(x, y+11);
  u8g2.print(F("Shake "));
  u8g2.print(shake_last_cnt);

  u8g2.setCursor(x, y+21);
  u8g2.print(F("Cool "));
  u8g2.print(new_display_cool_down_timer);

  u8g2.setCursor(x, y+31);
  u8g2.print(F("State "));
  u8g2.print(state);

}

void draw_1_4_battery(u8g2_uint_t x, u8g2_uint_t y)
{
  u8g2.setFont(u8g2_font_battery19_tn);
  u8g2.drawGlyph(x, y+20, 50);
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
  
    
    draw_1_4_delay(1,0);
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
  if ( shake_last_cnt >= 1 )
    return 1;
  return 0;
}

void handle_new_display(void)
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
      current_display++;
      if ( current_display >= DISPLAY_CNT )
	current_display = 0;
    }
  }
}

//===================================================


void next_state(void)
{
  switch(state)
  {
    case STATE_RESET:
      startup_timer = STARTUP_TIME;
      state = STATE_STARTUP_DISP_ON;
      display_timer = DISPLAY_TIME;
      break;
	
    // - - -  Start Up - - -
      
    case STATE_STARTUP_DISP_ON:
      if ( is_display_on_event() )
	display_timer = DISPLAY_TIME;
	
      if ( display_timer == 0 )
      {
	state = STATE_STARTUP_DISP_OFF;
      }
      else if ( startup_timer == 0 )
      {
	sensor_measure_timer = SENSOR_MEASURE_TIME;	
	state = STATE_MEASURE_DISP_ON;
      }
      break;
    case STATE_STARTUP_DISP_OFF:
      if ( is_display_on_event() )
      {
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
	display_timer = DISPLAY_TIME;

      if ( display_timer == 0 )
      {
	state = STATE_WARMUP_DISP_OFF;
      }
      else if ( sensor_warmup_timer == 0 )
      {
	sensor_measure_timer = SENSOR_MEASURE_TIME;
	state = STATE_MEASURE_DISP_ON;
      }
      break;
      
    case STATE_WARMUP_DISP_OFF:		// DONE
      if ( is_display_on_event() )
      {
	display_timer = DISPLAY_TIME;
	state = STATE_WARMUP_DISP_ON;
      }
      else if ( sensor_warmup_timer == 0 )
      {
	sensor_measure_timer = SENSOR_MEASURE_TIME;
	state = STATE_MEASURE_DISP_OFF;
      }
      break;

    // - - -  Sensor Measure - - -

    case STATE_MEASURE_DISP_ON:				// DONE
      if ( is_display_on_event() )
	display_timer = DISPLAY_TIME;
	
      if ( display_timer == 0 )
      {
	state = STATE_MEASURE_DISP_OFF;
      }
      else if ( sensor_measure_timer == 0 )
      {
	// ignored: Measurement is continued until the display goes off.
      }
      break;
      
    case STATE_MEASURE_DISP_OFF:		// DONE
      if ( is_display_on_event() )
      {
	display_timer = DISPLAY_TIME;
	state = STATE_MEASURE_DISP_ON;
      }
      else if ( sensor_measure_timer == 0 )
      {
	state = STATE_SENSOR_SLEEP_DISP_OFF;
      }
      break;

    // - - -  Sensor Sleep - - -

    case STATE_SENSOR_SLEEP_DISP_OFF:			// DONE
      if ( is_display_on_event() )
      {
	sensor_warmup_timer = SENSOR_WARMUP_TIME;
	display_timer = DISPLAY_TIME;
	state = STATE_WARMUP_DISP_ON;
      }
      else if ( is_sensor_sample_timer_alarm != 0 )
      {
	is_sensor_sample_timer_alarm = 0;
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
    handle_new_display();

    start = millis();
    readAirQuality();
    sgp.getIAQBaseline(&eco2_base, &tvoc_base);
    millis_sensor = millis() - start;

    
    start = millis();
    if ( current_display == 0 )
      draw_with_emo();
    else if ( current_display == 1 )
      draw_with_history();
    else if ( current_display == 2 )
      draw_system();
      
    millis_display = millis() - start;
      
  }

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  //noInterrupts();
  sleep_enable(); 
  //interrupts();  
  sleep_mode(); 
  sleep_disable(); 
}
