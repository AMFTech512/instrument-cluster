#ifndef GPIO_H
#define GPIO_H

#include <inttypes.h>

class GPIO {
    public:

        static void initGPIO(uint8_t pin);
        static void writePin(uint8_t pin, bool isOne);
};


#endif