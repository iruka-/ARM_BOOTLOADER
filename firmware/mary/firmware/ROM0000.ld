/*
 * GENERATED FILE - DO NOT EDIT
 * (C) Code Red Technologies Ltd, 2008-9    
 * Generated C linker script file for LPC1343 
 * (created from nxp_lpc13_c.ld (v3.1.4 (200912230917)) on Mon Jan 11 14:36:17 PST 2010)
*/

/* INCLUDE "lpc1114_lib_flash_crp.ld" */
/* INCLUDE "lpc1114_mem_flash_crp.ld" */

MEMORY
{
  /* Define each memory region */
  MFlash32 (rx) : ORIGIN = 0x0, LENGTH = 0x8000 /* 32k */
  RamLoc8 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x2000 /* 8k bytes */

}
  /* Define a symbol for the top of each memory region */
  __top_MFlash32 = 0x0 + 0x8000;
  __top_RamLoc8 = 0x10000100 + 0x1F00;



ENTRY(ResetISR)

SECTIONS
{
	.text :
	{
		FILL(0xFF);
		KEEP(*(.isr_vector))
		*(.text.Reset_Handler .text.SystemInit)
		. = 0x000002FC;
		KEEP(*(.crp))
		*(.text*)
		*(.rodata*)

	} > MFlash32


	/* for exception handling/unwind - some Newlib functions (in common with C++ and STDC++) use this. */
	
	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > MFlash32

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > MFlash32
	__exidx_end = .;

	_etext = .;
		
	.data :
	{
		_data = .;
		*(vtable)
		*(.data*)
		_edata = .;
	} > RamLoc8 AT>MFlash32

	/* zero initialized data */
	.bss :
	{
		_bss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	} > RamLoc8
	
	/* Where we put the heap with cr_clib */
	.cr_heap :
	{
		end = .;
		_pvHeapStart = .;
	} > RamLoc8

	_vRamTop = __top_RamLoc8 ;
	_vStackTop = _vRamTop;
}
