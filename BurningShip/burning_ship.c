//
// Created by paul on 10.01.18.
//
#include "burning_ship.h"

int getSteps(double x0, double y0, long maxSteps) {
    double x=0, y=0;
    double xtemp;
    for(int n=0; n<maxSteps; n++) {
        xtemp = x*x - y*y + x0;
        y = fabs(2*x*y + y0);
        x = fabs(xtemp);
        if(x*x + y*y > 4) {
            return n;
        }
    }
    return -1;
}

void getColor(int steps, uint8_t* rgb, int maxIterations) {
    if(steps == -1) {
        rgb[0] = rgb[1] = rgb[2] = 0;
    } else {
        double percentage = ((double)steps/maxIterations)*255;

        rgb[0] = (uint8_t)((int)(percentage*64)%255);
        rgb[1] = (uint8_t)((int)(percentage*128)%255);
        rgb[2] = (uint8_t)((int)(percentage*256)%255);

        /*rgb[0] = (steps*48)%255;
        rgb[1] = (steps*56)%255;
        rgb[2] = (steps*64)%255;*/

        /*rgb[0] = steps*16>255?255:steps*16;
        rgb[1] = steps*32>255?255:steps*32;
        rgb[2] = steps*64>255?255:steps*64;*/
    }
}
