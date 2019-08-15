/**************************************************************************//**
 * @file     system.c
 * @brief    CMSIS Device System Source File for
 *           NXP LPC80xm Device Series
 * @version  V1.0
 * @date     01. April 2017
 *
  ******************************************************************************/

//----------------------------------------------------------------------------
// Important!
// Please configure the desired initial clock setup for your project in:
// $proj_name/inc/chip_setup.h
//----------------------------------------------------------------------------


#include <stdint.h>
#include "LPC8xx.h"
#include "swm.h"
#include "syscon.h"
#include "iocon.h"
#include "fro.h"
#include "rom_api.h"
#include "chip_setup.h"


//----------------------------------------------------------------------------
// Validate the the user's selctions
//----------------------------------------------------------------------------
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
#define CHECK_RSVD(val, mask)                     (val & mask)

#if (CHECK_RANGE((FRO_FREQ_VAL), 0, 2))
   #error "FRO_FREQ_VAL: Value out of range."
#endif

#if (CHECK_RSVD((MAINCLKSEL_VAL),  ~0x00000003))
   #error "MAINCLKSEL: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((SYSAHBCLKDIV_VAL), 0, 255))
   #error "SYSAHBCLKDIV: Value out of range!"
#endif

#if (CHECK_RANGE(CLKIN_CLK_VAL, 1000000, 25000000))
   #error "CLKIN frequency is out of bounds"
#endif



//----------------------------------------------------------------------------
//  Calculate internal clock node frequency initial values
//----------------------------------------------------------------------------
#define __LPOSC_CLK    (1000000)

// determine output of the FRO_CLKDIV subsystem
#if FRO_FREQ_VAL == 0
  #define __FRO_OSCOUT (18000000)
#elif FRO_FREQ_VAL == 2
  #define __FRO_OSCOUT (30000000)
#else
  #define __FRO_OSCOUT (24000000)
#endif

#define __FRO_DIVIDERS_OUT (__FRO_OSCOUT / 2)

#define __FRO_CLK __FRO_DIVIDERS_OUT

#define  __FRO_DIV_CLK (__FRO_CLK / 2)

// determine external_clk
#define __SYS_OSC_CLK  (0)
#define __CLKIN_CLK    (CLKIN_CLK_VAL)
#define __EXTERNAL_CLK  __CLKIN_CLK

// determine main_clk
#if MAINCLKSEL_VAL == 0
  #define __MAIN_CLK       __FRO_CLK
#elif MAINCLKSEL_VAL == 1
  #define __MAIN_CLK       __EXTERNAL_CLK
#elif MAINCLKSEL_VAL == 2
  #define __MAIN_CLK       __LPOSC_CLK
#else
  #define __MAIN_CLK       __FRO_DIV_CLK
#endif

// determine system_ahb_clk
#define __SYSTEM_AHB_CLK     (__MAIN_CLK / SYSAHBCLKDIV_VAL)



//----------------------------------------------------------------------------
// Function name: SystemInit
// Sets up the initial chip clocking based on MACROs defined in chip_setup.h.
//----------------------------------------------------------------------------
void SystemInit (void) {

  uint32_t i;

  for (i = 1; i < 1; i++) __NOP();                    // To avoid a warning if variable i is unused

  // Enable clocks to IOCON and SWM upon entry, disable them upon exit
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (SWM | IOCON);

  // Step 0. Configure the FRO subsystem (choose the source for clocks fro and fro_div)
  #if (FRO_FREQ_VAL == 0)
    //temp |= (FRO_18MHZ << FRO_FREQ_SEL);
    LPC_PWRD_API->set_fro_frequency(18000);
  #elif (FRO_FREQ_VAL == 2)
    //temp |= (FRO_30MHZ << FRO_FREQ_SEL);
    LPC_PWRD_API->set_fro_frequency(30000);
  #else
    //temp |= (FRO_24MHZ << FRO_FREQ_SEL);
    LPC_PWRD_API->set_fro_frequency(24000);
  #endif
//temp = LPC_SYSCON->FROOSCCTRL;                      // Get the current register contents
//temp &= ~(FRO_FREQSEL_MASK);                        // Preserve all but the bits of interest [1:0]
///PC_SYSCON->FROOSCCTRL = temp;                      // Update the actual register
//LPC_SYSCON->FRODIRECTCLKUEN = 0;                    // Toggle the update register for the output mux
//LPC_SYSCON->FRODIRECTCLKUEN = 1;
//while (!(LPC_SYSCON->FRODIRECTCLKUEN & 1)) __NOP(); // Wait for update to take effect
    
  // Configure clk_in, if needed for main_clk or other (e.g. clock out, ADC clk.)
  #if ((MAINCLKSEL_VAL == 1) || (EXT_CLOCK_FORCE_ENABLE == 1))
    LPC_IOCON->PIO0_1           &= (IOCON_MODE_MASK | MODE_INACTIVE); 	// Disable pull-up and pull-down
    LPC_SWM->PINENABLE0         &= ~(CLKIN);          // Enable CLKIN func.
  #endif
  
  // Step 2. Power up the LP OSC if it's needed for main_clk
  #if (MAINCLKSEL_VAL == 2)
    LPC_SYSCON->PDRUNCFG        &= ~(LPOSC_PD);       // Power up the LP OSC
    for (i = 0; i < 200; i++) __NOP();                // Wait for osc to stabilize
  #endif

  // Step 4. Choose source for main_clk
  LPC_SYSCON->MAINCLKSEL        = MAINCLKSEL_VAL;     // Update the actual register
  LPC_SYSCON->MAINCLKUEN        = 0;                  // Toggle update register
  LPC_SYSCON->MAINCLKUEN        = 1;
  while (!(LPC_SYSCON->MAINCLKUEN & 1)) __NOP();      // Wait until updated

  // Step 6. Configure the main_clock divider
  LPC_SYSCON->SYSAHBCLKDIV      =  SYSAHBCLKDIV_VAL;  // Update the actual register

  LPC_SYSCON->SYSAHBCLKCTRL[0] &= ~(SWM | IOCON);     // Turn off peripheral clocks before leaving

} // end of SystemInit




//----------------------------------------------------------------------------
//  Global clock variable declarations and initial value assignments
//----------------------------------------------------------------------------
uint32_t main_clk        = __MAIN_CLK;
uint32_t lposc_clk       = __LPOSC_CLK;
uint32_t fro_clk         = __FRO_CLK;
uint32_t fro_div_clk     = __FRO_DIV_CLK;
uint32_t system_ahb_clk  = __SYSTEM_AHB_CLK;

//----------------------------------------------------------------------------
// Function name: SystemCoreClockUpdate
// Determines the actual system_ahb_clk (core clock), main_clock, 
// fro_clk, and fro_div_clk frequencies
// based on the current state of the device, and updates the associated 
// global clock variables.
//----------------------------------------------------------------------------
void SystemCoreClockUpdate (void)
{
  uint32_t external_clk;
  uint32_t fro_oscout, fro_clock;
  uint32_t temp;

  // Set the fro_clk and fro_div_clk variables according to current register settings
  temp = LPC_SYSCON->FROOSCCTRL;
  switch (temp & FRO_FREQSEL_MASK) {
    case 0: fro_oscout = 18000000; break;
    case 1: fro_oscout = 24000000; break;
    default:fro_oscout = 30000000; break;
  }
  fro_clock = 0;
  if ((LPC_SYSCON->PDRUNCFG & (FROOUT_PD | FRO_PD)) == 0x0) {
    fro_clock = fro_oscout;
  }
  fro_clk = fro_clock / 2;
  fro_div_clk = fro_clk / 2;

  // Set the external_clk variable according to current register values
//if (LPC_SYSCON->EXTCLKSEL == 0)
//  external_clk = __SYS_OSC_CLK;
//else
    external_clk = __CLKIN_CLK;

  // Set the lposc_clk variable 
  if (!(LPC_SYSCON->PDRUNCFG & LPOSC_PD)) {
    lposc_clk = __LPOSC_CLK;
  }
  else {
    lposc_clk = 0;
  }

  // Set the main_clk variable according to current register values
  switch (LPC_SYSCON->MAINCLKSEL & 0x3) {
    case 0:  main_clk = fro_clk;      break;
    case 1:  main_clk = external_clk; break;
    case 2:  main_clk = lposc_clk;    break;
    case 3:  main_clk = fro_div_clk;  break;
  }

  // Set the system_ahb_clk (a.k.a SystemCoreClock) variable according to current register values
  system_ahb_clk = main_clk / LPC_SYSCON->SYSAHBCLKDIV;

} // end of SystemCoreClockUpdate
