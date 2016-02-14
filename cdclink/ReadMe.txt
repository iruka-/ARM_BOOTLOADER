■ 概要

   これは、OpenOCD の DLL ハック（実験）です。
   即ち、外部に DLL を置いて、JTAGアダプターのドライバーの分離実装を試みています。

   JTAGアダプターとの通信はWindows汎用のUSB-CDCデバイス（シリアル通信）で行います。

■ 現在のステータス

   各種ARM基板を使用したJTAGアダプターが動作しています。

   - CQ-STARM (STM32)
   - CQ-FRK-NXP-ARM (LPC2388)
   - MARY基板 (LPC1114)

   通信速度はFT2232を用いたJTAGアダプターに比べると低速です。


  * ファームウェアの書き込み
  
  +----------------------------------------------+
  | 基板名         | ファームウェア(Hexファイル) |
  +----------------------------------------------+
  | CQ-STARM       | CQ_STARM-3000.hex           |
  | STBee          | STBEE-3000.hex              |
  | STBeeMini      | STBEE_MINI-3000.hex         |
  +----------------------------------------------+
  | MARY           | MARY-0000.hex               |
  | CQ-FRK-NXP-ARM | LPC2388-0000.hex            |
  +----------------------------------------------+

■ 試し方

  ターゲットマイコンの種類に合わせて、cq-binary ディレクトリにある、
  start-stm32.bat   (STM32マイコンのデバッグ用) か、
  start-fm3.bat     (FM3マイコンのデバッグ用) か、
  start-lpc2388.bat (LPC2388マイコンのデバッグ用) のどれかを
  WindowsのDOS窓から起動するか、Windowsのエクスプローラーから直接
  ダブルクリックで起動します。
  
  OpenOCDが起動すると、DOS窓に１０行程度のメッセージログが出て、ターゲット
  マイコンと接続したことを知らせてくれます。
  接続がうまくいかない場合はErrorのメッセージが出ています。

  英語メッセージなので分かりにくいですが、エラーした場合はその内容を
  よく読んでみてください。
  大抵の場合は配線ミスや電源供給の不具合でエラーになります。

  また、OpenOCDはlocalhost:3333番(GDB)や4444番(telnet)をListenする
  TCP/IPサーバーとして振舞います。

  起動直後にWindowsファイアーウォールなどが、OpenOCDのIP接続を検出して、
  それをブロックするか許可するかをユーザーに聞いてきますので、（最初の１回だけです）
  許可を選択してあげてください。

  OpenOCDは、DOS窓上にログをprintするだけの、そっけないコマンドラインツール
  に過ぎませんので、ユーザーがコントロールしたい場合はteratermなどのtelnet端末を
  起動して、localhost の 4444番ポートに telnet で接続します。
  
   正常に接続出来ているようでしたら、telnetで localhost:4444 番に接続して、OpenOCD
   コマンド (例: scan_chain など) を実行してみてください。


■ ディレクトリ構成

 cdclink --+- openocd -+  openocd.exe 本体 と cdclink.dll のソース.
                       |
                       +--helper\   ヘッダーファイル.
                       +--jtag\     ヘッダーファイル.
                       +--jimtcl\   ヘッダーファイル.
                       |
                       +--openocd_patch\  openocd本体側作成用の改造パッチ.

 firmware -+-- stm32\ ----- CQ-STARM用のfirmwareソース
           |
           +-- lpc2388\ --- CQ-FRK-NXP-ARM 用のfirmwareソース
           |
           +-- mary\ ------ MARY基板用のfirmwareソース


■ プログラムの再ビルド方法

   WindowsXP上のMinGW-gccコンパイラを用いて openocd/ ディレクトリにて makeしてください。
   makeすると、cdclink.dll が作成されます。

   openocd.exe本体を再ビルドする方法は、以下のURLを参照してください。

-http://hp.vector.co.jp/authors/VA000177/html/2010-09.html
   
   今回の改造部分ソースはopenocd_patch/ ディレクトリに置いています。

   Linux上でのビルドオプションは、こんな感じです。
   $ ./configure \
       --build=i686-pc-linux-gnu \
       --host=i586-mingw32msvc \
       --enable-maintainer-mode \
       --enable-dummy

   出来上がった openocd.exe 本体は、ドライバーとして、同一ディレクトリに存在する 
   cdclink.dll を起動時に呼び出します。

   (存在しなければ、dummyドライバーのみが組み込まれます)


■ ライセンス

   OpenOCDの配布ライセンスに準じます。


■ 展望

   cdclink.dll ファイルを(自力で)差し替えるだけで、自作デバイスがサポート可能になります。
   （たとえばATtiny2313を使用したJTAGアダプターなどをサポート出来る可能性があります）

   cdclink.dll のエントリーポイントは、
      DLL_int get_if_spec(struct jtag_command **q);
   だけです。引数のstruct jtag_command **qのqには、openocd本体のjtag_command_queueという
   グローバル変数のアドレスを渡します。
   戻り値は、(intになっていますが) ドライバー記述構造体のアドレスになります。





■ JTAGコマンドの追加とプロトコルについて

-hidcmd.h
 #define HIDASP_JTAG_WRITE	  0x18	//JTAG 書き込み.
 #define HIDASP_JTAG_READ	  0x19	//JTAG 読み書き.
-が追加されてます。

-HidReportの下りストリーム(PC->AVR) サイズは最大64バイトまでです.
 +------+------+-------------------+------+-------------------+------+-------------+-----+
 | 0x18 | jcmd |  data列           | jcmd |  data列           | jcmd |  data列     | 0x00|
 +------+------+-------------------+------+-------------------+------+-------------+-----+

--jcmdの1バイトは以下のように定義（その１）
 bit 7   6   5   4   3   2   1   0  
   +---+---+---+---+---+---+---+---+
   | 0 | b6| JTAG転送bit数(TDIの数)|   ＋ JTAG転送bit数分の TDIビット（LSBファースト）  
   +---+---+---+---+---+---+---+---+
--- b6はTDI送出の最終bitでTMSを1にするなら1 しないなら 0 ： TMSは最終bit以外は常時0

--jcmdの1バイトは以下のように定義（その２）
 bit 7   6   5   4   3   2   1   0  
   +---+---+---+---+---+---+---+---+
   | 1 |BITBANG転送回数(後続byte数)|   ＋ BITBANG転送回数分(byte数)のデータ
   +---+---+---+---+---+---+---+---+
--BITBANGデータの１回分は、TCK=LOWのサンプルとTCK=HIGHのサンプルをパックしたデータ。
 bit 7   6   5   4   3   2   1   0  
   +---+---+---+---+---+---+---+---+
   |TCK|TDI| - |TMS|TCK|TDI| - |TMS|  （上位４ビットが最初にセットされ、次に下位４ビットがセットされます）
   +---+---+---+---+---+---+---+---+
--TCKを変化させたくないときは、両方のTCKビットを同じ値にします。


-HidReportの登りストリーム(AVR->PC) サイズは最大64バイトまで. HIDASP_JTAG_READ発行時のみ折り返し返送されます.
 +----------------------------------------+
 | JTAG受信データ(TDOの読み取りビット列)  |  （最大64バイトまで）
 +----------------------------------------+
--ビット列はLSBファースト. 送信されたTDIビット列とそのまま対応しています.

----
その他補足
 #define HIDASP_JTAG_READ	  0x19	//JTAG 読み書き.
-を実行するときは、HidReportの下りストリーム(PC->AVR)を単純な形式（１コマンドのみ）にします。
 +------+------+-------------------+------+
 | 0x19 | jcmd |  data列           | 0x00 |
 +------+------+-------------------+------+
-jcmdがBitBangモードのときは、返答データはありません。JTAG(TDI列)のときのみ(TDO列)を返します。
-JTAGストリームが長い場合(56bit以上のTDIを送ってTDOを受け取る)は、56bit単位に分割転送します。
-その場合、最後のストリームの最終bitのみ、TMSを1にする処理が入ります。(b6=1のパケットを用意します)


-armblasterのファームを再ビルドするときは、CodeSourcery G++ Lite を使用してください。


■ OpenOCDの簡単な使い方 

-起動したらまず、localhost:4444番ポートにtelnetで(TeraTermなどを使って)繋いでください。

-TeraTermから、以下のようなコマンドをタイプすると、結果が表示されます。

|OpenOCDコマンド|意味|
|scan_chain		|接続されているＴＡＰのリストを見る。|
|reset halt		|ターゲットCPUをHALTさせる|
|reg			|ターゲットCPUのレジスタを見る|
|mdw アドレス カウント|display memory words <addr> [count]メモリーダンプ|
|step			|ＣＰＵをステップ実行させる|
|flash write_image erase main.hex	|main.hexをFLASH ROMに書き込む(そのまえにCPUをHALTにしておきます)|


■ 参考文献(LINK)

OpenOCDが動くまで (kimura Lab)
-http://www.kimura-lab.net/wiki/index.php/OpenOCD%E3%81%8C%E5%8B%95%E3%81%8F%E3%81%BE%E3%81%A7

OpenOCD (ベストテクノロジーさん)
-http://www.besttechnology.co.jp/modules/knowledge/?OpenOCD

OpenOCD本家
-http://openocd.berlios.de/web/
