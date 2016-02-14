rem ChaNさんの lpcsp を使って USBシリアル変換チップ経由でファームを書きこみます.
rem CQ-FRK-NXP-ARM 基板上のJP2をクローズして通電する必要があります.
rem lpcsp.ini にCP2102のCOMポート番号とボーレートなどを適切に記述してください.

rem http://elm-chan.org/works/sp78k/report.html
rem

lpcsp.exe LPC2388-0000.hex
