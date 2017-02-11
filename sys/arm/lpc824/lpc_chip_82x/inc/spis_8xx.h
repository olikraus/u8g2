/*
 * @brief LPC8XX SPI slave driver
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
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

#ifndef __SPIS_8XX_H_
#define __SPIS_8XX_H_

#include "spi_common_8xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup SPI_SLAVE_8XX CHIP: LPC8XX SPI slave driver
 * @ingroup SPI_COMMON_8XX
 * @{
 */

/**
 * Macro defines for SPI Status register
 */

/* Clear RXOV Flag */
#define SPI_STAT_CLR_RXOV       ((uint32_t) (1 << 2))
/* Clear TXUR Flag */
#define SPI_STAT_CLR_TXUR       ((uint32_t) (1 << 3))
/* Clear SSA Flag */
#define SPI_STAT_CLR_SSA        ((uint32_t) (1 << 4))
/* Clear SSD Flag */
#define SPI_STAT_CLR_SSD        ((uint32_t) (1 << 5))

struct SPIS_XFER;

/** @brief SPI slave select assertion callback
 * This callback is called from the SPI slave handler when an SPI slave select (SSEL)
 * is initially asserted. It is used to indicate the start of a slave transfer that
 * will happen on the bus.
 */
typedef void (*SPISlaveXferCSAssert)(struct SPIS_XFER *pSlaveXfer);

/** @brief SPI slave send data callback
 * This callback is called from the SPI slave handler when an SPI slave select (SSEL)
 * needs a data buffer to send.
 */
typedef void (*SPISlaveXferSend)(struct SPIS_XFER *pSlaveXfer);

/** @brief SPI slave receive data callback
 * This callback is called from the SPI slave handler when an SPI slave select (SSEL)
 * needs a buffer to place data.
 */
typedef void (*SPISlaveXferRecv)(struct SPIS_XFER *pSlaveXfer);

/** @brief SPI slave select de-assertion callback
 * This callback is called from the SPI slave handler when an SPI slave select (SSEL)
 * is de-asserted. It can be used to indicate the end of a transfer.
 */
typedef void (*SPISlaveXferCSDeAssert)(struct SPIS_XFER *pSlaveXfer);

/** SPI slave callback functions */
typedef struct {
	SPISlaveXferCSAssert    slaveXferCSAssert;		/** SPI transfer start callback, called on SPI CS assertion */
	SPISlaveXferSend        slaveXferSend;			/** SPI transfer data receive buffer callback, called when a receive buffer is needed */
	SPISlaveXferRecv        slaveXferRecv;			/** SPI transfer send buffer callback, called when data is needed */
	SPISlaveXferCSDeAssert  slaveXferCSDeAssert;	/** SPI transfer completion callback, called on SPI CS deassertion */
} SPIS_CALLBACKS_T;

/** Slave transfer data context */
typedef struct SPIS_XFER {
	const SPIS_CALLBACKS_T *pCB;	/** Pointer to SPI slave callback functions */
	union {							/** Pointer to receive buffer, set to NULL to toss receeive data */
		uint8_t *pRXData8;			/** Receive buffer used with data transfer size <= 8-bits, modified by driver */
		uint16_t *pRXData16;		/** Receive buffer used with data transfer size > 8-bits, modified by driver */
	};

	union {							/** Pointer to transmit buffer, set to NULL to transmit 0x0 */
		uint8_t *pTXData8;			/** Send buffer used with data transfer size <= 8-bits, modified by driver */
		uint16_t *pTXData16;		/** Send buffer used with data transfer size > 8-bits, modified by driver */
	};

	uint16_t rxCount;				/** Size of the pRXData buffer in items (not bytes), modified by driver */
	uint16_t txCount;				/** Number of items (not bytes) to send in pTXData buffer, modified by driver */
	uint16_t dataRXferred;			/** Total items (not bytes) received, modified by driver */
	uint16_t dataTXferred;			/** Total items (not bytes) transmitted, modified by driver */
	uint8_t sselNum;				/** Slave number assigned to this transfer, 0 - 3, modified by driver */
} SPIS_XFER_T;

/**
 * @brief	SPI slave transfer state change handler
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	xfers	: Pointer to a SPIS_XFER_T structure see notes below
 * @return	returns 0 on success, or SPI_STAT_RXOV and/or SPI_STAT_TXUR on an error
 * @note	See @ref SPIS_XFER_T for more information on this function. When using
 * this function, the SPI slave interrupts should be enabled and setup in the SPI
 * interrupt handler to call this function when they fire. This function is meant
 * to be called from the interrupt handler. The @ref SPIS_XFER_T data does not need
 * to be setup prior to the call and should be setup by the callbacks instead.<br>
 *
 * The callbacks are handled in the interrupt handler. If you are getting overflow
 * or underflow errors, you might need to lower the speed of the master clock or
 * extend the master's select assetion time.<br>
 */
uint32_t Chip_SPIS_XferHandler(LPC_SPI_T *pSPI, SPIS_XFER_T *xfer);

/**
 * @brief	Pre-buffers slave transmit data
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	xfers	: Pointer to a SPIS_XFER_T structure see notes below
 * @return	Nothing
 * @note Pre-buffering allows the slave to prime the transmit FIFO with data prior to
 * the master starting a transfer. If data is not pre-buffered, the initial slave
 * transmit data will always be 0x0 with a slave transmit underflow status.
 * Pre-buffering is best used when only a single slave select is used by an
 * application.
 */
STATIC INLINE void Chip_SPIS_PreBuffSlave(LPC_SPI_T *pSPI, SPIS_XFER_T *xfer)
{
	Chip_SPIS_XferHandler(pSPI, xfer);
}

/**
 * @brief	SPI slave transfer blocking function
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	xfers	: Pointer to a SPIS_XFER_T structure
 * @return	returns 0 on success, or SPI_STAT_RXOV and/or SPI_STAT_TXUR on an error
 * @note	This function performs a blocking transfer on the SPI slave interface.
 * It is not recommended to use this function. Once this function is called, it
 * will block forever until a slave transfer consisting of a slave SSEL assertion,
 * and de-assertion occur. The callbacks are still used for slave data buffer
 * management. SPI interrupts must be disabled prior to calling this function.
 */
uint32_t Chip_SPIS_XferBlocking(LPC_SPI_T *pSPI, SPIS_XFER_T *xfer);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SPIS_8XX_H_ */
