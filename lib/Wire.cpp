/*
  TwoWire.cpp - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2017 by Chuck Todd (ctodd@cableone.net) to correct Unconfigured Slave Mode reboot
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  #include "cfile.h"
}

#include "Wire.h"
#include <iostream>

#include <stdio.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

char *TwoWire::fileName = "/dev/i2c-1";
int TwoWire::fileDesc = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
}

// Public Methods //////////////////////////////////////////////////////////////

int TwoWire::begin(void)
{
  
  if ((fileDesc = open(fileName, O_RDWR)) < 0) {					// Open port for reading and writing
		printf("Failed to open i2c port [from TwoWire::begin(void)]\n");
		return -1;
	}
  return 0;
  // printf("File descriptor: %d\n", fileDesc);
}

int TwoWire::begin(uint8_t address)
{
  int retCode = begin();
  this->txAddress = address;
  printf("Inside of TwoWire::begin(uint8_t)\n");
  while (ioctl(fileDesc, I2C_SLAVE, address) < 0); 
  // {					// Set the port options and set the address of the device we wish to speak to
	// 	printf("Unable to get bus access to talk to slave [from TwoWire::begin(uint8_t)]\n");
	// 	exit(1);
	// }
  return retCode;
}

int TwoWire::begin(uint8_t sda, uint8_t scl)
{
  return begin();
}

int TwoWire::begin(int address)
{
  int retCode = begin();
  this->txAddress = address;
  printf("Inside of TwoWire::begin(int)\n");
  while (ioctl(fileDesc, I2C_SLAVE, address) < 0); 
  // {					// Set the port options and set the address of the device we wish to speak to
	// 	printf("Unable to get bus access to talk to slave [from TwoWire::begin(uint8_t)]\n");
	// 	exit(1);
	// }
  return retCode;
}

int TwoWire::end(void)
{
  return 0;
}

void TwoWire::setClock(uint32_t clock)
{
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
  return 0;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  return 0;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
  return 0;
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
  return 0;
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
  return 0;
}

int TwoWire::beginTransmission(uint8_t address)
{
  this->txAddress = address;
  this->txBufferLength = 0;
  this->txBuffer = {};
  if (ioctl(fileDesc, I2C_SLAVE, address) < 0) {					// Set the port options and set the address of the device we wish to speak to
		printf("Unable to get bus access to talk to slave [from TwoWire::beginTransmission(uint8_t)\n");
		return -1;
	}
  return 0;
}

int TwoWire::beginTransmission(int address)
{
  this->txAddress = address;
  this->txBufferLength = 0;
  this->txBuffer = {};
  if (ioctl(fileDesc, I2C_SLAVE, address) < 0) {					// Set the port options and set the address of the device we wish to speak to
		printf("Unable to get bus access to talk to slave [from TwoWire::beginTransmission(int)]\n");
		return -1;
	}
  return 0;
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to 
//	perform a repeated start. 
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
  endTransmission();
  return 0;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{

  // std::cout << "Printing buffer..." << std::endl;
  // for (int i = 0; i < this->txBufferLength; i++){
  //   printf("%x ", this->txBuffer.at(i));
  // }

  // std::cout << std::endl;

  if ((writeFile(fileDesc, &(this->txBuffer[0]), this->txBufferLength)) != this->txBufferLength) {								// Send register we want to read from	
		printf("Error writing to i2c slave [from TwoWire::endTransmission(void)]\n");
		return -1;
	}
  this->txBufferLength = 0;
  this->txBuffer.resize(0);
  return 0;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
  this->txBuffer.resize(this->txBufferLength + 1);
  this->txBuffer.at(this->txBufferLength) = data;
  this->txBufferLength++;
  return txBufferLength;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  this->txBuffer.resize(this->txBufferLength + quantity);
  
  for (int byte = 0; byte < quantity; byte++) {
    this->txBuffer.at(byte) = *(data + byte);
  }
  this->txBufferLength += quantity;
  return 0;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
  return 0;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
  return 0;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
  return 0;
}

void TwoWire::flush(void)
{
  // XXX: to be implemented.
}

// behind the scenes function that is called when data is received
void TwoWire::onReceiveService(uint8_t* inBytes, int numBytes)
{
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{

}

// sets function called on slave write
void TwoWire::onReceive( void (*function)(int) )
{
}

// sets function called on slave read
void TwoWire::onRequest( void (*function)(void) )
{
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();

