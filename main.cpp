#include <iostream>
#include <unistd.h>
#include "lib/SH1106Wire.h"

#define ADDRESS 0x3c

SH1106Wire disp(ADDRESS);

using namespace std;

void goodMorn();

int main() {

    disp.init();
    disp.flipScreenVertically();

    disp.setTextAlignment(TEXT_ALIGN_CENTER);
    uint8_t *font = OLEDDisplay::loadFont("fonts/Orbitron_Medium_16.olf");
    disp.setFont(font);
    disp.drawString(64, 20, "LS1 Powered");
    disp.display();
    
    usleep(5000000);

    while(true) {
        disp.clear();
        disp.drawImg(0, 0, "./images/battery.data");
        disp.drawImg(25, 0, "./images/chk-eng.data");
        disp.drawImg(0, 30, "./images/hlights.data");
        disp.drawImg(20, 30, "./images/oil-press.data");
        disp.display();

        usleep(500000);

        disp.clear();
        disp.drawImg(0, 0, "./images/battery.data");
        // disp.drawImg(25, 0, "./images/chk-eng.data");
        disp.drawImg(0, 30, "./images/hlights.data");
        disp.drawImg(20, 30, "./images/oil-press.data");
        disp.display();

        usleep(500000);
    }
    

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


void goodMorn() {
    // disp.setFont(Calligraffitti_Regular_16);
    disp.drawString(20, 10, "Good");
    disp.drawString(20, 30, "Morning");
    disp.drawRect(0, 0, 127, 63);
}
