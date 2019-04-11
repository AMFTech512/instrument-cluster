#!/bin/sh
/home/austin/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc -g -O -c ./lib/Gauge.cpp ./lib/OLEDDisplay.cpp ./lib/Wire.cpp -std=c++11
/home/austin/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++ main.cpp OLEDDisplay.o Wire.o Gauge.o -static -std=c++11
# sudo cp a.out /media/austin/911e138f-fc3d-4524-addd-ac745316121c/root/
# sync