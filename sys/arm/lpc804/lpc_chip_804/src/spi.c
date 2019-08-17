/*
 * spi.c
 *
 *  Created on: Apr 5, 2016
 *
 */

#include "LPC8xx.h"
//#include "utilities.h"
#include "spi.h"

uint32_t SPI_RXCount, SPI_TXCount;

void SPI0_IRQHandler(void)
{
  uint32_t active = LPC_SPI0->STAT & LPC_SPI0->INTENSET;

  if(active & SPI_STAT_RXRDY) {
		SPI_RXCount++;
		LPC_SPI0->INTENCLR = SPI_STAT_RXRDY;
  }
  if(active & SPI_STAT_TXRDY) {
		SPI_TXCount++;
		LPC_SPI0->INTENCLR = SPI_STAT_TXRDY;
  }
  return;
}

/*****************************************************************************
** Function name:		SPImasterWriteOnly
**
** Description:		  Write a block of data to the SPI slave using polling.
**									In this example, data width is limited to 8 bits only.
**
** parameters:      pointer to the write buffer.
**                  Length of the write data block 
** Returned value:		None
**
*****************************************************************************/
void SPImasterWriteOnly( uint8_t *WrBuf, uint32_t WrLen )
{	
    uint32_t i = 0;
	volatile uint32_t temp;

	LPC_SPI0->TXCTL &= ~(SPI_CTL_EOT);                 // Start a new transfer, clear the EOT bit
	SPI_FLASH_CS0();	/* Drive SPI CS low. */	
	if ( WrLen == 1 ) {
		while ( (LPC_SPI0->STAT & SPI_STAT_TXRDY) == 0 );
		LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | SPI_CTL_EOT | *WrBuf;
	}
	else {
		while ( i < WrLen ) {
			/* Move only if TXRDY is ready */
			while ( (LPC_SPI0->STAT & SPI_STAT_TXRDY) == 0 );
			if ( i == 0 ) {
				LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | SPI_TXDATCTL_RX_IGNORE | *WrBuf++;
			}
			else if ( i == WrLen-1 ) {
				LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | SPI_CTL_EOT | *WrBuf++;
			}
			else {
				LPC_SPI0->TXDAT = *WrBuf++;
			}
			i++;
		}
	}
	while ( (LPC_SPI0->STAT & SPI_STAT_RXRDY) == 0 );	/* For last frame to be sent, wait until RX on MISO arrives before raising CS. */
	temp = LPC_SPI0->RXDAT;
	SPI_FLASH_CS1();	/* Drive SPI CS high */
	return;
}

/*****************************************************************************
** Function name:		SPImasterWriteRead
**
** Description:		  Write data on the SPI MOSI line while reading data from MISO.
**									In this example, data width is limited to 8 bits only.
**
** parameters:      pointer to the write buffer
                    pointer to the read buffer
                    length of the write/read data buffer
** Returned value:	None
**
*****************************************************************************/
void SPImasterWriteRead( uint8_t *WrBuf, uint8_t *RdBuf, uint32_t WrLen )
{	
	uint32_t i = 0;

  LPC_SPI0->TXCTL &= ~(SPI_CTL_EOT);                 // Start a new transfer, clear the EOT bit	
	SPI_FLASH_CS0();	/* Drive SPI CS low. */	
	if ( WrLen == 1 ) {
		while ( (LPC_SPI0->STAT & SPI_STAT_TXRDY) == 0 );
		LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | SPI_CTL_EOT | *WrBuf;
		while ( (LPC_SPI0->STAT & SPI_STAT_RXRDY) == 0 );
		*RdBuf = LPC_SPI0->RXDAT;
	}
	else {
		while ( i < WrLen ) {
			/* Move only if TXRDY is ready */
			while ( (LPC_SPI0->STAT & SPI_STAT_TXRDY) == 0 );
			if ( i == 0 ) {
				LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | *WrBuf++;
			}
			else if ( i == WrLen-1 ) {
				LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | SPI_CTL_EOT | *WrBuf++;
			}
			else {
				LPC_SPI0->TXDAT = *WrBuf++;
			}
			while ( (LPC_SPI0->STAT & SPI_STAT_RXRDY) == 0 );
			*RdBuf++ = LPC_SPI0->RXDAT;
			i++;
		}
	}
	SPI_FLASH_CS1();	/* Drive SPI CS high */
	return;
}

/*****************************************************************************
** Function name:		SPImasterReadOnly
**
** Description:		  Read a block of data to the SPI slave using polling. It's almost
**                  the same as SPImasterWriteRead() except that SPI master writes 
**                  dummy data the MOSI.
**									In this example, data width is limited to 8 bits only.
**                  
** parameters:      pointer to the read buffer.
**                  Length of the write data block 
** Returned value:		None
**
*****************************************************************************/
void SPImasterReadOnly( uint8_t *RdBuf, uint32_t RdLen )
{	
	uint32_t i = 0;

  LPC_SPI0->TXCTL &= ~(SPI_CTL_EOT);                 // Start a new transfer, clear the EOT bit
	SPI_FLASH_CS0();	/* Drive SPI CS low. */	
	if ( RdLen == 1 ) {
		while ( (LPC_SPI0->STAT & SPI_STAT_TXRDY) == 0 );
		LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | SPI_CTL_EOT | DUMMY_BYTE;
		while ( (LPC_SPI0->STAT & SPI_STAT_RXRDY) == 0 );
		*RdBuf = LPC_SPI0->RXDAT;
	}
	else {
		while ( i < RdLen ) {
			/* Move only if TXRDY is ready */
			while ( (LPC_SPI0->STAT & SPI_STAT_TXRDY) == 0 );
			if ( i == 0 ) {
				LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | DUMMY_BYTE;
			}
			else if ( i == RdLen-1 ) {
				LPC_SPI0->TXDATCTL = SPI_CTL_LEN(DATA_WIDTH) | SPI_CTL_EOT | DUMMY_BYTE;
			}
			else {
				LPC_SPI0->TXDAT = DUMMY_BYTE;
			}
			while ( (LPC_SPI0->STAT & SPI_STAT_RXRDY) == 0 );
			*RdBuf++ = LPC_SPI0->RXDAT;
			i++;
		}
	}
	SPI_FLASH_CS1();	/* Drive SPI CS high */
	return;
}

