#include <iostream>
#include <unistd.h>
#include "lib/SSD1306Wire.h"
#include "lib/Gauge.h"
#include "lib/GPIO.h"

using namespace std;

int main() {

    Gauge gauge1;
    Gauge gauge2;

    gauge1.setBrightness(0x8);
    

    gauge1.oled_disp->setTextAlignment(TEXT_ALIGN_CENTER);
    uint8_t *font = OLEDDisplay::loadFont("fonts/Orbitron_Medium_16.olf");
    gauge1.oled_disp->setContrast(0xff, 241, 0x40);
    gauge1.oled_disp->setFont(font);
    gauge1.oled_disp->drawString(64, 0, "LS1 Powered");
    gauge1.oled_disp->display();

    usleep(1000000);

    uint16_t buff = 1;

    for(int i = 0; i < 16; i++) {
        gauge1.setLeds(buff);
        buff <<= 1;
        usleep(100000);
    }

    buff = 0x8000;

    for(int i = 0; i <= 16; i++) {
        gauge1.setLeds(buff);
        buff |= 0x8000 >> i;
        usleep(100000);
    }
    
    gauge2.setBrightness(0x8);
    gauge2.oled_disp->setContrast(0xff, 241, 0x40);
    gauge2.setMaxVal(120);

    font = OLEDDisplay::loadFont("fonts/Lato_Black_Italic_24.olf");
    gauge2.oled_disp->setFont(font);
    gauge2.oled_disp->setTextAlignment(TEXT_ALIGN_CENTER);

    for(int i = 0; i <= 120; i++) {
        gauge2.setVal(i);
        gauge2.oled_disp->clear();
        gauge2.oled_disp->drawString(64, 0, to_string(i) + " mph");
        gauge2.oled_disp->display();
        usleep(30000);
    }
    
    return 0;
}

