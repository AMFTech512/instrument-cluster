#include <fstream>
#include <iostream>
#include "sys/stat.h"
#include <cstring>
#include "GPIO.h"

using namespace std;

void GPIO::initGPIO(uint8_t pin) {

    struct stat dir;
    string filename = "/sys/class/gpio/gpio" + to_string(pin);
    if(stat(filename.c_str(), &dir) == 0)
        return;

    ofstream expFile;
    expFile.open("/sys/class/gpio/export");
    expFile << to_string(pin);
    expFile.close();

    ofstream dirFile;
    dirFile.open("/sys/class/gpio/gpio" + to_string(pin) + "/direction");
    dirFile << "out";
    dirFile.close();

}

void GPIO::writePin(uint8_t pin, bool isOne) {

    ofstream wFile;
    wFile.open("/sys/class/gpio/gpio" + to_string(pin) + "/value");
    
    if(isOne)
        wFile << "1";
    else
        wFile << "0";

    wFile.close();
    
}