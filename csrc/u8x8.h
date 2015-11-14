/*
  u8x8.h
  
  
  
  
  U8glib has several layers. Each layer is implemented with a callback function. 
  This callback function handels the messages for the layer.

  The topmost level is the display layer. It includes the following messages:
  
    U8X8_MSG_DISPLAY_SETUP			no communicaation with the display, setup memory ony
    U8X8_MSG_DISPLAY_INIT
    U8X8_MSG_DISPLAY_SET_FLIP_MODE
    U8X8_MSG_DISPLAY_SET_POWER_SAVE
    U8X8_MSG_DISPLAY_SET_CONTRAST
    U8X8_MSG_DISPLAY_DRAW_TILE

  A display driver may decided to breakdown these messages to a lower level interface or
  implement this functionality directly.
  

  One layer is the Command/Arg/Data interface. It can be used by the display layer
  to communicate with the display hardware.
  This layer only deals with data, commands and arguments. D/C line is unknown.
    U8X8_MSG_CAD_INIT
    U8X8_MSG_CAD_SET_I2C_ADR
    U8X8_MSG_CAD_SET_DEVICE
    U8X8_MSG_CAD_START_TRANSFER
    U8X8_MSG_CAD_SEND_CMD
    U8X8_MSG_CAD_SEND_ARG
    U8X8_MSG_CAD_SEND_DATA
    U8X8_MSG_CAD_END_TRANSFER
    
  The byte interface is there to send 1 byte (8 bits) to the display hardware.
  This layer depends on the hardware of a microcontroller, if a specific hardware 
  should be used (I2C or SPI). 
  If this interface is implemented via software, it may use the GPIO level for sending
  bytes.
    U8X8_MSG_BYTE_INIT
    U8X8_MSG_BYTE_SEND 30
    U8X8_MSG_BYTE_SET_DC 31
    U8X8_MSG_BYTE_START_TRANSFER
    U8X8_MSG_BYTE_END_TRANSFER
    U8X8_MSG_BYTE_SET_I2C_ADR
    U8X8_MSG_BYTE_SET_DEVICE

  GPIO and Delay
    U8X8_MSG_GPIO_INIT
    U8X8_MSG_DELAY_MILLI
    U8X8_MSG_DELAY_10MICRO
    U8X8_MSG_DELAY_100NANO
  
*/

#ifndef _U8X8_H
#define _U8X8_H

/*==========================================*/
/* Global Defines */

/* Undefine this to remove u8x8_display_SetContrast function */
#define U8X8_WITH_SET_CONTRAST

/* Undefine this to remove u8x8_display_SetFlipMode function */
#define U8X8_WITH_SET_FLIP_MODE

/* Select 0 or 1 for the default flip mode. This is not affected by U8X8_WITH_FLIP_MODE */
/* Note: Not all display types support a mirror functon for the frame buffer */
#define U8X8_DEFAULT_FLIP_MODE 0

/*==========================================*/
/* Includes */


#include <stdint.h>
#include <stddef.h>

#if defined(__GNUC__) && defined(__AVR__)
#include <avr/pgmspace.h>
#endif 

/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
extern "C" {
#endif


/*==========================================*/
/* U8G2 internal defines */


#ifdef __GNUC__
#  define U8X8_NOINLINE __attribute__((noinline))
#else
#  define U8X8_NOINLINE
#endif

#if defined(__GNUC__) && defined(__AVR__)
#  define U8X8_SECTION(name) __attribute__ ((section (name)))
#  define U8X8_FONT_SECTION(name) U8X8_SECTION(".progmem." name)
#  define u8x8_pgm_read(adr) pgm_read_byte_near(adr)
#endif

#ifndef U8X8_SECTION
#  define U8X8_SECTION(name) __attribute__ ((section (name)))
#  define U8X8_FONT_SECTION(name) 
#  define u8x8_pgm_read(adr) (*(const uint8_t *)(adr)) 
#endif

/*==========================================*/
/* U8X8 typedefs and data structures */


typedef struct u8x8_struct u8x8_t;
typedef struct u8x8_display_info_struct u8x8_display_info_t;
typedef struct u8x8_tile_struct u8x8_tile_t;

typedef uint8_t (*u8x8_msg_cb)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


//struct u8x8_mcd_struct
//{
//  u8x8_msg_cb cb;		/* current callback function */
//  u8x8_t *u8g2;		/* pointer to the u8g2 parent to minimize the number of args */
//  u8x8_mcd_t *next;
//};

struct u8x8_tile_struct
{
  uint8_t *tile_ptr;	/* pointer to one or more tiles */
  uint8_t cnt;		/* number of tiles */
  uint8_t x_pos;	/* tile x position */
  uint8_t y_pos;	/* tile x position */
};


struct u8x8_display_info_struct
{
  /* == general == */

  uint8_t chip_enable_level;			/* UC1601: 0 */
  uint8_t chip_disable_level;			/* opposite of chip_enable_level */
  
  uint8_t post_chip_enable_wait_ns;		/* UC1601: 5ns */
  uint8_t pre_chip_disable_wait_ns;		/* UC1601: 5ns */
  uint8_t reset_pulse_width_ms;		/* UC1601: 0.003ms --> 1ms */ 
  uint8_t post_reset_wait_ms;			/* UC1601: 6ms  */ 
  
  
  /* == SPI interface == */
  
  /* after SDA has been applied, wait this much time for the SCK data takeover edge */
  /* if this is smaller than sck_pulse_width_ns, then use the value from sck_pulse_width_ns */
  uint8_t sda_setup_time_ns;		/* UC1601: 12ns */
  /* the pulse width of the the clock signal, cycle time is twice this value */
  /* max freq is 1/(2*sck_pulse_width_ns) */
  uint8_t sck_pulse_width_ns;		/* UC1601: 15ns */
  
  /* data takeover edge:  0=falling edge, 1=rising edge*/
  /* initial default value for sck is low (0) for falling edge and high for rising edge */
  /* this means, default value is identical to sck_takeover_edge */
  uint8_t sck_takeover_edge;		/* UC1601: rising edge (1) */
  
  /* == I2C == */
  uint8_t i2c_bus_clock_100kHz;		/* UC1601: 1000000000/275 = 37 *100k */

  
  /* == 8 bit interface == */
  
  /* how long to wait after all data line are set */
  uint8_t data_setup_time_ns;		/* UC1601: 30ns */
  /* write enable pulse width */
  uint8_t write_pulse_width_ns;		/* UC1601: 40ns */
  
  /* == layout == */
  uint8_t tile_width;
  uint8_t tile_height;

  uint8_t default_x_offset;	/* default x offset for the display */
};

struct u8x8_struct
{
  const u8x8_display_info_t *display_info;
  u8x8_msg_cb display_cb;
  u8x8_msg_cb cad_cb;
  u8x8_msg_cb byte_cb;
  u8x8_msg_cb gpio_and_delay_cb;
  const uint8_t *font;
  uint8_t x_offset;	/* copied from info struct, can be modified in flip mode */
};



/*==========================================*/

/* helper functions */
void u8x8_d_helper_display_setup(u8x8_t *u8x8, const u8x8_display_info_t *display_info);
void u8x8_d_helper_display_init(u8x8_t *u8g2);

/* Display Interface */

/*
  Name: 	U8X8_MSG_DISPLAY_SETUP
  Args:	None
  Tasks:
    1) setup u8g2->display_info
      copy u8g2->display_info->default_x_offset to u8g2->x_offset
*/
#define U8X8_MSG_DISPLAY_SETUP 9

/*
  Name: 	U8X8_MSG_DISPLAY_INIT
  Args:	None
  Tasks:

    2) put interface into default state: 
	  execute u8x8_gpio_Init for port directions
	  execute u8x8_cad_Init for default port levels
    3) set CS status (not clear, may be done in cad/byte interface
    4) execute display reset (gpio interface)
    5) send setup sequence to display, do not activate display, disable "power save" will follow 
*/
#define U8X8_MSG_DISPLAY_INIT 10

/*
  Name: 	U8X8_MSG_DISPLAY_SET_POWER_SAVE
  Args:	arg_int: 0: normal mode (RAM is visible on the display), 1: nothing is shown
  Tasks:
    Depending on arg_int, put the display into normal or power save mode.
    Send the corresponding sequence to the display.
    In power save mode, it must be possible to modify the RAM content.
*/
#define U8X8_MSG_DISPLAY_SET_POWER_SAVE 11

/*
  Name: 	U8X8_MSG_DISPLAY_SET_FLIP_MODE
  Args:	arg_int: 0: normal mode, 1: flipped HW screen (180 degree)
  Tasks:
    Reprogramms the display controller to rotate the display by 
    180 degree (arg_int = 1) or not (arg_int = 0)
    This may change u8g2->x_offset if the display is smaller than the controller ram
    This message should only be supported if U8X8_WITH_FLIP_MODE is defined.
*/
#define U8X8_MSG_DISPLAY_SET_FLIP_MODE 13

/*  arg_int: 0..255 contrast value */
#define U8X8_MSG_DISPLAY_SET_CONTRAST 14

/*
  Name: 	U8X8_MSG_DISPLAY_DRAW_TILE
  Args:	
    arg_int: How often to repeat this tile pattern
    arg_ptr: pointer to u8x8_tile_t
        uint8_t *tile_ptr;	pointer to one or more tiles (number is "cnt")
	uint8_t cnt;		number of tiles
	uint8_t x_pos;		first tile x position
	uint8_t y_pos;		first tile y position 
  Tasks:
    One tile has exactly 8 bytes (8x8 pixel monochrome bitmap). 
    The lowest bit of the first byte is the upper left corner
    The highest bit of the first byte is the lower left corner
    The lowest bit of the last byte is the upper right corner
    The highest bit of the last byte is the lower left corner
    "tile_ptr" is the address of a memory area, which contains
    one or more tiles. "cnt" will contain the exact number of
    tiles in the memory areay. The size of the memory area is 8*cnt;
    Multiple tiles in the memory area form a horizontal sequence, this 
    means the first tile is drawn at x_pos/y_pos, the second tile is drawn
    at x_pos+1/y_pos, third at x_pos+2/y_pos.
    "arg_int" tells how often the tile sequence should be repeated:
    For example if "cnt" is two and tile_ptr points to tiles A and B,
    then for arg_int = 3, the following tile sequence will be drawn:
    ABABAB. Totally, cnt*arg_int tiles will be drawn. 
        
*/
#define U8X8_MSG_DISPLAY_DRAW_TILE 15

/*  arg_ptr: layout struct */
//#define U8X8_MSG_DISPLAY_GET_LAYOUT 16

/* u8x8_display.c */
uint8_t u8x8_display_DrawTile(u8x8_t *u8x8, uint8_t x, uint8_t y, uint8_t cnt, uint8_t *tile_ptr);

/* setup display memory structures */
void u8x8_display_Setup(u8x8_t *u8x8);
/* Init display, but keep display in power save mode. Usually this command must be followed by u8x8_display_SetPowerSave() */
void u8x8_display_Init(u8x8_t *u8x8);
/* wake up display from power save mode */
void u8x8_display_SetPowerSave(u8x8_t *u8x8, uint8_t is_enable);
void u8x8_display_SetFlipMode(u8x8_t *u8x8, uint8_t mode);
void u8x8_display_SetContrast(u8x8_t *u8x8, uint8_t value);
void u8x8_display_ClearScreen(u8x8_t *u8x8);



/*==========================================*/
/* Command Arg Data (CAD) Interface */

/*
  U8X8_MSG_CAD_INIT
    no args
    call U8X8_MSG_BYTE_INIT
    setup default values for the I/O lines
*/
#define U8X8_MSG_CAD_INIT 20


#define U8X8_MSG_CAD_SEND_CMD 21
/*  arg_int: cmd byte */
#define U8X8_MSG_CAD_SEND_ARG 22
/*  arg_int: arg byte */
#define U8X8_MSG_CAD_SEND_DATA 23
/* arg_int: expected cs level after processing this msg */
#define U8X8_MSG_CAD_START_TRANSFER 24
/* arg_int: expected cs level after processing this msg */
#define U8X8_MSG_CAD_END_TRANSFER 25
/* arg_int = 0: disable chip, arg_int = 1: enable chip */
#define U8X8_MSG_CAD_SET_I2C_ADR 26
#define U8X8_MSG_CAD_SET_DEVICE 27



/* u8g_cad.c */

#define u8x8_cad_Init(u8x8) ((u8x8)->cad_cb((u8x8), U8X8_MSG_CAD_INIT, 0, NULL ))

uint8_t u8x8_cad_SendCmd(u8x8_t *u8x8, uint8_t cmd) U8X8_NOINLINE;
uint8_t u8x8_cad_SendArg(u8x8_t *u8x8, uint8_t arg) U8X8_NOINLINE;
uint8_t u8x8_cad_SendData(u8x8_t *u8x8, uint8_t cnt, uint8_t *data) U8X8_NOINLINE;
uint8_t u8x8_cad_StartTransfer(u8x8_t *u8x8) U8X8_NOINLINE;
uint8_t u8x8_cad_EndTransfer(u8x8_t *u8x8) U8X8_NOINLINE;

/*
#define U8X8_C(c0)				(0x04), (c0)
#define U8X8_CA(c0,a0)			(0x05), (c0), (a0)
#define U8X8_CAA(c0,a0,a1)		(0x06), (c0), (a0), (a1)
#define U8X8_DATA()			(0x10)
#define U8X8_D1(d0)			(0x11), (d0)
*/

#define U8X8_C(c0)				(U8X8_MSG_CAD_SEND_CMD), (c0)
#define U8X8_CA(c0,a0)			(U8X8_MSG_CAD_SEND_CMD), (c0), (U8X8_MSG_CAD_SEND_ARG), (a0)
#define U8X8_CAA(c0,a0,a1)		(U8X8_MSG_CAD_SEND_CMD), (c0), (U8X8_MSG_CAD_SEND_ARG), (a0), (U8X8_MSG_CAD_SEND_ARG), (a1)
#define U8X8_D1(d0)			(U8X8_MSG_CAD_SEND_DATA), (d0)


#define U8X8_START_TRANSFER()	(U8X8_MSG_CAD_START_TRANSFER)
#define U8X8_END_TRANSFER()	(U8X8_MSG_CAD_END_TRANSFER)
#define U8X8_DLY(m)			(0xfe),(m)
#define U8X8_END()			(0xff)

void u8x8_cad_SendSequence(u8x8_t *u8x8, uint8_t const *data);
uint8_t u8x8_cad_110(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_cad_001(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*==========================================*/
/* Byte Interface */

#define U8X8_MSG_BYTE_INIT U8X8_MSG_CAD_INIT
#define U8X8_MSG_BYTE_SET_DC 32

#define U8X8_MSG_BYTE_SEND U8X8_MSG_CAD_SEND_DATA

#define U8X8_MSG_BYTE_START_TRANSFER U8X8_MSG_CAD_START_TRANSFER
#define U8X8_MSG_BYTE_END_TRANSFER U8X8_MSG_CAD_END_TRANSFER

#define U8X8_MSG_BYTE_SET_I2C_ADR U8X8_MSG_CAD_SET_I2C_ADR
#define U8X8_MSG_BYTE_SET_DEVICE U8X8_MSG_CAD_SET_DEVICE


uint8_t u8x8_byte_SetDC(u8x8_t *u8x8, uint8_t dc) U8X8_NOINLINE;
uint8_t u8x8_byte_SendByte(u8x8_t *u8x8, uint8_t byte) U8X8_NOINLINE;
uint8_t u8x8_byte_SendBytes(u8x8_t *u8x8, uint8_t cnt, uint8_t *data) U8X8_NOINLINE;

uint8_t u8x8_byte_8bit_sw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*==========================================*/
/* GPIO Interface */


/*
  U8X8_MSG_GPIO_AND_DELAY_INIT
  no args
  setup port directions, do not set IO levels, this is done with BYTE/CAD_INIT
*/
#define U8X8_MSG_GPIO_AND_DELAY_INIT 40

/* arg_int: milliseconds */
#define U8X8_MSG_DELAY_MILLI		41

#define U8X8_MSG_DELAY_10MICRO		42
#define U8X8_MSG_DELAY_100NANO		43
#define U8X8_MSG_DELAY_NANO		44

#define U8X8_MSG_GPIO_DC 45
#define U8X8_MSG_GPIO_CS 46		
#define U8X8_MSG_GPIO_RESET 47
#define U8X8_MSG_GPIO_CLOCK 48
#define U8X8_MSG_GPIO_DATA 49

#define u8x8_gpio_Init(u8x8) ((u8x8)->gpio_and_delay_cb((u8x8), U8X8_MSG_GPIO_AND_DELAY_INIT, 0, NULL ))

/*
#define u8x8_gpio_SetDC(u8x8, v) ((u8x8)->gpio_and_delay_cb((u8x8), U8X8_MSG_GPIO_DC, (v), NULL ))
#define u8x8_gpio_SetCS(u8x8, v) ((u8x8)->gpio_and_delay_cb((u8x8), U8X8_MSG_GPIO_CS, (v), NULL ))
#define u8x8_gpio_SetReset(u8x8, v) ((u8x8)->gpio_and_delay_cb((u8x8), U8X8_MSG_GPIO_RESET, (v), NULL ))
*/

#define u8x8_gpio_SetDC(u8x8, v) u8x8_gpio_call(u8x8, U8X8_MSG_GPIO_DC, (v))
#define u8x8_gpio_SetCS(u8x8, v) u8x8_gpio_call(u8x8, U8X8_MSG_GPIO_CS, (v))
#define u8x8_gpio_SetReset(u8x8, v) u8x8_gpio_call(u8x8, U8X8_MSG_GPIO_CS, (v))
#define u8x8_gpio_SetClock(u8x8, v) u8x8_gpio_call(u8x8, U8X8_MSG_GPIO_CLOCK, (v))
#define u8x8_gpio_SetData(u8x8, v) u8x8_gpio_call(u8x8, U8X8_MSG_GPIO_DATA, (v))

void u8x8_gpio_call(u8x8_t *u8x8, uint8_t msg, uint8_t arg) U8X8_NOINLINE;

#define u8x8_gpio_Delay(u8x8, msg, dly) u8x8_gpio_call((u8x8), (msg), (dly))
//void u8x8_gpio_Delay(u8x8_t *u8x8, uint8_t msg, uint8_t dly) U8X8_NOINLINE;


/*==========================================*/
/* u8x8_setup.c */
void u8x8_SetupDefaults(u8x8_t *u8x8);


/*==========================================*/
/* u8x8_d_stdio.c */
void u8x8_SetupStdio(u8x8_t *u8x8);

/*==========================================*/
/* u8x8_d_sdl_128x64.c */
void u8x8_Setup_SDL_128x64(u8x8_t *u8x8);
int u8g_sdl_get_key(void);

/*==========================================*/
/* u8x8_d_tga.c */
void u8x8_Setup_TGA_DESC(u8x8_t *u8x8);
void u8x8_Setup_TGA_LCD(u8x8_t *u8x8);
void tga_save(const char *name);

/*==========================================*/
/* u8x8_d_uc1701_dogs102.c */
uint8_t u8x8_d_uc1701_dogs102(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/*==========================================*/
/* u8x8_d_ssd1306_128x64_noname.c */
uint8_t u8x8_d_ssd1306_128x64_noname(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/*==========================================*/
/* u8x8_8x8.c */
void u8x8_Set8x8Font(u8x8_t *u8x8, const uint8_t *font_8x8);
void u8x8_Draw8x8Glyph(u8x8_t *u8x8, uint8_t x, uint8_t y, uint8_t encoding);
void u8x8_Draw8x8String(u8x8_t *u8x8, uint8_t x, uint8_t y, const char *s);

/*==========================================*/
/* start font list */
extern const uint8_t u8x8_font_5x7_f[] U8X8_FONT_SECTION("u8x8_font_5x7_f");
extern const uint8_t u8x8_font_5x7_r[] U8X8_FONT_SECTION("u8x8_font_5x7_r");
extern const uint8_t u8x8_font_5x7_n[] U8X8_FONT_SECTION("u8x8_font_5x7_n");
extern const uint8_t u8x8_font_5x8_f[] U8X8_FONT_SECTION("u8x8_font_5x8_f");
extern const uint8_t u8x8_font_5x8_r[] U8X8_FONT_SECTION("u8x8_font_5x8_r");
extern const uint8_t u8x8_font_5x8_n[] U8X8_FONT_SECTION("u8x8_font_5x8_n");
extern const uint8_t u8x8_font_amstrad_cpc_f[] U8X8_FONT_SECTION("u8x8_font_amstrad_cpc_f");
extern const uint8_t u8x8_font_amstrad_cpc_r[] U8X8_FONT_SECTION("u8x8_font_amstrad_cpc_r");
extern const uint8_t u8x8_font_amstrad_cpc_n[] U8X8_FONT_SECTION("u8x8_font_amstrad_cpc_n");

/* end font list */


#ifdef __cplusplus
}
#endif


#endif  /* _U8X8_H */

