#!/bin/sh
gcc -g -O -c ./lib/OLEDDisplay.cpp ./lib/Wire.cpp
g++ main.cpp OLEDDisplay.o Wire.o
