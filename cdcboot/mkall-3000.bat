rem ÇSéÌÇÃäÓî¬ópHEXÇ hexfiles/ à»â∫Ç…ê∂ê¨ÇµÇ‹Ç∑ÅB

make clean
make BOARD=STBee ROMADRS=3000
copy build\STBee-3000.hex hexfiles

make clean
make BOARD=STBeeMini ROMADRS=3000
copy build\STBeeMini-3000.hex hexfiles

make clean
make BOARD=CQSTARM ROMADRS=3000
copy build\CQSTARM-3000.hex hexfiles

make clean
make BOARD=DISCOVERY ROMADRS=3000
copy build\DISCOVERY-3000.hex hexfiles

