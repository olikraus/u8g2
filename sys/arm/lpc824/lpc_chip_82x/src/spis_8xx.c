/*
 * @brief LPC540XX SPI master driver
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licenser disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Flag used to tell polling function (if used) that a deassert event
   happened */
static volatile bool deasserted;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Determine SSEL associated with the current data value */
static uint8_t Chip_SPIS_FindSSEL(LPC_SPI_T *pSPI, uint32_t data)
{
	int i;
	uint8_t ssel = 0;

	for (i = 0; i <= 3; i++) {
		if ((data & SPI_RXDAT_RXSSELNUM_INACTIVE(i)) == 0) {
			/* Signal is active on low */
			ssel = (uint8_t) i;
		}
	}

	return ssel;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* SPI slave transfer state change handler */
uint32_t Chip_SPIS_XferHandler(LPC_SPI_T *pSPI, SPIS_XFER_T *xfer)
{
	uint32_t staterr, data;
	uint8_t flen;

	/* Get length of a receive value */
	flen = ((pSPI->TXCTRL & ~SPI_TXCTRL_RESERVED) >> 24) & 0xF;

	/* Get errors for later, we'll continue even if errors occur, but we notify
	   caller on return */
	staterr = Chip_SPI_GetStatus(pSPI) & (SPI_STAT_RXOV | SPI_STAT_TXUR);
	if (staterr != 0) {
		Chip_SPI_ClearStatus(pSPI, staterr);
	}

	/* Slave assertion */
	if ((Chip_SPI_GetStatus(pSPI) & SPI_STAT_SSA) != 0) {
		Chip_SPI_ClearStatus(pSPI, SPI_STAT_SSA);

		/* Determine SPI select. Read the data FIFO to get the slave number. Data
		   should not be in the receive FIFO yet, only the statuses */
		xfer->sselNum = Chip_SPIS_FindSSEL(pSPI, Chip_SPI_ReadRawRXFifo(pSPI));

		/* SSEL assertion callback */
		xfer->pCB->slaveXferCSAssert(xfer);
	}

	/* Slave de-assertion */
	if ((Chip_SPI_GetStatus(pSPI) & SPI_STAT_SSD) != 0) {
		Chip_SPI_ClearStatus(pSPI, SPI_STAT_SSD);
		deasserted = true;
		xfer->pCB->slaveXferCSDeAssert(xfer);
	}

	/* Transmit data? */
	while ((Chip_SPI_GetStatus(pSPI) & SPI_STAT_TXRDY) != 0) {
		if (xfer->txCount == 0) {
			/* Request a new buffer first */
			xfer->pCB->slaveXferSend(xfer);
		}

		/* Send 0 on empty buffer or count */
		if ((xfer->txCount == 0) || (xfer->pTXData8 == NULL)) {
			data = 0;
		}
		else {
			/* Copy buffer to data */
			if (flen > 8) {
				data = (uint32_t) *xfer->pTXData16;
				xfer->pTXData16++;
			}
			else {
				data = (uint32_t) *xfer->pTXData8;
				xfer->pTXData8++;
			}

			xfer->dataTXferred++;
			xfer->txCount--;
		}

		Chip_SPI_WriteTXData(pSPI, data);
	}

	/* Data received? */
	while ((Chip_SPI_GetStatus(pSPI) & SPI_STAT_RXRDY) != 0) {
		/* Get raw data and status */
		data = Chip_SPI_ReadRawRXFifo(pSPI);

		/* Only copy data when not ignoring receive */
		if (xfer->pRXData8 != NULL) {
			/* Enough size in current buffers? */
			if (xfer->rxCount == 0) {
				/* Request a new buffer first */
				xfer->pCB->slaveXferRecv(xfer);
			}

			/* Copy data to buffer */
			if (flen > 8) {
				*xfer->pRXData16 = (uint16_t) (data & 0xFFFF);
				xfer->pRXData16++;
			}
			else {
				*xfer->pRXData8 = (uint8_t) (data & 0xFF);
				xfer->pRXData8++;
			}

			xfer->dataRXferred++;
			xfer->rxCount--;
		}
	}

	return staterr;
}

/* SPI slave transfer blocking function */
uint32_t Chip_SPIS_XferBlocking(LPC_SPI_T *pSPI, SPIS_XFER_T *xfer)
{
	uint32_t status = 0;

	/* Wait forever until deassertion event */
	deasserted = false;
	while ((!deasserted) && (status == 0)) {
		status = Chip_SPIS_XferHandler(pSPI, xfer);
	}

	return status;
}
