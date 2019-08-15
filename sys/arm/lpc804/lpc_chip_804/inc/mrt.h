/*
 * lpc8xx_mrt.h
 *
 *  Created on: Apr 11, 2016
 *      Author: 
 */

#ifndef LPC8XX_MRT_H_
#define LPC8XX_MRT_H_

// INTVAL register values
#define NoForceLoad 0<<31
#define ForceLoad   (uint32_t)1<<31
// INTVAL register bit field shifters
#define MRT_LOAD    31

// Control register bit field shifters
#define MRT_INTEN 0
#define MRT_MODE  1
// Control register values
#define MRT_Repeat  0
#define MRT_OneShot 1
#define MRT_OneShotBusStall 2

// Status register
#define MRT_INTFLAG 0
#define MRT_RUN     1

// Idle channel register
#define IDLE_CHAN 4

// IRQ_FLAG register
#define GFLAG0 0
#define GFLAG1 1
#define GFLAG2 2
#define GFLAG3 3

#endif /* LPC8XX_MRT_H_ */
