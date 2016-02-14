/**********************************************************************
    ディレクトリ構成
 **********************************************************************
firmware ---+--- fm3     ---- Fujitsu FM3用
            +--- stm32   ---- STM32   CQ-STARM / STM8S-D / STBee / STBeeMini用.
            +--- rx62n   ---- Renesas FRK-RX62N用
            +--- sh2a    ---- Renesas SH2A用
            +--- lpc2388 ---- NXP CQ-FRK-NXP (LPC2388)用
            +--- mary    ---- NXP LPC1114 Mary用
            |
            |                 host側Tool
            +--- tools  --+-- cdctool -------- 簡易モニター
                          +-- w32term -------- RS232C転送ベンチマーク,無手順端末



/**********************************************************************
    ビルド方法など
 **********************************************************************

ファームウェアのビルド:

-基本的にファームウェアはgccクロスコンパイラでビルドします。
-ARM系の場合は、CodeSourcery G++ Lite 4.4.x を使用します。(Windows/Linux)
-RX62N,SH2Aの場合は、cygwin上でビルドされたgccクロスコンパイラを使用します。

-- Makefile が用意されています。GNU make を使用してビルドします。



参考ＵＲＬ

フリーARM用コンパイラの入手先など

CodeSourcery G++ Lite:
  https://sourcery.mentor.com/sgpp/lite/arm/portal/subscription3053

Maple IDE にも CodeSourcery G++ が含まれています。
  http://leaflabs.com/docs/maple-ide-install.html#download

SH2A用のCygwin-gcc
  http://www.kumikomi.net/interface/editors/2010/05/sh-2agcc.php

RX62N用のCygwin-gcc
  http://www.tokudenkairo.co.jp/rx62n/download.html


その他のフリーARM用コンパイラ

NETX-ARM-EABI-GCC
  http://sourceforge.jp/projects/sfnet_netx-arm-elf-gc/releases/?fulllist=1

YAGARTO GNU ARM toolchain
  http://www.yagarto.de/#download



 **********************************************************************

Windowsホスト側ツールのビルド:
-基本的にWindowsホスト側ツールはMinGW gccでビルドします。

-- 同じく Makefile が用意されています。GNU make を使用してビルドします。

MinGW gcc コンパイラの入手先

MinGW
  http://www.mingw.org/
  http://sourceforge.net/projects/mingw/files/


 **********************************************************************


/**********************************************************************
    ファームウェアのカスタマイズに関するヒント
 **********************************************************************

ファームウェアのカスタマイズに関するヒント　に関しましては、以下の
ドキュメントをお読みください。

firmware/fm3/src/FIRMWARE-HACKING-GUIDE.txt


 **********************************************************************
