rem ======== �܂��Acdclink.dll�����.

make clean
make all

rem ======== ���ɁASTM32�t�@�[�����쐬����.

cd ..\..\firmware\stm32\src\monitor

rem ======== �\�[�X�̓��������.

call src-sync.bat

cd ..

rem ======== STM32�t�@�[����S���쐬����.

call mkall.bat

rem ======== LPC2388�t�@�[����S���쐬����.

cd ..\..\lpc2388\firmware

call mkall.bat

rem ======== MARY�t�@�[�����쐬����.

cd ..\..\mary\firmware

make clean
make

rem ======== FM3�t�@�[�����쐬����.

cd ..\..\fm3\src

make clean
make

rem ======== cq-binary�ɏW�߂�.

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

