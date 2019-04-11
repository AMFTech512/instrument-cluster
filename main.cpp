#include <iostream>
#include <unistd.h>
#include "lib/SSD1306Wire.h"
#include "lib/Gauge.h"

#define ADDRESS 0x3c

SSD1306Wire disp(ADDRESS, GEOMETRY_128_32);
Gauge gauge;

using namespace std;

int main() {

    disp.init();

    gauge.setBrightness(0);

    disp.setTextAlignment(TEXT_ALIGN_CENTER);
    uint8_t *font = OLEDDisplay::loadFont("fonts/Orbitron_Medium_16.olf");
    disp.setFont(font);
    disp.drawString(64, 0, "LS1 Powered");
    disp.display();

    usleep(1000000);

    font = OLEDDisplay::loadFont("fonts/Lato_Black_Italic_24.olf");
    disp.setFont(font);

    uint16_t buff = 1;

    for(int i = 0; i < 16; i++) {
        gauge.setLeds(buff);
        buff <<= 1;
        usleep(100000);
    }

    buff = 0x8000;

    for(int i = 0; i <= 16; i++) {
        gauge.setLeds(buff);
        buff |= 0x8000 >> i;
        usleep(100000);
    }
    
    usleep(5000000);

    gauge.setMaxVal(120);

    for(int i = 0; i <= 120; i++) {
        gauge.setVal(i);
        disp.clear();
        disp.drawString(64, 0, to_string(i) + " mph");
        disp.display();
        usleep(30000);
    }

    


    // indicator images
    // while(true) {
    //     disp.clear();
    //     disp.drawImg(0, 0, "./images/battery.data");
    //     disp.drawImg(25, 0, "./images/chk-eng.data");
    //     disp.drawImg(0, 30, "./images/hlights.data");
    //     disp.drawImg(20, 30, "./images/oil-press.data");
    //     disp.display();

    //     usleep(500000);

    //     disp.clear();
    //     disp.drawImg(0, 0, "./images/battery.data");
    //     // disp.drawImg(25, 0, "./images/chk-eng.data");
    //     disp.drawImg(0, 30, "./images/hlights.data");
    //     disp.drawImg(20, 30, "./images/oil-press.data");
    //     disp.display();

    //     usleep(500000);
    // }
    

    // battery slide animation

    // while(true) {
    //     for(int i = 0; i < 128-battery_width; i++) {
    //         disp.clear();
    //         disp.drawImg(i, 10, battery_width, battery_height, battery_bits);
    //         disp.display();
    //     }

    //     for(int i = 128-battery_width; i >= 0; i--) {
    //         disp.clear();
    //         disp.drawImg(i, 10, battery_width, battery_height, battery_bits);
    //         disp.display();
    //     }
    // }
    
    return 0;
}

