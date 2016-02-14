
make clean
make USE_CP2102=Yes

make clean
make 

goto end1

make ROMADRS=0000 clean
make ROMADRS=0000
make ROMADRS=0000 clean

make ROMADRS=2000 clean
make ROMADRS=2000
make ROMADRS=2000 clean

:end1
