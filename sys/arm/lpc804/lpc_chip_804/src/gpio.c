/****************************************************************************
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation is hereby granted, under NXP Semiconductors' 
 * relevant copyright in the software, without fee, provided that it 
 * is used in conjunction with NXP Semiconductors microcontrollers. This 
 * copyright, permission, and disclaimer notice must appear in all copies of 
 * this code.
****************************************************************************/
#include "LPC8xx.h"			/* LPC8xx Peripheral Registers */
#include "gpio.h"
#include "syscon.h"


/*****************************************************************************
** Function name:	GPIOInit
**
** Description:		Enable clock, then reset the GPIO module.
**                      See syscon.h for register bit definitions.
**
** Parameters:		None
**
** Returned value:	None
*****************************************************************************/
void GPIOInit( void )
{
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (GPIO0 | GPIO1);
	
  /* Peripheral reset to the GPIO module. '0' asserts, '1' deasserts reset. */
  LPC_SYSCON->PRESETCTRL[0] &= (GPIO0_RST_N & GPIO1_RST_N);
  LPC_SYSCON->PRESETCTRL[0] |= ~(GPIO0_RST_N & GPIO1_RST_N);
  return;
}


/*****************************************************************************
** Function name:		GPIOGetPinValue
**
** Description:		  Read Current state of port pin, PIN register value
**
** parameters:			port number, bit position
** Returned value:	The value, obviously
**
*****************************************************************************/
uint32_t GPIOGetPinValue( uint32_t portNum, uint32_t bitPosi )
{
  return (LPC_GPIO_PORT->PIN[portNum] & (1<<bitPosi));
}





/*****************************************************************************
** Function name:		GPIOSetBitValue
**
** Descriptions:		Set/clear a bit in a specific position
**
** parameters:			port num, bit position, bit value
** 						
** Returned value:		None
**
*****************************************************************************/
void GPIOSetBitValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
  if ( bitVal )
  {
		LPC_GPIO_PORT->SET[portNum] = 1<<bitPosi;
  }
  else
  {
		LPC_GPIO_PORT->CLR[portNum] = 1<<bitPosi;
  }
  return;
}


/*****************************************************************************
** Function name:		GPIOSetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			portNum, bit position, direction (1 out, 0 input)
** 						
** Returned value:		None
**
*****************************************************************************/
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  if( dir )
  {
		LPC_GPIO_PORT->DIRSET[portNum] = 1<<bitPosi;
  }
  else
  {
		LPC_GPIO_PORT->DIRCLR[portNum] = 1<<bitPosi;
  }
  return;
}




/******************************************************************************
**                            End Of File
******************************************************************************/
