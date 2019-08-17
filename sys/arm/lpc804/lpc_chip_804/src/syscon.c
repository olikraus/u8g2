//
// syscon.c
//

#include "syscon.h"
#include "LPC8xx.h"




// Function: Enable_Periph_Clock
// Use like this: Enable_Periph_Clock(CLK_SPI0);
//
void Enable_Periph_Clock(uint32_t slot) {
  if (slot < 32) {
    LPC_SYSCON->SYSAHBCLKCTRL[0] |= 1<<slot;
  }
  else if (slot < 64) {
      LPC_SYSCON->SYSAHBCLKCTRL[1] |= 1<<(slot-32);
  }
}




// Function: Disable_Periph_Clock
// Use like this: Disable_Periph_Clock(CLK_SPI0);
//
void Disable_Periph_Clock(uint32_t slot) {
  if (slot < 32) {
    LPC_SYSCON->SYSAHBCLKCTRL[0] &= ~(1<<slot);
  }
  else if (slot < 64) {
      LPC_SYSCON->SYSAHBCLKCTRL[1] &= ~(1<<(slot-32));
  }
}




// Function: Do_Periph_Reset
// Use like this: Do_Periph_Reset(RESET_SPI0);
//
void Do_Periph_Reset(uint32_t slot) {
  if (slot < 32) {
    LPC_SYSCON->PRESETCTRL[0] &= ~(1<<slot);
    LPC_SYSCON->PRESETCTRL[0] |=  (1<<slot);
  }
  else if (slot < 64) {
      LPC_SYSCON->PRESETCTRL[1] &= ~(1<<(slot-32));
      LPC_SYSCON->PRESETCTRL[1] |=  (1<<(slot-32));
  }
}




// Function: Config_Syspll
// Use like this: Config_Syspll(SYSPLL_CLKSEL_EXTERNAL_CLK, 0x42);
//
//void Config_Syspll(unsigned int which_clock, unsigned int pll_ctrl_value) {
//  LPC_SYSCON->SYSPLLCLKSEL = which_clock;             // Select the input to the system PLL (sys_pll0_clk_src_i)
//  LPC_SYSCON->SYSPLLCLKUEN    = 0;                    // Toggle update register
//  LPC_SYSCON->SYSPLLCLKUEN    = 1;
//  while (!(LPC_SYSCON->SYSPLLCLKUEN & 1)) __NOP();    // Wait until updated
//  LPC_SYSCON->SYSPLLCTRL      = pll_ctrl_value;       // Update the SYSPLLCTRL register
//  LPC_SYSCON->PDRUNCFG        &= ~(SYSPLL_PD);        // Power up system PLL
//  while (!(LPC_SYSCON->SYSPLLSTAT & 1)) __NOP();      // Wait until PLL locked
//}




// Function: Config_Fclksel_Mux
// Use like this: Config_Fclksel_Mux(FCLK_TO_UART0, FCLKSEL_MAIN_CLK);
//
void Config_Fclksel_Mux(uint32_t to_which_periph, uint32_t which_clock) {
  LPC_SYSCON->FCLKSEL[to_which_periph] = which_clock;
}
