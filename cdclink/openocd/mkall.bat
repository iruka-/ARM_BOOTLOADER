rem ======== まず、cdclink.dllを作る.

make clean
make all

rem ======== 次に、STM32ファームを作成する.

cd ..\..\firmware\stm32\src\monitor

rem ======== ソースの同期を取る.

call src-sync.bat

cd ..

rem ======== STM32ファームを全部作成する.

call mkall.bat

rem ======== LPC2388ファームを全部作成する.

cd ..\..\lpc2388\firmware

call mkall.bat

rem ======== MARYファームを作成する.

cd ..\..\mary\firmware

make clean
make

rem ======== FM3ファームを作成する.

cd ..\..\fm3\src

make clean
make

rem ======== cq-binaryに集める.

cd ..\..\..\cq-binary

copy ..\cdclink\openocd\openocd*.exe
copy ..\cdclink\openocd\cdclink.dll

copy ..\firmware\tools\cdctool\*.exe


cd firmware

copy ..\..\firmware\stm32\src\hexfiles\*.*
copy ..\..\firmware\lpc2388\firmware\LPC2388-0000.hex
copy ..\..\firmware\lpc2388\firmware\LPC2388-0000.bin
copy ..\..\firmware\lpc2388\firmware\LPC2388-0000.elf
copy ..\..\firmware\lpc2388\firmware\LPC2388-USE-CP2102-0000.hex
copy ..\..\firmware\mary\firmware\MARY-0000.hex

copy ..\..\firmware\rx62n\src\*.mot
copy ..\..\firmware\sh2a\src\*.bin
copy ..\..\firmware\sh2a\src\*.mot
copy ..\..\firmware\fm3\src\*.hex

