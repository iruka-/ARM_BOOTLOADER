/* STBeeMini */
/*
 * Linker script for STM32.
 * Maple mini flash linker script.
 */

/*
 * Define memory spaces.
 */
MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00005000
  rom (rx)  : ORIGIN = 0x08003000, LENGTH = 0x00020000-0x3000
}

/*
 * Use medium density device vector table
 */
GROUP(libcs3_stm32_med_density.a)

REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);

/*
 * Define the rest of the sections
 */
INCLUDE common_rom.inc

