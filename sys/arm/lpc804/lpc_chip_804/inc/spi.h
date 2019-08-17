/**********************************************************************
***********************************************************************
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
* documentation is hereby granted, under NXP Semiconductors�
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/


#ifndef lpc8xx_SPI_H_
#define lpc8xx_SPI_H_

#include "LPC8xx.h"
#include "swm.h"

#define SPI_FLASH_CS1()    (LPC_GPIO_PORT->SET[0] = 1<<P0_16)				/* Use for LPC804 */
#define SPI_FLASH_CS0()    (LPC_GPIO_PORT->CLR[0] = 1<<P0_16)				/* Use for LPC804 */

#define DATA_WIDTH									8
#define DUMMY_BYTE									0x55

#define BUFFER_SIZE									64
#define LPC_SPI0BAUDRate            500000 

#define SPI_CFG_ENABLE			(1 << 0)
#define SPI_CFG_MASTER			(1 << 2)
#define SPI_CFG_SLAVE				(0 << 2)
#define SPI_CFG_LSBF				(1 << 3)
#define SPI_CFG_CPHA				(1 << 4)
#define SPI_CFG_CPOL				(1 << 5)
#define SPI_CFG_MOSIDRV			(1 << 6)
#define SPI_CFG_LOOPBACK		(1 << 7)
#define SPI_CFG_SPOL            (1<<8)
//#define SPI_CFG_SPOL(s)			(1 << (8 + s))

#define SPI_DLY_PREDELAY(d)		((d) << 0)
#define SPI_DLY_POSTDELAY(d)	((d) << 4)
#define SPI_DLY_FRAMEDELAY(d)	((d) << 8)
#define SPI_DLY_INTERDELAY(d)	((d) << 12)

#define SPI_STAT_RXRDY 				(1 << 0)
#define SPI_STAT_TXRDY 				(1 << 1)
#define SPI_STAT_RXOVERRUN 		(1 << 2)
#define SPI_STAT_TXUNDERRUN 	(1 << 3)
#define SPI_STAT_SELNASSERT 	(1 << 4)
#define SPI_STAT_SELNDEASSERT (1 << 5)
#define SPI_STAT_CLKSTALL 		(1 << 6)
#define SPI_STAT_ET             (1 << 7)
#define SPI_STAT_MSTIDLE        (1<<8)
#define SPI_STAT_ERROR_MASK		(SPI_STAT_RXOVERRUN|SPI_STAT_TXUNDERRUN|SPI_STAT_SELNASSERT|SPI_STAT_SELNDEASSERT|SPI_STAT_CLKSTALL)

#define SPI_RXRDYEN             (1<<0)
#define SPI_TXRDYEN             (1<<1)
#define SPI_RXOVEN              (1<<2)
#define SPI_TXUREN              (1<<3)
#define SPI_SSAEN               (1<<4)
#define SPI_SSDEN               (1<<5)

#define SPI_RXRDY             (1<<0)
#define SPI_TXRDY             (1<<1)
#define SPI_RXOV              (1<<2)
#define SPI_TXUR              (1<<3)
#define SPI_SSA               (1<<4)
#define SPI_SSD               (1<<5)

#define SPI_CTL_TXSSELN   (1<<16)
#define SPI_CTL_EOT       (1<<20)
#define SPI_CTL_EOF       (1<<21)
#define SPI_CTL_RXIGNORE  (1<<22)
#define SPI_CTL_LEN(b)    (((b)-1)<<24)

#define SPI_TXDATCTL_SSELN(s) 	(s << 16)
#define SPI_TXDATCTL_EOT 				(1 << 20)
#define SPI_TXDATCTL_EOF 				(1 << 21)
#define SPI_TXDATCTL_RX_IGNORE	(1 << 22)
#define SPI_TXDATCTL_FSIZE(s)		((s) << 24)

#define SPI_RXDAT_SOT						(1 << 20)

void SPI0_IRQHandler(void);
void SPImasterWriteOnly( uint8_t *WrBuf, uint32_t WrLen );
void SPImasterWriteRead( uint8_t *WrBuf, uint8_t *RdBuf, uint32_t WrLen );
void SPImasterReadOnly( uint8_t *RdBuf, uint32_t RdLen );


#endif /* lpc8xx_SPI_H_ */
