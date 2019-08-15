/*
 * lpc8xx_acomp.h
 *
 *  Created on: Apr 7, 2016
 *      Author:
 */

#ifndef LPC8XX_ACOMP_H_
#define LPC8XX_ACOMP_H_



// CTRL register shifters
#define HYS         25
#define INTENA      24
#define COMPEDGE    23
#define COMPSTAT    21
#define EDGECLR     20
#define COMP_VM_SEL 11
#define COMP_VP_SEL 8
#define COMPSA      6
#define EDGESEL     3

// HYS field descriptors
#define _20mV       0x3
#define _10mV       0x2
#define _5mV        0x1
#define NONE        0x0
// COMP_VM_SEL and COMP_VP_SEL field descriptors
#define DAC_OUT_0    0x7
#define V_BANDGAP    0x6
#define ACOMP_IN5    0x5
#define ACOMP_IN4    0x4
#define ACOMP_IN3    0x3
#define ACOMP_IN2    0x2
#define ACOMP_IN1    0x1
#define V_LADDER_OUT 0x0
// EDGESEL field descriptors
#define BOTH        0x3
#define RISING      0x1
#define FALLING     0x0

// LAD register shifters
#define LADREF      6
#define LADSEL      1
#define LADEN       0

// LADREF field descriptors
#define VDDCMP_PIN  1
#define SUPPLY_VDD  0


#endif /* LPC8XX_ACOMP_H_ */
