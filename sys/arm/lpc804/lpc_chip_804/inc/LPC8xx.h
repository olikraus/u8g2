// Attention please!
// This is the header file for the LPC80x product family only.

/****************************************************************************
 *   $Id:: LPC8xx.h 6437 2012-10-31 11:06:06Z dep00694                     $
 *   Project: NXP LPC8xx software example  
 *
 *   Description:
 *     CMSIS Cortex-M0+ Core Peripheral Access Layer Header File for 
 *     NXP LPC800 Device Series 
 *
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
#ifndef __LPC8xx_H__
#define __LPC8xx_H__

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup LPC8xx_Definitions LPC8xx Definitions
  This file defines all structures and symbols for LPC8xx:
    - Registers and bitfields
    - peripheral base address
    - PIO definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup LPC8xx_CMSIS LPC8xx CMSIS Definitions
  Configuration of the Cortex-M0+ Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition ----------------------------------- 
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  Reset_IRQn                    = -15,    /*!< 1 Reset Vector, invoked on Power up and warm reset*/  
  NonMaskableInt_IRQn           = -14,    /*!< 2 Non Maskable Interrupt                           */
  HardFault_IRQn                = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                   */
  SVCall_IRQn                   = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                     */
  DebugMonitor_IRQn             = -4,     /*!<  12  Debug Monitor                                 */
  PendSV_IRQn                   = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                     */
  SysTick_IRQn                  = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                 */

/******  LPC80x Specific Interrupt Numbers ********************************************************/
  SPI0_IRQn                     = 0,        /*!< SPI0                                             */
  DAC0_IRQn                     = 2,        /*!< DAC0 Interrupt                                   */	
  UART0_IRQn                    = 3,        /*!< USART0                                           */
  UART1_IRQn                    = 4,        /*!< USART1                                           */
  I2C1_IRQn                     = 7,        /*!< I2C1                                             */
  I2C0_IRQn                     = 8,        /*!< I2C0                                             */
  MRT_IRQn                      = 10,       /*!< MRT                                              */ 
  CMP_IRQn                      = 11,       /*!< Analog Comparator /CapTouch                      */
  WDT_IRQn                      = 12,       /*!< WDT                                              */
  BOD_IRQn                      = 13,       /*!< BOD                                              */
  FLASH_IRQn                    = 14,       /*!< FLASH                                            */
  WKT_IRQn                      = 15,       /*!< WKT Interrupt                                    */
  ADC_SEQA_IRQn                 = 16,       /*!< ADC Seq. A                                       */    
  ADC_SEQB_IRQn                 = 17,       /*!< ADC Seq. B                                       */
  ADC_THCMP_IRQn                = 18,       /*!< ADC Thresh Comp                                  */    
  ADC_OVR_IRQn                  = 19,       /*!< ADC overrun                                      */
  CTIMER0_IRQn                  = 23,       /*!< Timer 0 Interrupt                                */
  PININT0_IRQn                  = 24,       /*!< External Interrupt 0                             */
  PININT1_IRQn                  = 25,       /*!< External Interrupt 1                             */  
  PININT2_IRQn                  = 26,       /*!< External Interrupt 2                             */
  PININT3_IRQn                  = 27,       /*!< External Interrupt 3                             */
  PININT4_IRQn                  = 28,       /*!< External Interrupt 4                             */
  PININT5_IRQn                  = 29,       /*!< External Interrupt 5                             */
  PININT6_IRQn                  = 30,       /*!< External Interrupt 6                             */
  PININT7_IRQn                  = 31,       /*!< External Interrupt 7                             */
} IRQn_Type;

#define CAPT_IRQn CMP_IRQn

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0+ Processor and Core Peripherals */
#define __CM0PLUS_REV             0x0001
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

/*@}*/ /* end of group LPC8xx_CMSIS */


#include "core_cm0plus.h"                  /* Cortex-M0+ processor and core peripherals          */
#include "system.h"                        /* System Header                                      */


#if defined ( __CC_ARM   )
#pragma anon_unions                        // This is needed by the Keil compiler 
#endif

/******************************************************************************/
/*                Device Specific Peripheral Registers structures             */
/******************************************************************************/

//------------- System Control (SYSCON) --------------------------------------
typedef struct
{
  __IO uint32_t SYSMEMREMAP         ; ///< (0x000) System memory remap
  __IO uint32_t RESERVED0[8]        ; ///< (0x004 - 0x020)
  __IO uint32_t LPOSCCTRL           ; ///< (0x024) Low power oscillator control
  __IO uint32_t FROOSCCTRL          ; ///< (0x028) FRO oscillator control 
  __IO uint32_t RESERVED2[3]        ; ///< (0x02C - 0x034)
  __IO uint32_t SYSRSTSTAT          ; ///< (0x038) System reset status 0
  __IO uint32_t RESERVED3[5]        ; ///< (0x03C - 0x04C)
  __IO uint32_t MAINCLKSEL          ; ///< (0x050) Main clock source select
  __IO uint32_t MAINCLKUEN          ; ///< (0x054) Main clock source update enable
  __IO uint32_t SYSAHBCLKDIV        ; ///< (0x058) System clock divider
  __IO uint32_t RESERVED4[1]        ; ///< (0x05C)
  __IO uint32_t CAPTCLKSEL          ; ///< (0x060) /* Added by Andrew Su 2018-01-18. */
  __IO uint32_t ADCCLKSEL           ; ///< (0x064) ADC clock source select
  __IO uint32_t ADCCLKDIV           ; ///< (0x068) ADC clock divider
  __IO uint32_t RESERVED5[4]        ; ///< (0x06C - 0x078)
  __IO uint32_t LPOSCCLKEN          ; ///< (0x07C)
  union {
    __IO uint32_t SYSAHBCLKCTRL[2]  ;
    struct {
    __IO uint32_t SYSAHBCLKCTRL0    ; ///< (0x080) System clock group 0 control
    __IO uint32_t SYSAHBCLKCTRL1    ; ///< (0x084) System clock group 1 control
    };
  };
  union {
    __IO uint32_t PRESETCTRL[2]     ;
    struct {
    __IO uint32_t PRESETCTRL0       ; ///< (0x088) Peripheral reset group 0 control
    __IO uint32_t PRESETCTRL1       ; ///< (0x08C) Peripheral reset group 1 control
    };
  };
  union {
    __IO uint32_t FCLKSEL[11]       ;
    struct {
      __IO uint32_t UART0CLKSEL     ; ///< (0x090) FCLK0 clock source select
      __IO uint32_t UART1CLKSEL     ; ///< (0x094) FCLK1 clock source select
      __IO uint32_t unimp2CLKSEL    ; ///< (0x098) FCLK2 clock source select
      __IO uint32_t unimp3CLKSEL    ; ///< (0x09C) FCLK3 clock source select
      __IO uint32_t unimp4CLKSEL    ; ///< (0x0A0) FCLK4 clock source select
      __IO uint32_t I2C0CLKSEL      ; ///< (0x0A4) FCLK5 clock source select
      __IO uint32_t I2C1CLKSEL      ; ///< (0x0A8) FCLK6 clock source select
      __IO uint32_t unimp7CLKSEL    ; ///< (0x0AC) FCLK7 clock source select
      __IO uint32_t unimp8CLKSEL    ; ///< (0x0B0) FCLK8 clock source select
      __IO uint32_t SPI0CLKSEL      ; ///< (0x0B4) FCLK9 clock source select
      __IO uint32_t unimp10CLKSEL   ; ///< (0x0B8) FCLK10 clock source select
    };
  }; 
  __IO uint32_t RESERVED6[5]        ; ///< (0x0BC - 0x0CC)
  __IO uint32_t FRG0DIV             ; ///< (0x0D0) Fractional generator divider value
  __IO uint32_t FRG0MULT            ; ///< (0x0D4) Fractional generator multiplier value
  __IO uint32_t FRG0CLKSEL          ; ///< (0x0D8) FRG0 clock source select
  __IO uint32_t RESERVED8[5]        ; ///< (0x0DC - 0x0EC)
  __IO uint32_t CLKOUTSEL           ; ///< (0x0F0) CLKOUT clock source select
  __IO uint32_t CLKOUTDIV           ; ///< (0x0F4) CLKOUT clock divider
  __IO uint32_t RESERVED10[2]       ; ///< (0x0F8 - 0x0FC)
  __I  uint32_t PIOPORCAP0          ; ///< (0x100) POR captured PIO0 status 0
  __I  uint32_t RESERVED11[19]      ; ///< (0x104 - 0x14C) 
  __IO uint32_t BODCTRL             ; ///< (0x150) Brown-Out Detect
  __IO uint32_t SYSTCKCAL           ; ///< (0x154) System tick counter calibration
  __IO uint32_t RESERVED12[6]       ; ///< (0x158 - 0x16C)
  __IO uint32_t IRQLATENCY          ; ///< (0x170) IRQ delay. Allows trade-off between interrupt latency and determinism.
  __IO uint32_t NMISRC              ; ///< (0x174) NMI Source Control
  union {
    __IO uint32_t PINTSEL[8]        ;
    struct {
    __IO uint32_t PINTSEL0          ; ///< (0x178) GPIO Pin Interrupt Select 0
    __IO uint32_t PINTSEL1          ; ///< (0x17C) GPIO Pin Interrupt Select 1
    __IO uint32_t PINTSEL2          ; ///< (0x180) GPIO Pin Interrupt Select 2
    __IO uint32_t PINTSEL3          ; ///< (0x184) GPIO Pin Interrupt Select 3
    __IO uint32_t PINTSEL4          ; ///< (0x188) GPIO Pin Interrupt Select 4
    __IO uint32_t PINTSEL5          ; ///< (0x18C) GPIO Pin Interrupt Select 5
    __IO uint32_t PINTSEL6          ; ///< (0x190) GPIO Pin Interrupt Select 6
    __IO uint32_t PINTSEL7          ; ///< (0x194) GPIO Pin Interrupt Select 7
    };
  };
  __IO uint32_t RESERVED13[27]      ; ///< (0x198 - 0x200)
  __IO uint32_t STARTERP0           ; ///< (0x204) Start logic 0 pin wake-up enable
  __IO uint32_t RESERVED14[3]       ; ///< (0x208 - 0x210)
  __IO uint32_t STARTERP1           ; ///< (0x214) Start logic 1 interrupt wake-up enable
  __IO uint32_t RESERVED15[6]       ; ///< (0x218 - 0x22C)
  __IO uint32_t PDSLEEPCFG          ; ///< (0x230) Power-down states in deep-sleep mode
  __IO uint32_t PDAWAKECFG          ; ///< (0x234) Power-down states for wake-up from deep-sleep
  __IO uint32_t PDRUNCFG            ; ///< (0x238) Power configuration
  __IO uint32_t RESERVED16[111]     ; ///< (0x23C - 0x3F4)
  __I  uint32_t DEVICE_ID           ; ///< (0x3F8) Device ID
} LPC_SYSCON_TypeDef;




// ---------------- IOCON ----------------
typedef struct {                            
  __IO uint32_t PIO0_17;   // 0x00
  __IO uint32_t PIO0_13;   // 0x04
  __IO uint32_t PIO0_12;   // 0x08
  __IO uint32_t PIO0_5;    // 0x0C
  __IO uint32_t PIO0_4;    // 0x10
  __IO uint32_t PIO0_3;    // 0x14
  __IO uint32_t PIO0_2;    // 0x18
  __IO uint32_t PIO0_11;   // 0x1C
  __IO uint32_t PIO0_10;   // 0x20
  __IO uint32_t PIO0_16;   // 0x24
  __IO uint32_t PIO0_15;   // 0x28
  __IO uint32_t PIO0_1;    // 0x2C
  __IO uint32_t PIO0_21;   // 0x30
  __IO uint32_t PIO0_9;    // 0x34
  __IO uint32_t PIO0_8;    // 0x38
  __IO uint32_t PIO0_7;    // 0x3C
  __IO uint32_t PIO0_29;   // 0x40
  __IO uint32_t PIO0_0;    // 0x44
  __IO uint32_t PIO0_14;   // 0x48
  __IO uint32_t PIO0_28;   // 0x4C
  __IO uint32_t PIO0_27;   // 0x50
  __IO uint32_t PIO0_26;   // 0x54
  __IO uint32_t PIO0_20;   // 0x58
  __IO uint32_t PIO0_30;   // 0x5C
  __IO uint32_t PIO0_19;   // 0x60
  __IO uint32_t PIO0_25;   // 0x64
  __IO uint32_t PIO0_24;   // 0x68
  __IO uint32_t PIO0_23;   // 0x6C
  __IO uint32_t PIO0_22;   // 0x70
  __IO uint32_t PIO0_18;   // 0x74
} LPC_IOCON_TypeDef;




//------------- Power Management Unit (PMU) --------------------------
typedef struct
{
  __IO uint32_t PCON;                   //!< Offset: 0x000 Power control Register (R/W) 
  __IO uint32_t GPREG0;                 //!< Offset: 0x004 General purpose Register 0 (R/W)
  __IO uint32_t GPREG1;                 //!< Offset: 0x008 General purpose Register 1 (R/W)
  __IO uint32_t GPREG2;                 //!< Offset: 0x00C General purpose Register 2 (R/W)
  __IO uint32_t GPREG3;                 //!< Offset: 0x010 General purpose Register 3 (R/W)
  __IO uint32_t GPREG4;                 //!< Offset: 0x014 General purpose Register 4 (R/W)
  __I  uint32_t RESERVED[2];            // 0x18 - 0x1C   
  __IO uint32_t WUSRCREG;               // 0x20
  __IO uint32_t WUENAREG;               // 0x24
} LPC_PMU_TypeDef;




//------------- Switch Matrix (SWM) --------------------------
typedef struct
{
  union {
    __IO uint32_t PINASSIGN[10];
    struct {
      __IO uint32_t PINASSIGN0;   // 0x000
      __IO uint32_t PINASSIGN1;   // 0x004
      __IO uint32_t PINASSIGN2;   // 0x008
      __IO uint32_t PINASSIGN3;   // 0x00C
      __IO uint32_t PINASSIGN4;   // 0x010
      __IO uint32_t PINASSIGN5;   // 0x014
      __IO uint32_t PINASSIGN6;   // 0x018
      __IO uint32_t PINASSIGN7;   // 0x01C
      __IO uint32_t PINASSIGN8;   // 0x020
      __IO uint32_t PINASSIGN9;   // 0x024
    };
  };
  __I  uint32_t  Reserved0[86];   // 0x028 - 0x17C
  __IO uint32_t  PINASSIGN_4PIN;  // 0x180
  __I  uint32_t  Reserved1[15];   // 0x184 - 0x1BC
  __IO uint32_t  PINENABLE0;      // 0x1C0
  __IO uint32_t  PINENABLE1;      // 0x1C4
} LPC_SWM_TypeDef;




// ------------------------------------------------------------------------------------------------
// -----                      General Purpose I/O (GPIO)                                      -----
// ------------------------------------------------------------------------------------------------
typedef struct {                            
  __IO uint8_t B0[32];            // 0x00 - 0x1F Byte pin registers P0.0 - P0.31
  __IO uint8_t B1[32];            // 0x20 - 0x3F Byte pin registers P1.0 - P1.31
  __I  uint8_t Reserved0[4032];   // 0x40 - 0xFFF
  __IO uint32_t W0[32];           // 0x1000 - 0x107C Word pin registers P0.0 - P0.31
  __IO uint32_t W1[32];           // 0x1080 - 0x10FC Word pin registers P1.0 - P1.31
  __I  uint32_t Reserved1[960];   // 0x1100 - 0x1FFC (960d = 0x3c0)

  union {
    __IO uint32_t DIR[2];         // 0x2000 - 0x2004
    struct {
      __IO uint32_t DIR0;         // 0x2000
      __IO uint32_t DIR1;         // 0x2004
    };
  };

  __I  uint32_t Reserved2[30];    // 0x2008 - 0x207C

  union {
    __IO uint32_t MASK[2];        // 0x2080 - 0x2084
    struct {
      __IO uint32_t MASK0;        // 0x2080
      __IO uint32_t MASK1;        // 0x2084
    };
  };

  __I  uint32_t Reserved3[30];    // 0x2088 - 0x20FC

  union {
  __IO uint32_t PIN[2];           // 0x2100 - 0x2104
    struct {
      __IO uint32_t PIN0;         // 0x2100 
      __IO uint32_t PIN1;         // 0x2104
    };
  };

  __I  uint32_t Reserved4[30];    // 0x2108 - 0x217C

  union {
    __IO uint32_t MPIN[2];        // 0x22180 - 0x2184
    struct {
      __IO uint32_t MPIN0;        // 0x2180
      __IO uint32_t MPIN1;        // 0x2184
    };
  };

  __I  uint32_t Reserved5[30];    // 0x2188 - 0x21FC

  union {
    __IO uint32_t SET[2];         // 0x2200 -0x2204
    struct {
      __IO uint32_t SET0;         // 0x2200
      __IO uint32_t SET1;         // 0x2204
    };
  };

  __I  uint32_t Reserved6[30];    // 0x2208 - 0x227C

  union {
    __O uint32_t CLR[2];          // 0x2280 - 0x2284
    struct {
      __O  uint32_t CLR0;         // 0x2280
      __O  uint32_t CLR1;         // 0x2284
    };
  };

  __I  uint32_t Reserved7[30];    // 0x2288 - 0x22FC

  union {
    __O  uint32_t NOT[2];          // 0x2300 - 0x2304
    struct {
      __O  uint32_t NOT0;          // 0x2300
      __O  uint32_t NOT1;          // 0x2304
    };
  };

  __I  uint32_t Reserved8[30];    // 0x2308 - 0x237C

  union {
    __O uint32_t DIRSET[2];       // 0x2380 - 0x2384
    struct {
      __O  uint32_t DIRSET0;      // 0x2380
      __O  uint32_t DIRSET1;      // 0x2384
    };
  };

  __I  uint32_t Reserved9[30];    // 0x2388 - 0x23FC

  union {
    __O uint32_t DIRCLR[2];       // 0x2400 - 0x2404
    struct {
      __O  uint32_t DIRCLR0;      // 0x2400
      __O  uint32_t DIRCLR1;      // 0x2404
    };
  };

  __I  uint32_t Reserved10[30];   // 0x2408 - 0x247C

  union {
    __O uint32_t DIRNOT[2];       // 0x2480 - 0x2484
    struct {
      __O  uint32_t DIRNOT0;      // 0x2480
      __O  uint32_t DIRNOT1;      // 0x2484
    };
  };

} LPC_GPIO_PORT_TypeDef;




// ------------------------------------------------------------------------------------------------
// ----- Pin Interrupts and Pattern Match  (PIN_INT)                                          -----
// ------------------------------------------------------------------------------------------------
typedef struct {                            /*!< (@ 0xA0004000) PIN_INT Structure */
  __IO uint32_t ISEL;                       /*!< (@ 0xA0004000) Pin Interrupt Mode register */
  __IO uint32_t IENR;                       /*!< (@ 0xA0004004) Pin Interrupt Enable (Rising) register */
  __IO uint32_t SIENR;                      /*!< (@ 0xA0004008) Set Pin Interrupt Enable (Rising) register */
  __IO uint32_t CIENR;                      /*!< (@ 0xA000400C) Clear Pin Interrupt Enable (Rising) register */
  __IO uint32_t IENF;                       /*!< (@ 0xA0004010) Pin Interrupt Enable Falling Edge / Active Level register */
  __IO uint32_t SIENF;                      /*!< (@ 0xA0004014) Set Pin Interrupt Enable Falling Edge / Active Level register */
  __IO uint32_t CIENF;                      /*!< (@ 0xA0004018) Clear Pin Interrupt Enable Falling Edge / Active Level address */
  __IO uint32_t RISE;                       /*!< (@ 0xA000401C) Pin Interrupt Rising Edge register */
  __IO uint32_t FALL;                       /*!< (@ 0xA0004020) Pin Interrupt Falling Edge register */
  __IO uint32_t IST;                        /*!< (@ 0xA0004024) Pin Interrupt Status register */
  __IO uint32_t PMCTRL;                     /*!< (@ 0xA0004028) GPIO pattern match interrupt control register          */
  __IO uint32_t PMSRC;                      /*!< (@ 0xA000402C) GPIO pattern match interrupt bit-slice source register */
  __IO uint32_t PMCFG;                      /*!< (@ 0xA0004030) GPIO pattern match interrupt bit slice configuration register */
} LPC_PIN_INT_TypeDef;




//------------- CRC Engine (CRC) -----------------------------------------
typedef struct
{
  __IO uint32_t MODE;
  __IO uint32_t SEED;
  union {
  __I  uint32_t SUM;
  __O  uint32_t WR_DATA;
  };
} LPC_CRC_TypeDef;




//------------- Comparator (CMP) --------------------------------------------------
typedef struct {                            /*!< (@ 0x40024000) CMP Structure          */
  __IO uint32_t  CTRL;                      /*!< (@ 0x40024000) Comparator control register */
  __IO uint32_t  LAD;                       /*!< (@ 0x40024004) Voltage ladder register */
} LPC_CMP_TypeDef;




//------------- Self Wakeup Timer (WKT) --------------------------------------------------
typedef struct {                            /*!< (@ 0x40028000) WKT Structure          */
  __IO uint32_t  CTRL;                      /*!< (@ 0x40028000) Alarm/Wakeup Timer Control register */
       uint32_t  Reserved[2];
  __IO uint32_t  COUNT;                     /*!< (@ 0x4002800C) Alarm/Wakeup TImer counter register */
} LPC_WKT_TypeDef;




//------------- Multi-Rate Timer (MRT) --------------------------------------------------
typedef struct {
__IO uint32_t INTVAL;        							
__IO uint32_t TIMER;         							
__IO uint32_t CTRL;          							
__IO uint32_t STAT;          							
} MRT_Channel_cfg_Type;

typedef struct {
  MRT_Channel_cfg_Type Channel[4]; 		
  uint32_t Reserved0[45];          // Address offsets = 0x40 - 0xF0
  __IO uint32_t IDLE_CH; 			
  __IO uint32_t IRQ_FLAG; 						
} LPC_MRT_TypeDef;




//------------- USART -----------
typedef struct
{
  __IO uint32_t  CFG;
  __IO uint32_t  CTL;
  __IO uint32_t  STAT;
  __IO uint32_t  INTENSET;
  __O  uint32_t  INTENCLR;
  __I  uint32_t  RXDAT;
  __I  uint32_t  RXDATSTAT;
  __IO uint32_t  TXDAT;
  __IO uint32_t  BRG;
  __I  uint32_t  INTSTAT;
  __IO uint32_t  OSR;
  __IO uint32_t  ADDR;
} LPC_USART_TypeDef;




//------------- SPI -----------------------
typedef struct
{
  __IO uint32_t  CFG;			    /* 0x00 */
  __IO uint32_t  DLY;
  __IO uint32_t  STAT;
  __IO uint32_t  INTENSET;
  __O  uint32_t  INTENCLR;		/* 0x10 */
  __I  uint32_t  RXDAT;
  __IO uint32_t  TXDATCTL;
  __IO uint32_t  TXDAT;
  __IO uint32_t  TXCTL; 		  /* 0x20 */
  __IO uint32_t  DIV;
  __I  uint32_t  INTSTAT;
} LPC_SPI_TypeDef;




//------------- I2C -------------------------------
typedef struct
{
  __IO uint32_t  CFG;			  /* 0x00 */
  __IO uint32_t  STAT;
  __IO uint32_t  INTENSET;
  __O  uint32_t  INTENCLR;
  __IO uint32_t  TIMEOUT;		/* 0x10 */
  union {
    __IO uint32_t  CLKDIV;
    __IO uint32_t  DIV;
  };
  __IO uint32_t  INTSTAT;
       uint32_t  Reserved0[1];  
  __IO uint32_t  MSTCTL;			  /* 0x20 */
  __IO uint32_t  MSTTIME;
  __IO uint32_t  MSTDAT;
       uint32_t  Reserved1[5];
  __IO uint32_t  SLVCTL;			  /* 0x40 */
  __IO uint32_t  SLVDAT;
  __IO uint32_t  SLVADR0;
  __IO uint32_t  SLVADR1;
  __IO uint32_t  SLVADR2;			  /* 0x50 */
  __IO uint32_t  SLVADR3;
  __IO uint32_t  SLVQUAL0;
       uint32_t  Reserved2[9];
  __I  uint32_t  MONRXDAT;			/* 0x80 */		
} LPC_I2C_TypeDef;




//------------------- Standard Counter/Timer (CTIMER) ---------------------
typedef struct {
  __IO uint32_t IR;               // 0x00
  __IO uint32_t TCR;              // 0x04
  __IO uint32_t TC;               // 0x08
  __IO uint32_t PR;               // 0x0C
  __IO uint32_t PC;               // 0x10
  __IO uint32_t MCR;              // 0x14
  __IO uint32_t MR[4];            // 0x18 - 0x24
  __IO uint32_t CCR;              // 0x28
  __IO uint32_t CR[4];            // 0x2C - 0x38
  __IO uint32_t EMR;              // 0x3C
  __I  uint32_t RESERVED0[12];    // 0x40 - 0x6C 
  __IO uint32_t CTCR;             // 0x70
  __IO uint32_t PWMC;             // 0x74
} LPC_TIMER_TypeDef;




//------------- Widowed Watchdog Timer (WWDT) -----------------------------------------
typedef struct
{
  __IO uint32_t MOD;                    /*!< Offset: 0x000 Watchdog mode register (R/W) */
  __IO uint32_t TC;                     /*!< Offset: 0x004 Watchdog timer constant register (R/W) */
  __O  uint32_t FEED;                   /*!< Offset: 0x008 Watchdog feed sequence register (W) */
  __I  uint32_t TV;                     /*!< Offset: 0x00C Watchdog timer value register (R) */
       uint32_t RESERVED;               /*!< Offset: 0x010 RESERVED                          */
  __IO uint32_t WARNINT;                /*!< Offset: 0x014 Watchdog timer warning int. register (R/W) */
  __IO uint32_t WINDOW;                 /*!< Offset: 0x018 Watchdog timer window value register (R/W) */
} LPC_WWDT_TypeDef;




//------------- ADC -----------------------------------------
typedef struct {
  __IO uint32_t CTRL;           // 0x0
       uint32_t RESERVED0;      // 0x4
  __IO uint32_t SEQA_CTRL;      // 0x8
  __IO uint32_t SEQB_CTRL;      // 0xC
  __IO uint32_t SEQA_GDAT;      // 0x10
  __IO uint32_t SEQB_GDAT;      // 0x14
       uint32_t RESERVED1[2];   // 0x18 - 0x1C
  __IO uint32_t DAT[12];        // 0x20 - 0x4C
  __IO uint32_t THR0_LOW;       // 0x50
  __IO uint32_t THR1_LOW;       // 0x54
  __IO uint32_t THR0_HIGH;      // 0x58
  __IO uint32_t THR1_HIGH;      // 0x5C
  __IO uint32_t CHAN_THRSEL;    // 0x60
  __IO uint32_t INTEN;          // 0x64
  __IO uint32_t FLAGS;          // 0x68
  __IO uint32_t TRM;            // 0x6C
} LPC_ADC_TypeDef;




//------------- DAC ----------------
typedef struct {
  __IO uint32_t CR;             // 0x00
  __IO uint32_t CTRL;           // 0x04
  __IO uint32_t CNTVAL;         // 0x08
} LPC_DAC_TypeDef;

//------------- Capacitive Touch module (CAPT) ----------------
typedef struct {
  __IO uint32_t CTRL;        // 0x00
  __IO uint32_t STATUS;      // 0x04
  __IO uint32_t POLL_TCNT;   // 0x08
  __I  uint32_t reserved0;   // 0x0C
  __IO uint32_t INTENSET;    // 0x10
  __O  uint32_t INTENCLR;    // 0x14
  __I  uint32_t INTSTAT;     // 0x18
  __I  uint32_t reserved1;   // 0x1C
  __I  uint32_t TOUCH;       // 0x20
  __I  uint32_t A_hole[1014];// 0x24 - 0xFF8
  __I  uint32_t ID;          // 0xFFC
} LPC_CAPT_TypeDef;

#define	LUT_INs		5
#define PLU_INs		6
#define	PLU_LUTs	26
#define PLU_FFs		4
#define PLU_OUTs	8
	 
typedef struct {                       
	struct {                                        /* Input select register for LUT & Input... */
		union {
			__IO uint32_t INP[8];												/* Each LUT has maximum 5 inputs, the remaining are reserved. */
		  struct {
		    __IO uint32_t INP0;
		    __IO uint32_t INP1;
		    __IO uint32_t INP2;
		    __IO uint32_t INP3;
		    __IO uint32_t INP4;
		    uint32_t RESERVED[3];
		  };
	  } LUT_MUX[PLU_LUTs];
	};
	     uint32_t RESERVED0[8*64-8*PLU_LUTs]; /* ...-0x7FC */ 
  __IO uint32_t LUT_TRUTH[PLU_LUTs];        /* Truth-Table ("Look-up Table") programming */
	     uint32_t RESERVED1[64-PLU_LUTs];     /* ...-0x8FC */
  __I  uint32_t OUTPUTS;                    /* PLU Outputs Register (Read-only) */
	     uint32_t RESERVED2[3*64-1];          /* ...-0xBFC */
  __IO uint32_t OUTPUT_MUX[PLU_OUTs];       /* Select register for PLU Ouptut */
} LPC_PLU_TypeDef;


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
// Base addresses                                                             
#define LPC_FLASH_BASE         (0x00000000UL)
#define LPC_RAM_BASE           (0x10000000UL)
#define LPC_ROM_BASE           (0x1FFF0000UL)
#define LPC_APB0_BASE          (0x40000000UL)
#define LPC_AHB_BASE           (0x50000000UL)

// APB0 peripherals 
#define LPC_WWDT_BASE          (LPC_APB0_BASE + 0x00000)
#define LPC_MRT_BASE           (LPC_APB0_BASE + 0x04000)
#define LPC_WKT_BASE           (LPC_APB0_BASE + 0x08000)
#define LPC_SWM_BASE           (LPC_APB0_BASE + 0x0C000)
//                             (LPC_APB0_BASE + 0x10000)
#define LPC_DAC0_BASE          (LPC_APB0_BASE + 0x14000)
//                             (LPC_APB0_BASE + 0x18000)
#define LPC_ADC_BASE           (LPC_APB0_BASE + 0x1C000)
#define LPC_PMU_BASE           (LPC_APB0_BASE + 0x20000)
#define LPC_CMP_BASE           (LPC_APB0_BASE + 0x24000)
#define LPC_PLU_BASE           (LPC_APB0_BASE + 0x28000)
//                             (LPC_APB0_BASE + 0x2C000)
//                             (LPC_APB0_BASE + 0x30000)
//                             (LPC_APB0_BASE + 0x34000)
#define LPC_CTIMER0_BASE       (LPC_APB0_BASE + 0x38000)
//                             (LPC_APB0_BASE + 0x38000)
#define LPC_FLASHCTRL_BASE     (LPC_APB0_BASE + 0x40000)
#define LPC_IOCON_BASE         (LPC_APB0_BASE + 0x44000)
#define LPC_SYSCON_BASE        (LPC_APB0_BASE + 0x48000)
#define LPC_I2C0_BASE          (LPC_APB0_BASE + 0x50000)
#define LPC_I2C1_BASE          (LPC_APB0_BASE + 0x54000)
#define LPC_SPI0_BASE          (LPC_APB0_BASE + 0x58000)
//                             (LPC_APB0_BASE + 0x5C000)
#define LPC_CAPT_BASE          (LPC_APB0_BASE + 0x60000)
#define LPC_USART0_BASE        (LPC_APB0_BASE + 0x64000)
#define LPC_USART1_BASE        (LPC_APB0_BASE + 0x68000)
//                             (LPC_APB0_BASE + 0x6C000)
//                             (LPC_APB0_BASE + 0x70000)
//                             (LPC_APB0_BASE + 0x74000)

// AHB peripherals
#define LPC_CRC_BASE           (LPC_AHB_BASE + 0x00000)
//                             (LPC_AHB_BASE + 0x04000)
//                             (LPC_AHB_BASE + 0x08000)
//                             (LPC_AHB_BASE + 0x0C000)
//                             (LPC_AHB_BASE + 0x10000)
#define LPC_GPIO_PORT_BASE     (0xA0000000)
#define LPC_PIN_INT_BASE       (LPC_GPIO_PORT_BASE + 0x4000)

/******************************************************************************/
/*                         Peripheral declarations                            */
/******************************************************************************/

#define LPC_WWDT              ((LPC_WWDT_TypeDef   *) LPC_WWDT_BASE  )
#define LPC_MRT               ((LPC_MRT_TypeDef    *) LPC_MRT_BASE   )
#define LPC_WKT               ((LPC_WKT_TypeDef    *) LPC_WKT_BASE   )
#define LPC_SWM               ((LPC_SWM_TypeDef    *) LPC_SWM_BASE   )
#define LPC_DAC0              ((LPC_DAC_TypeDef    *) LPC_DAC0_BASE  )
#define LPC_ADC               ((LPC_ADC_TypeDef    *) LPC_ADC_BASE   )
#define LPC_PMU               ((LPC_PMU_TypeDef    *) LPC_PMU_BASE   )
#define LPC_CMP               ((LPC_CMP_TypeDef    *) LPC_CMP_BASE   )
#define LPC_CTIMER0           ((LPC_TIMER_TypeDef  *) LPC_CTIMER0_BASE  )
#define LPC_FLASHCTRL         ((LPC_FLASHCTRL_TypeDef *) LPC_FLASHCTRL_BASE )
#define LPC_IOCON             ((LPC_IOCON_TypeDef  *) LPC_IOCON_BASE )
#define LPC_SYSCON            ((LPC_SYSCON_TypeDef *) LPC_SYSCON_BASE)
#define LPC_I2C0              ((LPC_I2C_TypeDef    *) LPC_I2C0_BASE  )
#define LPC_I2C1              ((LPC_I2C_TypeDef    *) LPC_I2C1_BASE  )
#define LPC_SPI0              ((LPC_SPI_TypeDef    *) LPC_SPI0_BASE  )
#define LPC_CAPT              ((LPC_CAPT_TypeDef   *) LPC_CAPT_BASE  )
#define LPC_USART0            ((LPC_USART_TypeDef   *) LPC_USART0_BASE )
#define LPC_USART1            ((LPC_USART_TypeDef   *) LPC_USART1_BASE )

#define LPC_CRC               ((LPC_CRC_TypeDef    *) LPC_CRC_BASE   )
#define LPC_GPIO_PORT         ((LPC_GPIO_PORT_TypeDef  *) LPC_GPIO_PORT_BASE  )
#define LPC_PIN_INT           ((LPC_PIN_INT_TypeDef   *) LPC_PIN_INT_BASE  )
#define LPC_PLU0            	((LPC_PLU_TypeDef       *) LPC_PLU_BASE) 




///////////////////////////////////////////////////////////////////////////////
// Other chip-specific macro definitions (a.k.a. the chip.h section)
///////////////////////////////////////////////////////////////////////////////

// ACMP_I-to-IOCON mapping
#define ACMP_I1_PORT PIO0_0
#define ACMP_I2_PORT PIO0_1
#define ACMP_I3_PORT PIO0_14
#define ACMP_I4_PORT PIO0_16
#define ACMP_I5_PORT PIO0_21

// For looping through the pad controls
#define NUM_IOCON_SLOTS 30

#ifdef __cplusplus
}
#endif

#endif  /* __LPC8xx_H__ */
