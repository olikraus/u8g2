
#include <chip.h>
#include <delay.h>
#include "u8g2.h"


/*=======================================================================*/
/* Configuration */
#define SYS_TICK_PERIOD_IN_MS 100


/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_lpc824(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*=======================================================================*/
/* system procedures and sys tick master task */



volatile uint32_t sys_tick_irq_cnt=0;


void __attribute__ ((interrupt)) SysTick_Handler(void)
{  
  sys_tick_irq_cnt++;
}

/*=======================================================================*/

u8g2_t u8g2;

void draw(u8g2_t *u8g2)
{
    u8g2_SetFontMode(u8g2, 1);	// Transparent
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 0, 30, "U");
    
    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");
        
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");
    
    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);
  
    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
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

  /* setup display */
  u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_lpc824);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);

  
  for(;;)
  {
    //Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 0, 15);
    //delay_micro_seconds(1000000);

    u8g2_FirstPage(&u8g2);
    do
    {
      draw(&u8g2);
    } while( u8g2_NextPage(&u8g2) );
    
    //Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 0, 15);
    delay_micro_seconds(1000000);
  }
}