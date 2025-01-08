/* 

  u8g2_dma_test, 32MHz uC Clock

*/

#include <stdio.h>
#include "stm32l031xx.h"
#include "delay.h"
#include "u8g2.h"

/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0_sw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_gpio_and_delay_stm32l0_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

uint8_t u8x8_byte_stm32l0_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_stm32l0_dma_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*=======================================================================*/
/* global variables */

u8g2_t u8g2;                    // u8g2 object
uint8_t u8g2_x, u8g2_y;         // current position on the screen

volatile unsigned long SysTickCount = 0;


void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
}



void setHSIClock()
{
  
  /* test if the current clock source is something else than HSI */
  if ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) 
  {
    /* enable HSI */
    RCC->CR |= RCC_CR_HSION;    
    /* wait until HSI becomes ready */
    while ( (RCC->CR & RCC_CR_HSIRDY) == 0 )
      ;      
 
    /* enable the HSI "divide by 4" bit */
    RCC->CR |= (uint32_t)(RCC_CR_HSIDIVEN);
    /* wait until the "divide by 4" flag is enabled */
    while((RCC->CR & RCC_CR_HSIDIVF) == 0)
      ;
    
       
    /* then use the HSI clock */
    RCC->CFGR = (RCC->CFGR & (uint32_t) (~RCC_CFGR_SW)) | RCC_CFGR_SW_HSI; 
    
    /* wait until HSI clock is used */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
      ;
  }
  
  /* disable PLL */
  RCC->CR &= (uint32_t)(~RCC_CR_PLLON);
  /* wait until PLL is inactive */
  while((RCC->CR & RCC_CR_PLLRDY) != 0)
    ;

  /* set latency to 1 wait state */
  FLASH->ACR |= FLASH_ACR_LATENCY;
  
  /* At this point the HSI runs with 4 MHz */
  /* Multiply by 16 device by 2 --> 32 MHz */
  RCC->CFGR = (RCC->CFGR & (~(RCC_CFGR_PLLMUL| RCC_CFGR_PLLDIV ))) | (RCC_CFGR_PLLMUL16 | RCC_CFGR_PLLDIV2); 
  
  /* enable PLL */
  RCC->CR |= RCC_CR_PLLON; 
  
  /* wait until the PLL is ready */
  while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    ;

  /* use the PLL has clock source */
  RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); 
  /* wait until the PLL source is active */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) 
    ;

  SystemCoreClockUpdate();				/* Update SystemCoreClock global variable */  
}

/*
  Enable several power regions: PWR, GPIOA

  This must be executed after each reset.
*/
void startUp(void)
{  
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  RCC->IOPENR |= RCC_IOPENR_IOPBEN;		/* Enable clock for GPIO Port B */
  
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;	/* enable power interface (PWR) */
  PWR->CR |= PWR_CR_DBP;				/* activate write access to RCC->CSR and RTC */  

  SysTick->LOAD = (SystemCoreClock/1000)*10 - 1;   /* 10ms task */
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */      
}

/*=======================================================================*/
/* u8x8 display procedures */

/*
    SPI clock ~500KHz ?
    uc1609_slg19264_f   2Mhz  SW SPI           0.3 FPS         BSS 1692
    uc1609_slg19264_f   2Mhz  HW SPI          4.7 FPS         BSS 1692
    uc1609_slg19264_f   2Mhz  DMA SPI          5.0 FPS        BSS 1948
    uc1609_slg19264_1  2Mhz   SW SPI          0.3 FPS         BSS 348
    uc1609_slg19264_1  2Mhz   HW SPI          2.6 FPS         BSS 348
    uc1609_slg19264_1  2Mhz   DMA SPI          2.6 FPS        BSS 604

    SPI clock ~6MHz ?
    uc1609_slg19264_f   32Mhz  SW SPI          6.0 FPS         BSS 1692
    uc1609_slg19264_f   32Mhz  HW SPI          73,8 FPS         BSS 1692
    uc1609_slg19264_f   32Mhz  DMA SPI         76.7 FPS        BSS 1948   .
    uc1609_slg19264_1  32Mhz   SW SPI          5.6 FPS         BSS 348
    uc1609_slg19264_1  32Mhz   HW SPI          39.6 FPS         BSS 348
    uc1609_slg19264_1  32Mhz   DMA SPI         40.7 FPS        BSS 604
*/

void initDisplay(void)
{
  /* setup display */
  //u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R2, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0_sw_i2c);

  //u8g2_Setup_uc1609_slg19264_1(&u8g2, U8G2_R2, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_stm32l0_spi);
  
  //u8g2_Setup_uc1609_slg19264_1(&u8g2, U8G2_R2, u8x8_byte_stm32l0_hw_spi, u8x8_gpio_and_delay_stm32l0_spi);

  u8g2_Setup_uc1609_slg19264_f(&u8g2, U8G2_R2, u8x8_byte_stm32l0_dma_spi, u8x8_gpio_and_delay_stm32l0_spi);
  
  
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
}

void outChar(uint8_t c)
{
  u8g2_x+=u8g2_DrawGlyph(&u8g2, u8g2_x, u8g2_y, c);
}

void outStr(const char *s)
{
  while( *s )
    outChar(*s++);
}

void outHexHalfByte(uint8_t b)
{
  b &= 0x0f;
  if ( b < 10 )
    outChar(b+'0');
  else
    outChar(b+'a'-10);
}

void outHex8(uint8_t b)
{
  outHexHalfByte(b >> 4);
  outHexHalfByte(b);
}

void outHex16(uint16_t v)
{
  outHex8(v>>8);
  outHex8(v);
}

void outHex32(uint32_t v)
{
  outHex16(v>>16);
  outHex16(v);
}

void setRow(uint8_t r)
{
  u8g2_x = 0;
  u8g2_y = r;
}

/*=======================================================================*/

int main()
{
  uint8_t v = 0;
  uint32_t frame_cnt = 0;
  uint32_t start, diff, fps, fps_frac;
  setHSIClock();        /* enable 32 MHz Clock */
  //SystemCoreClockUpdate();
  startUp();               /* enable systick irq and several power regions  */
  initDisplay();          /* aktivate display */

  GPIOA->MODER &= ~GPIO_MODER_MODE9;	/* clear mode */
  GPIOA->MODER |= GPIO_MODER_MODE9_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;	/* no open drain */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED9;	/* low speed */
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED9_1;	/* 10 MHz */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_9;		/* atomic clear */
  
  
  frame_cnt = 0;
  fps = 0;
  fps_frac = 0;
  start = SysTickCount;
  for(;;)
  {
    diff = SysTickCount - start;
    if ( diff > 0 )
    {
      fps = frame_cnt*1000 / diff;              // diff are 10ms ticks
      fps_frac = fps % 10;
      fps /= 10;
    }
    GPIOA->BSRR = GPIO_BSRR_BR_9;		/* atomic clear */
    
    //u8g2_SetContrast(&u8g2, v);
    
    //u8g2_ClearBuffer(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_SetFont(&u8g2, u8g2_font_6x12_tf);
      u8g2_DrawStr(&u8g2, 0,12, "STM32L031");
      u8g2_DrawStr(&u8g2, 0,24, u8x8_u8toa(SystemCoreClock/1000000, 2));
      u8g2_DrawStr(&u8g2, 20,24, "MHz");
      
      u8g2_SetFont(&u8g2, u8g2_font_9x15B_tf);
      u8g2_DrawStr(&u8g2, 0,40, u8x8_u8toa(v, 3));

      u8g2_DrawStr(&u8g2, 0,60, "FPS:");
      u8g2_DrawStr(&u8g2, 35,60, u8x8_u8toa( fps, 3));
      u8g2_DrawStr(&u8g2, 35+3*8,60, ".");
      u8g2_DrawStr(&u8g2, 35+3*8+6,60, u8x8_u8toa( fps_frac, 1));
    } while( u8g2_NextPage(&u8g2));
    
    GPIOA->BSRR = GPIO_BSRR_BS_9;		/* atomic clear */
    
    //u8g2_SendBuffer(&u8g2);
    v++;
    frame_cnt++;
  }
  return 0;
}
