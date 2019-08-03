#!/bin/sh
arm-linux-gnueabihf-gcc -g -O -c ../lib/GPIO.cpp ../lib/Selector.cpp ../lib/Gauge.cpp ../lib/OLEDDisplay.cpp ../lib/Wire.cpp -std=c++11
arm-linux-gnueabihf-g++ ../main.cpp OLEDDisplay.o Wire.o Gauge.o GPIO.o Selector.o -static -std=c++11
# sudo cp a.out /media/austin/911e138f-fc3d-4524-addd-ac745316121c/root/
# sync