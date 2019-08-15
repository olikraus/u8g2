#ifndef __LPCPLU_H__
#define __LPCPLU_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
//#define	CONFIG_PLU_nLUT_IN		(5)
//#define CONFIG_PLU_nIN        (6)             /* Number of inputs */
//#define CONFIG_PLU_nLU        (26)            /* Number of LUTs */
//#define CONFIG_PLU_nFF        (4)             /* Number of FFs */
//#define CONFIG_PLU_nOU        (8)             /* Number of outputs */

#define	LUT_INs		5
#define PLU_INs		6
#define	PLU_LUTs	26
#define PLU_FFs		4
#define PLU_OUTs	8
	 
#pragma anon_unions
	 
typedef struct
{
	struct {                                        /* Input select register for LUT & Input... */
		union {
			__IO uint32_t INP[8];												/* Each LUT has maximum 5 inputs, the remaining are reserved. */
		  struct {
		    __IO uint32_t INP0;
		    __IO uint32_t INP1;
		    __IO uint32_t INP2;
		    __IO uint32_t INP3;
		    __IO uint32_t INP4;
		    uint32_t RESERVED[3];
		  };
	  } LUT_MUX[PLU_LUTs];
	};
	     uint32_t RESERVED0[8*64-8*PLU_LUTs]; /* ...-0x7FC */ 
  __IO uint32_t LUT_TRUTH[PLU_LUTs];        /* Truth-Table ("Look-up Table") programming */
	     uint32_t RESERVED1[64-PLU_LUTs];     /* ...-0x8FC */
  __I  uint32_t OUTPUTS;                    /* PLU Outputs Register (Read-only) */
	     uint32_t RESERVED2[3*64-1];          /* ...-0xBFC */
  __IO uint32_t OUTPUT_MUX[PLU_OUTs];       /* Select register for PLU Ouptut */

} LPC_PLU_T;

/* PLU driver error code */
#define PLU_OK									0
#define	PLU_INPUT_OUT_OF_RANGE	1
#define	PLU_LUT_OUT_OF_RANGE		2
#define	PLU_FF_OUT_OF_RANGE			3
#define	PLU_OUTPUT_OUT_OF_RANGE	4
#define	PLU_LUT_IN_DEFAULT			999
#define	PLU_ROUTINE_FAILED			9999
#define	PLU_STDL_OUT_OF_RANGE		0xDEAD

/* PLU masks, and micros if PLU tool is not used. */
#define	LUT_OUT_MASK						(1<<7)
#define	FF_OUT_MASK							(1<<6)
#define	PLU_INPUT_MASK					(0<<6)
#define	LUT_OUT(x)							(LUT_OUT_MASK   | (x & 0x7F))
#define	FF_OUT(x)								(FF_OUT_MASK    | (x & 0x3F))
#define	PLU_INPUT(x)						(PLU_INPUT_MASK | (x & 0x3F))
	
#define LUT_IN_OFFSET_INPUT			0
#define	LUT_IN_OFFSET_LUT_OUT		(PLU_INs)
#define	LUT_IN_OFFSET_FF				(PLU_INs+PLU_LUTs)
#define	LUT_IN_PLU_INPUT(x)			(LUT_IN_OFFSET_INPUT+x)
#define	LUT_IN_LUT_OUT(x)				(LUT_IN_OFFSET_LUT_OUT+x)
#define LUT_IN_FF(x)						(LUT_IN_OFFSET_FF+x)

#define	PLU_OUT_OFFSET_LUT_OUT	0
#define	PLU_OUT_OFFSET_FF				(PLU_LUTs)
#define	PLU_OUT_LUT(x)					(PLU_OUT_OFFSET_LUT_OUT+x)
#define	PLU_OUT_FF(x)						(PLU_OUT_OFFSET_FF+x)

extern uint32_t generate_plu_tt(uint32_t (* pnt_tt_function)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t), uint32_t * pnt_tt); 

#define INMUX_IN(x)  (x)
#define INMUX_LUT(x) (PLU_INs+x)
#define INMUX_FF(x)  (PLU_INs+PLU_LUTs+x)
#define INMUXDEF(x)  INMUX_LUT(x)

#define PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,in3,in4,tt) LPC_PLU0->LUT_MUX[lut].INP[0]=in0; \
																											 LPC_PLU0->LUT_MUX[lut].INP[1]=in1; \
																											 LPC_PLU0->LUT_MUX[lut].INP[2]=in2; \
																											 LPC_PLU0->LUT_MUX[lut].INP[3]=in3; \
																											 LPC_PLU0->LUT_MUX[lut].INP[4]=in4; \
																											 LPC_PLU0->LUT_TRUTH[lut]=tt
#define PLU_LUT_4IN_CUSTOM(lut,in0,in1,in2,in3,tt)     PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,in3,INMUXDEF(lut),tt)
#define PLU_LUT_3IN_CUSTOM(lut,in0,in1,in2,tt)         PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,INMUXDEF(lut),INMUXDEF(lut),tt)
#define PLU_LUT_2IN_CUSTOM(lut,in0,in1,tt)             PLU_LUT_5IN_CUSTOM(lut,in0,in1,INMUXDEF(lut),INMUXDEF(lut),INMUXDEF(lut),tt)

#define PLU_LUT_2IN_AND(lut,in0,in1)                   PLU_LUT_2IN_CUSTOM(lut,in0,in1,0x88888888)
#define PLU_LUT_3IN_AND(lut,in0,in1,in2)               PLU_LUT_3IN_CUSTOM(lut,in0,in1,in2,0x80808080)
#define PLU_LUT_4IN_AND(lut,in0,in1,in2,in3)           PLU_LUT_4IN_CUSTOM(lut,in0,in1,in2,in3,0x80008000)
#define PLU_LUT_5IN_AND(lut,in0,in1,in2,in3,in4)       PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,in3,in4,0x80000000)

#define PLU_LUT_2IN_NAND(lut,in0,in1)                  PLU_LUT_2IN_CUSTOM(lut,in0,in1,0x77777777)
#define PLU_LUT_3IN_NAND(lut,in0,in1,in2)              PLU_LUT_3IN_CUSTOM(lut,in0,in1,in2,0x7F7F7F7F)
#define PLU_LUT_4IN_NAND(lut,in0,in1,in2,in3)          PLU_LUT_4IN_CUSTOM(lut,in0,in1,in2,in3,0x7FFF7FFF)
#define PLU_LUT_5IN_NAND(lut,in0,in1,in2,in3,in4)      PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,in3,in4,0x7FFFFFFF)

#define PLU_LUT_2IN_OR(lut,in0,in1)                    PLU_LUT_2IN_CUSTOM(lut,in0,in1,0xEEEEEEEE)
#define PLU_LUT_3IN_OR(lut,in0,in1,in2)                PLU_LUT_3IN_CUSTOM(lut,in0,in1,in2,0xFEFEFEFE)
#define PLU_LUT_4IN_OR(lut,in0,in1,in2,in3)            PLU_LUT_4IN_CUSTOM(lut,in0,in1,in2,in3,0xFFFEFFFE)
#define PLU_LUT_5IN_OR(lut,in0,in1,in2,in3,in4)        PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,in3,in4,0xFFFFFFFE)

#define PLU_LUT_2IN_NOR(lut,in0,in1)                   PLU_LUT_2IN_CUSTOM(lut,in0,in1,0x11111111)
#define PLU_LUT_3IN_NOR(lut,in0,in1,in2)               PLU_LUT_3IN_CUSTOM(lut,in0,in1,in2,0x01010101)
#define PLU_LUT_4IN_NOR(lut,in0,in1,in2,in3)           PLU_LUT_4IN_CUSTOM(lut,in0,in1,in2,in3,0x00010001)
#define PLU_LUT_5IN_NOR(lut,in0,in1,in2,in3,in4)       PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,in3,in4,0x00000001)

#define PLU_LUT_2IN_XOR(lut,in0,in1)                   PLU_LUT_2IN_CUSTOM(lut,in0,in1,0x66666666)
#define PLU_LUT_3IN_XOR(lut,in0,in1,in2)               PLU_LUT_3IN_CUSTOM(lut,in0,in1,in2,0x96969696)
#define PLU_LUT_4IN_XOR(lut,in0,in1,in2,in3)           PLU_LUT_4IN_CUSTOM(lut,in0,in1,in2,in3,0x69966996)
#define PLU_LUT_5IN_XOR(lut,in0,in1,in2,in3,in4)       PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,in3,in4,0x96696996)

#define PLU_LUT_2IN_XNOR(lut,in0,in1)                  PLU_LUT_2IN_CUSTOM(lut,in0,in1,0x99999999)
#define PLU_LUT_3IN_XNOR(lut,in0,in1,in2)              PLU_LUT_3IN_CUSTOM(lut,in0,in1,in2,0x69696969)
#define PLU_LUT_4IN_XNOR(lut,in0,in1,in2,in3)          PLU_LUT_4IN_CUSTOM(lut,in0,in1,in2,in3,0x96699669)
#define PLU_LUT_5IN_XNOR(lut,in0,in1,in2,in3,in4)      PLU_LUT_5IN_CUSTOM(lut,in0,in1,in2,in3,in4,0x69969669)

#define PLU_LUT_2IN1SEL_MUX(lut,in0,in1,in2)           PLU_LUT_3IN_CUSTOM(lut,in0,in1,in2,0xCACACACA)
#define PLU_LUT_BUFFER(lut,in0)                        PLU_LUT_2IN_CUSTOM(lut,in0,INMUXDEF(lut),0xAAAAAAAA)
#define PLU_LUT_INVERTER(lut,in0)                      PLU_LUT_2IN_CUSTOM(lut,in0,INMUXDEF(lut),0x55555555)

#define OUTMUX_LUT(x)    (x)
#define OUTMUX_FF(x)     (PLU_LUTs+x)
#define PLU_OUT(out,src) LPC_PLU0->OUTPUT_MUX[out]=src


#ifdef __cplusplus
}
#endif

#endif  /* __LPCPLU_H__ */
