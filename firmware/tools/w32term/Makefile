#
#	Windows用です.
#
#	コンパイラは minGw gccを使用します。
#	試してはいませんがcygwinでも大丈夫なはずです。
#
#	ライブラリ libusb が必要です。
#
#	libusb をインストールしたディレクトリを 
#	下の変数にセットしてください.
#USBFLAGS = -I/usr/local/include
#USBLIBS = -L/usr/local/lib/gcc

# -lusb


EXE_SUFFIX = .exe

CC		= gcc
CFLAGS	= $(USBFLAGS) -O2 -Wall
LIBS	= $(USBLIBS)

TARG = w32term
OBJS = w32term.o rs232c.o fifo.o util.o rsbench.o
#thread.o

PROGRAM = $(TARG)$(EXE_SUFFIX)

all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJS)
	$(CC) -s -o $(PROGRAM) $(OBJS) $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)

test:
#	w32term -b921600 -c5 -t
	w32term -b460800 -c5 -t
#	w32term -b230400 -c5 -t
#	w32term -b115200 -c5 -t
#	w32term -b19200 -c5 -t

#
test6:
	w32term -b921600 -c6 -t
#	w32term -b460800 -c6 -t
#	w32term -b230400 -c6 -t
#	w32term -b100400 -c6 -t
