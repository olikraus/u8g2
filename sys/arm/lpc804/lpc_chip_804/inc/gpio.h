/****************************************************************************
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation is hereby granted, under NXP Semiconductors' 
 * relevant copyright in the software, without fee, provided that it 
 * is used in conjunction with NXP Semiconductors microcontrollers. This 
 * copyright, permission, and disclaimer notice must appear in all copies of 
 * this code.
 
****************************************************************************/
#ifndef __LPC8XX_GPIO_H 
#define __LPC8XX_GPIO_H


#define INPUT  0
#define OUTPUT 1

#define PORT0			0
#define PORT1			1

#define Bit0            0
#define Bit1            1
#define Bit2            2
#define Bit3            3
#define Bit4            4
#define Bit5            5
#define Bit6            6
#define Bit7            7
#define Bit8            8
#define Bit9            9
#define Bit10           10
#define Bit11           11
#define Bit12           12
#define Bit13           13
#define Bit14           14
#define Bit15           15
#define Bit16           16
#define Bit17           17
#define Bit18           18
#define Bit19           19
#define Bit20           20
#define Bit21           21
#define Bit22           22
#define Bit23           23
#define Bit24           24
#define Bit25           25
#define Bit26           26
#define Bit27           27
#define Bit28           28
#define Bit29           29
#define Bit30           30
#define Bit31           31


void PININT_Handler ( uint32_t irq_num );
void PININT0_IRQHandler(void);
void PININT1_IRQHandler(void);
void PININT2_IRQHandler(void);
void PININT3_IRQHandler(void);
void PININT4_IRQHandler(void);
void PININT5_IRQHandler(void);
void PININT6_IRQHandler(void);
void PININT7_IRQHandler(void);

void GPIOInit( void );
uint32_t GPIOGetPinValue( uint32_t portNum, uint32_t bitPosi );
void GPIOSetBitValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );

#endif /* end __LPC8XX_GPIO_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
