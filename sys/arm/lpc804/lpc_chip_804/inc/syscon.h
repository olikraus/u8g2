/*
 * lpc8xx_syscon.h
 *
 *  Created on: 
 *      Author: 
 */

#include <stdint.h>

#ifndef LPC8XX_SYSCON_H_
#define LPC8XX_SYSCON_H_


// SYSAHBCLKCTRL0 register bits
#define ROM       (1<<1)
#define RAM0_1    (1<<2)
#define FLASH     (1<<4)
#define I2C0      (1<<5)
#define GPIO      (1<<6)
#define GPIO0     (1<<6)
#define SWM       (1<<7)
#define WKT       (1<<9)
#define MRT       (1<<10)
#define SPI0      (1<<11)
#define CRC       (1<<13)
#define UART0     (1<<14)
#define UART1     (1<<15)
#define WWDT      (1<<17)
#define IOCON     (1<<18)
#define ACMP      (1<<19)
#define GPIO1     (1<<20)
#define I2C1      (1<<21)
#define ADC       (1<<24)
#define CTIMER0   (1<<25)
#define DAC0      (1<<27)
#define GPIO_INT  (1<<28)

// SYSAHBCLKCTRL1 register bits
#define CAPT      (1<<0)
#define PLU       (1<<5)

// SYSAHBCLKCTRL[] register bits (alternate form)
typedef enum {
  CLK_ROM = 1   ,
  CLK_RAM0_1    ,
  CLK_FLASH = 4 ,
  CLK_I2C0      ,
  CLK_GPIO0     ,
  CLK_SWM       ,
  CLK_WKT = 9   ,
  CLK_MRT       ,
  CLK_SPI0      ,
  CLK_CRC = 13  ,
  CLK_UART0     ,
  CLK_UART1     ,
  CLK_WWDT = 17 ,
  CLK_IOCON     ,
  CLK_ACMP      ,
  CLK_I2C1 = 21 ,
  CLK_ADC = 24  ,
  CLK_CTIMER0   ,
  CLK_DAC0 = 27 ,
  CLK_GPIO_INT  ,

  CLK_CAPT = 32,
  CLK_PLU = 37      
} CHIP_SYSCON_CLOCK_CTRL_T;



// PRESETCTRL0 register bits
#define FLASH_RST_N   ~(1<<4)
#define I2C0_RST_N    ~(1<<5)
#define GPIO0_RST_N   ~(1<<6)
#define SWM_RST_N     ~(1<<7)
#define WKT_RST_N     ~(1<<9)
#define MRT_RST_N     ~(1<<10)
#define SPI0_RST_N    ~(1<<11)
#define CRC_RST_N     ~(1<<13)
#define UART0_RST_N   ~(1<<14)
#define UART1_RST_N   ~(1<<15)
#define IOCON_RST_N   ~(1<<18)
#define ACMP_RST_N    ~(1<<19)
#define GPIO1_RST_N   ~(1<<20)
#define I2C1_RST_N    ~(1<<21)
#define ADC_RST_N     ~(1<<24)
#define CTIMER0_RST_N ~(1<<25)
#define DAC0_RST_N    ~(1<<27)
#define GPIOINT_RST_N ~(1<<28)

// PRESETCTRL1 register bits
#define CAPT_RST_N    ~(1<<0)
#define FRG0_RST_N    ~(1<<3)
#define PLU_RST_N     ~(1<<5)

// PRESETCTRL[] register bits (alternate form)
typedef enum {
  RESET_FLASH = 4 ,
  RESET_I2C0      ,
  RESET_GPIO0     ,
  RESET_SWM       ,
  RESET_WKT = 9   ,
  RESET_MRT       ,
  RESET_SPI0      ,
  RESET_CRC = 13  ,
  RESET_UART0     ,
  RESET_UART1     ,
  RESET_IOCON = 18,
  RESET_ACMP      ,
  RESET_I2C1 = 21 ,
  RESET_ADC = 24  ,
  RESET_CTIMER0   ,
  RESET_DAC0 = 27 ,
  RESET_GPIO_INT  ,

  RESET_CAPT = 32 ,
  RESET_FRG0 = 35 ,
  RESET_PLU = 37   
} CHIP_SYSCON_PERIPH_RESET_T;



// STARTERP0 register bits
#define PINT0_WAKEUP      (1<<0)
#define PINT1_WAKEUP      (1<<1)
#define PINT2_WAKEUP      (1<<2)
#define PINT3_WAKEUP      (1<<3)
#define PINT4_WAKEUP      (1<<4)
#define PINT5_WAKEUP      (1<<5)
#define PINT6_WAKEUP      (1<<6)
#define PINT7_WAKEUP      (1<<7)

// STARTERP1 register bits
#define SPI0_INT_WAKEUP   (1<<0)
#define USART0_INT_WAKEUP (1<<3)
#define USART1_INT_WAKEUP (1<<4)
#define I2C1_INT_WAKEUP   (1<<7)
#define I2C0_INT_WAKEUP   (1<<8)
#define WWDT_INT_WAKE     (1<<12)
#define BOD_INT_WAKE      (1<<13)
#define WKT_INT_WAKEUP    (1<<15)

// PDAWAKECFG and PDRUNCFG register bits
#define FROOUT_PD         (1<<0)
#define FRO_PD            (1<<1)
#define FLASH_PD          (1<<2)
#define BOD_PD            (1<<3)     // Also for PDSLEEPCFG
#define ADC_PD            (1<<4)
#define LPOSC_PD          (1<<6)     // Also for PDSLEEPCFG
#define DAC0_PD           (1<<13)
#define ACMP_PD           (1<<15)

// LPOSCCLKEN register bit field shifters
#define WWDT_CLK_EN 0
#define WKT_CLK_EN  1

// BODCTRL register bit field shifters
#define BODRSTLEV 0
#define BODINTVAL 2
#define BODRSTENA 4


// Below entries are for clock select mux functions
typedef enum {
  FCLKSEL_FRO_CLK     = 0,
  FCLKSEL_MAIN_CLK    = 1,
  FCLKSEL_FRG0CLK     = 2,
  FCLKSEL_FRO_DIV_CLK = 4,
  FCLKSEL_OFF         = 7
} SYSCON_FCLKSEL_CLK_T;

typedef enum {
  FCLK_TO_UART0 = 0,
  FCLK_TO_UART1 = 1,
  FCLK_TO_I2C0  = 5,
  FCLK_TO_I2C1  = 6,
  FCLK_TO_SPI0  = 9
} SYSCON_FCLKSEL_T;

typedef enum {
  FRGCLKSEL_FRO_CLK  = 0,
  FRGCLKSEL_MAIN_CLK = 1,
  FRGCLKSEL_OFF      = 3
} SYSCON_FRGCLKSEL_T;

typedef enum {
  CLKOUTSEL_FRO_CLK      = 0,
  CLKOUTSEL_MAIN_CLK     = 1,
  CLKOUTSEL_EXTERNAL_CLK = 3,
  CLKOUTSEL_LPOSC_CLK    = 4,
  CLKOUTSEL_OFF          = 7
} SYSCON_CLKOUTSEL_T;

typedef enum {
  ADCCLKSEL_FRO_CLK      = 0,
  ADCCLKSEL_EXTERNAL_CLK = 1,
  ADCCLKSEL_OFF          = 3
} SYSCON_ADCCLKSEL_T;











void Enable_Periph_Clock(uint32_t slot);
void Disable_Periph_Clock(uint32_t slot);
void Do_Periph_Reset(uint32_t slot);
//void Config_Syspll(unsigned int which_clock, unsigned int pll_ctrl_value);
void Config_Fclksel_Mux(uint32_t to_which_periph, uint32_t which_clock);





#endif /* LPC8XX_SYSCON_H_ */
