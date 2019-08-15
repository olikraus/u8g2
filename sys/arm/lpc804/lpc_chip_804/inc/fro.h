#ifndef __FRO_H__
#define __FRO_H__

// FROOSCTRL register shifters
#define FRO_FREQ_SEL       0
#define FRO_TRIM          11
#define FRO_TEMP_TRIM     12
#define FRO_DIRECT        17
#define FRO_DEBUG_ENABLE  18

// FROOSCTRL register values
#define FRO_18MHZ          0
#define FRO_24MHZ          1
#define FRO_30MHZ          2
#define FRO_FREQSEL_MASK   3
#define FRO_DIVIDERS_OUT   0
#define FRO_OSCOUT         1

#endif // __FRO_H


