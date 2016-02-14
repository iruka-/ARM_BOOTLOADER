OUTPUT_FORMAT("elf32-rx-le", "elf32-rx-le", "elf32-rx-le")
OUTPUT_ARCH(rx)
ENTRY("_PowerON_Reset")

MEMORY
{
    fvector(rx)     : org = 0xffffffa0, len = 0x60
    flash(rx)       : org = 0xfffa0000, len = 384k-0x60
    dataflash(r)    : org = 0x00100000, len = 32k
    ram(wx)         : org = 0x00000000, len = 64k

}

SECTIONS
{
  .fvectors :{
    KEEP(*(.fvectors))
  }>fvector

  .text :{
    *(.text)
    *(.text.*)
    *(P)
    etext = .;
  } > flash

  .rvectors :{
    _rvectors_start = .;
    KEEP(*(.rvectors))
    _rvectors_end = .;
  } > flash

  .init :{
    *(.init)
  } > flash

  .fini :{
    *(.fini)
  } > flash

  .got :{
    *(.got)
    *(.got.plt)
  } > flash

  .rodata :{
    *(.rodata)
    *(.rodata.*)
    *(C_1)
    *(C_2)
    *(C)
  . = ALIGN(0x1000);
    *(.dtc)		/* must be aligned with 0x????_?000	*/
    _erodata = .;
  } > flash

  .eh_frame_hdr :{
    *(.eh_frame_hdr)
  } > flash

  .eh_frame :{
    *(.eh_frame)
  } > flash

  .jcr :{
    *(.jcr)
  } > flash

  .tors :{
    __CTOR_LIST__ = .;
    ___ctors = .;
    *(.ctors)
    ___ctors_end = .;
    __CTOR_END__ = .;
    __DTOR_LIST__ = .;
    ___dtors = .;
    *(.dtors)
    ___dtors_end = .;
    __DTOR_END__ = .;
    _mdata = .;
  } > flash

  .data : AT (_mdata) {
    _data = .;
    *(.data)
    *(.data.*)
    *(D)
    *(D_1)
    *(D_2)
    _edata = .;
  } > ram

  .gcc_exc :{
    *(.gcc_exc)
  } > ram

  . = ALIGN(32 / 8);

  .bss :{
    _bss = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    *(B)
    *(B_1)
    *(B_2)
    _ebss = .;
    _end = .;
  } > ram

  . = ALIGN(32 / 8);

  .istack :{
    *(.istack)
    _istack = .;
  } > ram

  .ustack :{
    *(.ustack)
    _ustack = .;
  } > ram
  _ram_used = .;
}
