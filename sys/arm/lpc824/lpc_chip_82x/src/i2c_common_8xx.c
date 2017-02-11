/*
 * @brief LPC15xx I2C Common driver
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

#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Get the RESET ID corresponding to the given I2C base */
static CHIP_SYSCTL_PERIPH_RESET_T I2C_GetResetID(LPC_I2C_T *pI2C)
{
	uint32_t base = (uint32_t) pI2C;
	switch (base) {
		case LPC_I2C1_BASE:
			return RESET_I2C1;
		case LPC_I2C2_BASE:
			return RESET_I2C2;
		case LPC_I2C3_BASE:
			return RESET_I2C3;
		default:
			return RESET_I2C0;
	}
}

/* Get the CLOCK ID corresponding to the given I2C base */
static CHIP_SYSCTL_CLOCK_T I2C_GetClockID(LPC_I2C_T *pI2C)
{
	uint32_t base = (uint32_t) pI2C;
	switch (base) {
		case LPC_I2C1_BASE:
			return SYSCTL_CLOCK_I2C1;
		case LPC_I2C2_BASE:
			return SYSCTL_CLOCK_I2C2;
		case LPC_I2C3_BASE:
			return SYSCTL_CLOCK_I2C3;
		default:
			return SYSCTL_CLOCK_I2C0;
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initializes the LPC_I2C peripheral */
void Chip_I2C_Init(LPC_I2C_T *pI2C)
{
	/* Enable I2C clock */
	Chip_Clock_EnablePeriphClock(I2C_GetClockID(pI2C));

	/* Peripheral reset control to I2C */
	Chip_SYSCTL_PeriphReset(I2C_GetResetID(pI2C));
}

/* Shuts down the I2C controller block */
void Chip_I2C_DeInit(LPC_I2C_T *pI2C)
{
	/* Disable I2C clock */
	Chip_Clock_DisablePeriphClock(I2C_GetClockID(pI2C));
}
