# Digital Instrument Cluster
<h3>A project by Austin Fay</h3>

## Table of contents
- [Digital Instrument Cluster](#digital-instrument-cluster)
  - [Table of contents](#table-of-contents)
  - [Overview](#overview)
  - [System Architecture](#system-architecture)
    - [Block Diagram](#block-diagram)
    - [Connections](#connections)
    - [RPi-Gauge Communication](#rpi-gauge-communication)
    - [RPi-Vehicle Communication](#rpi-vehicle-communication)
  - [Hardware](#hardware)
    - [Hardware Overview](#hardware-overview)
    - [Raspberry Pi](#raspberry-pi)
    - [Selector Circuit and I2C Bus Module](#selector-circuit-and-i2c-bus-module)
      - [Selector Circuit](#selector-circuit)
    - [CAN Bus Controller Module](#can-bus-controller-module)
    - [Gauges](#gauges)
  - [Software](#software)
    - [Software Stack Overview](#software-stack-overview)
    - [Demo Program](#demo-program)
    - [Gauge Class](#gauge-class)
    - [OLED (SSD1306) Class](#oled-ssd1306-class)
    - [I2C Class](#i2c-class)
    - [Selector Class](#selector-class)
    - [GPIO Class](#gpio-class)
    - [The Linux Operating System](#the-linux-operating-system)
  - [Future Changes/Upgrades](#future-changesupgrades)

## Overview
The goal of this project is to build a custom digital instrument cluster for any vehicle that both looks cool and extends the functionality of a conventional instrument cluster. These goals are met in the following ways:
1. The system uses the universal OBDII port (featuring a CAN Bus) to read data from the vehicle.
2. The gauges are general purpose. 
3. Each gauge has an OLED screen to display any type of information.
4. Each gauge has an array of LEDs for aesthetic.

This document details the technical aspects of the current prototype. 

## System Architecture

### Block Diagram
![Block Diagram](./Block-diagram.svg)

### Connections
The Raspberry Pi uses various pins to commicate with other devices. The connections to these pins are shown in the following diagram:
![Connections](./raspberry-pi-pinout-cluster.png)
The following [System Architecture](#system-architecture) sections reference this diagram.

### RPi-Gauge Communication
This system uses the I2C protocol to communicate between the Raspberry Pi and the individual gauges. I2C is a two-wire communication protocol that facilitates the communication between one master and multiple slave devices. Each slave device has a 1-byte address, allowing the master to select a single device to communicate with. From the above diagram, the pins with the blue loop connect to the I2C Bus. Each gauge has two I2C devices: the OLED display and the LED controller. Unfortunately, all OLEDs and all LED controllers have the same I2C 1-byte address. This means that with normal I2C communication, every gauge would display the same information. To fix this, there is a second level of selection hardware implemented. The three pins with the green loop connect to the [Selector Circuit](#selector-circuit). With 3 bits (one pin for each bit), the Raspberry Pi can select up to 8 unique gauges to communicate with. With this hardware setup, the following process must be used for the Raspberry Pi to communicate with a gauge:
1. The Raspberry Pi sets the gauge selector bits (I0, I1, and I2) to select the gauge that it wants to communicate with. The selector circuit will disconnect all gauges from the I2C bus except for the desired gauge.
2. The Raspberry Pi initiates communication with the OLED module on the gauge by using its 1-byte I2C address.
3. The Raspberry Pi initiates communication with the LED controller by using its 1-byte I2C address.
4. The Raspberry Pi can then set the gauge selector bits to communicate with a different gauge.

### RPi-Vehicle Communication
The Raspberry Pi communicates with the vehicle by reading data from the vehicle's CAN bus. The Raspberry Pi cannot connect to the CAN bus directly, so it uses a [CAN Bus Controller Module](#can-bus-controller-module). This module connects to the Raspberry Pi using the SPI communication protocol. The pins with the pink loop are the SPI pins that connect to the [CAN Bus Controller Module](#can-bus-controller-module). This communication allows the Raspberry Pi to learn information such as speed, RPM, and gas level.

## Hardware

### Hardware Overview
The instrument cluster has the following major hardware components:
- Raspberry Pi - Responsible for reading data from OBDII port and updating information on the gauges.
- CAN Bus controller module - Facilitates communication between the Raspberry Pi and the vehicle.
- I2C Bus and Selector Circuit - Facilitates communication between the Rapsberry Pi and the individual gauges.
- Gauges - Display information to the user.

### Raspberry Pi
To control the whole system, the Raspberry Pi Zero was chosen. The Raspberry Pi Zero is the choice for the project for a few reasons:
1. It's primary job is communication and information processing. The 1GHz BCM2835 is fast and does not feature unnecessary microcontroller functionality.
2. It features 512 MB of RAM, more than enough to store multiple 1k display buffers and other information.
3. It features built in I2C and SPI controllers.
4. It features an SD Card slot, allowing for long-term storage of historical diagnostics data.
5. It runs Linux, allowing it to execute a broad set of software.
6. It lacks wireless capabilities, preventing it from being remotely hacked.

Here is a picture of the Raspberry Pi Zero:

<img src="./IMG_1560.jpg" style="width: 300px">

### Selector Circuit and I2C Bus Module
This module is responsible for facilitating the communication between the Raspberry Pi and each of the gauges. The module has two internal wires that act as the I2C Bus. The board also contains the selector circuit. Here is a picture of the [Selector Circuit](#selector-circuit) and I2C Bus module:

<img src="./IMG_1559.jpg" style="width: 300px">

#### Selector Circuit
The selector circuit works by taking a 3-bit input (I0-I2) and outputting a high voltage on one of eight pins (O0-O7). These output pins connect directly to the chip-select (CS) pin on each gauge.

![Selector Circuit Schematic](selector-circuit-schematic.jpg)

### CAN Bus Controller Module
This module is responsible for facilitating communication betewen the Raspberry Pi and the vehicle's CAN Bus. This is what allows the Raspberry Pi to determine information such as speed, RPM, and gas level. Here is a picture of the module:

![CAN Controller Module](./MCP2515_CAN_Bus_Breakout_Board.jpg)

### Gauges
The gauges are responsible for displaying information to the user. Here is a picture of a gauge:

<img src="./IMG_1565.jpg" style="width: 300px">

Each gauge has two major components:
1. OLED display
2. LED array

The OLED display is controlled by an in-glass SSD1306 chip. The LED array is controlled by the HT16K33 multiplexing chip. Both of these controller chips can communicate with the Raspberry Pi using the I2C protocol. Additionally, each gauge features a chip-select (CS) pin that allows the Raspberry Pi to select the gauge for exclusive communication (preventing it from sending identical information to multiple gauges). These gauges are custom made with the following schematic:

![Gauge Schematic](./Schematic_Digital-Gauge-Small_Sheet-1_20191230120144.png)

## Software

### Software Stack Overview
Since I have only been able to build a prototype so far, the Raspberry Pi simply runs a demo program to display dummy data on the gauges. The current software stack that I have built is represented with the following diagram, with each bit of code relying on the software underneath it:

![Software Stack](./Software-stack.svg)

**DISCLAIMER:** The following sections contain unfinished code. I realize that some of the code contains bad practices and may be difficult to read. This code will be refactored and cleaned up in the future. 

### Demo Program
The demo program is responsible for generating visuals and animations that appear on the gauges. Once I implement the part of the system that reads data from the vehicles CAN bus, this layer of software will be responsible for taking that data and generating graphics to display to the user. 

The demo program runs through the following animations:
1. Shows "DISPLAY ANYTHING" with a fun LED animation
2. Imitates a car accelerating
3. Imitates a car losing gas and displaying battery voltage

Here is the code for the demo program:

main.cpp
```C++
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
```

### Gauge Class
This class makes it easy for the upper levels of software to display information on the gauges by providing helper functions. 

`Gauge()` - The Gauge class constructor. This is responsible for initializing the gauges.

`void setLeds(uint16_t disp)` - Sets LED array on the gauge. There are 16 LEDs on each gauge, so two bytes (16 bits) are used.

`void setVal(uint32_t val)` - To avoid setting the LEDs directly, this function can be called to easily display a percentage. The LEDs act as a progress bar in this case, and `setMaxVal()` can be called to set the theoretical maximum value.

`void setMaxVal(uint32_t val)` - Sets a theoretical maximum value to be used in calculating percentages used by `setVal()`.

`void setNeedle(uint32_t val)` - Behaves just like `setVal()`, except it only illuminates one LED.

`void setBrightness(uint8_t val)` - Sets the brightness of the LED array (using a value between 0-255).

`void dispOn()` - Turns the LED array on.

`void dispOff()` - Turns the LED array off. 

`SSD1306Wire* getOled()` - Returns a pointer to an object that allows for easy control of the OLED display on the gauge.

Here is the code for the Gauge class:

Gauge.cpp
```C++
#include "Gauge.h"
#include <stdio.h>
#include "Selector.h"

uint8_t Gauge::num_gauges = 0;

Gauge::Gauge(uint8_t dispAddress) {

    static bool hasInitPins = false;

    if(!hasInitPins) {
        Selector::addPin(17);
        Selector::addPin(27);
        Selector::addPin(22);
        hasInitPins = true;
    }

    this->gauge_id = num_gauges;
    Gauge::num_gauges++;

    initCmds();

    Selector::select(this->gauge_id);
    this->oled_disp = new SSD1306Wire(dispAddress, GEOMETRY_128_32);
    this->oled_disp->init();
}

void Gauge::initCmds() {
    Wire.begin(this->led_addr);

    sendCommand(SYS_OSC_ON);
    sendCommand(ROW_DRV_OUTPUT);
    setBrightness(this->brightness);
    setLeds(this->disp);
    dispOn();
}

void Gauge::setLeds(uint16_t mDisp) {
    Selector::select(this->gauge_id);
    this->disp = mDisp;

    uint8_t right = mDisp & 0xff;
    uint8_t left = mDisp >> 8;

    int error = 0;

    error += Wire.beginTransmission(this->led_addr);
    Wire.write((uint8_t) 0x00);
    Wire.write(right);
    Wire.write((uint8_t) 0x00);
    Wire.write(left);
    error += Wire.endTransmission();

    if(error != 0){
        printf("[Gauge::setLeds(uint16_t)] There was an error, recalling self");
        initCmds();
        setLeds(mDisp);
    }
        
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
```

### OLED (SSD1306) Class
This class makes it easy for upper levels of software to display information on the OLED screens of the gauges. 

This code was taken from ThingPulse's open source library. [Click to view ThingPulse's project on GitHub.](https://github.com/ThingPulse/esp8266-oled-ssd1306)

### I2C Class
This class makes it easy for upper levels of software to communicate with devices connected to the I2C Bus. 

This code was taken from Arduino's Wire library and modified to run on the Raspberry Pi. Instead of setting various registers in the Arduino microcontroller, the new code simply writes streams of bytes to the `/dev/i2c-1` device mapped in the Linux filesystem. [Click to view Arduino's original Wire library on GitHub.](https://github.com/esp8266/Arduino/tree/master/libraries/Wire)

Here is an example of how I had to modify the code.

The original code had to map two pins on the Arduino as the SDA and SCL pins:

(Arduino) Wire.cpp
```C++
void TwoWire::begin(int sda, int scl, uint8_t address)
{
    default_sda_pin = sda;
    default_scl_pin = scl;
    twi_setAddress(address);
    twi_init(sda, scl);
    twi_attachSlaveTxEvent(onRequestService);
    twi_attachSlaveRxEvent(onReceiveService);
    flush();
}
```

The modified code doesn't have to worry about that. The Linux I2C driver takes care of that for us. Thus, we can simply write to the device file provided by Linux:

(Raspberry Pi) Wire.cpp
```C++
char *TwoWire::fileName = "/dev/i2c-1";
int TwoWire::fileDesc = 0;

int TwoWire::begin(void)
{
  // Open port for reading and writing
  if ((fileDesc = open(fileName, O_RDWR)) < 0) {					    
        // in this case, we failed to open the I2C device file
		printf("Failed to open i2c port [from TwoWire::begin(void)]\n");
		return -1;
	}
  return 0;
}
```

### Selector Class
The selector class makes it easy for upper levels of software to select a gauge to communicate with. It does this by controlling the three pins (I0-I2) of the [Selector Circuit](#selector-circuit). The selector class relies on the [GPIO class](#gpio-class) to function.

Here is the code:

Selector.cpp
```C++
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
```

### GPIO Class
The GPIO (General Purpose Input/Output) class makes it easy for upper levels of software to set the voltages of arbitrary pins on the Raspberry Pi. 

Here is the code:

GPIO.cpp
```C++
#include <fstream>
#include <iostream>
#include "sys/stat.h"
#include <cstring>
#include "GPIO.h"

using namespace std;

void GPIO::initGPIO(uint8_t pin) {

    struct stat dir;
    string filename = "/sys/class/gpio/gpio" + to_string(pin);
    if(stat(filename.c_str(), &dir) == 0)
        return;

    ofstream expFile;
    expFile.open("/sys/class/gpio/export");
    expFile << to_string(pin);
    expFile.close();

    ofstream dirFile;
    dirFile.open("/sys/class/gpio/gpio" + to_string(pin) + "/direction");
    dirFile << "out";
    dirFile.close();

}

void GPIO::writePin(uint8_t pin, bool isOne) {

    ofstream wFile;
    wFile.open("/sys/class/gpio/gpio" + to_string(pin) + "/value");
    
    if(isOne)
        wFile << "1";
    else
        wFile << "0";

    wFile.close();
    
}
```

### The Linux Operating System
All of the software described above runs on a version of Linux made for the Raspberry Pi Zero. For this project, I compiled the Linux Kernel from source in order to avoid all of the garbage that comes with a normal Linux distribution such as Raspbian. I used [Rasperry Pi's website](https://www.raspberrypi.org/documentation/linux/kernel/building.md) to help me with this task. I also loaded [BusyBox](https://busybox.net/about.html) onto the system in order to have some basic commands and functions available to me. With this setup, the Raspberry Pi boots quickly and runs my software without interruption from other unnecessary programs.

## Future Changes/Upgrades
What is described here is only a prototype. Here are features that I still need to add for it to be functional:
1. A method of converting the vehicle's 12v to 5v for powering the whole thing.
2. A method of mounting the gauges onto the dashboard of the vehicle.
3. The addition of the CAN Bus controller module.
4. The software to read data from the CAN Bus and display it on the gauges.

Once I get a barely functional product working, I intend to add the following features:
1. Easy connection to your phone or computer for configuring the gauge layout.
2. Larger versions of the gauges (to replace the speedometer and tachometer).
3. Data recording. This could be useful for diagnostics and forensics. 
4. The addition of cellular and GPS modules for tracking stolen vehicles.

Depending on the quality of the end-product, I may decide to sell this. I'm not sure yet. There is still much work to be done. 
