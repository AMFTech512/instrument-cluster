#include "Gauge.h"
#include <stdio.h>
#include "Selector.h"

uint8_t Gauge::num_gauges = 0;

Gauge::Gauge(uint8_t dispAddress) {

    static bool hasInitPins = false;

    if(!hasInitPins) {
        Selector::addPin(17);
        hasInitPins = true;
    }

    this->gauge_id = num_gauges;
    Gauge::num_gauges++;

    Wire.begin(this->led_addr);

    sendCommand(SYS_OSC_ON);
    sendCommand(ROW_DRV_OUTPUT);
    setBrightness(0xf);
    setLeds((uint16_t) 0);
    dispOn();

    Selector::select(this->gauge_id);
    this->oled_disp = new SSD1306Wire(dispAddress, GEOMETRY_128_32);
    this->oled_disp->init();
}

void Gauge::setLeds(uint16_t mDisp) {
    Selector::select(this->gauge_id);
    this->disp = mDisp;

    uint8_t right = mDisp & 0xff;
    uint8_t left = mDisp >> 8;

    Wire.beginTransmission(this->led_addr);
    Wire.write((uint8_t) 0x00);
    Wire.write(right);
    Wire.write((uint8_t) 0x00);
    Wire.write(left);
    Wire.endTransmission();
}

void Gauge::setNeedle(uint32_t val) {

    Selector::select(this->gauge_id);

    this->val = val;

    float black = (16.0 * (1.0 - ( (float) val / (float) this->maxVal )));
    uint16_t new_disp = 0x1 << ((uint16_t) (black));

    setLeds(new_disp);
}

void Gauge::setVal(uint32_t val) {

    Selector::select(this->gauge_id);

    this->val = val;

    float black = (16.0 * (1.0 - ( (float) val / (float) this->maxVal )));
    uint16_t new_disp = 0xffff << ((uint16_t) (black));

    setLeds(new_disp);
}

void Gauge::setMaxVal(uint32_t val) {
    this->maxVal = val;
}

void Gauge::dispOn() {

    Selector::select(this->gauge_id);
    sendCommand(DISP_ON);
}

void Gauge::dispOff() {

    Selector::select(this->gauge_id);
    sendCommand(DISP_OFF);
}

void Gauge::setBrightness(uint8_t val) {

    Selector::select(this->gauge_id);

    if(val > 0xf) return;

    this->brightness = val;
    sendCommand(BRGHT_SET | val);
}

void Gauge::setI2cAddress(uint8_t addr) {
    this->led_addr = addr;
}

void Gauge::sendCommand(uint8_t command) {

    Selector::select(this->gauge_id);

    Wire.beginTransmission(this->led_addr);
    Wire.write(command);
    Wire.endTransmission();
}

SSD1306Wire* Gauge::getOled() {
    Selector::select(this->gauge_id);
    return this->oled_disp;
}