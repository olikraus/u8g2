/*

  guihal.c
  
*/

#include "stm32l031xx.h"
#include "gui.h"

/*============================================*/
/* reset */
void do_reset(void)
{
  /* clear the display first, to provide some visual feedback to the user */
  u8g2_ClearDisplay(gui_menu.u8g2);

  __disable_irq();
  
  /* Deactivate tamper so that the uC can be programmed via UART */
  /* This is required, because RX pin is shared with Tamp2 */
  
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;
  
  EXTI->IMR &= ~EXTI_IMR_IM19;			/* disable tamper irq */

  RTC->TAMPCR = 0;						/* clear tamper detection */

  RTC->WPR = 0;						/* disable RTC write protection */
  RTC->WPR = 0;

  /* execute reset */
  
  NVIC_SystemReset();
}

/*============================================*/
/* load & store from/to permanent memory */


/* 5x 32bit */

void store_gui_data(uint32_t *data)
{
  RTC->BKP0R = data[0];
  RTC->BKP1R = data[1];
  RTC->BKP2R = data[2];
  RTC->BKP3R = data[3];
  RTC->BKP4R = data[4];
}


void load_gui_data(uint32_t *data)
{

  int i;
  
  for( i = 0; i < GUI_ALARM_CNT; i++ )
    data[i] = 0;
  
  data[0] = RTC->BKP0R;
  data[1] = RTC->BKP1R;
  data[2] = RTC->BKP2R;
  data[3] = RTC->BKP3R;
  data[4] = RTC->BKP4R;
}


/*============================================*/
/* input */

int is_por_reset(void)
{
  return 1;
}

int is_button_reset(void)
{
  return 0;
}

uint32_t get_boot_status_register(void)
{
  /* POR: 			00001100 (POR=1, PIN=1), reset pin has RC network connected */
  /* Boot via NVIC: 	00011100, compared to POR, the SW Reset flag is also set */
  
  return RCC->CSR >> 24;
}

extern volatile unsigned long PWR_CSR_Backup;

uint32_t get_pwr_status_register(void)
{

/*
  Bit 1 SBF: Standby flag
    This bit is set by hardware and cleared only by a POR/PDR (power-on reset/power-down
    reset) or by setting the CSBF bit in the PWR power control register (PWR_CR)
      0: Device has not been in Standby mode
      1: Device has been in Standby mode
  
  Bit 0 WUF: Wakeup flag
    This bit is set by hardware and cleared by a system reset or by setting the CWUF bit in the
    PWR power control register (PWR_CR)
      0: No wakeup event occurred
      1: A wakeup event was received from the WKUP pin or from the RTC
  
  
  POR (voltage off-on):  	00
  Reset Button: 		00
  WUF Reset:			11
  
  
*/
  
  return PWR_CSR_Backup & 255;
}


extern volatile unsigned long ResetReason;
uint32_t get_reset_reason(void)
{
    return ResetReason;
}

extern volatile unsigned long RTCWUCount;

uint32_t get_wakeup_count(void)
{
  return RTCWUCount;
}

int is_dst_by_date(uint8_t region);

uint32_t get_dst_by_date(void)
{
  return is_dst_by_date(1);
}

uint32_t get_dst_by_RTC(void)
{
  uint32_t dst_state = 0;
  if ( RTC->CR & RTC_CR_BCK )	/* BKP flag in the CR register */
    dst_state = 1;
  return dst_state;
}


/*============================================*/
/* output */

void SetAlarmSequence(const uint8_t *alarm_sequence);
extern const uint8_t ASeqTrippleBeep[];


void enable_alarm(void)
{
  SetAlarmSequence(ASeqTrippleBeep);
}

void disable_alarm(void)
{
  SetAlarmSequence(NULL);
  
}

void set_time(uint8_t ht, uint8_t ho, uint8_t mt, uint8_t mo, uint8_t st, uint8_t so)
{
  uint32_t v;
  
  v = ht;
  v <<= 4,
  v |= ho;
  v <<= 4;
  v |= mt;
  v <<= 4;
  v |= mo;
  v <<= 4;
  v |= st;
  v <<= 4;
  v |= so;
 
  __disable_irq();
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;

  RTC->ISR = RTC_ISR_INIT;				/* request RTC stop */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* wait for stop */
      ;
  RTC->PRER = 0x07f00ff;					/* 1 Hz clock */
  RTC->TR = v; 
  RTC->ISR =~ RTC_ISR_INIT; 				/* start RTC */
  
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
  __enable_irq();
}


/*
  weekday: 0 = monday
*/
void set_date(uint8_t yt, uint8_t yo, uint8_t mt, uint8_t mo, uint8_t dayt, uint8_t dayo, uint8_t weekday)
{
  uint32_t v;
  
  v = yt;
  
  v <<= 4,
  v |= yo;
  
  v <<= 3;
  v |= weekday+1;	/* monday starts with 1 on the STM32L0 */
  v <<= 1;  
  v |= mt;
  
  v <<= 4;
  v |= mo;
  
  v <<= 4;
  v |= dayt;
  
  v <<= 4;
  v |= dayo;
  
  __disable_irq();
  RTC->WPR = 0x0ca;					/* disable RTC write protection */
  RTC->WPR = 0x053;

  RTC->ISR = RTC_ISR_INIT;				/* request RTC stop */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* wait for stop */
      ;
  RTC->PRER = 0x07f00ff;					/* 1 Hz clock */
  RTC->DR = v; 
  RTC->ISR =~ RTC_ISR_INIT; 				/* start RTC */
  
  RTC->WPR = 0;						/* enable RTC write protection */
  RTC->WPR = 0;
  __enable_irq();
}

/* value 1..7, 0 is default (do not set) */
void set_contrast(void)
{
  uint8_t v = gui_data.contrast;
  if ( v > 0 )
  {
    v = v * 36;
    u8g2_SetContrast(gui_menu.u8g2, v);
  }
}
