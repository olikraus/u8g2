/*
  u8x8cb.c
  
  STM32L031 @ 32Mhz
  
*/

#include "stm32l031xx.h"
#include "delay.h"
#include "u8x8.h"

/*
  I2C:
    PA9: Clock
    PA10: Data
*/

uint8_t u8x8_gpio_and_delay_stm32l0_sw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      /* only support for software I2C*/
    
      RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
      __NOP();
      __NOP();
      
      GPIOA->MODER &= ~GPIO_MODER_MODE10;	/* clear mode for PA10 */
      //GPIOA->MODER |= GPIO_MODER_MODE10_0;	/* Output mode for PA10 */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_10;	/* no open drain for PA10 */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED10;	/* low speed for PA10 */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD10;	/* no pullup/pulldown for PA10 */
      //GPIOA->BSRR = GPIO_BSRR_BS_10;		/* atomic set PA10 */
    
      GPIOA->MODER &= ~GPIO_MODER_MODE9;	/* clear mode for PA9 */
      //GPIOA->MODER |= GPIO_MODER_MODE9_0;	/* Output mode for PA9 */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;	/* no open drain for PA9 */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED9;	/* low speed for PA9 */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;	/* no pullup/pulldown for PA9 */
      //GPIOA->BSRR = GPIO_BSRR_BS_9;		/* atomic set PA9 */
        
      break;
    case U8X8_MSG_DELAY_NANO:
      /* not required for SW I2C */
      break;
    
    case U8X8_MSG_DELAY_10MICRO:
      /* not used at the moment */
      break;
    
    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
      break;
   
    case U8X8_MSG_DELAY_MILLI:
      delay_micro_seconds(arg_int*1000UL);
      break;
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
      delay_micro_seconds(arg_int<=2?5:1);
      break;
    
    case U8X8_MSG_GPIO_I2C_CLOCK:
      
      if ( arg_int == 0 )
      {
	GPIOA->MODER &= ~GPIO_MODER_MODE9;	/* clear mode for PA10 */
	GPIOA->MODER |= GPIO_MODER_MODE9_0;	/* Output mode for PA10 */
	GPIOA->BSRR = GPIO_BSRR_BR_9;		/* atomic clr PA9 */
      }
      else
      {
	//GPIOA->BSRR = GPIO_BSRR_BS_9;		/* atomic set PA9 */
	GPIOA->MODER &= ~GPIO_MODER_MODE9;	/* clear mode for PA9: input mode */
      }
      break;
    case U8X8_MSG_GPIO_I2C_DATA:
      
      if ( arg_int == 0 )
      {
	GPIOA->MODER &= ~GPIO_MODER_MODE10;	/* clear mode for PA10 */
	GPIOA->MODER |= GPIO_MODER_MODE10_0;	/* Output mode for PA10 */
	GPIOA->BSRR = GPIO_BSRR_BR_10;		/* atomic clr PA10 */
      }
      else
      {
	//GPIOA->BSRR = GPIO_BSRR_BS_10;		/* atomic set PA10 */
	// input mode
	GPIOA->MODER &= ~GPIO_MODER_MODE10;	/* clear mode for PA10: input mode */
      }
      break;
/*
    case U8X8_MSG_GPIO_MENU_SELECT:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_SELECT_PORT, KEY_SELECT_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_NEXT_PORT, KEY_NEXT_PIN));
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_PREV_PORT, KEY_PREV_PIN));
      break;
    
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_HOME_PORT, KEY_HOME_PIN));
      break;
*/
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}

/*
  SPI:
    PA14: CD
    PA13: CS
    PA7: MOSI
    PA6: Reset
    PA5: SCK
*/

uint8_t u8x8_gpio_and_delay_stm32l0_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      /* only support for software I2C*/
    
      RCC->IOPENR |= RCC_IOPENR_IOPAEN;		/* Enable clock for GPIO Port A */
      __NOP();
      __NOP();
    
      /* setup PA14, PA13, PA7, PA6, PA5 */
    
      GPIOA->MODER &= ~GPIO_MODER_MODE14;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE14_0;	/* Output mode */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_14;	/* no open drain */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED14;	/* low speed */
      GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED14_1;	/* 10 MHz */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD14;	/* no pullup/pulldown */
      GPIOA->BSRR = GPIO_BSRR_BR_14;		/* atomic clear */

      GPIOA->MODER &= ~GPIO_MODER_MODE13;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE13_0;	/* Output mode */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_13;	/* no open drain */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13;	/* low speed */
      GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_1;	/* 10 MHz */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD13;	/* no pullup/pulldown */
      GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clear */
    
      GPIOA->MODER &= ~GPIO_MODER_MODE7;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE7_0;	/* Output mode */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7;	/* no open drain */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED7;	/* low speed */
      GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED7_1;	/* 10 MHz */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7;	/* no pullup/pulldown */
      GPIOA->BSRR = GPIO_BSRR_BR_7;		/* atomic clear */

      GPIOA->MODER &= ~GPIO_MODER_MODE6;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE6_0;	/* Output mode */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;	/* no open drain */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED6;	/* low speed */
      GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED6_1;	/* 10 MHz */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6;	/* no pullup/pulldown */
      GPIOA->BSRR = GPIO_BSRR_BR_6;		/* atomic clear */

      GPIOA->MODER &= ~GPIO_MODER_MODE5;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE5_0;	/* Output mode */
      GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;	/* no open drain */
      GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED5;	/* low speed */
      GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED5_1;	/* 10 MHz */
      GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;	/* no pullup/pulldown */
      GPIOA->BSRR = GPIO_BSRR_BR_5;		/* atomic clear */

      break;
    case U8X8_MSG_DELAY_NANO:
      /* required for SPI, but seems to work without any delay (at least with 2MHz system clock) */
      //delay_micro_seconds(1);
      break;
    
    case U8X8_MSG_DELAY_10MICRO:
      delay_micro_seconds(arg_int*10UL);
      break;
    
    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
      delay_micro_seconds((arg_int+9)/10);
      break;
   
    case U8X8_MSG_DELAY_MILLI:
      delay_micro_seconds(arg_int*1000UL);
      break;
    
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
      delay_micro_seconds(arg_int<=2?5:1);
      break;
    
    case U8X8_MSG_GPIO_SPI_CLOCK:      
      if ( arg_int == 0 )
	GPIOA->BSRR = GPIO_BSRR_BR_5;		/* atomic clr */
      else
	GPIOA->BSRR = GPIO_BSRR_BS_5;		/* atomic set */
      break;
      
    case U8X8_MSG_GPIO_SPI_DATA:
      if ( arg_int == 0 )
	GPIOA->BSRR = GPIO_BSRR_BR_7;		/* atomic clr */
      else
	GPIOA->BSRR = GPIO_BSRR_BS_7;		/* atomic set */
      break;
      
   case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
      if ( arg_int == 0 )
	GPIOA->BSRR = GPIO_BSRR_BR_13;		/* atomic clr */
      else
	GPIOA->BSRR = GPIO_BSRR_BS_13;		/* atomic set */
      break;
      
    case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
      if ( arg_int == 0 )
	GPIOA->BSRR = GPIO_BSRR_BR_14;		/* atomic clr */
      else
	GPIOA->BSRR = GPIO_BSRR_BS_14;		/* atomic set */
      break;
      
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int/*
      if ( arg_int == 0 )
	GPIOA->BSRR = GPIO_BSRR_BR_6;		/* atomic clr */
      else
	GPIOA->BSRR = GPIO_BSRR_BS_6;		/* atomic set */
      break;
      
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}


uint8_t u8x8_byte_stm32l0_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) 
{
  uint8_t *data;
  switch(msg) {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 ) 
     {
        while ( (SPI1->SR & SPI_SR_BSY) || (DMA1_Channel3->CNDTR != 0) )           // wait for transfer completion
          ;
        *(uint8_t *)&(SPI1->DR) = *data;
        data++;
        arg_int--;
      }  
      break;
    case U8X8_MSG_BYTE_INIT:
      /* enable clock for SPI */
      RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;       /* enable SPI1 */
      /* set a predivision if the system clock is too high, not required for 2MHz */
      RCC->CFGR &= ~RCC_CFGR_PPRE2_Msk;
      //RCC->CFGR |= RCC_CFGR_PPRE2_DIV4;
    
      /* output setup is already done, just enable the alternate mode here */

      GPIOA->MODER &= ~GPIO_MODER_MODE7;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE7_1;	/* Alternate function mode */
      GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL7_Msk;     /* clear af mode */
      GPIOA->AFR[0] |= 0 << GPIO_AFRL_AFRL7_Pos;  /* assign af mode (which is 0 for SPI) */
    
      GPIOA->MODER &= ~GPIO_MODER_MODE5;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE5_1;	/* Alternate function mode */
      GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL5_Msk;     /* clear af mode */
      GPIOA->AFR[0] |= 0 << GPIO_AFRL_AFRL5_Pos;  /* assign af mode (which is 0 for SPI) */

      /* setup and enable SPI subsystem */
      /* Note: We assume SPI mode 0 for the displays (true for the most modern displays), so CPHA and CPOL are forced to 0 here. SPI mode is here: u8x8->display_info->spi_mode */
        /*
        BR[2:0]: Baud rate control
        000: fPCLK/2
        001: fPCLK/4
        010: fPCLK/8
        011: fPCLK/16
        100: fPCLK/32
        101: fPCLK/64
        110: fPCLK/128
        111: fPCLK/256
        */
      /* Note: The below assignment will setup also the clock speed, which in turn depends on the uC master clock */
      SPI1->CR1 = SPI_CR1_MSTR                                                    // master
              //| SPI_CR1_BIDIMODE
              //| SPI_CR1_BIDIOE
              | SPI_CR1_SSM 
              | SPI_CR1_SSI
              //| SPI_CR1_BR_0
              //| SPI_CR1_BR_1
              //| SPI_CR1_BR_2          // speed
              //| SPI_CR1_CPHA
              //| SPI_CR1_CPOL
              ;
      SPI1->CR2 = 0;                                                            // SPI_CR2_TXDMAEN = transmit DMA enable
      SPI1->CR1 |= SPI_CR1_SPE;                                           // SPI enable              
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      while ( SPI1->SR & SPI_SR_BSY )           // wait for transfer completion
          ;
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:      
      while ( SPI1->SR & SPI_SR_BSY )           // wait for transfer completion
          ;
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    default:
      return 0;
  }  
  return 1;
}


uint8_t dma_buffer[256]; // required for DMA transfer

uint8_t u8x8_byte_stm32l0_dma_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) 
{
  uint16_t i;
  switch(msg) {
    case U8X8_MSG_BYTE_SEND:
      /* wait for completion of any previous transfer */
      while ( (SPI1->SR & SPI_SR_BSY) || (DMA1_Channel3->CNDTR != 0) )           // wait for transfer completion
          ;

      /* data in arg_ptr will be overwritten, once we leave this function, so create a copy of it (note: memcpy seems to be slower) */
      for( i = 0; i < arg_int; i++ )
        dma_buffer[i] = ((uint8_t *)arg_ptr)[i];
    
      /* setup and start DMA SPI transfer */
      DMA1_Channel3->CCR = 0;           // disable + reset channel 3
      /* defaults: 
          - 8 Bit access	--> ok
          - read from peripheral	--> changed
          - none-circular mode  --> ok
          - no increment mode   --> will be changed below
      */
      DMA1_Channel3->CNDTR = arg_int;                                        /* buffer size */
      DMA1_Channel3->CCR |= DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_EN;		/* increment memory, copy from M to P, enable DMA */

      break;
    case U8X8_MSG_BYTE_INIT:
      /* enable clock for SPI and DMA*/
      RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;       /* enable SPI1 */
      RCC->AHBENR |= RCC_AHBENR_DMA1EN;
      /* set a predivision if the system clock is too high, not required for 2MHz */
      RCC->CFGR &= ~RCC_CFGR_PPRE2_Msk;
      //RCC->CFGR |= RCC_CFGR_PPRE2_DIV16;
    
      /* output setup is already done, just enable the alternate mode here */

      GPIOA->MODER &= ~GPIO_MODER_MODE7;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE7_1;	/* Alternate function mode */
      GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL7_Msk;     /* clear af mode */
      GPIOA->AFR[0] |= 0 << GPIO_AFRL_AFRL7_Pos;  /* assign af mode (which is 0 for SPI) */
    
      GPIOA->MODER &= ~GPIO_MODER_MODE5;	/* clear mode */
      GPIOA->MODER |= GPIO_MODER_MODE5_1;	/* Alternate function mode */
      GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL5_Msk;     /* clear af mode */
      GPIOA->AFR[0] |= 0 << GPIO_AFRL_AFRL5_Pos;  /* assign af mode (which is 0 for SPI) */

    
      DMA1_CSELR->CSELR &= ~DMA_CSELR_C3S_Msk;          /* clear selection on Channel 3 */
      DMA1_CSELR->CSELR |= 1 << DMA_CSELR_C3S_Pos;      /* aktivate SPI_TX on Channel 3 */
      DMA1_Channel3->CCR = 0;           // disable + reset channel 3
      DMA1_Channel3->CNDTR = 0;                                        /* clear buffer size */
      DMA1_Channel3->CMAR = (uint32_t)dma_buffer;
      DMA1_Channel3->CPAR = (uint32_t)&(SPI1->DR);
      

      /* setup and enable SPI subsystem */
      /* Note: We assume SPI mode 0 for the displays (true for the most modern displays), so CPHA and CPOL are forced to 0 here. SPI mode is here: u8x8->display_info->spi_mode */
        
      /* Note: The below assignment will setup also the clock speed, which in turn depends on the uC master clock */
      SPI1->CR1 = SPI_CR1_MSTR                                                    // master
              //| SPI_CR1_BIDIMODE
              //| SPI_CR1_BIDIOE
              | SPI_CR1_SSM 
              | SPI_CR1_SSI
              //| SPI_CR1_BR_0
              //| SPI_CR1_BR_1
              //| SPI_CR1_BR_2          // speed
              //| SPI_CR1_CPHA
              //| SPI_CR1_CPOL
              ;
      SPI1->CR2 = SPI_CR2_TXDMAEN;                                                            // SPI_CR2_TXDMAEN = transmit DMA enable
      SPI1->CR1 |= SPI_CR1_SPE;                                           // SPI enable              
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      while ( (SPI1->SR & SPI_SR_BSY) || (DMA1_Channel3->CNDTR != 0) )           // wait for transfer completion
          ;
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:      
      while ( (SPI1->SR & SPI_SR_BSY) || (DMA1_Channel3->CNDTR != 0) )           // wait for transfer completion
          ;
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    default:
      return 0;
  }  
  return 1;
}
