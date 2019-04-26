#ifndef SELECTOR_H

#include <vector>
#include <inttypes.h>

#define SELECTOR_H

class Selector {

    public:
        static void select(uint8_t dev);
        static void addPin(uint8_t pin);

    private:
        static std::vector<uint8_t> pins;

};

#endif