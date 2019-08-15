/*
 * lpc8xx_adc.h
 *
 *  Created on: This day, August 18, 2016
 *      Author: The Creator
 */

#ifndef LPC8XX_ADC_H_
#define LPC8XX_ADC_H_



// CTRL register shifters
#define ADC_CLKDIV      0
#define ADC_LPWRMODE    10
#define ADC_CALMODE     30

// Sequence A and B control register shifters
#define ADC_CHANNELS    0
#define ADC_TRIGGER     12
#define ADC_TRIGPOL     18
#define ADC_SYNCBYPASS  19
#define ADC_START       26
#define ADC_BURST       27
#define ADC_SINGLESTEP  28
#define ADC_LOWPRIO     29
#define ADC_MODE        30
#define ADC_SEQ_ENA     31

// ADC hardware trigger inputs
#define NO_TRIGGER      0
#define PININT0_IRQ     1  
#define PININT1_IRQ     2  


#define TIM0_MAT3       5  
#define CMP0_OUT_ADC    6  
#define GPIO_INT_BMATCH 7
#define ARM_TXEV        8

// Threshold compare register shifters
#define THRLOW          15
#define THRHIGH         15

// INTEN register shifters
#define SEQA_INTEN      0
#define SEQB_INTEN      1
#define OVR_INTEN       2
#define ADCMPINTEN0     3
#define ADCMPINTEN1     5
#define ADCMPINTEN2     7
#define ADCMPINTEN3     9
#define ADCMPINTEN4     11
#define ADCMPINTEN5     13
#define ADCMPINTEN6     15
#define ADCMPINTEN7     17
#define ADCMPINTEN8     19
#define ADCMPINTEN9     21
#define ADCMPINTEN10    23
#define ADCMPINTEN11    25

// FLAGS register shifters
#define ADC_THCMP0          0
#define ADC_THCMP1          1
#define ADC_THCMP2          2
#define ADC_THCMP3          3
#define ADC_THCMP4          4
#define ADC_THCMP5          5
#define ADC_THCMP6          6
#define ADC_THCMP7          7
#define ADC_THCMP8          8
#define ADC_THCMP9          9
#define ADC_THCMP10         10
#define ADC_THCMP11         11
#define ADC_OVERRUN0        12
#define ADC_OVERRUN1        13
#define ADC_OVERRUN2        14
#define ADC_OVERRUN3        15
#define ADC_OVERRUN4        16
#define ADC_OVERRUN5        17
#define ADC_OVERRUN6        18
#define ADC_OVERRUN7        19
#define ADC_OVERRUN8        20
#define ADC_OVERRUN9        21
#define ADC_OVERRUN10       22
#define ADC_OVERRUN11       23
#define ADC_SEQA_OVR        24
#define ADC_SEQB_OVR        25
#define ADC_SEQA_INT        28
#define ADC_SEQB_INT        29
#define ADC_THCMP_INT       30
#define ADC_OVR_INT         31


// TRM register shifters
#define ADC_VRANGE      5



#endif /* LPC8XX_ADC_H_ */
