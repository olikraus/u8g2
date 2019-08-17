
#include <LPC8xx.h>
#include <syscon.h>
#include <gpio.h>
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
  
  /* */
  GPIOInit();
  
  /* enable clock for several subsystems */

  Enable_Periph_Clock(CLK_IOCON);
  Enable_Periph_Clock(CLK_SWM);
  

  //Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 15, IOCON_FUNC1);	/* RxD */  
  Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 0, 15);	

  for(;;)
  {
    Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 0, 15);
    delay_micro_seconds(1000000);
    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 0, 15);
    delay_micro_seconds(1000000);
  }
}
