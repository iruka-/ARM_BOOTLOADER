/* STBee */
/*
 * Linker script for STM32.
 * STM32 high density chip linker script. Loads to flash with Maple bootloader
 */

MEMORY
{
/*
  ram (rwx) : ORIGIN = 0x20000C00, LENGTH = 61K
  rom (rx)  : ORIGIN = 0x08005000, LENGTH = 492K
*/

  ram (rwx) : ORIGIN = 0x20000C00, LENGTH = 61K
  rom (rx)  : ORIGIN = 0x08003000, LENGTH = 500K

}

/*
 * TENTATIVELY,
 * Use MEdium Density device vector table
 */
GROUP(libcs3_stm32_med_density.a)

REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);

INCLUDE common_rom.inc

