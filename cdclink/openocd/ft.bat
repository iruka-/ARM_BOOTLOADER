rem 
rem カレントDIRにある main-0000.hexをSTM8S-DのSTM32CPUに書き込んで自動的に終了する.
rem
rem 

openocd-nogdb-hacks.exe -f hjlink.cfg -f stm32f1x.cfg -f batch.cfg

rem pause