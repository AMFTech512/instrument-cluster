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

uint8_t disp_buffer[1056+33];

// void render();

int main(int argc, char **argv)
{
	printf("**** CMPS03 test program ****\n");
	
	int fd;														// File descrition
	// For older raspberry pi modules use "/dev/i2c-0" instead of "/dev/i2c-1" for the i2c port
	char *fileName = "/dev/i2c-1";								// Name of the port we will be using
	int  address = 0x3c;										// Address of CMPS03 shifted right one bit
	uint8_t cmds[] = {
		0x00, // semd command stream
		0xae, // turn display off
		0xa1, // segment remapping, 0xa0 for normal image, 0xa1 for a backwards image
		0xc0, // set display right-side up (0xc8 for up-side down, 0xc0 for right-side up; common output scan direction)
		0xa8,
		0x3f, // set multiplex ratio
		0xd5,
		0xf0, // set oscillator frequncy
		0xdb,
		0x35, // VCOM deselect level set
		0x81,
		0x80, // set contrast (brightness) to like 50%
		0x30, // set pump voltage value
		0xad,
		0x8b, // use the internal voltage supply for the display (DC-DC off/on)
		//0x40, // set display start line to the first line
		0xaf, // turn display on
		0xa5 // test mode
	};										// Buffer for data being read/ written on the i2c bus


	
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
		printf("Error writing to i2c slave\n");
		exit(1);
	}

    // while(1) {
    //     for(int i = 0; i < sizeof(disp_buffer); i++) {
    //         if(i % 33 == 0) {
    //             disp_buffer[i] = 0x40;
    //             continue;
    //         }
    //         disp_buffer[i] = 0xff;
    //     }
    //     render(fd);
    //     for(int i = 0; i < sizeof(disp_buffer); i++) {
    //         if(i % 33 == 0) {
    //             disp_buffer[i] = 0x40;
    //             continue;
    //         }
    //         disp_buffer[i] = 0x00;
    //     }
    //     render(fd);
    // }

    
	
    
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


// void render(int fd) {

//     for(uint8_t page = 0; page < 8; page++) {
//         uint8_t pg_col_set[] = {0x00, (0xb0 + page), 0x00, 0x10};
//         write(fd, pg_col_set, sizeof(pg_col_set));
        
//         for(int g = 0; g < 4; g++) {
//             write(fd, (disp_buffer + (page * 132 + g * 33)), 33);
//         }
//         write(fd, (disp_buffer + (page + 1) * 128), 4);
//     }
    

// }
