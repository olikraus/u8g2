/* 
  GPIO pulse generator project for the STM32L031 
  
  I2C:
    Write 0, <gpio cmd>
    
*/

#include "stm32l031xx.h"
#include "core_cm0plus.h"

/*================================================*/
/* forward declaration */
void setGPIO( uint8_t n );
void clearGPIO(void);

/*================================================*/
/* queue */

#define GPIO_QUEUE_MAX 128
uint8_t gpio_queue_mem[GPIO_QUEUE_MAX];
uint8_t gpio_queue_start = 0;
uint8_t gpio_queue_end = 0;

/* this is called from the I2C interrupt procedures */
void addCmdToGPIOQueue(uint8_t n)
{
  uint8_t pos;
  pos = gpio_queue_end ;
  pos++;
  if ( pos >= GPIO_QUEUE_MAX )
    pos = 0;
  if ( pos == gpio_queue_start )
    return;		// queue  overflow
  gpio_queue_mem[gpio_queue_end] = n;
  gpio_queue_end = pos;
}

uint8_t isGPIOQueueEmpty(void)
{
  if ( gpio_queue_start == gpio_queue_end )
    return 1;
  return 0;
}

/* get the next command in the queue, return 255 if the queue is empty */
uint8_t getCmdFromGPIOQueue(void)
{
  uint8_t r = gpio_queue_mem[gpio_queue_start];
  if ( isGPIOQueueEmpty() )
    return 255;
  return r;
}
  
void removeCmdFromGPIOQueue(void)
{
  if ( isGPIOQueueEmpty() )
    return;
  __disable_irq();
  gpio_queue_start++;
  if ( gpio_queue_start >= GPIO_QUEUE_MAX )
    gpio_queue_start = 0;
  __enable_irq();
}

/*================================================*/
/* GPIO output state machine */

#define GPIO_STATE_IDLE 0
#define GPIO_STATE_TURN_ON 1
#define GPIO_STATE_WAIT_ON 2
#define GPIO_STATE_OFF 3
/* time is in ticks + 1 */
#define GPIO_STATE_ON_TICKS 0
#define GPIO_STATE_OFF_TICKS 4

volatile uint8_t gpio_state = GPIO_STATE_IDLE;
volatile uint8_t gpio_state_machine_output_number = 0;
volatile uint8_t gpio_state_machine_counter = 0;

void gpioNextState(void)
{
  switch(gpio_state)
  {
    case GPIO_STATE_IDLE:
      break;
    case GPIO_STATE_TURN_ON:
      setGPIO(gpio_state_machine_output_number);
      gpio_state_machine_counter = GPIO_STATE_ON_TICKS;
      gpio_state = GPIO_STATE_WAIT_ON;
      break;
    case GPIO_STATE_WAIT_ON:
      if ( gpio_state_machine_counter == 0 )
      {
	clearGPIO();
	gpio_state_machine_counter = GPIO_STATE_OFF_TICKS;
	gpio_state = GPIO_STATE_OFF;
      }
      else
      {
	gpio_state_machine_counter--;
      }
      break;
    case GPIO_STATE_OFF:
      if ( gpio_state_machine_counter == 0 )
      {
	gpio_state = GPIO_STATE_IDLE;
      }
      else
      {
	gpio_state_machine_counter--;
      }
      break;
    default:
      gpio_state = GPIO_STATE_IDLE;
      break;
  }
}

uint8_t gpioStartStateMachine(uint8_t gpio_number)
{
  /* can we enable the state machine? */
  if ( gpio_state != GPIO_STATE_IDLE )
    return 0;	/* not idle, can not start */
  /* set the gpio number */
  __disable_irq();
  gpio_state_machine_output_number = gpio_number;
  gpio_state = GPIO_STATE_TURN_ON;
  __enable_irq();
  return 1;
}

/*================================================*/
/* Queue & State Machine Connector */

void processQueue(void)
{
  uint8_t cmd;
  cmd = getCmdFromGPIOQueue();
  if ( cmd < 255 )
  {
    /* try to start the state machine */
    if ( gpioStartStateMachine(cmd) != 0 )
    {
      /* success, remove the cmd from the queue */
      removeCmdFromGPIOQueue();
    }
  }
}


/*==============================================*/
/* I2C */

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
  if ( i2c_idx == 0 )
  {
    /* additionall put this byte into the queue */
    addCmdToGPIOQueue(value);
  }
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

void i2c_init(unsigned char address)
{
  i2c_mem_init();
  i2c_hw_init(address);
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

/*================================================*/

volatile unsigned long SysTickCount = 0;

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  SysTickCount++;  
  gpioNextState();
}


/*
  Delay by the provided number of system ticks.
  The delay must be smaller than the RELOAD value.
  This delay has an imprecision of about +/- 20 system ticks.   
*/
static void _delay_system_ticks_sub(uint32_t sys_ticks)
{
  uint32_t start_val, end_val, curr_val;
  uint32_t load;
  
  start_val = SysTick->VAL;
  start_val &= 0x0ffffffUL;
  end_val = start_val;
  
  if ( end_val < sys_ticks )
  {
    /* check, if the operation after this if clause would lead to a negative result */
    /* if this would be the case, then add the reload value first */
    load = SysTick->LOAD;
    load &= 0x0ffffffUL;
    end_val += load;
  }
  /* counter goes towards zero, so end_val is below start value */
  end_val -= sys_ticks;		
  
  
  /* wait until interval is left */
  if ( start_val >= end_val )
  {
    for(;;)
    {
      curr_val = SysTick->VAL;
      curr_val &= 0x0ffffffUL;
      if ( curr_val <= end_val )
	break;
      if ( curr_val > start_val )
	break;
    }
  }
  else
  {
    for(;;)
    {
      curr_val = SysTick->VAL;
      curr_val &= 0x0ffffffUL;
      if ( curr_val <= end_val && curr_val > start_val )
	break;
    }
  }
}

/*
  Delay by the provided number of system ticks.
  Any values between 0 and 0x0ffffffff are allowed.
*/
void delay_system_ticks(uint32_t sys_ticks)
{
  uint32_t load4;
  load4 = SysTick->LOAD;
  load4 &= 0x0ffffffUL;
  load4 >>= 2;
  
  while ( sys_ticks > load4 )
  {
    sys_ticks -= load4;
    _delay_system_ticks_sub(load4);
  }
  _delay_system_ticks_sub(sys_ticks);
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

void initGPIO(void)
{
  RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
  __NOP();
  __NOP();
  

  GPIOA->MODER &= ~GPIO_MODER_MODE14;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE14_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_14;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED14;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD14;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_14;		/* atomic clr */
  
  GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr */
  
  GPIOA->MODER &= ~GPIO_MODER_MODE7;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE7_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED7;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_7;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE6;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE6_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED6;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_6;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE5;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE5_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED5;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_5;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE4;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE4_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED4;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_4;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE1;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE1_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED1;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_1;		/* atomic clr */

  GPIOA->MODER &= ~GPIO_MODER_MODE0;	/* clear mode  */
  GPIOA->MODER |= GPIO_MODER_MODE0_0;	/* Output mode */
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_0;	/* no Push/Pull */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED0;	/* low speed  */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;	/* no pullup/pulldown */
  GPIOA->BSRR = GPIO_BSRR_BR_0;		/* atomic clr */

}

void clearGPIO(void)
{
  GPIOA->BSRR = GPIO_BSRR_BR_14
    | GPIO_BSRR_BR_13
    | GPIO_BSRR_BR_7
    | GPIO_BSRR_BR_6
    | GPIO_BSRR_BR_5
    | GPIO_BSRR_BR_4
    | GPIO_BSRR_BR_1
    | GPIO_BSRR_BR_0;
}

/*
  0:	PA14
  1:	PA13
  2: 	PA7
  3: 	PA6
  4: 	PA5
  5: 	PA4
  6: 	PA1
  7: 	PA0
*/
void setGPIO( uint8_t n )
{
  clearGPIO();
  switch(n)
  {
     case 0: GPIOA->BSRR = GPIO_BSRR_BS_14; break;
     case 1: GPIOA->BSRR = GPIO_BSRR_BS_13; break;
     case 2: GPIOA->BSRR = GPIO_BSRR_BS_7; break;
     case 3: GPIOA->BSRR = GPIO_BSRR_BS_6; break;
     case 4: GPIOA->BSRR = GPIO_BSRR_BS_5; break;
     case 5: GPIOA->BSRR = GPIO_BSRR_BS_4; break;
     case 6: GPIOA->BSRR = GPIO_BSRR_BS_1; break;
     case 7: GPIOA->BSRR = GPIO_BSRR_BS_0; break;

  }
}

void setAllGPIO(void)
{
  GPIOA->BSRR = GPIO_BSRR_BS_14
    | GPIO_BSRR_BS_13
    | GPIO_BSRR_BS_7
    | GPIO_BSRR_BS_6
    | GPIO_BSRR_BS_5
    | GPIO_BSRR_BS_4
    | GPIO_BSRR_BS_1
    | GPIO_BSRR_BS_0;
}

int main()
{
  uint8_t i;
  initGPIO();
  
  setHSIClock();

  i2c_init(2*17);
  
  SysTick->LOAD = 32000*100 - 1;		// 100 ms
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
  

  /*
  for(;;)
  {
    delay_system_ticks(32000*200);
    setGPIO(7);
    delay_system_ticks(32000*200);
    clearGPIO();
  }
  */
  
  for( i = 0; i < 8; i++ )
    addCmdToGPIOQueue(i);

  for(;;)
  {
    processQueue();
  }
    
}
