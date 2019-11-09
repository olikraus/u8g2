/*
  use ctimer to do a LED blink
*/


#include <LPC8xx.h>
#include <iocon.h>
#include <syscon.h>
#include <gpio.h>
#include <swm.h>
#include <ctimer.h>
#include <delay.h>


/*=======================================================================*/
/* Configuration */
#define SYS_TICK_PERIOD_IN_MS 100


/*=======================================================================*/
/* system procedures and sys tick master task */



volatile uint32_t sys_tick_irq_cnt=0;


void __attribute__ ((interrupt)) SysTick_Handler(void)
{  
  sys_tick_irq_cnt++;
}

/*=======================================================================*/
/* 
  replacement for ConfigSWM(uint32_t func, uint32_t port_pin) 
  
  Args:
    fn: A function number, e.g. T0_MAT0, see swm.h
    port: A port number for the GPIO port (0..30)

*/
void mapFunctionToPort(uint32_t fn, uint32_t port)
{
  /* first reset the pin assignment to 0xff (this is also the reset value */
  LPC_SWM->PINASSIGN[fn/4] |= ((0xffUL)<<(8*(fn%4)));
  /* then write the destination pin to it */
  LPC_SWM->PINASSIGN[fn/4] &= ~((port^255UL)<<(8*(fn%4)));
}


/*=======================================================================*/
/*
  setup the hardware and start interrupts.
  called by "Reset_Handler"
*/
int __attribute__ ((noinline)) main(void)
{

  /* call to the lpc lib setup procedure. This will set the IRC as clk src and main clk to 24 MHz */
  SystemInit(); 

  /* if the clock or PLL has been changed, also update the global variable SystemCoreClock */
  SystemCoreClockUpdate();

  /* set systick and start systick interrupt */
  SysTick_Config(main_clk/1000UL*(unsigned long)SYS_TICK_PERIOD_IN_MS);
  
  /* init GPIOs */
  GPIOInit();
  
  /* enable clock for several subsystems */
  Enable_Periph_Clock(CLK_IOCON);
  Enable_Periph_Clock(CLK_SWM);
  Enable_Periph_Clock(CLK_CTIMER0);
  
  GPIOSetDir( PORT0, 15, OUTPUT);
  
  LPC_CTIMER0->PR = 0;	/* no prescale */
  LPC_CTIMER0->MR[3] = 15000000;  /* PWM cycle length: one second with 15MHz AHB clock */
  LPC_CTIMER0->MCR |= 1<<MR3R;		/* Use MR3 value to reset the counter: MR3 is the upper value and sets the PWM cycle */
  
  LPC_CTIMER0->MR[0] = 14000000;  /* PWM duty cycle in MR0 */
  LPC_CTIMER0->PWMC |= 1<<PWMEN0;  /* PWM mode for MR0 */
  
  /* invert output, will only work with PIO0_30, not with PIO0_15 */
  LPC_IOCON->PIO0_30 |= 1<<IOCON_INV;  

  /* connect subsystems to the GPIOs */
  /* Just for testing: The signal is routed via PIN0_30 */
  mapFunctionToPort(T0_MAT0, 30);
  mapFunctionToPort(LVLSHFT_IN0, 30);  
  mapFunctionToPort(LVLSHFT_OUT0, 15);


  /* enable the timer */
  LPC_CTIMER0->TCR |= 1<<CEN;		

}
