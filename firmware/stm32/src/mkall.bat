rem ４種の基板用HEXを hexfiles/ 以下に生成します。

del hexfiles\*.hex

make clean
make BASEBOARD=STBEE ROMADRS=3000
copy STBEE-3000.hex hexfiles

rem goto end1

make clean
make BASEBOARD=STBEE_MINI ROMADRS=3000
copy STBEE_MINI-3000.hex hexfiles

make clean
make BASEBOARD=CQ_STARM ROMADRS=3000
copy CQ_STARM-3000.hex hexfiles
copy CQ_STARM-3000.elf hexfiles
copy CQ_STARM-3000.bin hexfiles

make clean
make BASEBOARD=STM8S_D ROMADRS=3000
copy STM8S_D-3000.hex hexfiles

rem ブートローダー版も作成します.

make clean
make BASEBOARD=STM8S_D ROMADRS=0000
copy STM8S_D-0000.hex hexfiles

make clean
make BASEBOARD=CQ_STARM ROMADRS=0000
copy CQ_STARM-0000.hex hexfiles

:end1
