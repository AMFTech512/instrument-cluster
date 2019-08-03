#include <iostream>
#include <unistd.h>
#include "lib/SSD1306Wire.h"
#include "lib/Gauge.h"
#include "lib/GPIO.h"
#include "rpm.h"

using namespace std;

Gauge *gauge1;
Gauge *gauge2;

// void bootAnim();
void intro();
void batGas();
void rpm();

int main() {

    gauge1 = new Gauge();
    gauge2 = new Gauge();

    gauge1->setBrightness(0x8);
    gauge2->setBrightness(0x8);

    gauge1->getOled()->setContrast(0xff, 241, 0x40);
    gauge2->getOled()->setContrast(0xff, 241, 0x40);
    
    uint8_t *font = OLEDDisplay::loadFont("fonts/Lato_Black_Italic_24.olf");
    gauge2->getOled()->setFont(font);
    gauge2->getOled()->setTextAlignment(TEXT_ALIGN_CENTER);

    gauge1->getOled()->setFont(font);
    gauge1->getOled()->setTextAlignment(TEXT_ALIGN_CENTER);

    while(true) {
        intro();
        usleep(1000000);
        rpm();
        usleep(1000000);
        batGas();
        usleep(1000000);
    }
    
    return 0;
}

void intro() {

    gauge1->getOled()->clear();
    gauge2->getOled()->clear();

    gauge1->getOled()->drawString(64, 0, "DISPLAY");
    gauge2->getOled()->drawString(64, 0, "ANYTHING");

    gauge1->getOled()->display();
    gauge2->getOled()->display();

    uint16_t leds = 0x8001;

    for(int i = 0; i < 7; i++) {

        gauge1->setLeds(leds);
        gauge2->setLeds(leds);

        leds = ((leds >> 1) & 0xff00) | ((leds << 1) & 0xff);

        usleep(100000);
    }

    for(int i = 0; i < 9; i++) {

        gauge1->setLeds(leds);
        gauge2->setLeds(leds);

        leds = ((0x0100 << i) | (leds & 0xff00)) | ((0x80 >> i) | (leds & 0xff));

        usleep(100000);
    }

    for(int i = 0; i < 18; i+=2) {

        gauge1->setLeds(leds);
        gauge2->setLeds(leds);

        leds ^= 0x8000 >> i;

        usleep(100000);
    }

    for(int i = 0; i < 30; i++) {

        gauge1->setLeds(leds);
        gauge2->setLeds(leds);

        leds = ~leds;

        usleep(100000);
    }
}

void rpm() {

    gauge1->setMaxVal(130);
    gauge2->setMaxVal(10000);

    int len = sizeof(rpms)/sizeof(int);

    double inc = (double) 110/len;
    double speed = 0.0f;

    for(int t = 0; t < len; t++) {

        gauge1->setVal((uint32_t) speed);
        gauge1->getOled()->clear();
        gauge1->getOled()->drawString(64, 0, to_string((uint32_t) speed) + " MPH");
        gauge1->getOled()->display();

        speed += inc;

        gauge2->setVal(rpms[t]);
        gauge2->getOled()->clear();
        gauge2->getOled()->drawString(64, 0, to_string(rpms[t]) + " RPM");
        gauge2->getOled()->display();

        // usleep(1000);
    }
}

void batGas() {

    gauge1->getOled()->clear();
    gauge1->getOled()->drawString(80, 0, "13.2v");
    gauge1->getOled()->drawImg(10, 10, "./images/battery.data");
    gauge1->getOled()->display();
    gauge1->setLeds(0x0020);

    gauge2->setMaxVal(100);

    for(int i = 87; i >=0; i-=5) {

        gauge2->getOled()->clear();
        gauge2->getOled()->drawString(80, 0, to_string(i) + "%");
        gauge2->getOled()->drawImg(20, 0, "./images/gas.data");
        gauge2->getOled()->display();

        gauge2->setNeedle(i);

        usleep(700000);
    }
    
}

// void bootAnim() {
//     while(true) {
        
//         gauge1->getOled()->setTextAlignment(TEXT_ALIGN_CENTER);
//         uint8_t *font = OLEDDisplay::loadFont("fonts/Orbitron_Medium_16.olf");
//         gauge1->getOled()->setContrast(0xff, 241, 0x40);
//         gauge1->getOled()->setFont(font);
//         gauge1->getOled()->drawString(64, 0, "LS1 Powered");
//         gauge1->getOled()->display();

//         usleep(1000000);

//         uint16_t buff = 1;

//         for(int i = 0; i < 16; i++) {
//             gauge1->setLeds(buff);
//             buff <<= 1;
//             usleep(100000);
//         }

//         buff = 0x8000;

//         for(int i = 0; i <= 16; i++) {
//             gauge1->setLeds(buff);
//             buff |= 0x8000 >> i;
//             usleep(100000);
//         }
        
//         gauge2->getOled()->setContrast(0xff, 241, 0x40);
//         gauge2->setMaxVal(120);

//         font = OLEDDisplay::loadFont("fonts/Lato_Black_Italic_24.olf");
//         gauge2->getOled()->setFont(font);
//         gauge2->getOled()->setTextAlignment(TEXT_ALIGN_CENTER);

//         for(int i = 0; i <= 120; i++) {
//             gauge2->setVal(i);
//             gauge2->getOled()->clear();
//             gauge2->getOled()->drawString(64, 0, to_string(i) + " mph");
//             gauge2->getOled()->display();
//             usleep(30000);
//         }
//     }
// }