rem w32term -c3 -b115200 -USH2A-0000.bin
rem w32term -c3 -b115200 -s -USH2A-1C000000.bin
rem w32term -c7 -b115200 -s -USH2A-1C000000.bin

..\cdctool\cdcboot.exe -r SH2A-1C000000.mot

sleep 3

..\cdctool\cdctool.exe -iscript


