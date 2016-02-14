#
#	Windows用です.
#
#	コンパイラは MinGw gccを使用します。
#	試してはいませんがcygwinでも大丈夫なはずです。
#
EXE_SUFFIX = .dll

O = .o
CC		= gcc
CFLAGS	= $(USBFLAGS) -DHAVE_CONFIG_H -std=gnu99 -I. -I./jtag -I./helper -I./jimtcl -O2 -Wall
# -mno-cygwin
LIBS	= $(USBLIBS)

TARGET	= cdclink

OBJS	=  main.o bitbang.o rxbuffer.o commands.o interface.o log.o packet.o rs232c.o


PROGRAM = $(TARGET).dll $(TARGET).lib

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(OBJS)
	dllwrap -mwindows -mno-cygwin --add-stdcall-alias -o $(TARGET).dll $(OBJS) -lsetupapi -Wl,--out-implib,$(TARGET).lib 

#	dllwrap -mwindows -mno-cygwin --enable-stdcall-fixup --add-stdcall-alias -o $(TARGET).dll $(OBJS) -lsetupapi -Wl,--out-implib,$(TARGET).lib 



inst:
	cp $(TARGET).dll ../../../OpenGL/gltest/
#	cp $(TARGET).dll ../

clean:
	rm -f *.o $(PROGRAM)

#
