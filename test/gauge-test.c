// CMPS03 example c code for the Raspberry pi
// 
// Reads the bearing from the CMPS03 and displays
// it on the screen.
//
// By James Henderson, 2016

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef unsigned char uint8_t;

int main(int argc, char **argv)
{
	printf("**** CMPS03 test program ****\n");
	
	int fd;														// File descrition
	// For older raspberry pi modules use "/dev/i2c-0" instead of "/dev/i2c-1" for the i2c port
	char *fileName = "/dev/i2c-1";								// Name of the port we will be using
	int  address = 0x70;										// Address of CMPS03 shifted right one bit
	uint8_t cmds[] = {
		0x21
	};	
	
	uint8_t bright[] = {
		0xef
	};
	
	uint8_t on[] = {
		0x81
	};											// Buffer for data being read/ written on the i2c bus

	uint8_t data[] = {
		0x00,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
		0xff,
	};
	
	if ((fd = open(fileName, O_RDWR)) < 0) {					// Open port for reading and writing
		printf("Failed to open i2c port\n");
		exit(1);
	}
	
	if (ioctl(fd, I2C_SLAVE, address) < 0) {					// Set the port options and set the address of the device we wish to speak to
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}
	
    size_t buff_len = sizeof(cmds);
	if ((write(fd, cmds, buff_len)) != buff_len) {								// Send register we want to read from	
		printf("Error writing to i2c slave 1st\n");
		exit(1);
	}
   
	buff_len = sizeof(bright);
	if ((write(fd, bright, buff_len)) != buff_len) {								// Send register we want to read from	
		printf("Error writing to i2c slave 2nd\n");
		exit(1);
	}	

	buff_len = sizeof(on);
	if ((write(fd, on, buff_len)) != buff_len) {								// Send register we want to read from	
		printf("Error writing to i2c slave 3rd\n");
		exit(1);
	}	

	buff_len = sizeof(data);
	if ((write(fd, data, buff_len)) != buff_len) {								// Send register we want to read from	
		printf("Error writing to i2c slave 4th\n");
		exit(1);
	}	
    
	// if (read(fd, buf, 4) != 4) {								// Read back data into buf[]
	// 	printf("Unable to read from slave\n");
	// 	exit(1);
	// }
	// else {
	// 	unsigned char highByte = buf[2];
	// 	unsigned char lowByte = buf[3];
	// 	unsigned int result = (highByte <<8) + lowByte;			// Calculate bearing as a word value
	// 	printf("Software v: %u \n",buf[0]);
	// 	printf("Bearing as byte: %u \n",buf[1]);
	// 	printf("Bearing as decimal: %u.%u\n",result/10, result%10); // display bearing with decimal place
	// }
	
	return 0;
}