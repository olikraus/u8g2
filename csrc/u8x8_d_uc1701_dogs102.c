
#include "u8x8.h"




static const uint8_t u8x8_d_uc1701_dogs102_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_C(0x0e2),            			/* soft reset */
  U8X8_C(0x0ae),		                /* display off */
  U8X8_C(0x040),		                /* set display start line to 0 */
  
#if U8X8_DEFAULT_FLIP_MODE == 0 
  U8X8_C(0x0a1),		                /* ADC set to reverse */
  U8X8_C(0x0c0),		                /* common output mode */
#else
  U8X8_C(0x0a0),		                /* ADC set to reverse */
  U8X8_C(0x0c8),		                /* common output mode */
#endif
  
  U8X8_C(0x0a6),		                /* display normal, bit val 0: LCD pixel off. */
  U8X8_C(0x0a2),		                /* LCD bias 1/9 */
  U8X8_C(0x02f),		                /* all power  control circuits on */
  U8X8_C(0x027),		                /* regulator, booster and follower */
  U8X8_CA(0x081, 0x00e),		/* set contrast, contrast value, EA default: 0x010, previous value for S102: 0x0e */
  U8X8_C(0x0fa),		                /* Set Temp compensation */ 
  U8X8_C(0x090),		                /* 0.11 deg/c WP Off WC Off*/
  U8X8_C(0x0a4),		                /* normal display  */
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1701_dogs102_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		                /* display on */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1701_dogs102_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		                /* display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

#ifdef U8X8_WITH_SET_FLIP_MODE
static const uint8_t u8x8_d_uc1701_dogs102_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a1),				/* segment remap a0/a1*/
  U8X8_C(0x0c0),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_uc1701_dogs102_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a0),				/* segment remap a0/a1*/
  U8X8_C(0x0c8),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};
#endif


static const u8x8_display_info_t u8x8_uc1701_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 5,
  /* pre_chip_disable_wait_ns = */ 5,
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 6, 
  /* sda_setup_time_ns = */ 15,		/* if this is smaller  than sck_pulse_width_ns, use sck_pulse_width_ns, so use 15 instead of 12*/
  /* sck_pulse_width_ns = */ 15,
  /* sck_takeover_edge = */ 1,		/* rising edge */
  /* i2c_bus_clock_100kHz = */ 37,
  /* data_setup_time_ns = */ 30,
  /* write_pulse_width_ns = */ 40,
  /* tile_width = */ 13,
  /* tile_hight = */ 8,
#if U8X8_DEFAULT_FLIP_MODE == 0 
  /* default_x_offset = */ 0,
#else
  /* default_x_offset = */ 30,
#endif
};

uint8_t u8x8_d_uc1701_dogs102(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP:
      u8x8_d_helper_display_setup(u8x8, &u8x8_uc1701_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_uc1701_dogs102_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1701_dogs102_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1701_dogs102_powersave1_seq);
      break;
 #ifdef U8X8_WITH_SET_FLIP_MODE
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1701_dogs102_flip0_seq);
	u8x8->x_offset = 0;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1701_dogs102_flip1_seq);
	u8x8->x_offset = 30;
      }	
      break;
#endif
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int >> 2 );	/* uc1701 has range from 0 to 63 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8x8->x_offset;
      u8x8_cad_SendCmd(u8x8, 0x010 | (x>>4) );
      u8x8_cad_SendCmd(u8x8, 0x000 | ((x&15)));
      u8x8_cad_SendCmd(u8x8, 0x0b0 | (((u8x8_tile_t *)arg_ptr)->y_pos));
    
      do
      {
	c = ((u8x8_tile_t *)arg_ptr)->cnt;
	ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
	u8x8_cad_SendData(u8x8, c*8, ptr);	/* note: SendData can not handle more than 255 bytes */
	arg_int--;
      } while( arg_int > 0 );
      
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}


