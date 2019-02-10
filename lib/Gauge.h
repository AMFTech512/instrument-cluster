#ifndef GAUGE_H
#define GAUGE_H

#include "Wire.h"
#include <inttypes.h>

#define SYS_OSC_OFF 0x20
#define SYS_OSC_ON 0x21

#define ROW_DRV_OUTPUT 0xA0

#define DISP_OFF 0x80
#define DISP_ON 0x81

// OR with 4-bit brightness value
#define BRGHT_SET 0xe0

class Gauge {

    public: 
        Gauge();
        void setLeds(uint16_t disp);
        void setMaxVal(uint32_t val);
        void setVal(uint32_t val);
        void setI2cAddress(uint8_t addr);
        void setBrightness(uint8_t val);
        void dispOn();
        void dispOff();

    private:
        uint8_t i2c_address = 0x70;
        uint32_t maxVal = 100;
        uint32_t val = 0;
        uint16_t disp = 0;
        uint8_t brightness = 0xf;

        void sendCommand(uint8_t command);
};

#endif