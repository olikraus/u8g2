/*

  main.c
  
  PLU example
  
  switch at PIO0_02 and PIO0_10
  LED at PIO0_09 and PIO0_15
  
*/


#include <LPC8xx.h>
#include <syscon.h>
#include <gpio.h>
#include <delay.h>


void plu(void);

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
config: 0
PIO0_2 --> PLUINPUT2 --> LUT4_INP0
PIO0_10 --> IN0 --> OUT0 --> PIN0_22 --> PLUINPUT5 --> LUT4_INP1

Cfg | PIO10 PIO2 | result
1 | 0 0 | 1

Cfg | PIO10 PIO2 | result
2 | 0 1 | 1				falsch, sieht nach 2 | 1 0 | 1 aus (reihenfolge falsch???)

Cfg | PIO10 PIO2 | result
3 | 0 1 | 1				
3 | 1 1 | 1				

Config=1
PIO10 PIO2 | result
0 0 | 1			--> ok
0 1 | 0
1 0 | 0
1 1 | 0

Config=1
PIO10 PIO2 | result
0 0 | 0			--> ok
0 1 | 1
1 0 | 0
1 1 | 0



*/
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
  

  GPIOSetDir( PORT0, 15, OUTPUT);
  GPIOSetDir( PORT0, 9, OUTPUT);
  GPIOSetDir( PORT0, 2, INPUT);
  
  plu();

  for(;;)
  {
    GPIOSetBitValue(PORT0, 15, 1);
    delay_micro_seconds(100000);
    GPIOSetBitValue(PORT0, 15, 0);
    delay_micro_seconds(1000000);
  }
}
