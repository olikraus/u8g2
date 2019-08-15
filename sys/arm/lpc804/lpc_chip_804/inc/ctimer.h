#ifndef LPC8XX_CTIMER_H_
#define LPC8XX_CTIMER_H_

// Interrupt Register (IR) shifters
#define MR0INT 0
#define MR1INT 1
#define MR2INT 2
#define MR3INT 3
#define CR0INT 4
#define CR1INT 5
#define CR2INT 6
#define CR3INT 7

// Timer Control Register (TCR) shifters
#define CEN    0
#define CRST   1

// Match Control Register (MCR) shifters
#define MR0I   0
#define MR0R   1
#define MR0S   2
#define MR1I   3
#define MR1R   4
#define MR1S   5
#define MR2I   6
#define MR2R   7
#define MR2S   8
#define MR3I   9
#define MR3R   10
#define MR3S   11

// Capture Control Register (CCR) shifters
#define CAP0RE 0
#define CAP0FE 1
#define CAP0I  2
#define CAP1RE 3
#define CAP1FE 4
#define CAP1I  5
#define CAP2RE 6
#define CAP2FE 7
#define CAP2I  8
#define CAP3RE 9
#define CAP3FE 10
#define CAP3I  11

// External Match Register (EMR) shifters
#define EM0    0
#define EM1    1
#define EM2    2
#define EM3    3
#define EMC0   4
#define EMC1   6
#define EMC2   8
#define EMC3   10
// EMR bit fields
#define DO_NOTHING_ON_MATCH 0x0
#define CLEAR_ON_MATCH      0x1
#define SET_ON_MATCH        0x2
#define TOGGLE_ON_MATCH     0x3

// Count Control Register (CTCR) shifters
#define CTMODE 0
#define CINSEL 2
#define ENCC   4
#define SELCC  5
// CTCR bit fields
#define TIMER_MODE            0x0
#define COUNTER_MODE_RISING   0x1
#define COUNTER_MODE_FALLING  0x2
#define COUNTER_MODE_BOTH     0x3
#define CLEAR_ON_CAP0_RISING  0x0
#define CLEAR_ON_CAP0_FALLING 0x1
#define CLEAR_ON_CAP1_RISING  0x2
#define CLEAR_ON_CAP1_FALLING 0x3
#define CLEAR_ON_CAP2_RISING  0x4
#define CLEAR_ON_CAP2_FALLING 0x5
#define CLEAR_ON_CAP3_RISING  0x6
#define CLEAR_ON_CAP3_FALLING 0x7

// PWM Control Register (PWMC) shifters
#define PWMEN0 0
#define PWMEN1 1
#define PWMEN2 2
#define PWMEN3 3


#endif // LPC8XX_CTIMER_H_
