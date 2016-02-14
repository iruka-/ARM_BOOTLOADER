/* SH72620 Available Memory */

MEMORY
{
/*FRAM (wx) : ORIGIN = 0x1c000000, LENGTH = 40k */
  FRAM (wx) : ORIGIN = 0xfff82000, LENGTH = 40k /* Fast SRAM: 0xfff82000 to 0xfff8bfff */
}


/* Section Definitions */
SECTIONS
{
  .text :
  {
	KEEP(*(.VECTTBL))	/* Exception vector table */
	KEEP(*(.INTTBL))	/* Interrupt vector table */

	*(.text .text.*)	/* Program code */
	*(.rodata)		/* Read-only data (constants) */
	*(.rodata*)
	. = ALIGN(4);
	__text_end__ = . ;
  } > FRAM


  /* .data section which has initialized data */
  .data :
  {
	__data_start__ = . ;
	*(.data)
	*(.data.*)
	*(.zdata)
	. = ALIGN(4);
	__data_end__ = . ;
  } > FRAM


  /* .bss section which is to be initialized by 0 */
  .bss (NOLOAD) :
  {
	__bss_start__ = . ;
	*(.bss)
	*(.bss.*)
	*(COMMON)
	*(.got.*)
	. = ALIGN(4);
	__bss_end__ = . ;
	_end = . ;
  } > FRAM


  .stack (NOLOAD) :
  {
	__stack_start__ = . ;
	*(.STACK)
	*(.stack)
	. = ALIGN(4);
	__stack_end__ = . ;
  } > FRAM



  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  .debug         0 : { *(.debug*) }

}
