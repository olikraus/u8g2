/*
 * lpc8xx_swm.h
 *
 *  Created on: March 28, 2017
 *
 */

#ifndef LPC8XX_SWM_H_
#define LPC8XX_SWM_H_

#include "LPC8xx.h"

// Port pin number equates
#define P0_0      0
#define P0_1      1
#define P0_2      2
#define P0_3      3
#define P0_4      4
#define P0_5      5
#define P0_6      6
#define P0_7      7
#define P0_8      8
#define P0_9      9
#define P0_10    10
#define P0_11    11
#define P0_12    12
#define P0_13    13
#define P0_14    14
#define P0_15    15
#define P0_16    16
#define P0_17    17
#define P0_18    18
#define P0_19    19
#define P0_20    20
#define P0_21    21
#define P0_22    22
#define P0_23    23
#define P0_24    24
#define P0_25    25
#define P0_26    26
#define P0_27    27
#define P0_28    28
#define P0_29    29
#define P0_30    30
#define P0_31    31
#define P1_0     32
#define P1_1     33
#define P1_2     34
#define P1_3     35
#define P1_4     36
#define P1_5     37
#define P1_6     38
#define P1_7     39
#define P1_8     40
#define P1_9     41
#define P1_10    42
#define P1_11    43
#define P1_12    44
#define P1_13    45
#define P1_14    46
#define P1_15    47
#define P1_16    48
#define P1_17    49
#define P1_18    50
#define P1_19    51
#define P1_20    52
#define P1_21    53
#define P1_22    54
#define P1_23    55
#define P1_24    56
#define P1_25    57
#define P1_26    58
#define P1_27    59
#define P1_28    60
#define P1_29    61
#define P1_30    62
#define P1_31    63
#define NO_PIN   0xFF



// Function name equates (for normal people)
#define U0_TXD        0
#define U0_RXD        1
#define U0_RTS        2
#define U0_CTS        3

#define U0_SCLK       4
#define U1_TXD        5
#define U1_RXD        6
#define U1_SCLK       7

#define SPI0_SCK      8
#define SPI0_MOSI     9
#define SPI0_MISO     10
#define SPI0_SSEL0    11

#define SPI0_SSEL1    12
#define T0_CAP0       13
#define T0_CAP1       14
#define T0_CAP2       15

#define T0_MAT0       16
#define T0_MAT1       17
#define T0_MAT2       18
#define T0_MAT3       19

#define I2C0_SDA      20
#define I2C0_SCL      21
#define COMP0_OUT     22
#define ACOMP         22
#define CLKOUT        23

#define GPIO_INT_BMAT 24
#define LVLSHFT_IN0   25
#define LVLSHFT_IN1   26
#define LVLSHFT_OUT0  27

#define LVLSHFT_OUT1  28
#define I2C1_SDA      29
#define I2C1_SCL      30
#define PLU_CLKIN     31

#define CAPT_X0       32
#define CAPT_X1       33
#define CAPT_X2       34
#define CAPT_X3       35

#define CAPT_X4       36
#define CAPT_YL       37
#define CAPT_YH       38

#define num_funcs     39



// Function name equates for the PINASSIGN_4PIN register
#define min_4pin_func 100

#define PLU_IN0    100
#define PLU_IN1    101
#define PLU_IN2    102
#define PLU_IN3    103
#define PLU_IN4    104
#define PLU_IN5    105
#define PLU_OUT0   106
#define PLU_OUT1   107
#define PLU_OUT2   108
#define PLU_OUT3   109
#define PLU_OUT4   110
#define PLU_OUT5   111
#define PLU_OUT6   112
#define PLU_OUT7   113

#define max_4pin_func 113



// PINENABLE0 register defines
#define ACMP_I1   (1<<0)
#define ACMP_I2   (1<<1)
#define ACMP_I3   (1<<2)
#define ACMP_I4   (1<<3)
#define SWCLK     (1<<4)
#define SWDIO     (1<<5)
#define RESETN    (1<<6)
#define CLKIN     (1<<7)
#define WKTCLKIN  (1<<8)
#define VDDCMP    (1<<9)
#define ADC_0     (1<<10)
#define ADC_1     (1<<11)
#define ADC_2     (1<<12)
#define ADC_3     (1<<13)
#define ADC_4     (1<<14)
#define ADC_5     (1<<15)
#define ADC_6     (1<<16)
#define ADC_7     (1<<17)
#define ADC_8     (1<<18)
#define ADC_9     (1<<19)
#define ADC_10    (1<<20)
#define ADC_11    (1<<21)
#define ACMP_I5   (1<<22)
#define DACOUT0   (1<<23)

/**
 * @brief LPC8XX Switch Matrix PLU Movable pins (works for LPC804 with PLU)
 */
typedef enum CHIP_SWM_PLU_MOVABLE  {
	PLU_INPUT0_PIO0_0   = 0x00 << 0,	/*!< PLU_INPUT0 - PIO0_0 */
	PLU_INPUT0_PIO0_8   = 0x01 << 0,	/*!< PLU_INPUT0 - PIO0_8 */
	PLU_INPUT0_PIO0_17  = 0x02 << 0,	/*!< PLU_INPUT0 - PIO0_17 */
	PLU_INPUT0_NONE     = 0x03 << 0,	/*!< PLU_INPUT0 - NONE */
	PLU_INPUT1_PIO0_1   = 0x00 << 2,	/*!< PLU_INPUT1 - PIO0_1 */
	PLU_INPUT1_PIO0_9   = 0x01 << 2,	/*!< PLU_INPUT1 - PIO0_9 */
	PLU_INPUT1_PIO0_18  = 0x02 << 2,	/*!< PLU_INPUT1 - PIO0_18 */
	PLU_INPUT1_NONE     = 0x03 << 2,	/*!< PLU_INPUT1 - NONE */
	PLU_INPUT2_PIO0_2   = 0x00 << 4,	/*!< PLU_INPUT2 - PIO0_2 */
	PLU_INPUT2_PIO0_10  = 0x01 << 4,	/*!< PLU_INPUT2 - PIO0_10 */
	PLU_INPUT2_PIO0_19  = 0x02 << 4,	/*!< PLU_INPUT2 - PIO0_19 */
	PLU_INPUT2_NONE     = 0x03 << 4,	/*!< PLU_INPUT2 - NONE */
	PLU_INPUT3_PIO0_3   = 0x00 << 6,	/*!< PLU_INPUT3 - PIO0_3 */
	PLU_INPUT3_PIO0_11  = 0x01 << 6,	/*!< PLU_INPUT3 - PIO0_11 */
	PLU_INPUT3_PIO0_20  = 0x02 << 6,	/*!< PLU_INPUT3 - PIO0_20 */
	PLU_INPUT3_NONE     = 0x03 << 6,	/*!< PLU_INPUT3 - NONE */
	PLU_INPUT4_PIO0_4   = 0x00 << 8,	/*!< PLU_INPUT4 - PIO0_4 */
	PLU_INPUT4_PIO0_12  = 0x01 << 8,	/*!< PLU_INPUT4 - PIO0_12 */
	PLU_INPUT4_PIO0_21  = 0x02 << 8,	/*!< PLU_INPUT4 - PIO0_21 */
	PLU_INPUT4_NONE     = 0x03 << 8,	/*!< PLU_INPUT4 - NONE */
	PLU_INPUT5_PIO0_5   = 0x00 << 10,	/*!< PLU_INPUT5 - PIO0_5 */
	PLU_INPUT5_PIO0_13  = 0x01 << 10,	/*!< PLU_INPUT5 - PIO0_13 */
	PLU_INPUT5_PIO0_22  = 0x02 << 10,	/*!< PLU_INPUT5 - PIO0_22 */
	PLU_INPUT5_NONE     = 0x03 << 10,	/*!< PLU_INPUT5 - NONE */
	PLU_OUTPUT0_PIO0_7  = 0x00 << 12,	/*!< PLU_OUTPUT0 - PIO0_7 */
	PLU_OUTPUT0_PIO0_14 = 0x01 << 12,	/*!< PLU_OUTPUT0 - PIO0_14 */
	PLU_OUTPUT0_PIO0_23 = 0x02 << 12,	/*!< PLU_OUTPUT0 - PIO0_23 */
	PLU_OUTPUT0_NONE    = 0x03 << 12,	/*!< PLU_OUTPUT0 - NONE */
	PLU_OUTPUT1_PIO0_8  = 0x00 << 14,	/*!< PLU_OUTPUT1 - PIO0_8 */
	PLU_OUTPUT1_PIO0_15 = 0x01 << 14,	/*!< PLU_OUTPUT1 - PIO0_15 */
	PLU_OUTPUT1_PIO0_24 = 0x02 << 14,	/*!< PLU_OUTPUT1 - PIO0_24 */
	PLU_OUTPUT1_NONE    = 0x03 << 14,	/*!< PLU_OUTPUT1 - NONE */
	PLU_OUTPUT2_PIO0_9  = 0x00 << 16,	/*!< PLU_OUTPUT2 - PIO0_9 */
	PLU_OUTPUT2_PIO0_16 = 0x01 << 16,	/*!< PLU_OUTPUT2 - PIO0_16 */
	PLU_OUTPUT2_PIO0_25 = 0x02 << 16,	/*!< PLU_OUTPUT2 - PIO0_25 */
	PLU_OUTPUT2_NONE    = 0x03 << 16,	/*!< PLU_OUTPUT2 - NONE */
	PLU_OUTPUT3_PIO0_10 = 0x00 << 18,	/*!< PLU_OUTPUT3 - PIO0_10 */
	PLU_OUTPUT3_PIO0_17 = 0x01 << 18,	/*!< PLU_OUTPUT3 - PIO0_17 */
	PLU_OUTPUT3_PIO0_26 = 0x02 << 18,	/*!< PLU_OUTPUT3 - PIO0_26 */
	PLU_OUTPUT3_NONE    = 0x03 << 18,	/*!< PLU_OUTPUT3 - NONE */
	PLU_OUTPUT4_PIO0_11 = 0x00 << 20,	/*!< PLU_OUTPUT4 - PIO0_11 */
	PLU_OUTPUT4_PIO0_18 = 0x01 << 20,	/*!< PLU_OUTPUT4 - PIO0_18 */
	PLU_OUTPUT4_PIO0_27 = 0x02 << 20,	/*!< PLU_OUTPUT4 - PIO0_27 */
	PLU_OUTPUT4_NONE    = 0x03 << 20,	/*!< PLU_OUTPUT4 - NONE */
	PLU_OUTPUT5_PIO0_12 = 0x00 << 22,	/*!< PLU_OUTPUT5 - PIO0_12 */
	PLU_OUTPUT5_PIO0_19 = 0x01 << 22,	/*!< PLU_OUTPUT5 - PIO0_19 */
	PLU_OUTPUT5_PIO0_28 = 0x02 << 22,	/*!< PLU_OUTPUT5 - PIO0_28 */
	PLU_OUTPUT5_NONE    = 0x03 << 22,	/*!< PLU_OUTPUT5 - NONE */
	PLU_OUTPUT6_PIO0_13 = 0x00 << 24,	/*!< PLU_OUTPUT6 - PIO0_13 */
	PLU_OUTPUT6_PIO0_20 = 0x01 << 24,	/*!< PLU_OUTPUT6 - PIO0_20 */
	PLU_OUTPUT6_PIO0_29 = 0x02 << 24,	/*!< PLU_OUTPUT6 - PIO0_29 */
	PLU_OUTPUT6_NONE    = 0x03 << 24,	/*!< PLU_OUTPUT6 - NONE */
	PLU_OUTPUT7_PIO0_14 = 0x00 << 26,	/*!< PLU_OUTPUT7 - PIO0_14 */
	PLU_OUTPUT7_PIO0_21 = 0x01 << 26,	/*!< PLU_OUTPUT7 - PIO0_21 */
	PLU_OUTPUT7_PIO0_30 = 0x02 << 26,	/*!< PLU_OUTPUT7 - PIO0_30 */
	PLU_OUTPUT7_NONE    = 0x03 << 26,	/*!< PLU_OUTPUT7 - NONE */
} CHIP_SWM_PLU_MOVABLE_T;


// Function prototypes
void ConfigSWM(uint32_t func, uint32_t port_pin);
void EnableFixedPinFunc(uint32_t func);
void DisableFixedPinFunc(uint32_t func);

#endif /* LPC8XX_SWM_H_ */
