■ SH2A USB-CDC monitor について

  これは、sh7264_usb_function_sample.zip をgcc対応させて、ブートローダー
  として使用できるようにしたものです。


■ 使い方

（１）ブートローダーの書き込み。
  SH2A-FFF82000.bin を USERROM.BIN にリネームして、シリアルEEPROMの
  後半32kB に書き込みます。 ( ツールとして SPIwriterUser を使用します。 )

  書き込んだ後、JPP2クローズして再接続してください。
  
  接続確認は cdctool/cdctool.exe を用います。
  ブートローダーは cdctool/cdcboot.exe を使用してください。

（２）USB-CDCフレームワークとしての使用
  a.bat を実行すると、SH2A-1C000000.mot を 1C000000番地にロードして実行します。


（２）USB-CDCエコーバックテスト
  usbmain.c 内の定義を
  
  #define	ECHOBACK_TEST				(1)

  に変更してビルドしたものを実行すると、ファームウェア内でのエコーバックテスト
  専用のファームになります。
  適当な無手順端末を起動してエコーバックテストを実行してください。


■ 現時点での問題。

   FPUが使えません。


■ FPU 問題。

  なぜか FPUが使えません。
  gccのコンパイルオプション -m2a-nofpu を与えてください。

  fpscrレジスタ絡みの問題があるように思います。
  gccが古いせいかもしれません。

■ Windowsホストツールについて。

  cdctool.exe で逆アセンブルを実行するとARMと思って逆アセンブルする不具合があります。

  cdcboot.exe はihexとmotファイルしか解釈しません。
  cdcboot.exe がブート出来るエリアは0x1C00_0000～1MB の空間のみです。


■ 割り込みについて。

  割り込みハンドラーは USBとCMT0の２つを使用しています。
  それ以外の割り込みは全部Dummyに飛ばすことでメモリー節約しています。(vect_alias.h)

  さらに割り込みが必要になった場合は、common/vect_alias.h にある Dummyへの置き換えを
  （一部、あるいは全部）解除して使用してください。


■ ブートローダーをアプリとして使う方法。

  ブート領域FFF8_2000ターゲットにする代わりに 通常の1MB SRAM領域 1C00_0000 に
  バイナリーを作って、ロードすることでアプリケーションとして使えます。
  
  このディレクトリで普通にmakeしてください。
  make後、g.bat で書き込んでください。
  
  
■ ビルドする方法

  CQのサイトにあるCygwin版gccを使います。
  
  gccは cygwinをインストールしたディレクトリ（仮に D:\cygwin とします）
  から相対で usr/local/sh-tools に置きます。
  ( 上の例では D:\cygwin\usr\local\sh-tools\ となります )


■ 割り込みハンドラーのＣ記述/ASM記述の切り替えについて

Ｃ記述にする場合はcommon/intprg.c の#define INTASM をコメントアウト。

//	実際のハンドラーはsh2int.S アセンブラで定義する.
//
#define	INTASM

それから、Makefileの ASRCARM = の項、sh2int.S を消します。

※ gccの不具合により、FPU使用オプション -m2a 時はＣ記述するとレジスタが
壊れるために正常動作しません。



■ RESBANKについて

レジスタバンクの有効/無効はMakefileで切り替えています。

CDEFS =  -D$(RUN_MODE) -DUSE_RESBANK

FPUを使用する場合は、割り込みハンドラーをASM記述にする必要があります。
また、ASM記述の都合上、USE_RESBANK指定が必須になります。

※ レジスタバンク有効にした場合、15レベル以上の多重割り込みでバンクがオーバーフロー
しますので、基本的には多重割り込みを発生させないようにプログラムしてください。


■ FPUの使用について

FPUの有効/無効もMakefileで切り替えています。

# FPUを使う.
#
#USE_FPU = YES

※ 上記コメントアウトを外すとFPUを使用するようになりますが、現状のgccとnewlibに
おいて何らかのバグがあり、printf("%g",x)などが正常に動きません。
とりあえず、USE_FPUはコメントアウトしたままでのご使用をお勧めします。


■ KPIT版 sh-elf-gcc使用について

Cygwin版 gccのかわりにKPIT版 sh-elf-gccを使用する場合は Makefile-kpit-gcc
を使用してください。

KPIT版 sh-elf-gccを使用する場合は FPU 使用可能です。


■ usercmd() について。

cdctool.exe から userコマンドを実行すると、monit/usercmd.c 内のusercmd関数が
呼び出されます。ここに簡単なテストプログラムを書くことで、ファームウェア内
でのいろんなテストを気軽に行なうことが出来ます。
printfを有効にした場合は、libcの多くの関数がリンクされますので、コード量が
倍増します。（その場合32kBに入らないのでROM化出来ません）

printfでcdctool.exeのコンソールに出力する経路は、以下のようになっています。
- printf() =高レベルstdio
- write()  =低レベルI/O
- __trap34() システムコール呼び出し( ASMソース内では ___trap34: )
     実際には trapa #34 を行なわずに、sh2int.S内にて横取り
- trap_write() 呼び出し
- _user_putc() 呼び出し
- USBputch() 呼び出し
- ホスト側コンソール印字




ファイル一覧
------------------------
usb_firm/src/p_main.c
main.c
usbmain.c
debug.c
gpio.c
sh2asm.c
sh2int.S
sh2fpint.S
monitor/board.c
monitor/fifo.c
monitor/gpio.c
monitor/monit.c
monitor/picwrt.c
monitor/usbio.c
monitor/usercmd.c
usb_firm/src/c_datatbl.c
usb_firm/src/common/c_dataio.c
usb_firm/src/common/c_global.c
usb_firm/src/common/c_intrn.c
usb_firm/src/common/c_lib7264.c
usb_firm/src/common/c_libint.c
usb_firm/src/common/c_usbint.c
usb_firm/src/common/c_usb_dly.c
usb_firm/src/common/c_usb_dma.c
usb_firm/src/peri/p_changeep.c
usb_firm/src/peri/p_classvendor.c
usb_firm/src/peri/p_controlrw.c
usb_firm/src/peri/p_descrip.c
usb_firm/src/peri/p_intrn.c
usb_firm/src/peri/p_lib7264.c
usb_firm/src/peri/p_libint.c
usb_firm/src/peri/p_status.c
usb_firm/src/peri/p_stdreqget.c
usb_firm/src/peri/p_stdreqset.c
usb_firm/src/peri/p_usbint.c
usb_firm/src/peri/p_usbsig.c
common/cpg.c
common/dbsct.c
common/hwsetup.c
common/intprg.c
common/resetprg.c
common/sbrk.c
common/siochar.c
common/vecttbl.c
