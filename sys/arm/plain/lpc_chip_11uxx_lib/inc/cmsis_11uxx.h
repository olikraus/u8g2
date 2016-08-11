/*
 * @brief Basic CMSIS include file for LPC11UXX
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
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

#ifndef __CMSIS_11UXX_H_
#define __CMSIS_11UXX_H_

#include "lpc_types.h"
#include "sys_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup CMSIS_11UXX CHIP: LPC11UXX CMSIS include file
 * @ingroup CHIP_11XX_CMSIS_Drivers
 * Applies to LPC11U12, LPC11U13, LPC11U14, LPC11U23, LPC11U24,
 * LPC11U34, LPC11U35, LPC11U36, and LPC11U37 devices.
 * @{
 */

#if defined(__ARMCC_VERSION)
// Kill warning "#pragma push with no matching #pragma pop"
  #pragma diag_suppress 2525
  #pragma push
  #pragma anon_unions
#elif defined(__CWCC__)
  #pragma push
  #pragma cpp_extensions on
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
//  #pragma push // FIXME not usable for IAR
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

#if !defined(CHIP_LPC11UXX)
#error Incorrect or missing device variant (CHIP_LPC11UXX)
#endif

/** @defgroup CMSIS_11UXX_IRQ CHIP_LPC11UXX: LPC11UXX peripheral interrupt numbers
 * @{
 */

typedef enum LPC11UXX_IRQn {
	NonMaskableInt_IRQn           = -14,	/*!< 2 Non Maskable Interrupt                           */
	HardFault_IRQn                = -13,	/*!< 3 Cortex-M0 Hard Fault Interrupt                   */
	SVCall_IRQn                   = -5,		/*!< 11 Cortex-M0 SV Call Interrupt                     */
	PendSV_IRQn                   = -2,		/*!< 14 Cortex-M0 Pend SV Interrupt                     */
	SysTick_IRQn                  = -1,		/*!< 15 Cortex-M0 System Tick Interrupt                 */

	PIN_INT0_IRQn                 = 0,		/*!< Pin Interrupt 0 */
	PIN_INT1_IRQn                 = 1,		/*!< Pin Interrupt 1 */
	PIN_INT2_IRQn                 = 2,		/*!< Pin Interrupt 2 */
	PIN_INT3_IRQn                 = 3,		/*!< Pin Interrupt 3 */
	PIN_INT4_IRQn                 = 4,		/*!< Pin Interrupt 4 */
	PIN_INT5_IRQn                 = 5,		/*!< Pin Interrupt 5 */
	PIN_INT6_IRQn                 = 6,		/*!< Pin Interrupt 6 */
	PIN_INT7_IRQn                 = 7,		/*!< Pin Interrupt 7 */
	GINT0_IRQn                    = 8,		/*!< GPIO GROUP 0 interrupt */
	GINT1_IRQn                    = 9,		/*!< GPIO GROUP 1 interrupt */
	Reserved10_IRQn               = 10,		/*!< Reserved Interrupt                               */
	Reserved11_IRQn               = 11,
	Reserved12_IRQn               = 12,
	Reserved13_IRQn               = 13,
	SSP1_IRQn                     = 14,		/*!< SSP1 Interrupt                                   */
	I2C0_IRQn                     = 15,		/*!< I2C Interrupt                                    */
	TIMER_16_0_IRQn               = 16,		/*!< 16-bit Timer0 Interrupt                          */
	TIMER_16_1_IRQn               = 17,		/*!< 16-bit Timer1 Interrupt                          */
	TIMER_32_0_IRQn               = 18,		/*!< 32-bit Timer0 Interrupt                          */
	TIMER_32_1_IRQn               = 19,		/*!< 32-bit Timer1 Interrupt                          */
	SSP0_IRQn                     = 20,		/*!< SSP0 Interrupt                                   */
	UART0_IRQn                    = 21,		/*!< UART Interrupt                                   */
	USB0_IRQn                     = 22,		/*!< USB IRQ interrupt                                */
	USB0_FIQ_IRQn                 = 23,		/*!< USB FIQ interrupt                                */
	ADC_IRQn                      = 24,		/*!< A/D Converter Interrupt                          */
	WDT_IRQn                      = 25,		/*!< Watchdog timer Interrupt                         */
	BOD_IRQn                      = 26,		/*!< Brown Out Detect(BOD) Interrupt                  */
	FMC_IRQn                      = 27,		/*!< Flash Memory Controller Interrupt                */
	RESERVED28_IRQn               = 28,
	RESERVED29_IRQn               = 29,
	USB_WAKEUP_IRQn               = 30,		/*!< USB wake-up interrupt Interrupt                  */
	IOH_IRQn                      = 31,		/*!< I/O Handler IRQ (Only for LPC11U37)              */
} LPC11UXX_IRQn_Type;

/**
 * @}
 */

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/** @defgroup CMSIS_11UXX_COMMON CHIP: LPC11uxx Cortex CMSIS definitions
 * @{
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT             0			/*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2			/*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0			/*!< Set to 1 if different SysTick Config is used     */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CMSIS_11UXX_H_ */
