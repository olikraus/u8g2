/*
 * @brief LPC8xx clock driver
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
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

/* Inprecise clock rates for the watchdog oscillator */
static const uint32_t wdtOSCRate[WDTLFO_OSC_4_60 + 1] = {
	0,					/* WDT_OSC_ILLEGAL */
	600000,				/* WDT_OSC_0_60 */
	1050000,			/* WDT_OSC_1_05 */
	1400000,			/* WDT_OSC_1_40 */
	1750000,			/* WDT_OSC_1_75 */
	2100000,			/* WDT_OSC_2_10 */
	2400000,			/* WDT_OSC_2_40 */
	2700000,			/* WDT_OSC_2_70 */
	3000000,			/* WDT_OSC_3_00 */
	3250000,			/* WDT_OSC_3_25 */
	3500000,			/* WDT_OSC_3_50 */
	3750000,			/* WDT_OSC_3_75 */
	4000000,			/* WDT_OSC_4_00 */
	4200000,			/* WDT_OSC_4_20 */
	4400000,			/* WDT_OSC_4_40 */
	4600000				/* WDT_OSC_4_60 */
};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Compute a WDT or LFO rate */
static uint32_t Chip_Clock_GetWDTLFORate(uint32_t reg)
{
	uint32_t div;
	CHIP_WDTLFO_OSC_T clk;

	/* Get WDT oscillator settings */
	clk = (CHIP_WDTLFO_OSC_T) ((reg >> 5) & 0xF);
	div = reg & 0x1F;

	/* Compute clock rate and divided by divde value */
	return wdtOSCRate[clk] / ((div + 1) << 1);
}

/* Compute PLL frequency */
static uint32_t Chip_Clock_GetPLLFreq(uint32_t PLLReg, uint32_t inputRate)
{
	uint32_t m_val = ((PLLReg & 0x1F) + 1);

	return (inputRate * m_val);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Set System PLL clock source */
void Chip_Clock_SetSystemPLLSource(CHIP_SYSCTL_PLLCLKSRC_T src)
{
	LPC_SYSCTL->SYSPLLCLKSEL  = (uint32_t) src;
    
    /* sequnce a 0 followed by 1 to update PLL source selection */
	LPC_SYSCTL->SYSPLLCLKUEN  = 0;  
	LPC_SYSCTL->SYSPLLCLKUEN  = 1;  
}

/* Bypass System Oscillator and set oscillator frequency range */
void Chip_Clock_SetPLLBypass(bool bypass, bool highfr)
{
	uint32_t ctrl = 0;

	if (bypass) {
		ctrl |= (1 << 0);
	}
	if (highfr) {
		ctrl |= (1 << 1);
	}

	LPC_SYSCTL->SYSOSCCTRL = ctrl;
}

/* Set main system clock source */
void Chip_Clock_SetMainClockSource(CHIP_SYSCTL_MAINCLKSRC_T src)
{
	LPC_SYSCTL->MAINCLKSEL  = (uint32_t) src;
    
    /* sequnce a 0 followed by 1 to update MAINCLK source selection */
	LPC_SYSCTL->MAINCLKUEN  = 0;
	LPC_SYSCTL->MAINCLKUEN  = 1;
}

/* Set CLKOUT clock source and divider */
void Chip_Clock_SetCLKOUTSource(CHIP_SYSCTL_CLKOUTSRC_T src, uint32_t div)
{
	LPC_SYSCTL->CLKOUTSEL = (uint32_t) src;
    
    /* sequnce a 0 followed by 1 to update CLKOUT source selection */
	LPC_SYSCTL->CLKOUTUEN = 0;
	LPC_SYSCTL->CLKOUTUEN = 1;
	LPC_SYSCTL->CLKOUTDIV = div;
}

/* Return estimated watchdog oscillator rate */
uint32_t Chip_Clock_GetWDTOSCRate(void)
{
	return Chip_Clock_GetWDTLFORate(LPC_SYSCTL->WDTOSCCTRL & ~SYSCTL_WDTOSCCTRL_RESERVED);
}

/* Return System PLL input clock rate */
uint32_t Chip_Clock_GetSystemPLLInClockRate(void)
{
	uint32_t clkRate;

	switch ((CHIP_SYSCTL_PLLCLKSRC_T) (LPC_SYSCTL->SYSPLLCLKSEL & 0x3)) {
	case SYSCTL_PLLCLKSRC_IRC:
		clkRate = Chip_Clock_GetIntOscRate();
		break;

	case SYSCTL_PLLCLKSRC_SYSOSC:
		clkRate = Chip_Clock_GetMainOscRate();
		break;

	case SYSCTL_PLLCLKSRC_EXT_CLKIN:
		clkRate = Chip_Clock_GetExtClockInRate();
		break;

	default:
		clkRate = 0;
	}

	return clkRate;
}

/* Return System PLL output clock rate */
uint32_t Chip_Clock_GetSystemPLLOutClockRate(void)
{
	return Chip_Clock_GetPLLFreq((LPC_SYSCTL->SYSPLLCTRL & ~SYSCTL_SYSPLLCTRL_RESERVED),
								 Chip_Clock_GetSystemPLLInClockRate());
}

/* Return main clock rate */
uint32_t Chip_Clock_GetMainClockRate(void)
{
	uint32_t clkRate = 0;

	switch ((CHIP_SYSCTL_MAINCLKSRC_T) (LPC_SYSCTL->MAINCLKSEL & 0x3)) {
	case SYSCTL_MAINCLKSRC_IRC:
		clkRate = Chip_Clock_GetIntOscRate();
		break;

	case SYSCTL_MAINCLKSRC_PLLIN:
		clkRate = Chip_Clock_GetSystemPLLInClockRate();
		break;

	case SYSCTL_MAINCLKSRC_WDTOSC:
		clkRate = Chip_Clock_GetWDTOSCRate();
		break;

	case SYSCTL_MAINCLKSRC_PLLOUT:
		clkRate = Chip_Clock_GetSystemPLLOutClockRate();
		break;
	}

	return clkRate;
}

/* Return system clock rate */
uint32_t Chip_Clock_GetSystemClockRate(void)
{
	/* No point in checking for divide by 0 */
	return Chip_Clock_GetMainClockRate() / (LPC_SYSCTL->SYSAHBCLKDIV & ~SYSCTL_SYSAHBCLKDIV_RESERVED);
}

/* Get USART 0/1/2 UART base rate */
uint32_t Chip_Clock_GetUSARTNBaseClockRate(void)
{
	uint64_t inclk;
	uint32_t div;

	div = (uint32_t) Chip_Clock_GetUARTClockDiv();
	if (div == 0) {
		/* Divider is 0 so UART clock is disabled */
		inclk = 0;
	}
	else {
		uint32_t mult, divf;

		/* Input clock into FRG block is the divided main system clock */
		inclk = (uint64_t) (Chip_Clock_GetMainClockRate() / div);

		divf = Chip_SYSCTL_GetUSARTFRGDivider();
		if (divf == 0xFF) {
			/* Fractional part is enabled, get multiplier */
			mult = (uint32_t) Chip_SYSCTL_GetUSARTFRGMultiplier();

			/* Get fractional error */
			inclk = (inclk * 256) / (uint64_t) (256 + mult);
		}
	}

	return (uint32_t) inclk;
}

/* Set USART 0/1/2 UART base rate */
uint32_t Chip_Clock_SetUSARTNBaseClockRate(uint32_t rate, bool fEnable)
{
	uint32_t div, inclk;

	/* Input clock into FRG block is the main system clock */
	inclk = Chip_Clock_GetMainClockRate();

	/* Get integer divider for coarse rate */
	div = inclk / rate;
	if (div == 0) {
		div = 1;
	}

	/* Approximated rate with only integer divider */
	Chip_Clock_SetUARTClockDiv((uint8_t) div);

	if (fEnable) {
		uint32_t uart_fra_multiplier;

		/* Reset FRG */
		Chip_SYSCTL_PeriphReset(RESET_UARTFBRG);

		/* Enable fractional divider */
		Chip_SYSCTL_SetUSARTFRGDivider(0xFF);

		/* Compute the fractional divisor (the lower byte is the
		   fractional portion) */
		uart_fra_multiplier = ((inclk / div) * 256) / rate;

		/* ...just the fractional portion (the lower byte) */
		Chip_SYSCTL_SetUSARTFRGMultiplier((uint8_t) uart_fra_multiplier);
	}
	else {
		/* Disable fractional generator and use integer divider only */
		Chip_SYSCTL_SetUSARTFRGDivider(0);
	}

	return Chip_Clock_GetUSARTNBaseClockRate();
}

/* Get the IOCONCLKDIV clock rate */
uint32_t Chip_Clock_GetIOCONCLKDIVClockRate(CHIP_PIN_CLKDIV_T reg)
{
	uint32_t div = LPC_SYSCTL->IOCONCLKDIV[reg] & ~SYSCTL_IOCONCLKDIV_RESERVED;
	uint32_t main_clk = Chip_Clock_GetMainClockRate();
	
	return (div == 0) ? 0 : (main_clk / div);
}

void Chip_Clock_SetIOCONCLKDIV(CHIP_PIN_CLKDIV_T reg, uint8_t div)
{
	int t_reg = IOCONCLK_MAX-reg;
	LPC_SYSCTL->IOCONCLKDIV[t_reg] = div;
}
