#!/bin/sh

cp CQ_STARM-3000.bin    2
cutbin -f12,01,00       -l20  2 devd.bin
cutbin -f00,09,02          -l128 2 cnfd.bin

xdump devd.bin >devd.dmp
xdump cnfd.bin >cnfd.dmp
