/*
 * @brief LPC8XX SPI master driver
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

#ifndef __SPIM_8XX_H_
#define __SPIM_8XX_H_

#include "spi_common_8xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup SPI_MASTER_8XX CHIP: LPC8XX SPI master driver
 * @ingroup SPI_COMMON_8XX
 * @{
 */

/**
 * @brief	Get SPI master bit rate
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @return	The actual SPI clock bit rate
 */
uint32_t Chip_SPIM_GetClockRate(LPC_SPI_T *pSPI);

/**
 * @brief	Set SPI master bit rate
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	rate	: Desired clock bit rate for the SPI interface
 * @return	The actual SPI clock bit rate
 * @note	This function will set the SPI clock divider to get closest
 * to the desired rate as possible.
 */
uint32_t Chip_SPIM_SetClockRate(LPC_SPI_T *pSPI, uint32_t rate);

/**
 * @brief SPI Delay Configure Struct
 */
typedef struct {
	uint8_t PreDelay;					/** Pre-delay value in SPI clocks, 0 - 15 */
	uint8_t PostDelay;					/** Post-delay value in SPI clocks, 0 - 15 */
	uint8_t FrameDelay;					/** Delay value between frames of a transfer in SPI clocks, 0 - 15 */
	uint8_t TransferDelay;				/** Delay value between transfers in SPI clocks, 1 - 16 */
} SPIM_DELAY_CONFIG_T;

/**
 * @brief	Config SPI Delay parameters
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	pConfig	: SPI Delay Configure Struct
 * @return	Nothing
 */
void Chip_SPIM_DelayConfig(LPC_SPI_T *pSPI, SPIM_DELAY_CONFIG_T *pConfig);

/**
 * @brief	Forces an end of transfer for the current master transfer
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @return	Nothing
 * @note	Use this function to perform an immediate end of trasnfer for the
 * current master operation. If the master is currently transferring data started
 * with the Chip_SPIM_Xfer function, this terminates the transfer after the
 * current byte completes and completes the transfer.
 */
STATIC INLINE void Chip_SPIM_ForceEndOfTransfer(LPC_SPI_T *pSPI)
{
	pSPI->STAT = SPI_STAT_EOT;
}

/**
 * @brief	Assert a SPI select
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	sselNum	: SPI select to assert, 0 - 3
 * @return	Nothing
 */
void Chip_SPIM_AssertSSEL(LPC_SPI_T *pSPI, uint8_t sselNum);

/**
 * @brief	Deassert a SPI select
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	sselNum	: SPI select to deassert, 0 - 3
 * @return	Nothing
 */
void Chip_SPIM_DeAssertSSEL(LPC_SPI_T *pSPI, uint8_t sselNum);

/**
 * @brief	Enable loopback mode
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @return	Nothing
 * @note	Serial input is taken from the serial output (MOSI or MISO) rather
 * than the serial input pin.
 */
STATIC INLINE void Chip_SPIM_EnableLoopBack(LPC_SPI_T *pSPI)
{
	Chip_SPI_SetCFGRegBits(pSPI, SPI_CFG_LBM_EN);
}

/**
 * @brief	Disable loopback mode
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @return	Nothing
 */
STATIC INLINE void Chip_SPIM_DisableLoopBack(LPC_SPI_T *pSPI)
{
	Chip_SPI_ClearCFGRegBits(pSPI, SPI_CFG_LBM_EN);
}

struct SPIM_XFER;

/** @brief SPI master select assert callback
 * This callback is called from the SPI master handler when the SPI master
 * selects the slave (asserts SSEL).
 */
typedef void (*SPIMasterXferCSAssert)(struct SPIM_XFER *pMasterXfer);

/** @brief SPI master send data callback
 * This callback is called from the SPI master handler when the SPI master
 * needs a data buffer to send.
 */
typedef void (*SPIMasterXferSend)(struct SPIM_XFER *pMasterXfer);

/** @brief SPI master receive data callback
 * This callback is called from the SPI master handler when the SPI master
 * needs a buffer to place data into.
 */
typedef void (*SPIMasterXferRecv)(struct SPIM_XFER *pMasterXfer);

/** @brief SPI master transfer select deassert data callback
 * This callback is called from the SPI master handler when the SPI master
 * deasserts the slave select.
 */
typedef void (*SPIMMasterXferCSDeAssert)(struct SPIM_XFER *pMasterXfer);

/** @brief SPI master transfer done data callback
 * This callback is called from the SPI master handler when the SPI master
 * has completed the transfer and becomes idle.
 */
typedef void (*SPIMMasterXferDone)(struct SPIM_XFER *pMasterXfer);

/** SPI slave callback functions */
typedef struct {
	SPIMasterXferCSAssert   masterXferCSAssert;		/** SPI transfer CS assert, called when a slave select is asserted */
	SPIMasterXferSend       masterXferSend;			/** SPI transfer data receive buffer callback, called when a send buffer is needed */
	SPIMasterXferRecv       masterXferRecv;			/** SPI transfer send buffer callback, called when send buffer is needed (and SPI_TXCTL_RXIGNORE option is not set) */
	SPIMMasterXferCSDeAssert mMasterXferCSDeAssert;	/** SPI transfer CS deassert, called when a slave select is deasserted */
	SPIMMasterXferDone      mMasterXferDone;		/** SPI transfer done callback, called when transfer is complete */
} SPIM_CALLBACKS_T;

/** Slave transfer data context */
typedef struct SPIM_XFER {
	const SPIM_CALLBACKS_T *pCB;	/** Pointer to SPI master data callback functions */
	union {							/** Pointer to receive buffer, set to NULL to toss receeive data */
		uint8_t *pRXData8;			/** Receive buffer used with data transfer size <= 8-bits, modified by driver */
		uint16_t *pRXData16;		/** Receive buffer used with data transfer size > 8-bits, modified by driver */
	};

	union {							/** Pointer to transmit buffer, set to NULL to transmit 0x0 */
		uint8_t *pTXData8;			/** Send buffer used with data transfer size <= 8-bits, modified by driver */
		uint16_t *pTXData16;		/** Send buffer used with data transfer size > 8-bits, modified by driver */
	};

	uint32_t options;				/** Master transfer options, an OR'ed value of SPI_TXCTL_EOT, SPI_TXCTL_EOF, SPI_TXCTL_RXIGNORE, and SPI_TXCTL_FLEN(bits) */
	uint16_t rxCount;				/** Size of the pRXData buffer in items (not bytes), modified by driver */
	uint16_t txCount;				/** Number of items (not bytes) to send in pTXData buffer, modified by driver */
	uint16_t dataRXferred;			/** Total items (not bytes) received, modified by driver */
	uint16_t dataTXferred;			/** Total items (not bytes) transmitted, modified by driver */
	uint8_t sselNum;				/** Slave number assigned to this transfer, 0 - 3, used by driver to select slave */
	bool    terminate;				/** Transfer will terminate when txCount goes to 0 and master goes idle, must be set before last byte is sent */
} SPIM_XFER_T;

/**
 * @brief	SPI master transfer state change handler
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	xfers	: Pointer to a SPIM_XFER_T structure see notes below
 * @return	Nothing
 * @note	See @ref SPIM_XFER_T for more information on this function. When using
 * this function, the SPI master interrupts should be enabled and setup in the SPI
 * interrupt handler to call this function when they fire. This function is meant
 * to be called from the interrupt handler.
 */
void Chip_SPIM_XferHandler(LPC_SPI_T *pSPI, SPIM_XFER_T *xfer);

/**
 * @brief	Start non-blocking SPI master transfer
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	xfers	: Pointer to a SPIM_XFER_T structure see notes below
 * @return	Nothing
 * @note	This function starts a non-blocking SPI master transfer with the
 * parameters setup in the passed @ref SPIM_XFER_T structure. Once the transfer is
 * started, the interrupt handler must call Chip_SPIM_XferHandler to keep the
 * transfer going and fed with data. This function should only be called when
 * the master is idle.<br>
 *
 * This function must be called with the options and sselNum fields correctly
 * setup. Initial data buffers and the callback pointer must also be setup. No
 * sanity checks are performed on the passed data.<br>
 *
 * Example call:<br>
 * SPIM_XFER_T mxfer;
 * mxfer.pCB = &masterCallbacks;
 * mxfer.sselNum = 2; // Use chip select 2
 * mxfer.options = SPI_TXCTL_FLEN(8); // 8 data bits, supports 1 - 16 bits
 * mxfer.options |= SPI_TXCTL_EOT | SPI_TXCTL_EOF; // Apply frame and transfer delays to master transfer
 * mxfer.options |= SPI_TXCTL_RXIGNORE; // Ignore RX data, will toss receive data regardless of pRXData8 or pRXData16 buffer
 * mxfer.pTXData8 = SendBuffer;
 * mxfer.txCount = 16; // Number of bytes to send before SPIMasterXferSend callback is called
 * mxfer.pRXData8 = RecvBuffer; // Will not receive data if pRXData8/pRXData16 is NULL or SPI_TXCTL_RXIGNORE option is set
 * mxfer.rxCount = 16; // Number of bytes to receive before SPIMasterXferRecv callback is called
 * Chip_SPIM_Xfer(LPC_SPI0, &mxfer); // Start transfer
 *
 * Note that the transfer, once started, needs to be constantly fed by the callbacks.
 * The txCount and rxCount field only indicate the buffer size before the callbacks are called.
 * To terminate the transfer, the SPIMasterXferSend callback must set the terminate field.
 */
void Chip_SPIM_Xfer(LPC_SPI_T *pSPI, SPIM_XFER_T *xfer);

/**
 * @brief	Perform blocking SPI master transfer
 * @param	pSPI	: The base of SPI peripheral on the chip
 * @param	xfers	: Pointer to a SPIM_XFER_T structure see notes below
 * @return	Nothing
 * @note	This function starts a blocking SPI master transfer with the
 * parameters setup in the passed @ref SPIM_XFER_T structure. Once the transfer is
 * started, the callbacks in Chip_SPIM_XferHandler may be called to keep the
 * transfer going and fed with data. SPI interrupts must be disabled prior to
 * calling this function. It is not recommended to use this function.<br>
 */
void Chip_SPIM_XferBlocking(LPC_SPI_T *pSPI, SPIM_XFER_T *xfer);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SPIM_8XX_H_ */
