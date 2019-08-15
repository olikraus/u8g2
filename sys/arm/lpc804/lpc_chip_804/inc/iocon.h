#ifndef IOCON_H_
#define IOCON_H_


#define IOCON_MODE_MASK  (0xFFFFFFe7)
#define IOCON_DACEN_MASK (0xFFFeFFFF)

#define MODE_INACTIVE (0<<3)
#define MODE_PULLDOWN (1<<3)
#define MODE_PULLUP   (2<<3)
#define MODE_REPEATER (3<<3)



// DAC pin shifters
#define IOCON_MODE          3
#define IOCON_HYS           5
#define IOCON_INV           6
#define IOCON_RESERVED789   (0x1<<7)
#define IOCON_OD            10
#define IOCON_S_MODE        11
#define IOCON_CLK_DIV       13
#define IOCON_DAC_ENABLE    16



#endif // IOCON_H_



