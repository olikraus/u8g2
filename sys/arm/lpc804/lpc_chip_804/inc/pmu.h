#ifndef LPC8XX_PMU_H_
#define LPC8XX_PMU_H_

// PCON register shifters
#define PM        0
#define NODPD     3
#define SLEEPFLAG 8
#define DPDFLAG   11

// WUSRCREG and WUENAREG
#define PIO_15_WU (1<<0)
#define PIO_9_WU  (1<<1)
#define PIO_8_WU  (1<<2)
#define PIO_17_WU (1<<3)
#define PIO_13_WU (1<<4)
#define PIO_5_WU  (1<<5)
#define PIO_11_WU (1<<6)
#define PIO_10_WU (1<<7)

#endif // LPC8XX_PMU_H_
