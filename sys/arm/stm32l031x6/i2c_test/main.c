/* STM32L031 Eval Board: I2C Test */

#include "stm32l031xx.h"
#include "delay.h"
#include "u8x8.h"

/*=======================================================================*/
/* external functions */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*=======================================================================*/
/* global variables */
u8x8_t u8x8;                    // u8x8 object
uint8_t u8x8_x, u8x8_y;         // current position on the screen

volatile unsigned long SysTickCount = 0;

/*=======================================================================*/

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
}

/*
  Enable several power regions: PWR, GPIOA

  This must be executed after each reset.
*/
void startUp(void)
{  
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;	/* enable power interface (PWR) */
  PWR->CR |= PWR_CR_DBP;				/* activate write access to RCC->CSR and RTC */  
  
  SysTick->LOAD = (SystemCoreClock/1000)*50 - 1;   /* 50ms task */
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */      
}

/*=======================================================================*/
/* u8x8 display procedures */

void initDisplay(void)
{
  u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);
  u8x8_InitDisplay(&u8x8);
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetPowerSave(&u8x8, 0);
  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_r);  
  u8x8_x = 0;
  u8x8_y = 0;  
}


void outChar(uint8_t c)
{
  if ( u8x8_x >= u8x8_GetCols(&u8x8) )
  {
    u8x8_x = 0;
    u8x8_y++;
  }
  u8x8_DrawGlyph(&u8x8, u8x8_x, u8x8_y, c);
  u8x8_x++;
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
  u8x8_x = 0;
  u8x8_y = r;
}


/*==============================================*/
volatile unsigned char i2c_mem[256];     /* contains data, which read or written */
volatile unsigned char i2c_idx;                  /* the current index into i2c_mem */
volatile unsigned char i2c_is_write_idx;                  /* write state */

volatile uint16_t i2c_total_irq_cnt;
volatile uint16_t i2c_TXIS_cnt;
volatile uint16_t i2c_RXNE_cnt;


void i2c_mem_reset_write(void)
{
  i2c_is_write_idx = 1;  
}

void i2c_mem_init(void)
{
  i2c_idx = 0;
  i2c_mem_reset_write();
}

void i2c_mem_set_index(unsigned char value)
{
  i2c_idx = value;
  i2c_is_write_idx = 0;
}

void i2c_mem_write_via_index(unsigned char value)
{
  i2c_mem[i2c_idx++] = value;
}

unsigned char i2c_mem_read(void)
{
  i2c_mem_reset_write();
  i2c_idx++;
  return i2c_mem[i2c_idx];
}

void i2c_mem_write(unsigned char value)
{
  if ( i2c_is_write_idx != 0 )
  {
    i2c_mem_set_index(value);
  }
  else
  {
    i2c_is_write_idx = 0;
    i2c_mem_write_via_index(value);
  }
}



/* address: I2C address multiplied by 2 */
/* Pins PA9 (SCL) and PA10 (SDA) */
void i2c_hw_init(unsigned char address)
{
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;		/* Enable clock for I2C */
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  
    __NOP();                                                          /* extra delay for clock stabilization required? */
    __NOP();


  /* configure io */
  GPIOA->MODER &= ~GPIO_MODER_MODE9;	/* clear mode for PA9 */  
  GPIOA->MODER |= GPIO_MODER_MODE9_1;  /* alt fn */
  GPIOA->OTYPER |= GPIO_OTYPER_OT_9;    /* open drain */
  GPIOA->AFR[1] &= ~(15<<4);            /* Clear Alternate Function PA9 */
  GPIOA->AFR[1] |= 1<<4;                   /* I2C Alternate Function PA9 */
  
  GPIOA->MODER &= ~GPIO_MODER_MODE10;	/* clear mode for PA10 */  
  GPIOA->MODER |= GPIO_MODER_MODE10_1;  /* alt fn */
  GPIOA->OTYPER |= GPIO_OTYPER_OT_10;    /* open drain */
  GPIOA->AFR[1] &= ~(15<<8);            /* Clear Alternate Function PA10 */
  GPIOA->AFR[1] |= 1<<8;            /* I2C Alternate Function PA10 */
  
  
  RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;                      /* write 00 to the I2C clk selection register */
  RCC->CCIPR |= RCC_CCIPR_I2C1SEL_0;                      /* select system clock (01) */
  
  /* I2C init flow chart: Clear PE bit */
  
  I2C1->CR1 &= ~I2C_CR1_PE;             
  
  /* I2C init flow chart: Configure filter */
  
  /* leave at defaults */

  /* I2C init flow chart: Configure timing */
  /*
    standard mode 100kHz configuration
    SYSCLK = I2CCLK = 32 MHz
    PRESC = 6           bits 28..31
    SCLL = 0x13         bits 0..7
    SCLH = 0x0f         bits 8..15
    SDADEL = 0x02       bits 16..19
    SCLDEL = 0x04       bits 20..23
  */
  I2C1->TIMINGR = 0x60420f13;
  
  /* I2C init flow chart: Configure NOSTRECH */
  
  I2C1->CR1 |= I2C_CR1_NOSTRETCH;

  /* I2C init flow chart: Enable I2C */
  
  I2C1->CR1 |= I2C_CR1_PE;


  /* disable OAR1 for reconfiguration */
  I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
  
  I2C1->OAR1 = address;
  
  I2C1->OAR1 |= I2C_OAR1_OA1EN;


  /* enable interrupts */
  I2C1->CR1 |= I2C_CR1_STOPIE;
  I2C1->CR1 |= I2C_CR1_NACKIE;
  //I2C1->CR1 |= I2C_CR1_ADDRIE;
  I2C1->CR1 |= I2C_CR1_RXIE;
  I2C1->CR1 |= I2C_CR1_TXIE;
  
  

  /* load first value into TXDR register */
  I2C1->TXDR = i2c_mem[i2c_idx];


  /* enable IRQ in NVIC */
  NVIC_SetPriority(I2C1_IRQn, 0);
  NVIC_EnableIRQ(I2C1_IRQn);


  
}

void i2c_init()
{
  i2c_mem_init();
  i2c_hw_init(7*2);
}


void __attribute__ ((interrupt, used)) I2C1_IRQHandler(void)
{
  unsigned long isr = I2C1->ISR;

  i2c_total_irq_cnt ++;
  
  if ( isr & I2C_ISR_TXIS )
  {
    i2c_TXIS_cnt++;
    I2C1->TXDR = i2c_mem_read();
  }
  else if ( isr & I2C_ISR_RXNE )
  {
    i2c_RXNE_cnt++;
    i2c_mem_write(I2C1->RXDR);
    I2C1->ISR |= I2C_ISR_TXE;           // allow overwriting the TCDR with new data
    I2C1->TXDR = i2c_mem[i2c_idx];
  }
  else if ( isr & I2C_ISR_STOPF )
  {
    I2C1->ICR = I2C_ICR_STOPCF;
    I2C1->ISR |= I2C_ISR_TXE;           // allow overwriting the TCDR with new data
    I2C1->TXDR = i2c_mem[i2c_idx];
    i2c_mem_reset_write();
  }
  else if ( isr & I2C_ISR_NACKF )
  {
    I2C1->ICR = I2C_ICR_NACKCF;
    I2C1->ISR |= I2C_ISR_TXE;           // allow overwriting the TCDR with new data
    I2C1->TXDR = i2c_mem[i2c_idx];
    i2c_mem_reset_write();
  }
  else if ( isr & I2C_ISR_ADDR )
  {
    /* not required, the addr match interrupt is not enabled */
    I2C1->ICR = I2C_ICR_ADDRCF;
    I2C1->ISR |= I2C_ISR_TXE;           // allow overwriting the TCDR with new data
    I2C1->TXDR = i2c_mem[i2c_idx];
    i2c_mem_reset_write();
  }
 
  /* if at any time the addr match is set, clear the flag */
  /* not sure, whether this is required */
  if ( isr & I2C_ISR_ADDR )
  {
    I2C1->ICR = I2C_ICR_ADDRCF;
  }
    
}


/*==============================================*/

int main()
{
  setHSIClock();
  startUp();
  initDisplay();          /* aktivate display */
  i2c_init();

  __enable_irq();
  
  setRow(0); outStr("Hello World!"); 
  

  
  for(;;)
  {
    setRow(2); outHex32(SysTickCount); 
    setRow(3); outHex16(i2c_total_irq_cnt);
    setRow(4); outHex16(i2c_TXIS_cnt); outStr(" "); outHex16(i2c_RXNE_cnt);
    setRow(5); outStr("I2C_ISR:"); outHex32(I2C1->ISR);
    setRow(6); outStr("idx:    "); outHex8(i2c_idx);
    setRow(7); outHex8(i2c_mem[0]); outStr(" "); outHex8(i2c_mem[1]); outStr(" "); outHex8(i2c_mem[2]);
    
  }
}
