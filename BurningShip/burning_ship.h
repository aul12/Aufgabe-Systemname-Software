//
// Created by paul on 10.01.18.
//

#ifndef BURNINGSHIP_BURNING_SHIP_H
#define BURNINGSHIP_BURNING_SHIP_H

#include <stdint.h>
#include <complex.h>
#include <math.h>

void getColor(int steps, uint8_t* rgb, int maxIterations);
int getSteps(double x0, double y0, long maxSteps);

#endif //BURNINGSHIP_BURNING_SHIP_H
