rem ４種の基板用HEXを hexfiles/ 以下に生成します。

del hexfiles\*.hex

mkall-3000.bat
exit

make clean
make BOARD=STBee ROMADRS=0000
copy build\STBee-0000.hex hexfiles

make clean
make BOARD=STBeeMini ROMADRS=0000
copy build\STBeeMini-0000.hex hexfiles

make clean
make BOARD=CQSTARM ROMADRS=0000
copy build\CQSTARM-0000.hex hexfiles

make clean
make BOARD=DISCOVERY ROMADRS=0000
copy build\DISCOVERY-0000.hex hexfiles

mkall-3000.bat
