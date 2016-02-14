
CSRCS    = download_sample.c
TARGET   = download_sample
ASMSRCS  = startup.s
OBJS     = $(ASMSRCS:%.s=%.o) $(CSRCS:%.c=%.o)
LDSCRIPT = memory.def

GCC = /usr/local/sh-tools/bin/sh-elf-gcc
LD = /usr/local/sh-tools/bin/sh-elf-ld
AS = /usr/local/sh-tools/bin/sh-elf-as
OBJCOPY = /usr/local/sh-tools/bin/sh-elf-objcopy

all: $(OBJS) $(LDSCRIPT)
	$(LD) -Map $(TARGET).map -T $(LDSCRIPT) $(OBJS) -L$(GCCLIB) -lgcc -o $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
.c.o:
	$(GCC) -c -g -m2a $<
.s.o: 
	$(AS) $< -g -o $@
clean:
	rm *.o $(TARGET).elf $(TARGET).map $(TARGET).bin
