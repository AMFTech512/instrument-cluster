#include "Selector.h"
#include "GPIO.h"

std::vector<uint8_t> Selector::pins;

void Selector::select(uint8_t dev) {

    for(int i = 0; i < pins.size(); i++) {
        GPIO::writePin(pins.at(i), (dev >> i) & 1);
    }

}

void Selector::addPin(uint8_t pin) {
    GPIO::initGPIO(pin);
    pins.push_back(pin);
}