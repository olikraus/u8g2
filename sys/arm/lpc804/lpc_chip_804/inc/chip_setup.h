#include "board.h"


//
// The following parameters need to be defined for each project's inital clock setup (used in system.c))
// 
#define FRO_FREQ_VAL 2             // 0 = 18 MHz
                                   // 1 = 24 MHz (reset value)
                                   // 2 = 30 MHz

#define MAINCLKSEL_VAL 0           // 00 = fro (reset value)
                                   // 01 = external_clk
                                   // 10 = lposc_clk
                                   // 11 = fro_div

#define SYSAHBCLKDIV_VAL 1         // 0x00 = system_ahb_clk disabled (use with caution)
                                   // 0x01 = divide_by_1 (reset value)
                                   // 0x02 = divide_by_2
                                   // 0xFF = divide_by_255

#define CLKIN_CLK_VAL 12000000     // External Clock (CLKIN) frequency [Hz] must be in the range of  1 MHz to  25 MHz     

#define EXT_CLOCK_FORCE_ENABLE 0   // Force config. and enable of external_clk for use by other than main_clk
                                   // 0 = external_clk will be configured and enabled only if needed by main_clk or sys_pll0_clk.
                                   // 1 = external_clk will be configured and enabled (available for other, e.g. clock out).

// End of clocks configuration section










//
// The following parameters need to be defined for projects that use the debug UART (used in serial.c)
//
#define DBGUART 0                  // Choose the index for the debug UART (0 for UART0, 1 for UART1, etc.)
#define DBGBAUDRATE 9600           // Choose the baud rate for the debug UART
#define USE_VCOM_PORT 1            // '1' to use VCOM serial port, '0' to use user-defined port pins for debug UART

#if (USE_VCOM_PORT == 1)
  #define DBGTXPIN TARGET_TX       // For VCOM serial port (see board.h)
  #define DBGRXPIN TARGET_RX       // For VCOM serial port (see board.h)
#else
  #define DBGTXPIN P0_15           // Use with USB-to-RS232 break-out cable (choose your own favorite TxD pin)
  #define DBGRXPIN P0_14          // Use with USB-to-RS232 break-out cable (choose your own favorite RxD pin)
#endif

//
// The following are so the debug UART is selectable from any UART on the device (used in Serial.c)
//
#define __CONCAT(x,y,z) x##y##z
#define __XCONCAT(x,y,z) __CONCAT(x,y,z)
 
#define INDEX   DBGUART
#define pDBGU   __XCONCAT(LPC_USART,INDEX,)
#define DBGU    __XCONCAT(UART,INDEX,)
#define DBGUTXD __XCONCAT(U,INDEX,_TXD)
#define DBGURXD __XCONCAT(U,INDEX,_RXD)
#define DBGURST __XCONCAT(UART,INDEX,_RST_N)
#define DBGUIRQ __XCONCAT(UART,INDEX,_IRQn)

