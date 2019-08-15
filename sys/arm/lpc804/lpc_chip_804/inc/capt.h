/*
 * lpc8xx_capt.h
 *
 *  Created on: This day
 *      Author: The Creator
 */

#ifndef LPC8XX_CAPT_H_
#define LPC8XX_CAPT_H_

#define CAPT_X_PIN_COUNT    5U

// CTRL register 
#define POLLMODE_INACTIVE   (0x0<<0)
#define POLLMODE_NOW        (0x1<<0)
#define POLLMODE_CONTINUOUS (0x2<<0)
#define POLLMODE_LOWPWR     (0x3<<0)
#define POLLMODE            0             // Use this as a shifter for a 2-bit value

#define TYPE_NORMAL       (0x0<<2)
#define TYPE_3x3          (0x1<<2)
#define TYPE_5_INTERLEAVE (0x2<<2)

#define TYPE_TRIGGER_YH   (0x0<<4)
#define TYPE_TRIGGER_ACMP (0x1<<4)

#define WAIT              (0x1<<5)

#define DMA_NONE          (0x0<<6)
#define DMA_TOUCH         (0x1<<6)
#define DMA_BOTH          (0x2<<6)
#define DMA_BOTH_PLUS_TO  (0x3<<6)

#define FDIV              8               // Use this as a shifter for a 4-bit value

#define XPINUSE_HIGHZ     (0x0<<12)
#define XPINUSE_LOW       (0x1<<12)

#define INCHANGE          (0x1<<15)       // Use as an AND-mask, when reading this bit

#define X0_ACTV           1<<0
#define X1_ACTV           1<<1
#define X2_ACTV           1<<2
#define X3_ACTV           1<<3
#define X4_ACTV           1<<4
#define X5_ACTV           1<<5
#define X6_ACTV           1<<6
#define X7_ACTV           1<<7
#define X8_ACTV           1<<8
#define X9_ACTV           1<<9
#define X10_ACTV          1<<10
#define X11_ACTV          1<<11
#define X12_ACTV          1<<12
#define X13_ACTV          1<<13
#define X14_ACTV          1<<14
#define X15_ACTV          1<<15
#define XPINSEL           16              // Use this as a shifter for a 16-bit value


// STATUS, INTENSET, INTENCLR, INTSTAT registers
#define YESTOUCH          (1<<0)
#define NOTOUCH           (1<<1)
#define POLLDONE          (1<<2)
#define TIMEOUT           (1<<3)
#define OVERRUN           (1<<4)
#define BUSY              (1<<8)
#define XMAX              (0xFF<<16)      // Use as an AND-mask, when reading this field

// POLL_TCNT register
#define TCNT               0              // Use this as a shifter for a 12-bit value
#define TOUT               12             // Use this as a shifter for a 4-bit value
#define POLL               16             // Use this as a shifter for an 8-bit value
#define MDELAY             24             // Use this as a shifter for a 2-bit value
#define RDELAY             26             // Use this as a shifter for a 2-bit value
#define TCHLOWER           31             // Use this as a shifter for a 1-bit value

// TOUCH register
#define TOUCH_COUNT        (0xFFF<<0)     // Use as an AND-mask, when reading this field
#define TOUCH_XVAL         (0xF<<12)      // Use as an AND-mask, when reading this field
#define TOUCH_ISTOUCH      (0x1<<16)      // Use as an AND-mask, when reading this field
#define TOUCH_ISTO         (0x1<<17)      // Use as an AND-mask, when reading this field
#define TOUCH_SEQ          (0xF<<20)      // Use as an AND-mask, when reading this field
#define TOUCH_CHANGE       (0x1<<31)      // Use as an AND-mask, when reading this field

#endif /* LPC8XX_CAPT_H_ */
