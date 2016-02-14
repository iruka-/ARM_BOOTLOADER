--------------------------------------------------------------------------------
使用しましたUSBフレームワークの著作権は富士通が保有しています.
   	  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
	チップ固有ヘッダーの権利は富士通が保有しています.
   (C) Fujitsu Semiconductor Europe GmbH (FSEU) 
	  Cortex-M3 用CMSISの権利はARMが保有しています.
 	* Copyright (C) 2009 ARM Limited. All rights reserved.

今回新たに作成した、Copyright表記の無いソースに関してはPDS扱いにします。




--------------------------------------------------------------------------------
開発環境の入手(ARMコンパイラの入手)

参考ＵＲＬ

フリーARM用コンパイラの入手先など

CodeSourcery G++ Lite:
  https://sourcery.mentor.com/sgpp/lite/arm/portal/subscription3053

Maple IDE にも CodeSourcery G++ が含まれています。
  http://leaflabs.com/docs/maple-ide-install.html#download


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



/**********************************************************************
    ファームウェアのカスタマイズに関するヒント
 **********************************************************************

ファームウェアのカスタマイズに関するヒント　に関しましては、以下の
ドキュメントをお読みください。

FIRMWARE-HACKING-GUIDE.txt


--------------------------------------------------------------------------------
[EOF]
