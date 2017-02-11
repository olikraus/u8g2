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

static volatile bool xmitOn;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Get SPI master bit rate */
uint32_t Chip_SPIM_GetClockRate(LPC_SPI_T *pSPI)
{
	return Chip_Clock_GetSystemClockRate() / ((pSPI->DIV & ~SPI_DIV_RESERVED) + 1);
}

/* Set SPI master bit rate */
uint32_t Chip_SPIM_SetClockRate(LPC_SPI_T *pSPI, uint32_t rate)
{
	uint32_t baseClock, div;

	/* Get peripheral base clock rate */
	baseClock = Chip_Clock_GetSystemClockRate();

	/* Compute divider */
	div = baseClock / rate;

	/* Limit values */
	if (div == 0) {
		div = 1;
	}
	else if (div > 0x10000) {
		div = 0x10000;
	}
	pSPI->DIV = div - 1;

	return Chip_SPIM_GetClockRate(pSPI);
}

/* Configure SPI Delay parameters */
void Chip_SPIM_DelayConfig(LPC_SPI_T *pSPI, SPIM_DELAY_CONFIG_T *pConfig)
{
	pSPI->DLY = (SPI_DLY_PRE_DELAY(pConfig->PreDelay) |
				 SPI_DLY_POST_DELAY(pConfig->PostDelay) |
				 SPI_DLY_FRAME_DELAY(pConfig->FrameDelay) |
				 SPI_DLY_TRANSFER_DELAY(pConfig->TransferDelay - 1));
}

/* Assert a SPI select */
void Chip_SPIM_AssertSSEL(LPC_SPI_T *pSPI, uint8_t sselNum)
{
	uint32_t reg;

	reg = pSPI->TXCTRL & SPI_TXDATCTL_CTRLMASK;

	/* Assert a SSEL line by driving it low */
	reg &= ~SPI_TXDATCTL_DEASSERTNUM_SSEL(sselNum);

	pSPI->TXCTRL = reg;
}

/* Deassert a SPI select */
void Chip_SPIM_DeAssertSSEL(LPC_SPI_T *pSPI, uint8_t sselNum)
{
	uint32_t reg;

	reg = pSPI->TXCTRL & SPI_TXDATCTL_CTRLMASK;
	pSPI->TXCTRL = reg | SPI_TXDATCTL_DEASSERTNUM_SSEL(sselNum);
}

/* SPI master transfer state change handler */
void Chip_SPIM_XferHandler(LPC_SPI_T *pSPI, SPIM_XFER_T *xfer)
{
	uint32_t data;
	uint8_t flen;

	/* Get length of a receive value */
	flen = ((pSPI->TXCTRL & ~SPI_TXCTRL_RESERVED) >> 24) & 0xF;

	/* Master asserts slave */
	if ((Chip_SPI_GetStatus(pSPI) & SPI_STAT_SSA) != 0) {
		Chip_SPI_ClearStatus(pSPI, SPI_STAT_SSA);

		/* SSEL assertion callback */
		xfer->pCB->masterXferCSAssert(xfer);
	}

	/* Slave de-assertion */
	if ((Chip_SPI_GetStatus(pSPI) & SPI_STAT_SSD) != 0) {
		Chip_SPI_ClearStatus(pSPI, SPI_STAT_SSD);

		/* If transmitter disabled and deassert happens, the transfer is done */
		if (xmitOn == false) {
			xfer->pCB->mMasterXferDone(xfer);
		}
	}

	/* Transmit data? */
	while (((Chip_SPI_GetStatus(pSPI) & SPI_STAT_TXRDY) != 0) && (xmitOn == true)) {
		if ((xfer->txCount == 1) && (xfer->terminate)) {
			/* Transfer is done, this will be last data */
			Chip_SPIM_ForceEndOfTransfer(pSPI);
			xmitOn = false;
		}
		else if (xfer->txCount == 0) {
			/* Request a new buffer first */
			xfer->pCB->masterXferSend(xfer);
		}

		if (xfer->txCount > 0) {
			/* Send 0 if ignoring transmit */
			if (xfer->pTXData8 == NULL) {
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
			}

			Chip_SPI_WriteTXData(pSPI, data);
			xfer->txCount--;
		}
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
				xfer->pCB->masterXferRecv(xfer);
			}

			/* Copy data to buffer */
			if (xfer->rxCount > 0) {
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
	}
}

/* Start non-blocking SPI master transfer */
void Chip_SPIM_Xfer(LPC_SPI_T *pSPI, SPIM_XFER_T *xfer)
{
	/* Setup SPI master select, data length, EOT/EOF timing, and RX data ignore */
	pSPI->TXCTRL = xfer->options | SPI_TXDATCTL_DEASSERT_ALL;
	Chip_SPIM_AssertSSEL(pSPI, xfer->sselNum);

	/* Clear initial transfer states */
	xfer->dataRXferred = xfer->dataTXferred = 0;

	/* Call main handler to start transfer */
	xmitOn = true;
	Chip_SPIM_XferHandler(pSPI, xfer);
}

/* Perform blocking SPI master transfer */
void Chip_SPIM_XferBlocking(LPC_SPI_T *pSPI, SPIM_XFER_T *xfer)
{
	/* Start trasnfer */
	Chip_SPIM_Xfer(pSPI, xfer);

	/* Wait for termination */
	while (xmitOn == true) {
		Chip_SPIM_XferHandler(pSPI, xfer);
	}
}
