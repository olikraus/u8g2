/*
 * i2c.c
 *
 *  Created on: Apr 5, 2016
 *
 */

#include "LPC8xx.h"
//#include "utilities.h"
#include "i2c.h"

/*****************************************************************************
** Function name:		I2CmasterWrite
**
** Description:		  Write a block of data to the I2C slave using polling.
**
** parameters:      pointer to the write buffer starting with the I2C slave address.
**                  Length of the write data block 
** Returned value:		None
**
*****************************************************************************/
void I2CmasterWrite( uint8_t *WrBuf, uint8_t WrLen )
{	
	uint32_t i;
	
  WaitI2CMasterState(LPC_I2C0, I2C_STAT_MSTST_IDLE);	// Wait for the master state to be idle
  LPC_I2C0->MSTDAT = *WrBuf | 0;    									// Address with 0 for RWn bit (WRITE)
  LPC_I2C0->MSTCTL = CTL_MSTSTART;										// Start the transaction by setting the MSTSTART bit to 1 in the Master control register.
  WaitI2CMasterState(LPC_I2C0, I2C_STAT_MSTST_TX);		// Wait for the address to be ACK'd
	
	for ( i = 0; i < WrLen; i++ ) {
		LPC_I2C0->MSTDAT = *(WrBuf + i + 1);               // Send the data to the slave
		LPC_I2C0->MSTCTL = CTL_MSTCONTINUE;                // Continue the transaction
		WaitI2CMasterState(LPC_I2C0, I2C_STAT_MSTST_TX);   // Wait for the data to be ACK'd
  }
  LPC_I2C0->MSTCTL = CTL_MSTSTOP;                    // Send a stop to end the transaction
	return;
}

/*****************************************************************************
** Function name:		I2CmasterWriteRead
**
** Description:		  Write the command to the i2C slave and followed by using repeated start 
                    to read a block of data from the I2C slave.
**
** parameters:      pointer to the write buffer
                    pointer to the read buffer
                    length of the write data buffer
                    length of the read data buffer
                    e.g. WR_Addr, Write data (0~WrLen-1), RD_addr, Read data (0~RdLen-1)
** Returned value:	None
**
*****************************************************************************/
void I2CmasterWriteRead( uint8_t *WrBuf, uint8_t *RdBuf, uint8_t WrLen, uint8_t RdLen )
{	
	uint32_t i, i2c_addr;
	
	i2c_addr = *WrBuf;
  WaitI2CMasterState(LPC_I2C0, I2C_STAT_MSTST_IDLE);	// Wait for the master state to be idle
  LPC_I2C0->MSTDAT = i2c_addr;    										// Address with 0 for RWn bit (WRITE)
  LPC_I2C0->MSTCTL = CTL_MSTSTART;										// Start the transaction by setting the MSTSTART bit to 1 in the Master control register.
  WaitI2CMasterState(LPC_I2C0, I2C_STAT_MSTST_TX);		// Wait for the address to be ACK'd
	
	for ( i = 0; i < WrLen; i++ ) {
		LPC_I2C0->MSTDAT = *(WrBuf + i + 1);               // Send the data to the slave
		LPC_I2C0->MSTCTL = CTL_MSTCONTINUE;                // Continue the transaction
		WaitI2CMasterState(LPC_I2C0, I2C_STAT_MSTST_TX);   // Wait for the data to be ACK'd
  }

  LPC_I2C0->MSTDAT = i2c_addr | RD_BIT;    						// Address with 1 for RWn bit (READ)
  LPC_I2C0->MSTCTL = CTL_MSTSTART;										// Start the transaction by setting the MSTSTART bit to 1 in the Master control register.	

	for ( i = 0; i < RdLen; i++ ) {
		WaitI2CMasterState(LPC_I2C0, I2C_STAT_MSTST_RX);		// Wait for the data to be ACK'd
		*(RdBuf + i) = LPC_I2C0->MSTDAT;									// Send the data to the slave
		LPC_I2C0->MSTCTL = CTL_MSTCONTINUE;								// Continue the transaction
  }	
  LPC_I2C0->MSTCTL = CTL_MSTSTOP;                    // Send a stop to end the transaction
	return;
}

/*****************************************************************************
** Function name:		WaitI2CMasterState
**
** Description:		    Waits for I2C master pending, then compares the master
**                      state to the state parameter. If compare fails, enter
**                      a while(1) loop. If compare passes, return.
**
** parameters:
**                      ptr_LPC_I2C: A pointer to an I2C instance
**                      state: One of the 3-bit Master function state codes of the I2C
** Returned value:		None
**
*****************************************************************************/
void WaitI2CMasterState(LPC_I2C_TypeDef * ptr_LPC_I2C, uint32_t state) {

  while(!(ptr_LPC_I2C->STAT & STAT_MSTPEND));            // Wait for MSTPENDING bit set in STAT register
  if((ptr_LPC_I2C->STAT & MASTER_STATE_MASK) != state) { // If master state mismatch ...
    while(1);                                            // die here and debug the problem
  }
  return;                                                // If no mismatch, return

}




/*****************************************************************************
** Function name:		WaitI2CSlaveState
**
** Description:		    Waits for I2C slave pending, then compares the slave
**                      state to the state parameter. If compare fails, enter
**                      a while(1) loop. If compare passes, return.
**
** parameters:
**                      ptr_LPC_I2C: A pointer to an I2C instance
**                      state: One of the 2-bit slave function state codes of the I2C
** Returned value:		None
**
*****************************************************************************/
void WaitI2CSlaveState(LPC_I2C_TypeDef * ptr_LPC_I2C, uint32_t state) {

  while(!(ptr_LPC_I2C->STAT & STAT_SLVPEND));         // Wait for SLVPENDING bit in STAT register
  if((ptr_LPC_I2C->STAT & SLAVE_STATE_MASK) != state) // If state mismatches
    while(1);                                         // Die here, and debug the problem
  return;                                             // Otherwise, return

}

