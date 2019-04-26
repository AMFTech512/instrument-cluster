#include <iostream>
#include "rpm.h"

using namespace std;

int main() {

    int len = sizeof(rpms)/sizeof(int);

    double inc = (double) 130/len;
    double speed = 0.0f;
    // cout << inc << endl;
    for(int t = 0; t < len; t++) {

        cout << ((uint32_t) speed) << endl;

        speed += inc;
        // usleep(1000);
    }

    return 0;
}