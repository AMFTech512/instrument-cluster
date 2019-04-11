/**
    AUTHOR: Austin Fay

    Description: 
        This program is supposed to convert font data (or any data really) that was stored in some array 
        in some header file into an actual binary file. This is because the library that Austin was 
        working on at the time of writing this program was for a small microcontroller that doesn't have
        a file system, and Austin wanted to convert the library to work with the Raspberry Pi, which has
        a file system. 

*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "Lato_black_italic.h"

using namespace std;

// have font named 'font' of type uin8_t[] in h file

// you need to initialize both of these variables
string fileName = "Lato_Black_Italic_24.olf";
int size = sizeof(font);
char *fontData= (char*) font;

int main() {

    ofstream file(fileName, ios::binary | ios::out);

    if(!file) {
        cout << "Failed to open file" << endl;
        return 1;
    }

    file.write(fontData, size);
    file.close();

    cout << "Finished writing file" << endl;

    return 0;
}