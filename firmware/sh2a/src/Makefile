# Hey Emacs, this is a -*- makefile -*-
#
#	このMakefileは CQ出版 SH2A用のCygwin-gccに対応します.
#
#
# WinARM template makefile 
# by Martin Thomas, Kaiserslautern, Germany 
# <eversmith@heizung-thomas.de>
#
#
#
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# To rebuild project do "make clean" then "make all".
#
# Changelog:
# - 17. Feb. 2005  - added thumb-interwork support (mth)
# - 28. Apr. 2005  - added C++ support (mth)
# - 29. Arp. 2005  - changed handling for lst-Filename (mth)
# -  1. Nov. 2005  - exception-vector placement options (mth)
# - 15. Nov. 2005  - added library-search-path (EXTRA_LIB...) (mth)
# -  2. Dec. 2005  - fixed ihex and binary file extensions (mth)
# - 22. Feb. 2006  - added AT91LIBNOWARN setting (mth)
# - 19. Apr. 2006  - option FLASH_TOOL (default lpc21isp); variable IMGEXT (mth)
# - 23. Jun. 2006  - option USE_THUMB_MODE -> THUMB/THUMB_IW
# -  3. Aug. 2006  - added -ffunction-sections -fdata-sections to CFLAGS
#                    and --gc-sections to LDFLAGS. Only available for gcc 4 (mth)
# -  4. Aug. 2006  - pass SUBMDL-define to frontend (mth)
# - 11. Nov. 2006  - FLASH_TOOL-config, TCHAIN-config (mth)
# - 28. Mar. 2007  - remove .dep-Directory with rm -r -f and force "no error"
# - 24. Aprl 2007  - added "both" option for format (.bin and .hex)

# REMOVE DEFAULT MAKE RULE
MAKEFLAGS = -r

include ./files.mak

HWLIB=../HWLib

# Toolchain prefix (i.e arm-elf -> arm-elf-gcc.exe)
TCHAIN = sh-elf
#TCHAIN = arm-none-eabi

FLASH_TOOL = OPENOCD

# Hardware Selection:
#BASEBOARD = STM8S_D
#BASEBOARD = CQ_STARM
#BASEBOARD = STBEE
#BASEBOARD = STBEE_MINI
#BASEBOARD = RX62N
BASEBOARD = SH2A



# MCU name and submodel
#MCU      = arm7tdmi-s
#MCU      = cortex-m3
#MCU      = rx600
MCU      = sh2a

# ==== MCU submodel ====
#SUBMDL   = LPC2378
#SUBMDL   = LPC2388
#SUBMDL   = STM32F103
#SUBMDL   = RX62N
SUBMDL   = SH2A

# ==== MCU ROM USER START ADDRESS (=VECTOR TABLE) ====
# 大容量SRAM(1MByte)
#ROMADRS 	= 1C000000

# 高速SRAM(64kB) Serial-EEPROMからboot時.
ROMADRS 	= FFF82000



RUN_MODE=ROM_RUN

## == verbose command-line print ==
#CMDLINE_VERBOSE=1

# Target file name (without extension).
TARGET = $(BASEBOARD)-$(ROMADRS)

# List C source files here. (C dependencies are automatically generated.)
# use file-extension c for "c-only"-files

SRC = $(FILES)


# FPUを使う.
#
#USE_FPU = YES

# RESBANKを使う.
#
#
D_USE_RESBANK = -DUSE_RESBANK


ifdef	D_USE_RESBANK
#
#	割り込みハンドラーをアセンブラで書く場合、
#	以下の sh2int.S を加え、common/intprg.c のINTASM を有効にする.
#
ifdef	USE_FPU
ASRCARM = \
	sh2fpint.S
else
ASRCARM = \
	sh2int.S
endif

endif


# List C source files here which must be compiled in ARM-Mode.
# use file-extension c for "c-only"-files
SRCARM = 

# List C++ source files here.
# use file-extension cpp for C++-files (use extension .cpp)
CPPSRC = 

# List C++ source files here which must be compiled in ARM-Mode.
# use file-extension cpp for C++-files (use extension .cpp)
#CPPSRCARM = $(TARGET).cpp
CPPSRCARM = 

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = 


## Output format. (can be ihex or binary or both)
## (binary i.e. for openocd and SAM-BA, hex i.e. for lpc21isp and uVision)
#FORMAT = ihex
#FORMAT = binary
FORMAT = both

# Optimization level, can be [0, 1, 2, 3, s]. 
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s
#OPT = 2

## Using the Atmel AT91_lib produces warning with
## the default warning-levels. 
## yes - disable these warnings; no - keep default settings
AT91LIBNOWARN = yes
#AT91LIBNOWARN = no

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#DEBUG = stabs
DEBUG = dwarf-2

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS = Common/inc

# List any extra directories to look for library files here.
#     Each directory must be seperated by a space.
#EXTRA_LIBDIRS = ../arm7_efsl_0_2_4
EXTRA_LIBDIRS = 


# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99


#NOTE:
# - Low-density devices are STM32F101xx and STM32F103xx microcontrollers where
#   the Flash memory density ranges between 16 and 32 Kbytes.
# - Medium-density devices are STM32F101xx and STM32F103xx microcontrollers where
#   the Flash memory density ranges between 32 and 128 Kbytes.
# - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
#   the Flash memory density ranges between 256 and 512 Kbytes.
# - Connectivity-Line devices are STM32F105xx and STM32F107xx microcontrollers.
#
# ---> STM32F103VB 128KFlash=MD?

# Place -D or -U options for C here

CDEFS =  -D$(RUN_MODE) $(D_USE_RESBANK)

# -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD 

# Place -I options here
CINCS = 	\
 -I../inc		\
 -Iusb_firm/inc		\
 -I/usr/local/sh-tools/include		\
 -I/usr/local/sh-tools/newlib/sh-elf/include		\

#
# Place -D or -U options for ASM here
ADEFS =  -D$(RUN_MODE)

ifdef VECTOR_LOCATION
CDEFS += -D$(VECTOR_LOCATION)
ADEFS += -D$(VECTOR_LOCATION)
endif

CDEFS += -DSUBMDL_$(SUBMDL) -D_ROMADRS=0x$(ROMADRS) -D$(BASEBOARD)
ADEFS += -DSUBMDL_$(SUBMDL) -D_ROMADRS=0x$(ROMADRS) -D$(BASEBOARD)

# Compiler flags.

ifdef	USE_FPU
# FPUを使う.
#FPUFLAG = -m2a
FPUFLAG = -m2a-single
CDEFS += -DUSE_FPU
else
# FPUを使わない.
FPUFLAG = -m2a-nofpu
endif


#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing

#The following options are target specific:
#  -m1                         Generate SH1 code
#  -m2                         Generate SH2 code
#  -m2a                        Generate SH2a code
#  -m2a-nofpu                  Generate SH2a FPU-less code
#  -m2a-single                 Generate default single-precision SH2a code
#  -m2a-single-only            Generate only single-precision SH2a code
#  -m2e                        Generate SH2e code

#
# Flags for C and C++ (arm-elf-gcc/arm-elf-g++)
CFLAGS = -g$(DEBUG) $(FPUFLAG)
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += -Wall -Wcast-align -Wimplicit 
CFLAGS += -Wpointer-arith -Wswitch
#CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -ffunction-sections
#CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<) 
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

# flags only for C
CONLYFLAGS += -Wnested-externs 
CONLYFLAGS += $(CSTANDARD)

ifneq ($(AT91LIBNOWARN),yes)
#AT91-lib warnings with:
CFLAGS += -Wcast-qual
CONLYFLAGS += -Wmissing-prototypes 
CONLYFLAGS += -Wstrict-prototypes
CONLYFLAGS += -Wmissing-declarations
endif

# flags only for C++ (arm-elf-g++)
# CPPFLAGS = -fno-rtti -fno-exceptions
CPPFLAGS = 

# Assembler flags.
#  -Wa,...:    tell GCC to pass this to the assembler.
#  -ahlns:     create listing
#  -g$(DEBUG): have the assembler create line number information
#ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG)
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:.s=.lst),-g$(DEBUG)


#Additional libraries.

# Extra libraries
#    Each library-name must be seperated by a space.
#    To add libxyz.a, libabc.a and libefsl.a: 
#    EXTRA_LIBS = xyz abc efsl
#EXTRA_LIBS = efsl
EXTRA_LIBS =

#Support for newlibc-lpc (file: libnewlibc-lpc.a)
#NEWLIBLPC = -lnewlib-lpc

MATH_LIB = -lm

# CPLUSPLUS_LIB = -lstdc++


# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -L/usr/local/sh-tools/newlib/sh-elf/lib
LDFLAGS += -Wl,-Tsh72620_$(ROMADRS).ld
#LDFLAGS += -nostartfiles -nostdlib -Wl,-Tsh2a.ld
#LDFLAGS += -lc
#LDFLAGS += $(NEWLIBLPC) $(MATH_LIB)
LDFLAGS += $(MATH_LIB)
#LDFLAGS += -lc -lgcc 

LDFLAGS += -lgcc 
LDFLAGS += $(CPLUSPLUS_LIB)
LDFLAGS += $(patsubst %,-L%,$(EXTRA_LIBDIRS))
LDFLAGS += $(patsubst %,-l%,$(EXTRA_LIBS))



# ---------------------------------------------------------------------------
# Flash-Programming support using lpc21isp by Martin Maurer 
# only for Philips LPC and Analog ADuC ARMs
#
# Settings and variables:
#LPC21ISP = lpc21isp
LPC21ISP = lpc21isp
LPC21ISP_PORT = com1
LPC21ISP_BAUD = 38400
LPC21ISP_XTAL = 12000
# other options:
# * verbose output: -debug
# * enter bootloader via RS232 DTR/RTS (only if hardware supports this
#   feature - see Philips AppNote): -control
LPC21ISP_OPTIONS = -control
#LPC21ISP_OPTIONS += -debug
# ---------------------------------------------------------------------------


# Define directories, if needed.
## DIRARM = c:/WinARM/
## DIRARMBIN = $(DIRAVR)/bin/
## DIRAVRUTILS = $(DIRAVR)/utils/bin/

# Define programs and commands.
SHELL = sh
CC = $(TCHAIN)-gcc
CPP = $(TCHAIN)-g++
AR = $(TCHAIN)-ar
OBJCOPY = $(TCHAIN)-objcopy
OBJDUMP = $(TCHAIN)-objdump
SIZE = $(TCHAIN)-size
NM = $(TCHAIN)-nm
REMOVE = rm -f
REMOVEDIR = rm -f -r
COPY = cp

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = "-------- begin (mode: $(RUN_MODE)) --------"
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_ARM = "Compiling C (ARM-only):"
MSG_COMPILINGCPP = Compiling C++:
MSG_COMPILINGCPP_ARM = "Compiling C++ (ARM-only):"
MSG_ASSEMBLING = Assembling:
MSG_ASSEMBLING_ARM = "Assembling (ARM-only):"
MSG_CLEANING = Cleaning project:
MSG_FORMATERROR = Can not handle output-format
MSG_LPC21_RESETREMINDER = You may have to bring the target in bootloader-mode now.

# Define all object files.
COBJ      = $(SRC:.c=.o) 
AOBJ      = $(ASRC:.S=.o)
COBJARM   = $(SRCARM:.c=.o)
AOBJARM   = $(ASRCARM:.S=.o)
#AOBJARM   = $(ASRCARM:.s=.o)
CPPOBJ    = $(CPPSRC:.cpp=.o) 
CPPOBJARM = $(CPPSRCARM:.cpp=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(ASRCARM:.S=.lst) $(SRC:.c=.lst) $(SRCARM:.c=.lst)
#LST = $(ASRC:.s=.lst) $(ASRCARM:.s=.lst) $(SRC:.c=.lst) $(SRCARM:.c=.lst)
LST += $(CPPSRC:.cpp=.lst) $(CPPSRCARM:.cpp=.lst)

# Compiler flags to generate dependency files.
### GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS  = -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
#all: begin gccversion sizebefore build sizeafter finished end
all: begin gccversion build size_elf finished end

ifeq ($(FORMAT),ihex)
build: elf hex lss sym
hex: $(TARGET).hex
IMGEXT=hex
else 
ifeq ($(FORMAT),binary)
build: elf bin lss sym
bin: $(TARGET).bin
IMGEXT=bin
else 
ifeq ($(FORMAT),both)
build: elf hex bin lss sym
hex: $(TARGET).hex
bin: $(TARGET).bin
else 
$(error "$(MSG_FORMATERROR) $(FORMAT)")
endif
endif
endif

elf: $(TARGET).elf
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Eye candy.
begin:
	@echo
	@echo $(MSG_BEGIN)

#	@echo AOBJARM=$(AOBJARM)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo


# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi

size_elf: $(TARGET).elf
	$(TCHAIN)-size $(TARGET).elf
#	-rm main.dfu
#	hex2dfu main.hex main.dfu


dump:
	srec2bin.exe $(TARGET).hex 1 >$(TARGET).dmp

# Display compiler version information.
gccversion : 
	@$(CC) --version

# Program the device.
ifeq ($(FLASH_TOOL),UVISION)
# Program the device with Keil's uVision (needs configured uVision-Workspace). 
program: $(TARGET).hex
	@echo
	@echo "Programming with uVision"
#	C:\Keil\uv3\Uv3.exe -f uvisionflash.Uv2 -ouvisionflash.txt
	$(REMOVE) ../Common_WinARM/$(TARGET).hex
	$(COPY) $(TARGET).hex ../Common_WinARM/
	C:\Keil\uv3\Uv3.exe -f ..\Common_WinARM\uvisionflash.Uv2
else
ifeq ($(FLASH_TOOL),OPENOCD)
# Program the device with Dominic Rath's OPENOCD in "batch-mode", needs cfg and "reset-script".
program: $(TARGET).bin
	@echo
	@echo "Programming with OPENOCD"
	openocd_start_flash.cmd
#OOCD_load_image=$(TARGET).bin
#include oocd_flashing.mk
#program : $(TARGET).bin oocd_programprepare oocd_programdoit
#program : $(TARGET).bin oocd_programdoit
else
# Program the device.  - lpc21isp will not work for SAM7
program: $(TARGET).hex
	@echo
	@echo $(MSG_LPC21_RESETREMINDER)
	$(LPC21ISP) $(LPC21ISP_OPTIONS) $(LPC21ISP_FLASHFILE) $(LPC21ISP_PORT) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)
endif
endif


# Create final output file (.hex) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O ihex $< $@
	$(OBJCOPY) -O srec $< $*.mot
	
# Create final output file (.bin) from ELF output file.
%.bin: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O binary $< $@


# Create extended listing file from ELF output file.
# testing: option -C
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -m sh2a -d $< > $@

#	$(OBJDUMP) -m sh2a -h -S -C $< > $@


# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM)

ifdef CMDLINE_VERBOSE
#
%.elf:  $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM) --output $@ $(LDFLAGS)
#
else
#
%.elf:  $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM)
	@echo $(MSG_LINKING) $@
	@$(CC) $(ALL_CFLAGS) $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM) --output $@ $(LDFLAGS)
#
endif


# Compile: create object files from C source files. ARM/Thumb
ifdef CMDLINE_VERBOSE
#
$(COBJ) : %.o : %.c
	$(CC) -c $(ALL_CFLAGS) $(CONLYFLAGS) $< -o $@ 
#
else
#
$(COBJ) : %.o : %.c
	@echo $(CC) -c $<
	@$(CC) -c $(ALL_CFLAGS) $(CONLYFLAGS) $< -o $@ 
#
endif

#	@echo
#	@echo $(MSG_COMPILING) $<

# Compile: create object files from C source files. ARM-only
$(COBJARM) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING_ARM) $<
	$(CC) -c $(ALL_CFLAGS) $(CONLYFLAGS) $< -o $@ 

# Compile: create object files from C++ source files. ARM/Thumb
$(CPPOBJ) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	$(CPP) -c $(ALL_CFLAGS) $(CPPFLAGS) $< -o $@ 

# Compile: create object files from C++ source files. ARM-only
$(CPPOBJARM) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $<
	$(CPP) -c $(ALL_CFLAGS) $(CPPFLAGS) $< -o $@ 


# Compile: create assembler files from C source files. ARM/Thumb
## does not work - TODO - hints welcome
##$(COBJ) : %.s : %.c
##	$(CC) $(THUMB) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files. ARM/Thumb
$(AOBJ) : %.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Assemble: create object files from assembler source files. ARM-only
#$(AOBJARM) : %.o : %.S
ifdef CMDLINE_VERBOSE
#
$(AOBJARM) : %.o : %.s
	@echo
	@echo $(MSG_ASSEMBLING_ARM) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@
#
else
#
$(AOBJARM) : %.o : %.s
	@echo $(CC) -c $< 
	@$(CC) -c $(ALL_ASFLAGS) $< -o $@
#
endif


# Target: clean project.
clean: begin clean_list finished end

cleanall: clean
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).mot


clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(COBJ)
	$(REMOVE) $(CPPOBJ)
	$(REMOVE) $(AOBJ)
	$(REMOVE) $(COBJARM)
	$(REMOVE) $(CPPOBJARM)
	$(REMOVE) $(AOBJARM)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRCARM:.c=.d)
	$(REMOVE) $(CPPSRC:.cpp=.d)
	$(REMOVE) $(CPPSRCARM:.cpp=.d)
	$(REMOVEDIR) .dep | exit 0
	$(REMOVE) *.hex
	$(REMOVE) *.mot
	$(REMOVE) *.lss
	$(REMOVE) *.elf
	$(REMOVE) *.sym
	$(REMOVE) *.bin
	$(REMOVE) *.map

#	$(REMOVE) $(SRC:.c=.s)
#	$(REMOVE) $(SRCARM:.c=.s)
#	$(REMOVE) $(CPPSRC:.cpp=.s) 
#	$(REMOVE) $(CPPSRCARM:.cpp=.s) 

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
	build elf hex bin lss sym clean clean_list program dump

