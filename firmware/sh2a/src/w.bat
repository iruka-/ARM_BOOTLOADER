make clean
make -j 4 ROMADRS=FFF82000
copy SH2A-FFF82000.bin  ..\..\..\sh2a\SPIwriterUser\USERROM.bin


