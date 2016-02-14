# ARM Cortex-M3 HOBBY BOOTLOADER Project

cdctool/README-ja.txt
  Mapleという、ARM用の Arduinoに似た開発環境での使用を想定した、
  USB-CDCクラスのブートローダーです。

firmware/ReadMe.txt
  ARM Cortex-M3 の各種開発ボードに対応した、
  USB-CDCクラスのブートローダーです。

firmware/sh2a/src/ReadMe.txt

firmware/stm32/ReadMe.txt

firmware/stm32/src/readme.txt

firmware/lpc1114/HWLib/CMSISv1p30_LPC11xx/cmsis_readme.txt

firmware/lpc2388/ReadMe.txt

firmware/rx62n/src/ReadMe.txt

firmware/fm3/Readme.txt

firmware/fm3/src/ReadMe.txt

firmware/tools/w32term/ReadMe.txt
   Windows コマンドラインで使用できる、簡易的なシリアル通信ソフトです。

cdcboot/README-ja.txt
これは、libmaple 
		（leaflabsが開発し、mapleで使われているSTM32用のArduino風ライブラリ）
をベースにしたＣ言語専用のフレームワークです。（Ｃ＋＋言語サポートを削除しています）


CQ-STARM , STM8S-DISCOVERY , STBee , STBeeMini の基板に対応しています。

cdcboot/README

cdclink/ReadMe.txt

cdclink/openocd/ReadMe.txt
   これは、OpenOCD の DLL ハック（実験）です。
   即ち、外部に DLL を置いて、JTAGアダプターのドライバーの分離実装を試みています。

   JTAGアダプターとの通信はWindows汎用のUSB-CDCデバイス（シリアル通信）で行います。

