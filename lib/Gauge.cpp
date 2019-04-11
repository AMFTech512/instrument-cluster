#include "Gauge.h"
#include <stdio.h>

Gauge::Gauge() {

    Wire.begin(this->i2c_address);

    sendCommand(SYS_OSC_ON);
    sendCommand(ROW_DRV_OUTPUT);
    setBrightness(0xf);
    setLeds((uint16_t) 0);
    dispOn();
}

void Gauge::setLeds(uint16_t mDisp) {

    this->disp = mDisp;

    uint8_t right = mDisp & 0xff;
    uint8_t left = mDisp >> 8;

    Wire.beginTransmission(this->i2c_address);
    Wire.write((uint8_t) 0x00);
    Wire.write(right);
    Wire.write((uint8_t) 0x00);
    Wire.write(left);
    Wire.endTransmission();
}

void Gauge::setVal(uint32_t val) {
    this->val = val;

    float black = (16.0 * (1.0 - ((float)val/(float)this->maxVal)));
    uint16_t new_disp = 0xffff << ((uint16_t) (black));

    setLeds(new_disp);
}

void Gauge::setMaxVal(uint32_t val) {
    this->maxVal = val;
}

void Gauge::dispOn() {
    sendCommand(DISP_ON);
}

void Gauge::dispOff() {
    sendCommand(DISP_OFF);
}

void Gauge::setBrightness(uint8_t val) {

    if(val > 0xf) return;

    this->brightness = val;
    sendCommand(BRGHT_SET | val);
}

void Gauge::setI2cAddress(uint8_t addr) {
    this->i2c_address = addr;
}

void Gauge::sendCommand(uint8_t command) {
    Wire.beginTransmission(this->i2c_address);
    Wire.write(command);
    Wire.endTransmission();
}