#include <fstream>
#include <iostream>

#include "GPIO.h"

// we're writing to GPIO 17

using namespace std;

void initGPIO() {
    static bool hasInit = false;

    if(hasInit)
        return;

    ofstream expFile;
    expFile.open("/sys/class/gpio/export");
    expFile << "17";
    expFile.close();

    ofstream dirFile;
    dirFile.open("/sys/class/gpio/gpio17/direction");
    dirFile << "out";
    dirFile.close();

    hasInit = true;
}

void write1() {

    ofstream wFile;
    wFile.open("/sys/class/gpio/gpio17/value");
    wFile << "1";
    wFile.close();
    
}

void write0() {

    ofstream wFile;
    wFile.open("/sys/class/gpio/gpio17/value");
    wFile << "0";
    wFile.close();
    
}