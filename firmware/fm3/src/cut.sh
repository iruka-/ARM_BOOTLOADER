#!/bin/sh

cp FM3-0000.bin    2
cutbin -f12,01,00       -l20  2 devd.bin
cutbin -f00,09,02       -l128 2 cnfd.bin

xdump devd.bin >devd3.dmp
xdump cnfd.bin >cnfd3.dmp
