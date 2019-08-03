#ifndef GAUGE_H
#define GAUGE_H

#include "Wire.h"
#include <inttypes.h>
#include "SSD1306Wire.h"

#define SYS_OSC_OFF 0x20
#define SYS_OSC_ON 0x21

#define ROW_DRV_OUTPUT 0xA0

#define DISP_OFF 0x80
#define DISP_ON 0x81

// OR with 4-bit brightness value
#define BRGHT_SET 0xe0

#define DISP_ADDRESS 0x3c

class Gauge {

    public: 
        Gauge(uint8_t dispAddress = DISP_ADDRESS);
        void setLeds(uint16_t disp);
        void setMaxVal(uint32_t val);
        void setVal(uint32_t val);
        void setNeedle(uint32_t val);
        void setI2cAddress(uint8_t addr);
        void setBrightness(uint8_t val);
        void dispOn();
        void dispOff();
        SSD1306Wire* getOled();

        uint8_t gauge_id;

    private:
        uint8_t led_addr = 0x70;
        uint32_t maxVal = 100;
        uint32_t val = 0;
        uint16_t disp = 0;
        uint8_t brightness = 0xf;

        SSD1306Wire *oled_disp;

        void sendCommand(uint8_t command);

        static uint8_t num_gauges;
        
        
};

#endif