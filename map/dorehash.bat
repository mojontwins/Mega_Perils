@echo off
rehashmap %1.txt 6 6 99 bin 3rd
copy map.0.bin ..\dev\%1map.0.bin
copy map.1.bin ..\dev\%1map.1.bin
