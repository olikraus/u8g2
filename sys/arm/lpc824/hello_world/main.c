
#include <chip.h>
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
  Chip_SystemInit(); 

  /* if the clock or PLL has been changed, also update the global variable SystemCoreClock */
  SystemCoreClockUpdate();

  /* set systick and start systick interrupt */
  SysTick_Config(SystemCoreClock/1000UL*(unsigned long)SYS_TICK_PERIOD_IN_MS);
  
  /* enable clock for several subsystems */
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON | SYSCTL_CLOCK_GPIO | SYSCTL_CLOCK_SWM);

  /* turn on GPIO */
  Chip_GPIO_Init(LPC_GPIO_PORT);	/* Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GPIO); */
  

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