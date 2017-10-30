/* LED blink project for the STM32L031 */

#include "stm32l031xx.h"

volatile unsigned long SysTickCount = 0;

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  if ( SysTickCount & 1 )
    GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set PA13 */
  else
    GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */
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

/*==============================================*/
unsigned char i2c_mem[256];     /* contains data, which read or written */
unsigned char i2c_idx;                  /* the current index into i2c_mem */
unsigned char i2c_is_write_idx;                  /* write state */

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
    i2c_mem_set_index(value);
  else
    i2c_mem_write_via_index(value);
}



/* address: I2C address multiplied by 2 */
void i2c_hw_init(unsigned char address)
{
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;		/* Enable clock for I2C */
  RCC->CIPR &= ~RCC_CCIPR_I2C1SEL;                      /* write 00 to the I2C clk selection register */
  RCC->CIPR |= RCC_CCIPR_I2C1SEL_0;                      /* select system clock (01) */
  
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


  /* I2C init flow chart: Enable I2C */
  
  I2C1->CR1 |= I2C_CR1_PE;
  
  
}


void __attribute__ ((interrupt, used)) I2C1_IRQHandler(void)
{
  unsigned long iSr = I2C1->ISR;
  if ( isr & I2C_ISR_TXIS )
  {
    I2C1->TXDR = i2c_mem_read();
  }
  else if ( isr & I2C_ISR_RXNE )
  {
    i2c_mem_write(I2C1->RXDR);
  }
  else if ( isr & I2C_ISR_STOPF )
  {
    I2C1->ICR = I2C_ICR_STOPCF;
    I2C1->TXDR = i2c_mem[i2c_idx];
    i2c_mem_reset_write();
  }
  else if ( isr & I2C_ISR_NACKF )
  {
    I2C1->ICR = I2C_ICR_NACKCF;
    I2C1->TXDR = i2c_mem[i2c_idx];
    i2c_mem_reset_write();
  }
  else if ( isr & I2C_ISR_ADDR )
  {
    I2C1->ICR = I2C_ICR_ADDRCF;
    I2C1->TXDR = i2c_mem[i2c_idx];
    i2c_mem_reset_write();
  }
    
}


/*==============================================*/

int main()
{
  setHSIClock();
  
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  
  
  __NOP();
  __NOP();
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode for PA13 */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode for PA13 */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* no Push/Pull for PA13 */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed for PA13 */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown for PA13 */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr PA13 */

  SysTick->LOAD = 2000*500 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
    
  for(;;)
    ;
}
