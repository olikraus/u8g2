/*

  stm32l031x6.ld
  
  Modified for stm32l0 from the original nokeep.ld script from the 
  arm-none-eabi examples by olikraus@gmail.com
  
  Assuming, that the original nokeep.ld file is available under
  the GNU General Public License, this file is  available under the
  same license.
  
  There are three modifications:
  
  1. Provide symbols for the stm32l0 startup code
  
  The following symbols are required for the stm32l0 startup 
  code (e.g. startup_stm32l031xx.s)
  
  _sidata	 start address for the initialization values of the .data section
  _sdata	start address for the .data section. defined in linker script
  _edata	end address for the .data section. defined in linker script
  _sbss	start address for the .bss section. defined in linker script
  _ebss	end address for the .bss section. defined in linker script
  _estack	top address of the stack
  
  2. Stack size estimation / calculation
  
  _Stack_Size has been added to allow better stack size calculation

  3. KEEP keywords
  
  Additionall KEEPs added for .init and .fini. Without this KEEP the 
  generated code will not work, because of the missing _init function.
  
  4. Bugfix: Allign the end of the flash area
  
*/

_Stack_Size = 0x400; /* stm32l0: estimated amount of stack */


/* Linker script to configure memory regions. 
 * Need modifying for a specific board. 
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 32K
  RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 8K
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 * 
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __copy_table_start__
 *   __copy_table_end__
 *   __zero_table_start__
 *   __zero_table_end__
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 */
ENTRY(Reset_Handler)

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text*)

		/* the st32l0 startup code calls __libc_init_array, which calls the _init */
		/* ... sooo.... better keep the init and fini sections */
		  
		KEEP ( *(.init) )		
		KEEP ( *(.fini) )

		/* .ctors */
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)

		/* .dtors */
 		*crtbegin.o(.dtors)
 		*crtbegin?.o(.dtors)
 		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
 		*(SORT(.dtors.*))
 		*(.dtors)

		*(.rodata*)

		*(.eh_frame*)
		
		/* allign the end of the flash area */
		. = ALIGN(4);
	} > FLASH

	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH
	__exidx_end = .;

	/* To copy multiple ROM to RAM sections,
	 * uncomment .copy.table section and,
	 * define __STARTUP_COPY_MULTIPLE in startup_ARMCMx.S */
	/*
	.copy.table :
	{
		. = ALIGN(4);
		__copy_table_start__ = .;
		LONG (__etext)
		LONG (__data_start__)
		LONG (__data_end__ - __data_start__)
		LONG (__etext2)
		LONG (__data2_start__)
		LONG (__data2_end__ - __data2_start__)
		__copy_table_end__ = .;
	} > FLASH
	*/

	/* To clear multiple BSS sections,
	 * uncomment .zero.table section and,
	 * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_ARMCMx.S */
	/*
	.zero.table :
	{
		. = ALIGN(4);
		__zero_table_start__ = .;
		LONG (__bss_start__)
		LONG (__bss_end__ - __bss_start__)
		LONG (__bss2_start__)
		LONG (__bss2_end__ - __bss2_start__)
		__zero_table_end__ = .;
	} > FLASH
	*/

	__etext = .;
	_sidata = .;		/* for stm32l0 startup code */
		
	.data : AT (__etext)
	{
		__data_start__ = .;
		_sdata = .;		/* for stm32l0 startup code */
		*(vtable)
		*(.data*)

		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		*(.preinit_array)
		PROVIDE_HIDDEN (__preinit_array_end = .);

		. = ALIGN(4);
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		*(SORT(.init_array.*))
		*(.init_array)
		PROVIDE_HIDDEN (__init_array_end = .);


		. = ALIGN(4);
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		*(SORT(.fini_array.*))
		*(.fini_array)
		PROVIDE_HIDDEN (__fini_array_end = .);

		*(.jcr)
		. = ALIGN(4);
		/* All data end */
		__data_end__ = .;
		_edata = .;		/* for stm32l0 startup code */

	} > RAM

	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		_sbss =  .;		/* for stm32l0 startup code */
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
		_ebss =  .;		/* for stm32l0 startup code */
	} > RAM
	
	.heap (COPY):
	{
		__end__ = .;
		PROVIDE(end = .);
		*(.heap*)
		__HeapLimit = .;
	} > RAM

	/* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
	.stack_dummy (COPY):
	{
		*(.stack*)
		. = .  + _Stack_Size;	/* estimated stack size */
	} > RAM

	/* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
	__StackTop = ORIGIN(RAM) + LENGTH(RAM);
	_estack = __StackTop;	/* for stm32l0 startup code */
	__StackLimit = __StackTop - SIZEOF(.stack_dummy);
	PROVIDE(__stack = __StackTop);
	
	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
