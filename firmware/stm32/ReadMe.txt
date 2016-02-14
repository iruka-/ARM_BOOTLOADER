■ 概要

  これは、ＣＱ出版社雑誌 DesignWaveマガジン2008-5月号の ARM Cortex-M3 付録基板
  の仮想COMポートサンプルファームウェアです。

  ネタ元は、STMicroelectronicsが提供しているUSBアプリケーションフレームワーク
  "UM0424 STM32F10xxx USB development kit" に含まれている仮想COMポート用の
  ファームウェアを、Code Sourcery G++ Lite ( GNU Makefile ) でビルド出来るように
  再構成し直しました。


■ 現在のステータス

  ・Code Sourcery G++ Lite でビルドが通せるようになっています。
  ・WindowsPCに接続すると仮想COMポートとして認識されます。


■ ディレクトリ構成

src/		. . . . . . . . . 仮想COMサンプルソース(ここでMakeします)
inc/		. . . . . . . . . 仮想COMサンプルソース用のヘッダーファイル
driver/		. . . . . . . . . WindowsXP用のinfファイル

--------------

HWLib/			. . . . . . . . . ライブラリディレクトリ
HWLib/EVAL/                         EVALボード用のライブラリソース、ヘッダー
HWLib/CMSIS/                        CMSIS用ライブラリソース、ヘッダー
HWLib/STM32F10x_StdPeriph_Driver/   STM32F10x ペリフェラルライブラリソース、ヘッダー
HWLib/STM32_USB-FS-Device_Driver/   USB FSデバイス ライブラリソース、ヘッダー

